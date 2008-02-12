/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitminmaxfilter.h"


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
    pluginFamily  = "JitFx";               //family of plugin
    pluginGroup   = "Filter";              //group of plugin
    pluginName    = "MinMaxFilter";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;        //class of plugin ie CPU, GPU, MeshFX

    QString guid = "29ba3055-e0b8-11d9-947b-00e08161165f";
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
    label[0]="Min Max Filter";

    numoptions = 2;
    button = new QString[numoptions];
    button[0]="Min";
    button[1]="Max";

}

void    MyPlugin::processImage(void)
{

    QImage* buffer = new QImage( Width, Height, 32 );

   //we can play with this value
    int N=5+(int)slider[1];

    if (N < 1)
    {
        N = 1;
    }

    if (N>11) N=11;

    //true defaults to Min
    bool doMinOrMax = true;

    if (option[1]) doMinOrMax = true;
    if (option[2]) doMinOrMax = false;

    //the main fx loop
    for (int x=N/2; x < Width-N/2; x++)
    {
        for (int y=N/2; y < Height-N/2; y++)
        {

            runEffect(x, y, N, buffer,doMinOrMax);

        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}

void MyPlugin::runEffect(int x, int y, int N, QImage * buffer, bool doMin)
{
    //debug("running effect");

    int I,J;
    int color[3];

    static int rgbval[3][11][11];

    int xval,yval;

    if (doMin)
    {
        color[0] = color[1] = color[2] = 255;
    }
    else
    {
        color[0] = color[1] = color[2] = 0;
    }

    for (J=-N/2; J<=N/2; J++)
    {
        for(I=-N/2; I<=N/2; I++)
        {

            xval = x+I;
            yval = y+J;

            //error checking
            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

            rgbval[0][I+N/2][J+N/2] =qRed(*p);
            rgbval[1][I+N/2][J+N/2] =qGreen(*p);
            rgbval[2][I+N/2][J+N/2] =qBlue(*p);
        }
    }

    for (J=0; J<=N-1; J++)
    {
        for (I=0; I<=N-1; I++)
        {
            for(int loop=0; loop<=2; loop++)
            {
                if (doMin)
                {
                    if (rgbval[loop][I][J] < color[loop]) color[loop] = rgbval[loop][I][J];
                }
                else
                {
                    if (rgbval[loop][I][J] > color[loop]) color[loop] = rgbval[loop][I][J];
                }
            }
        }
    }

    //write out pixels to buffer
    checkColorBounds(color[0],color[1],color[2]);

    QRgb *e = (QRgb *)buffer->scanLine(y) + (x);
    *e = qRgb(color[0], color[1], color[2]);

}
