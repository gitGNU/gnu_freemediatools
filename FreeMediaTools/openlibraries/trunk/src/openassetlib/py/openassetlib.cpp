// openassetlib.cpp

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openpluginlib/pl/openplugin.hpp>
#include <openassetlib/al/al.hpp>
#include <openassetlib/py/python.hpp>
#include <openassetlib/py/py.hpp>

namespace opl = olib::openpluginlib;
namespace py  = boost::python;

namespace olib { namespace openassetlib { namespace detail {

	namespace {

		struct guid_generator_wrapper : public guid_generator, public py::wrapper<guid_generator>
		{
			const opl::string generate_guid()
			{
				return py::call<opl::string>(this->get_override("generate_guid").ptr());
			}
		};

		// Filesystem plugin helper functions
		al::set_ptr filesystem_search1(const opl::opl_ptr& ptr, const opl::string& filename_regex, const opl::string& search_folder, bool recurse)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::search(plugin, filename_regex, search_folder, recurse);
		}

		al::set_ptr filesystem_search2(const opl::opl_ptr& ptr, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::search(plugin, filename_regex, search_folder, recurse, filesize_comparitor, comparison_filesize);
		}

		al::set_ptr filesystem_search3(const opl::opl_ptr& ptr, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& datetime_comparitor, const std::tm& comparison_datetime)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::search(plugin, filename_regex, search_folder, recurse, datetime_comparitor, comparison_datetime);
		}

		
		al::set_ptr (*filesystem_plugin_search1)(const opl::opl_ptr& plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse)                                                                             = &filesystem_search1;
		al::set_ptr (*filesystem_plugin_search2)(const opl::opl_ptr& plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& filesize_comparitor, long long comparison_filesize)      = &filesystem_search2;
		al::set_ptr (*filesystem_plugin_search3)(const opl::opl_ptr& plugin, const opl::string& filename_regex, const opl::string& search_folder, bool recurse, const opl::string& datetime_comparitor, const std::tm& comparison_datetime) = &filesystem_search3;

		// Metadata plugin helper functions
		al::set_ptr store_metadata(const opl::opl_ptr& ptr, const al::set_ptr& set)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::store(plugin, set);
		}

		al::set_ptr restore_metadata(const opl::opl_ptr& ptr, const al::set_ptr& set)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::restore(plugin, set);
		}

		al::set_ptr (*metadata_plugin_store)(const opl::opl_ptr& plugin, const al::set_ptr& set)	= &store_metadata;
		al::set_ptr (*metadata_plugin_restore)(const opl::opl_ptr& plugin, const al::set_ptr& set)	= &restore_metadata;

		// Set save & load.  Persists set to disk
		bool save__set(const opl::opl_ptr& ptr, const opl::string& xmlfile, const al::set_ptr& set)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::save_set(plugin, xmlfile, set);
		}

		al::set_ptr load__set(const opl::opl_ptr& ptr, const opl::string& xmlfile)
		{
			al::al_plugin_ptr plugin = boost::shared_dynamic_cast<al::al_plugin>( ptr );
			if( !plugin )
				return al::set_ptr( );

			return al::load_set(plugin, xmlfile);
		}

		bool		(*save_set)(const opl::opl_ptr& plugin, const opl::string& xmlfile, const al::set_ptr& set)	= &save__set;
		al::set_ptr	(*load_set)(const opl::opl_ptr& plugin, const opl::string& xmlfile)							= &load__set;

		// Set searches
		struct predicate_wrapper : public al::predicate, public py::wrapper<al::predicate>
		{
			virtual bool operator()(const al::asset_ptr& asset) const
			{
				return py::call<bool>(this->get_override("operator()").ptr(), asset);
			}
		};

		al::set_ptr (*set_search)(const al::set_ptr& set, const al::predicate& pred) = &al::search;
	}

	void py_openassetlib( )
	{
		py::class_<guid_generator_wrapper, boost::noncopyable>("guid_generator")
			.def("generate_guid", py::pure_virtual(&guid_generator::generate_guid))
			;

		py::class_<std::tm>("tm")
			.add_property("tm_year",	&std::tm::tm_year)
			.add_property("tm_mon",		&std::tm::tm_mon)
			.add_property("tm_mday",	&std::tm::tm_mday)
			.add_property("tm_hour",	&std::tm::tm_hour)
			.add_property("tm_min",		&std::tm::tm_min)
			.add_property("tm_sec",		&std::tm::tm_sec)
			.add_property("tm_yday",	&std::tm::tm_yday)
			.add_property("tm_wday",	&std::tm::tm_wday)
			.add_property("tm_isdst",	&std::tm::tm_isdst)
			;

		py::def("init",				&init);
		py::def("uninit",			&uninit);
		py::def("generate_guid",	&generate_guid);

		py::def("search", filesystem_plugin_search1);
		py::def("search", filesystem_plugin_search2);
		py::def("search", filesystem_plugin_search3);

		py::def("store",	metadata_plugin_store);
		py::def("restore",	metadata_plugin_restore);

		py::def("save_set", save_set);
		py::def("load_set", load_set);

		py::class_< predicate_wrapper, boost::noncopyable >("predicate")
			.def("__call__", py::pure_virtual(&al::predicate::operator()))
			;

		py::class_< al::key_value_regex_search_predicate, py::bases<al::predicate> >("key_value_regex_search_predicate", py::init<const opl::string&, const opl::string&>())
			.def("__call__", &al::key_value_regex_search_predicate::operator())
			;

		py::class_< al::location_filename_regex_predicate, py::bases<al::predicate> >("location_filename_regex_predicate", py::init<const opl::string&, const opl::string&>())
			.def("__call__", &al::location_filename_regex_predicate::operator())
			;

		py::def("search", set_search);

		py::def("is_convertible",	&al::is_convertible);
		py::def("convert_to_image",	&al::convert_to_image,	py::return_value_policy<py::return_by_value>());
		py::def("convert_to_media",	&al::convert_to_media,	py::return_value_policy<py::return_by_value>());
		py::def("store_image",		&al::store_image);
		py::def("restore_image",	&al::restore_image);

		py::def("calculate_union",			&al::calculate_union);
		py::def("calculate_intersection",	&al::calculate_intersection);
		py::def("calculate_difference",		&al::calculate_difference);
	}

}}}
