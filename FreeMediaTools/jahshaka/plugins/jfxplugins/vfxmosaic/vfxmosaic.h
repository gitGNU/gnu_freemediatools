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

extern "C" {  VFXMOSAIC_API jahPlugin* getPluginClass();  };

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

    void  jahMosaic  (int start_level, int Xdim, int Ydim);

    QRgb  getColor(int x, int y);

    void  setRectangle (int x, int y, int x1, int y1, QRgb color, QImage *image);


//private:

    QString data;

};




#endif
