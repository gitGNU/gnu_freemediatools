/*******************************************************************************
**
** The header file for the Jahshaka fontmanager module 
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

//this is how we do it
//FTGLTextureFont* myFont = FTGLFontManager::Instance().GetFont( "arial.ttf", 72);

#include    <map>
#include    <string>
#include    "FTGLTextureFont.h"

class FTGLFontManager
{
    public:
        static FTGLFontManager& getInstance();
        ~FTGLFontManager();

        FTFont* GetFont( const char *filename, int size);
    
        
    private:
        // Hide these 'cause this is a singleton.
        FTGLFontManager(){}
        FTGLFontManager( const FTGLFontManager&){};
        FTGLFontManager& operator = ( const FTGLFontManager&){ return *this;};
        
        // container for fonts
        FontList fonts;
    
};
