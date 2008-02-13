/*******************************************************************************
**
** The source file for the Jahshaka core asset file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "coreobject.h"
#include "openmedialib.h"

// Convert ARGB to BGRA on big endian systems (will need to be modified for the systems that 
// are simply confused - intel mac being a case a point...).

static inline void convertToGLFormat( QImage& img )
{
	if ( img.depth( ) < 32 )
		img = img.convertDepth( 32 );

	if ( QImage::systemByteOrder() == QImage::BigEndian ) 
	{
		int height = img.height( );
		int width = img.width( );
		uint *p = (uint*)img.scanLine( 0 );
		uint *end = p + width;
		for ( int i = 0; i < height; i++ ) 
		{
			while ( p < end ) 
			{
				*p = ( *p << 24 ) | ( ( *p & 0xff00 ) << 8 ) | ( ( *p & 0xff0000 ) >> 8 ) | ( ( *p & 0xff000000 ) >> 24 );
				p++;
			}
			p = end;
			end += width;
		}
	}
}

//////////////////////////////////////////////////////////
//assign a asset to the layer object
//this can be audio, video, etc
void JahLayer::assignAsset(assetData clip, VideoCategory::TYPE cliptype, bool update_video_settings)
{
    if (cliptype == int(VideoCategory::IMAGE))
    {
        asset = clip;

        if ( !getInframeValid() || update_video_settings)
        {
            m_inframe = 1;
            m_outframe = clip.endframe - clip.startframe + 1;
        }

        layerAssetStatus = true;
    }

    if (cliptype == VideoCategory::KEY)
    {
      if(jtrace)
	jtrace->debug("ASSIGNING KEY");
        keyasset = clip;

        if ( !getInframeValid() || update_video_settings)
        {
            m_keyinframe = 1;
            m_keyoutframe = clip.endframe - clip.startframe + 1;
        }

        //this should be renamed to keyHasAsset;
        keyAssetStatus = TRUE;
    }

}

//////////////////////////////////////////////////////////////////
// initalizes a layer  as clip or key with a new media asset
// imagetype is either CLIP or KEY

bool JahLayer::LoadLayerAsset( VideoCategory::TYPE imagetype )
{
    //set up tracer
  if(jtrace)
      jtrace->debug( "JahLayer::","Loading Asset into Layer");


    QImage   buf;

    //initialize the media exchange if its not already here
    //this should be moved out so its not checked every time

    if (mediaExchangePresent)
    {
      if (mediaExchange)
	{
	  mediaExchangePresent = false;
	  delete mediaExchange;
	  mediaExchange = NULL;
	  setEffectsSlidersHaveChanged( true );
	}
      else
	{
	  // TODO : logic bomb
	}
    }
		
    addMediaObj();

    if (imagetype == VideoCategory::IMAGE)
    {
        buf = mediaExchange->getImage(asset, m_currentframe);
		convertToGLFormat( buf );

        updateGlTexture (buf);
        InitializeTexture(textureData);
    }

    if (imagetype == VideoCategory::NOCATEGORY)
    {
		//if its no-category we need to override
		//the image displayed
		//as we are using this for new layers
        if ( !buf.load( JahBasePath+"Pixmaps/jahlayer.png" ) ) {	// Load first image from file
	  if(jtrace)
	    jtrace->error( "Could not read default image, using single-color instead." );
            QImage dummy( 256, 256, 32 );
            dummy.fill( Qt::blue.rgb() );
            buf = dummy;
        }

        updateGlTexture (buf);
        InitializeTexture(textureData);
    }

    if (imagetype == VideoCategory::KEY)
    {
        //this should use the keyasset
        buf = mediaExchange->getImage(keyasset, m_currentframe);
		convertToGLFormat( buf );

        if (invertKey)
        {
            buf.invertPixels();
        }

        buildGlTexture (textureKey, buf);
        InitializeTexture(textureKey);
    }

    return true;

}

////////////////////////////////////////////////////////////
// video and image-sequencing routines
// updates the textures in 3d space from the asset texture


void JahLayer::updateVidTex(int theframe)
{
    //////////////////////////////////////////////////////////////////
    // if there is a layer asset

    if ( layerAssetStatus )
    {
        if ( updateVidTexAsset(theframe) )
        {
            updateTexture();
        }
    }

    //////////////////////////////////////////////////////////////////
    // if there is a key asset

    if ( keyAssetStatus )
    {
        if ( updateVidTexKeyasset(theframe) )
        {
            updateKeyTexture();
        }
    }
}

bool 
JahLayer::updateVidTexAsset(int frame)
{
    int current_frame = frame;

    const int start_frame = getSlipFrameOffset();
    const int time_delta = frame - start_frame;
    const int number_of_video_frames = m_outframe - m_inframe + 1;
    int cycle_number = time_delta / number_of_video_frames;
    const int frame_delta = time_delta % number_of_video_frames;
    int video_frame_number = m_inframe + frame_delta;
    
    if (loop)
    {
        if (video_frame_number < 0)
        {
            // This can happen if the slip value is greater than zero
            current_frame = m_outframe + frame_delta;
            // Force the cycle number to be odd
            cycle_number++;
        }
        else
        {
            current_frame = video_frame_number;
        }

        if ( ping && (cycle_number % 2) )
        {
            if (frame_delta >= 0)
            {
                current_frame = m_outframe - frame_delta;
            }
            else
            {
                current_frame = m_inframe - frame_delta;
            }
        }
    }
    else
    {
        if (time_delta < 0)
        {
            current_frame = m_inframe;
        }
        else if (time_delta >= number_of_video_frames)
        {
            current_frame = m_outframe;
        }
        else
        {
            current_frame = video_frame_number;
        }
    }

    mediaExchange->getStreamImagePtr(asset, textureData.objectImage, current_frame);
    convertToGLFormat( textureData.objectImage );
    InitializeTexture(textureData);

	setEffectsSlidersHaveChanged( true ); 

    return true;
}

bool JahLayer::updateVidTexKeyasset(int frame)
{
    int current_frame = frame;

    const int start_frame = getKeySlipFrameOffset();
    const int time_delta = frame - start_frame;
    const int number_of_video_frames = m_keyoutframe - m_keyinframe + 1;
    int cycle_number = time_delta / number_of_video_frames;
    const int frame_delta = time_delta % number_of_video_frames;
    int video_frame_number = m_keyinframe + frame_delta;
    
    if (loop)
    {
        if (video_frame_number < 0)
        {
            // This can happen if the slip value is greater than zero
            current_frame = m_outframe + frame_delta;
            // Force the cycle number to be odd
            cycle_number++;
        }
        else
        {
            current_frame = video_frame_number;
        }

        if ( ping && (cycle_number % 2) )
        {
            if (frame_delta >= 0)
            {
                current_frame = m_outframe - frame_delta;
            }
            else
            {
                current_frame = m_inframe - frame_delta;
            }
        }
    }
    else
    {
        if (time_delta < 0)
        {
            current_frame = m_keyinframe;
        }
        else if (time_delta >= number_of_video_frames)
        {
            current_frame = m_keyoutframe;
        }
        else
        {
            current_frame = video_frame_number;
        }
    }

    mediaExchange->getStreamImagePtr(keyasset, textureKey.objectImage, current_frame);
    convertToGLFormat(textureKey.objectImage);

    if (invertKey) 
    { 
        textureKey.objectImage.invertPixels(); 
    }
    
    InitializeTexture(textureKey);

	setEffectsSlidersHaveChanged( true ); 

    return true;
}


