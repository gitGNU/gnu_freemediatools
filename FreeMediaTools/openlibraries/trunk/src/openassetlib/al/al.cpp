// al.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/regex.hpp>
#include <openassetlib/al/al.hpp>
#include <openassetlib/al/set.hpp>
#include <openassetlib/al/base64_codec.hpp>

#if defined( WIN32 ) && !defined( USE_LIBXML2 )
#import <msxml6.dll>
#pragma warning( disable : 4290 )
#include <atlbase.h>
#else
#include <libxml/tree.h>
#include <libxml/parser.h>
#endif

namespace fs = boost::filesystem;

namespace olib { namespace openassetlib {

	const char* SET_XML_ROOT_ELEMENT_NAME	= "jahreel";
	const char* SET_XML_NAME_ATTRIBUTE_NAME	= "name";
	const char* SET_XML_ASSET_ELEMENT_NAME	= "asset";

	AL_DECLSPEC bool init(guid_generator_ptr guid_gen)
	{
		if(guid_gen == guid_generator_ptr())
		{
			std::cerr << "ERROR: Unable to initialize openassetlib. Invalid GUID generator" << std::endl;
			return false;
		}

		al::guid_factory::initialize(guid_gen);

		opl::init( "" );

#if defined(_WIN32) && !defined(USE_LIBXML2)
		::CoInitialize(0);
#endif

		return true;
	}

	AL_DECLSPEC bool uninit()
	{
		opl::uninit( );

		al::guid_factory::uninitialize();

#if defined(_WIN32) && !defined(USE_LIBXML2)
		::CoUninitialize();
#endif

		return true;
	}

	AL_DECLSPEC const al::guid_ptr generate_guid()
	{
		return al::guid_factory::instance().create_guid();
	}

	  
	  al::query_traits::query_traits( const opl::wstring& filename )
	    : filename_( filename )
	  { }
	  
	  opl::wstring al::query_traits::libname( ) const
	  { return L""; }

  opl::wstring al::query_traits::to_match( ) const
  { return filename_; }

  opl::wstring al::query_traits::type( ) const
  { return L"input"; }
  
  int al::query_traits::merit( ) const
  { return 0; }

			
			
  al::oil_query_traits::oil_query_traits( const opl::wstring& filename )
    : filename_( filename )
  { }
	  
  opl::wstring al::oil_query_traits::libname( ) const
	  { return L"openimagelib"; }

  opl::wstring al::oil_query_traits::to_match( ) const
	  { return filename_; }

  opl::wstring al::oil_query_traits::type( ) const
	  { return L"input"; }
  
  int al::oil_query_traits::merit( ) const
				{ return 0; }


	  
  al::oml_query_traits::oml_query_traits( const opl::wstring& filename )
	    : filename_( filename )
				{ }

  opl::wstring al::oml_query_traits::libname( ) const
				{ return L"openmedialib"; }
	  
  opl::wstring al::oml_query_traits::to_match( ) const
  { return filename_; }

  opl::wstring al::oml_query_traits::type( ) const
  { return L"input"; }
  
  int al::oml_query_traits::merit( ) const
				{ return 0; }
  

	  
  al::ool_query_traits::ool_query_traits( const opl::wstring& filename )
    : filename_( filename )
  { }
	  
  opl::wstring al::ool_query_traits::libname( ) const
	  { return L"openobjectlib"; }
  
  opl::wstring al::ool_query_traits::to_match( ) const
  { return filename_; }
	  
  opl::wstring al::ool_query_traits::type( ) const
  { return L"input"; }
  
  int al::ool_query_traits::merit( ) const
  { return 0; }
	  


  al::set_search_predicate::set_search_predicate(const predicate& pred)
    :	predicate_(pred)
  {
    result_set_ = set::create();
  };
  
  al::set_search_predicate::~set_search_predicate()
  {
  };
	  
  void al::set_search_predicate::operator()(const asset_ptr& asset)
  {
    if(predicate_(asset))
      result_set_->push_back(asset);
  }
	  
  al::set_ptr al::set_search_predicate::result()
  {
    return result_set_;
  };
  
  
  namespace al
  {
  opl::string get_filepath(const asset_ptr& asset)
  {
    asset::const_iterator location_cit = asset->find("location");
    if(location_cit == asset->end())
      return opl::string( );
    
    //asset::const_iterator filename_cit = asset->find("filename");
    //if(filename_cit == asset->end())
    //	return opl::string( );
    
    return location_cit->second;
    /*
      try
      {
      asset::const_iterator location_cit = asset->find("location");
					if(location_cit == asset->end())
						return fs::path();

					asset::const_iterator filename_cit = asset->find("filename");
					if(filename_cit == asset->end())
						return fs::path();

					fs::path filepath(location_cit->second.c_str());
					filepath /= fs::path(filename_cit->second.c_str());
					if(!fs::exists(filepath))
						return fs::path();
					if(fs::is_directory(filepath))
						return fs::path();

					return filepath;
				}
				catch(const fs::filesystem_error& e)
				{
					fprintf(stderr, "filesystem_error: %s\n", e.what());
					return fs::path();
				}
*/
			}
		//}

  AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse)
		{
			if(filesystem_plugin == al_plugin_ptr())
				return set_ptr();

			opl::string xml;
			xml +=	"<filesystem_query searchfolder=\"";
			xml += search_folder;
			xml += "\" recurse=\"";
			xml += (recurse ? "TRUE" : "FALSE");
			xml += "\" filenameregex=\"";
			xml += filename_regex;
			xml += "\"/>";
			return filesystem_plugin->load(xml);
		}

		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize)
		{
			if(filesystem_plugin == al_plugin_ptr())
				return set_ptr();

			try
			{
				opl::string xml;
				xml +=	"<filesystem_query searchfolder=\"";
				xml += search_folder;
				xml += "\" recurse=\"";
				xml += (recurse ? "TRUE" : "FALSE");
				xml += "\" filenameregex=\"";
				xml += filename_regex;
				xml += "\">\n\t<filesize comparitor=\"";
				xml += filesize_comparitor;
				xml += "\">";
				xml += boost::lexical_cast<opl::string>(comparison_filesize);
				xml += "</filesize>\n</filesystem_query>";
				return filesystem_plugin->load(xml);
			}
			catch (const boost::bad_lexical_cast& e)
			{
				std::cerr << "Exception: " << e.what();
				return set_ptr();
			}
		}

		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& datetime_comparitor, const std::tm& comparison_datetime)
		{
			if(filesystem_plugin == al_plugin_ptr())
				return set_ptr();

			try
			{
				opl::string xml;
				xml +=	"<filesystem_query searchfolder=\"";
				xml += search_folder;
				xml += "\" recurse=\"";
				xml += (recurse ? "TRUE" : "FALSE");
				xml += "\" filenameregex=\"";
				xml += filename_regex;
				xml += "\">\n\t<lastmodified comparitor=\"";
				xml += datetime_comparitor;
				xml += "\">\n\t\t<year>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_year + 1900);
				xml += "</year>\n\t\t<month>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_mon);
				xml += "</month>\n\t\t<day>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_mday);
				xml += "</day>\n\t\t<hour>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_hour);
				xml += "</hour>\n\t\t<minute>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_min);
				xml += "</minute>\n\t\t<second>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_sec);
				xml += "</second>\n\t</lastmodified>\n</filesystem_query>";
				return filesystem_plugin->load(xml);
			}
			catch(const boost::bad_lexical_cast& e)
			{
				std::cerr << "Exception: " << e.what();
				return set_ptr();
			}
		}

		AL_DECLSPEC set_ptr search(const al_plugin_ptr& filesystem_plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize, const opl::string& datetime_comparitor,  const std::tm& comparison_datetime)
		{
			if(filesystem_plugin == al_plugin_ptr())
				return set_ptr();

			try
			{
				opl::string xml;
				xml +=	"<filesystem_query searchfolder=\"";
				xml += search_folder;
				xml += "\" recurse=\"";
				xml += (recurse ? "TRUE" : "FALSE");
				xml += "\" filenameregex=\"";
				xml += filename_regex;
				xml += "\">\n\t<filesize comparitor=\"";
				xml += filesize_comparitor;
				xml += "\">";
				xml += boost::lexical_cast<opl::string>(comparison_filesize);
				xml += "</filesize>\n\t<lastmodified comparitor=\"";
				xml += datetime_comparitor;
				xml += "\">\n\t\t<year>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_year + 1900);
				xml += "</year>\n\t\t<month>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_mon);
				xml += "</month>\n\t\t<day>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_mday);
				xml += "</day>\n\t\t<hour>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_hour);
				xml += "</hour>\n\t\t<minute>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_min);
				xml += "</minute>\n\t\t<second>";
				xml += boost::lexical_cast<opl::string>(comparison_datetime.tm_sec);
				xml += "</second>\n\t</lastmodified>\n</filesystem_query>";
				return filesystem_plugin->load(xml);
			}
			catch(const boost::bad_lexical_cast& e)
			{
				std::cerr << "Exception: " << e.what();
				return set_ptr();
			}
		}


		AL_DECLSPEC set_ptr restore(const al_plugin_ptr& database_plugin, const set_ptr& set)
		{
			if(database_plugin == al_plugin_ptr())
				return set_ptr();

			if(set == set_ptr())
				return set_ptr();

			if(set->empty())
				return set_ptr();

			opl::string xml("<database_query cmd=\"recover\">\n");
			set::iterator it = set->begin();
			do
			{
				xml += "\t<asset guid=\"";
				xml += (*it)->guid()->string();
				xml += "\"/>\n";

			} while(++it != set->end());
			xml += "</database_query>";

			// keep pointer continuity, by reusing assets passed.
			set_ptr returned_set = database_plugin->load(xml);
			returned_set->name(set->name());
			for(set::const_iterator ret_it = returned_set->begin();
				ret_it != returned_set->end();
				ret_it++)
			{
				asset_ptr asset = set->find((*ret_it)->guid());
				if(asset == asset_ptr())
					continue;

				for(asset::const_iterator metadata_it = (*ret_it)->begin();
					metadata_it != (*ret_it)->end();
					metadata_it++)
				{
					asset->set(metadata_it->first, metadata_it->second);
				}
			}
			return returned_set;
		}

		AL_DECLSPEC set_ptr store(const al_plugin_ptr& database_plugin, const set_ptr& set)
		{
			if(database_plugin == al_plugin_ptr())
				return set_ptr();

			return database_plugin->store(set);
		}


		AL_DECLSPEC bool save_set(const al_plugin_ptr& database_plugin, const opl::string& xmlfile, const set_ptr& set)
		{
			if(database_plugin == al_plugin_ptr())
				return false;

			if(set == set_ptr())
				return false;

			if(olib::openassetlib::al::store(database_plugin, set) != set_ptr())
				return false;

#if defined(_WIN32) && !defined(USE_LIBXML2) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			try
			{
				MSXML2::IXMLDOMDocument2Ptr pXMLDoc(__uuidof(MSXML2::DOMDocument60));
				pXMLDoc->validateOnParse = VARIANT_FALSE;

				MSXML2::IXMLDOMProcessingInstructionPtr pIXMLDOMProcessingInstruction = pXMLDoc->createProcessingInstruction(L"xml", L"version='1.0' encoding='UTF-8'");
				pXMLDoc->appendChild(pIXMLDOMProcessingInstruction);

				MSXML2::IXMLDOMNodePtr pNode = pXMLDoc->createNode(NODE_ELEMENT, SET_XML_ROOT_ELEMENT_NAME, "");
				((MSXML2::IXMLDOMElementPtr)pNode)->setAttribute(_bstr_t(SET_XML_NAME_ATTRIBUTE_NAME), _bstr_t(set->name().c_str()));
				pXMLDoc->appendChild(pNode);

				MSXML2::IXMLDOMNodePtr	pChildNode	= NULL;
				for(al::set::const_iterator it = set->begin();
					it != set->end();
					it++)
				{
					pChildNode = pXMLDoc->createNode(NODE_ELEMENT, _bstr_t(SET_XML_ASSET_ELEMENT_NAME), "");
					pChildNode->text = _bstr_t((*it)->guid()->string().c_str());
					pNode->appendChild(pChildNode);
				}

				pXMLDoc->save(_variant_t(xmlfile.c_str()));
				return true;
			}
			catch(_com_error& e)
			{
				std::cout << "Exception (_com_error): " << CT2CA(e.ErrorMessage()) << std::endl;
				return false;
			}

#else /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			xmlDocPtr pXMLDoc = xmlNewDoc(xmlCharStrdup("1.0"));
			if(pXMLDoc == NULL)
			{
				xmlFreeDoc(pXMLDoc);
				return false;
			}

			xmlNodePtr pNode = xmlNewDocRawNode(pXMLDoc, NULL, xmlCharStrdup(SET_XML_ROOT_ELEMENT_NAME), NULL);
			if(pNode == NULL)
			{
				xmlFreeDoc(pXMLDoc);
				return false;
			}

			xmlDocSetRootElement(pXMLDoc, pNode);
			xmlNewProp(pNode, xmlCharStrdup(SET_XML_NAME_ATTRIBUTE_NAME), xmlCharStrdup(set->name().c_str()));
			xmlNodePtr pChildNode = NULL;
			for(al::set::const_iterator it = set->begin();
				it != set->end();
				it++)
			{
				pChildNode = xmlNewTextChild(pNode, NULL, xmlCharStrdup(SET_XML_ASSET_ELEMENT_NAME), xmlCharStrdup((*it)->guid()->string().c_str()));
			}

			if(xmlSaveFormatFile(xmlfile.c_str(), pXMLDoc, 1) == -1)
			{
				xmlFreeDoc(pXMLDoc);
				return false;
			}

			xmlFreeDoc(pXMLDoc);
			return true;

#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		}

		AL_DECLSPEC set_ptr load_set(const al_plugin_ptr& database_plugin, const opl::string& xmlfile)
		{
			if(database_plugin == al_plugin_ptr())
				return set_ptr();

#if defined(_WIN32) && !defined(USE_LIBXML2) //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			try
			{
				MSXML2::IXMLDOMDocument2Ptr pXMLDoc(__uuidof(MSXML2::DOMDocument60));
				pXMLDoc->validateOnParse = VARIANT_FALSE;
				if(pXMLDoc->load(_variant_t(xmlfile.c_str())) == VARIANT_FALSE)
					return set_ptr();

				MSXML2::IXMLDOMElementPtr pElement = pXMLDoc->documentElement;
				if(pElement == NULL)
					return set_ptr();

				if(pElement->nodeName != _bstr_t(SET_XML_ROOT_ELEMENT_NAME))
					return set_ptr();

				opl::string name;
				MSXML2::IXMLDOMNamedNodeMapPtr pNodeMap = pElement->attributes;
				if(pNodeMap != NULL)
				{
					MSXML2::IXMLDOMNodePtr pNode = pNodeMap->getNamedItem(_bstr_t(SET_XML_NAME_ATTRIBUTE_NAME));
					if(pNode != NULL)
						name = opl::string((const char*)pNode->text);
				}

				MSXML2::IXMLDOMNodeListPtr pNodeList = pElement->getElementsByTagName(_bstr_t(SET_XML_ASSET_ELEMENT_NAME));
				if(pNodeList == NULL)
					return set_ptr();

				if(pNodeList->length == 0)
					return set_ptr();

				set_ptr set = set::create(name);
				for(unsigned char idx = 0; idx < pNodeList->length; idx++)
				{
					set->push_back(asset_factory::instance().create_asset(guid_factory::instance().create_guid(opl::string((const char*)pNodeList->item[idx]->text))));
				}

				return restore(database_plugin, set);
			}
			catch(const _com_error& e)
			{
				std::cout << "Exception (_com_error): " << CT2CA(e.ErrorMessage());
				return set_ptr();
			}

#else /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			xmlDocPtr	pXMLDoc	= NULL;
			xmlNodePtr	pNode	= NULL;

			pXMLDoc = xmlParseFile(xmlfile.c_str());
			if(pXMLDoc == NULL)
				return set_ptr();

			pNode = xmlDocGetRootElement(pXMLDoc);
			if(		(pNode == NULL)
				||	(pNode->type != XML_ELEMENT_NODE) )
			{
				xmlFreeDoc(pXMLDoc);
				xmlCleanupParser();
				return set_ptr();
			}

			if(xmlStrcmp(pNode->name, xmlCharStrdup(SET_XML_ROOT_ELEMENT_NAME)) != 0)
			{
				xmlFreeDoc(pXMLDoc);
				xmlCleanupParser();
				return set_ptr();
			}

			opl::string name;
			xmlChar* attrib = xmlGetProp(pNode, xmlCharStrdup(SET_XML_NAME_ATTRIBUTE_NAME));
			if(attrib != NULL)
			{
				name = (const char*)attrib;
				xmlFree(attrib);
			}

			set_ptr set = set::create(name);
			for(pNode = pNode->children; 
				pNode != NULL; 
				pNode = pNode->next)
			{
				if(		(pNode->type == XML_ELEMENT_NODE)
					&&	(xmlStrcmp(xmlCharStrdup(SET_XML_ASSET_ELEMENT_NAME), pNode->name) == 0)
					&&	(pNode->children != NULL)
					&&	(pNode->children->type == XML_TEXT_NODE) )
				{
					set->push_back(asset_factory::instance().create_asset(guid_factory::instance().create_guid(opl::string((const char*)pNode->children->content))));
				}
			}

			xmlFreeDoc(pXMLDoc);
			xmlCleanupParser();

			return restore(database_plugin, set);

#endif ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		}

		AL_DECLSPEC set_ptr search(const set_ptr& set, const predicate& pred)
		{
			return std::for_each(set->begin(), set->end(), set_search_predicate(pred)).result();
		}

		AL_DECLSPEC bool is_convertible(asset_ptr asset)
		{
			opl::string filepath = get_filepath(asset);
			if(filepath.empty())
				return false;

#ifdef USE_DISCOVERY
			typedef opl::discovery<query_traits> discovery;
			discovery plugins(query_traits(opl::to_wstring(filepath)));
			if(plugins.empty())
				return false;
#else
				return false;
#endif
			return true;
		}

		AL_DECLSPEC oil::il::image_type_ptr convert_to_image(asset_ptr asset)
		{
			opl::string filepath = get_filepath(asset);
			if(filepath.empty())
				return oil::il::image_type_ptr();

#ifdef USE_DISCOVERY
			typedef opl::discovery<oil_query_traits> discovery;
			discovery plugins(oil_query_traits(opl::to_wstring(filepath)));
			if(plugins.empty())
				return oil::il::image_type_ptr();

			plugins.sort(opl::highest_merit_sort());

			opl::opl_ptr plugin;
			discovery::const_iterator plugin_it = plugins.begin();
			do
			{
				plugin = plugin_it->create_plugin("");
				if(!plugin)
					continue;

				oil::il::image_type_ptr image = boost::shared_dynamic_cast<oil::il::openimagelib_plugin>(plugin)->load( filepath );
				if(image)
				{
					asset->set("width",		image->width());
					asset->set("height",	image->height());

					return image;
				}

			} while(++plugin_it != plugins.end());
#endif
			return oil::il::image_type_ptr();
		}

		AL_DECLSPEC oml::ml::input_type_ptr convert_to_media(asset_ptr asset)
		{
			opl::string filepath = get_filepath(asset);
			if(filepath.empty())
			{
				asset::const_iterator it = asset->find("file");
				if (it != asset->end())
					//filepath = fs::path(it->second.c_str( ), fs::native);
					filepath += it->second;
			}

			if(filepath.empty())
				return oml::ml::input_type_ptr();

#ifdef USE_DISCOVERY
			typedef opl::discovery<oml_query_traits> discovery;
			discovery plugins(oml_query_traits(opl::to_wstring(filepath)));
			if(plugins.empty())
				return oml::ml::input_type_ptr();

			plugins.sort(opl::highest_merit_sort());

			opl::opl_ptr plugin;
			discovery::const_iterator plugin_it = plugins.begin();
			do
			{
				plugin = plugin_it->create_plugin("");
				if(!plugin)
					continue;

				oml::ml::input_type_ptr media = oml::ml::create_input(opl::to_wstring(filepath) );
				if(media)
				{
					oml::ml::frame_type_ptr frame = media->fetch( );

					asset->set("frames",		media->get_frames());
					asset->set("video_streams",	media->get_video_streams());

					if ( frame->get_image( ) )
					{
						asset->set("width",		frame->get_image()->width());
						asset->set("height",	frame->get_image()->height());
						asset->set("sar",		frame->sar());
					}

					asset->set("audio_streams",	media->get_audio_streams());
					asset->set("fps",			frame->fps());

					return media;
				}

			} while(++plugin_it != plugins.end());

#endif
			return oml::ml::input_type_ptr();
		}

		AL_DECLSPEC ool::sg::scene_ptr convert_to_object(asset_ptr)
		{
			return ool::sg::scene_ptr();
		}

		AL_DECLSPEC bool store_image(asset_ptr asset, oil::il::image_type_ptr image, const opl::string& image_property_name_prefix)
		{
			if(asset == asset_ptr())
				return false;

			if(image == oil::il::image_type_ptr())
				return false;

			oil::il::image_type::size_type		width			= image->width();
			oil::il::image_type::size_type		height			= image->height();
			oil::il::image_type::size_type		size			= image->size();
			oil::il::image_type::const_pointer	data			= image->data(0, false);
			opl::wstring						colour_space	= image->pf();

			size_t numbytes_returned = 0;
			char* pbuffer = base64_encode(static_cast<const unsigned char*>(data), size, &numbytes_returned);
			if(!pbuffer)
				return false;

			opl::string data_as_a_string(pbuffer);
			delete [] pbuffer;

			asset->set(image_property_name_prefix + "::binary_data",	data_as_a_string);
			asset->set(image_property_name_prefix + "::size",			static_cast<const int>(size));
			asset->set(image_property_name_prefix + "::width",			static_cast<const int>(width));
			asset->set(image_property_name_prefix + "::height",			static_cast<const int>(height));
			asset->set(image_property_name_prefix + "::colour_space",	opl::to_string(colour_space));

			return true;
		}

		AL_DECLSPEC oil::il::image_type_ptr restore_image(asset_ptr asset, const opl::string& image_property_name_prefix)
		{
			if(asset == asset_ptr())
				return oil::il::image_type_ptr();

			opl::string binary_data;
			opl::string colour_space;
			opl::string width;
			opl::string height;
			opl::string size;

			if(		((binary_data	= asset->get(image_property_name_prefix + "::binary_data"))		==	asset::mapped_type())
				||	((colour_space	= asset->get(image_property_name_prefix + "::colour_space"))	==	asset::mapped_type())
				||	((width			= asset->get(image_property_name_prefix + "::width"))			==	asset::mapped_type())
				||	((height		= asset->get(image_property_name_prefix + "::height"))			==	asset::mapped_type())
				||	((size			= asset->get(image_property_name_prefix + "::size"))			==	asset::mapped_type()))
			{
				return oil::il::image_type_ptr();
			}

			size_t numbytes_returned = 0;
			unsigned char* pbuffer = base64_decode(binary_data.c_str(), &numbytes_returned);
			if(!pbuffer)
				return oil::il::image_type_ptr();

			try
			{
				oil::il::image_type_ptr image = oil::il::allocate(opl::to_wstring(colour_space).c_str(), boost::lexical_cast<int>(width.c_str()), boost::lexical_cast<int>(height.c_str()));
				if(image != oil::il::image_type_ptr())
					memcpy((void*)image->data(0, false), (const void*)pbuffer, boost::lexical_cast<int>(size.c_str()));
				delete [] pbuffer;
				return image;
			}
			catch(const boost::bad_lexical_cast& e)
			{
				fprintf(stdout, "Exception: %s\n", e.what());
				delete [] pbuffer;
				return oil::il::image_type_ptr();
			}
		}

		AL_DECLSPEC set_ptr calculate_union(const set_ptr& set1, const set_ptr& set2)
		{
			if(set1->empty() && set2->empty())
				return set::create();
			else if(set1->empty())
				return set::create(set2);
			else if(set2->empty())
				return set::create(set1);
			else
			{
				set_ptr union_set = set::create(set2);
				set::const_iterator it1 = set1->begin();
				do
				{
					bool found = false;

					set::const_iterator it2 = set2->begin();
					while(it2 != set2->end())
					{
						if(*((*it1)->guid().get()) == *((*it2)->guid().get()))
						{
							found = true;
							break;
						}

						it2++;
					}

					if(!found)
					{
						union_set->push_back(*it1);
					}

				} while(++it1 != set1->end());

				return union_set;
			}
		}

		AL_DECLSPEC set_ptr calculate_intersection(const set_ptr& set1, const set_ptr& set2)
		{
			set_ptr intersection_set = set::create();
			if(!set1->empty() && !set2->empty())
			{
				// TODO: This is inefficient and needs to be reworked!...
				set::const_iterator it1 = set1->begin();
				do
				{
					set::const_iterator it2 = set2->begin();
					while(it2 != set2->end())
					{
						if(*((*it1)->guid().get()) == *((*it2)->guid().get()))
							intersection_set->push_back(*it1);

						it2++;
					}

				} while(++it1 != set1->end());
			}

			return intersection_set;
		}

		AL_DECLSPEC set_ptr calculate_difference(const set_ptr& set1, const set_ptr& set2)
		{
			if(set1->empty() && set2->empty())
				return set::create();
			else if(set1->empty())
				return set::create(set2);
			else if(set2->empty())
				return set::create(set1);
			else
			{
				// TODO: This is inefficient and needs to be reworked!...
				set_ptr difference_set = set::create();
				set::const_iterator it1 = set1->begin();
				do
				{
					bool found = false;

					set::const_iterator it2 = set2->begin();
					while(it2 != set2->end())
					{
						if(*((*it1)->guid().get()) == *((*it2)->guid().get()))
						{
							found = true;
							break;
						}

						it2++;
					}

					if(!found)
					{
						difference_set->push_back(*it1);
					}

				} while(++it1 != set1->end());

				set::const_iterator it2 = set2->begin();
				do
				{
					bool found = false;

					set::const_iterator it1 = set1->begin();
					while(it1 != set1->end())
					{
						if(*((*it1)->guid().get()) == *((*it2)->guid().get()))
						{
							found = true;
							break;
						}

						it1++;
					}

					if(!found)
					{
						difference_set->push_back(*it2);
					}

				} while(++it2 != set2->end());

				return difference_set;
			}
		}

		// implementation of field_equal_to_predicate used for set searches
		key_value_regex_search_predicate::key_value_regex_search_predicate(const opl::string& key_regex, const opl::string& value_regex)
			:	key_regex_(key_regex)
			,	value_regex_(value_regex)
		{
		}

		key_value_regex_search_predicate::~key_value_regex_search_predicate()
		{
		}

		bool key_value_regex_search_predicate::operator()(const asset_ptr& asset) const
		{
			for(asset::const_iterator cit = asset->begin();
				cit != asset->end();
				cit++)
			{
				if(!boost::regex_match(cit->first.c_str(), boost::regex(key_regex_.c_str())))
					continue;

				if(boost::regex_match(cit->second.c_str(), boost::regex(value_regex_.c_str())))
					return true;
			}

			return false;
		}
		
		// implementation of location_filename_regex_predicate used for set searches
		location_filename_regex_predicate::location_filename_regex_predicate(const opl::string& location_regex, const opl::string& filename_regex)
			:	location_regex_(location_regex)
			,	filename_regex_(filename_regex)
		{
		}

		location_filename_regex_predicate::~location_filename_regex_predicate()
		{
		}

		bool location_filename_regex_predicate::operator()(const asset_ptr& asset) const
		{
			asset::const_iterator location_cit = asset->find("location");
			if(location_cit == asset->end())
				return false;

			if(!boost::regex_match(location_cit->second.c_str(), boost::regex(location_regex_.c_str())))
				return false;

			asset::const_iterator filename_cit = asset->find("filename");
			if(filename_cit == asset->end())
				return false;

			if(!boost::regex_match(filename_cit->second.c_str(), boost::regex(filename_regex_.c_str())))
				return false;

			return true;
		}
	}
}
} 
