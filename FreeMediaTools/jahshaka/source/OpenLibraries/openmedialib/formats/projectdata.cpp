/*******************************************************************************
**
** The source file for the Jahshaka project library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "projectdata.h"
#include <string>

/////////////////////////////////////////////
//we need to get rid of these routines asap

projectData::projectData()
{
	configureProjectData();
	defaultformat = 3;
};

int  projectData::getNumformats(void)
{
	return numberformats;
}

int  projectData::getDefaultformat(void)
{
	return defaultformat;
}

int  projectData::getXres(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;

	return jahProjects[thedata].xres;
}

int  projectData::getYres(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return jahProjects[thedata].yres;
}

QString  projectData::getFamily(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return jahProjects[thedata].family;
}

QString  projectData::getTitle(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return jahProjects[thedata].title;
}

double  projectData::getPixelRatio(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return jahProjects[thedata].pixelratio;
}

int  projectData::getFPS(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return int(jahProjects[thedata].framerate + 0.5);
}

bool  projectData::getDropFrame(int data)
{
	int thedata = data;
	if (thedata>numberformats) thedata = 3;
	return jahProjects[thedata].family == "NTSC";
}

void  projectData::buildComboBox(QComboBox * box)
{
    for (int i=0; i<numberformats; i++)
        box->insertItem(jahProjects[i].title);
}

int      
projectData::getFieldNumber(std::string name_string)
{
    for (int i = 0; i < numberformats; i++)
    {
        std::string title_string = jahProjects[i].title.data();

        if (title_string == name_string)
        {
            return jahProjects[i].getFieldNumber();
        }
    }

    return 0;
}

void projectData::configureProjectData(void)
{
	
    ///////////////////////////////////////////////////////////
    // NTSC Formats
    
    int nf=0;
    
    jahProjects[nf].family     = "NTSC";
    jahProjects[nf].title      = "NTSC";
    jahProjects[nf].xres       = 640;
    jahProjects[nf].yres       = 480;
    jahProjects[nf].frameratio = 1.333;
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "NTSC";
    jahProjects[nf].title      = "NTSC Full";
    jahProjects[nf].xres       = 648;
    jahProjects[nf].yres       = 486;
    jahProjects[nf].frameratio = 1.333;
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "NTSC";
    jahProjects[nf].title      = "NTSC DV";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 480;
    jahProjects[nf].frameratio = 1.350; 
    jahProjects[nf].frameratiotitle = "3:2";
    jahProjects[nf].pixelratio = 10.0 / 11.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "lower";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "NTSC";
    jahProjects[nf].title      = "NTSC D1";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 486;
    jahProjects[nf].frameratio = 1.333; 
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 10.0 / 11.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "NTSC";
    jahProjects[nf].title      = "NTSC D1 Ananmorphic";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 486;
    jahProjects[nf].frameratio = 1.778; 
    jahProjects[nf].frameratiotitle = "16:9";
    jahProjects[nf].pixelratio = 40.0 / 33.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    ///////////////////////////////////////////////////////////
    // Pal Formats
    
    jahProjects[nf].family     = "PAL";
    jahProjects[nf].title      = "PAL";
    jahProjects[nf].xres       = 768;
    jahProjects[nf].yres       = 576;
    jahProjects[nf].frameratio = 1.333; 
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 25.0;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "PAL";
    jahProjects[nf].title      = "PAL DV";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 576;
    jahProjects[nf].frameratio = 1.333; 
    jahProjects[nf].frameratiotitle = "5:4";
    jahProjects[nf].pixelratio = 59.0 / 54.0;
    jahProjects[nf].framerate  = 25.0;
    jahProjects[nf].fieldorder = "lower";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "PAL";
    jahProjects[nf].title      = "PAL D1";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 576;
    jahProjects[nf].frameratio = 1.333;
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 59.0 / 54.0;
    jahProjects[nf].framerate  = 25.0;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "PAL";
    jahProjects[nf].title      = "PAL D1 Anamorphic";
    jahProjects[nf].xres       = 720;
    jahProjects[nf].yres       = 576;
    jahProjects[nf].frameratio = 1.777; 
    jahProjects[nf].frameratiotitle = "16:9";
    jahProjects[nf].pixelratio = 118.0 / 81.0;
    jahProjects[nf].framerate  = 25.0;
    jahProjects[nf].fieldorder = "upper";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    ////////////////////////////////////////////////////////////
    // HD Formats
    
    jahProjects[nf].family     = "HDTV";
    jahProjects[nf].title      = "HDTV 720/30p";
    jahProjects[nf].xres       = 1280;
    jahProjects[nf].yres       = 720;
    jahProjects[nf].frameratio = 1.778; 
    jahProjects[nf].frameratiotitle = "16:9";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 30.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "HDTV";
    jahProjects[nf].title      = "HDTV 1080/24p";
    jahProjects[nf].xres       = 1920;
    jahProjects[nf].yres       = 1080;
    jahProjects[nf].frameratio = 1.778; 
    jahProjects[nf].frameratiotitle = "16:9";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 24.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    ////////////////////////////////////////////////////////////
    // Film Formats
    
    jahProjects[nf].family     = "FILM";
    jahProjects[nf].title      = "Film Full 2K";
    jahProjects[nf].xres       = 2048;
    jahProjects[nf].yres       = 1556;
    jahProjects[nf].frameratio = 1.316; 
    jahProjects[nf].frameratiotitle = "512:389";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 24.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "FILM";
    jahProjects[nf].title      = "Cinemascope";
    jahProjects[nf].xres       = 1828;
    jahProjects[nf].yres       = 1556;
    jahProjects[nf].frameratio = 2.350; 
    jahProjects[nf].frameratiotitle = "457:389";
    jahProjects[nf].pixelratio = 2.0;
    jahProjects[nf].framerate  = 24.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "FILM";
    jahProjects[nf].title      = "Academy Cineon Full";
    jahProjects[nf].xres       = 3656;
    jahProjects[nf].yres       = 2664;
    jahProjects[nf].frameratio = 1.372; 
    jahProjects[nf].frameratiotitle = "457:333";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 24.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;

    jahProjects[nf].family     = "FILM";
    jahProjects[nf].title      = "Academy Cineon Half";
    jahProjects[nf].xres       = 1828;
    jahProjects[nf].yres       = 1332;
    jahProjects[nf].frameratio = 1.372; 
    jahProjects[nf].frameratiotitle = "457:333";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 24.0;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    ////////////////////////////////////////////
    jahProjects[nf].family     = "WEB";
    jahProjects[nf].title      = "Web Boadcast";
    jahProjects[nf].xres       = 320;
    jahProjects[nf].yres       = 240;
    jahProjects[nf].frameratio = 1.0;
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 29.97;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
    jahProjects[nf].family     = "CELL";
    jahProjects[nf].title      = "Cell Boadcast";
    jahProjects[nf].xres       = 180;
    jahProjects[nf].yres       = 240;
    jahProjects[nf].frameratio = 1.0;
    jahProjects[nf].frameratiotitle = "4:3";
    jahProjects[nf].pixelratio = 1.0;
    jahProjects[nf].framerate  = 15;
    jahProjects[nf].fieldorder = "none";
    jahProjects[nf].setFieldNumber(nf);
    nf++;
    
};

projectData::~projectData(void) 
{

};

jahResolution::jahResolution()
{

}

jahResolution::~jahResolution()
{

}
