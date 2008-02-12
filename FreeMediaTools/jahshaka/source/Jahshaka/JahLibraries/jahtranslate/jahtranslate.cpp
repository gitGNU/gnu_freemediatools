/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

//#include <iostream>

#include "jahtranslate.h"

class JahTranslate::JahTranslateDestroyer
{
private:
	JahTranslate* _pInstance;
public:
	JahTranslateDestroyer()						throw()	{	_pInstance = NULL;		};
	~JahTranslateDestroyer()					throw()	{	delete _pInstance;		};
	void SetInstance(JahTranslate* pInstance)	throw()	{	_pInstance = pInstance;	};
};

JahTranslate::JahTranslateDestroyer JahTranslate::_destroyer;

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

JahTranslate* JahTranslate::_instance=0 ;

JahTranslate::JahTranslate() 
    throw()
{   

    theLanguage = ENGLISH;
}

JahTranslate::~JahTranslate() 
    throw()
{

}

JahTranslate* JahTranslate::getInstance()
    throw()
{
    if (_instance == 0)  // is it the first call?
    {  
    _instance = new JahTranslate; // create sole instance
	_destroyer.SetInstance(_instance);
    }
    return _instance; // address of sole instance
}


/////////////////////////////////////////////////////////////////
//routines to set data start here

bool JahTranslate::setLanguage(QString text)
    throw()
{

    QString lang = text.lower();
    bool langchange = false;

    if ( lang == "english" )    { theLanguage = ENGLISH;    langchange = true; }
    if ( lang == "french" )     { theLanguage = FRENCH;     langchange = true; }
    if ( lang == "german" )     { theLanguage = GERMAN;     langchange = true; }
    if ( lang == "portuguese" ) { theLanguage = PORTUGUESE; langchange = true; }
    if ( lang == "spanish" )    { theLanguage = SPANISH;    langchange = true; }
    if ( lang == "italian" )    { theLanguage = ITALIAN;    langchange = true; }

    if (!langchange)            { theLanguage = ENGLISH; }

    return langchange;
}


//now we all the respective translator
QString JahTranslate::tr(QString text)
    throw()
{

    QString translated;

    switch(theLanguage) {

        case ENGLISH      : {  translated = text;                      break; }
        case FRENCH       : {  translated = translateFrench(text);     break; }
        case GERMAN       : {  translated = translateGerman(text);     break; }
        case PORTUGUESE   : {  translated = translatePortuguese(text); break; }
        case SPANISH      : {  translated = translateSpanish(text);    break; }
        case ITALIAN      : {  translated = translateItalian(text);    break; }

        default           : {  translated = text; break; }
    }

    return translated;
}

