/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitpseudocolor.h"


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
    pluginFamily  = "JitFx";              //family of plugin
    pluginGroup   = "Color";              //group of plugin
    pluginName    = "PseudoColor";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;       //class of plugin ie CPU, GPU, MeshFX

    QString guid = "3c51060c-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 2;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="L";
    sliderlabel[1]="H";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Color Range";

}

void    MyPlugin::processImage(void)
{

    int w,h;

    int r,g,b;

    //source image size here
    h = vfximage->height();     //    int h = (int)layerlist->at(1)->thelayer->textureData.height;
    w = vfximage->width();      //    int w = (int)layerlist->at(1)->thelayer->textureData.width;

    //create dynamic buffer
    QImage* buffer = new QImage( w, h, 32 );


    int x1,y1;
    int mask1[3][3];
    int mask2[3][3];
    int GX,GY, EDGE;

    int pixval;


    mask1[0][0] = -1; mask1[1][0] = -2; mask1[2][0] = -1;
    mask1[0][1] =  0; mask1[1][1] =  0; mask1[2][1] =  0;
    mask1[0][2] =  1; mask1[1][2] =  2; mask1[2][2] =  1;

    mask2[0][0] = -1; mask2[1][0] =  0; mask2[2][0] =  1;
    mask2[0][1] = -2; mask2[1][1] =  0; mask2[2][1] =  2;
    mask2[0][2] = 11; mask2[1][2] =  0; mask2[2][2] =  1;

    int T1,T2;
    bool userange,therange;

    T1  = (int) slider[1];      //xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);
    T2  = (int) slider[2];      //xc = (int)layerlist->at(effectobj)->thelayer->node->tx + (w/2);

    if (T2<=T1)
    {
        userange = false;
    }
    else
    {
        userange = true;
    }

    ////////////////////////////////////////////
    //main imageprocessing loop

    for (int y=1; y < h-1; y++)
    {
        for (int x=1; x < w-1; x++)
        {
            GX=0; GY=0;

            for (y1=-1; y1<=1; y1++)
            {
                for(x1=-1; x1<=1; x1++)
                {
                    //assumes image is grey so all values are the same
                    QRgb *p = (QRgb *)vfximage->scanLine(y+y1)+(x+x1);
                    pixval = qRed(*p);

                    GX+= mask1[x1+1][y1+1]* pixval;
                    GY+= mask2[x1+1][y1+1]* pixval;
                }

                EDGE=abs(GX)+abs(GY);


                //this just figures out if we are using 1 or 2 sliders for range...
                therange = false;

                if (!userange)
                {
                    if (EDGE>T1) therange = true;
                }
                else
                {
                    if (EDGE>T1 && EDGE<T2) therange = true;
                }


                //output image data here
                if (therange)
                {
                    QRgb *p = (QRgb *)vfximage->scanLine(y)+x;
                    r=qRed(*p); g=qGreen(*p); b=qBlue(*p);

                    QRgb *e = (QRgb *)buffer->scanLine(y)+x;

                    *e = qRgb(255, g, b);  //was 255,0,0
                }
                else
                {
                    uint *pnew = (uint *)buffer->scanLine(y)+x;
                    uint *pold = (uint *)vfximage->scanLine(y)+x;

                    *pnew = *pold;
                }

            }
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}
