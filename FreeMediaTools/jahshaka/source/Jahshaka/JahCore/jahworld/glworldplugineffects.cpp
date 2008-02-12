/*******************************************************************************
**
** The source file for the Jahshaka effects objects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qlabel.h>
#include <qcheckbox.h>
#include "glworld.h"
#include "glcore.h"
#include "coreobject.h"
#include <jahpluginlib.h>

////////////////////////////////////////////////
//plugin routines here
//we need to split this into a calling routine and 3 subroutines
//but we need to make sure we are sending the image over to the cpufx
//the most accurate way ie using pointers...
//to reduce the memory latency of passing back and forth
void GLWorld::parsePluginOption(int option)  //used for path global for layers
{
    EffectLayer* effect_layer = getActiveEffectLayer();
    if(effect_layer != NULL && effect_layer->getEffectInfo()->getType() == EffectInfo::JAH_PLUGIN_TYPE)
    {
	    int plugin_number = effect_layer->getPluginNumber();

        //do some error checking here
        int  nb  = m_pJPlugIn->vfxPlugins->at(plugin_number)->getNumberButtons(); 
        if (nb>=1)
            m_pJPlugIn->vfxPlugins->at(plugin_number)->setOptionStatus(option,
                m_aJahModuleOptions[option-1]->isChecked());
    }
}

void GLWorld::doPlugin(JahLayer* image_layer, int plugin_number, QImage& pix)
{
    JahPluginListElement* current_plugin = m_pJPlugIn->vfxPlugins->at(plugin_number);
	
    if (current_plugin->isPluginLoaded())
    {
        //first check if it is a imagefx or a gpufx
        int pluginclass = current_plugin->thePlugin->getClass();
		
        if (pluginclass == JAH_CPU_PLUGIN) //cpufx
        {
			
            //send the source image over
            current_plugin->thePlugin->setImage(pix);
			
            //sends local slider settings over to plugin
            udpatePluginSettings(plugin_number);
			
            ////////////////////////////////////
            //execute plugin
			
            //main call here
			try
			{
				current_plugin->thePlugin->processImage();
				
				//get the result
				pix = current_plugin->thePlugin->getImage();
			}
			catch(...)
			{
				JahTrace::getInstance()->error("GLWorld::doPlugin", "Exception thrown from CPU plugin!!");
			}
        }
		else if (pluginclass==JAH_GPU_PLUGIN) //gpufx
        {
            //send data for gpufx over
            current_plugin->thePlugin->setGpuImageSize(image_layer->textureData.width,
				image_layer->textureData.height);
	    current_plugin->
	      thePlugin->setTexRatios(
				      image_layer->textureData.texwidthratio,
				      image_layer->textureData.texheightratio);

            current_plugin->thePlugin->setCoreSize(m_pCore->getRenderWidth(),
						   m_pCore->getRenderHeight());

            current_plugin->thePlugin->setCameraDistance(m_fCameraDistance);
            current_plugin->thePlugin->setTexId(image_layer->textureData.texID);
            GLuint src_texture_id = image_layer->getBaseOrCompositeTextureId();
			current_plugin->thePlugin->setSrcTextureId(src_texture_id);
            GLuint dest_texture_id = image_layer->getTextureComposite().texID;
			current_plugin->thePlugin->setDestTextureId(dest_texture_id);
            current_plugin->thePlugin->setScreenWidth(m_pCore->getRenderWidth());
            current_plugin->thePlugin->setScreenHeight(m_pCore->getRenderHeight());
			
            if (current_plugin->thePlugin->usesMesh)
            {
                current_plugin->thePlugin->setMeshXCoord((GLfloat *)image_layer->MeshXCoord);
                current_plugin->thePlugin->setMeshYCoord((GLfloat *)image_layer->MeshYCoord);
                current_plugin->thePlugin->setMeshZCoord((GLfloat *)image_layer->MeshZCoord);
				
                int xd = image_layer->JAHMESH_X_DIMENSION;
                int yd = image_layer->JAHMESH_Y_DIMENSION;
				
                current_plugin->thePlugin->setMeshXDimension(xd);
                current_plugin->thePlugin->setMeshYDimension(yd);
            }
			
            //sends local slider settings over to plugin
            udpatePluginSettings(plugin_number);
			
            ///////////////////////////////////////////////////
            //execute plugin
			try
			{
				current_plugin->thePlugin->processImage();
			}
			catch(...)
			{
				JahTrace::getInstance()->error("GLWorld::doPlugin", "Exception thrown from GPU plugin!!");
			}
        }
		else if (pluginclass==JAH_MESH_PLUGIN) //gpufx
        {
			
            current_plugin->thePlugin->setMeshXCoord((GLfloat *)image_layer->MeshXCoord);
            current_plugin->thePlugin->setMeshYCoord((GLfloat *)image_layer->MeshYCoord);
            current_plugin->thePlugin->setMeshZCoord((GLfloat *)image_layer->MeshZCoord);
			
            int xd = image_layer->JAHMESH_X_DIMENSION;
            int yd = image_layer->JAHMESH_Y_DIMENSION;
			
            current_plugin->thePlugin->setMeshXDimension(xd);
            current_plugin->thePlugin->setMeshYDimension(yd);
			
            //sends local slider settings over to plugin
            udpatePluginSettings(plugin_number);
			
            ///////////////////////////////////////////////////
            //execute plugin
			try
			{
				current_plugin->thePlugin->processImage();
			}
			catch(...)
			{
				JahTrace::getInstance()->error("GLWorld::doPlugin", "Exception thrown from MESH plugin!!");
			}
        }
    }
}

//can we move this into effectplugins.cpp ?
void GLWorld::udpatePluginSettings(int pl)
{
    //JahPluginLib* m_pJPlugIn = JahPluginLib::getInstance();
	
	
    //set plugin values - get this data from the plugin please
    if (m_pJPlugIn->vfxPlugins->at(pl)->isPluginLoaded())
    {
		
        int ns = m_pJPlugIn->vfxPlugins->at(pl)->getNumberSliders(); //      3;
        int nb = m_pJPlugIn->vfxPlugins->at(pl)->getNumberButtons();  //      0;
		
		
        //these should not work on plugin directly but should use
        //vfxplugin class routines instead
        //but since they are called so frequently... we are going direct for now
		
        JahLayer* current_layer = getCurrentEffectLayer();
        JahKeyframeObject* keyframe_object = current_layer->layernodes;
		
        if (ns > 0)
        {
            for (int x = 1; x <= ns; x++)
            {
				
                switch ( x )
                {
				case 1    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->rx ); break; }
				case 2    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->ry ); break; }
				case 3    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->rz ); break; }
				case 4    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->tx ); break; }
				case 5    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->ty ); break; }
				case 6    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->tz ); break; }
				case 7    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->sx ); break; }
				case 8    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->sy ); break; }
				case 9    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setSlider( 
								x, keyframe_object->m_node->sz ); break; }
					
				default   : { break; }
                }
				
				
            }
        }
		
        //we need to add buttons to ui first :)
        if (nb>0)
        {
            //these are now updated on the fly vs all at one time
            //which is very ineffective and slows down plugins mon
            /*
            for (int x=1; x<=nb; x++)
            {
				
                switch ( x )
                {
				case 1    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setOption( 
								x, current_layer->option1 ); break; }
				case 2    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setOption( 
								x, current_layer->option2 ); break; }
				case 3    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setOption( 
								x, current_layer->option3 ); break; }
				case 4    : { m_pJPlugIn->vfxPlugins->at(pl)->thePlugin->setOption( 
								x, current_layer->option4 ); break; }
					
				default   : { break; }
                }
            }
            */
        }
    }
}


void				
GLWorld::setPluginAxisSliderLabels(EffectLayer* effect_layer)
{
	int plugin_number = effect_layer->getPluginNumber();
	JahPluginListElement* current_plugin = m_pJPlugIn->vfxPlugins->at(plugin_number);
				
	int number_of_sliders = current_plugin->getNumberSliders();
				
	for (int i = 0; i < number_of_sliders; i++)
	{
		QString slider_label = current_plugin->getSliderLabel(i);
		m_aJahSliderLabels[i]->setText(slider_label);
	}
}

