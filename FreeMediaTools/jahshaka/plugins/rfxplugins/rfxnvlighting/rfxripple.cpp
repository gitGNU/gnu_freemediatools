/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rfxnvlighting.h"
#include <math.h>


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
    pluginFamily  = "RasFx";            //family of plugin
    pluginGroup   = "Effect";           //group of plugin
    pluginName    = "NVlighting";          //name of plugin
    pluginClass   = JAH_GPU_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "a29a8942-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 9;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="X";
    sliderlabel[1]="Y";
    sliderlabel[2]="Z";
    sliderlabel[3]="R";
    sliderlabel[4]="G";
    sliderlabel[5]="B";
    sliderlabel[6]="D";
    sliderlabel[7]="E";
    sliderlabel[8]="D";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="NV30 Lighting";

    //important gpu support variable here
    HASNV  = true;
    HASARB = false;

    //for mesh support
    usesMesh = true;
}

void    MyPlugin::processImage(void)
{
   /////////////////////////////////////////////////////
    //we initialize the effect the first time we use it

    debug("in here");

    if (!gpuactive)
    {
        initializeGpu();
        gpuactive = true;
    }

    ////////////////////////////////////////////////////
    //we only run the effect if initialization went ok
    if (gpuinitialized)
    {
        ////////////////////////////////////
        //need to switch between arb and nv
        //arb gets the preference but not all plugins have arb versions

        if (USENV)     { processGpuFx();         return; }
        if (USEARB)    { processGpuArbFx();      return; }

        //if we got here it means the hardware isnt suported
        debug("plugin doesnt support current configuration");
    }
}

////////////////////////////////////////////
//main imageprocessing loop


//we are testing this out with the ripple effects
//so far so good, cept for a few issues
//with spikes...
//rever to rfxnvlighting.cpp.bak when working

void    MyPlugin::processGpuFx(void)
{
    debug("and now in here");


    GLfloat                 ripple_radius;
    GLfloat                 wave_height;
    GLfloat                 half_wave_width;

    GLfloat                 radians;
    GLfloat                 current_radius;
    GLfloat                 current_radius_offset;

    GLfloat                 inner_limit;
    GLfloat                 outer_limit;

    GLfloat z_offset;

    int                     i, j;
    GLfloat                 xx, yy, zz;

    ripple_radius = (slider[1] * 2.0) + 100.0;
    wave_height = (slider[2] / 2.0) + 10.0;

    half_wave_width = (PI / 2.0) * wave_height;

    inner_limit = ripple_radius - half_wave_width;
    outer_limit = ripple_radius + half_wave_width;

    for (i = 0; i < MESH_X_DIMENSION; i++)
    {
        //for (j = 0; j < (MESH_Y_DIMENSION ); j++)
        for (j = 0; j < (MESH_Y_DIMENSION + 1); j++)
        {
            getMeshCoord(i, j, xx, yy, z_offset);
            //layers[1].getMeshCoord(i, j, xx, yy, z_offset);

            current_radius = sqrt((xx * xx) + (yy * yy));

            if ((current_radius < outer_limit) && (current_radius > inner_limit))
            {
                current_radius_offset = current_radius - ripple_radius;

                radians = current_radius_offset / half_wave_width;
                zz = z_offset + ((wave_height * cos(radians)) * 5.0);
            }
            else
            {
                zz = z_offset;
            }

            //set x and y coords in mesh
            setMeshCoord(i, j, xx, yy, zz);
        }
    }



}




