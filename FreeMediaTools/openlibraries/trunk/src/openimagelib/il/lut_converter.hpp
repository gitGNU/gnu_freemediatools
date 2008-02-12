
// il - An image library representation.

// Copyright (C) 2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef LUT_CONVERTER_INC_
#define LUT_CONVERTER_INC_

#include <functional>

#include <openpluginlib/pl/fast_math.hpp>

namespace olib { namespace openimagelib { namespace il {

struct byte_to_byte : public std::unary_function<unsigned char, unsigned char>
{
	result_type operator( )( argument_type v ) const
	{ return v; }
};

struct byte_to_float : public std::unary_function<unsigned char, float>
{
	result_type operator( )( argument_type v ) const
	{ return v / 255.0f; }
};

struct float_to_float : public std::unary_function<float, float>
{
	result_type operator( )( argument_type v ) const
	{ return v; }
};

// Assumes normalise( im, 255.0f ) has already been called.
struct float_to_byte : public std::unary_function<float, unsigned char>
{
	result_type operator( )( argument_type v ) const
	{ return static_cast<result_type>( openpluginlib::fast_floorf( v ) ); }
};

} } }

#endif
