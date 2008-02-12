/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHDATAIO_H
#define JAHDATAIO_H

#include <qstring.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "corestructures.h"
#include "coreobject.h"

#include <fstream>
using namespace std;

class JahDataIO 
{

public:
	JahDataIO();

	~JahDataIO();

	////////////////////////////////////////////////////////////////
	//routines to load a world in
	static bool readFileIntoBuffer(char*& buffer, char* file_name);

	/////////////////////////////////////////////////////////////////
    // file io parseing routines

    static QString	JahParse(QString message);
    static QString	JahParse(QString message, int value);
    static QString	JahParse(QString message, float value);
    static QString	JahParse(QString message, QString value);
    static QString	JahParse(QString message, ObjectCategory::TYPE cat );
    static QString	JahParse(QString message, LayerCategory::TYPE cat );
    static QString	JahParse(QString debugmessage, VideoCategory::TYPE cat);
    static QString	JahParse(QString debugmessage, VideoType::TYPE cat);

    static int		ParseDataInt(QString data);
    static QString	ParseDataString(QString data); 
    static QString	ParseWinLocationString(QString data); //hack for windows C: / D: drive names
    static QString	ParseWinDataString(QString data);
    static QString	ParseDataFilename(QString data);
    static float	ParseDataFloat(QString data);

	//from charutilities
	static int const c_max_path = 4096;

	static char * stripExtension(					// ao_dest_name and a_source_name can be the same
		char * const & ao_dest_name,		// make sure it is allocated, use c_max_path
		char const * const & a_source_name);

	static char * getExtension(					// ao_dest_extension and a_source_name can be the same
		char * const & ao_dest_extension,	// make sure it is allocated, use c_max_path
		char const * const & a_source_name);

	static bool checkExtension ( 
		char const * const & a_filename,
		char const * const & a_extension);

	static char * insureExtension(							// ao_dest_name_with_extension and a_source_name can be the same
		char * const & ao_dest_name_with_extension, // make sure it is allocated, use c_max_path
		char const * const & a_source_name,			
		char const * const & a_extension);

	static char * getPathWithoutFinalSlash(		// ao_dest_path and a_source_name can be the same
		char * const & ao_dest_path,		// make sure it is allocated, use c_max_path
		char const * const & a_source_name);

	static char * toLowercaseInPlace(char * const & ai_string);



private:
	////////////////////////////////////////
	//pointer to tracer singleton
	//JahTrace			* jtrace;

	//JahLayer * thelayer;	
    //QString jahLoadfilename; //need a routine to set this

};


#endif






