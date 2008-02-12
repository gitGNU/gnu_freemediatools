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

#include "jahtracer.h"


class JahTrace::JahTraceDestroyer
{
private:
	JahTrace* _pInstance;
public:
	JahTraceDestroyer()						throw()	{	_pInstance = NULL;		};
	~JahTraceDestroyer()					throw()	{	delete _pInstance;		};
	void SetInstance(JahTrace* pInstance)	throw()	{	_pInstance = pInstance;	};
};

/*
Entries in _labels must correspond to
those in MessageLevel
*/


const char* JahTrace::_labels[] = {

    ""     ,	// not used -- for "LevelNothing"
    "[*!] " ,
    "[ W]  " ,
    "[ i] " ,   // intentional -- informational messages have no prefix
    "[*D]  " ,
    ""         // not used -- for "LevelNothing"

} ;

JahTrace::MessageLevel JahTrace::defaultLevel = JahTrace::LevelInfo ;

const JahTrace::MessageLevel JahTrace::LEVEL_MIN = JahTrace::LevelNothing ;
const JahTrace::MessageLevel JahTrace::LEVEL_MAX = JahTrace::LevelEverything ;

JahTrace* JahTrace::_instance=0 ;
JahTrace::JahTraceDestroyer JahTrace::_destroyer;

/* - - - - - - - - - - - - - - - - - - - - */

JahTrace::JahTrace()
    throw()
{
    _level = defaultLevel ;

    printtofile=false;

    return ;

} // Logger ctor

/* - - - - - - - - - - - - - - - - - - - - */

JahTrace::~JahTrace()
    throw()
{
        //debug(">>closing out tracer");
        if (printtofile) 
        {
            jahfile.close(); 
        }

} // Logger ctor

/* - - - - - - - - - - - - - - - - - - - - */

JahTrace* JahTrace::getInstance()
    throw()
{

    if (_instance == 0)  // is it the first call?
    {  
    _instance = new JahTrace; // create sole instance
	_destroyer.SetInstance(_instance);
    }
    return _instance; // address of sole instance

} // Logger::getInstance()

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::printMessage( MessageLevel level , const char* message ) const
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

void JahTrace::printMessage( MessageLevel level , const char* id , const char* message ) const
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

void JahTrace::printMessage( MessageLevel level , const char* id , int message ) const
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

void JahTrace::printMessage( MessageLevel level , const char* id , bool message ) const
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

void JahTrace::printMessage( MessageLevel level , const char* id , const char* message, int value ) const
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

void JahTrace::printMessage( MessageLevel level , const char* id , const char* message, bool value ) const
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

void JahTrace::debug( const char* message ) const
    throw()
{

    printMessage( LevelDebug , message ) ;
    return ;

} // Logger::debug( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::debug( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelDebug , id , message ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::debug( const char* id , int message ) const
    throw()
{

    printMessage( LevelDebug , id , message ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::debug( const char* id , const char* message, bool value ) const
    throw()
{

    printMessage( LevelDebug , id , message, value ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::debug( const char* id , const char* message, int value ) const
    throw()
{

    printMessage( LevelDebug , id , message, value ) ;
    return ;

} // Logger::debug( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::info( const char* message ) const
    throw()
{

    printMessage( LevelInfo , message ) ;
    return ;

} // Logger::info( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::info( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelInfo , id , message ) ;
    return ;

} // Logger::info( const char* , const char* ) 


/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::warn( const char* message ) const
    throw()
{

    printMessage( LevelWarn , message ) ;
    return ;

} // Logger::warn( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::warn( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelWarn , id , message ) ;
    return ;

} // Logger::warn( const char* , const char* ) 


/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::error( const char* message ) const
    throw()
{

    printMessage( LevelError , message ) ;
    return ;

} // Logger::error( const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::error( const char* id , const char* message ) const
    throw()
{

    printMessage( LevelError , id , message ) ;
    return ;

} // Logger::error( const char* , const char* ) 

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::levelUp( void )
    throw()
{

    if( _level < LEVEL_MAX ){
        ++_level ;
    }

    return ;

} // levelUp

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::levelDown( void )
    throw()
{

    if( _level > LEVEL_MIN ){
        --_level ;
    }

    return ;

} // levelDown

/* - - - - - - - - - - - - - - - - - - - - */

void JahTrace::setLevel( MessageLevel newLevel )
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

void JahTrace::setFileSave( bool  fileout ) throw()
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
            jahfile <<  "Jahshaka Tracer Log File " << "\n"; 
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
