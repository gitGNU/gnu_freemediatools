/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "csmandelbrot.h"
#include <math.h>

/////////////////////////////////////////////////////////
// external linked clearspeed routine
extern "C" void mandel_main(float minx, float miny, float maxx, float maxy, int numrows, int numcolumns, unsigned int res);

////////////////////////////////////////////////////////
// clearspeed settings

//#define    NUMROWS    1024
//#define    NUMCOLS    1024

extern struct pixel_t               coordinate_out_buffer[1024][1024];
extern char                         char_out_buffer[1024][1024];

struct pixel_t
{
	int        x;
	int        y;
};

///////////////////////////////////////////////////
// Mandatory routine used to return instance of
// plugin to calling application


namespace PLUGIN
{
  namespace CSMANDELBROT {


jahPlugin* getPluginClass()
{
    return new MyPlugin();
    //return new MyPlugin;
}

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::initializePlugin(void)
{
    pluginName    = "CSmandelbrot";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;                  //class of plugin ie CPU, GPU, MeshFX

    QString guid = "d898c2d8-dc36-11d9-9475-00e08161165f";
    setGuid(guid);

    //we are using 1 slider
    usesliders = true;
    numsliders = 3;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X";
    sliderlabel[1]="Y";
    sliderlabel[2]="R";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Mandelbrot Fx";
}

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::processImage(void)
{


    QImage newimage;
    newimage = vfximage->copy();
    

//    uint* pnew = (uint *)newimage.scanLine(row) + column;
//    uint* pold = (uint *)vfximage->scanLine(coordinate_out_buffer[row][column].y) + coordinate_out_buffer[row][column].x;


    // Size of image to be generated
    // pulled from the size of the input image
    float NUMROWS = Height;
    float NUMCOLS = Width;

    float MINX, MINY, MAXX, MAXY = 0.0f;

    //works these out according to interface values
    getMandelbrotMinMax( MINX, MINY, MAXY, MAXX );

    // Maximum number of iterations
    int RES = 50;

    int col_count, row_count;

    // Create some space on the stack for the results.
    //we are keeping this since it would be cool to buffer the entire operation
    //though we will have to make the array 2 dimensinal to do this
    //const int BUFROWS = NUMCOLS; //wont work without using a const
    int BUFROWS = (int)NUMCOLS; //wont work without using a const

    QRgb * buffer = new QRgb[BUFROWS];

    int manval;  //used to store the return val of the mandelbrot operations
    QRgb mypix;  //this is used to check and set pixel colors

    
    // All of the computation is done using CS routines
    // This generates a map char_out_buffer of the mandelbrot, which is merged in with the
    // effects in the loops below
    mandel_main(MINX, MINY, MAXX, MAXY, (int)NUMROWS, (int)NUMCOLS, RES);


    // Iterate over the rows
    for (row_count = 0; row_count < NUMROWS; row_count++)
    {
        // Iterate over the columns
        for (col_count = 0; col_count < NUMCOLS; col_count++)
        {
            // Calculate new (x,y) position to use
            //x = MINX + (col_count * STEPX);
            //y = MINY + (row_count * STEPY);

            // Calculate the Mandlebrot value for (x,y)

            manval = char_out_buffer[row_count][col_count];

            //this gets the existing pixels color
            mypix = newimage.pixel(col_count, row_count);

            //this calculates the new color for the array
            buffer[col_count] = getMandelbrotColor(manval, mypix, 2);

            // This gets a pixel from the color map
            //buffer[col_count] = (0xff << 24) | mcmap[manval];

        }

        // Print out the current row
        // this makes it slower for now but its just like the Cn routine
        for (int mc=0; mc < NUMCOLS; mc++)
        { 
            newimage.setPixel(mc, row_count, buffer[mc]);
        }
    }

    //return image
    *vfximage = newimage;

}

}
}
