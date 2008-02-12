/*******************************************************************************
**
** The header file for the Jahshaka videodata file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef ASSETDATA_H
#define ASSETDATA_H

// System Includes
#include <map> 
#include <string> 
#include <vector>
#include <qstring.h>
#include <qimage.h>
#include <qfile.h>

#ifdef AUDIOSUPPORT
#include "sndfile.h"
#endif

////////////////////////////////////////////////////////
// asset categories

class VideoCategory 
{ 
public:
    enum TYPE
    {
	    IMAGE, KEY, CLIP, CLIPKEY, AUDIO, MEDIA, NOCATEGORY 
    };

    VideoCategory();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

extern std::map<VideoCategory::TYPE,std::string> * gVideoCategoryEnumToString ;
extern std::map<std::string,VideoCategory::TYPE> * gVideoCategoryStringToEnum ;

class VideoType
{ 
public:
    enum TYPE
    {
	    STILL, SEQUENCE, MOVIE, AVI, MPEG, MP3, AIFF, NOTYPE 
    };

    VideoType();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

extern std::map<VideoType::TYPE,std::string> * gVideoTypeEnumToString ;
extern std::map<std::string,VideoType::TYPE> * gVideoTypeStringToEnum ;


//we store clip size data in here as well for easy access
struct mediaSize
{
    mediaSize()
        : width( 0 ),
          height( 0 )
    {}

    int         width;
    int         height;
};


//asset grows up and becomes a object
class assetData
{
public:
    //category informaiton
    VideoCategory::TYPE theCategory;
    VideoType::TYPE     theType;

    //for the clip
    QString     clipname;
    int         clipnumber;

    QString     location,filename;
    QString     extension;
    int         startframe, endframe;
    int         clipinframe, clipoutframe;
    bool        videotex;
    QString	videoext;

    std::vector<std::string> m_frame_name_vector;

    std::vector<std::string>& getFrameNameVector() { return m_frame_name_vector; }
   
    //desktop position & size data
    double      xpos;
    double      ypos;
    double      xsize;
    double      ysize;

    double      xNativeSize;
    double      yNativeSize;

    bool	locked;
    bool	filmstrip;
    int		frameOffset;
    int         blank_length;

    QString	desktop;

    assetData(void) 
    {
        //set up deafults
        startframe		= 0;	//SOM
        endframe		= 0;	//EOM
        clipinframe		= 0;	//In point
        clipoutframe	        = 0;	//Out point
        xpos			= 0;
        ypos			= 0;
        xsize			= 0;
        ysize			= 0;
        xNativeSize		= 0;
        yNativeSize		= 0;
        locked			= false;
        filmstrip		= false;
        frameOffset		= 0;
        clipnumber		=-1;
        blank_length            = 0;
        
        clipname = "";
        
        theCategory = VideoCategory::NOCATEGORY;
        theType = VideoType::NOTYPE;

        videotex = false;
    };

    ~assetData(void) {};

    bool operator <( assetData other )
    {
        return clipnumber < other.clipnumber;
    }

    //other routines
    int         getNumFrames(void) const;
    QString     getFullFileName() const;
    int         getMaxFrames() const;
    void 	validate();

    void saveXML( std::fstream & ai_fstream ); 

};


#ifdef AUDIOSUPPORT
struct AudioData
{
	QString clipname;
	QString filename;
	SNDFILE* sndfile_handle;
	SF_INFO file_info;
	unsigned long file_lenght;
};
#endif


#endif //ASSETDATA_H






