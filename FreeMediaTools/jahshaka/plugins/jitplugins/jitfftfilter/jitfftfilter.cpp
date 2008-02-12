/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitfftfilter.h"

#define FFT_MAX

///////////////////////////////////////////////////
// Mandatory routine used to return instance of
// plugin to calling application


jahPlugin* getPluginClass()
{
    return new MyPlugin();
    //return new MyPlugin;
}

//////////////////////////////////////////////////
// Plugin image subroutines start here
void    MyPlugin::initializePlugin(void)
{
    pluginFamily  = "JitFx";               //family of plugin
    pluginGroup   = "Filter";              //group of plugin
    pluginName    = "FFTFilter";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;        //class of plugin ie CPU, GPU, MeshFX

    QString guid = "19abda3d-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 2;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="H";
    sliderlabel[1]="W";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="FFT Filter";

    //numoptions = 2;
    //button = new QString[numoptions];
    //button[0]="Min";
    //button[1]="Max";

}

void    MyPlugin::processImage(void)
{

    unsigned char            horizontal_blur;
    unsigned char            vertical_blur;
    COMPLEX_IMAGE             fft1; 
    COMPLEX_IMAGE            fft2;
    float                    real_1, imaginary_1;
    float                    real_2, imaginary_2;
    int                     row, column;
    Image_c                    pattern_image;
    Image_c                    picture_image;
    FFT_c                    fft_transform;
    int                     full_image_size;
    int                     original_width;
    int                     original_height;
    uint                    current_pixel;
    uint                    red;
    uint                    green;
    uint                    blue;
    QImage                  newimage; 
    int                     max_dimension;
    int                     use_row;
    int                     use_column;  

    newimage = vfximage->copy();

    //original_width =  (int)layers[1].textureData.width;
    //original_height = (int)layers[1].textureData.height;
    
    original_width =  (int)vfximage->width();
    original_height = (int)vfximage->height();
    
    // Force things to behave properly 
    max_dimension = (original_height > original_width) ? original_height : original_width;

    // Make the image square and round up to smallest  width 2^n and <= 4096   
    if (max_dimension > 2 && max_dimension < 4) { full_image_size = 4; }  
    else if (max_dimension > 4 && max_dimension < 8) { full_image_size = 8; }
    else if (max_dimension > 8 && max_dimension < 16) { full_image_size = 16; }
    else if (max_dimension > 16 && max_dimension < 32) { full_image_size =32; }
    else if (max_dimension > 32 && max_dimension < 64) { full_image_size = 64; }
    else if (max_dimension > 64 && max_dimension < 128) { full_image_size = 128; }
    else if (max_dimension > 128 && max_dimension < 256) { full_image_size = 256; }
    else if (max_dimension > 256 && max_dimension < 512) { full_image_size = 512; }
    else if (max_dimension > 512 && max_dimension < 1024) { full_image_size = 1024; }
    else if (max_dimension > 1024 && max_dimension < 2048) { full_image_size = 2048; }
    else if (max_dimension > 2048 && max_dimension < 4096) { full_image_size = 4096; }
    else if (max_dimension > 4096) { full_image_size = 4096; }
    else { full_image_size = max_dimension; }   // Don't change good values    
    
    // Values returned from user input sliders
    //horizontal_blur = abs((int)layers[effectobj].node->tx);
    horizontal_blur = abs((int)slider[1]);
    
    //vertical_blur = abs((int)layers[effectobj].node->ty);
    vertical_blur = abs((int)slider[2]);

    // Create a blur map based on the specified blur values
    pattern_image.createBlurMap(full_image_size, full_image_size, horizontal_blur, vertical_blur);

    // Create the picture image data structure
    picture_image.image_data = picture_image.allocateImage(full_image_size, full_image_size);
    picture_image.image_data->info->num_rows = full_image_size;
    picture_image.image_data->info->num_columns = full_image_size;
    picture_image.image_data->info->origin_x = 0;
    picture_image.image_data->info->origin_y = 0;
    
    printf("Bytes per scanline = %d\n", newimage.bytesPerLine());


    // Load the picture image data structure
    for (row = 0; row < full_image_size; row++) 
    {
        for (column = 0; column < full_image_size; column++) 
        {
            if ((row < original_height) && (column < original_width))
            {
                // Convert image to grey scale
                current_pixel = (uint)newimage.pixel(column, row);

                if (newimage.depth() > 8)
                {
                    red = (current_pixel >> 16) & 0xff;
                    green = (current_pixel >> 8) & 0xff;
                    blue = (current_pixel >> 0) & 0xff;
                    current_pixel =( (red + green + blue) / 3 );
                }

                picture_image.image_data->pixel_data[row][column] = (uchar)current_pixel;
            }
            else 
            {
                // Pad the edges with the last real pixel
                if (row >= original_height) 
                { 
                    use_row = original_height - 1; 
                }
                else
                {
                    use_row = row;
                }

                if (column >= original_width) 
                { 
                    use_column = original_width - 1; 
                }
                else
                {
                    use_column = column;
                }

                picture_image.image_data->pixel_data[row][column] = 
                    255;
                //picture_image.image_data->pixel_data[use_row][use_column];
            }
        }
    }

    if ((horizontal_blur == 0) && (vertical_blur == 0)) { goto math_done; }

    // Do the math
    fft_transform.image_fftoc(pattern_image.image_data, &fft1);
    fft_transform.normalize_set();
    fft_transform.image_fftoc(picture_image.image_data, &fft2);
    fft_transform.normalize_clear();

    for (row = 0; row < full_image_size; row++)
    {
        for (column = 0; column < full_image_size; column++)
        {
            real_1 = fft1[row][column];    
            imaginary_1 = fft1[row][column + full_image_size];
            real_2 = fft2[row][column];
            imaginary_2 = fft2[row][column + full_image_size];

            fft_transform.cprod(real_2, imaginary_2, &real_1, &imaginary_1);
            fft1[row][column] = real_1; 
            fft1[row][column + full_image_size] = imaginary_1;
        }
    }

    fft_transform.freecomplex(fft2); 
    fft2 = (COMPLEX_IMAGE)0;
    
    fft_transform.image_fftinvoc(fft1, &fft2);
    fft_transform.freecomplex(fft1); 
    fft1 = (COMPLEX_IMAGE)0;

    fft_transform.filt_toint(fft2, picture_image.image_data, 0);
    fft_transform.hack_reorder_quadrants(picture_image.image_data);

  math_done:
    // Write the data back
    for (row = 0; row < original_height; row++) 
    {
        for (column = 0; column < original_width; column++) 
        {
            
            // *((uchar*)newimage.scanLine(row) + column) = 
            // picture_image.image_data->pixel_data[row][column];
            
            current_pixel = (uint)picture_image.image_data->pixel_data[row][column];

            if (newimage.depth() > 8)
            {
                current_pixel = current_pixel | (current_pixel << 8);         // Copy to gree  
                current_pixel = current_pixel | (current_pixel << 8);         // Copy to blue
                current_pixel = current_pixel | 0xff000000;                   // Set alpha
            }

            newimage.setPixel(column, row, current_pixel);
        }
    }

    // Free up the allocated memory for the images
    pattern_image.freeImage(pattern_image.image_data); 
    picture_image.freeImage(picture_image.image_data);

	*vfximage = newimage;
	//pix = newimage;



}

/*
void MyPlugin::runEffect(int x, int y, int N, QImage * buffer, bool doMin)
{

}
*/
