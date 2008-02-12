/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxswirl.h"
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
    pluginName    = "SwirlFx";         //name of plugin
    pluginFamily  = "JahFx";           //family of plugin
    pluginGroup   = "Color";           //group of plugin
    pluginClass   = JAH_CPU_PLUGIN;    //class of plugin ie CPU, GPU, MeshFX

    QString guid = "120d96a8-e0b8-11d9-947b-00e08161165f";
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
    label[0]="Swirl Fx";

}

void    MyPlugin::processImage(void)
{

    int xc,yc;
    double R;
 
    QImage buffer;              // this plugin uses a static buffer
    buffer = vfximage->copy();  //since we dont operate directly on the pixels


    //user feedback variables
    xc = (int) slider[1] + (Width/2);      //xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    yc = (int) slider[2] + (Height/2);      //yc = (int)layerlist->at(effectobj)->thelayer->node->ty + (h/2);
    R  = (int) slider[3] *  2.0;       //R = (int)layerlist->at(effectobj)->thelayer->node->rx * 2.0;

    double gamma = 2.0;
    float  p[2]; // = new double [2];
    int xp, yp;
    //int r,g,b;

    double PI = 3.14159265358979323846;

    if (R==0) R=1;

    double dx,dy,radius,u,a;

    for (int x=0; x < Width; x++)
    {
        for (int y=0; y < Height; y++)
        {

            //////////////////////////////////////
            // sqrt
            dx = x-xc;
            dy = y-yc;
            radius = sqrt(dx*dx + dy*dy);

            u = pow(radius,gamma)/R;
            a = (180/PI)*atan2(dy,dx);
            a = a + radius;
            a = a * PI/180;

            //p[0] = radius * cos(a);
            //p[1] = radius * sin(a);
            p[0] = u * cos(a);
            p[1] = u * sin(a);

            xp = (int) p[0]+xc;
            yp = (int) p[1]+yc;


            if ( (xp < Width) && (yp < Height) && (xp >=0) && (yp >= 0))
            {

                uint *pnew = (uint *)buffer.scanLine(y) + x;
                uint *pold = (uint *)vfximage->scanLine(yp) + xp;

                *pnew = *pold;

            }
        }


    }

    *vfximage = buffer;

}

/*
    //this is the built in polar effect
    //refreence the one above and either create a new one or merge this in

    int w = (int)layerlist->at(1)->thelayer->textureData.width;
    int h = (int)layerlist->at(1)->thelayer->textureData.height;

    int xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    int yc = (int)layerlist->at(effectobj)->thelayer->node->ty + (h/2);

    QImage newimage;   newimage = pix.copy();

    double gamma = 2.0;
    float  p[2]; // = new double [2];
    int xp, yp;

    double R = layerlist->at(effectobj)->thelayer->node->rx;

    if (R==0) R=1;

    double dx,dy,radius,u,a;

    //radius affects x,y and dx,dy here need some work
    for (int x=0; x < w; x++)
    {
        for (int y=0; y < h; y++)
        {

            dx = x-xc;
            dy = y-yc;
            // radius =R;
            radius = sqrt(dx*dx + dy*dy)-(R+100);
            if (radius<=0) radius = 0;

            u = pow(radius,gamma)/R;
            a = (180/PI)*atan2(dy,dx);
            a = a + radius;
            a = a * PI/180;

            p[0] = radius * cos(a);
            p[1] = radius * sin(a);
            xp = (int) p[0]+xc;
            yp = (int) p[1]+yc;

            if ( (xp < w) && (yp < h) && (xp >=0) && (yp >= 0))
            {
                uint *pnew = (uint *)newimage.scanLine(y) + x;
                uint *pold = (uint *)pix.scanLine(yp) + xp;
                *pnew = *pold;
            }
        }
    }
    pix = newimage;

*/

/*
//the radial line effect
//use this to crate a new plugin called radial lines
void GLEffect::doRadialLines(QImage& pix ) {

    int h = (int)layerlist->at(1)->thelayer->textureData.height;
    int w = (int)layerlist->at(1)->thelayer->textureData.width;

    QImage newimage;   newimage = pix.copy();

    int xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    int yc = (int)layerlist->at(effectobj)->thelayer->node->ty + (h/2);

    double gamma = 2.0;
    float  p[2]; // = new double [2];
    int xp, yp;

    double R = (int)layerlist->at(effectobj)->thelayer->node->rx;

    if (R==0) R=1;
    double dx,dy,radius,u,a;

    //radius affects x,y and dx,dy here need some work
    for (int x=0; x < w; x++) {
        for (int y=0; y < h; y++) {

            dx = x-xc;
            dy = y-yc;
            radius =R; if (radius<=0) radius = 0;
            //radius = sqrt(dx*dx + dy*dy)-(R+100);
            //    if (radius<=0) radius = 0;

            u = pow(radius,gamma)/R;
            a = (180/PI)*atan2(dy,dx);
            a = a + radius;
            a = a * PI/180;

            p[0] = radius * cos(a);
            p[1] = radius * sin(a);
            xp = (int) p[0]+xc;
            yp = (int) p[1]+yc;

            if ( (xp < w) && (yp < h) && (xp >=0) && (yp >= 0))
            {
				uint *pnew = (uint *)newimage.scanLine(y) + x;
				uint *pold = (uint *)pix.scanLine(yp) + xp;
				*pnew = *pold;
				//     newimage.setPixel(x,y, ( pix.pixel(xp,yp) ) );
            }
        }
    }
    pix = newimage;

}
*/

