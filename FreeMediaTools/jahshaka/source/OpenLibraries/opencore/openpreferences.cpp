/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qimage.h>
#include "openpreferences.h"

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

OpenPrefs OpenPrefs::_instance ;

OpenPrefs::OpenPrefs() 
    throw()
{   
    //these dont actually need to be initialized   
    BasePath             = "invalid";
    MediaPath            = "invalid";
	useExternalPath		 = false;
}

OpenPrefs::~OpenPrefs() 
    throw()
{

}

OpenPrefs& OpenPrefs::getInstance()
    throw()
{
    return (_instance);
}


/////////////////////////////////////////////////////////////////
//routines to set data start here

void OpenPrefs::initializeOpenLib(std::string path)  
{ 
	BasePath            =  path; 
	useExternalPath = true;

	//here we initalize the openimagelib as well
    //////////////////////////////////////////////////////////////
    //initialize image types
	//should do this in the openimagelib with a static member
    QImageIO::defineIOHandler ( "TGA","^TGA[0-9][0-9][a-z]",0, &kimgio_tga_read, &kimgio_tga_write );
    
	#ifndef JAHIRIX
	QImageIO::defineIOHandler ( "RGB","^RGB[0-9][0-9][a-z]",0, &kimgio_rgb_read, &kimgio_rgb_write );
	#endif
	
	QImageIO::defineIOHandler ( "EXR","^EXR[0-9][0-9][a-z]", 0, vmfx_exr_read, 0 );
    QImageIO::defineIOHandler ( "TIF","^TIF[0-9][0-9][a-z]", 0, vmfx_exr_read, 0 );
}

void OpenPrefs::setBasePath(std::string path)  
{ 
	BasePath            =  path; 
	useExternalPath = true;
}

void OpenPrefs::setMediaPath(std::string path) 
{ 
	MediaPath           =  path; 
}


/////////////////////////////////////////////////////////////////
//routines to get data start here

std::string OpenPrefs::getBasePath(void)     { return BasePath;    }
std::string OpenPrefs::getMediaPath(void)    { return MediaPath;   }

