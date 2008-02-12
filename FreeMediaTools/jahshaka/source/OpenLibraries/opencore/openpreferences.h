/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef OPENPREFS_H
#define OPENPREFS_H

#include <string>

#include "openimagelib.h"

/////////////////////////////////////////////////////////////
//need to move tis to be pointer based like the tracer
//so the openlibraries have ready access to whats in here

class OpenPrefs
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
		~OpenPrefs() throw() ;
        #endif

    private:

        static OpenPrefs _instance ;

        // Hide these 'cause this is a singleton.       
        OpenPrefs() throw() ; 

        #ifndef WIN32
		~OpenPrefs() throw() ;    
        #endif

        OpenPrefs( OpenPrefs&);
        OpenPrefs& operator = ( const OpenPrefs&) ;

        /// the sole instance of this class
        std::string BasePath;
        std::string MediaPath;

		bool useExternalPath;

    public:
        //gets a instance of this object
        static OpenPrefs& getInstance() throw();

		//does the same as setBasePath for now
        void initializeOpenLib ( std::string path ); 

        void setBasePath ( std::string path ); 
        void setMediaPath ( std::string path ); 

        ///////////////////////////////////
        //routines to get variables

        std::string getBasePath ( void ); 
        std::string getMediaPath ( void ); 

};

#endif

