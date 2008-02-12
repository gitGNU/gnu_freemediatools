/*******************************************************************************
**              T H E        J A H S H A K A      P R O J E C T                *
**                                                                             *
**                        http://www.jahshaka.com                              *
**                                                                             *
** Created:  Fri Dec 1 23:53:52 2000                                           *
**      by:  Jah Shaka                                                         *
**   email:  jahshaka@users.sourceforge.net                                    *
**                                                                             *
** Copyright (C) 2000-2006 VM Inc.                                *
**                                                                             *
** This program is free software which we release under the GNU General Public *
** License. You may redistribute and/or modify this program under the terms    *
** of that license as published by the Free Software Foundation; either        *
** version 2 of the License, or (at your option) any later version.            *
**                                                                             *
** This program is distributed in the hope that it will be useful,             *
** but WITHOUT ANY WARRANTY; without even the implied warranty of              *
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
** GNU General Public License for more details.                                *
**                                                                             *
** To get a copy of the GNU General Puplic License, write to the Free Software *
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   *
**                                                                             *
*******************************************************************************/

#include <qapplication.h>

//for the main application
#ifdef JAHPLAYER
#include "jahplayer.h"
#else
#include "jahshaka.h"
#endif

#ifdef SPACEBALL
#include "spaceball.h"
#endif

#include "anime.h"
#include "jahscript.h"

int main(int argc, char** argv) 
{
    /////////////////////////////////////////////////////////
    // initialize the application
    std::string JahVersion="2.0";

    QApplication::setColorSpec( QApplication::ManyColor );
#ifdef Q_WS_MAC
    // On mac we have bizarre palette problems if we try to take any of the desktop default palette.
    // e.g. click away from the application, then back, the palette changes (this occurs even in the Qt
    // examples! This next line tells the app to not use any desktop palette to start with. All is then fine. 
    QApplication::setDesktopSettingsAware( false );
#endif


    //spaceball customizes the QApplication class
    #ifdef SPACEBALL
    App app(argc,argv);
    #else
    QApplication app(argc,argv);
    #endif

    ///////////////////////////////////////////////
    // Check for Qt support before starting
    #if QT_VERSION < 300
    qDebug( "We only support Qt Versions 3.0 and up. Exiting." );
    return -1;
    #endif

    ///////////////////////////////////////////////
    // Check for OpenGL support before starting
    if ( !QGLFormat::hasOpenGL() ) 
    {
        qDebug( "This system has no OpenGL support. Exiting." );
        return -1;
    };

    /////////////////////////////////////////////////////////
    // initialize the tracer
	JahTrace * jtrace;
    jtrace = JahTrace::getInstance();	//set up tracer

    //set the level of reporting for the tracer
    jtrace->setLevel ( JahTrace::LevelNothing );       //reports nothing

    //find out startup options
    QString jahoptions;

    bool debuglevel = false;
    bool debugsave  = false;
	bool maximize   = false;
	bool fullscreen = false;

    for ( int i = 0; i < app.argc(); i++ )
    {
        jahoptions += ( app.argv()[i] );
        jahoptions += ":";
	}

    //parse options
    if (jahoptions.contains("debug"))
    {
        debuglevel = true;    
        jtrace->setLevel ( JahTrace::LevelEverything );  //reports everything
    }
	
	if (jahoptions.contains("file"))
    {
        debugsave = true;

        //saves errors to log file in temp
        jtrace->setFileSave ( true ); 
    }
	
	if ( jahoptions.contains( "maximize" ) )
	{
		maximize = true;
	}
	else if ( jahoptions.contains( "fullscreen" ) )
	{
		fullscreen = true;
	}

    ////////////////////////////////////////////////
    //force overides for debugging here if you like
	#ifdef DEBUG
    jtrace->setLevel ( JahTrace::LevelEverything );  //reports everything
    //jtrace->setFileSave ( true ); 
	#endif

    //report the tracer initialization
    jtrace->info( "Starting Jahshaka System Tracer" );

    //report the runtime options
    jtrace->debug( "Using Runtime Options",jahoptions.data() );

    /////////////////////////////////////////////////////////
    // initialize the preferences

    JahPrefs& jprefs = JahPrefs::getInstance();
    jtrace->info( "Initializing Preferences" );

	//check the prefs for compatability
    jprefs.setJahVersion ( JahVersion );

    //if the user has upgraded the version wont match 
    jprefs.checkVersion();

	//load the prefss in now
    jprefs.loadPreferences();

    /////////////////////////////////////////////////////////
    // initialize the Translator
	JahTranslate * jt;
    jt = JahTranslate::getInstance();

	std::string lang = jprefs.getLanguage();
    jt->setLanguage(lang.data());

    jtrace->info( "Initialized Translator" );

    ///////////////////////////////////////////////
    // Create the Main object and Launch
    Jahshaka jahshaka;

    //check to see if we can initialize spaceball
    #ifdef SPACEBALL
    if ( !MagellanInit( jahshaka.x11AppDisplay(), jahshaka.winId() ) ) {
        jtrace->warn( "This system has no Spaceball Driver running." );
    };
    #endif

    app.setMainWidget(&jahshaka);

    if ( fullscreen )
    {
    	jahshaka.showFullScreen();
    }
    else if ( maximize )
    {
    	jahshaka.showMaximized();
    }
    else
    {
    	jahshaka.show();
    }

    return app.exec();


}
