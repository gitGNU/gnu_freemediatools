// asset.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <openassetlib/al/asset.hpp>
#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace al = olib::openassetlib::al;
namespace fs = boost::filesystem;
namespace py = boost::python;

namespace olib { namespace openassetlib { namespace detail {

	namespace {

		al::asset::const_iterator	(al::asset::*begin)() const	= &al::asset::begin;
		al::asset::const_iterator	(al::asset::*end)() const	= &al::asset::end;

		const opl::string	(al::asset::*get_guid_string)(void) const	= &al::asset::guid_string;
		const al::guid_ptr	(al::asset::*get_guid)(void) const			= &al::asset::guid;

		void (al::asset::*set_bool)		(const opl::string& key, const bool value)			= &al::asset::set;
		void (al::asset::*set_long)		(const opl::string& key, const long value)			= &al::asset::set;
		void (al::asset::*set_int)		(const opl::string& key, const int value)			= &al::asset::set;
		void (al::asset::*set_float)	(const opl::string& key, const float value)			= &al::asset::set;
		void (al::asset::*set_string)	(const opl::string& key, const opl::string value)	= &al::asset::set;

		al::asset_ptr create_asset()
		{
			return al::asset_factory::instance().create_asset();
		}

		al::asset_ptr create_asset(const al::guid_ptr& guid)
		{
			return al::asset_factory::instance().create_asset(guid);
		}

		al::asset_ptr create_asset(const al::asset_ptr& asset)
		{
			return al::asset_factory::instance().create_asset(asset);
		}

		al::asset_ptr create_asset(const opl::string& fullpath)
		{
//			try
//			{
				//fs::path filepath(fullpath.c_str(), fs::native);
#if 0
				if(!fs::exists(filepath))
					return al::asset_ptr();
				if(fs::is_directory(filepath))
					return al::asset_ptr();
#endif

				al::asset_ptr asset = al::asset_factory::instance().create_asset();
				//asset->set("filename", filepath.leaf());
				asset->set("location", fullpath);
				return asset;
//			}
//			catch(const fs::filesystem_error& e)
//			{
//				fprintf(stderr, "filesystem_error: %s", e.what());
//				return al::asset_ptr();
//			}
		}

		al::asset_ptr (*create_asset1)()							= &create_asset;
		al::asset_ptr (*create_asset2)(const al::guid_ptr& guid)	= &create_asset;
		al::asset_ptr (*create_asset3)(const al::asset_ptr& asset)	= &create_asset;
		al::asset_ptr (*create_asset4)(const opl::string& filepath)	= &create_asset;

	}

	void py_asset()
	{
		py::class_< std::pair<const opl::string, opl::string> >( "pair_tmp" )
			.def( py::init<> ( ) )
			.def( py::init<opl::string, opl::string>( ) )
			.add_property(	"first", 
							py::make_getter(	&std::pair<const opl::string, opl::string>::first, 
												py::return_value_policy<py::return_by_value>( ) ) ) 
			.add_property(	"second", 
							py::make_getter(	&std::pair<const opl::string, opl::string>::second, 
												py::return_value_policy<py::return_by_value>( ) ), 
							py::make_setter(	&std::pair<const opl::string, opl::string>::second, 
												py::return_value_policy<py::return_by_value>( ) ) )
			;

		py::class_<al::asset, boost::noncopyable, al::asset_ptr, al::asset::reference>( "asset", py::no_init )
			.def( "guid", get_guid_string )
			.def( "get_guid", get_guid )
			.def( "get", &al::asset::get )
			.def( "set", set_bool )
			.def( "set", set_long )
			.def( "set", set_int )
			.def( "set", set_float )
			.def( "set", set_string )
			.def( "__iter__", py::range( begin, end ) )
			.def( "empty", &al::asset::empty )
			.def( "find", &al::asset::find_metadata )
			.def( "size", &al::asset::size )
			.def( "__eq__", &al::guid::operator== )
			.def( "__ne__", &al::guid::operator!= )
			;

		py::def( "create_asset", create_asset1 );
		py::def( "create_asset", create_asset2 );
		py::def( "create_asset", create_asset3 );
		py::def( "create_asset", create_asset4 );
	}

}}}
