/*******************************************************************************
**
** The source file for the Jahshaka tracer module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <string>
#include <fstream>
#include <time.h>
#include <iostream>

#include "opentracer.h"
#include <FreeMediaTools.h>

class OL_Trace::OL_TraceDestroyer
{
private:
	OL_Trace* _pInstance;
public:
	OL_TraceDestroyer()						throw()	{	_pInstance = NULL;		};
	~OL_TraceDestroyer()					throw()	{	delete _pInstance;		};
	void SetInstance(OL_Trace* pInstance)	throw()	{	_pInstance = pInstance;	};
};

/*
Entries in _labels must correspond to
those in MessageLevel
*/


const char* OL_Trace::_labels[] = {

    ""     ,	// not used -- for "LevelNothing"
    "[*!] " ,
    "[ W]  " ,
    "[ i] " ,   // intentional -- informational messages have no prefix
    "[*D]  " ,
    ""         // not used -- for "LevelNothing"

} ;

OL_Trace::MessageLevel OL_Trace::defaultLevel = OL_Trace::LevelInfo ;

const OL_Trace::MessageLevel OL_Trace::LEVEL_MIN = OL_Trace::LevelNothing ;
const OL_Trace::MessageLevel OL_Trace::LEVEL_MAX = OL_Trace::LevelEverything ;

OL_Trace* OL_Trace::_instance=0 ;
OL_Trace::OL_TraceDestroyer OL_Trace::_destroyer;

/* - - - - - - - - - - - - - - - - - - - - */

OL_Trace::OL_Trace()
    throw()
{
    #ifdef DEBUG
    _level = LevelEverything;
    #else
    _level = defaultLevel;
    #endif

    printtofile=false;

    return ;

} // Logger ctor

/* - - - - - - - - - - - - - - - - - - - - */

OL_Trace::~OL_Trace()
    throw()
{
        //debug(">>closing out tracer");
        if (printtofile) 
        {
            jahfile.close(); 
        }

} // Logger ctor

/* - - - - - - - - - - - - - - - - - - - - */

OL_Trace* OL_Trace::getInstance()
    throw()
{

    if (_instance == 0)  // is it the first call?
    {  
    _instance = new OL_Trace; // create sole instance
	_destroyer.SetInstance(_instance);
    }
    return _instance; // address of sole instance

} // Logger::getInstance()

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* message ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level )
    {
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << message << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << message << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* id , const char* message ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level ){
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* id , int message ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level ){
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* id , bool message ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level ){
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << id << " : " << message << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* id , const char* message, int value ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level ){
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << id << " : " << message << value << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << value << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << id << " : " << message << value << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << value << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::printMessage( MessageLevel level , const char* id , const char* message, bool value ) const
    throw()
{

    /*
    see note in setLevel() for explanation of why
    we don't check the provided message level
    */

    if( level <= _level ){
        if( LevelError == _level )
        {
            std::cerr << _labels[ level ] << id << " : " << message << value << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        } 
        else 
        {
            std::cout << _labels[ level ] << id << " : " << message << value << std::endl ; 

            if (printtofile) 
            { 
                //QTextStream t( jahfile );
                jahfile << _labels[ level ] << id << " : " << message << "\n"; 
            }
        }
    }

    return ;

} // Logger::printMessage( MessageLevel , const char* , const char* )

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::debug( const char* message ) const
    throw()
{

    printMessage( LevelDebug , message ) ;
    return ;

} // Logger::debug( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::debug( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelDebug , id , message ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::debug( const char* id , int message ) const
    throw()
{

    printMessage( LevelDebug , id , message ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::debug( const char* id , const char* message, bool value ) const
    throw()
{

    printMessage( LevelDebug , id , message, value ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::debug( const char* id , const char* message, int value ) const
    throw()
{

    printMessage( LevelDebug , id , message, value ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::info( const char* message ) const
    throw()
{

    printMessage( LevelInfo , message ) ;
    return ;

} // Logger::info( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::info( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelInfo , id , message ) ;
    return ;

} // Logger::info( const char* , const char* ) 


/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::warn( const char* message ) const
    throw()
{

    printMessage( LevelWarn , message ) ;
    return ;

} // Logger::warn( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::warn( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelWarn , id , message ) ;
    return ;

} // Logger::warn( const char* , const char* ) 


/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::error( const char* message ) const
    throw()
{

    printMessage( LevelError , message ) ;
    return ;

} // Logger::error( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::error( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelError , id , message ) ;
    return ;

} // Logger::error( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::levelUp( void )
    throw()
{

    if( _level < LEVEL_MAX ){
        ++_level ;
    }

    return ;

} // levelUp

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::levelDown( void )
    throw()
{

    if( _level > LEVEL_MIN ){
        --_level ;
    }

    return ;

} // levelDown

/* - - - - - - - - - - - - - - - - - - - - */

void OL_Trace::setLevel( MessageLevel newLevel )
    throw()
{
    /*
    Purists beware -- we don't check the array index here because
    setLevel() should only be called using the predefined constants
    in MessageLevel. -eqm
    */

    _level = newLevel ;

    return ;

} // setLevel()

void OL_Trace::setFileSave( bool  fileout ) throw()
{

    if (fileout)
    {
    
        //needs to be different for windows and macintosh
		std::string loglocation;
		#ifdef WIN32
		loglocation = "C:/jahlogfile.txt";
		#else
        loglocation = "/tmp/jahlogfile.txt";
		#endif


        //jahfile = new QFile(loglocation);
        //if ( jahfile->open( IO_WriteOnly | IO_Append ) ) 

	

        jahfile.open( loglocation.data( ), std::ios::out | std::ios::app );
		if ( jahfile.is_open( ) ) 
        {
            //write the header here   
            //QDate thedate;
			time_t l_time;
			time( & l_time );
			tm * l_the_date = localtime( & l_time );
			std::string thedatestr = asctime ( l_the_date );
            //std::string thedatestr = thedate.toString(Qt::TextDate).data();
            
            //QTime thetime;
            //std::string thetimestr= thetime.toString(Qt::TextDate).data();
                        
            //QTextStream t( jahfile );
            jahfile <<  ( const char * )"---------------------------" << "\n"; 
	    jahfile <<  FMT_PROGRAM_TITLE << " Tracer Log File " << "\n";
            //jahfile <<  "Date: " << thedatestr.data() << " Time: " <<  thetimestr.data() << "\n"; 
            jahfile <<  "Date & Time: " << thedatestr.data() << "\n"; 
            jahfile <<  "---------------------------" << "\n"; 

            printtofile = true;
        }
        else
        {
            printtofile = false;
        }
    }

    return ;

}

/* - - - - - - - - - - - - - - - - - - - - */

/* EOF Logger.cc */
