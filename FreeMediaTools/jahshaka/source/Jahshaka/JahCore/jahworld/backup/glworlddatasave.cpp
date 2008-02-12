/*******************************************************************************
**
** The source file for the Jahshaka glworld dataio code
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include "jahdataio.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <jahpreferences.h>
#include <openmedialib.h>

////////////////////////////////////
//needed for new xml stuff
#include "jahloadxml.h"

/////////////////////////////////////////////////////////////////////////////
// xml save stuff here

void GLWorld::saveSceneFile(std::string const& a_save_filename)
{
    std::string text;
    std::string JahVersion;

    setSceneSaveIndentationDepth(0);

    // Needed to initialize static members in constructor;
    SceneFileXml scene_file_xml( text = "" );

    //initalize the global vars from the prefs
    JahPrefs & jprefs = JahPrefs::getInstance( );
    JahVersion  = jprefs.getJahVersion( ).data( );


    std::fstream l_file_stream;
    l_file_stream.open( a_save_filename.data( ) , std::ios::out );
    if ( l_file_stream.is_open( ) == false && !getAutoSaveInProgress() ) 
    {
        QMessageBox::information(0, "Could not write to file  ", a_save_filename.data( ) );
        return;
    }

	saveHeaders( l_file_stream, a_save_filename, m_qsModuleName.data() );

    if (m_qsModuleName == "Tracker")
    {
        SaveTrackerData(l_file_stream);
    }
    else
    {
        saveAllLayers(l_file_stream);
    }

    sceneDecreaseIndent();
    l_file_stream << "</JAHSHAKA>" << "\n";  


    ///////////////////////////////////////////////////////////////////////////
    //now close the file and let the user know
    l_file_stream.close();

    if ( !getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "File saved successfully ", a_save_filename.data() );
    }	
}

void GLWorld::saveAllLayers( std::fstream& ai_fstream ) 
{
    for ( LayerListEntry* layer = getLayerList()->first(); layer; layer = getLayerList()->next() )
    {
        JahLayer* jah_layer = layer->thelayer;
        int layer_number = getLayerList()->at();
        bool module_uses_world_layer =
                   m_qsModuleName == "Animation" 
                || m_qsModuleName == "Paint" 
                || m_qsModuleName == "Text";
                ;

        bool dont_save_layer =
                   jah_layer->getCategory() == ObjectCategory::EFFECT
                || jah_layer->getCategory() == ObjectCategory::JAHLIGHT
                || (!module_uses_world_layer && jah_layer->objtype == ObjectCategory::BACKDROP)
                ;
                


        if (dont_save_layer)
        {
        }
        else 
        {
            ai_fstream << sceneGetIndent() << "<LAYER" ;
            ai_fstream << " name = '" << jah_layer->layername.data() << "'" ;
            ai_fstream << " activation = '" << jah_layer->layerStatus << "'" ;
            ai_fstream << ">" << "\n" ; 


            sceneIncreaseIndent();
            ai_fstream << sceneGetIndent() << "<KEY" ;
            ai_fstream << " invert = '" << jah_layer->invertKey << "'" ;
            ai_fstream << " draw = '" << jah_layer->drawtheKey << "'" ;
            ai_fstream << " status = '" << jah_layer->keyAssetStatus << "'" ;
            ai_fstream << " keyinframe = '" << jah_layer->m_keyinframe << "'" ;
            ai_fstream << " keyoutframe = '" << jah_layer->m_keyoutframe << "'" ;
            ai_fstream << " use_open_gl_keyer = '" << jah_layer->getUseOpenGlKeyer() << "'" ;
            ai_fstream << " />" << "\n" ; 

            ai_fstream << sceneGetIndent() << "<TYPE" ;
            ai_fstream << JahDataIO::JahParse( " layertype = '", jah_layer->layertype ).data() << "'" ;
            ai_fstream << JahDataIO::JahParse( " objtype = '", jah_layer->objtype ).data() << "'" ;
            ai_fstream << " composite_type = '" << CompositeType::getName( jah_layer->getCompositeType() ) << "'" ;
            ai_fstream << " />" << "\n" ; 

            saveLightLayers(ai_fstream, jah_layer);
			saveEffectLayers(ai_fstream, jah_layer);
			SaveProperties(ai_fstream, jah_layer);
        }



        //now write the objects information out
        switch ( jah_layer->layertype ) 
        {
            case LayerCategory::LAYER :     
            {
                //break this out into its own subroutine like the others
                if ( jah_layer->objtype == ObjectCategory::BACKDROP ) 
                {
                    if (!module_uses_world_layer)
                    {
                        break;
                    }

                    //need to save special options for backdrop such as gridstatus, selection status
                    ai_fstream << sceneGetIndent() << "<BACKDROP" ;
                    ai_fstream << " grid_setting = '" << m_bGridVal << "'" ;
                    ai_fstream << " aliasing = '" << m_bPolySmooth << "'" ;
                    ai_fstream << " text_safe = '" << m_pTextSafe->layerStatus << "'" ;
                    ai_fstream << ">" << "\n" ; 

                    if ( jah_layer->layerAssetStatus )
                    {  
                        SaveAsset( ai_fstream , jah_layer );  
                    }

                    SaveNodes( ai_fstream, jah_layer );
                    ai_fstream << sceneGetIndent() << "</BACKDROP>" << "\n";  
                } 
                else if (jah_layer->getCategory() != ObjectCategory::EFFECT)
                {
                    if ( jah_layer->layerAssetStatus )
                    {  
                        SaveAsset( ai_fstream , jah_layer ); 
                    }

                    //need to use a better flag here 
                    //ie is there a key?
                    if ( jah_layer->keyAssetStatus )
                    { 
                        SaveKeyAsset( ai_fstream , layer_number );
                    }

                    SaveNodes( ai_fstream, jah_layer );
                }

                break; 
            }

            case LayerCategory::PARTICLE :     
            {
                if (jah_layer->layerAssetStatus)
                { 
                    SaveAsset( ai_fstream , jah_layer );   
                }

                ai_fstream << sceneGetIndent() << "<PARTICLE ";
                ai_fstream << " type = '" << ParticleCategory::getName(jah_layer->ParticleStyle) << "'" ;
                ai_fstream << " draw = '" << ParticleDrawstyle::getName(jah_layer->ParticleDraw) << "'" ;
                ai_fstream << " color = '" << jah_layer->ParticleColors << "'" ;
                ai_fstream << " />" << "\n";

                SaveNodes( ai_fstream , jah_layer );
                break; 
            }	

            case LayerCategory::OBJECT :     
            {
                if (jah_layer->layerAssetStatus)
                { 
                    SaveAsset( ai_fstream , jah_layer ); 
                }

                SaveObjectData( ai_fstream , layer_number );
                SaveNodes( ai_fstream , jah_layer );
                break; 
            }

            case LayerCategory::FRAME :
            {   
                break;  
            }

            case LayerCategory::TEXT :     
            {
                if (jah_layer->layerAssetStatus)
                {  
                    SaveAsset( ai_fstream , jah_layer ); 
                }

                SaveColor( ai_fstream , layer_number );
                SaveText( ai_fstream , layer_number );
                SaveNodes( ai_fstream , jah_layer );
                break;
            }

            case LayerCategory::GRID : 
            { 
                break; 
            }

            default : 
            {
                break;
            }
        }

        if (dont_save_layer)
        {
        }
		else
        {
            sceneDecreaseIndent();
            ai_fstream << sceneGetIndent() << "</LAYER>" << "\n";
        }
    }
}  // save scene files to disk

const char* 
GLWorld::sceneGetIndent()
{
    static std::string indentation;
    indentation = "";

    for (int i = 0; i < getSceneSaveIndentationDepth(); i++)
    {
        for (size_t j = 0; j < JAH_XML_TAB_DEPTH; j++)
        {
            indentation += " ";
        }
    }

    return (const char*)indentation.c_str();
}


void GLWorld::SaveObjectData( std::fstream & ai_fstream , unsigned int const & a_i ) 
{
    unsigned int const & i = a_i;
    JahLayer* jah_layer = getLayerList()->at(i)->thelayer;

    ai_fstream << sceneGetIndent() << "<OBJECT" ;  
    ai_fstream << " filename = '" << jah_layer->objfilename.data() << "'" ; 
    char l_extension[ JahDataIO::c_max_path ]; 
    ( void ) JahDataIO::getExtension( l_extension , jah_layer->objextension.data( ) );
    ai_fstream << " extension = '." << l_extension << "'" ; 
    ai_fstream << " />"<< "\n" ;
} 






void GLWorld::SaveParticleData( std::fstream & ai_fstream , unsigned int const & a_i )
{
    unsigned int const & i = a_i;
    JahLayer* jah_layer = getLayerList()->at(i)->thelayer;

    ai_fstream << sceneGetIndent() << "<STYLE = '" ;  

    ai_fstream << ParticleCategory::getName(jah_layer->ParticleStyle) << "'" ;
    ai_fstream << " draw = '" << jah_layer->ParticleDraw << "'" ;
    ai_fstream << " color = '" << jah_layer->ParticleColors << "'" ;

    ai_fstream << " />" << "\n" ;  

}  // save scene files to disk

void GLWorld::SaveCameraNodes( std::fstream & ai_fstream ) 
{
    int j = 0 ;
    int count = (int)m_pCamera->layernodes->m_nodelist->count();
     
    ai_fstream << sceneGetIndent() << "<KEYFRAMES count = '" << count << "'>" << "\n" ;
    sceneIncreaseIndent();

    do  
    {
        motionNode keyframe_copy;

        if (count < 1)
        {
            keyframe_copy.copy( m_pCamera->layernodes->m_node );
        }
        else
        {
            keyframe_copy.copy( m_pCamera->layernodes->m_nodelist->at( j ) );
        }

        ai_fstream << sceneGetIndent() << "<KEYFRAME" ;

        if ( count > 0 )
        {
            ai_fstream << " TC = '" <<
	      m_pCamera->layernodes->m_nodelist->at(
						    j
						    )->m_frame_number << "'" ;
        }
        else
        {
            ai_fstream << " TC = '0'" ;
        }


        ai_fstream << " SX = '" << keyframe_copy.sx << "'" ;  
        ai_fstream << " SY = '" << keyframe_copy.sy << "'" ;  
        ai_fstream << " SZ = '" << keyframe_copy.sz << "'" ;  

        ai_fstream << " TX = '" << keyframe_copy.tx << "'" ;  
        ai_fstream << " TY = '" << keyframe_copy.ty << "'" ;  
        ai_fstream << " TZ = '" << keyframe_copy.tz << "'" ;  

        ai_fstream << " RX = '" << keyframe_copy.rx << "'" ;  
        ai_fstream << " RY = '" << keyframe_copy.ry << "'" ;  
        ai_fstream << " RZ = '" << keyframe_copy.rz << "'" ;  

        ai_fstream << " AL = '" << keyframe_copy.Alpha << "'" ;  

        ai_fstream << " extrude = '" << keyframe_copy.extrude << "'\n" ;  

        sceneIncreaseIndent();
        ai_fstream << sceneGetIndent() << "layer_enabled = '" << keyframe_copy.layer_enabled << "'\n" ;  

        sceneIncreaseIndent();
        ai_fstream << sceneGetIndent() << "' ambient_red = '" << int(keyframe_copy.ambient_light_color.x * 255.0f) << "'";  
        ai_fstream << "' ambient_green = '" << int(keyframe_copy.ambient_light_color.y * 255.0f) << "'";  
        ai_fstream << "' ambient_blue = '" << int(keyframe_copy.ambient_light_color.z * 255.0f) << "'\n";  

        ai_fstream << sceneGetIndent() << "' diffuse_red = '" << int(keyframe_copy.diffuse_light_color.x * 255.0f) << "'";  
        ai_fstream << "' diffuse_green = '" << int(keyframe_copy.diffuse_light_color.y * 255.0f) << "'";  
        ai_fstream << "' diffuse_blue = '" << int(keyframe_copy.diffuse_light_color.z * 255.0f) << "'\n";  

        ai_fstream << sceneGetIndent() << "' specular_red = '" << int(keyframe_copy.specular_light_color.x * 255.0f) << "'";  
        ai_fstream << "' specular_green = '" << int(keyframe_copy.specular_light_color.y * 255.0f) << "'";  
        ai_fstream << "' specular_blue = '" << int(keyframe_copy.specular_light_color.z * 255.0f) << "'";  

        ai_fstream << " />" << "\n"; 
        sceneDecreaseIndent();

        j ++ ;
    }
    while ( j < count ) ;

    sceneDecreaseIndent();

    ai_fstream << sceneGetIndent() << "</KEYFRAMES>" << "\n";  
}  // save scene files to disk


void GLWorld::SaveNodes( std::fstream & ai_fstream , JahLayer* jah_layer ) 
{
    int j = 0 ;
    int count = (int)jah_layer->layernodes->m_nodelist->count();
     
    ai_fstream << sceneGetIndent() << "<KEYFRAMES count = '" << count << "'>" << "\n" ;
    sceneIncreaseIndent();

    do  
    {
        motionNode keyframe_copy;

        if (count < 1)
        {
            keyframe_copy.copy( jah_layer->layernodes->m_node );
        }
        else
        {
            keyframe_copy.copy( jah_layer->layernodes->m_nodelist->at( j ) );
        }

        ai_fstream << sceneGetIndent() << "<KEYFRAME" ;

        if ( count > 0 )
        {
            ai_fstream << " TC = '" << jah_layer->layernodes->m_nodelist->at( j )->m_frame_number << "'" ;  
        }
        else
        {
            ai_fstream << " TC = '0'" ;
        }


        ai_fstream << " SX = '" << keyframe_copy.sx << "'" ;  
        ai_fstream << " SY = '" << keyframe_copy.sy << "'" ;  
        ai_fstream << " SZ = '" << keyframe_copy.sz << "'" ;  

        ai_fstream << " TX = '" << keyframe_copy.tx << "'" ;  
        ai_fstream << " TY = '" << keyframe_copy.ty << "'" ;  
        ai_fstream << " TZ = '" << keyframe_copy.tz << "'" ;  

        ai_fstream << " RX = '" << keyframe_copy.rx << "'" ;  
        ai_fstream << " RY = '" << keyframe_copy.ry << "'" ;  
        ai_fstream << " RZ = '" << keyframe_copy.rz << "'" ;  

        ai_fstream << " AL = '" << keyframe_copy.Alpha << "'" ;  

        ai_fstream << " extrude = '" << keyframe_copy.extrude << "'\n" ;  

        sceneIncreaseIndent();
        ai_fstream << sceneGetIndent() << "layer_enabled = '" << keyframe_copy.layer_enabled << "'\n" ;  

        ai_fstream << sceneGetIndent() << "ambient_red = '" << int(keyframe_copy.ambient_light_color.x * 255.0f) << "'";  
        ai_fstream << " ambient_green = '" << int(keyframe_copy.ambient_light_color.y * 255.0f) << "'";  
        ai_fstream << " ambient_blue = '" << int(keyframe_copy.ambient_light_color.z * 255.0f) << "'\n";  

        ai_fstream << sceneGetIndent() << "diffuse_red = '" << int(keyframe_copy.diffuse_light_color.x * 255.0f) << "'";  
        ai_fstream << " diffuse_green = '" << int(keyframe_copy.diffuse_light_color.y * 255.0f) << "'";  
        ai_fstream << " diffuse_blue = '" << int(keyframe_copy.diffuse_light_color.z * 255.0f) << "'\n";  

        ai_fstream << sceneGetIndent() << "specular_red = '" << int(keyframe_copy.specular_light_color.x * 255.0f) << "'";  
        ai_fstream << " specular_green = '" << int(keyframe_copy.specular_light_color.y * 255.0f) << "'";  
        ai_fstream << " specular_blue = '" << int(keyframe_copy.specular_light_color.z * 255.0f) << "'";  

        ai_fstream << " />" << "\n"; 
        sceneDecreaseIndent();

        j ++ ;
    }
    while ( j < count ) ;

    sceneDecreaseIndent();

    ai_fstream << sceneGetIndent() << "</KEYFRAMES>" << "\n";  
}  // save scene files to disk


void GLWorld::SaveColor( std::fstream & ai_fstream , unsigned int const & a_i ) 
{
    unsigned int const & i = a_i;
    JahLayer* jah_layer = getLayerList()->at(i)->thelayer;

    ai_fstream << sceneGetIndent() << "<COLOR" ;  

    ai_fstream << " red = '" << jah_layer->Red << "'" ;  
    ai_fstream << " green = '" << jah_layer->Green << "'" ;  
    ai_fstream << " blue = '" << jah_layer->Blue << "'" ;  

    ai_fstream << " />" << "\n";  

}  // save scene files to disk

void GLWorld::SaveText( std::fstream & ai_fstream , unsigned int const & a_i ) 
{
    unsigned int const & i = a_i;
    JahLayer* jah_layer = getLayerList()->at(i)->thelayer;

    ai_fstream << sceneGetIndent() << "<TEXT" ;  

    std::string text_string = jah_layer->text->text.data();

    // Replace carriage return with &#10
    size_t carriage_return_position = text_string.find("\n", 0);
    std::string carriage_return = "&#10";

    while(carriage_return_position != std::string::npos)
    {
        text_string.replace(carriage_return_position, 1, carriage_return);
        carriage_return_position = text_string.find("\n", carriage_return_position + 1);
    }

    ai_fstream << " text = '" << text_string << "'" ;  
    ai_fstream << " font = '" << jah_layer->text->font.data() << "'" ;  
    ai_fstream << " style = '" << FontDrawstyle::getName(jah_layer->text->fontstyle) << "'" ;  
    ai_fstream << " extrude_depth = '" << jah_layer->layernodes->m_node->extrude << "'" ;  

    ai_fstream << " />" << "\n";  

}  // save scene files to disk

void GLWorld::SaveAsset( std::fstream & ai_fstream , JahLayer* jah_layer ) 
{
    assetData asset = jah_layer->asset;

    if (asset.theCategory == VideoCategory::NOCATEGORY)
    {
        asset.theCategory = VideoCategory::IMAGE;
        asset.theType = VideoType::STILL;
        asset.clipname = "Empty";
        asset.clipnumber = 1;
        asset.clipname = "jahlayer"; 
        asset.filename = "jahlayer"; 
        asset.extension = ".bmp";
        asset.location = m_qsJahBasePath + "Pixmaps/";
        asset.videoext = "";
    }

    ai_fstream << sceneGetIndent() << "<ASSET" ;  

    ai_fstream << " layer_status = '" << jah_layer->layerAssetStatus << "'" ;  

    ai_fstream << JahDataIO::JahParse( " category = '" , asset.theCategory ).data( ) << "'" ;  
    ai_fstream << JahDataIO::JahParse( " type = '" , asset.theType ).data( ) << "'" ;  

    ai_fstream << " clipname = '" << asset.clipname.data( ) << "'" ;  
    ai_fstream << " clipnumber = '" << asset.clipnumber << "'" ;  

    ai_fstream << "\n" ;

    sceneIncreaseIndent();
    char l_location[ JahDataIO::c_max_path ]; 
    ( void ) JahDataIO::getPathWithoutFinalSlash( l_location , asset.location.data( ) );
    ai_fstream << sceneGetIndent() << " location = '" << l_location << "/'" ; 

    ai_fstream << "\n" ;

    ai_fstream << sceneGetIndent() << " filename = '" << asset.filename.data( ) << "'" ;  

    char l_extension[ JahDataIO::c_max_path ]; 
    ( void ) JahDataIO::getExtension( l_extension , asset.extension.data( ) );
    ai_fstream << " extension = '." << l_extension << "'" ; 

    ai_fstream << "\n" ;

    ai_fstream << sceneGetIndent() << " startframe = '" << asset.startframe << "'" ;  
    ai_fstream << " endframe = '" << asset.endframe << "'" ;  
    ai_fstream << " clipinframe = '" << asset.clipinframe << "'" ;  
    ai_fstream << " clipoutframe = '" << asset.clipoutframe << "'" ;

    // Not sure why we have to do this.  Prevents segmentation violations
    if (asset.videoext != NULL)
    {
        ai_fstream << " videoext = '" << asset.videoext.data( ) << "'" ;  
    }
    else
    {
        ai_fstream << " videoext = ''" ;  
    }

    ai_fstream << " videotex = '" << asset.videotex << "'\n";  

    ai_fstream << sceneGetIndent();
    //this is not asset data here but is used for video slip and should be stored in the asset
    ai_fstream << " slip = '" << jah_layer->getSlipFrameOffset() << "'" ;  
    ai_fstream << " inframe_number = '" << jah_layer->m_inframe << "'" ;  
    ai_fstream << " outframe_number = '" << jah_layer->m_outframe << "'" ;  
    ai_fstream << " showedges = '" << jah_layer->showEdges << "'" ;  


    ai_fstream << " loop = '" << jah_layer->loop << "'" ;  
    ai_fstream << " ping = '" << jah_layer->ping << "'" ;  

    //this is not really needed as its only for the desktop
    //but we need to set it to 0...    
    ai_fstream << " xpos = '" << asset.xpos << "'" ;  
    ai_fstream << " ypos = '" << asset.ypos << "'" ;  

    ai_fstream << " />" << "\n";
    sceneDecreaseIndent();

}  

void GLWorld::SaveKeyAsset( std::fstream & ai_fstream , unsigned int const & a_i ) 
{
    unsigned int const & i = a_i;
    JahLayer* jah_layer = getLayerList()->at(i)->thelayer;

    ai_fstream << sceneGetIndent() << "<KEYASSET" ;  

    ai_fstream << JahDataIO::JahParse( " category = '" , jah_layer->keyasset.theCategory ).data( ) << "'" ;  
    ai_fstream << JahDataIO::JahParse( " type = '" , jah_layer->keyasset.theType ).data( ) << "'" ;  

    ai_fstream << " clipname = '" << jah_layer->keyasset.clipname.data( ) << "'" ;  
    ai_fstream << " clipnumber = '" << jah_layer->keyasset.clipnumber << "'" ;  

    ai_fstream << "\n" ;

    sceneIncreaseIndent();

    char l_location[ JahDataIO::c_max_path ]; 
    ( void ) JahDataIO::getPathWithoutFinalSlash( l_location , jah_layer->keyasset.location.data( ) );
    ai_fstream << sceneGetIndent() << " location = '" << l_location << "/'\n" ; 
    ai_fstream << sceneGetIndent() << " filename = '" << jah_layer->keyasset.filename.data( ) << "'" ;  

    char l_extension[ JahDataIO::c_max_path ]; 
    ( void ) JahDataIO::getExtension( l_extension , jah_layer->keyasset.extension.data( ) );
    ai_fstream << " extension = '." << l_extension << "'" ; 

    ai_fstream << "\n" ;

    ai_fstream << sceneGetIndent() << " startframe = '" << jah_layer->keyasset.startframe << "'" ;  
    ai_fstream << " endframe = '" << jah_layer->keyasset.endframe << "'" ;  

    if ( jah_layer->keyasset.videoext.length() ) 
    {
        ai_fstream << " videoext = '" << jah_layer->keyasset.videoext.data( ) << "'" ; 
    }
    else
    {
        ai_fstream << " videoext = ''" ; 
    }

    ai_fstream << " videotex = '" << jah_layer->keyasset.videotex << "'" ;  
    ai_fstream << " blendmode = '" << SceneFileXml::getBlendmodeString(jah_layer->blendmode2) << "'" ;  

    //this is not asset data here but is used for video slip and should be stored in the asset
    ai_fstream << " slip = '" << jah_layer->getKeySlipFrameOffset() << "'" ;  
    ai_fstream << " inframe_number = '" << jah_layer->m_keyinframe << "'" ;  
    ai_fstream << " outframe_number = '" << jah_layer->m_keyoutframe << "'" ;  

    //this is not really needed as its only for the desktop
    //but we need to set it to 0...    
    ai_fstream << " xpos = '" << jah_layer->keyasset.xpos << "'" ;  
    ai_fstream << " ypos = '" << jah_layer->keyasset.ypos << "'" ;  

    ai_fstream << " />" << "\n";

    sceneDecreaseIndent();
}  

void
GLWorld::saveLightLayers(std::fstream& ai_fstream, JahLayer* jah_layer)
{
    QPtrList<JahLayer>* lights_list = jah_layer->getLightsList();
	JahLayer* light_layer = lights_list->first();

	for ( ; light_layer; light_layer = lights_list->next() )
	{
		ai_fstream << sceneGetIndent() << "<LIGHT " << "enabled = '";
        ai_fstream << light_layer->layerIsEnabled() << "'>\n";
		sceneIncreaseIndent();
		SaveNodes( ai_fstream, light_layer );
		sceneDecreaseIndent();
		ai_fstream << sceneGetIndent() << "</LIGHT>\n";
	}

}


void
GLWorld::saveEffectLayers(std::fstream& ai_fstream, JahLayer* jah_layer)
{
	QPtrList<EffectLayer>* effects_list = jah_layer->getEffectsList();
	EffectLayer* effect_layer = effects_list->first();

	for ( ; effect_layer; effect_layer = effects_list->next() )
	{
		JahLayer* effect_jah_layer = effect_layer->getJahLayer();
		ai_fstream << sceneGetIndent() << "<EFFECT " ;
		EffectInfo::EFFECT_CATEGORY category = effect_jah_layer->getEffectCategory();
		ai_fstream << "category = '" << EffectInfo::getEffectCategoryName(category) << "'";
		ai_fstream << ">" << "\n"; 
		sceneIncreaseIndent();

		if (effect_layer->getType() == EffectInfo::JAH_PLUGIN_TYPE)
		{
			ai_fstream << sceneGetIndent() << "<PLUGIN " ;
			ai_fstream << "name = '" << std::string( effect_layer->effectname.data() ) << "'";
			ai_fstream << " guid = '" << effect_layer->getGuid() << "'";
			ai_fstream << " />" << "\n" ; 
		}
		else
		{
			ai_fstream << sceneGetIndent() << "<BUILTIN " ;
			EffectInfo::EFFECT_TYPE type = effect_layer->getType();
			ai_fstream << "name = '" << std::string( effect_layer->effectname.data() ) << "'";
			ai_fstream << " type = '" << EffectInfo::getEffectTypeName(type) << "'";
			ai_fstream << " />" << "\n" ; 
		}

		SaveNodes( ai_fstream, effect_jah_layer );

		sceneDecreaseIndent();
		ai_fstream << sceneGetIndent() << "</EFFECT>" << "\n";
	}

}

void
GLWorld::saveHeaders(std::fstream& l_file_stream, std::string const & a_save_filename, std::string module_name)
{
    l_file_stream << "<?xml version='1.0' encoding='utf-8' ?>" << "\n" ; 

    l_file_stream << "<!-- Jahshaka Scene File -->" << "\n\n" ; 

    l_file_stream << "<JAHSHAKA" ;
    //l_file_stream << " jah_version = '" << jprefs.getJahVersion() << "'" ; 
    // jahversion set explicitly - FIXME
    l_file_stream << " jah_version = '" << "2.0RC2" << "'" ;
    l_file_stream << " xml_file_version = '0.01'" ;
    l_file_stream << ">" << "\n" ;  

    sceneIncreaseIndent();
    l_file_stream << sceneGetIndent() << "<SCENE_FILE" ;
    l_file_stream << " name = '" ;
    l_file_stream << JahDataIO::ParseDataFilename( a_save_filename.data( ) ).data( ) << "'" ;
    l_file_stream << " />" << "\n" ;  

    if ( getSceneSaveExport() )
    {
        l_file_stream << sceneGetIndent() << "<EXPORT/>\n" ;
    }

    l_file_stream << sceneGetIndent() << "<MODULE" ;
    l_file_stream << " name = '" << module_name << "'" ;  
    l_file_stream << " />" << "\n" ;

    l_file_stream << sceneGetIndent() << "<FRAMES" ; 
    projectData project_data;
    QString projectResVal = project_data.getTitle(
						  m_nJahResolutionValue
						  );
    l_file_stream << " resolution = '" << projectResVal.data() << "'" ;
    l_file_stream << " code = '" << m_nJahResolutionValue << "'" ;
    l_file_stream << " number = '" << m_nMaxFrame << "'" ; 
    l_file_stream << " />" << "\n" ;
}

void
GLWorld::saveEffects()
{
	JahLayer* jah_layer = getActiveJahLayer();

    setSceneSaveIndentationDepth(0);

    if (jah_layer == NULL)
	{
		return;
	}

	if (jah_layer->getCategory() == ObjectCategory::EFFECT)
	{
		jah_layer = jah_layer->getParent();
	}

	if (jah_layer == NULL)
	{
		return;
	}

	if ( !canAcceptEffects( jah_layer->getCategory() ) )
	{
		return;
	}

	QPtrList<EffectLayer>* effects_list = jah_layer->getEffectsList();

	if (effects_list == NULL)
	{
		return;
	}


	QString name =
	  QFileDialog::getSaveFileName(
				       m_qsScenePath,
				       "*.jsf",
				       this );

	if ( getAutoSaveInProgress() )
	{
		QMessageBox::information(0, "Auto save in progress - try again in a few seconds", name);
		return;
	}

	if ( name.isEmpty() ) 
	{
		return;
	}

	if (!name.endsWith(".jsf"))
	{
		name += ".jsf";
	}

	std::string name_string = name.data();
	saveEffects(name_string, jah_layer);
}

void
GLWorld::saveEffects(std::string& file_name, JahLayer* jah_layer)
{
	std::string text;
	std::string JahVersion;

	// Needed to initialize static members in constructor;
	SceneFileXml scene_file_xml( text = "" );

    //initalize the global vars from the prefs
    JahPrefs & jprefs = JahPrefs::getInstance( );
    JahVersion  = jprefs.getJahVersion( ).data( );

    std::fstream file_stream;
    file_stream.open( file_name.data() , std::ios::out );

    if ( file_stream.is_open( ) == false && !getAutoSaveInProgress() ) 
    {
        QMessageBox::information(0, "Could not write to file  ", file_name.data( ) );
        return;
    }

	int last_slash = int( file_name.find_last_of("/") );
	int last_period = int( file_name.find_last_of(".") );
	int count = last_period - 1 - last_slash;
	std::string layer_name = file_name.substr(last_slash + 1, count);
	saveHeaders( file_stream, file_name, "Effects");
	file_stream << sceneGetIndent() << "<LAYER name = '" << layer_name << "' activation = '1'>\n";

	sceneIncreaseIndent();
	file_stream << sceneGetIndent() << "<KEY invert = '0' draw = '0' status = '0' keyinframe = '0' keyoutframe = '0' />\n";
    file_stream << sceneGetIndent() << "<TYPE layertype = 'LAYER' objtype = 'MESHLAYER' />\n";
    saveEffectLayers(file_stream, jah_layer);
	SaveProperties(file_stream, jah_layer);
	SaveAsset( file_stream, jah_layer );
	SaveNodes( file_stream, jah_layer );
	sceneDecreaseIndent();

	file_stream << sceneGetIndent() << "</LAYER>\n";

    sceneDecreaseIndent();
    file_stream << "</JAHSHAKA>" << "\n";  


    ///////////////////////////////////////////////////////////////////////////
    //now close the file and let the user know
    file_stream.close();

    if ( !getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "File saved successfully ", file_name.data( ) );
    }	
}


void
GLWorld::SaveProperties( std::fstream& file_stream, JahLayer* jah_layer )
{
	file_stream << sceneGetIndent() << "<PROPERTIES" ;
	file_stream << " blend = '" << jah_layer->blend << "'" ;
	file_stream << " depth = '" << jah_layer->depth << "'" ;
	file_stream << " cull = '" << jah_layer->cull << "'" ;
	file_stream << " lighting = '" << jah_layer->getLightingEnabled() << "'" ;
    file_stream << " translate_first = '" << jah_layer->getTranslateFirst() << "'" ;
	file_stream << " foreground = '" << jah_layer->foreground << "'" ;
	file_stream << " smooth = '" << jah_layer->smooth << "'" ;
	file_stream << " reflect = '" << jah_layer->reflect << "'" ;
	file_stream << " />" << "\n" ;
}

void 
GLWorld::SaveTrackerData( std::fstream& ai_fstream )
{
    LayerListEntry* layer_list_entry = getLayerList()->first();
    layer_list_entry = getLayerList()->next();
    JahLayer* jah_layer = layer_list_entry->getJahLayer();
    ai_fstream << sceneGetIndent() << "<LAYER" ;
    ai_fstream << " name = '" << jah_layer->layername.data() << "'" ;
    ai_fstream << " activation = '" << jah_layer->layerStatus << "'" ;
    ai_fstream << ">" << "\n" ; 
    sceneIncreaseIndent();
    ai_fstream << sceneGetIndent() << "<TYPE layertype = 'LAYER' objtype = 'JAHLAYER' composite_type = 'Standard' />\n";

    if (jah_layer->layerAssetStatus)
    {
        SaveAsset(ai_fstream, jah_layer);
    }

    sceneDecreaseIndent();
    
    layer_list_entry = getLayerList()->next();

    for ( ; layer_list_entry; layer_list_entry = getLayerList()->next() )
    {
        jah_layer = layer_list_entry->getJahLayer();

        ai_fstream << sceneGetIndent() << "<LAYER" ;
        ai_fstream << " name = '" << jah_layer->layername.data() << "'" ;
        ai_fstream << " activation = '" << jah_layer->layerStatus << "'" ;
        ai_fstream << ">" << "\n" ; 

        sceneIncreaseIndent();
        ai_fstream << sceneGetIndent() << "<TYPE layertype = 'TRACKER_POINT' objtype = 'TRACKER_POINT' composite_type = 'Standard' />\n";
        SaveNodes(ai_fstream, jah_layer);
        sceneDecreaseIndent();
    }

}
