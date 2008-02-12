// base64_codec.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef BASE64_CODEC_INC_
#define BASE64_CODEC_INC_

#include <map>
#include <boost/shared_ptr.hpp>

namespace olib { namespace openassetlib {

	char*			base64_encode(const unsigned char* data, size_t numbytes, size_t* numbytes_returned);
	unsigned char*	base64_decode(const char* text, size_t* numbytes_returned);

}}

#endif
