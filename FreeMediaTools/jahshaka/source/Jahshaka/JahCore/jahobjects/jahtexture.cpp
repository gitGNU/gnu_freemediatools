/*******************************************************************************
**
** The source file for the Jahshaka Core jahtexture module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "coreobject.h"
#include "gpumathlib.h"


////////////////////////////////////////////////////////////
// build, initialize and rebuild textures

//this one works directly on the motionPath's predefined textureData
//and is quicker
void JahLayer::updateGlTexture (  QImage & buf  ) 
{
    //we should check if a images is 24 or 32 bit here for the texture module
    int mapwidth =  buf.width();
    int mapheight = buf.height();

    int mapvalw, mapvalh;

    getPowerOfTwoTextureSize(mapvalw, mapvalh, mapwidth, mapheight);

    textureData.objectImage   = buf.copy();

    //fix this to save on texture ram ie test image for bitdepth first
    int depth                 = buf.depth();
    textureData.bpp           = depth;        
    //textured.bpp            = 32;         // default was to 32bit but we need to test for this for efficiency

    textureData.wresolution   = mapvalw;    // was layerres
    textureData.hresolution   = mapvalh;    // was layerres
    textureData.width         = mapwidth;
    textureData.height        = mapheight;

    textureData.qimage        = QImage(mapvalw, mapvalh, depth);

    textureData.texwidth      = ( textureData.width / textureData.wresolution);
    textureData.texheight     =  1- ( textureData.height / textureData.hresolution );

    textureData.texwidthratio = textureData.width / textureData.wresolution;
    textureData.texheightratio = textureData.height / textureData.hresolution;

    //mapping ratio to product video aspect 4:3
    //calculation used is 640/2=320 /10 = 32
    textureData.x = (mapwidth/2)/10;
    textureData.y = ((mapheight/2)/10) * 1.24; //aspect ratio corrections here

    textureData.mapwidth = (GLfloat)mapwidth / 2;
    textureData.mapheight= (GLfloat)mapheight / 2;

}

//havent figured out exactly why we need 2 of these routines
//this routine returns a TextureImage... where is it used
void
JahLayer::buildGlTexture (TextureImage& texture_image, QImage& buf) 
{
    //we should check if a images is 24 or 32 bit here for the texture module
    int mapwidth =  buf.width();
    int mapheight = buf.height();
    int mapvalw, mapvalh;

    getPowerOfTwoTextureSize(mapvalw, mapvalh, mapwidth, mapheight);

    texture_image.objectImage   = buf.copy();

    //fix this to save on texture ram ie test image for bitdepth first
    int depth         = buf.depth();
    texture_image.bpp           = depth;
    //texture_image.bpp           = 32;         // default was to 32bit but we need to test for this for efficiency

    texture_image.wresolution    = mapvalw;    // was layerres
    texture_image.hresolution    = mapvalh;    // was layerres

	texture_image.width         = mapwidth;
    texture_image.height        = mapheight;

    texture_image.qimage        = QImage(mapwidth, mapheight, depth);

    texture_image.texwidth      = ( texture_image.width / texture_image.wresolution);
    texture_image.texheight     =  1- ( texture_image.height / texture_image.hresolution );

    texture_image.texwidthratio = textureData.width / textureData.wresolution;
    texture_image.texheightratio = textureData.height / textureData.hresolution;

    //  calculation is           640/2=320 /10 = 32
    texture_image.x = (mapwidth/2)/10;
    texture_image.y = ((mapheight/2)/10) * 1.24; //aspect ratio corrections here

    //mapping ration to product video aspect 4:3
    texture_image.mapwidth = (GLfloat)mapwidth / 2;
    texture_image.mapheight= (GLfloat)mapheight / 2;
}

//would be great to make this faster with bitBlt
//but it doesnt seem to be working
void JahLayer::InitializeTexture(TextureImage& texture_image) 
{
    if (   texture_image.getPreviousWidth() != texture_image.qimage.width() 
        || texture_image.getPreviousHeight() != texture_image.qimage.height()
        || texture_image.getPreviousDepth() != texture_image.qimage.depth()
        )
    {
        texture_image.qimage.scale( int(texture_image.qimage.width() ), int(texture_image.qimage.height() ) );
        texture_image.qimage.convertDepth(32);
    }

    // QImages are upside down
    texture_image.qimage = texture_image.objectImage.mirror(false, true);
    texture_image.setPreviousWidth( texture_image.qimage.width() ); 
    texture_image.setPreviousHeight( texture_image.qimage.height() );
    texture_image.setPreviousDepth( texture_image.qimage.depth() );
}

void JahLayer::changeKeyStatus()
{
    //debug("change key");
    invertKey = !(invertKey);
    textureKey.objectImage.invertPixels();

    buildGlTexture (textureKey, textureKey.objectImage);
    InitializeTexture(textureKey);  

    makeTexture();

}


void JahLayer::setLayerTexture(void) 
{
    //debug("set layer");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  textureData.texID );
}

void JahLayer::bindKeyTexture(void) 
{
  jtrace->debug("set key");
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  textureKey.texID );
}

////////////////////////////////////////////////////////////
// make the texture itself

void JahLayer::makeTexture( void )
{
    if (textureData.texID)     
    {   
        glDeleteTextures(1, &textureData.texID); 
    }

    if ( getTextureComposite().texID )
    {
        glDeleteTextures(1, &m_texture_composite.texID); 
    }

    if ( getCategory() == ObjectCategory::LARGE_LAYER )
    {
        makeLargeTexture();
    }
    else
    {
        makeLayerTexture();
    }

    if (  keyAssetStatus == TRUE) 
    {
        if (textureKey.texID)     
        {   
            glDeleteTextures(1,& textureKey.texID);  
        }

        makeKeyTexture();
    }
}


void
JahLayer::makeLargeTexture()
{
    // Split into four smaller textures (safe for power of two textures)
    int image_width = (int)textureData.qimage.width( );//wresolution;
    int image_height = (int)textureData.qimage.height( );//hresolution;
    int left_image_width = (int)(textureData.qimage.width( ) / 2.0f );//wresolution / 2.0f);
    int right_image_width = image_width - left_image_width;
    int bottom_image_height = (int)(textureData.qimage.height( ) / 2.0f );//hresolution / 2.0f);
    int top_image_height = image_height - bottom_image_height;

    int left_texture_width;
    int right_texture_width;
    int bottom_texture_height;
    int top_texture_height;
    getPowerOfTwoTextureSize(left_texture_width, bottom_texture_height, left_image_width, bottom_image_height);
    getPowerOfTwoTextureSize(right_texture_width, top_texture_height, right_image_width, top_image_height);

    textureData.setTileImageWidth(0, left_image_width);
    textureData.setTileImageHeight(0, bottom_image_height);
    textureData.setTileTextureWidth(0, left_texture_width);
    textureData.setTileTextureHeight(0, bottom_texture_height);

    textureData.setTileImageWidth(1, right_image_width);
    textureData.setTileImageHeight(1, bottom_image_height);
    textureData.setTileTextureWidth(1, right_texture_width);
    textureData.setTileTextureHeight(1, bottom_texture_height);

    textureData.setTileImageWidth(2, right_image_width);
    textureData.setTileImageHeight(2, top_image_height);
    textureData.setTileTextureWidth(2, right_texture_width);
    textureData.setTileTextureHeight(2, top_texture_height);

    textureData.setTileImageWidth(3, left_image_width);
    textureData.setTileImageHeight(3, top_image_height);
    textureData.setTileTextureWidth(3, left_texture_width);
    textureData.setTileTextureHeight(3, top_texture_height);

    int buffer_size;

    buffer_size = left_texture_width * bottom_texture_height * 4;
    unsigned char* buffer = new unsigned char[buffer_size];

    createEmpty2DTexture(textureData.getTileTextureIdPtr(0), GL_BGRA, left_texture_width, bottom_texture_height);
    getRectangleSubImage(buffer, textureData.qimage.bits(), 0, 0, left_image_width, bottom_image_height, image_width, image_height, left_texture_width, false); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, left_texture_width, bottom_texture_height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    delete [] buffer;
    buffer_size = right_texture_width * bottom_texture_height * 4;
    buffer = new unsigned char[buffer_size];
    createEmpty2DTexture(textureData.getTileTextureIdPtr(1), GL_BGRA, right_texture_width, bottom_texture_height);
    getRectangleSubImage(buffer, textureData.qimage.bits(), left_image_width, 0, right_image_width, bottom_image_height, image_width, image_height, right_texture_width, false); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, right_texture_width, bottom_texture_height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    delete [] buffer;
    buffer_size = right_texture_width * top_texture_height * 4;
    buffer = new unsigned char[buffer_size];
    createEmpty2DTexture(textureData.getTileTextureIdPtr(2), GL_BGRA, right_texture_width, top_texture_height);
    getRectangleSubImage(buffer, textureData.qimage.bits(), left_image_width, bottom_image_height, right_image_width, top_image_height, image_width, image_height, left_texture_width, false); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, right_texture_width, top_texture_height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    delete [] buffer;
    buffer_size = left_texture_width * top_texture_height * 4;
    buffer = new unsigned char[buffer_size];
    createEmpty2DTexture(textureData.getTileTextureIdPtr(3), GL_BGRA, left_texture_width, top_texture_height);
    getRectangleSubImage(buffer, textureData.qimage.bits(), 0, bottom_image_height, left_image_width, top_image_height, image_width, image_height, right_texture_width, false); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, left_texture_width, top_texture_height, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    delete [] buffer;
}

void 
JahLayer::makeLayerTexture( void ) 
{   
    if (textureData.width == 0 || textureData.height == 0)
    {
        return;
    }

    // Copy the original class into the working one
    getTextureComposite() = textureData;
    GLint texture_width = (GLint)textureData.wresolution;
    GLint texture_height = (GLint)textureData.hresolution;

    ///////////////////////////////////////////////////////////
    // for themain texture

    glGenTextures(1, &textureData.texID);
    glBindTexture(GL_TEXTURE_2D,  textureData.texID);   // 2d texture (x and y size)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    //glBindTexture(GL_TEXTURE_2D,  textureData.texID); 
 
    glTexImage2D( GL_TEXTURE_2D, 0, 4, texture_width, 
        texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLint)textureData.width, 
        (GLint)textureData.height, GL_BGRA, GL_UNSIGNED_BYTE, textureData.qimage.bits());

    ///////////////////////////////////////////////////////////
    // This is an identical sized texture for caching composited results

    glGenTextures(1, &(getTextureComposite().texID) );
    glBindTexture(GL_TEXTURE_2D,  getTextureComposite().texID);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //glBindTexture(GL_TEXTURE_2D,  getTextureComposite().texID); 

    glTexImage2D( GL_TEXTURE_2D, 0, 4, texture_width, 
        texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLint)textureData.width, 
        (GLint)textureData.height, GL_BGRA, GL_UNSIGNED_BYTE, textureData.qimage.bits());
}

void JahLayer::makeKeyTexture( void ) 
{  //procedure to map textureimage to keytexture

    if (textureKey.width == 0 || textureKey.height == 0)
    {
        return;
    }

    QImage* qimage_ptr;
    qimage_ptr = &textureKey.qimage;

    // Safety check to make sure the glTexSubImage2D() call below doesn't overrun the qimage data buffer
    // Should be handled upstream somewhere before we get to this point
    if (qimage_ptr->depth() < 32)
    {
        *qimage_ptr = qimage_ptr->convertDepth(32);
    }

    glGenTextures(1, & textureKey.texID);
    glBindTexture(GL_TEXTURE_2D,  textureKey.texID);   // 2d texture (x and y size)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexImage2D( GL_TEXTURE_2D, 0, 4,  (GLint)textureKey.wresolution, 
        (GLint)textureKey.hresolution, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLint)textureKey.width, 
        (GLint)textureKey.height, GL_BGRA, GL_UNSIGNED_BYTE, textureKey.qimage.bits());                   
}

void JahLayer::updateTexture( void ) 
{
    QImage* qimage_ptr;
    qimage_ptr = &textureData.qimage;

    // Safety check to make sure the glTexSubImage2D() call below doesn't overrun the qimage data buffer
    // Should be handled upstream somewhere before we get to this point
    if (qimage_ptr->depth() < 32)
    {
        *qimage_ptr = textureData.qimage.convertDepth(32);
    }

    glBindTexture(GL_TEXTURE_2D,  textureData.texID);   // 2d texture (x and y size)

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, qimage_ptr->width(), qimage_ptr->height(),
                    GL_BGRA, GL_UNSIGNED_BYTE, qimage_ptr->bits());
}

void JahLayer::updateKeyTexture( void ) 
{
    QImage* qimage_ptr;
    qimage_ptr = &textureKey.qimage;

    // Safety check to make sure the glTexSubImage2D() call below doesn't overrun the qimage data buffer
    // Should be handled upstream somewhere before we get to this point
    if (qimage_ptr->depth() < 32)
    {
        *qimage_ptr = textureKey.qimage.convertDepth(32);
    }

    glBindTexture(GL_TEXTURE_2D,  textureKey.texID);   // 2d texture (x and y size)

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, qimage_ptr->width(), qimage_ptr->height(),
                    GL_BGRA, GL_UNSIGNED_BYTE, qimage_ptr->bits());
}


///////////////////////////////////////////////////////////////////////////////
// for the particle system

inline float JahLayer::Gaussian2(float x, float y, float sigma)
{
    // The sqrt of 2 pi.
#define SQRT2PI 2.506628274631000502415765284811045253006

    //exp is the exponential function
    return exp(-0.5 * (x*x + y*y) / (sigma*sigma)) / (SQRT2PI * sigma);
}

////////////////////////////////////////////////////
// we can also use this to create brushes in paint
void JahLayer::MakeGaussianSpotTexture()
{
#define DIM 32
#define DIM2 (DIM>>1)
#define TEX_SCALE 7.0

    //glGenTextures(1, (GLuint *)&texture);
    //glBindTexture(GL_TEXTURE_2D, texture);

    glGenTextures(1, (GLuint *)&textureData.texID);
    glBindTexture(GL_TEXTURE_2D, textureData.texID);


    float *img = new float[DIM*DIM];

    for(int y = 0; y < DIM; y++)
    {
        for(int x = 0; x < DIM; x++)
        {
            if(x==0 || x==DIM-1 || y==0 || y==DIM-1)
            {
                img[(y * DIM) + x] = 0;
            }
            else
            {
                img[(y * DIM) + x] = TEX_SCALE * Gaussian2(x - DIM2, y - DIM2, (DIM * 0.15f) );
            }
        }
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    float col[4] = {1.f, 1.f, 1.f, 1.f};
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, col);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    //GL_ASSERT();

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA16, DIM, DIM, GL_ALPHA, GL_FLOAT, img);
    //gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, DIM, DIM, GL_ALPHA, GL_FLOAT, img);
}






















