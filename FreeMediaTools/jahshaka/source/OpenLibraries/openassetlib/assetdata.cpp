/*******************************************************************************
**
** The source file for the Jahshaka videodata library
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable : 4786)
#endif

// System Includes
#include <map> 
#include <string> 
#include <fstream> 

// Own Include
#include "assetdata.h"

std::vector<std::string>   VideoCategory::m_type_string_vector(VideoCategory::NOCATEGORY + 1);
static VideoCategory video_category_init;
VideoCategory::VideoCategory()
{
    m_type_string_vector[IMAGE] = "IMAGE";
    m_type_string_vector[KEY] = "KEY";
    m_type_string_vector[CLIP] = "CLIP";
    m_type_string_vector[CLIPKEY] = "CLIPKEY";
    m_type_string_vector[AUDIO] = "AUDIO";
    m_type_string_vector[MEDIA] = "MEDIA";
    m_type_string_vector[NOCATEGORY] = "NOCATEGORY";
}

VideoCategory::TYPE
VideoCategory::getType(std::string& type_name)
{
    for (size_t i = 0; i < m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOCATEGORY;
}

std::vector<std::string>   VideoType::m_type_string_vector(VideoType::NOTYPE + 1);
static VideoType video_type_init;
VideoType::VideoType()
{
    m_type_string_vector[STILL] = "STILL";
    m_type_string_vector[SEQUENCE] = "SEQUENCE";
    m_type_string_vector[MOVIE] = "MOVIE";
    m_type_string_vector[AVI] = "AVI";
    m_type_string_vector[MPEG] = "MPEG";
    m_type_string_vector[MP3] = "MP3";
    m_type_string_vector[AIFF] = "AIFF";
    m_type_string_vector[NOTYPE] = "NOTYPE";
}

VideoType::TYPE
VideoType::getType(std::string& type_name)
{
    for (size_t i = 0; i < m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOTYPE;
}



// Other Includes

// defines
#define TOMAP(x,y,z) x[z]=#z;y[#z]=z
#define STRINGDATA(X) ( X.data() ? X.data() : "" )  


// set a map to retrieve enums from their string
static std::map<std::string,VideoCategory::TYPE> * videoCategoryMapper ( )
{

    static std::map<VideoCategory::TYPE,std::string> lEnumToString ;
    static std::map<std::string,VideoCategory::TYPE> lStringToEnum ;
#if 0
	// order here doesn't matter
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::IMAGE) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::KEY) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::CLIP) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::CLIPKEY) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::AUDIO) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::MEDIA) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoCategory::NOCATEGORY) ; 

	gVideoCategoryEnumToString = & lEnumToString ;
#endif
	return & lStringToEnum ;
}
std::map<VideoCategory::TYPE,std::string> * gVideoCategoryEnumToString ;
std::map<std::string,VideoCategory::TYPE> * gVideoCategoryStringToEnum = videoCategoryMapper ( ) ;

// set a map to retrieve enums from their string
static std::map<std::string,VideoType::TYPE> * videoTypeMapper ( )
{
    static std::map<VideoType::TYPE,std::string> lEnumToString ;
    static std::map<std::string,VideoType::TYPE> lStringToEnum ;
#if 0
	// order here doesn't matter
    TOMAP(lEnumToString,lStringToEnum,VideoType::STILL) ; 
    TOMAP(lEnumToString,lStringToEnum,VideoType::SEQUENCE) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::MOVIE) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::AVI) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::MPEG) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::MP3) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::AIFF) ; 
	TOMAP(lEnumToString,lStringToEnum,VideoType::NOTYPE) ; 

	gVideoTypeEnumToString = & lEnumToString ;
#endif
	return & lStringToEnum ;
}
std::map<VideoType::TYPE,std::string> * gVideoTypeEnumToString ;
std::map<std::string,VideoType::TYPE> * gVideoTypeStringToEnum = videoTypeMapper ( ) ;


void assetData::saveXML( std::fstream & ai_fstream ) 
{
	ai_fstream << "<ASSETDATA\n" ;

	ai_fstream << " theCategory = '" << (*gVideoCategoryEnumToString)[theCategory] << "'\n" ;  
	ai_fstream << " theType = '" << (*gVideoTypeEnumToString)[theType] << "'\n" ;  

	ai_fstream << " clipname = '" << STRINGDATA(clipname) << "'\n" ;  
	ai_fstream << " clipnumber = '" << clipnumber << "'\n" ;  

	ai_fstream << " location = '" << STRINGDATA(location) << "'\n" ;  
	ai_fstream << " filename = '" << STRINGDATA(filename) << "'\n" ;  
	ai_fstream << " extension = '" << STRINGDATA(extension) << "'\n" ;  

	ai_fstream << " startframe = '" << startframe << "'\n" ;  
	ai_fstream << " endframe = '" << endframe << "'\n" ;  
	ai_fstream << " clipinframe = '" << clipinframe << "'\n" ;  
	ai_fstream << " clipoutframe = '" << clipoutframe << "'\n" ;  
	ai_fstream << " videotex = '" << videotex << "'\n" ;  
	ai_fstream << " videoext = '" << STRINGDATA(videoext) << "'\n" ;  
   
	ai_fstream << " xpos = '" << xpos << "'\n" ;  
	ai_fstream << " ypos = '" << ypos << "'\n" ;  
   
	ai_fstream << " xsize = '" << xsize << "'\n" ;  
	ai_fstream << " ysize = '" << ysize << "'\n" ;  
   
	ai_fstream << " xNativeSize = '" << xNativeSize << "'\n" ;  
	ai_fstream << " yNativeSize = '" << yNativeSize << "'\n" ;  

	ai_fstream << " desktop = '" << STRINGDATA(desktop) << "'\n" ;  

	ai_fstream << " />" << "\n" ;     
}


void assetData::validate()
{
	//checks and corrects silly asset data values
	//just for debug
	if(startframe < 0)
	{
		startframe = 0;
	}

	if(endframe < 0)
	{
		endframe = 0;
	}

	if(clipinframe < 0)
	{
		clipinframe = 0;
	}

	if(clipoutframe < 0)
	{
		clipoutframe = 0;
	}

	if(startframe > endframe)
	{
		startframe = endframe;
	}


	if(clipinframe < startframe)
	{
		clipinframe = startframe;
	}

	if(clipoutframe > endframe)
	{
		clipoutframe = endframe;
	}
}

/////////////////////////////////////////////
//we need to get rid of these routines asap

QString assetData::getFullFileName() const
{
    QString loadfile;

    loadfile = location + filename + videoext + extension;

    return loadfile;
};

int  assetData::getMaxFrames() const
{
    int max = endframe;

    if (max == 0)
    {
        max = 1;
    }

    return max;
};

int assetData::getNumFrames() const
{
	return endframe-startframe + 1;
}

