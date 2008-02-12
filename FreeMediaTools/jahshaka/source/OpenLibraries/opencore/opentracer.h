/*******************************************************************************
**
** The header file for the Jahshaka tracer module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef OPENTRACER_H
#define OPENTRACER_H

#include <fstream>

//#include "qfile.h"

/**
\brief
A logging/trace system

For now this simply decides whether to print a given message,
and if so, prepends a label before passing it on to standard
output or standard error.

There's plenty of room to grow here, clearly.

To further centralize the message-handling, this object is
implemented as a GoF-style singleton.
*/

class OL_Trace {

    public:

    //note we had to move the destructor from private
    //to public for this to compile on windows in VC6++

    /**
    \brief
    destructor -- this method should never be called by client code
    */
    #ifdef WIN32
    ~OL_Trace() throw() ; 
    #endif
    /**
    \brief message types

    Entries here must correspond to those in _labels
    */
    enum MessageLevel {
        LevelNothing ,	///< don't report any messages
        LevelError ,	///< report only errors
        LevelWarn ,		///< report errors and warnings
        LevelInfo ,		///< report errors, warnings, and info messages
        LevelDebug ,	///< report error, warning, info, and debug messages
        LevelEverything	///< report all messages
    } ;


    private:

    /// match message types to a label, which prefixes the printed message
    static const char* _labels[] ;

    /// default level against which message severity levels are compared
    static MessageLevel defaultLevel ;

    /// the sole instance of this class
    static OL_Trace* _instance ;

	// These are needed to ensure that the singletons destructor is called on application shutdown.
	class OL_TraceDestroyer;
	static OL_TraceDestroyer _destroyer;


    /// determines which messages get printed, based on their index in the <TT>labels[]</TT> array
    int _level ;

    //these are for print to file option
//    QFile *jahfile;
    mutable std::fstream jahfile;
    bool printtofile;

    /**
    \brief
    constructor -- labeled "private" because this is a singleton
    */
    OL_Trace() throw() ; 

    /**
    \brief
    destructor -- this method should never be called by client code
    */
    #ifndef WIN32
    ~OL_Trace() throw() ; 
    #endif

    /**
    \brief
    copy constructor -- this object may never be copied.

    This method is not even implemented, as it will never be called.
    */
    OL_Trace( const OL_Trace& ) ;

    /**
    \brief this object should never be assigned, so this method is not even implemented
    */
    OL_Trace& operator=( const OL_Trace& ) ;

    static const MessageLevel
        LEVEL_MIN ,		///< minimum message severity level
        LEVEL_MAX		///< maximum message severity level

    ;

    /**
    \brief print the provided message, based on its severity level
    */
    void printMessage( MessageLevel level , const char* message ) const throw() ;

    /**
    \brief print the provided message and a brief identifier, based on its severity level

    The "identifier" in this case would be the name of the calling method or object.
    */
    void printMessage( MessageLevel level , const char* id , const char* message ) const throw() ;

    void printMessage( MessageLevel level , const char* id , bool message ) const throw() ;

    void printMessage( MessageLevel level , const char* id , const int message ) const throw() ;

    void printMessage( MessageLevel level , const char* id , const char* message, bool value ) const throw() ;

    void printMessage( MessageLevel level , const char* id , const char* message, int value ) const throw() ;


    protected:
    /* empty */


    public:

    /**
    \brief fetch a reference to the singleton Logger instance
    */
    static OL_Trace* getInstance() throw() ;


    /// print debug-level messages
    void debug( const char* message ) const throw() ;

    /// print debug-level messages, with an identifier
    void debug( const char* id , const char* message ) const throw() ;

    /// print debug-level messages, with an identifier
    void debug( const char* id , const int message ) const throw() ;

    /// print debug-level messages, with an identifier and a value
    void debug( const char* id , const char* message, const bool value) const throw() ;

    /// print debug-level messages, with an identifier and a value
    void debug( const char* id , const char* message, const int value) const throw() ;

    /// print info-level messages
    void info( const char* message ) const throw() ;

    /// print info-level messages, with an identifier
    void info( const char* id , const char* message ) const throw() ;


    /// print warn-level messages
    void warn( const char* message ) const throw() ;

    /// print warn-level messages, with an identifier
    void warn( const char* id , const char* message ) const throw() ;


    /// print error-level messages
    void error( const char* message ) const throw() ;

    /// print error-level messages, with an identifier
    void error( const char* id , const char* message ) const throw() ;


    /// increment the message level
    void levelUp( void ) throw() ; 


    /// decrement the message level
    void levelDown( void ) throw() ; 


    /**
    \brief explicitly set the message level

    The compiler should catch attempts to use values other than
    those provided in the MessageLevel enum.

    */
    void setLevel( MessageLevel newLevel ) throw() ; 

    void setFileSave( bool filesave ) throw() ; 

} ; 

/* EOF jahtracer.h */

#endif // #ifndef JAHTRACER_H
