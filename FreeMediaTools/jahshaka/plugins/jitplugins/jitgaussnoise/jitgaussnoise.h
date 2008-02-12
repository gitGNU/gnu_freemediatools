/*******************************************************************************
**
** A Jahshaka JitFx Plugin
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

#include <math.h>
#include <stdlib.h>

class jahPlugin;

extern "C" {  JITGAUSSNOISE_API jahPlugin* getPluginClass();  };

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

    QRgb   getGaussNoise(float VAR, float MEAN);

    void   setImage(QImage theimage) { vfximage = new QImage (theimage); }
    QImage getImage(void)            { return *vfximage; }
    void   processImage(void);

    int    gaussRand(void);

//private:

    QString data;
    

};




#endif
