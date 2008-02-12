/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxgrey.h"

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
    pluginName    = "Greyscale";       //name of plugin
    pluginFamily  = "JahFx";           //family of plugin
    pluginGroup   = "Color";           //group of plugin
    pluginClass   = JAH_CPU_PLUGIN;    //class of plugin ie CPU, GPU, MeshFX

    QString guid = "cea99ec1-e0b7-11d9-947b-00e08161165f";
    setGuid(guid);

}

//////////////////////////////////////////////////
// Plugin image subroutines start here

void    MyPlugin::processImage(void)
{

    int r,g,b,grey;
    int xval,yval;


    for ( int y=0; y<Height; y++ )
    {
        for ( int x=0; x<Width; x++ )
        {
            //if we can do this one SCANLINE at a time it would be faster
            //uchar *pe = pix.scanLine(y);
            xval = x;
            yval = y;

            //error checking
            checkResBounds(xval,yval);

            QRgb *p = (QRgb *)vfximage->scanLine(yval) + (xval);

            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p); //debug("here2");

            grey=( (r+g+b)/3 ); //debug("here3");
            *p = qRgb(grey,grey,grey);
        }
    }

}


/*
void GLEffect::doGrey(QImage& pix)
{

    int r,g,b,grey;
    QString conv,text;

    //note that this crashes when x or y are over the scope
    //for some reason this happens with the default image

    //int yy = (int)layers[1].textureData.height;
    //int xx = (int)layers[1].textureData.width;
    int yy = (int)layerlist->at(1)->thelayer->textureData.height;
    int xx = (int)layerlist->at(1)->thelayer->textureData.width;

    for ( int y=0; y<yy; y++ )
    {
        for ( int x=0; x<xx; x++ )
        {
            //if we can do this one SCANLINE at a time it would be faster
            //uchar *pe = pix.scanLine(y);

            QRgb *p = (QRgb *)pix.scanLine(y) + x; //debug("here1");

            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p); //debug("here2");

            grey=( (r+g+b)/3 ); //debug("here3");
            *p = qRgb(grey,grey,grey);
        }
    }
}
*/

