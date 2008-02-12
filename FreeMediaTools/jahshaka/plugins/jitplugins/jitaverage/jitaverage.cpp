/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitaverage.h"
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
    pluginFamily  = "JitFx";            //family of plugin
    pluginGroup   = "Filter";           //group of plugin
    pluginName    = "Average";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "19abda3c-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 1;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="A";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Average Area";

}

void    MyPlugin::processImage(void)
{
    int w,h;

    //source image size here
    h = vfximage->height();     //    int h = (int)layerlist->at(1)->thelayer->textureData.height;
    w = vfximage->width();      //    int w = (int)layerlist->at(1)->thelayer->textureData.width;

    //create dynamic buffer
    QImage* buffer = new QImage( w, h, 32 );

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

    for (int x=1; x < w-1; x++)
    {
        for (int y=1; y < h-1; y++)
        {
            //first get the average color
            color = getAverage(x,y,N);

            checkResBounds(x,y);

            //now set new buffer value to average..
            QRgb *e = (QRgb *)buffer->scanLine(y)+x;
            *e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}

QRgb MyPlugin::getAverage(int x, int y, int N)
{
//debug("get average");
    int r,g,b;
    int ra,ga,ba;
  
    int counter;
    QRgb color;

    ra = ga = ba = 0;
    counter = 0;

    //we need to find the average image color from
    //x-n,y-n to x+n,y+n
    for (int xl = x-N; xl<=x+N; xl++)
    {
        for (int yl = y-N; yl<=y+N; yl++)
        {
            counter +=1;

            int yval = yl;
            int xval = xl;

            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval)+xval;
            r=qRed(*p); g=qGreen(*p); b=qBlue(*p);

            ra = ra+r;
            ga = ga+g;
            ba = ba+b;
        }
    }


    color = qRgb (ra/counter,ga/counter,ba/counter);

    return color;
}








