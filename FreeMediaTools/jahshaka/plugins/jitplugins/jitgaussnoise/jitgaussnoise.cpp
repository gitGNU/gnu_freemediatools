/*******************************************************************************
**
** A Jahshaka JitFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "jitgaussnoise.h"

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
    pluginFamily  = "JitFx";             //family of plugin
    pluginGroup   = "Noise";             //group of plugin
    pluginName    = "GaussNoise";        //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;      //class of plugin ie CPU, GPU, MeshFX

    QString guid = "22d5e42c-e0b8-11d9-947b-00e08161165f";
    setGuid(guid);


    //we are using 1 slider
    usesliders = true;
    numsliders = 2;
    sliderlabel = new QString[numsliders];
    sliderlabel[0]="L";
    sliderlabel[1]="H";


    //for the headings
    uselabels = true;
    numlabels = 1;
    label = new QString[numlabels];
    label[0]="Noise Limits";

    numoptions = 2;
    button = new QString[numoptions];
    button[0]="Add";
    button[1]="Subtract";
}


void    MyPlugin::processImage(void)
{
    int w,h;
    QRgb color,color1;

    //we can set these when image is sent over
    h = vfximage->height();
    w = vfximage->width();

    float VAR;
    float MEAN;

    //user feedback variables
    VAR  = (float) ((slider[1]+1)*10) + 800 ;
    MEAN = slider[2] ;


    //no options is always quicker
    bool useoptions = false;

    if (option[1]) useoptions = true;
    if (option[2]) useoptions = true;


    //////////////////////////////////////////////////////
    // main loop here

    int r1, g1, b1;

    for ( int y=0; y<h; y++ )
    {
        for ( int x=0; x<w; x++ )
        {
            color  = getGaussNoise(VAR,MEAN);

            if (useoptions)
            {
                //if we are adding, subtracting or oring do this
                color1 = getColor(x,y);

                //now set new buffer value to algorithim..
                QRgb *e = (QRgb *)vfximage->scanLine(y)+x;


                if (option[1] && (!option[2]))
                {
                    r1 = qRed  (color) + qRed  (color1);
                    checkColorBounds(r1);

                    g1 = qGreen(color) + qGreen(color1);
                    checkColorBounds(g1);

                    b1 = qBlue (color) + qBlue (color1);
                    checkColorBounds(b1);

                	//set the pixel
                	*e = qRgb(r1,g1,b1);  //was 255,0,0
                }

                if (option[2] && (!option[1]))
                {
                    r1 = qRed  (color1) - qRed  (color);
                    checkColorBounds(r1);

                    g1 = qGreen(color1) - qGreen(color);
                    checkColorBounds(g1);

                    b1 = qBlue (color1) - qBlue (color);
                    checkColorBounds(b1);

                	//set the pixel
                	*e = qRgb(r1,g1,b1);  //was 255,0,0
                }
            }
            else
            {
                //if we do this one SCANLINE at a time it would be faster
                QRgb *e = (QRgb *)vfximage->scanLine(y) + x; //debug("here1"); buffer
                //QRgb *e = (QRgb *)buffer->scanLine(y) + x; //debug("here1");

                *e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
            }

        }
    }


}

QRgb MyPlugin::getGaussNoise(float VAR, float MEAN)
{
    //debug("get average");
    QRgb color;

    float NOISE;
    float theta;

    NOISE = sqrt (-2 * VAR * log(1.0-(float)gaussRand() / 32767.1));

    theta = (float) gaussRand() * 1.9175345E-4 - 3.14159265;

    NOISE = NOISE * cos(theta);
    NOISE = NOISE + MEAN;

    int val = (int)((unsigned char)NOISE + 0.5);

    checkColorBounds(val);

    color = qRgb (val,val,val);
    return color;
}

int    MyPlugin::gaussRand(void)
{
    int therand;

    therand =0+(int) (32767.0*rand()/(RAND_MAX+1.0));

    return therand;
}
