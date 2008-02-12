// sqlite3_metadata_plugin.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <iostream>
#include <stdexcept>

#if defined(_WIN32) && !defined(USE_LIBXML2)
#	import <msxml6.dll>
#	pragma warning( push )
#	pragma warning( disable : 4290 )
#	include <atlbase.h>
#	pragma warning( pop )
#else
#	include <libxml/tree.h>
#	include <libxml/parser.h>
#endif

#include <openassetlib/plugins/sqlite/sqlite3_metadata_plugin.hpp>
#include <openassetlib/plugins/sqlite/database.hpp>

namespace olib { namespace openassetlib { namespace plugins { namespace sqlite3 {

	namespace
	{
		const char* XML_ELEMENT_NAME_ASSET	= "asset";
		const char* XML_ATTRIBUTE_NAME_GUID	= "guid";

#if defined(_WIN32) && !defined(USE_LIBXML2) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		al::set_ptr handleRecoverCmd(database* pdb, MSXML2::IXMLDOMElementPtr pElement)
		{
			// Process asset child elements
			MSXML2::IXMLDOMNodeListPtr pNodeList = pElement->getElementsByTagName(_bstr_t(XML_ELEMENT_NAME_ASSET));
			if(pNodeList == NULL)
				return al::set_ptr();

			al::set_ptr set = al::set::create();
			for(unsigned char idx = 0; 
				idx < pNodeList->length; 
				idx++)
			{
				MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = pNodeList->item[idx]->attributes;
				if(pNodeMap == NULL)
					return al::set_ptr();

				// read guid attribute
				MSXML2::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(_bstr_t(XML_ATTRIBUTE_NAME_GUID));
				if(pNode == NULL)
					return al::set_ptr();

				al::asset_ptr asset = pdb->restore((const char*)pNode->text);
				if(asset != al::asset_ptr())
				{
					set->push_back(asset);
					asset->clear_dirty_flag();
				}
			}

			return set;
		}

#else /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		al::set_ptr handleRecoverCmd(database* pdb, xmlNodePtr pNode)
		{
			pNode = pNode->children;
			if(pNode == NULL)
				return al::set_ptr();

			al::set_ptr set = al::set::create();
			for(xmlNodePtr cur_node = pNode; cur_node; cur_node = cur_node->next)
			{
				if(		(cur_node->type == XML_ELEMENT_NODE)
					&&	(xmlStrcmp(cur_node->name, xmlCharStrdup(XML_ELEMENT_NAME_ASSET)) == 0) )
				{
					xmlChar* guid = xmlGetProp(cur_node, xmlCharStrdup(XML_ATTRIBUTE_NAME_GUID));
					if(guid != NULL)
					{
						al::asset_ptr asset = pdb->restore((const char*)guid);
						if(asset != al::asset_ptr())
						{
							set->push_back(asset);
							asset->clear_dirty_flag();
						}

						xmlFree(guid);
					}
				}
			}

			if(set->empty())
				return al::set_ptr();

			return set;
		}

#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}

sqlite3_metadata_plugin::sqlite3_metadata_plugin(const char* config)
{
#if defined(_DEBUG)
	fprintf(stdout, "sqlite3_metadata_plugin config = %s\n", config);
#endif

#if defined(_WIN32) && !defined(USE_LIBXML2)
	::CoInitialize(0);
#endif

	pdb_ = new database();
	if(!pdb_->open(fs::path(config, fs::native)))
	{
		delete pdb_;
		pdb_ = NULL;
	}
}

sqlite3_metadata_plugin::~sqlite3_metadata_plugin(void)
{
	if(pdb_)
	{
		pdb_->close();
		delete pdb_;
	}

#if defined(_WIN32) && !defined(USE_LIBXML2)
	::CoUninitialize();
#endif
}

al::set_ptr sqlite3_metadata_plugin::store(const al::set_ptr& set)
{
	if(set->empty())
		return al::set_ptr();

	al::set_ptr failed_set = al::set::create();
	al::set::const_iterator cit = set->begin();
	do
	{
		if((*cit)->is_dirty())
		{
			if(!pdb_->store(*cit))
				failed_set->push_back(*cit);
			else
				(*cit)->clear_dirty_flag();
		}

	} while(++cit != set->end());

	if(failed_set->empty())
		return al::set_ptr();

	return failed_set;
}

al::set_ptr sqlite3_metadata_plugin::load(const opl::string& query_xml)
{
	const char* XML_ROOT_ELEMENT_NAME	= "database_query";
	const char* XML_ATTRIBUTE_NAME_CMD	= "cmd";
	const char* CMD_NAME_RECOVER		= "recover";

#if defined(_WIN32) && !defined(USE_LIBXML2) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	try
	{
		// Create XML DOM and load up with string passed in
		MSXML2::IXMLDOMDocument2Ptr pXMLDoc(__uuidof(MSXML2::DOMDocument60));
		pXMLDoc->validateOnParse = VARIANT_FALSE;
		if(pXMLDoc->loadXML(_bstr_t(query_xml.c_str())) == VARIANT_FALSE)
			return al::set_ptr();

		// Get the root element
		MSXML2::IXMLDOMElementPtr pElement = pXMLDoc->documentElement;
		if(pElement == NULL)
			return al::set_ptr();

		// Check root elements name
		if(pElement->nodeName != _bstr_t(XML_ROOT_ELEMENT_NAME))
			return al::set_ptr();

		// Get attribute node map
		MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = pElement->attributes;
		if(pNodeMap == NULL)
			return al::set_ptr();

		// Read cmd attribute
		MSXML2::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(_bstr_t(XML_ATTRIBUTE_NAME_CMD));
		if(pNode == NULL)
			return al::set_ptr();

		opl::string cmd(pNode->text);
		if(cmd.compare(CMD_NAME_RECOVER) != 0)
			return al::set_ptr();

		return handleRecoverCmd(pdb_, pElement);
	}
	catch(_com_error& e)
	{
		std::cerr << "COM Exception: " << CT2CA(e.ErrorMessage()) << std::endl;
		return al::set_ptr();
	}
	catch(const std::runtime_error& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
		return al::set_ptr();
	}

#else /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	xmlDocPtr	pXMLDoc	= NULL;
	xmlNodePtr	pNode	= NULL;
	try
	{
		// Create XML DOM and load up with string passed in
		pXMLDoc = xmlParseMemory(query_xml.c_str(), query_xml.length());
		if(pXMLDoc == NULL)
			return al::set_ptr();

		// Get the root element
		pNode = xmlDocGetRootElement(pXMLDoc);
		if(		(pNode == NULL)
			||	(pNode->type != XML_ELEMENT_NODE) )
			return al::set_ptr();

		// Check root elements name
		if(xmlStrcmp(pNode->name, xmlCharStrdup(XML_ROOT_ELEMENT_NAME)) != 0)
			return al::set_ptr();

		// Get attribute
		xmlChar* pcmd = xmlGetProp(pNode, xmlCharStrdup(XML_ATTRIBUTE_NAME_CMD));
		if(pcmd == NULL)
			return al::set_ptr();

		if(xmlStrcmp(xmlCharStrdup(CMD_NAME_RECOVER), pcmd) == 0)
		{
			xmlFree(pcmd);
			al::set_ptr set = handleRecoverCmd(pdb_, pNode);

			xmlFreeDoc(pXMLDoc);
			xmlCleanupParser();
			return set;
		}

		return al::set_ptr();
	}
	catch(const std::runtime_error& e)
	{
		xmlFreeDoc(pXMLDoc);
		xmlCleanupParser();

		std::cerr << "Exception: " << e.what() << std::endl;
		return al::set_ptr();
	}

#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

} } } }
