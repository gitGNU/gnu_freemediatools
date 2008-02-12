/*******************************************************************************
**
** The source file for the Jahshaka timer class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahtimer.h"

void JahTimer::starttime( void ) 
{ 
    //start_time=0; 
    //end_time=0;     
    
	#ifndef __APPLE__
    ftime(&current_time); 
    //start_time = (int)current_time.millitm; 

    start_time_float = (double)current_time.time + ((double)current_time.millitm / 1000.0);
	#endif
}

void JahTimer::endtime  ( void ) 
{ 
	#ifndef __APPLE__
    ftime(&current_time);
    //end_time = (int)current_time.millitm;
    end_time_float = (double)current_time.time + ((double)current_time.millitm / 1000.0);
    total_time_float = end_time_float - start_time_float;
	#endif
}

///////////////////////////////////////////////////////
//routines to print or return timing data

void JahTimer::printtime( std::string data ) 
//void JahTimer::printtime( const char * data ) 
{
    #ifndef __APPLE__
    
    char l_buffer[50];
  
    //time in seconds 
    sprintf( l_buffer ,"%f" , total_time_float );
    //sprintf( l_buffer , "%lu" , total_time_float );

    std::string outtext = data + l_buffer;

    //print data to stdout
    fprintf( stdout , outtext.data());
 
    #endif
}


double JahTimer::returnTimeFps( void ) 
{
   #ifndef __APPLE__
      
    //time in seconds?
    double timefps = (double) (1.0/total_time_float);   
    
    return timefps;

    #else
    return 0;
    #endif

}
