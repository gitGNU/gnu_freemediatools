/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitmedianfilter.h"


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
    pluginName    = "MedianFilter";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;        //class of plugin ie CPU, GPU, MeshFX

    QString guid = "29ba3054-e0b8-11d9-947b-00e08161165f";
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
    label[0]="Median Filter";

}

void    MyPlugin::processImage(void)
{

    QImage* buffer = new QImage( Width, Height, 32 );

   //we can play with this value
    int N=7+(int)slider[1];

    //need to error check this for array sizes
    if (N>11) N=11;
    if (N<0)  N=0;


    for (int x=N/2; x < Width-N/2; x++)
    {
        for (int y=N/2; y < Height-N/2; y++)
        {
            runEffect(x, y, N, buffer);
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}

void MyPlugin::runEffect(int x, int y, int N, QImage * buffer)
{
    int I,J;
    int Z;
    int rval, gval, bval;

    int IR, IG, IB;

    static int AR[121], AG[121], AB[121];
    int ARV, AGV, ABV;

    int xval,yval;

    Z=0;

    for (J=-N/2; J<=N/2; J++)
    {
        for(I=-N/2; I<=N/2; I++)
        {
            xval = x+I;
            yval = y+J;

            //error checking
            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

            AR[Z] =qRed(*p);
            AG[Z] =qGreen(*p);
            AB[Z] =qBlue(*p);

            Z++;
        }
    }


    for (J=1; J<=N*N-1; J++)
    {
        ARV = AR[J];
        AGV = AG[J];
        ABV = AB[J];

        IR = IB = IG = J-1;

        while(IR>=0 && AR[IR] > ARV)
        {
            AR[IR+1]=AR[IR];
            IR=IR-1;
        }
        AR[IR+1]=ARV;

        while(IB>=0 && AB[IB] > ABV)
        {
            AB[IB+1]=AB[IB];
            IB=IB-1;
        }
        AB[IB+1]=ABV;

        while(IG>=0 && AG[IG] > AGV)
        {
            AG[IG+1]=AG[IG];
            IG=IG-1;
        }
        AG[IG+1]=AGV;

    }

    rval = AR[N*N/2];
    gval = AG[N*N/2];
    bval = AB[N*N/2];

    checkColorBounds(rval,gval,bval);

    QRgb *e = (QRgb *)buffer->scanLine(y) + (x);
    *e = qRgb(rval, gval, bval);


}
