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

extern "C" {  RFXNVFOG_API jahPlugin* getPluginClass();  };

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
    void   processGpuFx(void);
    void   processGpuArbFx(void){};  //not in use yet

    //for the turbulence creator
    void  createTurbulenceTextureGPU(int width, int height, float intensity,
                                        int horizontal_mesh_size, int vertical_mesh_size, int number_of_layers);


    //variables for effect
    GLuint      turbulence_texture;
    int*        permutation_table;
    float4*     gradient_table;
};




#endif
