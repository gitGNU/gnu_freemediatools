/*******************************************************************************
**
** The source file for the Jahshaka glworldglcore.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include "glcore.h"
#include "jahrender.h"
#include "jahstats.h"
#include <map>
#include <jahtimer.h>
#include <qtimer.h>

void GLWorld::buildEffects(JahLayer* image_layer, bool draw_only_to_selected_effect)
{
    QImage renderimage;

    GLint current_shade_model;
    glGetIntegerv(GL_SHADE_MODEL, &current_shade_model);
    glShadeModel(GL_SMOOTH);

    executeEffectsList(image_layer, true, draw_only_to_selected_effect);

    image_layer->setCompositeTextureUpdatedByLighting(false);
    image_layer->setEffectsUpdated(true);

    glShadeModel(current_shade_model);    
}

void GLWorld::updatePosition( )
{
    //checks to see if the play button is on and increments frames
    if ( getAnimating() )
    {  
        updateAnimation(m_nCurrentFrame); 
        incrementCurrentFrame(1);

        if ( m_nCurrentFrame > m_nMaxFrame ) 
        {
            m_nCurrentFrame = m_nMinFrame;
            updateAnimation(m_nCurrentFrame);
            updateGL();
        }
    }
}

void GLWorld::schedulePosition( )
{
    /////////////////////////////////////////////////
    // finished the scene now Wait this many msecs before redraw
    if ( m_bAnimation ) 
    {
        startSingleShotTimer();  
    } 
}

///////////////////////////////////////////////////////////////////
// the main opengl routine that iterates through all objects
// and paints the scene for us
void GLWorld::paintGL()
{
    if ( getDontPaint() )
    {
        return;
    }

    check_gl();

    //stop the timer only if fps timing is enabled...    
    static double timefps; //to be replaced by stats object

    //start the timer only if timing is enabled...    
    if (m_bStatsShowStats)
    {
      m_pCoreTimer->starttime();
    }

    //turn on hardware aliasing for the scene if it is enabled
    if (m_bHWAliasing)
    {
        glEnable(GL_MULTISAMPLE_ARB);
    }

    //clear the scene and set up matrices
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

	/////////////////////////////////////////
	//first pass we draw the background layer
    glPushMatrix();

    //draw black video safe area if no layer present
    //if (!select) //not sure why we ifdef'd this
    //{ 
    if ( isLayerVisible( getJahLayer(0) ) )     
    {  
        m_pRenderSpace->drawLayer( getJahLayer(0), false);  
    }
    else 
    {   
        m_pRenderSpace->drawLayer(m_pTitleSafe, true); 
    }
    //}

	glPopMatrix();

	/////////////////////////////////////////
	//second pass we draw all the elements in the scene
	glPushMatrix();

    //we position the m_pCamera here
    m_pRenderSpace->positionObject(m_pCamera->layernodes->m_node);

    //draw the grid if its visible
    if (m_bGridVal) 
    {  
        m_pRenderSpace->drawLayer(m_pGrid, true); 
    }

    //set up opengl layer selection
    glInitNames(); 
    glPushName(0);                      // initialize the selection stack

    int number_of_base_layers = 1;
    drawAllLayers(number_of_base_layers);
    
    if (m_bHWAliasing)
    { 
        glDisable(GL_MULTISAMPLE_ARB);
    }

	glPopMatrix();

	////////////////////////////////////////////////
	//last pass we draw foreground locked elements 
	glPushMatrix();

    //if textsafe is on draw it as well
    if (m_pTitleSafe->layerStatus) 
    {  
        m_pRenderSpace->drawLayer(m_pTitleSafe, true);  
    }

    glFlush(); 

    if (m_bStatsShowStats)
    {
        m_pCoreTimer->endtime();
        timefps = m_pCoreTimer->returnTimeFps();

        //for now we are passing the fps over
        //what we need is a stas object that stores other data for display
        drawOverlayStats(timefps);
    }

    glPopMatrix();

    check_gl();
}

////////////////////////////////////////////////////////////////
//this routine is used to update effects before a screen redraw
void  GLWorld::updateEffects()
{
    m_bSlidersHaveChanged = true;

    if (getActiveJahLayer()->getCategory() == ObjectCategory::EFFECT)
    {
        getActiveJahLayer()->getParent()->setEffectsSlidersHaveChanged(true);
    }

}

void GLWorld::drawOverlayStats(double fps) 
{	
    ////////////////////////////////////////////
    //parse the data sent over 
    QString selectedfps = QString("Selected FPS: %1 ").arg(m_fJahFPS);
    QString coretiming  = QString("Timing: %1 ").arg( getRedrawDelay() );
    QString corefps     = QString("Core FPS: %1").arg(fps);

    QString fulldata = selectedfps + coretiming + corefps;

    std::string outtext1 = fulldata.data();

    ////////////////////////////////////////////
    //grab the rest from the stats object

    //grab the stats object
    JahStats& jstats = JahStats::getInstance();
    std::string outtext2 = jstats.getstatsdata1()+jstats.getstatsdata2();
    std::string outtext3 = jstats.getstatsdata3();

    ////////////////////////////////////////////
    //draw the text    
    glPushMatrix(); 

    //onscreen positional data
    //needs to be relative to the format 
    //ie ntsc is different from pal from 2k!
    //currently we are relative to NTSC only
    GLfloat tx  = -340.0;
    GLfloat ty  =  260.0;
    GLfloat tz  =    0.0;

    //qfonts get rendered directly from the core
    //color of text to be drawn
    glColor3f(0.0,1.0,0.0);
    m_pCore->renderText ( tx, ty, tz, outtext1.data()  );

    glColor3f(1.0,1.0,0.0);
    m_pCore->renderText ( tx, ty+40, tz, outtext2.data()  );
    m_pCore->renderText ( tx, ty+20, tz, outtext3.data()  );

    glPopMatrix();
}

void GLWorld::initializeWorld() 
{
    //set up some default variables and objects
    m_bPolySmooth=0;
    m_pActiveLayer = NULL;
    m_nCurrentFrame=1;
    m_nLastLayer = 0; 
    m_bSelect = FALSE;  // m_lastLayer is used to unselect...

    m_bAnimation = FALSE; 
    m_bAnimSliderSelected=FALSE;  

    Globals::setRendering(false);
    m_nLastX = m_nLastY = 0;  
    m_nCurrentFrame =0;

    // set up the world first   - is this necessary? what are the defaults
	if (m_pCamera)
	{ 
		m_pCamera->layernodes->m_node->sx = m_pCamera->layernodes->m_node->sy = 
        m_pCamera->layernodes->m_node->sz = m_pCamera->layernodes->m_node->sc = 1.0;
	}

    ////////////////////////////////////////////////
    // note these three should be shared display lists 
    // and not layers

    // set up the black title safe area
    //should be called rendersafe
    m_pTitleSafe->layertype = LayerCategory::LAYER; 
    m_pTitleSafe->objtype = ObjectCategory::MAINBACK;

    m_pTitleSafe->blend = TRUE; 
    m_pTitleSafe->depth = FALSE;
    m_pTitleSafe->layerStatus = TRUE;
    m_pTitleSafe->drawtheLayer = TRUE;

    // this is the green m_pTitleSafe overlay
    //m_pTitleSafe->layernodes->m_node->sx = 0.8f; 
    //m_pTitleSafe->layernodes->m_node->sy = 0.8f; //it is scaled to fit here...
    m_pTitleSafe->layertype = LayerCategory::FRAME; 
    m_pTitleSafe->objtype = ObjectCategory::TITLEGRID;
    m_pTitleSafe->Red=1.0; 
    m_pTitleSafe->Green=0.0; 
    m_pTitleSafe->Blue=0.0;
    m_pTitleSafe->blend = FALSE; 
    m_pTitleSafe->depth = FALSE;
    m_pTitleSafe->foreground = TRUE; 
    m_pTitleSafe->layerStatus = FALSE;
    m_pTitleSafe->drawtheLayer = TRUE;

    // this is the grid
    //-----------BEGIN
    m_pGrid->layertype = LayerCategory::GRID;
    m_pGrid->Red=0.75; m_pGrid->Green=0.75; m_pGrid->Blue=0.75;
    m_pGrid->blend = TRUE; m_pGrid->depth = TRUE;
    m_pGrid->layerStatus = TRUE;
    //-----------END

}

void GLWorld::initializeObjects() {   /* this is overridden */    }

void GLWorld::raiseCore(void)
{
	changeZoom( int( m_pCore->getZoom( ) ) );
    updateGL();
}



void 
GLWorld::initializeMeshLayer(int layer)
{
    JahLayer* jah_layer = getLayerList()->at(layer)->thelayer;

    initializeMeshLayer(jah_layer);
}

void 
GLWorld::initializeMeshLayer(JahLayer* layer)
{

    int                     i, j;
    GLfloat                 xx, yy;

    GLfloat                 x_unit_size;
    GLfloat                 y_unit_size;
    float                   image_width;
    float                   image_height;
    float					mesh_x;
    float					mesh_y;

    image_width = layer->getImageWidth();
    image_height = layer->getImageHeight();

    mesh_x  = (float)layer->JAHMESH_X_DIMENSION;
    mesh_y  = (float)layer->JAHMESH_Y_DIMENSION;

    x_unit_size = image_width  / mesh_x;
    y_unit_size = image_height / mesh_y;

    for (i = 0; i <= mesh_x; i++)
    {
        xx = (float)(i - layer->JAHMESH_X_DIMENSION_DIV_2) * x_unit_size;

        for (j = 0; j <= (mesh_y); j++)
        {
            yy = (float)(j - layer->JAHMESH_Y_DIMENSION_DIV_2) * y_unit_size;

            layer->setMeshCoord(i, j, xx, yy, 0.0);
            layer->setMeshNormal(i, j, 0.0f, 0.0f, 1.0f);
        }
    }
}

bool
GLWorld::imageLayerNeedsEffectsRedraw(JahLayer* image_layer)
{
    //printf("GLWorld::imageLayerNeedsEffectsRedraw : image_layer->getEffectsSlidersHaveChanged() = %d\n",
    //       image_layer->getEffectsSlidersHaveChanged() );
    return 
    (
        image_layer->getEffectsSlidersHaveChanged() 
     || getAnimationUpdated() && image_layer->getEffectsHaveKeyFrames()
     || ( image_layer->getCompositeTextureUpdatedByLighting() &&
          image_layer->getLightingNeedsToUpdateCompositeTexture()
        )

    );
}

void
GLWorld::drawAllLayers(int number_of_base_layers, bool draw_only_to_selected_effect)
{
    // Sort the layers by Z value using a map
    std::map<int, LayerListEntry*> layer_map;

    for ( int i = number_of_base_layers; i < int( getLayerList()->count() ); ++i )
    {
        JahLayer* jah_layer = getJahLayer(i);
        if ( jah_layer->objtype == ObjectCategory::EFFECT)
        {
            continue;
        }

        int depth = ( int )jah_layer->layernodes->m_node->tz;
        bool collision;

        // We need to do this to keep the map from clobbering layers at the same Z value
        do 
        {
            collision = false;

            if ( layer_map.find(depth) != layer_map.end() )
            {
                depth++;
                collision = true;
            }

        } while (collision);

        layer_map[depth] = getLayerList()->at(i);
    }

    std::map<int, LayerListEntry*>::iterator layer_map_iterator = layer_map.begin();

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    //UNUSED int left_spacing = (m_pCore->getRenderWidth() - m_nX_RESOLUTION) / 2;
    //UNUSED int bottom_spacing = (m_pCore->getRenderHeight() - m_nY_RESOLUTION) / 2;

    static bool composite_texture_initialized = false;

    if (!composite_texture_initialized)
    {
        int composite_texture_width;
        int composite_texture_height;

        getPowerOfTwoTextureSize(composite_texture_width, composite_texture_height, 
            m_nX_RESOLUTION, m_nY_RESOLUTION);

        createEmpty2DTexture(getCompositeTextureIdPtr(), GL_RGBA, 
            composite_texture_width, composite_texture_height);

        getCompositeSizeRatioPtr()->x = float(m_nX_RESOLUTION) 
	  / float(composite_texture_width);
        getCompositeSizeRatioPtr()->y = float(m_nY_RESOLUTION) 
	  / float(composite_texture_height);

        composite_texture_initialized = true;
    }


    for (; layer_map_iterator != layer_map.end(); layer_map_iterator++)
    {
        int layer_number = getLayerList()->find(layer_map_iterator->second);
        JahLayer* image_layer = getJahLayer(layer_number);
        image_layer->setEffectsUpdated(false);

        if ( isLayerVisible(image_layer) && !image_layer->foreground)     
        {
            if ( canAcceptEffects(image_layer->objtype) )
            {
                initializeMeshLayer(layer_number);

                if ( image_layer->getFirstPass() )
                {
                    image_layer->saveMeshCoords();
                    image_layer->saveMeshNormals();
                    image_layer->setFirstPass(false);
                }

                if ( imageLayerNeedsEffectsRedraw(image_layer) )
                {
                    buildEffects(image_layer, draw_only_to_selected_effect);
                    image_layer->saveMeshCoords();
                    image_layer->saveMeshNormals();
                }
                else
                {
                    image_layer->restoreMeshCoords();
                    image_layer->restoreMeshNormals();
                }
            }

            bool set_position_flag = true;

            m_pRenderSpace->drawLayer(image_layer, set_position_flag);

            //then draw the keyframes if visible
            if (image_layer->drawKeyframes)
            {
                m_pRenderSpace->drawNodes();
            }
        }

    }

    //m_pRenderSpace->makeLayerOutline();

    //glPopMatrix();

    /////////////////////////////////////////////////////////////////////////////
    //draw the foreground layers ie objects not affected by worldspace or sorting

    layer_map_iterator = layer_map.begin();

    for (; layer_map_iterator != layer_map.end(); layer_map_iterator++)
    {
        int layer_number = getLayerList()->find(layer_map_iterator->second);
        JahLayer* image_layer = getJahLayer(layer_number);

        glPushMatrix();

        if (image_layer) 
        {
            if (isLayerVisible(image_layer) && image_layer->foreground) 
            {
                if ( canAcceptEffects(image_layer->objtype) )
                {
                    initializeMeshLayer(layer_number);

                    if ( image_layer->getFirstPass() )
                    {
                        image_layer->saveMeshCoords();
                        image_layer->saveMeshNormals();
                        image_layer->setFirstPass(false);
                    }

                    if ( imageLayerNeedsEffectsRedraw(image_layer) )
                    {
                        buildEffects(image_layer, draw_only_to_selected_effect);
                        image_layer->saveMeshCoords();
                        image_layer->saveMeshNormals();
                    }
                    else
                    {
                        image_layer->restoreMeshCoords();
                        image_layer->restoreMeshNormals();
                    }
                }

                bool depth_save = image_layer->depth;

                if ( layer_map.size() == 1 )
                {
                    // Not sure why this needs to be done
                    // Without it the layer can go behind the world layer
                    // FIXME
                    m_pRenderSpace->drawLayer(image_layer, true);
                }

                image_layer->depth = false;
                m_pRenderSpace->drawLayer(image_layer, true);

                if(image_layer->drawKeyframes) 
                { 
                    m_pRenderSpace->drawNodes(); 
                }

                image_layer->depth = depth_save;
            }
        }

        image_layer->setEffectsSlidersHaveChanged(false);
        float alpha = image_layer->layernodes->m_node->Alpha;
        image_layer->setPreviousAlpha(alpha);

        glPopMatrix();
    }

    setAnimationUpdated(false);
}

