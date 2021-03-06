/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitrgbvary.h"
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
    pluginFamily  = "JitFx";              //family of plugin
    pluginGroup   = "Noise";              //group of plugin
    pluginName    = "RgbJitter";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;       //class of plugin ie CPU, GPU, MeshFX

    QString guid = "550644bf-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);

    //we are using 1 slider
    usesliders = true;
    numsliders = 1;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="Z";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="RGB Vary";

    jitter_init(2);
}



#define JITTER_TABLE_BITS   10
#define JITTER_TABLE_SIZE   (1<<JITTER_TABLE_BITS)
#define JITTER_MASK         (JITTER_TABLE_SIZE-1)

// jitter macros
#define jitterx(x,y,s) \
(uranx[((x+(y<<2))+irand[(x+s)&JITTER_MASK])&JITTER_MASK])
#define jittery(x,y,s) \
(urany[((y+(x<<2))+irand[(y+s)&JITTER_MASK])&JITTER_MASK])

// global varables
int irand[JITTER_TABLE_SIZE];    // jitter look-up table
int uranx[JITTER_TABLE_SIZE];    // jitter look-up table
int urany[JITTER_TABLE_SIZE];    // jitter look-up table


void    MyPlugin::processImage(void)
{

    int w,h;

    //source image size here
    //store these values for faster processing
    //and just update when vfximage is updated
    h = vfximage->height();
    w = vfximage->width();

    QRgb color;
    int N;
    int val;

    val = (int)slider[1];

    if (val>0)
        N  = val;
    else
        N = 0;

    static int theval;

    if (theval != N)
    {
        theval = N;
        jitter_init(theval);
    }

    //create dynamic buffer
    QImage* buffer = new QImage( w, h, 32 );

    ////////////////////////////////////////////
    //main imageprocessing loop
    int p, q;
    int rgb[3];

    for (int x=1; x < w-1; x++)
    {
        for (int y=1; y < h-1; y++)
        {
            color = getColor(x,y);

            rgb[0] = qRed    (color);
            rgb[1] = qGreen  (color);
            rgb[2] = qBlue   (color);

            for (int i = 0; i < 3; i++)
            {
                if (rgb[i] < 248)
                {
                   // bump up to next intensity if there is enough jitter
                   p = rgb[i] & 7;
                   q = jitterx(x, y, i);
                   if (p <= q)
                      rgb[i] += 8;

                   // add some noise
                   q = rgb[i] + jittery(x, y, i);

                   // make sure resulting intensity is within allowable range
                   if (q >= 0 && q <= 255)
                      rgb[i] = q;

                   // mask off lower 3 bits & store
                   rgb[i] = rgb[i] & 0xf8;
                }

            }

            QRgb *e = (QRgb *)buffer->scanLine(y)+x;

            *e = qRgb(rgb[0], rgb[1], rgb[2]);  //was 255,0,0

            //*e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}


void MyPlugin::jitter_init(int NOISE_LEVEL)
{
    int i,urand;

// magnitude of noise generated (0,1,2 allowed values)
//#define NOISE_LEVEL 2

    // determine order to fill table in
    for (i = 0; i < JITTER_TABLE_SIZE; ++i)
        irand[i] = rand() % JITTER_TABLE_SIZE;

    // fill in the X table
    for (i = 0; i < JITTER_TABLE_SIZE; ++i)
    {
        uranx[i] = urand = rand() % JITTER_TABLE_SIZE;
        uranx[i] = ((urand << 3) + urand) >> JITTER_TABLE_BITS;
    }

    // fill in the Y table
    for (i = 0; i < JITTER_TABLE_SIZE; ++i)
    {
        urany[i] = urand = rand() % JITTER_TABLE_SIZE;
        urany[i] = ((((urand * NOISE_LEVEL * 2) + JITTER_TABLE_SIZE/2)
                     >> JITTER_TABLE_BITS) - NOISE_LEVEL) << 3;
    }

}

