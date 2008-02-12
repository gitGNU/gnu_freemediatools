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

class jahPlugin;

extern "C" {  JITAVERAGE_API jahPlugin* getPluginClass();  };

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

    void   setImage(QImage theimage) { vfximage = new QImage (theimage); }
    QImage getImage(void)            { return *vfximage; }
    void   processImage(void);

    QRgb getAverage(int x, int y, int N);

    void convertToPolar(double &a, double &r, int x, int y);
    bool convertFromPolar(int &x, int &y, double a, double r);


    QString data;
    

};




#endif
