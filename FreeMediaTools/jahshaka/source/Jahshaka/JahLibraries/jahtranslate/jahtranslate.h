/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHTRANS_H
#define JAHTRANS_H

#include    "qstring.h"
#include    "qsettings.h"


enum JahLanguage { ENGLISH, SPANISH, FRENCH, PORTUGUESE, GERMAN, ITALIAN };

class JahTranslate
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
    ~JahTranslate() throw() ;
        #endif

    private:

        static JahTranslate* _instance ;

		// These are needed to ensure that the singletons destructor is called on application shutdown.
		class JahTranslateDestroyer;
		static JahTranslateDestroyer _destroyer;

        // Hide these 'cause this is a singleton.       
        JahTranslate() throw() ; 

        #ifndef WIN32
        ~JahTranslate() throw() ;    
        #endif

        JahTranslate( const JahTranslate&);
        JahTranslate& operator = ( const JahTranslate&) ;

		JahLanguage theLanguage;

    public:
        //gets a instance of this object
        static JahTranslate* getInstance() throw();

		//sets the language
        bool setLanguage ( QString text ) throw(); 
        
		//converts a string
		QString tr       ( QString text ) throw();


		//the translators
        QString translateFrench     ( QString text ) throw();;
        QString translatePortuguese ( QString text ) throw(); 
        QString translateSpanish    ( QString text ) throw(); 
        QString translateGerman     ( QString text ) throw(); 
        QString translateItalian ( QString text ) throw(); 

};

#endif

