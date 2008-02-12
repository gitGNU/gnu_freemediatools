/*
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "global.h"

#include <qimage.h>
#include "qimagefilter.h"


EXTERN_C void store_qimage(char* outname, unsigned char* src[], int offset, int incr, int height)
{
    Qimage_filter_c                qimage_filter;

    qimage_filter.storeQimage(outname, src, offset, incr, height);
}


void Qimage_filter_c::storeQimage(char* outname, unsigned char* src[], int offset, int incr, int height)
{
    int                                 i, j;
    int                                 y, u, v, r, g, b;
    int                                 crv, cbu, cgu, cgv;
    unsigned char                       *py, *pu, *pv;
    static unsigned char                *u422, *v422, *u444, *v444;
	unsigned int						current_pixel;
	unsigned int						buffer_offset;

    if (chroma_format == CHROMA444)
    {
        u444 = src[1];
        v444 = src[2];
    }
    else
    {
        if (!u444)
        {
            if (chroma_format == CHROMA420)
            {
                u422 = new unsigned char[Coded_Picture_Width >> 1];
                v422 = new unsigned char[Coded_Picture_Width >> 1];
            }

            u444 = new unsigned char[Coded_Picture_Width];
            v444 = new unsigned char[Coded_Picture_Width];
        }

        if (chroma_format==CHROMA420)
        {
            conv420to422(src[1], u422);
            conv420to422(src[2], v422);
            conv422to444(u422, u444);
            conv422to444(v422, v444);
        }
        else
        {
            conv422to444(src[1], u444);
            conv422to444(src[2], v444);
        }
    }


    crv = Inverse_Table_6_9[matrix_coefficients][0];
    cbu = Inverse_Table_6_9[matrix_coefficients][1];
    cgu = Inverse_Table_6_9[matrix_coefficients][2];
    cgv = Inverse_Table_6_9[matrix_coefficients][3];
  
    for (i = 0; i < height; i++)
    {
        py = src[0] + offset + (incr * i);
        pu = u444 + offset + (incr * i);
        pv = v444 + offset + (incr * i);

        for (j = 0; j < horizontal_size; j++)
        {
            u = *pu++ - 128;
            v = *pv++ - 128;
            y = 76309 * (*py++ - 16); 
            r = Clip[(y + crv*v + 32768)>>16];
            g = Clip[(y - cgu*u - cgv*v + 32768)>>16];
            b = Clip[(y + cbu*u + 32786)>>16];

			current_pixel = (0xff << 24) | (r << 16) | (g << 8) | (b << 0);
			//image.setPixel(j, i, current_pixel);
			buffer_offset = ((i * horizontal_size) + j) * 4;
			// Do it this way to avoid big/little endian issues
			*(unsigned int*)&mpeg_data.pixel_buffer[buffer_offset] = current_pixel;
        }
    }
}
*/
