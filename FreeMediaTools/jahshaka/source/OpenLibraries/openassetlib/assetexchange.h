/*******************************************************************************
**
** The header file for the Jahshaka assetexchange object
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef ASSETEXCHANGE_H
#define ASSETEXCHANGE_H

#include "assetdata.h"

#include <qstring.h>
#include <qimage.h>
#include <qfile.h>
#include <qdir.h>

#include <string.h>

class mediaData;
class assetData;
class OL_Trace;

/////////////////////////////////////////////////////////////
// need a lot of cleaning up here!
// needs to be a singleton (MJM)

class assetExchange
{
    public:

    //constructor and destructor
    assetExchange(void);
    ~assetExchange(void);
    
    bool openMedia( assetData & );


    //used to see if a mediaobject is valid (on disk)
    bool   isMediaValid(assetData &theasset);
    
    //media access routines
    bool   getMediaFilename(assetData &theasset, int frame, QString &returnName);
    static std::string resolveFullPath(assetData &ad, int frame);
    bool   getMediaLength(assetData &theasset, int &frame);
    
    QImage getImage(assetData &theasset, int frame=1);
    int    getImageWidth(assetData &theasset, int frame=1);
    int    getImageHeight(assetData &theasset, int frame=1);
    
    static QString getsequencefilename( assetData &theasset, int frame );
    
    //for streaming we keep the media objects state open and share it with the routines
    //we are missing a open stream and close strem - maybe put a close in the destructor?
    QImage* getStreamImagePtr(assetData &theasset, QImage& qimage, int frame = 1);
    
    //cant have a bool default here until debugged and merged with the routine above
    QImage getStreamImage(assetData &theasset, int frame,bool glstatus );
    
    QImage * getStreamImagePtr(assetData &theasset, int frame=1, bool glstatus = false);
    
    /////////////////////////////////////////////////////////
    // utilities to load assets from files
    bool getExtension(QString & string);
    bool checkExtension(QString & string);
    
    bool isMediaExtension(QString & string);
    bool isAudioExtension(QString & string);
    bool isImageExtension(QString & string);
    
    bool isanumber(QString & string);
    
    bool loadAsset(QString filename, bool loadit, assetData & newasset, int clipn=0);
    
    assetData loadJahMediaAsset(QString filename,QString shortName,
                                QString pathString,QString extension, 
                                int clipnumber, bool & itWorked );
    
    assetData loadJahImageAsset(QString filename,QString shortName,
                                QString pathString,QString extension, 
                                int clipnumber, bool & itWorked);
    
    assetData loadJahAudioAsset(QString filename,QString shortName,
                                QString pathString,QString extension, 
                                int clipnumber, bool & itWorked);
    
    assetData loadJahSequenceAsset(QString filename,QString shortName,
                                   QString pathString,QString extension,
                                   QString theextension, 
                                   int startFrame, int numberFrames, 
                                   int clipnumber, bool & loadit);
    
    //////////////////////////////////////////////////////////
    //utility routines used to package and export assets
    void convertToGLFormat( QImage& img );
    
    //////////////////////////////////////////////////////////
    //utility routines used to package and export assets
    void ExportAsset(assetData &asset, QString destination);
    
	bool getCorrectAspectRatio( ) { return m_correct_ar; }
	void setCorrectAspectRatio( bool on ) { m_correct_ar = on; }

private:
    bool   streamMediaintoimage( const QString loadfilename,  QImage & image, int frame);
    
    ////////////////////////////////////////
    //pointer to tracer singleton
    OL_Trace			* jtrace;
    
    QString JahBasePath;
    //QString filename;
    
    //internal utility routines
    bool   loadFileintoimage( const QString loadfilename, QImage & image);

    //we dont initialize this unless we are streaming...
    mediaData * openStream;

    bool streamOpen;
	bool m_correct_ar;

    //copy utility used by ExportAsset
    bool copyFile( const QString oldFilePath, const QString newFilePath);
    
    QString theFileName;

public:
    void saveXML( std::fstream & ai_fstream ) ; 
    
    QImage thecacheimage;
};

#endif






