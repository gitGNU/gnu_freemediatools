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
/*
  this crashes :
  
*/
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




/*
test= {
  m_effects_list = 0x8d9b6f0,
  m_lights_list = 0x8682e50,
  m_camera_layer = 0x1e,
  m_check_list_item = 0x8682f58,
  m_layer_list_entry = 0x1f,
  m_effect_layer = 0x49,
  m_plugin_number = 1,
  m_parent = 0x0,
  m_is_chroma_key_layer = false,
  m_contains_cpu_effects = false,
  m_effects_sliders_have_changed = false,
  m_effects_have_key_frames = false,
  m_previous_text_extrude = 0,
  m_use_pbuffer = false,
  m_stabilizing_state = 0x0,
  m_light0_ambient =
  {
    <float4> = {x = 0, y = 0, z = 0,w = 0},
    <No data fields>},
  m_light0_diffuse = {<float4> = {x = 0, y = 0, z = 0,
				  w = 0}, <No data fields>},
  m_light0_specular = {<float4> = {x = 0, y = 2.66246708e-44,
				   z = 4.20389539e-44, w = 1.02294788e-43}, <No data fields>},
  m_light0_position = { x = 1.40129846e-45, y = 0, z = 0, w = 0},
  m_light0_shininess = 0,
  m_translate_first = false, m_previous_alpha = 0,
  m_composite_texture_updated_by_lighting = false,
  m_effects_updated = false,
  jplugin = 0x0,
  usedagpu = false,
  objtype = ObjectCategory::BACKDROP,
  layertype = LayerCategory::LAYER,

  layername = {static null = {static null = <same as static member of an already seen type>,
			      d = 0x85257e0, static shared_null = 0x85257e0},
	       d = 0x8682e38,
	       static shared_null = 0x85257e0}, m_layer_name_string = {static npos = 4294967295,
								       _M_dataplus = {<std::allocator<char>> = {<__gnu_cxx::new_allocator<char>> = {<No data fields>}, <No data fields>}, _M_p = 0x0}},

  layerStatus = false, selected = false,
  drawtheLayer = true,
  drawtheKey = false,
  invertKey = 24,

  asset = {
    theCategory = 31,
    theType = 73,
    clipname = {static null = {static null = <same as static member of an already seen type>, d = 0x85257e0,  static shared_null = 0x85257e0},
		d = 0x1, static shared_null = 0x85257e0},
    clipnumber = 0, location = {static null = {static null = <same as static member of an already seen type>, d = 0x85257e0, static shared_null = 0x85257e0},
				d = 0x0, static shared_null = 0x85257e0},
    filename = {static null = {
      static null = <same as static member of an already seen type>, d = 0x85257e0,
      static shared_null = 0x85257e0},
		d = 0x0, static shared_null = 0x85257e0},

    extension = {static null = {
      static null = <same as static member of an already seen type>, d = 0x85257e0,
      static shared_null = 0x85257e0},
		 d = 0x0, static shared_null = 0x85257e0},

    startframe = 0, endframe = 0, clipinframe = 0, clipoutframe = 0, videotex = false,

    videoext = {static null = {
      static null = <same as static member of an already seen type>, d = 0x85257e0,
      static shared_null = 0x85257e0},
		d = 0x0, static shared_null = 0x85257e0},

    m_frame_name_vector = {<std::_Vector_base<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >> = {
      _M_impl = {<std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >> = {<__gnu_cxx::new_allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >> = {<No data fields>},
													      <No data fields>}, _M_start = 0x0,
		 _M_finish = 0x0, _M_end_of_storage = 0x0}
    }, <No data fields>},
  }

  xpos = 5.9415882147027625e-313, ypos = 1.0397779377212031e-312,
  xsize = -3.6176512501546781e-44, ysize = 0, xNativeSize = -nan(0xfffff00000000),
  yNativeSize = 0, locked = 31, filmstrip = false, frameOffset = 31, blank_length = 48,
  desktop = {static null = {static null = <same as static member of an already seen type>,
			    d = 0x85257e0, static shared_null = 0x85257e0},
	     d = 0x11,
	     static shared_null = 0x85257e0}
}, keyasset = {theCategory = 141004976,
	       theType = 141004976, clipname = {static null = {
		 static null = <same as static member of an already seen type>, d = 0x85257e0,
		 static shared_null = 0x85257e0}
						, d = 0x8681e58, static shared_null = 0x85257e0},
	       clipnumber = 17, location = {static null = {
		 static null = <same as static member of an already seen type>, d = 0x85257e0,
		 static shared_null = 0x85257e0}
					    , d = 0x86790d0, static shared_null = 0x85257e0},
	       filename = {static null = {
		 static null = <same as static member of an already seen type>, d = 0x85257e0,
		 static shared_null = 0x85257e0}
			   , d = 0x86790d0, static shared_null = 0x85257e0},
	       extension = {static null = {
		 static null = <same as static member of an already seen type>, d = 0x85257e0,
		 static shared_null = 0x85257e0}
			    , d = 0x8681e58, static shared_null = 0x85257e0},
	       startframe = 17, endframe = 141006960, clipinframe = 141006960,
	       clipoutframe = 141041240, videotex = 17, videoext = {static null = {
		 static null = <same as static member of an already seen type>, d = 0x85257e0,
		 static shared_null = 0x85257e0}
								    , d = 0x8679890, static shared_null = 0x85257e0},

	       m_frame_name_vector = {<std::_Vector_base<std::basic_string<char, std::char_traits<char>, std::allocator<char> >,std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > > >> = {
		 _M_impl = {<std::allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >> = {<__gnu_cxx::new_allocator<std::basic_string<char, std::char_traits<char>, std::allocator<char> > >> = {<No data fields>},
															 <No data fields>},
			    _M_start = 0x8679890,
			    _M_finish = 0x8681e58, _M_end_of_storage = 0x11}
	       },
				      <No data fields>},
	       xpos = 3.5776607979721602e-268, ypos = 3.614361207774048e-313,
	       xsize = 3.5777347387338128e-268, ysize = 3.614361207774048e-313,
	       xNativeSize = 3.5822451251946238e-268, yNativeSize = 3.614361207774048e-313,
	       locked = 16, filmstrip = 168, frameOffset = 141010960, blank_length = 141041240,
	       desktop = {static null = {static null = <same as static member of an already seen type>,
					 d = 0x85257e0, static shared_null = 0x85257e0}
			  , d = 0x11,
			  static shared_null = 0x85257e0}
},
  m_asset_name_string = {static npos = 4294967295,
			 _M_dataplus = {<std::allocator<char>> = {<__gnu_cxx::new_allocator<char>> = {<No data fields>},
								  <No data fields>},
					_M_p = 0x867afb0 "\b0h\b\b0h\b"}
  },
    layerAssetStatus = 176,
      keyAssetStatus = 175, mediaExchange = 0x8681e58, mediaExchangePresent = 17,
      m_inframe_valid = false, m_currentframe = 141012944, m_inframe = 1, m_outframe = 1,
      m_slip_frame_offset = 73, m_keycurrentframe = 1, m_keyinframe = 0, m_keyoutframe = 0,
      m_key_slip_frame_offset = 0, m_default_texture_id = 0, layernodes = 0x0,
      drawKeyframes = false, option1 = false, option2 = false, option3 = false, option4 = false,
      option5 = false, textureData = {objectImage = {data = 0x0}
				      , bpp = 0, wresolution = 0,
				      hresolution = 0, width = 0, height = 0, x = 0, y = 0, qimage = {data = 0x0}
				      ,
				      texwidth = 1.02294788e-43, texheight = 1.40129846e-45, texwidthratio = 0,
				      texheightratio = 0, texID = 0, m_tile_texture_id = {0, 0, 0, 0}

				      , m_tile_texture_width = {
					0, 0, 0, 0}
				      , m_tile_texture_height = {0, 0, 0, 0}
				      , m_tile_image_width = {0, 73, 1, 0}
				      ,
				      m_tile_image_height = {0, 0, 0, 0}
				      , mapwidth = 0, mapheight = 0, m_previous_width = 0,
				      m_previous_height = 0, m_previous_depth = 0}
      , textureKey = {objectImage = {data = 0x0}
		      ,
		      bpp = 0, wresolution = 0, hresolution = 0, width = 2.70620161e-40,
		      height = -6.96832285e-06, x = 1.02294788e-43, y = 1.40129846e-45, qimage = {data = 0x0}
		      ,
		      texwidth = 0, texheight = 0, texwidthratio = 0, texheightratio = 0, texID = 0,
		      m_tile_texture_id = {0, 0, 0, 0}, m_tile_texture_width = {0, 0, 0, 0}
		      ,
		      m_tile_texture_height = {0, 0, 73, 1}
		      , m_tile_image_width = {0, 0, 0, 0}
		      ,
		      m_tile_image_height = {0, 0, 0, 0}
		      , mapwidth = 0, mapheight = 0, m_previous_width = 0,
		      m_previous_height = 0, m_previous_depth = 0}
	,
	m_first_effect_layer_being_rendered = false, m_composite_qimage = {data = 0x0}
	  ,
	  m_composite_qimage_flipped = {data = 0x0}
	    , m_texture_composite = {objectImage = {
	      data = 0x49}
				     , bpp = 1, wresolution = 0, hresolution = 0, width = 0, height = 0, x = 0,
				     y = 0, qimage = {data = 0x0}
				     , texwidth = 0, texheight = 0, texwidthratio = 0,
				     texheightratio = 0, texID = 0, m_tile_texture_id = {0, 0, 0, 0}
				     , m_tile_texture_width = {
				       73, 1, 0, 0}
				     , m_tile_texture_height = {0, 0, 0, 0}
				     , m_tile_image_width = {0, 0, 0, 0}
				     ,
				     m_tile_image_height = {0, 0, 0, 0}
				     , mapwidth = 0, mapheight = 0, m_previous_width = 137,
				     m_previous_height = 3080238, m_previous_depth = 7340143}
	      , m_key_composite = {
		objectImage = {data = 0x6e0065}
		, bpp = 6881388, wresolution = 1.04693839e-38,
		hresolution = 1.04693825e-38, width = 9.27553225e-39, height = 4.31642947e-39,
		x = 1.01020517e-38, y = 1.06530787e-38, qimage = {data = 0x6c0061}
		,
		texwidth = 4.31641966e-39, texheight = 9.55105275e-39, texwidthratio = 1.04693825e-38,
		texheightratio = 4.31640985e-39, texID = 6357098, m_tile_texture_id = {7536744, 6357096,
										       6357099, 47}
		, m_tile_texture_width = {0, 0, 0, 0}
		, m_tile_texture_height = {0, 0, 0,
					   192713}
		, m_tile_image_width = {-1226190480, -1226190480, 0, 0}
		, m_tile_image_height = {
		  0, 25, 1, 141046304}
		, mapwidth = 0, mapheight = 5.46506401e-44, m_previous_width = 64,
		m_previous_height = 137, m_previous_depth = 3080238}
		,
		m_previous_rendered_layer_effect_category = 7340143, m_texture_composite_dirty = 101,
		  m_qimage_composite_dirty = false, m_select_colors_individually = 110,
		  m_first_pass = false, m_create_chroma_alpha_mask = 108, m_src_blend_factor = 7471202,
		  m_dst_blend_factor = 7471201, m_composite_type = 6619241, m_lighting_enabled = 115,
		  m_show_lights = false, m_use_open_gl_keyer = 47, m_use_fast_shader_keyer = false,
		  loadLight = 105, lightNumber = 7602291, light_diffuse = {9.91836951e-39, 4.31641966e-39,
									   9.55105275e-39, 1.04693825e-38}
		  , light_ambient = {4.31640985e-39, 8.90819166e-39,
				     1.05612278e-38, 8.90818886e-39}
		    , light_specular = {8.90819307e-39, 6.58610278e-44, 0,
					0}
		      , layerfog = false, MeshXCoord = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.50324616e-44,
							 1.40129846e-45, 6.98746168e-34, 0, 5.46506401e-44, 8.96831017e-44, 2.38220739e-44,
							 6.4601643e-34, 6.98902656e-34, 6.98840942e-34, 1.02294788e-43, 1.40129846e-45,
							 0 <repeats 14 times>, 2.38220739e-44, 6.98907799e-34, 1.69557114e-43, -6.96832285e-06,
							 -6.96832285e-06, 6.9875976e-34, 6.98757188e-34, 0, 4.20389539e-45, 1.4573504e-43,
							 3.36311631e-44, 6.98761597e-34, 9.27554206e-39, 1.59748025e-43}
							, {0, 0, 5.7453237e-44,
							   6.98900452e-34, -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44, 2.38220739e-44,
							   6.98907799e-34, 6.98765637e-34, 2.35418142e-43, 3.36311631e-44, 6.98764535e-34,
							   6.98761229e-34, 0, 5.60519386e-45, 1.68155816e-43, 5.60519386e-44, 5.57914965e-34,
							   6.15297964e-39, 0, 0, 0, -nan(0x7fffff), 0, 0, 0, 2.06411264e-42, -6.96860661e-06,
							   -6.96860661e-06, 6.98764535e-34, 6.98764535e-34, 2.24207754e-44, 2.38220739e-44,
							   6.98907799e-34, 6.98769678e-34, 5.60519386e-44, 3.36311631e-44, 6.98768576e-34,
							   6.98766005e-34, 0, 4.20389539e-45, 8.96831017e-44, 3.36311631e-44, 6.98770413e-34,
							   9.27554206e-39, 1.59748025e-43, 0, 0}
							, {5.7453237e-44, 6.98753882e-34,
							   -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98773719e-34, 1.79366203e-43, 3.36311631e-44, 6.98772617e-34, 6.98770045e-34, 0,
							   5.60519386e-45, 2.12997367e-43, 3.36311631e-44, 6.98774454e-34, 6.15309455e-39,
							   6.24485858e-39, 1.20511668e-43, 0, 5.7453237e-44, 6.98753882e-34, -6.96832285e-06, 0,
							   9.80908925e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34, 6.9877776e-34,
							   3.02680468e-43, 3.36311631e-44, 6.98776658e-34, 6.98774086e-34, 0, 4.20389539e-45,
							   3.36311631e-43, 3.36311631e-44, 6.98778494e-34, 9.27555187e-39, 1.59748025e-43, 0, 0,
							   5.7453237e-44, 6.98753882e-34, -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44}
							, {
							  2.38220739e-44, 6.98907799e-34, 6.987818e-34, 4.25994733e-43, 3.36311631e-44,
							  6.98780698e-34, 6.98778127e-34, 0, 5.60519386e-45, 4.59625896e-43, 3.36311631e-44,
							  6.98782535e-34, 6.15309455e-39, 6.24485858e-39, 6.86636248e-44, 0, 5.7453237e-44,
							  6.98753882e-34, -6.96832285e-06, 0, 9.80908925e-45, 1.12103877e-44, 2.38220739e-44,
							  6.98907799e-34, 6.98785841e-34, 5.49308998e-43, 3.36311631e-44, 6.98784739e-34,
							  6.98782168e-34, 0, 4.20389539e-45, 5.82940161e-43, 3.36311631e-44, 6.98786576e-34,
							  9.27554206e-39, 1.59748025e-43, 0, 0, 5.7453237e-44, 6.98753882e-34, -6.96832285e-06,
							  0, 7.00649232e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34, 6.98792086e-34,
							  6.72623263e-43, 3.36311631e-44, 6.98790984e-34}
							, {6.98786208e-34, 0, 5.60519386e-45,
							   7.06254426e-43, 1.00893489e-43, 6.98821841e-34, 6.15309455e-39, 6.24485858e-39,
							   2.93880454e-39, 1.01019957e-38, 1.01020405e-38, 1.01938928e-38, 1.02857353e-38,
							   9.64287283e-39, 1.38728548e-43, 0, 0, 0, 0, 0, 0, 0, 9.43073866e-43, 6.98753882e-34,
							   6.98825147e-34, 0, 0, 4.48415509e-44, 2.38220739e-44, 6.98907799e-34, 6.98796127e-34,
							   5.60519386e-44, 3.36311631e-44, 6.98795025e-34, 6.98792453e-34, 0, 5.60519386e-45,
							   8.96831017e-44, 3.36311631e-44, 6.98796861e-34, 9.27554206e-39, 1.59748025e-43, 0, 0,
							   5.7453237e-44, 6.98764535e-34, -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44}
							, {
							  2.38220739e-44, 6.98907799e-34, 6.98797963e-34, 1.79366203e-43, 3.36311631e-44,
							  6.98798698e-34, 6.98796494e-34, 0, 4.20389539e-45, 5.60519386e-45, 2.38220739e-44,
							  6.98907799e-34, 6.987998e-34, 2.35418142e-43, 3.36311631e-44, 6.98800535e-34,
							  6.98798331e-34, 0, 4.20389539e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34,
							  6.98803841e-34, 2.91470081e-43, 3.36311631e-44, 6.98802739e-34, 6.98800167e-34, 0,
							  4.20389539e-45, 3.25101244e-43, 3.36311631e-44, 6.98804576e-34, 9.27554206e-39,
							  1.59748025e-43, 0, 0, 5.7453237e-44, 6.98764535e-34, -6.96832285e-06, 0,
							  7.00649232e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34, 6.98807882e-34,
							  4.14784345e-43, 3.36311631e-44, 6.9880678e-34, 6.98804208e-34, 0}
							, {4.20389539e-45,
							   4.48415509e-43, 3.36311631e-44, 6.98808616e-34, 2.93884238e-39, 7.89794796e-39, 0, 0,
							   5.7453237e-44, 6.98764535e-34, -6.96832285e-06, 0, 8.40779079e-45, 1.12103877e-44,
							   2.38220739e-44, 6.98907799e-34, 6.98811922e-34, 5.3809861e-43, 3.36311631e-44,
							   6.9881082e-34, 6.98808249e-34, 0, 4.20389539e-45, 5.71729773e-43, 3.36311631e-44,
							   6.98812657e-34, 9.27555187e-39, 1.59748025e-43, 0, 0, 5.7453237e-44, 6.98764535e-34,
							   -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98815963e-34, 6.61412875e-43, 3.36311631e-44, 6.98814861e-34, 6.9881229e-34, 0,
							   4.20389539e-45, 6.95044038e-43, 3.36311631e-44, 6.98816698e-34, 2.93884238e-39,
							   4.5000346e-39}
							, {0, 0, 5.7453237e-44, 6.98764535e-34, -6.96832285e-06, 0,
							   8.40779079e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34, 6.98820004e-34,
							   7.8472714e-43, 3.36311631e-44, 6.98818902e-34, 6.98816331e-34, 0, 4.20389539e-45,
							   8.18358303e-43, 3.36311631e-44, 6.98820739e-34, 9.27554206e-39, 1.59748025e-43, 0, 0,
							   5.7453237e-44, 6.98764535e-34, -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44,
							   2.38220739e-44, 6.98907799e-34, 6.98826249e-34, 9.08041405e-43, 3.36311631e-44,
							   6.98825147e-34, 6.98820371e-34, 0, 4.20389539e-45, 1.74882048e-42, 1.00893489e-43,
							   6.98866656e-34, 2.93884238e-39, 4.5000346e-39, 5.96935209e-39, 8.90819727e-39,
							   1.01938928e-38, 1.02857353e-38, 9.64287283e-39, 1.38728548e-43}
							, {0, 0, 0, 0, 0, 0, 0,
							   0, 2.14398665e-43, 6.98753882e-34, 6.98833963e-34, 0, 2.38220739e-44, 4.48415509e-44,
							   2.38220739e-44, 6.98907799e-34, 6.9883029e-34, 5.60519386e-44, 3.36311631e-44,
							   6.98829188e-34, 6.98826616e-34, 0, 5.60519386e-45, 8.96831017e-44, 3.36311631e-44,
							   6.98831024e-34, 9.27554206e-39, 1.59748025e-43, 0, 0, 5.7453237e-44, 6.98790984e-34,
							   -6.96832285e-06, 0, 7.00649232e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98835065e-34, 1.79366203e-43, 3.36311631e-44, 6.98833963e-34, 6.98830657e-34, 0,
							   5.60519386e-45, 2.06271134e-42, 5.60519386e-44, 5.57919374e-34, 7.89785267e-39, 0}
							, {
							  0, 0, -nan(0x7fffff), 0, 0, 0, 5.7453237e-44, 5.58128024e-34, -6.96827237e-06,
							  6.98836167e-34, 6.98836167e-34, 6.98836167e-34, 0, 1.40129846e-45, 0, 5.60519386e-44,
							  2.38220739e-44, 6.98695108e-34, 0, 0, 5.7453237e-44, 9.6428392e-39, 1.00102372e-38,
							  1.02857115e-38, 4.31642947e-39, 8.90819166e-39, 9.91837932e-39, 1.1112231e-38,
							  1.04693881e-38, 0, 3.50324616e-44, 1.40129846e-45, 6.98836902e-34, 0, 2.24207754e-44,
							  2.24207754e-44, 3.50324616e-44, 8.99994308e-39, 1.02857283e-38, 0, 5.60519386e-45,
							  3.36311631e-44, 5.7453237e-44, 5.68728044e-34, 7.89785407e-39, 0, 0, 0,
							  -nan(0x7fffff), 0}
							, {0, 0, 1.8286945e-42, -6.9685957e-06, -6.9685957e-06,
							   6.98842412e-34, 6.98842412e-34, 2.24207754e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98845351e-34, 5.60519386e-44, 3.36311631e-44, 6.98846085e-34, 6.98843881e-34, 0,
							   5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98847187e-34,
							   1.12103877e-43, 3.36311631e-44, 6.98847922e-34, 6.98845718e-34, 0, 5.60519386e-45,
							   5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98854167e-34, 1.68155816e-43,
							   3.36311631e-44, 6.98850861e-34, 6.98847555e-34, 0, 5.60519386e-45, 2.01786979e-43,
							   5.60519386e-44, 6.9885784e-34, 1.00102204e-38, 6.42852957e-39, 9.91839754e-39,
							   2.93888722e-39, 6.88773228e-39, 0, 0, 0, 2.14398665e-43, 6.98833963e-34}
							, {
							  6.98859677e-34, 0, 1.68155816e-44, 3.36311631e-44, 3.36311631e-44, 6.98854901e-34,
							  5.32652884e-39, 5.14285925e-39, 7.98740125e-44, 0, 5.7453237e-44, 6.98842412e-34,
							  -6.96832285e-06, 0, 9.80908925e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34,
							  6.98856003e-34, 1.23314265e-43, 3.36311631e-44, 6.98856738e-34, 6.98854534e-34, 0,
							  5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98862983e-34,
							  1.79366203e-43, 3.36311631e-44, 6.98859677e-34, 6.98856371e-34, 0, 5.60519386e-45,
							  4.70836284e-43, 5.60519386e-44, 6.98887228e-34, 9.27553925e-39, 8.90819587e-39,
							  9.09187527e-39, 1.02857311e-38, 1.41531145e-43, 0, 0, 0, 2.14398665e-43,
							  6.98833963e-34, 6.98869962e-34, 0}
							, {1.54142831e-44, 3.36311631e-44, 3.36311631e-44,
							   6.98863718e-34, 5.32653585e-39, 5.14285925e-39, 7.98740125e-44, 0, 5.7453237e-44,
							   6.98850861e-34, -6.96832285e-06, 0, 9.80908925e-45, 1.12103877e-44, 2.38220739e-44,
							   6.98907799e-34, 6.9886482e-34, 1.23314265e-43, 3.36311631e-44, 6.98865554e-34,
							   6.9886335e-34, 0, 5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34,
							   6.98873269e-34, 1.79366203e-43, 3.36311631e-44, 6.98869962e-34, 6.98865187e-34, 0,
							   5.60519386e-45, 7.39885589e-43, 1.00893489e-43, 6.98876942e-34, 9.18368554e-39,
							   1.00102106e-38, 2.93890543e-39, 9.64282098e-39, 9.27553925e-39, 1.01020601e-38,
							   6.42852957e-39, 9.91839754e-39, 1.51340234e-43, 0, 0, 0, 0, 0}
							, {0, 0, 2.14398665e-43,
							   6.98833963e-34, 6.98880248e-34, 0, 2.66246708e-44, 3.36311631e-44, 3.36311631e-44,
							   6.98874003e-34, 5.32653585e-39, 4.68368177e-39, 7.14662217e-44, 0, 5.7453237e-44,
							   6.98859677e-34, -6.96832285e-06, 0, 9.80908925e-45, 1.12103877e-44, 2.38220739e-44,
							   6.98907799e-34, 6.98875105e-34, 1.23314265e-43, 3.36311631e-44, 6.9887584e-34,
							   6.98873636e-34, 0, 5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34,
							   6.98883554e-34, 1.79366203e-43, 3.36311631e-44, 6.98880248e-34, 6.98875473e-34, 0,
							   5.60519386e-45, 1.05377645e-42, 1.00893489e-43, 0, 9.18368554e-39, 1.00102106e-38,
							   2.93890543e-39, 9.64282098e-39, 9.27553925e-39, 1.01020601e-38, 6.61220056e-39,
							   9.91836951e-39}
							, {1.42932443e-43, 0, 0, 0, 0, 0, 0, 0, 6.74024561e-43, 6.98833963e-34,
							   6.98764535e-34, 0, 2.66246708e-44, 3.36311631e-44, 3.36311631e-44, 6.98884289e-34,
							   5.32653585e-39, 4.68368177e-39, 7.14662217e-44, 0, 5.7453237e-44, 6.98869962e-34,
							   -6.96832285e-06, 0, 9.80908925e-45, 1.12103877e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98885391e-34, 1.23314265e-43, 3.36311631e-44, 6.98886126e-34, 6.98883921e-34, 0,
							   5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98890166e-34,
							   1.79366203e-43, 3.36311631e-44, 6.98889064e-34, 6.98885758e-34, 0, 4.20389539e-45,
							   2.12997367e-43, 5.60519386e-44, 6.98895676e-34, 2.9388732e-39, 1.01938326e-38}
							, {
							  9.18368554e-39, 8.90818186e-39, 1.06530787e-38, 0, 0, 0, 2.03188277e-43,
							  6.98869962e-34, 6.98897513e-34, 0, 1.68155816e-44, 2.24207754e-44, 2.38220739e-44,
							  6.98907799e-34, 6.98892003e-34, 5.60519386e-44, 3.36311631e-44, 6.98892738e-34,
							  6.98890534e-34, 0, 4.20389539e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34,
							  6.9889384e-34, 1.12103877e-43, 3.36311631e-44, 6.98894574e-34, 6.9889237e-34, 0,
							  5.60519386e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98898615e-34,
							  1.68155816e-43, 3.36311631e-44, 6.98897513e-34, 6.98894207e-34, 0, 5.60519386e-45,
							  4.70836284e-43, 5.60519386e-44, 0, 9.91838493e-39, 6.06118759e-39, 8.90819867e-39,
							  9.09185425e-39, 1.0561218e-38, 1.62550622e-43}
							, {0, 0, 1.47136339e-43, 6.98869962e-34,
							   6.98842412e-34, 0, 1.821688e-44, 2.24207754e-44, 2.38220739e-44, 6.98907799e-34,
							   6.98900452e-34, 5.60519386e-44, 3.36311631e-44, 6.98901187e-34, 6.98898982e-34, 0,
							   4.20389539e-45, 5.60519386e-45, 2.38220739e-44, 6.98907799e-34, 6.98753882e-34,
							   1.12103877e-43, 3.36311631e-44, 0, 6.98900819e-34, 0, 5.60519386e-45, 1.8272932e-42,
							   1.68155816e-43, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 20, 100, 0, 0, 1, 1, 1, 1, 1, 1}
							, {1,
							   1, 1, 1, 1, 1, 1.40129846e-45, 0, 3.51025265e-42, 6.9891037e-34, 6.98919737e-34,
							   6.98929105e-34, 6.98938472e-34, 6.98947839e-34, 6.98957206e-34, 6.98966573e-34,
							   6.98975941e-34, 6.98985308e-34, 6.98994675e-34, 6.99004042e-34, 6.9901341e-34,
							   8.52376668e-34, 0, 0, 2.80259693e-45, 2.80259693e-45, 1.40129846e-45, 5.60519386e-45,
							   2.80259693e-45, 2.80259693e-45, 1.40129846e-45, 2, 2, 1, 1.40129846e-45,
							   1.40129846e-45, 0, 0, 5.60519386e-45, 5.60519386e-45, 8.97952056e-42, -5.87426257e-07,
							   0, 1.12103877e-44, 1.12103877e-44, 1.12103877e-44, 1.12103877e-44, 0, 0, 0}
							, {
							  2.38220739e-44, 0, 0, 0, -4.36771643e-07, 0, 0, 6.98625313e-34, 5.60519386e-45,
							  2.80259693e-45, 4.48415509e-44, 4.48415509e-44, 5.60519386e-45, 2.80259693e-45,
							  -4.10740995e-07, 0, 0, 0, 2.80259693e-45, 2.80259693e-45, 1.40129846e-45,
							  0 <repeats 18 times>, 1.40129846e-45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
							, {
							  0 <repeats 40 times>, 1.40129846e-45, 0, 0, 0, 0, 0, 0, 0, 0, 0}
							, {
							  0 <repeats 41 times>, 1.40129846e-45, 0, 0, 0, 0, 0, 0, 0, 0}
							, {0 <repeats 42 times>,
							   1.40129846e-45, 0, 0, 0, 0, 0, 0, 0}
							, {0 <repeats 43 times>, 1.40129846e-45, 0, 0, 0,
							   0, 0, 0}
							, {0 <repeats 44 times>, 1.40129846e-45, 0, 0, 0, 0, 0}
							, {
							  0 <repeats 45 times>, 1.40129846e-45, 0, 0, 0, 0}
							, {0 <repeats 46 times>,
							   1.40129846e-45, 0, 0, 0}
							, {0 <repeats 47 times>, 1.40129846e-45, 0, 0}
							, {
							  0 <repeats 48 times>, 1.40129846e-45, 0}
							, {0 <repeats 49 times>, 1.40129846e-45}
							, {
							  0 <repeats 34 times>, 2.61528536e-40, -6.96877032e-06, -6.96877032e-06,
							  6.99022777e-34, 6.99022777e-34, 0 <repeats 11 times>}
							, {
							  0 <repeats 50 times>}
							<repeats 44 times>}
			, MeshYCoord = {{
			  0 <repeats 50 times>}
					<repeats 74 times>}
			  , MeshZCoord = {{
			    0 <repeats 50 times>}
					  <repeats 74 times>}
			    , m_mesh_save = {{{x = 0, y = 0, z = 0,
					       w = 0}
					      <repeats 50 times>}
					     <repeats 21 times>, {{x = 0, y = 0, z = 0,
								   w = 0}
								  <repeats 17 times>, {x = 0, y = 7.66902624e-40, z = 9.18467065e-41, w = 0}
								  , {
								    x = 0, y = 0, z = 0, w = 0}
								  <repeats 32 times>}
					     , {{x = 0, y = 0, z = 0,
						 w = 0}
						<repeats 50 times>}
					     <repeats 52 times>}
			      , MeshXNormal = {{
				0 <repeats 50 times>}
					       <repeats 74 times>}
				, MeshYNormal = {{
				  0 <repeats 50 times>}
						 <repeats 43 times>, {0, 0, 0, 0, 0, 8.5874933e-40,
								      9.18467065e-41, 0 <repeats 43 times>}
						 , {0 <repeats 50 times>}
						 <repeats 30 times>}
				  ,
				  MeshZNormal = {{0 <repeats 50 times>}
						 <repeats 74 times>}

				    , m_mesh_normal_save = {{{x = 0,
							      y = 0, z = 0, w = 0}
							     <repeats 50 times>}

				    <repeats 55 times>, 
{

{
  x = 0, 
    y = 0,
    z = 0,
    w = 0}
 <repeats 35 times>, {x = 0, y = 2.61527135e-40, z = 1.43941378e-41, w = 0}
 , {
   x = 2.1019477e-44, y = 1.79366203e-43, z = 1.16207241e-33, w = 0}
 , {x = 0, y = 0,
      z = 0, w = 0}
 <repeats 13 times>}
				      , {{x = 0, y = 0, z = 0, w = 0}
				      , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 1.12103877e-44}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 5.87747175e-39, y = 0, z = 8.52483197e-34, w = 8.52478789e-34}
							       , {
								 x = 8.52514788e-34, y = 8.52500829e-34, z = 6.47057845e-34, w = 8.52605889e-34}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       <repeats 42 times>}
							    , {{x = 0, y = 0, z = 0,
								w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
							    , {{
							      x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0,
								w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
							    , {{
							      x = 0, y = 0, z = 0, w = 0}
							       <repeats 44 times>, {x = 0, y = 2.75506488e-40, z = 0,
										    w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 1.40129846e-45, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 1.12103877e-44, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							    }
							    , {{x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 1.91936187e-38, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 0, z = 1.11359507e-39, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 1.40129846e-45}
							       , {x = 2.80259693e-45, y = 4.20389539e-45, z = 0,
								  w = 1.40129846e-45}
							       <repeats 15 times>, {x = 2.80259693e-45, y = 4.20389539e-45,
										    z = 1.79366203e-43, w = 1.40129846e-45}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       <repeats 19 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 35 times>, {
								 x = 0, y = 0, z = 7.07606457e-34, w = 7.07859005e-34}
							       , {x = 1.19123275e-33,
								  y = 1.1624302e-33, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 1.40129846e-45,
								  y = 4.59177481e-41, z = 0, w = 4.59177481e-41}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 1.40129846e-45, w = 0}
							       , {
								 x = 4.59177481e-41, y = 4.59177481e-41, z = 4.59177481e-41, w = 4.59177481e-41}
							       , {
								 x = 4.59177481e-41, y = 4.59177481e-41, z = 4.59177481e-41, w = 4.59177481e-41}
							       , {
								 x = 4.59177481e-41, y = 4.59177481e-41, z = 0, w = 0}
							    }
							    , {{x = 0, y = 2.24207754e-44,
								z = 2.24207754e-44, w = 2.24207754e-44}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 1.16671929e-33}
							       , {x = 0,
								  y = 1.16207792e-33, z = 9.21956299e-41, w = 5.60519386e-44}
							       , {x = 5.60519386e-44,
								  y = 4.62428493e-44, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 4.62428493e-44, w = 4.62428493e-44}
							       , {x = 4.62428493e-44, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 1.40129846e-45, z = 2.80259693e-45, w = 4.20389539e-45}
							       , {x = 0,
								  y = 1.40129846e-45, z = 2.80259693e-45, w = 4.20389539e-45}
							       , {x = 0,
								  y = 1.40129846e-45, z = 2.80259693e-45, w = 4.20389539e-45}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       <repeats 38 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 49 times>, {
								 x = 0, y = 1.40129846e-45, z = 0, w = 0}
							    }
							    , {{x = 0, y = 2.80259693e-45, z = 0,
								w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 9.21942286e-41, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 2.36942783e-38, z = 2.36942783e-38, w = 2.36942783e-38}
							       , {
								 x = 2.36942783e-38, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 1.40129846e-45, z = 2.52233724e-44, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       <repeats 17 times>, {x = 4.59401689e-41,
										    y = 4.59415702e-41, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							    }
							    , {{x = 0, y = 4.48415509e-44, z = 0, w = 3.58732407e-43}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 1.40129846e-45, y = 0, z = 2.80259693e-45, w = 0}
							       , {x = 0,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 1.40129846e-45,
								  w = 0}
							       , {x = 1.83670992e-40, y = 1.40129846e-45, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 4.59177481e-41, y = 1.40129846e-45, z = 1.40129846e-45,
								  w = 1.40129846e-45}
							       , {x = 1.40129846e-45, y = 0, z = 0, w = 0}
							       , {x = 7.07592865e-34,
								  y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {
								 x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0, z = 0, w = 0}
							       , {x = 0, y = 0,
								  z = 0, w = 0}
							       , {x = 0, y = 0, z = 1.43955391e-41, w = 0}
							       , {x = 5.60519386e-44,
								  y = 1.79366203e-43, z = 1.16212016e-33, w = 0}
							       , {x = 0, y = 0, z = 0,
								  w = 0}
							       <repeats 17 times>, {x = 0, y = 0, z = 0, w = 1.12103877e-44}
							       , {
								 x = 1.40129846e-45, y = 0, z = 0, w = 0}
							       , {x = 5.87792017e-39, y = 0,
								  z = 8.52179772e-34, w = 6.47487635e-34}
							       , {x = 6.47489104e-34, y = 9.60183459e-34,
								  z = 8.51478884e-34, w = 8.51481823e-34}
							    }
							    , {{x = 0, y = 0, z = 0,
								w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
							    , {{
							      x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0,
								w = 0}
							       <repeats 50 times>}
							    , {{x = 0, y = 0, z = 0, w = 0}
							       <repeats 50 times>}
				    }
				      ,
				      MeshFirsttime = false, JAHIMAGE_SCALE = 0, JAHMESH_X_DIMENSION = 0,
					JAHMESH_Y_DIMENSION = 0, JAHMESH_X_DIMENSION_DIV_2 = 0, JAHMESH_Y_DIMENSION_DIV_2 = 0,
					text = 0x0, updateFont = false, objfilename = {static null = {
					  static null = <same as static member of an already seen type>, d = 0x85257e0,
					  static shared_null = 0x85257e0}
										       , d = 0x0, static shared_null = 0x85257e0}
					,
					objextension = {static null = {
					  static null = <same as static member of an already seen type>, d = 0x85257e0,
					  static shared_null = 0x85257e0}
							, d = 0x0, static shared_null = 0x85257e0}
					  ,
					  jahobject = 0x0, jahobjectPresent = false, loadObject = false, loadParticles = false,
					    ParticleStyle = ParticleCategory::ATOMIC, ParticleDraw = ParticleDrawstyle::PARLINE,
					    ParticleFirsttime = false, ParticleColors = false, particle_handle = 0, action_handle = 0,
					    maxParticles = 0, demoNum = 0, numSteps = 0, prim = 0, listID = 0, SpotTexID = 0,
					    BlurRate = 0, SteerShape = 0, widthOffset = 0, heightOffset = 0, layerEffect = false,
					    negative = false, swaprgb = false, mirror = false, blur = false, hblur = 0, vblur = 0,
					    blendmode1 = 0, blendmode2 = 0, Red = 0, Green = 0, Blue = 0, blend = false,
					    depth = false, cull = false, lighting = false, reflect = false, smooth = false,
					    foreground = false, m_is_slected_visible = false, ObjectDisplayMode = false,
					    showEdges = false, loop = false, ping = false, jtrace = 0x0, JahBasePath = {static null = {
					      static null = <same as static member of an already seen type>, d = 0x85257e0,
					      static shared_null = 0x85257e0}
															, d = 0x0, static shared_null = 0x85257e0}
					    ,

					    JahMediaPath = {static null = {
					      static null = <same as static member of an already seen type>, d = 0x85257e0,
					      static shared_null = 0x85257e0}
							    , d = 0x0, static shared_null = 0x85257e0}
					      ,
					      X_RESOLUTION = 0, 
					      Y_RESOLUTION = 0, 
					      m_texture_format = 0}
*/
