/*******************************************************************************
**
** The source file for the Jahshaka color objects module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"

/*
#include "jahplugins.h"
#include "jahplugintemplate.h"

////////////////////////////////////////////////
//plugin routines here
//we need to split this into a calling routine and 3 subroutines
//but we need to make sure we are sending the image over to the cpufx
//the most accurate way ie using pointers...
//to reduce the memory latency of passing back and forth

void GLColorize::doPlugin(QImage& pix, int i)
{

    //used to print out timing data for the effects
    bool printFxTimeData = false;

    //first get the number of the plugin se we call the right one...
    //int plugFx = effects->at(i)->fxlayer->pluginnumber;
    int plugFx = m_layerlist->at(1)->fxPluginNumber(i-2);

    if (jplugin->vfxPlugins->at(plugFx)->isPluginLoaded())
    {

        //first check if it is a imagefx or a gpufx
        int pluginclass = jplugin->vfxPlugins->at(plugFx)->thePlugin->getClass();

        if (pluginclass==JAH_CPU_PLUGIN) //cpufx
        {

            //send the source image over
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setImage(pix);

            //sends local slider settings over to plugin
            udpatePluginSettings(plugFx);

            ////////////////////////////////////
            //execute plugin
            if (printFxTimeData) fxtimer->starttime();

            //main call here
            jplugin->vfxPlugins->at(plugFx)->thePlugin->processImage();

            if (printFxTimeData)
            {
                fxtimer->endtime();
                //fxtimer->printtime("Time for "+jplugin->vfxPlugins->at(plugFx)->getPluginName());
				std::string l_time = "Time for ";
				l_time += jplugin->vfxPlugins->at(plugFx)->getPluginName().data();
                fxtimer->printtime(l_time);
            }

            //get the result
            pix = jplugin->vfxPlugins->at(plugFx)->thePlugin->getImage();
        }

        if (pluginclass==JAH_GPU_PLUGIN) //gpufx
        {

            //send data for gpufx over
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setGpuImageSize(m_layerlist->at(1)->thelayer->textureData.width,
                                                                        m_layerlist->at(1)->thelayer->textureData.height);

            jplugin->vfxPlugins->at(plugFx)->thePlugin->setTexRatios(m_layerlist->at(1)->thelayer->textureData.texwidthratio,
                                                                    m_layerlist->at(1)->thelayer->textureData.texheightratio);

            jplugin->vfxPlugins->at(plugFx)->thePlugin->setCoreSize(core->m_renderwidth,core->m_renderheight);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setCameraDistance(camera_distance);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setTexId(m_layerlist->at(1)->thelayer->textureData.texID);

            jplugin->vfxPlugins->at(plugFx)->thePlugin->setScreenWidth(core->m_renderwidth);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setScreenHeight(core->m_renderheight);

            if (jplugin->vfxPlugins->at(plugFx)->thePlugin->usesMesh)
            {
                jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshXCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshXCoord);
                jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshYCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshYCoord);
                jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshZCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshZCoord);

                int xd = m_layerlist->at(1)->thelayer->JAHMESH_X_DIMENSION;
                int yd = m_layerlist->at(1)->thelayer->JAHMESH_Y_DIMENSION;

                jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshXDimension(xd);
                jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshYDimension(yd);
            }

            //sends local slider settings over to plugin
            udpatePluginSettings(plugFx);

            ///////////////////////////////////////////////////
            //execute plugin
            if (printFxTimeData) fxtimer->starttime();

            jplugin->vfxPlugins->at(plugFx)->thePlugin->processImage();

            if (printFxTimeData)
            {
                fxtimer->endtime();
//                fxtimer->printtime("Time for "+jplugin->vfxPlugins->at(plugFx)->getPluginName());
				std::string l_time = "Time for ";
				l_time += jplugin->vfxPlugins->at(plugFx)->getPluginName().data();
                fxtimer->printtime(l_time);
            }
        }

        if (pluginclass==JAH_MESH_PLUGIN) //gpufx
        {

            jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshXCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshXCoord);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshYCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshYCoord);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshZCoord((GLfloat *)m_layerlist->at(1)->thelayer->MeshZCoord);

            int xd = m_layerlist->at(1)->thelayer->JAHMESH_X_DIMENSION;
            int yd = m_layerlist->at(1)->thelayer->JAHMESH_Y_DIMENSION;

            jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshXDimension(xd);
            jplugin->vfxPlugins->at(plugFx)->thePlugin->setMeshYDimension(yd);

            //sends local slider settings over to plugin
            udpatePluginSettings(plugFx);

            ///////////////////////////////////////////////////
            //execute plugin
            if (printFxTimeData) fxtimer->starttime();

            jplugin->vfxPlugins->at(plugFx)->thePlugin->processImage();

            if (printFxTimeData)
            {
                fxtimer->endtime();
                //fxtimer->printtime("Time for "+jplugin->vfxPlugins->at(plugFx)->getPluginName());
				std::string l_time = "Time for ";
				l_time += jplugin->vfxPlugins->at(plugFx)->getPluginName().data();
                fxtimer->printtime(l_time);
            }
        }

    }
}

//can we move this into effectplugins.cpp ?
void GLColorize::udpatePluginSettings(int pl)
{
    //JahPluginLib* jplugin = JahPluginLib::getInstance();


    //set plugin values - get this data from the plugin please
    if (jplugin->vfxPlugins->at(pl)->isPluginLoaded())
    {

        int ns = jplugin->vfxPlugins->at(pl)->getNumberSliders(); //      3;
        int nb = jplugin->vfxPlugins->at(pl)->getNumberButtons();  //      0;


        //these should not work on plugin directly but should use
        //vfxplugin class routines instead
        //but since they are called so frequently... we are going direct for now

        if (ns>0)
        {
            for (int x=0; x<=ns; x++)
            {

                switch ( x )
                {
                    case 1    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->rx ); break; }
                    case 2    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->ry ); break; }
                    case 3    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->rz ); break; }
                    case 4    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->tx ); break; }
                    case 5    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->ty ); break; }
                    case 6    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->tz ); break; }
                    case 7    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->sx ); break; }
                    case 8    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->sy ); break; }
                    case 9    : { jplugin->vfxPlugins->at(pl)->thePlugin->setSlider( 
                                    x,m_layerlist->at(effectobj)->thelayer->layernodes->m_node->sz ); break; }

                    default   : { break; }
                }


            }
        }

        //we need to add buttons to ui first :)
        if (nb>0)
        {
            for (int x=1; x<=nb; x++)
            {

                switch ( x )
                {
                    case 1    : { jplugin->vfxPlugins->at(pl)->thePlugin->setOption( 
                                    x,m_layerlist->at(effectobj)->thelayer->option1 ); break; }
                    case 2    : { jplugin->vfxPlugins->at(pl)->thePlugin->setOption( 
                                    x,m_layerlist->at(effectobj)->thelayer->option2 ); break; }
                    case 3    : { jplugin->vfxPlugins->at(pl)->thePlugin->setOption( 
                                    x,m_layerlist->at(effectobj)->thelayer->option3 ); break; }
                    case 4    : { jplugin->vfxPlugins->at(pl)->thePlugin->setOption( 
                                    x,m_layerlist->at(effectobj)->thelayer->option4 ); break; }

                    default   : { break; }
                }
            }
        }
    }

}

*/

