
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef STREAM_INC_
#define STREAM_INC_

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

#include <boost/shared_ptr.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/string.hpp>

namespace olib { namespace openpluginlib {

class OPENPLUGINLIB_DECLSPEC stream
{
public:
	typedef char				value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;

public:
	explicit stream( pointer data = 0, std::ios::openmode flags = std::ios::in | std::ios::out, std::size_t max_size = 0 )
		: data_( data )
		, offset_( 0 )
		, max_size_( max_size )
		, flags_( flags )
	{ }
	
	explicit stream( const string& path, std::ios::openmode flags = std::ios::in | std::ios::out, std::size_t max_size = 0 );
	
	~stream( );
	
	std::size_t read( pointer data, std::size_t bytes );
	std::size_t write( const_pointer data, std::size_t bytes );
	void seek( std::size_t bytes, std::ios::seekdir flags );
	std::size_t getline( pointer data, std::size_t bytes );

	std::ios::openmode flags( ) const { return flags_; }
	bool is_null( ) const { return data_ == 0; }
	const char* data( ) const { return data_; }
	std::size_t offset( ) const { return offset_; }
	std::size_t max_size( ) const { return max_size_; }
	
	string path( ) const;
	wstring wpath( ) const;

private:
	std::size_t actual_bytes( std::size_t bytes ) const;

private:
	stream( const stream& );
	stream& operator=( const stream& );

private:
	pointer data_;
	std::size_t offset_, max_size_;
	std::ios::openmode flags_;
	string path_;
	
private:
	boost::iostreams::mapped_file mapped_;
};

typedef boost::shared_ptr<stream> stream_ptr;

OPENPLUGINLIB_DECLSPEC stream_ptr make_stream( const string& path, std::ios::openmode flags );
OPENPLUGINLIB_DECLSPEC stream_ptr make_stream( void* data, std::ios::openmode flags );

} }

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif
