/*******************************************************************************
**
** The header file for the gpumathlib class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc..
** Released under the GNU General Public License
**
*******************************************************************************/

#include "gpumathlib.h"
//#include <qimage.h>


// build permuation/gradient table for noise
void initVertexNoiseConstants(int table_size, int*& permutation_table, float4*& gradient_table, int gradient_size)
{
    int                     i;
    int                     random_index;
    int                     temp;

    permutation_table = new int[gradient_size];         // permutation table
    gradient_table = new float4[gradient_size];             // gradient table

    // initalize random gradients
    for(i = 0; i < table_size; i++)
    {
        permutation_table[i] = i;

        gradient_table[i].x = sfrand();
        gradient_table[i].y = sfrand();
        gradient_table[i].z = sfrand();

        normalize3f(gradient_table[i].x, gradient_table[i].y, gradient_table[i].z);
    }

    // initialize permutation table (random shuffle)
    for(i = 0; i < table_size; i++)
    {
        random_index = (rand() >> 4) % table_size;
        temp = permutation_table[i];
        permutation_table[i] = permutation_table[random_index];
        permutation_table[random_index] = temp;

        gradient_table[i].w = (float)permutation_table[i];

        gradient_table[(2 * table_size) - 1 - i].x = gradient_table[i].x;
        gradient_table[(2 * table_size) - 1 - i].y = gradient_table[i].y;
        gradient_table[(2 * table_size) - 1 - i].z = gradient_table[i].z;
        gradient_table[(2 * table_size) - 1 - i].w = gradient_table[i].w;
    }
}

void createRandomTexture( int width, int height, GLuint & random_texture )
{
    GLubyte* texels = new GLubyte[width * height * 4];

    int                         random;

    for( GLint y = 0; y < height; y++ )
    {
        for( GLint x = 0; x < width; x++ )
        {
            random = rand();
            texels[ (y * width * 4) + (x * 4) + 0 ] = random % 256;     // red
            texels[ (y * width * 4) + (x * 4) + 1 ] = random % 256;     // green
            texels[ (y * width * 4) + (x * 4) + 2 ] = random % 256;     // blue
            texels[ y*width*4 + x*4 + 3 ] = 255;                        // alpha
        }
    }

    random_texture = 0;
    glGenTextures( 1, &random_texture );
    glBindTexture( GL_TEXTURE_2D, random_texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels );

    delete [] texels;
}


void createContrastTexture( int width, int height, double , double exponent,GLuint & contrast_texture )
{
    /* A white texture is generated. Afterwards black sprinkles are added.
    * These are shifted towards the 0 value of the y axis to archieve sort of a gradient.
    */
    GLubyte* texels = new GLubyte[width * height * 4];

    int                             x;
    int                             y;

    for( y = 0; y < height; y++ )
    {
        for( x = 0; x < width; x++ )
        {
            texels[ (y * width * 4) + (x * 4) + 0 ] = 255;              // red
            texels[ (y * width * 4) + (x * 4) + 1 ] = 255;              // green
            texels[ (y * width * 4) + (x * 4) + 2 ] = 255;              // blue
            texels[ (y * width * 4) + (x * 4) + 3 ] = 255;              // alpha
        }
    }

    for( int i = 0; i < (width * height); i++ )
    {
        x = (int)( ((double)rand()/(double)RAND_MAX) * (double)(width - 1.0) );
        y = (int)( (pow((double)rand() / (double)RAND_MAX, fabs(exponent))) * (double)(height - 1.0) );

        texels[ (y * width * 4) + (4 * x) + 0 ] = 0;
        texels[ (y * width * 4) + (4 * x) + 1 ] = 0;
        texels[ (y * width * 4) + (4 * x) + 2 ] = 0;
    }

    contrast_texture = 0;
    glGenTextures( 1, &contrast_texture );
    glBindTexture( GL_TEXTURE_2D, contrast_texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels );

    delete [] texels;
}

void
getRectangleSubImage( unsigned char* dest, unsigned char* source, 
                     int x_offset, int y_offset,
                     int sub_width, int sub_height,
                     int full_width, int full_height, int texture_width, bool invert )
{
    unsigned int* dest_pointer = (unsigned int*)dest;

    if (invert)
    {
        int start_row = full_height - 1 - y_offset ;

        for (int row = start_row; row > start_row - sub_height; row--)
        {
            unsigned int* source_pointer = (unsigned int*)( source + (row * full_width * 4) + (x_offset * 4) );

            for (int column = 0; column < sub_width; column++)
            {
                *dest_pointer++ = *source_pointer++;
            }
            
            dest_pointer += texture_width - sub_width;
        }
    }
    else
    {
        for (int row = y_offset; row < y_offset + sub_height; row++)
        {
            unsigned int* source_pointer = (unsigned int*)( source + (row * full_width * 4) + (x_offset * 4) );

            for (int column = 0; column < sub_width; column++)
            {
                *dest_pointer++ = *source_pointer++;
            }
            
            dest_pointer += texture_width - sub_width;
        }
    }
}



void
createEmpty2DTexture(GLuint* texture_id_ptr, GLenum format, int width, int height)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, texture_id_ptr);
    glBindTexture(GL_TEXTURE_2D, *texture_id_ptr);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    check_gl();
    getPowerOfTwoTextureSize(width, height, width, height);
    glTexImage2D( GL_TEXTURE_2D, 0, 4, width, height, 0, format, GL_UNSIGNED_BYTE, NULL );
    check_gl();
}

bool
getPowerOfTwoTextureSize(int& texture_width, int& texture_height, int width, int height)
{
    if ( Globals::getGlVersion().find("2.0") != std::string::npos )
    {
        texture_width = width;
        texture_height = height;
        return true;
    }

    bool width_found = false;
    bool height_found = false;

    for (int i = 2; i <= 0x2000; i *= 2)
    {
        if ( !width_found && (width <= i) )
        {
            texture_width = i;
            width_found = true;
        }

        if ( !height_found && (height <= i) )
        {
            texture_height = i;
            height_found = true;
        }

        if (width_found && height_found)
        {
            return true;
        }
    }

    return false;
}


void 
jahstd::glGetTextureSubImage(int x_offset, int y_offset, int buffer_width, int buffer_height, int texture_width, 
                             int texture_height, unsigned char* buffer)
{
    int buffer_size = (texture_width + 1) * (texture_height + 1) * 10;
    unsigned int* texture_buffer = new unsigned int[buffer_size];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)texture_buffer);

    getRectangleSubImage( buffer, (unsigned char*)texture_buffer, x_offset, y_offset, buffer_width, buffer_height,
                        texture_width, texture_height, buffer_width);

    delete [] texture_buffer;
}
