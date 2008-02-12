/*******************************************************************************
**
** The header file for the Jahshaka fontmanager module 
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahfontmanager.h"

using namespace std;

typedef map< string, FTFont*> FontList;
typedef FontList::const_iterator FontIter;

static FTGLFontManager& getInstance()
{
    static FTGLFontManager tm;
    return tm;
}

~FTGLFontManager()
{
    FontIter font;
    for( font = fonts.begin(); font != fonts.end(); font++)
    {
        delete (*font).second;;
    }

    fonts.clear();
}


FTFont* GetFont( const char *filename, int size)
{
    char buf[256];
    sprintf(buf, "%s%i", filename, size);
    string fontKey = string(buf);

    FontIter result = fonts.find( fontKey);
    if( result != fonts.end())
    {
        LOGMSG( "Found font %s in list", filename);
        return result->second;
    }

    FTFont* font = new FTGLTextureFont;

    string fullname = path + string( filename);

    if( !font->Open( fullname.c_str()))
    {
        LOGERROR( "Font %s failed to open", fullname.c_str());
        delete font;
        return NULL;
    }

    if( !font->FaceSize( size))
    {
        LOGERROR( "Font %s failed to set size %i", filename, size);
        delete font;
        return NULL;
    }

    fonts[fontKey] = font;

    return font;

}

