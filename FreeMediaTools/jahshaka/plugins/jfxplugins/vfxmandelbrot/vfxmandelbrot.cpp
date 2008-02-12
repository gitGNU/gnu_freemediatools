/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxmandelbrot.h"
#include <math.h>
#include "mandelcolor.h"


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
    pluginFamily  = "JahFx";                 //family of plugin
    pluginGroup   = "Effects";               //group of plugin
    pluginName    = "MandelbrotFx";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;          //class of plugin ie CPU, GPU, MeshFX

    QString guid = "e9964d1e-e0b7-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 6;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X Scale";
    sliderlabel[1]="Y Scale";
    sliderlabel[2]=" ";
    sliderlabel[3]="X Position";
    sliderlabel[4]="Y Position";
    sliderlabel[5]=" ";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Mandelbrot Fx";

    //this should be a dropdown menu...
    numoptions = 4;
    button = new QString[numoptions];
    button[0]="Overlay";
    button[1]="Add";
    button[2]="Subtract";
    button[3]="Solid";

}


void    MyPlugin::processImage(void)
{
    //get the processing mode
    //no options is always quicker
    bool mandelmode = true;
    int  manstyle   = 1;

    if (option[1]) manstyle = 1;
    if (option[2]) manstyle = 2;
    if (option[3]) manstyle = 3;

    //used for solid
    if (option[4]) mandelmode = false;



    // Size of image to be generated
    // pulled from the size of the input image
    float NUMROWS = Height;
    float NUMCOLS = Width;

    float MINX, MINY, MAXX, MAXY = 0.0f;

    //works these out according to interface values
    getMandelbrotMinMax( MINX, MINY, MAXY, MAXX );

    // Increments for each screen coordinate
    float STEPX = ((MAXX - MINX) / NUMCOLS);
    float STEPY = ((MAXY - MINY) / NUMROWS);

    // Maximum number of iterations
    int RES = 50;

    float x, y;
    int col_count, row_count;
    int xval,yval;

    // Create some space on the stack for the results.
    //we are keeping this since it would be cool to buffer the entire operation
    //though we will have to make the array 2 dimensinal to do this
    //const int BUFROWS = NUMCOLS; //wont work without using a const
    int BUFROWS = (int)NUMCOLS; //wont work without using a const

    QRgb * buffer = new QRgb[BUFROWS];

    int manval;  //used to store the return val of the mandelbrot operations

    // Iterate over the rows
    for (row_count = 0; row_count < NUMROWS; row_count++)
    {
        // Iterate over the columns
        for (col_count = 0; col_count < NUMCOLS; col_count++)
        {
            // Calculate new (x,y) position to use
            x = MINX + (col_count * STEPX);
            y = MINY + (row_count * STEPY);

            // Calculate the Mandlebrot value for (x,y)
            manval = doMandelbrotCalcRes(x, y, RES);

            //this gets the existing pixels color
            xval = col_count;
            yval = row_count;

            //error checking
            checkResBounds(xval,yval);
            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

            //this calculates the new color for the array
            //this overrides the existing color
            if (mandelmode)
            {
                buffer[col_count] = getMandelbrotColor(manval,*p,manstyle);  //1,2 or 3 default is 2
            }


            //this overrides the existing color
            if (!mandelmode)
            {
                buffer[col_count] = (0xff << 24) | mcmap[manval];
            }
        }

        // Print out the current row
        // this makes it slower for now but its just like the Cn routine
        for (int mc=0; mc< NUMCOLS; mc++)
        {
            xval = mc;
            yval = row_count;

            checkResBounds(xval,yval);

            QRgb *e = (QRgb *)vfximage->scanLine(yval) + (xval);
            *e = buffer[mc];
        }
    }

}





bool MyPlugin::getMandelbrotMinMax( float& MINX, float& MINY, float& MAXY, float& MAXX )
{
    //bool returns if values have changed so we know we need to recompute!
    // Coordinates of fractal to be evaluated

    //default settings
    MINX = -1.5f;  MAXX = 1.5f;
    MINY = -1.25f; MAXY = 1.25f;

    //we scale first
    //float sx = -(float)layers[effectobj].node->sx;
    //float sy = -(float)layers[effectobj].node->sy;

    float sx = -(float)slider[1];
    float sy = -(float)slider[2];

    MINX  = MINX * (10+sx)/10;   MAXX  = MAXX * (10+sx)/10;
    MINY  = MINY * (10+sy)/10;   MAXY  = MAXY * (10+sy)/10;

    //then we do transformations
    //float tx = -(float)layers[effectobj].node->tx;
    //float ty = -(float)layers[effectobj].node->ty;

    float tx = -(float)slider[4];
    float ty = -(float)slider[5];

    MINX += tx/100; MAXX += tx/100;
    MINY += ty/100; MAXY += ty/100;

    return true;

}


QRgb MyPlugin::getMandelbrotColor( int manval, QRgb mypix, int mode )
{

    int r,g,b;

    switch(mode) {

        //default overlay mode
        case 1  : {
                    r=qRed(mypix);
                    g=qGreen(mypix);
                    b=qBlue(mypix);

                    if (manval==0) manval=1;

                    r = (short)( 255 * pow(  (r/255.0) ,manval ));  //manval was r,g,b
                    g = (short)( 255 * pow(  (g/255.0) ,manval ));
                    b = (short)( 255 * pow(  (b/255.0) ,manval ));

                    break; }

        case 2  : {
                    r=qRed(mypix)+manval;
                    g=qGreen(mypix)+manval;
                    b=qBlue(mypix)+manval;

                    break; }


        case 3  : {
                    r=qRed(mypix)-manval;
                    g=qGreen(mypix)-manval;
                    b=qBlue(mypix)-manval;

                    break; }

        default : {
					r = g = b = 0;
					break; }
    }

    //some saftey tests here to make sure colors dont go out of range
    checkColorBounds(r,g,b);

    QRgb newpix=qRgb(r,g,b);
    return newpix;

}

// Evaluate the termination condition
int MyPlugin::doMandelbrotTerminate( float x, float y ) {

    return (x*x + y*y) > 4.0f;

}

// Calculate the result for the a given (x,y) position
int MyPlugin::doMandelbrotCalcRes(float x, float y, int res ) {

    int result;
    int turnedon;
    int i;
    float tx;
    float xcalc, ycalc;

    // Initialise cumulative variables
    xcalc = x;
    ycalc = y;

    result = 0;

    // Use a variable to determine if we're
    // carrying out useful work
    turnedon = 2;

    // Loop up to res times for each position
    // res determines the quality of the picture;
    // more iterations, a better picture, but slower
    // to generate.
    for (i = 0; i < res; i++)
    {
        // Only calculate if we're carrying out useful work
        if (turnedon)
        {
            // Should we terminate or continue?
            if (doMandelbrotTerminate(xcalc, ycalc))
            {
                // Final result is number of iterations
                // it took to achieve termination
                result = i + 1;
                turnedon = 0;
            }
            else
            {
                // Calculate next iteration
                tx    = (xcalc * xcalc) - (ycalc * ycalc) + x;
                ycalc = (2.0f * xcalc * ycalc) + y;
                xcalc = tx;
            }
        }
    }

    return result;


}






