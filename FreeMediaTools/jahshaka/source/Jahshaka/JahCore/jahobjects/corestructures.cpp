/*******************************************************************************
**
** The header file for the Jahshaka core structures file
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

// Own Include
#include <gpumathlib.h>
#include "corestructures.h"

std::vector<std::string> CompositeType::m_type_string_vector(CompositeType::STANDARD_COMPOSITE_TYPE + 1);
static CompositeType composite_type_init;
CompositeType::CompositeType()
{
        m_type_string_vector[A_COMPOSITE_TYPE] = "A";
        m_type_string_vector[B_COMPOSITE_TYPE] = "B";
        m_type_string_vector[A_OVER_B_COMPOSITE_TYPE] = "A over B";
        m_type_string_vector[B_OVER_A_COMPOSITE_TYPE] = "B over A";
        m_type_string_vector[A_IN_B_COMPOSITE_TYPE] = "A in B";
        m_type_string_vector[B_IN_A_COMPOSITE_TYPE] = "B in A";
        m_type_string_vector[A_OUT_B_COMPOSITE_TYPE] = "A out B";
        m_type_string_vector[B_OUT_A_COMPOSITE_TYPE] = "B out A";
        m_type_string_vector[A_ATOP_B_COMPOSITE_TYPE] = "A atop B";
        m_type_string_vector[B_ATOP_A_COMPOSITE_TYPE] = "B atop A";
        m_type_string_vector[A_XOR_B_COMPOSITE_TYPE] = "A xor B";
        m_type_string_vector[STANDARD_COMPOSITE_TYPE] = "Standard";
}

CompositeType::TYPE
CompositeType::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return STANDARD_COMPOSITE_TYPE;
}

std::vector<std::string>   ParticleCategory::m_type_string_vector(ParticleCategory::NOT_A_TYPE + 1);
static ParticleCategory particle_category_init;
ParticleCategory::ParticleCategory()
{
    m_type_string_vector[ATOMIC] = "ATOMIC";
    m_type_string_vector[JETSPRAY] = "JETSPRAY";
    m_type_string_vector[WATERFALL] = "WATERFALL";
    m_type_string_vector[ROCKET] = "ROCKET";
    m_type_string_vector[EXPLOSION] = "EXPLOSION";
    m_type_string_vector[SWIRL] = "SWIRL";
    m_type_string_vector[BALLOONS] = "BALLOONS";
    m_type_string_vector[SHAPE] = "SHAPE";
    m_type_string_vector[FIREFLIES] = "FIREFLIES";
    m_type_string_vector[SHOWER] = "SHOWER";
    m_type_string_vector[FOUNTAIN] = "FOUNTAIN";
    m_type_string_vector[NOT_A_TYPE] = "NOT_A_TYPE";
}



std::vector<std::string>   ParticleDrawstyle::m_type_string_vector(ParticleDrawstyle::NOT_A_TYPE + 1);
static ParticleDrawstyle particle_drawstyle_init;
ParticleDrawstyle::ParticleDrawstyle()
{
    m_type_string_vector[PARLINE] = "PARLINE";
    m_type_string_vector[PARPOINT] = "PARPOINT";
    m_type_string_vector[PARGAUSS] = "PARGAUSS";
    m_type_string_vector[PARTEXTURE] = "PARTEXTURE";
    m_type_string_vector[NOT_A_TYPE] = "NOT_A_TYPE";
}


// Other Includes

std::vector<std::string>   FontDrawstyle::m_type_string_vector(FontDrawstyle::NOT_A_TYPE + 1);
static FontDrawstyle font_drawstyle_init;
FontDrawstyle::FontDrawstyle()
{
    m_type_string_vector[FBITMAP] = "FBITMAP";
    m_type_string_vector[FPIXMAP] = "FPIXMAP";
    m_type_string_vector[FOUTLINE] = "FOUTLINE";
    m_type_string_vector[FPOLYGON] = "FPOLYGON";
    m_type_string_vector[FEXTRUDE] = "FEXTRUDE";
    m_type_string_vector[FTEXTURE] = "FTEXTURE";
    m_type_string_vector[NOT_A_TYPE] = "NOT_A_TYPE";
}

std::vector<std::string>   LayerCategory::m_type_string_vector(LayerCategory::NOT_A_TYPE + 1);
static LayerCategory layer_category_init;
LayerCategory::LayerCategory()
{
    m_type_string_vector[LAYER] = "LAYER";
    m_type_string_vector[PARTICLE] = "PARTICLE";
    m_type_string_vector[OBJECT] = "OBJECT";
    m_type_string_vector[FRAME] = "FRAME";
    m_type_string_vector[TEXT] = "TEXT";
    m_type_string_vector[GRID] = "GRID";
    m_type_string_vector[AXIS] = "AXIS";
    m_type_string_vector[LIGHT] = "LIGHT";
    m_type_string_vector[TRACKER_POINT] = "TRACKER_POINT";
    m_type_string_vector[NOT_A_TYPE] = "NOT_A_TYPE";
}

std::vector<std::string>   ObjectCategory::m_type_string_vector(ObjectCategory::NOT_A_TYPE + 1);
static ObjectCategory object_category_init;
ObjectCategory::ObjectCategory()
{
    m_type_string_vector[JAHLAYER] = "JAHLAYER";
    m_type_string_vector[MESHLAYER] = "MESHLAYER";
    m_type_string_vector[LARGE_LAYER] = "LARGE_LAYER";
    m_type_string_vector[MAINBACK] = "MAINBACK";
    m_type_string_vector[BACKDROP] = "BACKDROP";
    m_type_string_vector[BEZIERS] = "BEZIERS";
    m_type_string_vector[OBJECT] = "OBJECT";
    m_type_string_vector[CUBE] = "CUBE";
    m_type_string_vector[SPHERE] = "SPHERE";
    m_type_string_vector[CYLINDER] = "CYLINDER";
    m_type_string_vector[JAHTEXT] = "JAHTEXT";
    m_type_string_vector[TITLEGRID] = "TITLEGRID";
    m_type_string_vector[JAHPARTICLE] = "JAHPARTICLE";
    m_type_string_vector[JAHAXIS] = "JAHAXIS";
    m_type_string_vector[JAHLIGHT] = "JAHLIGHT";
    m_type_string_vector[JAHFOG] = "JAHFOG";
    m_type_string_vector[EFFECT] = "EFFECT";
    m_type_string_vector[TRACKER_POINT] = "TRACKER_POINT";
    m_type_string_vector[NOT_A_TYPE] = "NOT_A_TYPE";
}

ParticleCategory::TYPE
ParticleCategory::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOT_A_TYPE;
}

ParticleDrawstyle::TYPE
ParticleDrawstyle::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOT_A_TYPE;
}

FontDrawstyle::TYPE
FontDrawstyle::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOT_A_TYPE;
}

LayerCategory::TYPE
LayerCategory::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOT_A_TYPE;
}

ObjectCategory::TYPE
ObjectCategory::getType(std::string& type_name)
{
    for (int i = 0; i < (int)m_type_string_vector.size(); i ++)
    {
        if (m_type_string_vector[i] == type_name)
        {
            return TYPE(i);
        }
    }

    return NOT_A_TYPE;
}


void 
TextureImage::getTextureRatios(float4& ratios)
{
    ratios.x = texwidthratio;
    ratios.y = texheightratio;
}


void 
TextureImage::getTextureRatios(float2& ratios)
{
    ratios.x = texwidthratio;
    ratios.y = texheightratio;
}

TextureImage::TextureImage( )
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

int         TextureImage::getImageWidth()
{
  return int(width);
}

int         TextureImage::getImageHeight()
{
  return int(height);
}

GLuint
TextureImage::getTexId()
{
  return texID;
}

GLuint*     TextureImage::getTexIdPtr()
{
  return &texID; 
}

GLuint*    
TextureImage::getTileTextureIdPtr(int index)
{
  return &(m_tile_texture_id[index]);
}

GLuint      TextureImage::getTileTextureId(int index) 
{
  return m_tile_texture_id[index]; 
}

QString textObj::getText()
{
  return text;
};
