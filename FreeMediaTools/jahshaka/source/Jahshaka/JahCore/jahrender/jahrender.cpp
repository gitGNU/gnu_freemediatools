/*******************************************************************************
**
** The header file for the Jahshaka render module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahrender.h"
#include "glworld.h"


//#include "GL/glext.h"


JahRender::JahRender(GLWorld* glworld_ptr)
{ 
    jtrace = JahTrace::getInstance();	//set up tracer

    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    JahBasePath  = jprefs.getBasePath().data();
    setGlworldPtr(glworld_ptr);

}

JahRender::~JahRender(void) { }

void JahRender::setToolMode(MouseMode mmode) 
{ 
    theToolMode = mmode; 
}

void JahRender::positionObject( motionNode * node ) 
{
    if ( thelayer->getTranslateFirst() )
    {
        glTranslatef( node->tx, node->ty, node->tz );
    }

    glRotatef( node->rx, 1.0, 0.0, 0.0 );
    glRotatef( node->ry, 0.0, 1.0, 0.0 );
    glRotatef( node->rz, 0.0, 0.0, 1.0 );

    if ( !thelayer->getTranslateFirst() )
    {
        glTranslatef( node->tx, node->ty, node->tz );
    }


    glScalef ( node->sx, node->sy, node->sz );
    //glScalef ( (node->sx / 10.0) + 0.9, (node->sy / 10.0) + 0.9, (node->sz / 10.0) + 0.9 );

}

int 
JahRender::drawLights(JahLayer* jah_layer)
{
    int number_of_enabled_lights = 0;
    JahLayer* camera = jah_layer->getCameraLayer();

    QPtrList<JahLayer>* light_list = NULL;

    int light_number = 0;

    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            // Global lights
            light_list = camera->getLightsList();
        }
        else if ( jah_layer->getLightsList() && jah_layer->getLightsList()->count() )
        {
            // Local lighting
            light_list = jah_layer->getLightsList();
        }
        else
        {
            // No local lights
            return number_of_enabled_lights;
        }

        if (i == 1 && jah_layer == camera)
        {
            // Don't light the camera layer twice
            return number_of_enabled_lights;
        }

        JahLayer* light_layer = light_list->first();

        for ( ; light_layer; light_layer = light_list->next() )
        {
            if ( light_layer->layerIsEnabled() )
            {
                number_of_enabled_lights++;
                unsigned int gl_light_enum = GL_LIGHT0 + light_number;

                if (light_number <= GL_MAX_LIGHTS)
                {
                    motionNode* motion_node = light_layer->layernodes->m_node;

                    glEnable(GL_LIGHTING); 
                    glEnable(gl_light_enum);

                    //float directional = 0.0f;
                    float non_directional = 1.0f;

                    float4 light_position(motion_node->tx, motion_node->ty, motion_node->tz, non_directional);

                    if (jah_layer->objtype == ObjectCategory::OBJECT)
                    {
                        // OpenObjectLib coords are out of sync with rest of app
                        // So we need this hack
                        light_position.z += Globals::getCameraDistance();
                    }

                    light_position /= 5.0f;

                    glLightfv(gl_light_enum, GL_AMBIENT, (const GLfloat*)&motion_node->ambient_light_color.x);
                    glLightfv(gl_light_enum, GL_DIFFUSE, (const GLfloat*)&motion_node->diffuse_light_color.x);
                    glLightfv(gl_light_enum, GL_SPECULAR, (const GLfloat*)&motion_node->specular_light_color.x);
                    glLightfv(gl_light_enum, GL_POSITION, (const GLfloat*)&light_position);

                    light_number++;
                }
            }
        }
    }

    return number_of_enabled_lights;
}

/////////////////////////////////////////////////////////
//the core routine that draws the layer into OpenGL space
// we need ways to speed this up as its the core routine
/////////////////////////////////////////////////////////

void JahRender::drawLayer(JahLayer * layer, bool set_position) 
{
	/////////////////////////////////////////////////////
	//initialize our local layer object
	thelayer = layer;

    int number_of_enabled_lights = 0;

	/////////////////////////////////////////////////////
	//parse layer data

    if ( thelayer->layerStatus )
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        //now we set up drawing variables and draw the object
        if ( thelayer->layerfog ) 
		{ 
			glEnable(GL_FOG); 
		}                     

        // used for object selection - background is excluded
        //if (layer->objtype!=ObjectCategory::BACKDROP || layer->objtype!=ObjectCategory::MAINBACK) 
		//{  glLoadName(layer->layernumber); }
        
		if (thelayer->objtype!=ObjectCategory::BACKDROP || thelayer->objtype!=ObjectCategory::MAINBACK) 
		{  
			glLoadName(layer->asset.clipnumber); //not clipnumber but rather sortnumber!
		}

        // set object color and transparency
        glColor4f( thelayer->Red,  thelayer->Green,  thelayer->Blue, ( thelayer->layernodes->m_node->Alpha/100) );

        if ( thelayer->depth)      
		{   
             glEnable(GL_DEPTH_TEST);       
		}

        std::string layer_name;

#ifndef JAHPLAYER

        JahLayer* camera = thelayer->getCameraLayer();

        if ( thelayer->canAcceptLighting() && thelayer->getLightingEnabled() &&
            (   ( thelayer->getLightsList() && thelayer->getLightsList()->count() ) 
             || ( camera && camera->getLightsList() && camera->getLightsList()->count() ) )
           )
        {
            float alpha = thelayer->layernodes->m_node->Alpha;

            if ( thelayer->getLightingNeedsToUpdateCompositeTexture() )
            {    
                if ( thelayer->getTextureCompositeDirty() &&
                     layer->getCompositeTextureUpdatedByLighting() )
                {
                    // The last time the composite was updated it was here,
                    // so start over from the base image
                    thelayer->setTextureCompositeDirty(false);
                }

                drawQuadToPbufferAndUpdateCompositeTexture(thelayer);
                thelayer->setCompositeTextureUpdatedByLighting(true);     
            }

            number_of_enabled_lights = drawLights(thelayer);
        }

        if ( thelayer->blend )     
		{    
			glEnable(GL_BLEND);
            glBlendFunc( thelayer->getSrcBlendFactor(), thelayer->getDstBlendFactor() ); 
		}

#endif

        if (set_position)
        {
            positionObject(layer->layernodes->m_node);
        }

        if ( thelayer->reflect)    
		{    
			glEnable(GL_TEXTURE_GEN_S);    
            glEnable(GL_TEXTURE_GEN_T);
		}

        if ( thelayer->smooth)     
		{    
			glShadeModel( GL_SMOOTH ); 
		} 
		else 
		{ 
			glShadeModel(GL_FLAT); 
		}

        //when culling is involved we draw the cull first
        if ( layer->cull )      
		{    
			glEnable(GL_CULL_FACE); 
            glCullFace(GL_FRONT);
            makeObject();
            glCullFace(GL_BACK); 
		}

        bool draw_glsl_key_layer = thelayer->drawtheKey && jahstd::glslSupport() && !thelayer->getUseOpenGlKeyer() &&
            (thelayer->objtype == ObjectCategory::JAHLAYER || thelayer->objtype == ObjectCategory::MESHLAYER);

        if (thelayer->drawtheKey) 
		{   
            if (draw_glsl_key_layer)
            {
                makeKeyLayerGLSL(thelayer, set_position);
            }
            else
            {
                glBlendFunc(thelayer->blendmode1, layer->blendmode2);
                float depth = thelayer->layernodes->m_node->tz;

                float z_offset = (depth > -700.0f) ? -0.2f : (depth < -1000.0f) ? -0.8f : -0.4f;
                makeKey(z_offset);
                glBlendFunc(GL_ONE, GL_ONE); 
            }
		}

		//////////////////////////////////////
        //draw the main object here
        if ( thelayer->layerStatus && !draw_glsl_key_layer )
		{   
            makeObject(); 
		}


		/////////////////////////////////////////////////
        // clean up environment
        //if ( layer->keyStatus ) { glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); }
        if ( thelayer->blend )     
		{    
			glDisable(GL_BLEND);           
		}

        if ( thelayer->lighting || number_of_enabled_lights )  
		{    
			glDisable(GL_LIGHTING); 

            for (int i = 0; i < number_of_enabled_lights; i++)
            {
                unsigned int gl_light_enum = GL_LIGHT0 + i;
                glDisable(gl_light_enum);
            }
		}

        if ( thelayer->cull)       
		{    
			glDisable(GL_CULL_FACE);       
		}

        if ( thelayer->reflect)    
		{    
			glDisable(GL_TEXTURE_GEN_S);   
			glDisable(GL_TEXTURE_GEN_T);
		}

		////////////////////////////////////////////////////////////
        //if object is selected we draw the selection profile here
        #ifndef JAHPLAYER
        if ( thelayer->selected )  
		{    
			makeTheLayerOutline();       
		}
        #endif

        if ( thelayer->depth )     
		{    
			glDisable(GL_DEPTH_TEST);      
		}

        if ( thelayer->layerfog ) 
		{     
			glDisable(GL_FOG);             
		}                     // Enables GL_FOG

        glPopMatrix();
    }
}

void JahRender::drawNodes(void) 
{
// we need ways to speed this up as its the core routine

    if ( thelayer->layerStatus )  
    {

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        makeKeyFrames();

        glPopMatrix();

    }

}


