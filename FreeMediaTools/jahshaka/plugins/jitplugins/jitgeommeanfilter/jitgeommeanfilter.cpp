/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitgeommeanfilter.h"


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
    pluginFamily  = "JitFx";                 //family of plugin
    pluginGroup   = "Filter";                //group of plugin
    pluginName    = "GeomMeanFilter";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;          //class of plugin ie CPU, GPU, MeshFX

    QString guid = "22d5e42d-e0b8-11d9-947b-00e08161165f";
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
    label[0]="Geometry Mean Filter";

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



    for (int x=N/2; x < Width-N/2; x++)
    {
        for (int y=N/2; y < Height-N/2; y++)
        {

            //run the effect
            runEffect(x, y, N, buffer);

        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}

void MyPlugin::runEffect(int x, int y, int N, QImage * buffer)
{
    int I,J,loop;
    int color[3];


    static int aval[3][121];
    static float tarval[3][121];
    static float product[3];

    int xval,yval;

    int Z=0;

    for (J=-N/2; J<=N/2; J++)
    {
        for(I=-N/2; I<=N/2; I++)
        {
            xval = x+I;
            yval = y+J;

            //error checking
            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

            aval[0][Z] =qRed(*p);
            aval[1][Z] =qGreen(*p);
            aval[2][Z] =qBlue(*p);

            Z++;
        }
    }

    for (J=0; J<=N*N-1; J++)
    {
        for (loop =0; loop<=2; loop++)
        {
            tarval[loop][J] = pow((double)aval[loop][J],(double)(1.0/(float)(N*N)));
        }
    }

    product[0] = product[1] = product[2] = 1.0;

    for (J=0; J<=N*N-1; J++)
    {

        for (loop=0; loop<=2; loop++)
        {
            product[loop] *=tarval[loop][J];

            if (product[loop]>255)
            {
                color[loop] = 255;
            }
            else
            {
                color[loop] = (unsigned char)product[loop];
            }
        }

        ///////////////////////////////////////////////
        // write out the pixel

        checkColorBounds(color[0],color[1],color[2]);

        QRgb *e = (QRgb *)buffer->scanLine(y) + (x);
        *e = qRgb(color[0], color[1], color[2]);
    }

}
