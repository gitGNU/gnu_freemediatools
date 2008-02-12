/*******************************************************************************
**
** The header file for the Jahshaka core structures file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef CORESTRUCTURES_H
#define CORESTRUCTURES_H

// System Includes
#include <map> 
#include <string> 
#include <fstream> 
#include <qimage.h>

// Other Includes
#include "gpumathlib.h"

// Forward Declarations
class FTFont;


/////////////////////////////////////////////////////////
//core data structures
/////////////////////////////////////////////////////////

enum MouseMode { MROTATE, MTRANSLATE, MSCALE, MOFF };

struct desktopclip { int clip; };

/////////////////////////////////////////////////////////
//core layer structures
class LayerCategory
{
public:
    enum TYPE     
    { 
        LAYER, PARTICLE, OBJECT, FRAME, TEXT, 
        GRID, AXIS, LIGHT, TRACKER_POINT, NOT_A_TYPE
    };

    LayerCategory();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

class CompositeType
{
public:
    enum TYPE
    {
        A_COMPOSITE_TYPE,
        B_COMPOSITE_TYPE,
        A_OVER_B_COMPOSITE_TYPE,
        B_OVER_A_COMPOSITE_TYPE,
        A_IN_B_COMPOSITE_TYPE,
        B_IN_A_COMPOSITE_TYPE,
        A_OUT_B_COMPOSITE_TYPE,
        B_OUT_A_COMPOSITE_TYPE,
        A_ATOP_B_COMPOSITE_TYPE,
        B_ATOP_A_COMPOSITE_TYPE,
        A_XOR_B_COMPOSITE_TYPE,
        STANDARD_COMPOSITE_TYPE
    };

    CompositeType();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>     m_type_string_vector;
};


class ObjectCategory    
{ 
public:
    enum TYPE
    {
        JAHLAYER, MESHLAYER, LARGE_LAYER, MAINBACK, BACKDROP,
        BEZIERS, OBJECT, CUBE, 
        SPHERE, CYLINDER, JAHTEXT, TITLEGRID, 
        JAHPARTICLE, JAHAXIS, JAHLIGHT, JAHFOG, 
        EFFECT, TRACKER_POINT, NOT_A_TYPE
    };

    ObjectCategory();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
}; 



/////////////////////////////////////////////////////////
//core texture structure
//objectimage is the source image and texture is the image after formatting
//this makes the layers twice the size but is necessary since
//opengl doesnt suport non ^2 texture maps 
//maybe we can use the texID ? 
//opengl 1.2 supports non ^2 texture maps we can fix it then

class TextureImage 
{
public:
    //can we get rid of this qimage here?
    //replace with pointer to database???
    QImage       objectImage;                            // Image Data (Up To 32 Bits)
    GLuint       bpp;                                    // Image Color Depth In Bits Per Pixel.
    GLfloat      wresolution;                            // resolution for texture map
    GLfloat      hresolution;                            // resolution for texture map
    GLfloat      width;                                  // Image Width
    GLfloat      height;                                 // Image Height
    GLfloat      x;                                      // 3dspace x cord
    GLfloat      y;                                      // 3dspace y cord
    
    //can we get rid of this qimage here?
    //replace with pointer to database???
    QImage       qimage;                                 // the qimage
    GLfloat      texwidth;                               // texture Width for map
    GLfloat      texheight;                              // texture Height for map
    GLfloat      texwidthratio;                          // ratio of texture width to next largest power of two
    GLfloat      texheightratio;                         // ratio of texture height to next largest power of two
    GLuint       texID;                                  // Texture ID Used To Select A Texture
    GLuint       m_tile_texture_id[4];                   // Used for tiling into four sub-textures
    int          m_tile_texture_width[4];                // Used for tiling into four sub-textures
    int          m_tile_texture_height[4];               // Used for tiling into four sub-textures
    int          m_tile_image_width[4];                  // Used for tiling into four sub-textures
    int          m_tile_image_height[4];                 // Used for tiling into four sub-textures
    
    GLfloat      mapwidth;                               // Image mapping width
    GLfloat      mapheight;                              // Image mapping height

public:
	TextureImage( )
    	: bpp( 0 )
    	, wresolution( 0.0f )
    	, hresolution( 0.0f )
    	, width( 0.0f )
    	, height( 0.0f )
    	, x( 0.0f )
    	, y( 0.0f )
    	, texwidth( 0.0f )
    	, texheight( 0.0f )
    	, texwidthratio( 0.0f )
    	, texheightratio( 0.0f )
    	, texID( 0 )
    	, mapwidth( 0.0f )
    	, mapheight( 0.0f )
        , m_previous_width( 0 )
        , m_previous_height( 0 )
        , m_previous_depth( 0 )
	{ 
        //bool stop_here = true;
    }

    int         getTextureWidth() { return int(wresolution); }
    int         getTextureHeight() { return int(hresolution); }
    int         getImageWidth() { return int(width); }
    int         getImageHeight() { return int(height); }
    GLuint      getTexId() { return texID; }
    GLuint*     getTexIdPtr() { return &texID; } 
    GLuint      getTileTextureId(int index) { return m_tile_texture_id[index]; }
    GLuint*     getTileTextureIdPtr(int index) { return &(m_tile_texture_id[index]); } 
    int         getTileTextureWidth(int index) { return m_tile_texture_width[index]; }
    int         getTileTextureHeight(int index) { return m_tile_texture_height[index]; }
    int         getTileImageWidth(int index) { return m_tile_image_width[index]; }
    int         getTileImageHeight(int index) { return m_tile_image_height[index]; }
    void        setTileImageWidth(int index, int width) { m_tile_image_width[index] = width; }
    void        setTileImageHeight(int index, int height) { m_tile_image_height[index] = height; }
    void        setTileTextureWidth(int index, int width) { m_tile_texture_width[index] = width; }
    void        setTileTextureHeight(int index, int height) { m_tile_texture_height[index] = height; }
    void        getTextureRatios(float4& ratios);
    void        getTextureRatios(float2& ratios);
    int         getPreviousWidth() { return m_previous_width; }
    void        setPreviousWidth(int width) { m_previous_width = width; }
    int         getPreviousHeight() { return m_previous_height; }
    void        setPreviousHeight(int height) { m_previous_height = height; }
    int         getPreviousDepth() { return m_previous_depth; }
    void        setPreviousDepth(int depth) { m_previous_depth = depth; }

protected:
    int         m_previous_width;
    int         m_previous_height;
    int         m_previous_depth;

	//void saveXML( std::fstream & ai_fstream ) ;
};



/////////////////////////////////////////////////////////
//core font data structures

class FontDrawstyle     
{ 
public:
    enum TYPE
    {
        FBITMAP, FPIXMAP, FOUTLINE, FPOLYGON, FEXTRUDE, FTEXTURE, NOT_A_TYPE
    };

    FontDrawstyle();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

extern std::map<FontDrawstyle,std::string> * gFontDrawstyleEnumToString ;
extern std::map<std::string,FontDrawstyle> * gFontDrawstyleStringToEnum ;

class textObj 
{
public:
    QString     text;
    QString     font;
    double      extrude;
    int         size;
    FontDrawstyle::TYPE fontstyle;

    //for freetype fonts
    bool fontshere;
    int width;
    int height;

    bool bold;
    bool italic;
    bool underline;
    bool strikeout;

    int  pointsize;
    int  pixelsize;
    int  weight;
    int  pitch;
    int  stretch;

    //for freetype fonts
    FTFont* fonts[6];

    //for Qt font support
    QFont			*theqfont;

    QString     getText() { return text; };

	//void saveXML( std::fstream & ai_fstream ) ;
};




////////////////////////////////////////////////////////////
// for particle objects

class ParticleCategory
{
public:
    enum TYPE     
    { 
        ATOMIC, JETSPRAY, WATERFALL, ROCKET, EXPLOSION, SWIRL,
        BALLOONS, SHAPE, FIREFLIES, SHOWER, FOUNTAIN, NOT_A_TYPE 
    };

    ParticleCategory();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

extern std::map<ParticleCategory,std::string> * gParticleCategoryEnumToString ;
extern std::map<std::string,ParticleCategory> * gParticleCategoryStringToEnum ;

class ParticleDrawstyle
{
public:
    enum TYPE     
    { 
        PARLINE, PARPOINT, PARGAUSS, PARTEXTURE, NOT_A_TYPE
    };

    ParticleDrawstyle();
    static TYPE                 getType(std::string& type_name);
    static const std::string&   getName(TYPE type) { return m_type_string_vector[type]; }

protected:
    static std::vector<std::string>   m_type_string_vector;
};

extern std::map<ParticleDrawstyle,std::string> * gParticleDrawstyleEnumToString ;
extern std::map<std::string,ParticleDrawstyle> * gParticleDrawstyleStringToEnum ;

#endif





