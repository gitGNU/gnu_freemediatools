/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitmmsefilter.h"


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
    pluginFamily  = "JitFx";             //family of plugin
    pluginGroup   = "Filter";            //group of plugin
    pluginName    = "MMSEFilter";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;      //class of plugin ie CPU, GPU, MeshFX

    QString guid = "3307fd8a-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 2;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="V";
    sliderlabel[1]="F";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Adaptive MMSE Filter";

}

void    MyPlugin::processImage(void)
{

    QImage* buffer = new QImage( Width, Height, 32 );

   //we can play with this value
    int N = 5 + (int)slider[1];
    int Nvar = 5 + (int)slider[2];

    if (N < 1)
    {
        N = 1;
    }

    if (N > 20)
    {
        N = 20;
    }

    if (Nvar < 1)
    {
        Nvar = 1;
    }


    for (int x=N/2; x < Width-N/2; x++)
    {
        for (int y=N/2; y < Height-N/2; y++)
        {
            runEffect(x, y, N, Nvar, buffer);
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}

void MyPlugin::runEffect(int x, int y, int N, int NVAR, QImage * buffer)
{
    //debug("running effect");

    int xval,yval;
    int loop;

    int color[3];
    int srcColor[3];

    int sum[3], sum1[3];
    sum[0]  = sum[1]  = sum[2]  = 0;
    sum1[0] = sum1[1] = sum1[2] = 0;

    float fsecond[3], fvar[3], fmean[3];

    int total = 0;

    ////////////////////////////////////////////
    // build the tables for the effect

    for (int y1=-N/2; y1<=N/2; y1++)
    {
        for(int x1=-N/2; x1<=N/2; x1++)
        {
            xval = x+x1;
            yval = y+y1;

            //error checking
            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);
            srcColor[0] =qRed(*p);
            srcColor[1] =qGreen(*p);
            srcColor[2] =qBlue(*p);

            for(loop=0; loop<=2; loop++)
            {
                sum[loop]    = sum[loop]  + srcColor[loop];
                sum1[loop]   = sum1[loop] + srcColor[loop] * srcColor[loop];
            }

            total +=1;
        }
    }

    for(loop=0; loop<=2; loop++)
    {
        fsecond[loop] = (float)sum1[loop]/(float)total;
        fmean[loop]   = (float)sum[loop] /(float)total;
        fvar[loop]    = fsecond[loop] - fmean[loop]*fmean[loop];
    }

    //////////////////////////////////////////////
    //get the current pixel

    xval = x;
    yval = y;
    checkResBounds(xval,yval);
    QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

    srcColor[0] =qRed(*p);
    srcColor[1] =qGreen(*p);
    srcColor[2] =qBlue(*p);

    //////////////////////////////////////////////
    //get the new pixel

    for(loop=0; loop<=2; loop++)
    {
        if (fvar[loop]==0.0)
            color[loop] = (int)(fmean[loop]+0.5);
        else
        {
            color[loop] =(int)((1-NVAR/NVAR) * srcColor[loop] + NVAR/fvar[loop]*fmean[loop]+0.5);
        }
    }

    ///////////////////////////////////////////////
    //now write out the pixel
    checkColorBounds(color[0],color[1],color[2]);

    QRgb *e = (QRgb *)buffer->scanLine(y) + (x);
    *e = qRgb(color[0],color[1],color[2]);

}
