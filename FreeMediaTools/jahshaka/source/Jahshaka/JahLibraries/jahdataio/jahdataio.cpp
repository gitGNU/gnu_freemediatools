/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahdataio.h"


//==============================================================================
JahDataIO::JahDataIO()
{
	//set up tracer
	//jtrace = JahTrace::getInstance();	
	//set up tracer
}

JahDataIO::~JahDataIO()
{
	//
	//
	//
}

bool         
JahDataIO::readFileIntoBuffer(char*& buffer, char* file_name)
{
    ifstream input_file;
    input_file.open(file_name);

    if (!input_file)
    {
        return false;
    }

    input_file.seekg(0, ios::end);
    long file_size = input_file.tellg();
    input_file.seekg(0, ios::beg);


    buffer = new char[file_size+1];
    input_file.read(buffer, file_size);
    buffer[file_size] = '\0';

    return true;
}

//////////////////////////////////////////////////////////////////
// Paresing utilities used in the save and load routines

QString JahDataIO::JahParse(QString debugmessage)
{
    return debugmessage;
}

QString JahDataIO::JahParse(QString debugmessage, int value)
{
    QString stringval; debugmessage += stringval.setNum(value);

    return debugmessage;
}

QString JahDataIO::JahParse(QString debugmessage, LayerCategory::TYPE category)
{
    std::string name = LayerCategory::getName(category);
    QString name_qstring = QString( name.data() );  
    debugmessage += name_qstring;

    return debugmessage;
}

QString JahDataIO::JahParse(QString debugmessage, ObjectCategory::TYPE category)
{
    std::string name = ObjectCategory::getName(category);
    QString name_qstring = QString( name.data() );  
    debugmessage += name_qstring;

    return debugmessage;
}

QString JahDataIO::JahParse(QString debugmessage, VideoCategory::TYPE category)
{
    std::string name = VideoCategory::getName(category);
    QString name_qstring = QString( name.data() );  
    debugmessage += name_qstring;

    return debugmessage;
}


QString JahDataIO::JahParse(QString debugmessage, VideoType::TYPE type)
{
    std::string name = VideoType::getName(type);
    QString name_qstring = QString( name.data() );  
    debugmessage += name_qstring;

    return debugmessage;
}


QString JahDataIO::JahParse(QString debugmessage, float value)
{
    QString stringval; debugmessage += stringval.setNum(value);

    return debugmessage;
}

QString JahDataIO::JahParse(QString debugmessage, QString value)
{
    debugmessage += value;

    return debugmessage;
}

//////////////////////////////////////////////////////////////////
// Conversion utilities used in the save and load routines


/* these are the conversions
short toShort  (bool*ok=0, intbase=10)const
ushorttoUShort (bool*ok=0, intbase=10)const
int   toInt    (bool*ok=0, intbase=10)const
uint  toUInt   (bool*ok=0, intbase=10)const
long  toLong   (bool*ok=0, intbase=10)const
ulong toULong  (bool*ok=0, intbase=10)const
float toFloat  (bool*ok=0)const
doubletoDouble (bool*ok=0)const
*/

int JahDataIO::ParseDataInt(QString data)
{

	int index,jitlen;

	index	= data.findRev(":");   index +=1;     //find the colon and skip over it
	jitlen = data.length();    data = data.right(jitlen - index) ;
	int integer = data.toInt();
	return integer;
}

float JahDataIO::ParseDataFloat(QString data)
{

	int index,jitlen;

	index	= data.findRev(":");   index +=1;     //find the colon and skip over it
	jitlen = data.length();    data = data.right(jitlen - index) ;
	float integer = data.toFloat();
	return integer;
}

QString JahDataIO::ParseDataString(QString data) 
{

	int index,jitlen;

	index	= data.findRev(":");   
	index +=1;     //find the colon and skip over it
	jitlen = data.length();    data = data.right(jitlen - index) ;
	return data;
}

//this is te new hacvk for c: and d: stuff
QString JahDataIO::ParseWinLocationString(QString data) 
{
debug("loogin at:"+data);

	int index,jitlen;  // itsat:C:

	index	= data.findRev(":");   //location from left to right of the first colon

		if (data.find(':',0) == index)
		{
			//its not a drive letter
			index +=1;     //find the colon and skip over it
		}
		else
		{
			//its a drive letter
			index -=1;
		}

	//index +=1;     //find the colon and skip over it
	jitlen = data.length();    data = data.right(jitlen - index) ;
	return data;
}

//used for windows paths where the drive letter messes up our search as it has a :
QString JahDataIO::ParseWinDataString(QString data)
{
	int index,jitlen;

	index	= data.find(":");   
	index	+=1;     //find the colon and skip over it

	jitlen	= data.length();    
	data	= data.right(jitlen - index) ;
	
	return data;
}

QString JahDataIO::ParseDataFilename(QString data)
{ //caution, on unix this may be a  \ backslash

	int index,jitlen;

	index	= data.findRev("/");   index +=1;     //find the slash and skip over it
	jitlen = data.length();    data = data.right(jitlen - index) ;
	jitlen = data.length();    data = data.left(jitlen - 4) ; //file extension is four characters including the dot .jsf
	return data;
}


