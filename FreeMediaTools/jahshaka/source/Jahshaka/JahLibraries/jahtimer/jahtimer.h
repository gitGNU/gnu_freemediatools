/*******************************************************************************
**
** The header file for the Jahshaka timer class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHTIMER_H
#define JAHTIMER_H

#include <string>
//#include <qstring.h>
#include <sys/timeb.h>

//#include "jahtracer.h"

class JahTimer
{
    
public:
	JahTimer() { }
	~JahTimer(){ }

    void   starttime( void );
    void   endtime  ( void );
    void   printtime( std::string data );
    double returnTimeFps( void ); 

private:

    timeb                           current_time;
    
    //used for timing effects
    double           				start_time_float; 
    double							end_time_float;
    double							total_time_float;
    
};

#endif
