// al_plugin.hpp - Interface to an openassetlib plugin.
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef AL_PLUGIN_INC_
#define AL_PLUGIN_INC_

#include <openpluginlib/pl/openplugin.hpp>
#include <openassetlib/al/set.hpp>

namespace olib { namespace openassetlib { namespace al {

//======================================================

typedef boost::shared_ptr<class al_plugin>	al_plugin_ptr;

class AL_DECLSPEC al_plugin : public olib::openpluginlib::openplugin
{
public:
	virtual ~al_plugin(void)	{};

	virtual set_ptr	store(const set_ptr& set)			= 0;
	virtual set_ptr	load(const opl::string& query_xml)	= 0;

protected:
	explicit al_plugin(void)	{};
};

} } }

#endif
