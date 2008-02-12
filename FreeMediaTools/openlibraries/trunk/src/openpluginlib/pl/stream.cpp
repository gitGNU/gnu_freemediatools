
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <boost/filesystem/operations.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/stream.hpp>

namespace fs = boost::filesystem;

namespace olib { namespace openpluginlib {

stream::stream( const string& path, std::ios::openmode flags, std::size_t max_size )
	: data_( 0 )
	, offset_( 0 )
	, max_size_( max_size )
	, flags_( flags )
	, path_( path )
{
	fs::path fs_path( path.c_str( ), fs::native );
	if( fs::exists( fs_path ) )
	{
		mapped_.open( fs_path.native_file_string( ), flags );
		if( mapped_.is_open( ) )
		{
			if( ( flags_ && std::ios::out ) != 0 ) data_ = const_cast<char*>( mapped_.const_data( ) );
			else data_ = mapped_.data( );

			max_size_ = mapped_.size( );
		}
	}
}

stream::~stream( )
{
	if( mapped_.is_open( ) )
	{
		data_ = 0;
		offset_ = 0;
		mapped_.close( );
	}
}

std::size_t stream::read( stream::pointer data, std::size_t bytes )
{
	bytes = actual_bytes( bytes );

	memcpy( data, data_ + offset_, bytes ); 
	offset_ += bytes;
	
	return bytes;
}

std::size_t stream::write( stream::const_pointer data, std::size_t bytes )
{
	bytes = actual_bytes( bytes );

	memcpy( data_ + offset_, data, bytes );
	offset_ += bytes;

	return bytes;
}

void stream::seek( std::size_t bytes, std::ios::seekdir flags )
{
	if( flags == std::ios::beg )
		offset_ = bytes;
	else if( flags == std::ios::cur )
		offset_ += bytes;
	else if( flags == std::ios::end )
		offset_ = max_size_ - bytes;
}

std::size_t stream::getline( pointer data, std::size_t bytes )
{
	bytes = actual_bytes( bytes );
	
	std::size_t i = offset_, j = 0;
	for( ; *( data_ + i ) != '\n' && i < offset_ + bytes; ++i, ++j )
		*data++ = *( data_ + i );
	
	if( *( data_ + i ) == '\n' )
		*data++ = '\0', ++j;
	
	offset_ += j;
	
	return bytes;
}

string stream::path( ) const
{
	return fs::path( path_.c_str( ), fs::native ).native_file_string( );
}

wstring stream::wpath( ) const
{
	return to_wstring( path( ) );
}

std::size_t stream::actual_bytes( std::size_t bytes ) const
{
	if( offset_ + bytes > max_size_ )
		bytes = max_size_ - offset_;
		
	return bytes;
}

stream_ptr make_stream( const string& path, std::ios::openmode flags )
{
	return stream_ptr( new stream( path, flags ) );
}

stream_ptr make_stream( void* data, std::ios::openmode flags )
{
	return stream_ptr( new stream( static_cast<char*>( data ), flags ) );
}

} }
