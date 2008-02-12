/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#ifndef VFXPLUGIN_H
#define VFXPLUGIN_H

#include "config.h"
#include "jahplugintemplate.h"
#include <qstring.h>

class jahPlugin;

extern "C" {  VFXMANDELBROT_API jahPlugin* getPluginClass();  };

class MyPlugin : public jahPlugin
{

public:
    MyPlugin()
    {
        initSettings();

        initializePlugin();
    };

    ~MyPlugin()
    {
        debug(">>DeBooted the plugin");
    };

    void   initializePlugin();

    void   processImage(void);

    bool getMandelbrotMinMax( float& MINX, float& MINY, float& MAXY, float& MAXX );
    QRgb getMandelbrotColor( int manval, QRgb mypix, int mode );
    int  doMandelbrotTerminate( float x, float y );
    int  doMandelbrotCalcRes(float x, float y, int res );



};




#endif
