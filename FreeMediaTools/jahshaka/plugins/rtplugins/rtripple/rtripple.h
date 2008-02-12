/*******************************************************************************
**
** A Jahshaka RasFx Plugin
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

#include <GL/glew.h>
#include "gpumathlib.h"
#include <qgl.h>

class jahPlugin;

extern "C" {  RTRIPPLE_API jahPlugin* getPluginClass();  };


class MyPlugin : public jahPlugin
{

public:
    MyPlugin()
    {
        initSettings();

        initializePlugin();
    };

    ~MyPlugin()
    {};

    void   initializePlugin();

    void   processImage(void);
    
    void   processRtFx(void);



    //variables for effect
    /*
    GLuint      random_texture;
    GLuint      contrast_texture;

    int*        permutation_table;
    float4*     gradient_table;

    bool	    randomTextureInitialized;
    bool	    contrastTextureInitialized;
    */
};




#endif
