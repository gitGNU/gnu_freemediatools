
// sg - A scene graph representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef XML_VALUE_TOKENIZER_INC_
#define XML_VALUE_TOKENIZER_INC_

#ifdef WIN32
#import <msxml4.dll> raw_interfaces_only
#endif

#include <vector>

#include <boost/tokenizer.hpp>

#include <openpluginlib/pl/string.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/lexical_cast.hpp>

#if defined ( HAVE_FLEX_STRING )

// flex_string doesn't implement operator>>. The below
// is a hack implementation for use with boost::lexical_cast.

template <typename E, class T, class A, class S>
std::basic_istream<typename flex_string<E, T, A, S>::value_type, typename flex_string<E, T, A, S>::traits_type>&
operator>>(
    std::basic_istream<typename flex_string<E, T, A, S>::value_type,
    typename flex_string<E, T, A, S>::traits_type>& is,
    flex_string<E, T, A, S>& str)
{
    std::basic_string<E, T, A> tmp;

    is >> tmp;
    str.assign( tmp.begin( ), tmp.end( ) );

    return is;
}

#endif

namespace olib { namespace openobjectlib { namespace sg { namespace actions {

typedef boost::tokenizer<boost::char_separator<wchar_t>, openpluginlib::wstring::iterator, openpluginlib::wstring> wtokenizer;

#ifdef WIN32
bool get_value_from_name( const openpluginlib::wstring& name, MSXML2::ISAXAttributes __RPC_FAR* attributes, openpluginlib::wstring& value )
{
    wchar_t *w_str = NULL;
    int      length;

#if defined _MSC_VER && !defined __ICL
    if( SUCCEEDED(  attributes->getValueFromName(               // the casts are very M$ unfortunate.
                    reinterpret_cast<unsigned short*>( L"" ), 0,
                    ( unsigned short* )( name.c_str( ) ), ( int ) name.size( ),
                    reinterpret_cast<unsigned short**>( &w_str ), &length ) ) )
#else
    if( SUCCEEDED( attributes->getValueFromName(
        L"", 0,
        name.c_str( ), name.size( ),
        &w_str, &length ) ) )
#endif
    {
        value = openpluginlib::wstring( w_str, length );
        return true;
    }

    return false;
}
#else                                       // #ifdef WIN32
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
#endif                                      // #ifdef WIN32

struct xml_value_tokenizer
{
    static const int reserve_siz = 100;

#ifdef WIN32
    template<class T>
    bool tokenize( const openpluginlib::wstring& name, MSXML2::ISAXAttributes __RPC_FAR* attributes, std::vector<T>& values )
#else
    template<class T>
    bool tokenize( const openpluginlib::wstring& name, xmlChar** attributes, std::vector<T>& values )
#endif
    {
        openpluginlib::wstring str;
        if( get_value_from_name( name, attributes, str ) )
        {
            boost::char_separator<wchar_t> sep( L" ," );
            wtokenizer wtok( str.begin( ), str.end( ), sep );

            values.reserve( reserve_siz );

            for( wtokenizer::const_iterator I = wtok.begin( ); I != wtok.end( ); ++I )
                values.push_back( boost::lexical_cast<T>( I->c_str( ) ) );

            return true;
        }

        return false;
    }
};

} } } }

#endif
