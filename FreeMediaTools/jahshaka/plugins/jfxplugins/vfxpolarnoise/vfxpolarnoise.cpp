/*******************************************************************************
**
** A Jahshaka JahFx Plugin
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the LGPL License
**
*******************************************************************************/

#include "vfxpolarnoise.h"
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
    pluginFamily  = "JahFx";                 //family of plugin
    pluginGroup   = "Effects";               //group of plugin
    pluginName    = "PolarNoiseFx";          //name of plugin
    pluginClass   = JAH_CPU_PLUGIN;          //class of plugin ie CPU, GPU, MeshFX

    QString guid = "01cd2f2e-e0b8-11d9-947b-00e08161165f";
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
    label[0]="Polar Noise";

    numoptions = 2;
    button = new QString[numoptions];
    button[0]="Add";
    button[1]="Subtract";
}

void    MyPlugin::processImage(void)
{

    int w,h;

    //source image size here
    //store these values for faster processing
    //and just update when vfximage is updated
    h = Height;
    w = Width;

    QRgb color,color1;
    int N;
    int val;

    val = (int)slider[1];

    if (val>0)
        N  = (int) (val/10);
    else
        N = 0;

    //create dynamic buffer
    QImage* buffer = new QImage( w, h, 32 );

    //no options is always quicker
    bool useoptions = false;

    if (option[1]) useoptions = true;
    if (option[2]) useoptions = true;


    ////////////////////////////////////////////
    //main imageprocessing loop

    //int r1, g1, b1;
    int rgbval[3];


    for (int x=1; x < w-1; x++)
    {
        for (int y=1; y < h-1; y++)
        {

            color  = getPolarNoise(x,y,N);

            if (useoptions)
            {
                //if we are adding, subtracting or oring do this
                color1 = getColor(x,y);

                //now set new buffer value to algorithim..
                QRgb *e = (QRgb *)buffer->scanLine(y)+x;


                if (option[1] && (!option[2]))
                {
                    rgbval[0] = qRed  (color) + qRed  (color1);
                    rgbval[1] = qGreen(color) + qGreen(color1);
                    rgbval[2] = qBlue (color) + qBlue (color1);

                    checkColorBounds(rgbval[0],rgbval[1],rgbval[2]);

                	//set the pixel
                	*e = qRgb(rgbval[0],rgbval[1],rgbval[2]);  //was 255,0,0
                }

                if (option[2] && (!option[1]))
                {
                    rgbval[0] = qRed  (color1) - qRed  (color);
                    rgbval[1] = qGreen(color1) - qGreen(color);
                    rgbval[2] = qBlue (color1) - qBlue (color);

                    checkColorBounds(rgbval[0],rgbval[1],rgbval[2]);

                	//set the pixel
                	*e = qRgb(rgbval[0],rgbval[1],rgbval[2]);  //was 255,0,0
                }

            }
            else
            {
                //now set new buffer value to algorithim..
                QRgb *e = (QRgb *)buffer->scanLine(y)+x;

                *e = qRgb(qRed(color), qGreen(color), qBlue(color));  //was 255,0,0
            }
        }
    }

      *vfximage = buffer->copy();
      delete buffer;
}


QRgb MyPlugin::getPolarNoise(int x, int y, int N)
{
//debug("get average");

    int r,g,b;
    int xval, yval;
    QRgb color;

    //convert to polar
    double a,ra;

    xval = x;
    yval = y;

    if (N==0) N=1;

    //////////////////////////////////////////
    //polar noisefx

    convertToPolar(a, ra,  xval,  yval);

    float ra1 = (ra * ra)/N;
    float a1 = a+ra1/N;

    convertFromPolar(xval, yval, a1, ra1);

    /////////////////////////////////////////
    //return value

    checkResBounds(xval,yval);
    QRgb *p = (QRgb *)vfximage->scanLine(yval)+xval;
    r=qRed(*p); g=qGreen(*p); b=qBlue(*p);

    checkColorBounds(r,g,b);
    color = qRgb (r,g,b);

    return color;
}


void MyPlugin::convertToPolar(double &a, double &r, int x, int y)
{

    double dx = x - (vfximage->width()/2);
    double dy = y - (vfximage->height()/2);

    //convert to polar
    a = atan2(dy,dx);
    r = sqrt((dx*dx) + (dy*dy));

}

bool MyPlugin::convertFromPolar(int &x, int &y, double a, double r)
{
    //convert polar back
    int w = vfximage->width();
    int h = vfximage->height();

    double p[2];

    p[0] = r * cos(a);
    p[1] = r * sin(a);

    int xp,yp;
    double xc = (w/2);
    double yc = (h/2);

    xp = (int) (p[0]+xc);
    yp = (int) (p[1]+yc);

    x = xp;
    y = yp;

    if ( (xp < w) && (yp < h) && (xp >=0) && (yp >= 0))
        return true;
    else
        return false;


}








