/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxbathroom.h"
#include <math.h>


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
    pluginFamily  = "JahFx";            //family of plugin
    pluginGroup   = "Effects";          //group of plugin
    pluginName    = "BathroomFx";       //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "8a9ec23c-e0b7-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 1;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="V";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Bathroom Fx";
}

void    MyPlugin::processImage(void)
{

    //create dynamic buffer
    QImage* buffer = new QImage( Width, Height, 32 );

    //sample area here
    //int N = 5;
    int N;
    int val;

    val = (int)slider[1];

    if (val>0)
        N  = (int) (val/10);      //xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    else
        N = 0;

    QRgb color;

    ////////////////////////////////////////////
    //main imageprocessing loop

    for (int x=1; x < Width-1; x++)
    {
        for (int y=1; y < Height-1; y++)
        {

            color = getBathroom(x,y,N);

            checkResBounds(x,y);

            //now set new buffer value to average..
            QRgb *e = (QRgb *)buffer->scanLine(y)+x;
            *e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}



QRgb MyPlugin::getBathroom(int x, int y, int N)
{
//debug("get average");

    int r,g,b;
    int xval, yval;
    int Nval;
    //double a,ra;

    QRgb color;

    //bounds checking on the N value here
    if (N==0) N=1;

    //bathroom 1
    Nval = N*2;
    xval = x + (x%Nval)-N;
    yval = y;


    //ok output now
    checkResBounds(xval,yval);

    QRgb *p = (QRgb *)vfximage->scanLine(yval)+xval;
    r=qRed(*p); g=qGreen(*p); b=qBlue(*p);

    checkColorBounds(r,g,b);

    color = qRgb (r,g,b);
    return color;
}









