/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHSTATS_H
#define JAHSTATS_H

#include <string>

#include "jahpreferences.h"

class JahStats
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
        ~JahStats() throw() ;
        #endif

    private:

        static JahStats _instance ;

        // Hide these 'cause this is a singleton.       
        JahStats() throw() ; 

        #ifndef WIN32
        ~JahStats() throw() ;    
        #endif

        JahStats( JahStats&);

        JahStats& operator = ( const JahStats&) ;

        //these are all variables that have to be set
        bool ARBsupport;
        bool NVsupport;

        std::string stats_glvendor;
        std::string stats_glversion;
        std::string stats_glrender;
        
        //these are variables that return data to the outside world
		std::string statsdata1;
		std::string statsdata2;
		std::string statsdata3;

    public:
        //gets a instance of this object
        static JahStats& getInstance() throw();
     
        void initalizeStats(void) throw();

        //need to clean this up later to return all in one call
        //retreive stats
        std::string getstatsdata1(void);
        std::string getstatsdata2(void);
        std::string getstatsdata3(void);

        //set stats data
        void setVendor(const std::string &);
        void setVersion(const std::string &);
        void setRenderer(const std::string &);

        void setNVsupport(bool);
        void setARBsupport(bool);



};

#endif

