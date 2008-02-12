/*******************************************************************************
**
** A Jahshaka RasFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "rtripple.h"
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
    pluginFamily  = "RtFx";              //family of plugin
    pluginGroup   = "Geometry";          //group of plugin
    pluginName    = "RtRipple";          //name of plugin
    pluginClass   = JAH_MESH_PLUGIN;     //class of plugin ie CPU, GPU, MeshFX

    QString guid = "e6591a87-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);

    //we are using 1 slider
    usesliders = true;
    numsliders = 4;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Radius";
    sliderlabel[1]="Wave Height";
    sliderlabel[2]="Porcupine Height";

    sliderlabel[3]="Porcupine Frequency";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Realtime Ripple";

    //this should be a dropdown menu...
    numoptions = 1;
    button = new QString[numoptions];
    button[0]="Porcupine";

    //for mesh support
    usesMesh = true;
}

void    MyPlugin::processImage(void)
{
   /////////////////////////////////////////////////////
    //we initialize the effect the first time we use it

    //debug("in here");

    processRtFx();
}

////////////////////////////////////////////
//main imageprocessing loop


//we are testing this out with the ripple effects
//so far so good, cept for a few issues
//with spikes...
//rever to rfxnvlighting.cpp.bak when working

void    MyPlugin::processRtFx(void)
{

    GLfloat                 ripple_radius;
    GLfloat                 wave_height;
    GLfloat                 half_wave_width;

    GLfloat                 radians;
    GLfloat                 current_radius;
    GLfloat                 current_radius_offset;

    GLfloat                 inner_limit;
    GLfloat                 outer_limit;

    int                     porcupine_frequency;
    GLfloat                 porcupine_height;
    bool porcupine;

    GLfloat z_offset;

    int                     i, j;
    GLfloat                 xx, yy, zz;

    ripple_radius = (slider[1] * 2.0) + 100.0;
    wave_height = (slider[2] / 2.0) + 10.0;

    porcupine_height = (slider[3] +100.0 );

    porcupine_frequency = ((int)slider[4] + 10 );

    if (porcupine_frequency < 1) 
    {
        porcupine_frequency = 1;
    }
    
    half_wave_width = (PI / 2.0) * wave_height;

    inner_limit = ripple_radius - half_wave_width;
    outer_limit = ripple_radius + half_wave_width;

    /////////////////////////////////////////////////////////////////////////////
    //this used to induce random spikes into the ripple when the depth increased
    //hmm dont work any more... but it was really cool
    //think it was fixed when we moved to glfloat from int on the sliders?

    int limits;
    if (option[1])
    {
        limits = MESH_Y_DIMENSION + 2;
        porcupine = true;
    }
    else
    {
        limits = MESH_Y_DIMENSION+1;
        porcupine = false;
    }


    for (i = 0; i < MESH_X_DIMENSION; i++)
    {
        for (j = 0; j < limits; j++)
        {
            getMeshCoord(i, j, xx, yy, z_offset);

            current_radius = sqrt((xx * xx) + (yy * yy));

            if ((current_radius < outer_limit) && (current_radius > inner_limit))
            {
                current_radius_offset = current_radius - ripple_radius;

                radians = current_radius_offset / half_wave_width;
                zz = z_offset + ((wave_height * cos(radians)) * 5.0);

                if (porcupine)
                {
                    if (((int)zz % porcupine_frequency)==0) zz+=porcupine_height;
                }
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




