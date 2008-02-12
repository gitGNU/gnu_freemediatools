/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxmosaic.h"
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
    pluginFamily  = "JahFx";             //family of plugin
    pluginGroup   = "Effects";           //group of plugin
    pluginName    = "MosaicFx";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;      //class of plugin ie CPU, GPU, MeshFX

    QString guid = "fada94ea-e0b7-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 1;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="C";

    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Mosaic Fx";

}

void    MyPlugin::processImage(void)
{

    int w,h;
    //int xc,yc;

    //int r,g,b;

    //source image size here
    h = vfximage->height();     //    int h = (int)layerlist->at(1)->thelayer->textureData.height;
    w = vfximage->width();      //    int w = (int)layerlist->at(1)->thelayer->textureData.width;


    //sample area here
    //int N = 5;
    //int N;
    int val;

    val = (int)slider[1];
    if (val<1) val=1;

    //val = 5;

    ////////////////////////////////////////////
    //main imageprocessing loop

    jahMosaic (val,w,h);

}




void  MyPlugin::jahMosaic  (int start_level, int Xdim, int Ydim)
{
    //auto int size, size2;  /* Current Region Size & size/2 */
    int size, size2;  /* Current Region Size & size/2 */
    int Ix, Iy;       /* Image Space Indices */

    //create dynamic buffer
    QImage* buffer = new QImage( Xdim, Ydim, 32 );
    QRgb color;

    //size = 1 << start_level;
    size = start_level;

    for (Iy=0;  Iy <= Ydim;  Iy += size)
    {   for (Ix=0;  Ix <= Xdim;  Ix += size)
        {

            color = getColor  (Ix, Iy);

            setRectangle (Ix, Iy, size, size, color, buffer);

        }
    }

    /* Sampling and Gridding Loop */

    size2 = size / 2;

    *vfximage = buffer->copy();

    while (size > 1)
    {
        for (Iy=0;  Iy <= Ydim;  Iy += size)
        {   for (Ix=0;  Ix <= Xdim;  Ix += size)
            {
                color = getColor  (Ix, Iy + size2);
                setRectangle (Ix, Iy+size2, size2, size2, color, buffer);

                color = getColor  (Ix + size2, Iy + size2);
                setRectangle (Ix + size2, Iy + size2, size2, size2, color, buffer);

                color = getColor  (Ix + size2, Iy);
                setRectangle (Ix + size2, Iy, size2, size2, color, buffer);

            }
        }

        size  = size2;
        size2 = size2 / 2;
    }


      *vfximage = buffer->copy();
      delete buffer;
}


void MyPlugin::setRectangle (int x, int y, int w, int h, QRgb color, QImage *image)
{
    //Rectangle (Ix, Iy, size, size);
    for (int yy = y; yy<= y+h; yy++)
    {
        for (int xx=x; xx<=x+w; xx++)
        {
            int xval = xx;
            int yval = yy;

            checkResBounds(xval,yval);

            QRgb *e = (QRgb *)image->scanLine(yval)+xval;
            *e = qRgb(qRed(color), qGreen(color), qBlue(color));

        }
    }
}

QRgb MyPlugin::getColor(int x, int y)
{
    int r,g,b;

    checkResBounds(x,y);

    //return value
    QRgb *p = (QRgb *)vfximage->scanLine(y)+x;
    r=qRed(*p); g=qGreen(*p); b=qBlue(*p);

    checkColorBounds(r);
    checkColorBounds(g);
    checkColorBounds(b);

    QRgb color = qRgb (r,g,b);
    return color;
}

