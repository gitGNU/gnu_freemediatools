// base64_codec.cpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <openassetlib/al/base64_codec.hpp>

namespace olib { namespace openassetlib {

	namespace {

		static const char* encode_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		unsigned char decode_byte(const char ch)
		{
			if(ch >= 'A' && ch <= 'Z')
				return ch - 'A';
			else if(ch >= 'a' && ch <= 'z')
				return 26 + (ch - 'a');
			else if(ch >= '0' && ch <= '9')
				return 52 + (ch - '0');
			else if(ch == '+')
				return 62;
			else if(ch == '/')
				return 63;
			else
				return (unsigned char)-1;
		}
	}

	// Encoding rules taken from MIME section of http://en.wikipedia.org/wiki/Base64
	char* base64_encode(const unsigned char* data, size_t numbytes, size_t* numbytes_returned)
	{
		if(		(data == NULL)
			||	(numbytes == 0)
			||	(numbytes_returned == NULL) )
			return NULL;

		//     [3 bytes equates to 4 chars] [remaining bytes + padding] [CR every 57bytes] [NULL]
		//                         |                      |                        |         | 
		size_t retbuff_size = 4*(numbytes/3) + ((numbytes%3 == 0) ? 0 : 4) + (numbytes/57) + 1;
		char* retbuff = new char[retbuff_size];
		if(!retbuff)
			return NULL;

		*numbytes_returned = retbuff_size;
		size_t offset = 0;
		size_t remaining_bytes = numbytes;
		unsigned long encbuf = 0;

		for(size_t idx = 0;
			idx < numbytes;
			idx += 3)
		{
			if(		(idx > 0)
				&&	(idx%57 == 0) )
				retbuff[offset++] = '\n';

			remaining_bytes = (numbytes - idx);
			if(remaining_bytes >= 3)
			{
				encbuf  = 0x00FF0000 & ((unsigned long)(data[idx]  ) << 16);
				encbuf |= 0x0000FF00 & ((unsigned long)(data[idx+1]) <<  8);
				encbuf |= 0x000000FF & ((unsigned long)(data[idx+2])      );

				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 18))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 12))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >>  6))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf      ))];
			}
			else if(remaining_bytes == 2)
			{
				encbuf  = 0x00FF0000 & ((unsigned long)(data[idx]  ) << 16);
				encbuf |= 0x0000FF00 & ((unsigned long)(data[idx+1]) <<  8);

				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 18))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 12))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >>  6))];
				retbuff[offset++] = '=';
			}
			else if(remaining_bytes == 1)
			{
				encbuf  = 0x00FF0000 & ((unsigned long)(data[idx]  ) << 16);

				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 18))];
				retbuff[offset++] = encode_char[(char)(0x0000003F & (encbuf >> 12))];
				retbuff[offset++] = '=';
				retbuff[offset++] = '=';
			}
		}

		retbuff[offset] = 0;
		return retbuff;
	}

	unsigned char* base64_decode(const char* text, size_t* numbytes_returned)
	{
		if(		(text == NULL)
			||	(numbytes_returned == NULL) )
			return NULL;

		size_t length = strlen(text);
		size_t newlines = length/(76 + 1);
		size_t num_chars_to_decode = length - newlines;
		if(num_chars_to_decode%4)
			return NULL;

		size_t retbuff_size = (num_chars_to_decode/4)*3 - ((text[length - 1] == '=') ? 1 : 0) - ((text[length - 2] == '=') ? 1 : 0);
		unsigned char* retbuff = new unsigned char[retbuff_size];
		if(!retbuff)
			return NULL;

		*numbytes_returned = retbuff_size;
		size_t offset = 0;
		unsigned long decbuf = 0;
		char ch1, ch2, ch3, ch4;

		size_t idx = 0;
		while((idx + 4) <= length)
		{
			// skip the inserted newlines which occur every 76th character to be decoded
			if(		(idx >= 76)
				&&	(idx%(76 + 1) == 76) )
			{
				if(text[idx] == '\n')
				{
					idx++;
					continue;
				}
				else
					return NULL;
			}

			ch1 = decode_byte(text[idx]);
			if(ch1 == -1)
				return NULL;

			ch2 = decode_byte(text[idx + 1]);
			if(ch2 == -1)
				return NULL;

			if(		((idx + 2) == (length - 2))
				&&	(text[idx + 2] == '=') )
			{
				decbuf  = ((unsigned long)(ch1) & 0x0000003F) << 18;
				decbuf |= ((unsigned long)(ch2) & 0x0000003F) << 12;

				retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf >> 16));
				return retbuff;
			}

			ch3 = decode_byte(text[idx + 2]);
			if(ch3 == -1)
				return NULL;

			if(		((idx + 3) == (length - 1))
				&&	(text[idx + 3] == '=') )
			{
				decbuf  = ((unsigned long)(ch1) & 0x0000003F) << 18;
				decbuf |= ((unsigned long)(ch2) & 0x0000003F) << 12;
				decbuf |= ((unsigned long)(ch3) & 0x0000003F) <<  6;

				retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf >> 16));
				retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf >>  8));
				return retbuff;
			}

			ch4 = decode_byte(text[idx + 3]);
			if(ch4 == -1)
				return NULL;

			decbuf  = ((unsigned long)(ch1) & 0x0000003F) << 18;
			decbuf |= ((unsigned long)(ch2) & 0x0000003F) << 12;
			decbuf |= ((unsigned long)(ch3) & 0x0000003F) <<  6;
			decbuf |= ((unsigned long)(ch4) & 0x0000003F);

			retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf >> 16));
			retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf >>  8));
			retbuff[offset++] = (unsigned char)(0x000000FF & (decbuf));

			idx += 4;
		}

		return retbuff;
	}

}}
