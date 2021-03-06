
// openpluginlib - An plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef STRING_INC_
#define STRING_INC_

#if _MSC_VER
#pragma warning ( push )
#pragma warning ( disable:4244 4251 4503 4512 )
#endif

#if defined( HAVE_FLEX_STRING )

#include <openpluginlib/pl/flex_string.h>

#else

#include <string>

#endif

#include <vector>

#if _MSC_VER >= 1400
#include <functional>
#include <hash_map>
#endif

namespace olib { namespace openpluginlib {

#if defined( HAVE_FLEX_STRING )
	typedef flex_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>, SmallStringOpt<SimpleStringStorage<wchar_t>, 25> > wstring;
	typedef flex_string<char, std::char_traits<char>, std::allocator<char>, SmallStringOpt<SimpleStringStorage<char>, 25> >				string;
#else
	typedef std::wstring wstring;
	typedef std::string  string;
#endif

    typedef std::vector<string> string_list;
	typedef std::vector<wstring> wstring_list;
	
	typedef std::vector<string> string_vec;
	typedef std::vector<wstring> wstring_vec;

} }

namespace std
{
#if defined( HAVE_FLEX_STRING ) 
	template<>
	struct less<olib::openpluginlib::string>
	{
		bool operator( )( const olib::openpluginlib::string& lhs, const olib::openpluginlib::string& rhs ) const
		{ return lhs.compare( rhs ) < 0; }
	};
	
	template<>
	struct less<olib::openpluginlib::wstring>
	{
		bool operator( )( const olib::openpluginlib::wstring& lhs, const olib::openpluginlib::wstring& rhs ) const
		{ return lhs.compare( rhs ) < 0; }
	};
	
#if  _MSC_VER >= 1400
	inline size_t hash_value( const olib::openpluginlib::string& str )
	{ return stdext::hash_value( str.c_str( ) ); }

	inline size_t hash_value( const olib::openpluginlib::wstring& str )
	{ return stdext::hash_value( str.c_str( ) ); }
#endif
#endif
}

#if _MSC_VER
#pragma warning ( pop )
#endif

#endif
