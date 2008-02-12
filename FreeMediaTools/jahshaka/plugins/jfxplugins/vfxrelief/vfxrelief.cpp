/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxrelief.h"
#include <math.h>


///////////////////////////////////////////////////
// Mandatory routine used to return instance of
// plugin to calling application
jahPlugin* getPluginClass()
{
    return new MyPlugin();
}

//////////////////////////////////////////////////
// Plugin image subroutines start here
void    MyPlugin::initializePlugin(void)
{
    pluginFamily  = "JahFx";             //family of plugin
    pluginGroup   = "Effects";           //group of plugin
    pluginName    = "ReliefFx";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;      //class of plugin ie CPU, GPU, MeshFX

    QString guid = "08679271-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 1;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="S";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Releif Strength";
}

///////////////////////////////////////////////////
// The main plugin processing routine

void    MyPlugin::processImage(void)
{

    //create dynamic buffer
    QImage* buffer = new QImage( Width, Height, 32 );

    //set val to the number held in user feedback slider 1
    int val = (int)slider[1];
    if (val<=1) val =1;

    ////////////////////////////////////////////
    //main imageprocessing loop
    QRgb color;
    int xval,yval;

    for (int x=1; x < Width-1; x++)
    {
        for (int y=1; y < Height-1; y++)
        {
            //get the coords for the current pixel to be processed
            xval = x;
            yval = y;

            //get the new, processedcolor value from the plugin subroutine
            color = getRelief(xval,yval,val);

            //makes ure the values are within the images boundaries
            checkResBounds(xval,yval);

            //now set new buffer value to the new color..
            QRgb *e = (QRgb *)buffer->scanLine(yval)+xval;
            *e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
        }
    }

    //return the buffer
    *vfximage = buffer->copy();
    delete buffer;
}

///////////////////////////////////////////////////
// The releiffx image processing routine

QRgb MyPlugin::getRelief(int x, int y, int N)
{
    //set up variables here
    QRgb color;
    int r1,g1,b1;
    int r2,g2,b2;
    int r,g,b;

    //make sure values are in range
    checkResBounds(x,y);

    //get color at x/y
    QRgb *p = (QRgb *)vfximage->scanLine(y)+x;
    r1=qRed(*p); g1=qGreen(*p); b1=qBlue(*p);

    //offset pixel position
    x = x+2;
    y = y+2;

    //make sure new values are in range
    checkResBounds(x,y);

    //get color at offset position
    QRgb *q = (QRgb *)vfximage->scanLine(y)+x;
    r2=qRed(*q); g2=qGreen(*q); b2=qBlue(*q);

    //do the releif calculation
    r = r1 + (N/2 - r2); checkColorBounds(r);
    g = g1 + (N/2 - g2); checkColorBounds(g);
    b = b1 + (N/2 - b2); checkColorBounds(b);

    //set the color
    color = qRgb (r,g,b);

    //return the new color
    return color;
}








