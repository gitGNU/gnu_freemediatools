
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef XML_VALUE_TOKENIZER_INC_
#define XML_VALUE_TOKENIZER_INC_

#ifdef WIN32
#import <msxml6.dll> raw_interfaces_only
#endif

#include <cstdlib>
#include <vector>

#include <boost/tokenizer.hpp>

#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/sg/lexical_cast.hpp>

#if defined ( HAVE_FLEX_STRING )

// flex_string doesn't implement operator>>. The below
// is a hack implementation for use with boost::lexical_cast.

template <typename E, class T, class A, class S>
std::basic_istream<typename flex_string<E, T, A, S>::value_type, typename flex_string<E, T, A, S>::traits_type>& 
operator>>(
    std::basic_istream<typename flex_string<E, T, A, S>::value_type,
    typename flex_string<E, T, A, S>::traits_type>& is,
    flex_string<E, T, A, S>& str )
{
	std::basic_string<E, T, A> tmp;
	
	is >> tmp;
	str.assign( tmp.begin( ), tmp.end( ) );
	
	return is;
}

#endif

namespace olib { namespace openobjectlib { namespace sg { namespace actions {

namespace
{
	// 
	template<typename Target>
	struct string_to
	{
		template<typename Source>
		Target operator( )( const Source& arg )
		{ return boost::lexical_cast<Target, Source>( arg ); }
	};
	
	// specializations
	template<>
	struct string_to<float>
	{
		template<typename Source>
		float operator( )( const Source& arg )
		{ return static_cast<float>( wcstod( arg, 0 ) ); }
	};
	
	template<>
	struct string_to<int>
	{
		template<typename Source>
		int operator( )( const Source& arg )
		{ return static_cast<int>( wcstol( arg, 0, 10 ) ); }
	};
}

struct xml_value_tokenizer
{
	static const int RESERVE_SIZE = 1000;
		
#ifdef WIN32
	template<class T>
	bool tokenize( const openpluginlib::wstring& name, MSXML2::ISAXAttributes __RPC_FAR* attributes, std::vector<T>& values )
#else
	bool get_value_from_name( const openpluginlib::wstring& name, xmlChar** attributes, openpluginlib::wstring& value )
	{
		if( !attributes ) return false;

		xmlChar** begin = attributes;
		while( *begin )
		{
			if( name == openpluginlib::to_wstring( reinterpret_cast<char*>( *begin ) ) )
			{
				value = openpluginlib::to_wstring( reinterpret_cast<char*>( *( begin + 1 ) ) );
				return true;
			}

			begin += 2;
		}
		
		return false;
	}
	
	template<class T>
	bool tokenize( const openpluginlib::wstring& name, xmlChar** attributes, std::vector<T>& values )
#endif
	{
#ifdef WIN32
		wchar_t* value = NULL;
		int length;

		if( SUCCEEDED(	attributes->getValueFromName(
						( unsigned short*  ) L"", 0,
						( unsigned short*  ) name.c_str( ), ( int ) name.size( ),
						( unsigned short** ) &value, &length ) ) )
#else
		std::wstring str;
		if( get_value_from_name( name, attributes, str ) )
#endif
		{
			typedef boost::tokenizer<boost::char_separator<wchar_t>, openpluginlib::wstring::const_iterator, openpluginlib::wstring> wtokenizer;

#ifdef WIN32
			openpluginlib::wstring str( value, length );
#endif
			boost::char_separator<wchar_t> sep( L" ,\"" );
			wtokenizer wtok( str.begin( ), str.end( ), sep );

			values.reserve( RESERVE_SIZE );
			for( wtokenizer::const_iterator I = wtok.begin( ); I != wtok.end( ); ++I )
				values.push_back( string_to<T>( )( I->c_str( ) ) );

			return true;
		}
					
		return false;
	}
};

} } } }

#endif
