/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxfisheye.h"

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
    pluginName    = "FisheyeFx";         //name of plugin
    pluginFamily  = "JahFx";             //family of plugin
    pluginGroup   = "Effects";           //group of plugin
    pluginClass   = JAH_CPU_PLUGIN;      //class of plugin ie CPU, GPU, MeshFX

    QString guid = "cea99ec0-e0b7-11d9-947b-00e08161165f";
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
    label[0]="Fisheye Fx";
}

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::processImage(void)
{

    int w,h;
    int xc,yc;
    double R;
 
    QImage buffer;              // this plugin uses a buffer
    buffer = vfximage->copy();  //since we dont operate directly on the pixels

    //source image size here
    h = vfximage->height();     //    int h = (int)layerlist->at(1)->thelayer->textureData.height;
    w = vfximage->width();      //    int w = (int)layerlist->at(1)->thelayer->textureData.width;

    //user feedback variables
    xc = (int) slider[1] + (w/2);      //xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    yc = (int) slider[2] + (h/2);      //yc = (int)layerlist->at(effectobj)->thelayer->node->ty + (h/2);
    R  = (int) slider[3] *  2.0;       //R = (int)layerlist->at(effectobj)->thelayer->node->rx * 2.0;

    double gamma = 2.0;
    float  p[2]; // = new double [2];
    int xp, yp;
    //int r,g,b;


    if (R==0) R=1;

    double dx,dy,radius,u,a;

    for (int x=0; x < w; x++)
    {
        for (int y=0; y < h; y++)
        {

            dx = x-xc; dy = y-yc;
            radius = sqrt(dx*dx + dy*dy);

            u = pow(radius,gamma)/R;
            a = atan2(dy,dx);

            p[0] = u * cos(a); p[1] = u * sin(a);
            xp = (int) p[0]+xc; yp = (int) p[1]+yc;

            if ( (xp < w) && (yp < h) && (xp >=0) && (yp >= 0))
            {
                uint *pnew = (uint *)buffer.scanLine(y) + x;
                uint *pold = (uint *)vfximage->scanLine(yp) + xp;

                *pnew = *pold;
            }
        }


    }

    *vfximage = buffer;

}

