
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <algorithm>
#include <cstdlib>
#include <sstream>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/pcos/property.hpp>
#include <openpluginlib/pl/pcos/subject.hpp>
#include <openpluginlib/pl/pcos/property_container.hpp>

namespace opl = olib::openpluginlib;

namespace olib { namespace openpluginlib { namespace pcos {

namespace
{
	// implementation of parsing code for pcos::any

	template<typename T>
	T parse_string( const wstring& str )
	{
		std::wistringstream istrm( str.c_str( ) );
		T v;
		istrm >> v;
		return v;	
	}

	template <> OPENPLUGINLIB_DECLSPEC bool parse_string( const wstring& str )
	{
		if ( str.find( L"true" ) != wstring::npos )
			return true;

		return false;
	}

	template <> OPENPLUGINLIB_DECLSPEC string parse_string( const wstring& str )
	{
		/// the best that can be hoped for in this case is straight decimation yields something
		return to_string( str );
	}

	template <> OPENPLUGINLIB_DECLSPEC wstring parse_string( const wstring& str )
	{
		return str;
	}

	template OPENPLUGINLIB_DECLSPEC int parse_string( const wstring& str );
	template OPENPLUGINLIB_DECLSPEC double parse_string( const wstring& str );
	template OPENPLUGINLIB_DECLSPEC unsigned int parse_string( const wstring& str );

	/// split lists on colons
	template < typename RESULT_T > RESULT_T split_list( const wstring& str )
	{
		const wchar_t SEPARATOR = L':';

		RESULT_T result;
		size_t last_pos = 0;
		size_t current_pos = str.find( SEPARATOR );
		while ( current_pos != wstring::npos )
		{
			result.push_back( parse_string< typename RESULT_T::value_type >( wstring( str, last_pos, current_pos - last_pos ) ) );

			last_pos = ++current_pos;
			current_pos = str.find( SEPARATOR, last_pos );
		}
		
		result.push_back( parse_string< typename RESULT_T::value_type >( wstring( str, last_pos, current_pos ) ) );

		return result;
	}

	template <> OPENPLUGINLIB_DECLSPEC wstring_list parse_string( const wstring& str )
	{
		return split_list< wstring_list >( str );
	}

	template <> OPENPLUGINLIB_DECLSPEC string_list parse_string( const wstring& str )
	{
		return split_list< string_list >( str );
	}

	template <> OPENPLUGINLIB_DECLSPEC int_list parse_string( const wstring& str )
	{
		return split_list< int_list >( str );
	}

	template <> OPENPLUGINLIB_DECLSPEC uint_list parse_string( const wstring& str )
	{
		return split_list< uint_list >( str );
	}

	template <> OPENPLUGINLIB_DECLSPEC double_list parse_string( const wstring& str )
	{
		return split_list< double_list >( str );
	}

	class equals : public boost::static_visitor<bool>
	{
	public:
		template<typename T, typename U>
		bool operator( )( const T&, const U& ) const
		{
			return false;
		}
		
		template<typename T>
		bool operator( )( const T& lhs, const T& rhs ) const
		{
			return lhs == rhs;
		}
	};

	template<typename T>
	struct convert_to : public boost::static_visitor<T>
	{
		template<typename U>
		T operator( )( const U& ) const
		{ return T( ); }
		
		T operator( )( const T& t ) const
		{ return t; }
	};

	template<>
	struct convert_to<bool> : public boost::static_visitor<bool>
	{
		bool operator( )( bool b ) const
		{ return b; }
		bool operator( )( int i ) const
		{ return i != 0; }
		bool operator( )( unsigned int i ) const
		{ return i != 0; }
		bool operator( )( float f ) const
		{ return f != 0.0f; }
		bool operator( )( double d ) const
		{ return d != 0.0; }
		bool operator( )( const string& s ) const
		{ return s == "true"; }
		bool operator( )( const wstring& s ) const
		{ return s == L"true"; }
		bool operator( )( const int_list& ) const
		{ return false; }
		bool operator( )( const uint_list& ) const
		{ return false; }
		bool operator( )( const double_list& ) const
		{ return false; }
		bool operator( )( const string_list& ) const
		{ return false; }
		bool operator( )( const wstring_list& ) const
		{ return false; }
		bool operator( )( const void_vec& ) const
		{ return false; }
	};
	
	template<>
	struct convert_to<int> : public boost::static_visitor<int>
	{
		int operator( )( bool b ) const
		{ return b != 0; }
		int operator( )( int i ) const
		{ return i; }
		int operator( )( unsigned int i ) const
		{ return i; }
		int operator( )( float f ) const
		{ return static_cast<int>( f ); }
		int operator( )( double d ) const
		{ return static_cast<int>( d ); }
		int operator( )( const string& s ) const
		{ return strtol( s.c_str( ), 0, 10 ); }
		int operator( )( const wstring& s ) const
		{ return wcstol( s.c_str( ), 0, 10 ); }
		int operator( )( const int_list& ) const
		{ return 0; }
		int operator( )( const uint_list& ) const
		{ return 0; }
		int operator( )( const double_list& ) const
		{ return 0; }
		int operator( )( const string_list& ) const
		{ return 0; }
		int operator( )( const wstring_list& ) const
		{ return 0; }
		int operator( )( const void_vec& ) const
		{ return 0; }
	};

	template<>
	struct convert_to<float> : public boost::static_visitor<float>
	{
		float operator( )( bool b ) const
		{ return b ? 1.0f : 0.0f; }
		float operator( )( int i ) const
		{ return float( i ); }
		float operator( )( unsigned int i ) const
		{ return float( i ); }
		float operator( )( float f ) const
		{ return f; }
		float operator( )( double d ) const
		{ return float( d ); }
		float operator( )( const string& s ) const
		{ return float( strtod( s.c_str( ), 0 ) ); }
		float operator( )( const wstring& s ) const
		{ return float( wcstod( s.c_str( ), 0 ) ); }
		float operator( )( const int_list& ) const
		{ return 0.0f; }
		float operator( )( const uint_list& ) const
		{ return 0.0f; }
		float operator( )( const double_list& ) const
		{ return 0.0f; }
		float operator( )( const string_list& ) const
		{ return 0.0f; }
		float operator( )( const wstring_list& ) const
		{ return 0.0f; }
		float operator( )( const void_vec& ) const
		{ return 0.0f; }
	};

	template<>
	struct convert_to<double> : public boost::static_visitor<double>
	{
		double operator( )( bool b ) const
		{ return b != 0; }
		double operator( )( int i ) const
		{ return i; }
		double operator( )( unsigned int i ) const
		{ return i; }
		double operator( )( float f ) const
		{ return static_cast<double>( f ); }
		double operator( )( double d ) const
		{ return d; }
		double operator( )( const string& s ) const
		{ return strtod( s.c_str( ), 0 ); }
		double operator( )( const wstring& s ) const
		{ return wcstod( s.c_str( ), 0 ); }
		double operator( )( const int_list& ) const
		{ return 0.0; }
		double operator( )( const uint_list& ) const
		{ return 0.0; }
		double operator( )( const double_list& ) const
		{ return 0.0; }
		double operator( )( const string_list& ) const
		{ return 0.0; }
		double operator( )( const wstring_list& ) const
		{ return 0.0; }
		double operator( )( const void_vec& ) const
		{ return 0.0; }
	};
	
	template<>
	struct convert_to<string> : public boost::static_visitor<string>
	{
		string operator( )( bool b ) const
		{ return b ? "true" : "false"; }
		string operator( )( int i ) const
		{
			std::ostringstream strm;
			strm << i;
			return strm.str( );
		}
		string operator( )( unsigned int i ) const
		{
			std::ostringstream strm;
			strm << i;
			return strm.str( );
		}
		string operator( )( float f ) const
		{
			std::ostringstream strm;
			strm << f;
			return strm.str( );
		}
		string operator( )( double d ) const
		{
			std::ostringstream strm;
			strm << d;
			return strm.str( );
		}
		string operator( )( const string& s ) const
		{ return s; }
		string operator( )( const wstring& s ) const
		{ return opl::to_string( s ); }
		string operator( )( const int_list& ) const
		{ return ""; }
		string operator( )( const uint_list& ) const
		{ return ""; }
		string operator( )( const double_list& ) const
		{ return ""; }
		string operator( )( const string_list& ) const
		{ return ""; }
		string operator( )( const wstring_list& ) const
		{ return ""; }
		string operator( )( const void_vec& ) const
		{ return ""; }
	};
	
	template<>
	struct convert_to<wstring> : public boost::static_visitor<wstring>
	{
		wstring operator( )( bool b ) const
		{ return b ? L"true" : L"false"; }
		wstring operator( )( int i ) const
		{
			std::wostringstream strm;
			strm << i;
			return strm.str( );
		}
		wstring operator( )( unsigned int i ) const
		{
			std::wostringstream strm;
			strm << i;
			return strm.str( );
		}
		wstring operator( )( float f ) const
		{
			std::wostringstream strm;
			strm << f;
			return strm.str( );
		}
		wstring operator( )( double d ) const
		{
			std::wostringstream strm;
			strm << d;
			return strm.str( );
		}
		wstring operator( )( const string& s ) const
		{ return opl::to_wstring( s ); }
		wstring operator( )( const wstring& s ) const
		{ return s; }
		wstring operator( )( const int_list& ) const
		{ return L""; }
		wstring operator( )( const uint_list& ) const
		{ return L""; }
		wstring operator( )( const double_list& ) const
		{ return L""; }
		wstring operator( )( const string_list& ) const
		{ return L""; }
		wstring operator( )( const wstring_list& ) const
		{ return L""; }
		wstring operator( )( const void_vec& ) const
		{ return L""; }
	};

	template<>
	struct convert_to<void*> : public boost::static_visitor<void*>
	{
		void* operator( )( bool ) const
		{ return 0; }
		void* operator( )( int ) const
		{ return 0; }
		void* operator( )( unsigned int ) const
		{ return 0; }
		void* operator( )( float ) const
		{ return 0; }
		void* operator( )( double ) const
		{ return 0; }
		void* operator( )( const string& ) const
		{ return 0; }
		void* operator( )( const wstring& ) const
		{ return 0; }
		void* operator( )( const int_list& ) const
		{ return 0; }
		void* operator( )( const uint_list& ) const
		{ return 0; }
		void* operator( )( const double_list& ) const
		{ return 0; }
		void* operator( )( const string_list& ) const
		{ return 0; }
		void* operator( )( const wstring_list& ) const
		{ return 0; }
		void* operator( )( void* ptr ) const
		{ return ptr; }
		void* operator( )( const void_vec& ptr ) const
		{ return !ptr.empty( ) ? ptr[ 0 ] : 0; }
	};

	template<>
	struct convert_to<string_list> : public boost::static_visitor<string_list>
	{
		string_list operator( )( bool b ) const
		{
			string_list l;
			b ? l.push_back( "true" ) : l.push_back( "false" );
			return l;
		}
		string_list operator( )( int i ) const
		{
			std::ostringstream strm;
			strm << i;
			
			string_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		string_list operator( )( unsigned int i ) const
		{
			std::ostringstream strm;
			strm << i;
			
			string_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		string_list operator( )( float f ) const
		{
			std::ostringstream strm;
			strm << f;
			
			string_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		string_list operator( )( double d ) const
		{
			std::ostringstream strm;
			strm << d;
			
			string_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		string_list operator( )( const string& s ) const
		{ return parse_string<string_list>( opl::to_wstring( s ) ); }
		string_list operator( )( const wstring& s ) const
		{ return parse_string<string_list>( s ); }
		string_list operator( )( const int_list& ) const
		{ return string_list( ); }
		string_list operator( )( const uint_list& ) const
		{ return string_list( ); }
		string_list operator( )( const double_list& ) const
		{ return string_list( ); }
		string_list operator( )( const string_list& l ) const
		{ return l; }
		string_list operator( )( const wstring_list& ) const
		{ return string_list( ); }
		string_list operator( )( const void_vec& ) const
		{ return string_list( ); }
	};

	template<>
	struct convert_to<wstring_list> : public boost::static_visitor<wstring_list>
	{
		wstring_list operator( )( bool b ) const
		{
			wstring_list l;
			b ? l.push_back( L"true" ) : l.push_back( L"false" );
			return l;
		}
		wstring_list operator( )( int i ) const
		{
			std::wostringstream strm;
			strm << i;
			
			wstring_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		wstring_list operator( )( unsigned int i ) const
		{
			std::wostringstream strm;
			strm << i;
			
			wstring_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		wstring_list operator( )( float f ) const
		{
			std::wostringstream strm;
			strm << f;
			
			wstring_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		wstring_list operator( )( double d ) const
		{
			std::wostringstream strm;
			strm << d;
			
			wstring_list l;
			l.push_back( strm.str( ) );

			return l;
		}
		wstring_list operator( )( void* ) const
		{ return wstring_list( ); }
		wstring_list operator( )( const string& s ) const
		{ return parse_string<wstring_list>( opl::to_wstring( s ) ); }
		wstring_list operator( )( const wstring& s ) const
		{ return parse_string<wstring_list>( s ); }
		wstring_list operator( )( const int_list& ) const
		{ return wstring_list( ); }
		wstring_list operator( )( const uint_list& ) const
		{ return wstring_list( ); }
		wstring_list operator( )( const double_list& ) const
		{ return wstring_list( ); }
		wstring_list operator( )( const string_list& ) const
		{ return wstring_list( ); }
		wstring_list operator( )( const wstring_list& l ) const
		{ return l; }
		wstring_list operator( )( const void_vec& ) const
		{ return wstring_list( ); }
	};
	
	template<>
	struct convert_to<void_vec> : public boost::static_visitor<void_vec>
	{
		void_vec operator( )( bool ) const
		{ return void_vec( ); }
		void_vec operator( )( int ) const
		{ return void_vec( ); }
		void_vec operator( )( unsigned int ) const
		{ return void_vec( ); }
		void_vec operator( )( float ) const
		{ return void_vec( ); }
		void_vec operator( )( double ) const
		{ return void_vec( ); }
		void_vec operator( )( const string& ) const
		{ return void_vec( ); }
		void_vec operator( )( const wstring& ) const
		{ return void_vec( ); }
		void_vec operator( )( void* s ) const
		{ return void_vec( 1, s ); }
		void_vec operator( )( const int_list& ) const
		{ return void_vec( ); }
		void_vec operator( )( const uint_list& ) const
		{ return void_vec( ); }
		void_vec operator( )( const double_list& ) const
		{ return void_vec( ); }
		void_vec operator( )( const string_list& ) const
		{ return void_vec( ); }
		void_vec operator( )( const wstring_list& ) const
		{ return void_vec( ); }
		void_vec operator( )( const void_vec& v ) const
		{ return v; }
	};
}

class property::property_impl
{
public:
	property_impl( const key& k )
		: key_( k ),
		  always_notify( false )
		{}

	/// note: clone doesn't copy across the subject i.e. if you clone a
	/// property, you wouldn't (I don't think) expect the observers to 
	/// notice if the clone changes...
	property_impl* clone() const
	{
		property_impl* result = new property_impl( key_ );
		result->value = value;
		result->always_notify = always_notify;
		result->container_ = container_;

		return result;
	}

	key key_;
	any value;
	bool always_notify;
	subject subject_;
	property_container container_;
};

property property::NULL_PROPERTY;

property::property()
{
	*this = NULL_PROPERTY;
}

property::property( const key& k )
	: impl_( new property_impl( k ) )
{}

property::~property()
{}

void property::attach( boost::shared_ptr< observer > obs )
{
	impl_->subject_.attach( obs );
}

void property::detach( boost::shared_ptr< observer > obs )
{
	impl_->subject_.attach( obs );
}

void property::update()
{
	impl_->subject_.update();
}

void property::block( boost::shared_ptr< observer > obs )
{
	impl_->subject_.block( obs );
}

void property::unblock( boost::shared_ptr< observer > obs )
{
	impl_->subject_.unblock( obs );
}

property property::get_property_with_string( const char* k ) const
{
	return impl_->container_.get_property_with_string( k );
}

property property::get_property_with_key( const key& k ) const
{
	return impl_->container_.get_property_with_key( k );
}

key_vector property::get_keys() const
{
	return impl_->container_.get_keys();
}

property* property::clone() const
{
	property* result = new property( get_key() );
	result->impl_.reset( impl_->clone() );

	return result;
}

key property::get_key() const
{
	return impl_->key_;
}

property& property::operator=( const any& v )
{
	if( impl_->always_notify || !boost::apply_visitor( equals( ), v, impl_->value ) )
	{
		impl_->value = v;
		update( );
	}

	return *this;
}

property& property::operator=( const property& p )
{
	impl_ = p.impl_;
	if ( !impl_.get() )
		return *this;

	update();

	return *this;
}

bool property::operator==( const property& rhs ) const
{
	return impl_ == rhs.impl_;
}

bool property::operator!=( const property& rhs ) const
{
	return !operator==( rhs );
}

template < typename T > void property::set( const T& v )
{
	operator=( any( v ) );
}

void property::set_from_string( const wstring& str )
{
	operator=( str );
}

void property::set_from_property( const property& p )
{
	impl_->value = p.impl_->value;
	update( );
}
  
template < typename T > T property::value() const
{
	return boost::apply_visitor( convert_to<T>( ), impl_->value );
}

template < typename T > T* property::pointer() const
{
	return boost::get<T>( &impl_->value );
}
	
template < typename T > bool property::is_a() const
{
	if( T* p = boost::get<T>( &impl_->value ) )
		return true;
	
	return false;
}

void property::set_always_notify( bool b )
{
	impl_->always_notify = b;
}

void property::accept( visitor& v )
{
	v.visit_property( *this );
	v.visit_property_container( impl_->container_ );
}

bool property::valid() const
{ 
	return *this != NULL_PROPERTY; 
}

template OPENPLUGINLIB_DECLSPEC int property::value<int>( ) const;
template OPENPLUGINLIB_DECLSPEC unsigned int property::value<unsigned int>( ) const;
template OPENPLUGINLIB_DECLSPEC float property::value<float>( ) const;
template OPENPLUGINLIB_DECLSPEC double property::value<double>( ) const;
template OPENPLUGINLIB_DECLSPEC int_list property::value<int_list>( ) const;
template OPENPLUGINLIB_DECLSPEC uint_list property::value<uint_list>( ) const;
template OPENPLUGINLIB_DECLSPEC double_list property::value<double_list>( ) const;
template OPENPLUGINLIB_DECLSPEC bool property::value<bool>( ) const;
template OPENPLUGINLIB_DECLSPEC void* property::value<void*>( ) const;
template OPENPLUGINLIB_DECLSPEC void_vec property::value<void_vec>( ) const;

template OPENPLUGINLIB_DECLSPEC wstring* property::pointer< wstring >() const;
template OPENPLUGINLIB_DECLSPEC string* property::pointer< string >() const;
template OPENPLUGINLIB_DECLSPEC wstring_vec* property::pointer< wstring_vec >() const;
template OPENPLUGINLIB_DECLSPEC string_vec* property::pointer< string_vec >() const;

template OPENPLUGINLIB_DECLSPEC void property::set< int >( const int& );
template OPENPLUGINLIB_DECLSPEC void property::set< unsigned int >( const unsigned int& );
template OPENPLUGINLIB_DECLSPEC void property::set< double >( const double& );
template OPENPLUGINLIB_DECLSPEC void property::set< int_list >( const int_list& );
template OPENPLUGINLIB_DECLSPEC void property::set< uint_list >( const uint_list& );
template OPENPLUGINLIB_DECLSPEC void property::set< double_list >( const double_list& );
template OPENPLUGINLIB_DECLSPEC void property::set< bool >( const bool& );

template OPENPLUGINLIB_DECLSPEC bool property::is_a< int >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< unsigned int >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< double >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< int_list >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< uint_list >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< double_list >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< bool >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< void_vec >() const;

template OPENPLUGINLIB_DECLSPEC wstring property::value< wstring >() const;
template OPENPLUGINLIB_DECLSPEC string property::value< string >() const;
template OPENPLUGINLIB_DECLSPEC wstring_list property::value< wstring_list >() const;
template OPENPLUGINLIB_DECLSPEC string_list property::value< string_list >() const;

template OPENPLUGINLIB_DECLSPEC void property::set< wstring >( const wstring& );
template OPENPLUGINLIB_DECLSPEC void property::set< string >( const string& );
template OPENPLUGINLIB_DECLSPEC void property::set< wstring_list >( const wstring_list& );
template OPENPLUGINLIB_DECLSPEC void property::set< string_list >( const string_list& );

template OPENPLUGINLIB_DECLSPEC bool property::is_a< wstring >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< string >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< wstring_list >() const;
template OPENPLUGINLIB_DECLSPEC bool property::is_a< string_list >() const;

} } }
