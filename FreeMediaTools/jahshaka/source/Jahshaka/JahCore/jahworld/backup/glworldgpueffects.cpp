/*******************************************************************************
**
** The source file for the Jahshaka effects objects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qdir.h>
#include "glworldlists.h"
#include "glcore.h"
#include "glworldgpueffects.h"
#include "jahrender.h"
#include <sstream>
#include <jahpluginlib.h>

void 
GLWorld::chooseNewGPUEffect(LayerListEntry* parent, QWidget* thebutton, int x_offset, int y_offset)
{
    //add on demand layers...
    QPopupMenu *menu = new QPopupMenu( 0 );
    
    int total_effect_count = getNumberEmbeddedGPUEffects();
    
    int menu_item_number = 0;
    int first_effect_index = EffectInfo::GPU_TYPE_START + 1;
    
    if (m_bNV_GPUSupport)  // None of these effects work on ATI for some reason - even the GLSL FIXME
    {
        for (int i = first_effect_index; i < EffectInfo::EFFECT_TYPE_END; i++)
        {
            EffectInfo* effect = m_aGPUEffectsSelectionVector[i];
            EffectInfo::EFFECT_TYPE effect_type = effect->getType();
            
            menu->insertItem( QString( (*effect->getName(effect_type) ).data() ), menu_item_number, 0 );
            menu_item_number++;
        }
    }
	else
	{
		int effect_number = EffectInfo::STRETCH_CROP_GPU_TYPE;
		EffectInfo* effect = 
		  m_aGPUEffectsSelectionVector[effect_number];
		EffectInfo::EFFECT_TYPE effect_type = effect->getType();
		menu->insertItem( 
				 QString( 
					 (*effect->getName(effect_type) 
					  ).data() 
					 ), menu_item_number, 0 );
		menu_item_number++;

		effect_number = EffectInfo::CHROMA_KEYER_GLSL_TYPE;
		effect = m_aGPUEffectsSelectionVector[effect_number];
		effect_type = effect->getType();
		menu->insertItem( QString( (*effect->getName(effect_type) ).data() ), menu_item_number, 0 );
		menu_item_number++;
	}


    
    // Add the plugins to the menu
    if (m_pJPlugIn->vfxPluginsCounter > 0)
    {
        total_effect_count = addPluginsToMenu(JAH_GPU_PLUGIN, total_effect_count, menu);
    }
    
    if ( !m_bGPUSupport )
    {
        for (int i=0; i <= total_effect_count; i++)
        {
            menu->setItemEnabled(i, false);
        }
    }
    
    // Select an effect from the menu    
    menu->setMouseTracking( TRUE );        
    QPoint menu_point(0, thebutton->height() + 1);

    if (x_offset)
    {
        menu_point.setX(x_offset);
        menu_point.setY(y_offset);
    }

    int effect_id = menu->exec( thebutton->mapToGlobal(menu_point) );
	addGPUEffectLayer(effect_id, parent->getJahLayer(), menu->text(effect_id).data() );
    delete menu;   
}

void
GLWorld::addGPUEffectLayer(int effect_id, JahLayer* parent_jah_layer, const char* effect_name)
{
    QString title;
    EffectInfo::EFFECT_TYPE effect_type = EffectInfo::NOT_A_TYPE;
    int plugin_id = -1;
    int first_effect_index = EffectInfo::GPU_TYPE_START + 1;
    std::string guid = "";
    bool added = TRUE;

    if ( effect_id != -1 && parent_jah_layer->layerAssetStatus )
    {
        int vector_index = effect_id + first_effect_index;
        
        if ( vector_index < EffectInfo::EFFECT_TYPE_END )
        {
            effect_type = m_aGPUEffectsSelectionVector[vector_index]->getType();
            title = QString( 
			    (*m_aGPUEffectsSelectionVector[
							   vector_index
			     ]->getName(effect_type) ).data() );
        }
        else
        {
            QString thename = effect_name;
            plugin_id = m_pJPlugIn->getPluginId(effect_name, JAH_GPU_PLUGIN);
            
            if (plugin_id >= 0)
            {
                title = m_pJPlugIn->vfxPlugins->at(plugin_id)->getPluginName();
                effect_type = EffectInfo::JAH_PLUGIN_TYPE;
                guid = std::string( m_pJPlugIn->vfxPlugins->at(plugin_id)->getGuid().data() );
            }
            else
            {
                added = FALSE;
            }
        }
    }
    
    if (added && (effect_id >= 0) && parent_jah_layer->layerAssetStatus)
    {
        addEffectLayer(parent_jah_layer->getLayerListEntry(), title, effect_type, plugin_id, guid);
        parent_jah_layer->setEffectsSlidersHaveChanged(true);
        updateGL();
    }
    
    ///////////////////////////////////////////////////
    // clean up on the way out
    m_sliders_have_changed = true;
}

////////////////////////////  NV30 Effects ////////////////////////////////


////////////////////////////////////////////////////////
// experimental work using offscreen rendering here
// for the gpgpu effects

bool GLWorld::m_bNoiseMatrixInitialized = false;
bool GLWorld::m_bContrastTextureInitialized = false;
bool GLWorld::m_random_texture_initialized = false;
int* GLWorld::m_permutation_table = NULL;
float4* GLWorld::m_gradient_table = NULL;

GLuint GLWorld::m_nGradientTexture;
GLuint GLWorld::m_permutation_texture;
GLuint GLWorld::m_turbulence_texture;
GLuint GLWorld::m_random_texture;
GLuint GLWorld::m_nContrastTexture;


void GLWorld::doNV30EarthquakeGPU(JahLayer* base_layer) {
    
    GLint                                       width;
    GLint                                       height;
    
    int                                         horizontal_mesh_size;
    int                                         vertical_mesh_size;
    float                                       float_horizontal_mesh_size;
    float                                       float_vertical_mesh_size;
    
    float                                       no_distortion_radius;
    
    float4                                      Displacement;
    
    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;
    
    unsigned char*                              vertex_program = NULL;
    unsigned char*                              fragment_program = NULL;
    
    static int                                  count = 0;
    
    static float4                               noise_scale;
    static float4                               noise_translate;
    
    float2                                      movement;
    
    int                                         ii, jj;
    GLfloat                                     i, j;
    
    int                                         num_distortion_layers;
    int                                         speed;
    
    //////////////////////////////////////////////////////////////////////
    
    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;
    
    float float_width = (float)width;
    float float_height = (float)height;
    
    ////////////////////////////////////////////////////////////////////////
    
    Displacement.x = -(getCurrentEffectLayer()->layernodes->m_node->tx / 1.0) + 27.0;
    Displacement.y = -(getCurrentEffectLayer()->layernodes->m_node->ty / 1.0) + 27.0;
    Displacement.z = (getCurrentEffectLayer()->layernodes->m_node->tz) + 80.0;
    
    num_distortion_layers = (int)(1.0 + (getCurrentEffectLayer()->layernodes->m_node->rx / 10.0));
    speed = 10 + (int)(getCurrentEffectLayer()->layernodes->m_node->ry / 10.0);
    no_distortion_radius = getCurrentEffectLayer()->layernodes->m_node->rz;
    
    
#define USE_OFFSCREEN_RENDERING         FALSE
    
#if USE_OFFSCREEN_RENDERING
    jahshaka_display = XOpenDisplay(NULL);
    jahshaka_glx_context = glXGetCurrentContext();
    jahshaka_glx_drawable = glXGetCurrentDrawable();
    
    if (shader_pbuffer == 0)
    {
        create_pbuffer(PIXEL_BUFFER_WIDTH, PIXEL_BUFFER_HEIGHT);
    }
    
    glXMakeCurrent(shader_display, shader_pbuffer, shader_context);
    
    glViewport(0, 0, width, height);
    
    glDrawBuffer(GL_FRONT);
    glReadBuffer(GL_FRONT);
#endif  // USE_OFFSCREEN_RENDERING
    
    horizontal_mesh_size = 3;
    vertical_mesh_size = 2;
    
    movement.x = (base_layer->layernodes->m_node->sx * 50.0) + 11.0;
    movement.y = movement.x;
    
    float_horizontal_mesh_size = (float)horizontal_mesh_size;
    float_vertical_mesh_size = (float)vertical_mesh_size;
    
    noise_scale.x = 1.3f;
    noise_scale.y = 1.3f;
    noise_scale.z = 1.3f;
    
    glActiveTextureARB(GL_TEXTURE0_ARB);
    base_layer->bindBaseOrCompositeTexture();
    
    if (!m_noise_matrix_initialized)
    {
        initVertexNoiseConstants(DISTORTION_TABLE_SIZE, m_permutation_table, m_gradient_table, GRADIENT_TABLE_SIZE);
        //        initializeMesh(float_width, float_height);
        m_noise_matrix_initialized = true;
    }
    
    count++;
    
    ////////////////////////////////////////////////////////////////////
    
    vertex_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/earthquake_vert_gpu.vp");
    glGenProgramsNV(1, &vertex_program_handle);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, (GLsizei)strlen((char*)vertex_program), vertex_program);
    
    ////////////////////////////////////////////////////////////////////
    
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_TEXTURE, GL_IDENTITY_NV);
    
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    
    glProgramParameters4fvNV(GL_VERTEX_PROGRAM_NV, 8, DISTORTION_TABLE_SIZE * 2, (float*)m_gradient_table);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 8 + (DISTORTION_TABLE_SIZE * 2), (float*)&m_gradient_table[0]);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 8 + (DISTORTION_TABLE_SIZE * 2) + 1, (float*)&m_gradient_table[1]);
    
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 74, float_height, 0.0, 0.0, 0.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 75, float_height / 2.0f, 0.0, 0.0, 0.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 76, no_distortion_radius, 0.0, 0.0, 0.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 77, (float*)&Displacement.x);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 78, 10000.0, 10000.0, 10000.0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 79, 0.37f, 0.5f, 1.0f, 2.0f);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 80, 0.03125f, 32.0f, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 81, 1.0f, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 82, 3.0f, 3.0f, 3.0f, 0);
    
    ////////////////////////////////////////////////////////////////////
    
    fragment_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/earthquake_frag_gpu.fp");
    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle,
        (GLsizei)strlen((char*)fragment_program), (const GLubyte*)fragment_program);
    
    if (glGetError() == GL_INVALID_OPERATION) 
    {
        find_shader_program_error(fragment_program, "earthquake_frag_gpu.fp");
    }
    
    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);
    
    
    /////////////////////////////////////////////////////////////////////
    
    GLint           current_matrix_mode;
    
    glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);
    glMatrixMode( GL_TEXTURE );
    glLoadIdentity();
    
    //draws texturized flame layers and blends them
    for (int layer = 0; layer < num_distortion_layers; ++layer)
    {
        glPushMatrix();
        
        noise_translate.x = (float)count / movement.x;
        noise_translate.y = (float)count / movement.y;
        noise_translate.z = (float)count / 300.0 / movement.y;
        
        
        //animate noise by scaling and translating
        glScalef(noise_scale.x, noise_scale.y, noise_scale.z);
        glTranslatef(noise_translate.x, noise_translate.y, noise_translate.z);
        
        GLfloat                 x_unit_size = width / (float)horizontal_mesh_size;
        GLfloat                 y_unit_size = height / (float)vertical_mesh_size;
        
        glBegin(GL_TRIANGLES);
        
        for (jj = 0; jj < vertical_mesh_size; jj++) 
        {
            j = (float)jj;
            
            for (ii = 0; ii < horizontal_mesh_size; ii++) 
            {
                i = (float)ii;
                
                // First Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055,  
                    ((j + 0) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);
                
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 1) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055, 
                    ((j + 0) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0); 
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);
                
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 0) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055, 
                    ((j + 1) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);
                
                
                // Second Triangle
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB,
                    ((i + 1) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055, 
                    ((j + 1) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);
                
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 1) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055,
                    ((j + 0) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + ((i + 1) * x_unit_size), (-float_height / 2.0) + (j * y_unit_size), 0.0);
                
                glMultiTexCoord4fARB(GL_TEXTURE0_ARB, 
                    ((i + 0) / float_horizontal_mesh_size) * base_layer->textureData.texwidthratio + 0.00055,
                    ((j + 1) / float_vertical_mesh_size) * base_layer->textureData.texheightratio, 0.0, 1.0);
                glVertex3f( (-float_width / 2.0) + (i * x_unit_size), (-float_height / 2.0) + ((j + 1) * y_unit_size), 0.0);
                
            }
        }
        
        glEnd(); //GL_QUADS
        
        glPopMatrix();   // GL_TEXTURE
        
        glBindTexture(GL_TEXTURE_2D, base_layer->getTextureComposite().texID);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - width) / 2,
            (core->getRenderHeight() - height) / 2, width, height);
        
    }
    
    ////////////////////////////////////////////////////////////////////////
    
    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);
    delete vertex_program;
    delete fragment_program;
    
    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);
    
#if USE_OFFSCREEN_RENDERING
    glXMakeCurrent(jahshaka_display, jahshaka_glx_drawable, jahshaka_glx_context);
#endif  // USE_OFFSCREEN_RENDERING
    
    glMatrixMode(current_matrix_mode);
}


//////////////////////////////////////////////////////////////////////////
//cant us this since we moved the turbulence generator out
//but cant move this out yet since we cant do meshfx in plugins yet

void GLWorld::doNV30CharcoalGPU(JahLayer* base_layer) {
    
    GLint                                       width;
    GLint                                       height;
    
    float4                                      light1_position;
    float4                                      light2_position;
    
    float                                       transparency;
    float                                       blend;
    float                                       random_factor;
    
    double                                      density;
    static double                               previous_density = 0.0;
    double                                      exponent;
    static double                               previous_exponent = 0.0;
    double                                      darker;
    
    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;
    
    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;
    
    int                                         row;
    int                                         column;
    
    float                                       f_row;
    float                                       f_column;
    
    float2                                      lower_left_tex_coords;
    float2                                      lower_right_tex_coords;
    float2                                      upper_left_tex_coords;
    float2                                      upper_right_tex_coords;
    
    float4                                      lower_left_vertex_coords;
    float4                                      lower_right_vertex_coords;
    float4                                      upper_left_vertex_coords;
    float4                                      upper_right_vertex_coords;
    
    float4                                      lower_left_normal_coords;
    float4                                      lower_right_normal_coords;
    float4                                      upper_left_normal_coords;
    float4                                      upper_right_normal_coords;
    
    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;
    
    light1_position.x = (getCurrentEffectLayer()->layernodes->m_node->rx * 5) + 200.0;
    light1_position.y = (getCurrentEffectLayer()->layernodes->m_node->ry * 5);
    light1_position.z = (getCurrentEffectLayer()->layernodes->m_node->rz * 5) + 200.0;
    light1_position.w = 1.0;
    
    transparency = -(getCurrentEffectLayer()->layernodes->m_node->tx / 100.0) + 1.5;
    blend = (getCurrentEffectLayer()->layernodes->m_node->ty / 100.0) + 1.0;
    random_factor = (getCurrentEffectLayer()->layernodes->m_node->tz / 100.0) + 1.0;
    
    light2_position.x = light1_position.x;
    light2_position.y = light1_position.y;
    light2_position.z = light1_position.z;
    light2_position.w = light1_position.w;
    
    density = (double)(((getCurrentEffectLayer()->layernodes->m_node->sx)) + 2.0);
    exponent = (double)(((getCurrentEffectLayer()->layernodes->m_node->sy - 1.0)) + 3.5);
    darker = (double)(((getCurrentEffectLayer()->layernodes->m_node->sz - 1.0) / 10.0) + 0.1);
    //printf("darker = %f\n", darker);
    
    if (!m_random_texture_initialized)
    {
        createRandomTexture(512, 512, m_random_texture);
        m_random_texture_initialized = TRUE;
    }
    
    if (!m_bContrastTextureInitialized || (exponent != previous_exponent))
    {
        if (m_bContrastTextureInitialized)
        {
            glDeleteTextures(1, &m_nContrastTexture);
        }
        
        createContrastTexture(1024, 512, density, exponent, m_nContrastTexture);
        m_bContrastTextureInitialized = TRUE;
    }
    
    vertex_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/charcoal_vert_gpu.vp");

    //qDebug("PATH:"+JahBasePath);

    glGenProgramsNV(1, &vertex_program_handle);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, (GLsizei)strlen((char*)vertex_program), vertex_program);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_MODELVIEW, GL_IDENTITY_NV);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8, GL_MODELVIEW, GL_INVERSE_TRANSPOSE_NV);
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    
    
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 12, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 13, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 14, (float*)&light1_position);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 15, (float*)&light2_position);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 16, width, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 17, height, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 18, core->getCameraDistance(), 0, 0, 0);
    
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 19, 2, 0, 1, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 20, 0.5, 0.5, 0, 0);
    
    
    glActiveTextureARB(GL_TEXTURE0_ARB);

    base_layer->bindBaseOrCompositeTexture();

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, m_nContrastTexture);
    
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture(GL_TEXTURE_2D, m_random_texture);
    
    fragment_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/charcoal_frag_gpu.fp");
    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle, 
        (GLsizei)strlen((char*)fragment_program), (const GLubyte*)fragment_program);
    
    if (glGetError() == GL_INVALID_OPERATION) 
    {
        find_shader_program_error(fragment_program, "charcoal_frag_gpu.fp");
    }
    
    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);
    
    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("darker"), 
        (const GLubyte*)"darker", darker, 0.0f, 0.0f, 0.0f);
    
    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("transparency"), 
        (const GLubyte*)"transparency", transparency, 0.0f, 0.0f, 0.0f);
    
    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("blend"), 
        (const GLubyte*)"blend", blend, 0.0f, 0.0f, 0.0f);
    
    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("random_factor"), 
        (const GLubyte*)"random_factor", random_factor, 0.0f, 0.0f, 0.0f);
    
    
    float texwidthratio = base_layer->textureData.texwidthratio;
    float texheightratio = base_layer->textureData.texheightratio;
    
    
    
    glBegin(GL_QUADS);
    
    for (row = 0; row < base_layer->JAHMESH_Y_DIMENSION; row++)
    {
        f_row = (float)row;
        
        for (column = 0; column < base_layer->JAHMESH_X_DIMENSION; column++)
        {
            f_column = (float)column;
            
            lower_left_tex_coords.x = (f_column + 0.0) / base_layer->JAHMESH_X_DIMENSION;
            lower_left_tex_coords.y = (f_row + 0.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            lower_right_tex_coords.x = (f_column + 1.0) / base_layer->JAHMESH_X_DIMENSION;
            lower_right_tex_coords.y = (f_row + 0.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            upper_right_tex_coords.x = (f_column + 1.0) / base_layer->JAHMESH_X_DIMENSION;
            upper_right_tex_coords.y = (f_row + 1.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            upper_left_tex_coords.x = (f_column + 0.0) / base_layer->JAHMESH_X_DIMENSION;
            upper_left_tex_coords.y = (f_row + 1.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            getMeshNormal(base_layer, lower_left_normal_coords, column, row, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, lower_right_normal_coords, column + 1, row, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, upper_right_normal_coords, column + 1, row + 1, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, upper_left_normal_coords, column, row + 1, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            
            base_layer->getMeshCoord(column, row,
                lower_left_vertex_coords.x,
                lower_left_vertex_coords.y,
                lower_left_vertex_coords.z);
            
            base_layer->getMeshCoord(column + 1, row,
                lower_right_vertex_coords.x,
                lower_right_vertex_coords.y,
                lower_right_vertex_coords.z);
            
            base_layer->getMeshCoord(column + 1, row + 1,
                upper_right_vertex_coords.x,
                upper_right_vertex_coords.y,
                upper_right_vertex_coords.z);
            
            base_layer->getMeshCoord(column, row + 1,
                upper_left_vertex_coords.x,
                upper_left_vertex_coords.y,
                upper_left_vertex_coords.z);
            
            
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_left_tex_coords.x * texwidthratio, 
                lower_left_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_left_tex_coords.x, lower_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_left_tex_coords.x / density, lower_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(lower_left_normal_coords.x, lower_left_normal_coords.y, lower_left_normal_coords.z);
            //             glVertex3f(lower_left_vertex_coords.x, lower_left_vertex_coords.y, lower_left_vertex_coords.z);
            glVertex2f(lower_left_vertex_coords.x, lower_left_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_right_tex_coords.x * texwidthratio, 
                lower_right_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_right_tex_coords.x, lower_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_right_tex_coords.x / density, lower_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(lower_right_normal_coords.x, lower_right_normal_coords.y, lower_right_normal_coords.z);
            //             glVertex3f(lower_right_vertex_coords.x, lower_right_vertex_coords.y, lower_right_vertex_coords.z);
            glVertex2f(lower_right_vertex_coords.x, lower_right_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_right_tex_coords.x * texwidthratio, 
                upper_right_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_right_tex_coords.x, upper_right_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_right_tex_coords.x / density, upper_right_tex_coords.y, 0.0, 1.0); 
            glNormal3f(upper_right_normal_coords.x, upper_right_normal_coords.y, upper_right_normal_coords.z);
            //             glVertex3f(upper_right_vertex_coords.x, upper_right_vertex_coords.y, upper_right_vertex_coords.z);
            glVertex2f(upper_right_vertex_coords.x, upper_right_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_left_tex_coords.x * texwidthratio, 
                upper_left_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_left_tex_coords.x, upper_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_left_tex_coords.x / density, upper_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(upper_left_normal_coords.x, upper_left_normal_coords.y, upper_left_normal_coords.z);
            //             glVertex3f(upper_left_vertex_coords.x, upper_left_vertex_coords.y, upper_left_vertex_coords.z);
            glVertex2f(upper_left_vertex_coords.x, upper_left_vertex_coords.y);
        }
    }
    
    glEnd();
    
    
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, base_layer->getTextureComposite().texID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - width) / 2, 
        (core->getRenderHeight() - height) / 2, width, height);
    
    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);
    delete vertex_program;
    delete fragment_program;
    
    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);
    
    previous_density = density;
    previous_exponent = exponent;
    
}

void GLWorld::doNV30LightingGPU(JahLayer* base_layer) {
    
    GLint                                       width;
    GLint                                       height;
    
    float4                                      light_position;
    float4                                      color;
    
    double                                      density;
    static double                               previous_density = 0.0;
    double                                      exponent;
    static double                               previous_exponent = 0.0;
    double                                      darker;
    
    GLuint                                      vertex_program_handle;
    GLuint                                      fragment_program_handle;
    
    unsigned char*                              vertex_program;
    unsigned char*                              fragment_program;
    
    int                                         row;
    int                                         column;
    
    float                                       f_row;
    float                                       f_column;
    
    float2                                      lower_left_tex_coords;
    float2                                      lower_right_tex_coords;
    float2                                      upper_left_tex_coords;
    float2                                      upper_right_tex_coords;
    
    float4                                      lower_left_vertex_coords;
    float4                                      lower_right_vertex_coords;
    float4                                      upper_left_vertex_coords;
    float4                                      upper_right_vertex_coords;
    
    float4                                      lower_left_normal_coords;
    float4                                      lower_right_normal_coords;
    float4                                      upper_left_normal_coords;
    float4                                      upper_right_normal_coords;
    
    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;
    
    light_position.x = (getCurrentEffectLayer()->layernodes->m_node->rx * 5) + 200.0;
    light_position.y = (getCurrentEffectLayer()->layernodes->m_node->ry * 5);
    light_position.z = (getCurrentEffectLayer()->layernodes->m_node->rz * 5) + 500.0;
    light_position.w = 1.0;
    
    color.x = (getCurrentEffectLayer()->layernodes->m_node->tx / 1000.0) + 1.0;
    color.y = (getCurrentEffectLayer()->layernodes->m_node->ty / 1000.0) + 1.0;
    color.z = (getCurrentEffectLayer()->layernodes->m_node->tz / 1000.0) + 1.0;
    color.w = 1.0;
    
    density = (double)(((getCurrentEffectLayer()->layernodes->m_node->sx)) + 2.0);
    exponent = (double)(((getCurrentEffectLayer()->layernodes->m_node->sy - 1.0)) + 3.5);
    darker = (double)(((getCurrentEffectLayer()->layernodes->m_node->sz - 1.0) / 10.0) + 0.1);
    //printf("darker = %f\n", darker);
    
    vertex_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/lighting_vert_gpu.vp");
    glGenProgramsNV(1, &vertex_program_handle);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    glLoadProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle, (GLsizei)strlen((char*)vertex_program), vertex_program);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 0, GL_MODELVIEW_PROJECTION_NV, GL_IDENTITY_NV);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 4, GL_MODELVIEW, GL_IDENTITY_NV);
    glTrackMatrixNV(GL_VERTEX_PROGRAM_NV, 8, GL_MODELVIEW, GL_INVERSE_TRANSPOSE_NV);
    glEnable(GL_VERTEX_PROGRAM_NV);
    glBindProgramNV(GL_VERTEX_PROGRAM_NV, vertex_program_handle);
    
    
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 12, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 13, 0.0, 1.0, 1.0, 1.0);
    glProgramParameter4fvNV(GL_VERTEX_PROGRAM_NV, 14, (float*)&light_position);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 15, width, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 16, height, 0, 0, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 17, core->getCameraDistance(), 0, 0, 0);
    
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 18, 2, 0, 1, 0);
    glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV, 19, 0.5, 0.5, 0, 0);
    
    
    glActiveTextureARB(GL_TEXTURE0_ARB);
    base_layer->bindBaseOrCompositeTexture();
    
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, m_nContrastTexture);
    
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture(GL_TEXTURE_2D, m_random_texture);
    
    fragment_program = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/lighting_frag_gpu.fp");
    glGenProgramsNV(1, &fragment_program_handle);
    glLoadProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle, 
        (GLsizei)strlen((char*)fragment_program), (const GLubyte*)fragment_program);
    
    if (glGetError() == GL_INVALID_OPERATION) 
    {
        find_shader_program_error(fragment_program, "lighting_frag_gpu.fp");
    }
    
    glEnable(GL_FRAGMENT_PROGRAM_NV);
    glBindProgramNV(GL_FRAGMENT_PROGRAM_NV, fragment_program_handle);
    
    glProgramNamedParameter4fNV(fragment_program_handle, (GLsizei)strlen("light_color"), 
        (const GLubyte*)"light_color", color.x, color.y, color.z, color.w);
    
    float texwidthratio = base_layer->textureData.texwidthratio;
    float texheightratio = base_layer->textureData.texheightratio;
    
    
    
    glBegin(GL_QUADS);
    
    for (row = 0; row < base_layer->JAHMESH_Y_DIMENSION; row++)
    {
        f_row = (float)row;
        
        for (column = 0; column < base_layer->JAHMESH_X_DIMENSION; column++)
        {
            f_column = (float)column;
            
            lower_left_tex_coords.x = (f_column + 0.0) / base_layer->JAHMESH_X_DIMENSION;
            lower_left_tex_coords.y = (f_row + 0.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            lower_right_tex_coords.x = (f_column + 1.0) / base_layer->JAHMESH_X_DIMENSION;
            lower_right_tex_coords.y = (f_row + 0.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            upper_right_tex_coords.x = (f_column + 1.0) / base_layer->JAHMESH_X_DIMENSION;
            upper_right_tex_coords.y = (f_row + 1.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            upper_left_tex_coords.x = (f_column + 0.0) / base_layer->JAHMESH_X_DIMENSION;
            upper_left_tex_coords.y = (f_row + 1.0) / base_layer->JAHMESH_Y_DIMENSION;
            
            getMeshNormal(base_layer, lower_left_normal_coords, column, row, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, lower_right_normal_coords, column + 1, row, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, upper_right_normal_coords, column + 1, row + 1, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            getMeshNormal(base_layer, upper_left_normal_coords, column, row + 1, base_layer->JAHMESH_X_DIMENSION, base_layer->JAHMESH_Y_DIMENSION);
            
            base_layer->getMeshCoord(column, row,
                lower_left_vertex_coords.x,
                lower_left_vertex_coords.y,
                lower_left_vertex_coords.z);
            
            base_layer->getMeshCoord(column + 1, row,
                lower_right_vertex_coords.x,
                lower_right_vertex_coords.y,
                lower_right_vertex_coords.z);
            
            base_layer->getMeshCoord(column + 1, row + 1,
                upper_right_vertex_coords.x,
                upper_right_vertex_coords.y,
                upper_right_vertex_coords.z);
            
            base_layer->getMeshCoord(column, row + 1,
                upper_left_vertex_coords.x,
                upper_left_vertex_coords.y,
                upper_left_vertex_coords.z);
            
            
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_left_tex_coords.x * texwidthratio, 
                lower_left_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_left_tex_coords.x, lower_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_left_tex_coords.x / density, lower_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(lower_left_normal_coords.x, lower_left_normal_coords.y, lower_left_normal_coords.z);
            //             glVertex3f(lower_left_vertex_coords.x, lower_left_vertex_coords.y, lower_left_vertex_coords.z);
            glVertex2f(lower_left_vertex_coords.x, lower_left_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, lower_right_tex_coords.x * texwidthratio, 
                lower_right_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, lower_right_tex_coords.x, lower_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, lower_right_tex_coords.x / density, lower_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(lower_right_normal_coords.x, lower_right_normal_coords.y, lower_right_normal_coords.z);
            //             glVertex3f(lower_right_vertex_coords.x, lower_right_vertex_coords.y, lower_right_vertex_coords.z);
            glVertex2f(lower_right_vertex_coords.x, lower_right_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_right_tex_coords.x * texwidthratio, 
                upper_right_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_right_tex_coords.x, upper_right_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_right_tex_coords.x / density, upper_right_tex_coords.y, 0.0, 1.0); 
            glNormal3f(upper_right_normal_coords.x, upper_right_normal_coords.y, upper_right_normal_coords.z);
            //             glVertex3f(upper_right_vertex_coords.x, upper_right_vertex_coords.y, upper_right_vertex_coords.z);
            glVertex2f(upper_right_vertex_coords.x, upper_right_vertex_coords.y);
            
            glMultiTexCoord4fARB(GL_TEXTURE0_ARB, upper_left_tex_coords.x * texwidthratio, 
                upper_left_tex_coords.y * texheightratio, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE1_ARB, upper_left_tex_coords.x, upper_left_tex_coords.y, 0.0, 1.0); 
            glMultiTexCoord4fARB(GL_TEXTURE2_ARB, upper_left_tex_coords.x / density, upper_left_tex_coords.y, 0.0, 1.0); 
            glNormal3f(upper_left_normal_coords.x, upper_left_normal_coords.y, upper_left_normal_coords.z);
            //             glVertex3f(upper_left_vertex_coords.x, upper_left_vertex_coords.y, upper_left_vertex_coords.z);
            glVertex2f(upper_left_vertex_coords.x, upper_left_vertex_coords.y);
        }
    }
    
    glEnd();
    
    
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, base_layer->getTextureComposite().texID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - width) / 2, 
        (core->getRenderHeight() - height) / 2, width, height);
    
    glDisable(GL_VERTEX_PROGRAM_NV);
    glDisable(GL_FRAGMENT_PROGRAM_NV);
    delete vertex_program;
    delete fragment_program;
    
    glDeleteProgramsNV(1, &vertex_program_handle);
    glDeleteProgramsNV(1, &fragment_program_handle);
    
    previous_density = density;
    previous_exponent = exponent;
    
}

////////////////////////////////////////////////////////
//a utility routine used by the gpgpu effects
//should pass the layer over and use the one in the gpumathlib and
//get rid of this later

void GLWorld::getMeshNormal(JahLayer* base_layer, float4& normal, int mesh_x_coordinate, int mesh_y_coordinate, 
                            int mesh_x_dimension, int mesh_y_dimension)
{
    bool                        on_the_mesh_left_edge = FALSE;
    bool                        on_the_mesh_bottom_edge = FALSE;
    bool                        on_the_mesh_right_edge = FALSE;
    bool                        on_the_mesh_top_edge = FALSE;
    bool                        on_the_mesh_edge;
    float4                      vector1;
    float4                      vector2;
    float4                      point1;
    float4                      point2;
    float4                      point3;
    float4                      normal1;
    float4                      normal2;
    float4                      normal3;
    float4                      normal4;
    
    point1.w = 1.0;
    point2.w = 1.0;
    point3.w = 1.0;
    
    if (mesh_x_coordinate == 0)
    {
        on_the_mesh_left_edge = TRUE;
    }
    
    if (mesh_y_coordinate == 0)
    {
        on_the_mesh_bottom_edge = TRUE;
    }
    
    if (mesh_x_coordinate == (mesh_x_dimension))
    {
        on_the_mesh_right_edge = TRUE;
    }
    
    if (mesh_y_coordinate == (mesh_y_dimension))
    {
        on_the_mesh_top_edge = TRUE;
    }
    
    on_the_mesh_edge = on_the_mesh_left_edge || on_the_mesh_bottom_edge || on_the_mesh_right_edge || on_the_mesh_top_edge;
    
    if (on_the_mesh_edge)
    {
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate, 
            point1.x, point1.y, point1.z);
        
        if (on_the_mesh_right_edge)
        {
            base_layer->getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate, 
                point2.x, point2.y, point2.z);
            get_vector_from_points(vector1, point2, point1);
        }
        else
        {
            base_layer->getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate,
                point2.x, point2.y, point2.z);
            get_vector_from_points(vector1, point1, point2);
        }
        
        if (on_the_mesh_top_edge)
        {
            base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
                point3.x, point3.y, point3.z);
            get_vector_from_points(vector2, point3, point1);
        }
        else
        {
            base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
                point3.x, point3.y, point3.z);
            get_vector_from_points(vector2, point1, point3);
        }
        
        
        get_cross_product_4(normal, vector1, vector2);
    }
    else
    {
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate,
            point1.x, point1.y, point1.z);
        
        // Upper right quad
        base_layer->getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate, 
            point2.x, point2.y, point2.z);
        
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
            point3.x, point3.y, point3.z);
        
        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal1, vector1, vector2);
        
        // Upper left quad
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate + 1,
            point2.x, point2.y, point2.z);
        
        base_layer->getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate,
            point3.x, point3.y, point3.z);
        
        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal2, vector1, vector2);
        
        // Lower left quad
        base_layer->getMeshCoord(mesh_x_coordinate - 1, mesh_y_coordinate,
            point2.x, point2.y, point2.z);
        
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
            point3.x, point3.y, point3.z);
        
        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal3, vector1, vector2);
        
        // Lower right quad
        base_layer->getMeshCoord(mesh_x_coordinate, mesh_y_coordinate - 1,
            point2.x, point2.y, point2.z);
        
        base_layer->getMeshCoord(mesh_x_coordinate + 1, mesh_y_coordinate,
            point3.x, point3.y, point3.z);
        
        get_vector_from_points(vector1, point1, point2);
        get_vector_from_points(vector2, point1, point3);
        get_cross_product_4(normal4, vector1, vector2);
        
        normal.x = (normal1.x + normal2.x + normal3.x + normal4.x) / 4;
        normal.y = (normal1.y + normal2.y + normal3.y + normal4.y) / 4;
        normal.z = (normal1.z + normal2.z + normal3.z + normal4.z) / 4;
        normal.w = 1.0;
        //      normal.w = (normal1.w + normal2.w + normal3.w + normal4.w) / 4;
    }
    
    normalize_vector4(normal);
}

void GLWorld::doChromaKeyerGLSLSeparateAlpha(JahLayer* key_layer) 
{
    int             key_texture_width;
    int             key_texture_height;
    static GLuint   key_texture_id;
    static bool     textures_initialized = false;

    int buffer_width = key_layer->getXResolution();
    int buffer_height = key_layer->getYResolution();
    
    getPowerOfTwoTextureSize(key_texture_width, key_texture_height, buffer_width, buffer_height);
    
    if (!textures_initialized)
    {
        createEmpty2DTexture(&key_texture_id, GL_RGBA, key_texture_width, key_texture_height);
        textures_initialized = true;
    }

    key_layer->objtype = ObjectCategory::JAHLAYER;

    setTextureImageParameters(&key_layer->textureKey, key_texture_width, key_texture_height, 
                              buffer_width, buffer_height, key_texture_id);

    motionNode* chroma_values = getCurrentEffectLayer()->layernodes->m_node;

    float red_low = chroma_values->rx;
    float red_high = chroma_values->tx;
    float green_low = chroma_values->ry;
    float green_high = chroma_values->ty;
    float blue_low = chroma_values->rz;
    float blue_high = chroma_values->tz;
    
    m_red_low = red_low / 255.0f;
    m_red_high = red_high / 255.0f;
    m_green_low = green_low / 255.0f;
    m_green_high = green_high / 255.0f;
    m_blue_low = blue_low / 255.0f;
    m_blue_high = blue_high / 255.0f;
    m_alpha_low = 0.0f;
    m_alpha_high = 0.0f;

    glslChromaKeyer(key_layer, getCompositeTextureId(), key_texture_id);

    saveEffectRenderingInBaseTexture(key_layer);

    key_layer->setIsChromaKeyLayer(true);
}

void
GLWorld::saveEffectRenderingInBaseTexture(JahLayer* base_layer)
{
    TextureImage* texture_data = &base_layer->textureKey;
    int width = base_layer->getXResolution();
    int height = base_layer->getYResolution();

    int lower_left_pixel_offset_x = ( core->getRenderWidth() - width ) / 2;
    int lower_left_pixel_offset_y = ( core->getRenderHeight() - height ) / 2;

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, texture_data->getTexId() );
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lower_left_pixel_offset_x, 
                        lower_left_pixel_offset_y, width, height);
}


static const char* keyer_module_fragment_shader =
{
        "   uniform sampler2D      base_texture;                                    \n"
        "   uniform sampler2D      key_texture;                                     \n"
        "   uniform bool           select_colors_individually;                      \n" 
        "   uniform bool           create_chroma_alpha_mask;                        \n" 
        "   uniform vec2           red_limits;                                      \n" 
        "   uniform vec2           green_limits;                                    \n" 
        "   uniform vec2           blue_limits;                                     \n" 
        "   uniform vec4           clear_color;                                     \n" 
        "   uniform float          soften_value;                                    \n" 
        "                                                                           \n"
        "   void main()                                                             \n"
        "   {                                                                       \n"
        "       vec2    base_position = gl_TexCoord[0].st;                          \n"
        "       vec2    key_position = gl_TexCoord[1].st;                           \n"
        "       vec4    base_color = texture2D(base_texture, base_position);        \n"
        "       vec4    key_color = texture2D(key_texture, key_position);           \n"
        "                                                                           \n"
        "       if (key_color.z > key_color.y)                                      \n"
        "       {                                                                   \n"
        "           key_color.z -= soften_value;                                    \n"
        "                                                                           \n"
        "           if (key_color.z < key_color.y)                                  \n"
        "           {                                                               \n"
        "               key_color.z = key_color.y;                                  \n"
        "           }                                                               \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       vec4    white = vec4(1.0, 1.0, 1.0, 1.0);                           \n"
        "                                                                           \n"
        "       float   red = key_color.x;                                          \n"
        "       float   green = key_color.y;                                        \n"
        "       float   blue = key_color.z;                                         \n"
        "       bool    clear = (red == clear_color.x) &&                           \n"
        "                       (green == clear_color.y) &&                         \n"
        "                       (blue == clear_color.z);                            \n"
        "                                                                           \n"
        "       float   red_low = red_limits.x;                                     \n"
        "       float   red_high = red_limits.y;                                    \n"
        "       float   green_low = green_limits.x;                                 \n"
        "       float   green_high = green_limits.y;                                \n"
        "       float   blue_low = blue_limits.x;                                   \n"
        "       float   blue_high = blue_limits.y;                                  \n"
        "                                                                           \n"
        "       if (create_chroma_alpha_mask)                                       \n"
        "       {                                                                   \n"
        "           gl_FragColor = white;                                           \n"
        "       }                                                                   \n"
        "       else                                                                \n"
        "       {                                                                   \n"
        "           gl_FragColor = key_color;                                       \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       if (select_colors_individually)                                     \n"
        "       {                                                                   \n"
        "           if (clear || (red >= red_low && red <= red_high) )              \n"          
        "           {                                                               \n"
        "               gl_FragColor.x = base_color.x;                              \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           if (clear || (green >= green_low && green <= green_high) )      \n"          
        "           {                                                               \n"
        "               gl_FragColor.y = base_color.y;                              \n"
        "           }                                                               \n"
        "                                                                           \n"
        "           if (clear || (blue >= blue_low && blue <= blue_high) )          \n"          
        "           {                                                               \n"
        "               gl_FragColor.z = base_color.z;                              \n"
        "           }                                                               \n"
        "                                                                           \n"
        "       }                                                                   \n"
        "       else                                                                \n"
        "       {                                                                   \n"
        "           if (                                                            \n"
        "                ( (red >= red_low && red <= red_high) &&                   \n"
        "                 (green >= green_low && green <= green_high) &&            \n"           
        "                 (blue >= blue_low && blue <= blue_high) ) || clear        \n"
        "              )                                                            \n"
        "           {                                                               \n"
        "               gl_FragColor = base_color;                                  \n"
        "           }                                                               \n"  
        "       }                                                                   \n"
        "                                                                           \n"
        "       gl_FragColor.w = 1.0;                                               \n"
        "   }                                                                       \n"
};

void
GLWorld::glslChromaKeyer(JahLayer* key_layer, GLuint base_texture_id, GLuint key_texture_id)
{
    GLsizei message_length;
    char message[1000];

    int buffer_width = key_layer->getXResolution(); 
    int buffer_height = key_layer->getYResolution();

    int lower_left_pixel_offset_x = ( core->getRenderWidth() - buffer_width ) / 2;
    int lower_left_pixel_offset_y = ( core->getRenderHeight() - buffer_height ) / 2;
    
    int buffer_width_div_2 = buffer_width / 2;
    int buffer_height_div_2 = buffer_height / 2;

    int key_texture_width;
    int key_texture_height;

    getPowerOfTwoTextureSize(key_texture_width, key_texture_height, buffer_width, buffer_height);
    
    float key_texture_width_ratio = float(buffer_width) / float(key_texture_width);
    float key_texture_height_ratio = float(buffer_height) / float(key_texture_height);

    float base_texture_width_ratio = getCompositeSizeRatioPtr()->x;
    float base_texture_height_ratio = getCompositeSizeRatioPtr()->y;
    
    float4 clear_color;
    
    if (m_shader_program == NULL)
    {
        glGetFloatv(GL_COLOR_CLEAR_VALUE, &clear_color.x); 
        m_fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, keyer_module_fragment_shader);
        m_shader_vector.push_back(m_fragment_shader);
        
        m_shader_program = new GlslProgram(m_shader_vector);
        m_shader_program_handle = *m_shader_program->getGlHandle();
        
        m_select_colors_individually_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "select_colors_individually");
        m_create_chroma_alpha_mask_handle =
            glGetUniformLocationARB(m_shader_program_handle, "create_chroma_alpha_mask");
        m_red_limits_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "red_limits");
        m_green_limits_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "green_limits");
        m_blue_limits_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "blue_limits");
        m_clear_color_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "clear_color");
        m_soften_value_handle = 
            glGetUniformLocationARB(m_shader_program_handle, "soften_value");

        
        glGetInfoLogARB(m_shader_program_handle, 1000, &message_length, message);
    }

    // Draw the key layer. 
    // Any other effects applied to this layer below the keyer will not be used
    if (key_layer->layerStatus)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        key_layer->setIsChromaKeyLayer(false);
        renderSpace->drawLayer(titlesafe, true);
        renderSpace->drawLayer(key_layer, true);
    }

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture(GL_TEXTURE_2D, key_texture_id);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lower_left_pixel_offset_x,
        lower_left_pixel_offset_y, buffer_width, buffer_height);
    glEnable(GL_TEXTURE_2D);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, base_texture_id);
    glEnable(GL_TEXTURE_2D);

    glUseProgramObjectARB(m_shader_program_handle);
    
    m_shader_program->setTextureHandle("base_texture", 0);
    m_shader_program->setTextureHandle("key_texture", 1);
    
    bool select_colors_individually = getCurrentEffectLayer()->getSelectColorsIndividually();
    bool create_chroma_alpha_mask = getCurrentEffectLayer()->getCreateChromaAlphaMask();

    float rounding_error_tolerance = 0.0001f;

    glUniform1iARB(m_select_colors_individually_handle, select_colors_individually);
    glUniform1iARB(m_create_chroma_alpha_mask_handle, create_chroma_alpha_mask);
    glUniform2fARB(m_red_limits_handle, m_red_low - rounding_error_tolerance, m_red_high + rounding_error_tolerance);
    glUniform2fARB(m_green_limits_handle, m_green_low - rounding_error_tolerance, m_green_high + rounding_error_tolerance);
    glUniform2fARB(m_blue_limits_handle, m_blue_low - rounding_error_tolerance, m_blue_high + rounding_error_tolerance);
    glUniform4fARB(m_clear_color_handle, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glUniform1fARB(m_soften_value_handle, m_soften_value);
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBegin(GL_QUADS);
    {
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f); 
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f); 
        glVertex3f(-buffer_width_div_2, -buffer_height_div_2, 0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, base_texture_width_ratio, 0.0f);           
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, key_texture_width_ratio, 0.0f);           
        glVertex3f(buffer_width_div_2, -buffer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, base_texture_width_ratio, base_texture_height_ratio);           
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, key_texture_width_ratio, key_texture_height_ratio);           
        glVertex3f(buffer_width_div_2, buffer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, base_texture_height_ratio);           
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, key_texture_height_ratio);           
        glVertex3f(-buffer_width_div_2, buffer_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);
    glUseProgramObjectARB(0);
}

static const char* keyer_effect_fragment_shader =
{
        "   uniform sampler2D      key_texture;                                     \n"
        "   uniform vec2           red_limits;                                      \n" 
        "   uniform vec2           green_limits;                                    \n" 
        "   uniform vec2           blue_limits;                                     \n" 
        "   uniform vec4           clear_color;                                     \n" 
        "   uniform float          soften_value;                                    \n" 
        "                                                                           \n"
        "   void main()                                                             \n"
        "   {                                                                       \n"
        "       vec2    key_position = gl_TexCoord[0].st;                           \n"
        "       vec4    key_color = texture2D(key_texture, key_position);           \n"
        "                                                                           \n"
        "       if (key_color.z > key_color.y)                                      \n"
        "       {                                                                   \n"
        "           key_color.z -= soften_value;                                    \n"
        "                                                                           \n"
        "           if (key_color.z < key_color.y)                                  \n"
        "           {                                                               \n"
        "               key_color.z = key_color.y;                                  \n"
        "           }                                                               \n"
        "       }                                                                   \n"
        "                                                                           \n"
        "       float   red = key_color.x;                                          \n"
        "       float   green = key_color.y;                                        \n"
        "       float   blue = key_color.z;                                         \n"
        "       bool    clear = (red == clear_color.x) &&                           \n"
        "                       (green == clear_color.y) &&                         \n"
        "                       (blue == clear_color.z);                            \n"
        "                                                                           \n"
        "       float   red_low = red_limits.x;                                     \n"
        "       float   red_high = red_limits.y;                                    \n"
        "       float   green_low = green_limits.x;                                 \n"
        "       float   green_high = green_limits.y;                                \n"
        "       float   blue_low = blue_limits.x;                                   \n"
        "       float   blue_high = blue_limits.y;                                  \n"
        "                                                                           \n"
        "       gl_FragColor = key_color;                                           \n"
        "                                                                           \n"
        "       if (                                                                \n"
        "            ( (red >= red_low && red <= red_high) &&                       \n"
        "             (green >= green_low && green <= green_high) &&                \n"           
        "             (blue >= blue_low && blue <= blue_high) ) || clear            \n"
        "          )                                                                \n"
        "       {                                                                   \n"  
        "           gl_FragColor.w = 0.0;                                           \n"
        "       }                                                                   \n"  
        "   }                                                                       \n"
};

void GLWorld::doChromaKeyerGLSL(JahLayer* key_layer) 
{
    int image_width = key_layer->getTextureData().getImageWidth();
    int image_height = key_layer->getTextureData().getImageHeight();
    
    float image_width_div_2 = float(image_width) / 2.0f;
    float image_height_div_2 = float(image_height) / 2.0f;

    float4 texture_ratios;
    key_layer->getTextureData().getTextureRatios(texture_ratios);

    motionNode* chroma_values = getCurrentEffectLayer()->layernodes->m_node;

    float red_low = chroma_values->rx / 255.0f;
    float red_high = chroma_values->tx / 255.0f;
    float green_low = chroma_values->ry / 255.0f;
    float green_high = chroma_values->ty / 255.0f;
    float blue_low = chroma_values->rz / 255.0f;
    float blue_high = chroma_values->tz / 255.0f;
    float soften_value = (chroma_values->sx - 1.0f) / 2.55f;

    static GLint                        red_limits_handle;
    static GLint                        green_limits_handle;
    static GLint                        blue_limits_handle;
    static GLint                        clear_color_handle;
    static GLint                        soften_value_handle;

    static GLhandleARB                  shader_program_handle = 0;
    static GlslShader*                  fragment_shader;
    static GlslProgram*                 shader_program = NULL;
    static std::vector<GlslShader*>     shader_vector;

    GLsizei message_length;
    char message[1000];

    float4 clear_color;
    
    if (shader_program == NULL)
    {
        glGetFloatv(GL_COLOR_CLEAR_VALUE, &clear_color.x); 
        fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, keyer_effect_fragment_shader);
        shader_vector.push_back(fragment_shader);
        
        shader_program = new GlslProgram(shader_vector);
        shader_program_handle = *shader_program->getGlHandle();
        
        red_limits_handle = 
            glGetUniformLocationARB(shader_program_handle, "red_limits");
        green_limits_handle = 
            glGetUniformLocationARB(shader_program_handle, "green_limits");
        blue_limits_handle = 
            glGetUniformLocationARB(shader_program_handle, "blue_limits");
        clear_color_handle = 
            glGetUniformLocationARB(shader_program_handle, "clear_color");
        soften_value_handle = 
            glGetUniformLocationARB(shader_program_handle, "soften_value");
        
        glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    key_layer->bindBaseOrCompositeTexture();
    glEnable(GL_TEXTURE_2D);

    glUseProgramObjectARB(shader_program_handle);
    
    shader_program->setTextureHandle("key_texture", 0);
    
    glUniform2fARB(red_limits_handle, red_low, red_high);
    glUniform2fARB(green_limits_handle, green_low, green_high);
    glUniform2fARB(blue_limits_handle, blue_low, blue_high);
    glUniform1fARB(soften_value_handle, soften_value);
    glUniform4fARB(clear_color_handle, clear_color.x, clear_color.y,
                   clear_color.z, clear_color.w);
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBegin(GL_QUADS);
    {
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f); 
        glVertex3f(-image_width_div_2, -image_height_div_2, 0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texture_ratios.x, 0.0f);           
        glVertex3f(image_width_div_2, -image_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texture_ratios.x, texture_ratios.y);           
        glVertex3f(image_width_div_2, image_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, texture_ratios.y);           
        glVertex3f(-image_width_div_2, image_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    glBindTexture(GL_TEXTURE_2D, key_layer->getTextureComposite().texID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - image_width) / 2,
        (core->getRenderHeight() - image_height) / 2, image_width, image_height);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glUseProgramObjectARB(0);
}

const char* 
GLWorld::m_tracker_fragment_shader = 
{
    "   uniform sampler2D      image_texture;                                                           \n"
    "   uniform sampler2D      point0_texture;                                                          \n"
    "   uniform sampler2D      point1_texture;                                                          \n"
    "   uniform sampler2D      point2_texture;                                                          \n"
    "   uniform sampler2D      point3_texture;                                                          \n"
    "                                                                                                   \n"
    "   uniform vec2           point0_position;                                                         \n" 
    "   uniform vec2           point1_position;                                                         \n" 
    "   uniform vec2           point2_position;                                                         \n" 
    "   uniform vec2           point3_position;                                                         \n" 
    "                                                                                                   \n"
    "   uniform float          point0_radius;                                                           \n" 
    "   uniform float          point1_radius;                                                           \n" 
    "   uniform float          point2_radius;                                                           \n" 
    "   uniform float          point3_radius;                                                           \n" 
    "                                                                                                   \n"
    "   uniform float          point0_tracking_radius;                                                  \n" 
    "   uniform float          point1_tracking_radius;                                                  \n" 
    "   uniform float          point2_tracking_radius;                                                  \n" 
    "   uniform float          point3_tracking_radius;                                                  \n" 
    "                                                                                                   \n"
    "   uniform bool           point0_enabled;                                                          \n" 
    "   uniform bool           point1_enabled;                                                          \n" 
    "   uniform bool           point2_enabled;                                                          \n" 
    "   uniform bool           point3_enabled;                                                          \n" 
    "                                                                                                   \n"
    "   uniform vec2           image_lower_left_corner;                                                 \n"
    "   uniform vec2           point_texture_step;                                                      \n"
    "   uniform vec2           image_texture_step;                                                      \n"
    "   uniform vec2           image_texture_ratio;                                                     \n"
    "   uniform vec2           point_texture_ratio;                                                     \n"
    "   uniform vec2           point_texture_size;                                                      \n"
    "                                                                                                   \n"
    "   uniform float          point0_sensitivity;                                                      \n"
    "   uniform float          point1_sensitivity;                                                      \n"
    "   uniform float          point2_sensitivity;                                                      \n"
    "   uniform float          point3_sensitivity;                                                      \n"
    "                                                                                                   \n"
    "   void main()                                                                                     \n"
    "   {                                                                                               \n"
    "       vec2    image_texture_coords = gl_TexCoord[0].st;                                           \n"
    "       vec4    image_color = texture2D(image_texture, image_texture_coords);                       \n"
    "       vec2    pixel_position = gl_FragCoord.xy;                                                   \n"
    "                                                                                                   \n"
    "       const vec4    red = vec4(1.0, 0.0, 0.0, 1.0);                                               \n"
    "       const vec4    green = vec4(0.0, 1.0, 0.0, 1.0);                                             \n"
    "       const vec4    grey = vec4(0.5, 0.5, 0.5, 1.0);                                              \n"
    "       const vec4    yellow = vec4(1.0, 1.0, 0.0, 1.0);                                            \n"
    "       const float   search_area_color_factor = 0.3;                                               \n"
    "                                                                                                   \n"
    "       gl_FragColor = image_color;                                                                 \n"
    "                                                                                                   \n"
    "       vec2 delta;                                                                                 \n"
    "       float radius;                                                                               \n"
    "       float diameter;                                                                             \n"
    "       vec2 position;                                                                              \n"
    "       float checksum;                                                                             \n"
    "       vec2 image_pixel_position = pixel_position - image_lower_left_corner;                       \n"
    "       vec2 image_texture_center = image_texture_ratio / 2.0;                                      \n"
    "       vec2 point_texture_center = point_texture_ratio / 2.0;                                      \n"
    "       vec2 point_texture_size_div_2 = point_texture_size / 2.0;                                   \n"
    "       vec2 tex_coords;                                                                            \n"
    "       vec4 expected_color;                                                                        \n"
    "       vec4 actual_color;                                                                          \n"
    "       vec4 difference;                                                                            \n"
    "                                                                                                   \n"
    "       if (point0_enabled)                                                                         \n"
    "       {                                                                                           \n"
    "           delta = pixel_position - point0_position;                                               \n"
    "           radius = sqrt( dot(delta, delta) );                                                     \n"
    "                                                                                                   \n"
    "           if (radius < point0_radius)                                                             \n"
    "           {                                                                                       \n"
    "               gl_FragColor = red;                                                                 \n"
    "           }                                                                                       \n"
    "                                                                                                   \n"
    "           if (radius < point0_tracking_radius)                                                    \n"
    "           {                                                                                       \n"
    "               checksum = 0.0;                                                                     \n"
    "               diameter = point0_radius * 2.0;                                                     \n"
    "                                                                                                   \n"
    "               for (position.x = 0.0; position.x < diameter; position.x += 1.0)                    \n"
    "               {                                                                                   \n"
    "                   for (position.y = 0.0; position.y < diameter; position.y += 1.0)                \n"
    "                   {                                                                               \n"
    "                       delta = position - point0_radius;                                           \n"
    "                       tex_coords = (point_texture_size_div_2 + delta) * point_texture_step;       \n"
    "                       expected_color = texture2D(point0_texture, tex_coords);                     \n"
    "                                                                                                   \n"
    "                       tex_coords = (image_pixel_position + delta) * image_texture_step;           \n"
    "                       actual_color = texture2D(image_texture, tex_coords);                        \n"
    "                       difference = actual_color - expected_color;                                 \n"
    "                       checksum += dot(difference, difference);                                    \n"
    "                   }                                                                               \n"
    "               }                                                                                   \n"
    "                                                                                                   \n"
	"				float color = checksum / point0_sensitivity;										\n"
	"               gl_FragColor = vec4(color, color, color, 1.0);                                      \n"
    "           }                                                                                       \n"
    "       }                                                                                           \n"
    "                                                                                                   \n"
    "       if (point1_enabled)                                                                         \n"
    "       {                                                                                           \n"
    "           delta = point1_position - pixel_position;                                               \n"
    "           radius = sqrt( dot(delta, delta) );                                                     \n"
    "                                                                                                   \n"
    "           if (radius < point1_radius)                                                             \n"
    "           {                                                                                       \n"
    "               gl_FragColor = green;                                                               \n"
    "           }                                                                                       \n"
    "                                                                                                   \n"
    "           if (radius < point1_tracking_radius)                                                    \n"
    "           {                                                                                       \n"
    "               checksum = 0.0;                                                                     \n"
    "               diameter = point1_radius * 2.0;                                                     \n"
    "                                                                                                   \n"
    "               for (position.x = 0.0; position.x < diameter; position.x += 1.0)                    \n"
    "               {                                                                                   \n"
    "                   for (position.y = 0.0; position.y < diameter; position.y += 1.0)                \n"
    "                   {                                                                               \n"
    "                       delta = position - point1_radius;                                           \n"
    "                       tex_coords = (point_texture_size_div_2 + delta) * point_texture_step;       \n"
    "                       expected_color = texture2D(point1_texture, tex_coords);                     \n"
    "                                                                                                   \n"
    "                       tex_coords = (image_pixel_position + delta) * image_texture_step;           \n"
    "                       actual_color = texture2D(image_texture, tex_coords);                        \n"
    "                       difference = actual_color - expected_color;                                 \n"
    "                       checksum += dot(difference, difference);                                    \n"
    "                   }                                                                               \n"
    "               }                                                                                   \n"
    "                                                                                                   \n"
	"				float color = checksum / point1_sensitivity;										\n"
	"               gl_FragColor = vec4(color, color, color, 1.0);                                      \n"
    "           }                                                                                       \n"
    "       }                                                                                           \n"
    "                                                                                                   \n"
    "       if (point2_enabled)                                                                         \n"
    "       {                                                                                           \n"
    "           delta = point2_position - pixel_position;                                               \n"
    "           radius = sqrt( dot(delta, delta) );                                                     \n"
    "                                                                                                   \n"
    "           if (radius < point2_radius)                                                             \n"
    "           {                                                                                       \n"
    "               gl_FragColor = grey;                                                                \n"
    "           }                                                                                       \n"
    "                                                                                                   \n"
    "           if (radius < point2_tracking_radius)                                                    \n"
    "           {                                                                                       \n"
    "               checksum = 0.0;                                                                     \n"
    "               diameter = point2_radius * 2.0;                                                     \n"
    "                                                                                                   \n"
    "               for (position.x = 0.0; position.x < diameter; position.x += 1.0)                    \n"
    "               {                                                                                   \n"
    "                   for (position.y = 0.0; position.y < diameter; position.y += 1.0)                \n"
    "                   {                                                                               \n"
    "                       delta = position - point2_radius;                                           \n"
    "                       tex_coords = (point_texture_size_div_2 + delta) * point_texture_step;       \n"
    "                       expected_color = texture2D(point2_texture, tex_coords);                     \n"
    "                                                                                                   \n"
    "                       tex_coords = (image_pixel_position + delta) * image_texture_step;           \n"
    "                       actual_color = texture2D(image_texture, tex_coords);                        \n"
    "                       difference = actual_color - expected_color;                                 \n"
    "                       checksum += dot(difference, difference);                                    \n"
    "                   }                                                                               \n"
    "               }                                                                                   \n"
    "                                                                                                   \n"
	"				float color = checksum / point2_sensitivity;										\n"
	"               gl_FragColor = vec4(color, color, color, 1.0);                                      \n"
    "           }                                                                                       \n"
    "       }                                                                                           \n"
    "                                                                                                   \n"
    "       if (point3_enabled)                                                                         \n"
    "       {                                                                                           \n"
    "           delta = point3_position - pixel_position;                                               \n"
    "           radius = sqrt( dot(delta, delta) );                                                     \n"
    "                                                                                                   \n"
    "           if (radius < point3_radius)                                                             \n"
    "           {                                                                                       \n"
    "               gl_FragColor = yellow;                                                              \n"
    "           }                                                                                       \n"
    "                                                                                                   \n"
    "           if (radius < point3_tracking_radius)                                                    \n"
    "           {                                                                                       \n"
    "               checksum = 0.0;                                                                     \n"
    "               diameter = point3_radius * 2.0;                                                     \n"
    "                                                                                                   \n"
    "               for (position.x = 0.0; position.x < diameter; position.x += 1.0)                    \n"
    "               {                                                                                   \n"
    "                   for (position.y = 0.0; position.y < diameter; position.y += 1.0)                \n"
    "                   {                                                                               \n"
    "                       delta = position - point3_radius;                                           \n"
    "                       tex_coords = (point_texture_size_div_2 + delta) * point_texture_step;       \n"
    "                       expected_color = texture2D(point3_texture, tex_coords);                     \n"
    "                                                                                                   \n"
    "                       tex_coords = (image_pixel_position + delta) * image_texture_step;           \n"
    "                       actual_color = texture2D(image_texture, tex_coords);                        \n"
    "                       difference = actual_color - expected_color;                                 \n"
    "                       checksum += dot(difference, difference);                                    \n"
    "                   }                                                                               \n"
    "               }                                                                                   \n"
    "                                                                                                   \n"
	"				float color = checksum / point3_sensitivity;										\n"
	"               gl_FragColor = vec4(color, color, color, 1.0);                                      \n"
    "           }                                                                                       \n"
    "                                                                                                   \n"
    "       }                                                                                           \n"
    "                                                                                                   \n"
    "       gl_FragColor.w = 1.0;                                                                       \n"
    "   }                                                                                               \n\0"
};

void            
GLWorld::doStabilizeGLSL(JahLayer* image_layer)
{
    GLsizei message_length;
    char message[1000];

    //glClear(GL_COLOR_BUFFER_BIT);
    StabilizingState* stabilizing_state = image_layer->getStabilizingState();

    if ( stabilizing_state == NULL )
    {
        stabilizing_state = new StabilizingState;
        image_layer->setStabilizingState(stabilizing_state);

        // There needs to be a different shader for stabilize and tracking.  We don't want tracking circles appearing
        // in the stabilize effect.
        const char* stabilizing_shader = getTrackerFragmentShader();
        GlslShader* fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, stabilizing_shader);
        stabilizing_state->setFragmentShader(fragment_shader);
        stabilizing_state->m_shader_vector.push_back(fragment_shader);
        
        GlslProgram* shader_program = new GlslProgram(stabilizing_state->m_shader_vector);
        stabilizing_state->setShaderProgram(shader_program);
        GLhandleARB shader_program_handle = *shader_program->getGlHandle();
        stabilizing_state->setShaderProgramHandle(shader_program_handle);

        stabilizing_state->setImageLowerLeftCornerHandle( glGetUniformLocationARB(shader_program_handle, "image_lower_left_corner") );
        stabilizing_state->setImageTextureStepHandle( glGetUniformLocationARB(shader_program_handle, "image_texture_step") );
        stabilizing_state->setPointTextureStepHandle( glGetUniformLocationARB(shader_program_handle, "point_texture_step") );
        stabilizing_state->setImageTextureRatioHandle( glGetUniformLocationARB(shader_program_handle, "image_texture_ratio") );
        stabilizing_state->setPointTextureRatioHandle( glGetUniformLocationARB(shader_program_handle, "point_texture_ratio") );
        stabilizing_state->setPointTextureSizeHandle( glGetUniformLocationARB(shader_program_handle, "point_texture_size") );
        
        for (size_t i = 0; i < MAX_STABILIZING_POINTS; i++)
        {
            std::ostringstream declaration;
            declaration << "point" << i << "_radius";
            std::string declaration_string = declaration.str();
            char* declaration_char_string = (char*)declaration_string.c_str();
 
            stabilizing_state->setPointRadiusHandle(i, 
                    glGetUniformLocationARB( shader_program_handle, (const char*)declaration_char_string ) );

            declaration.str("");
            declaration << "point" << i << "_tracking_radius";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            stabilizing_state->setPointTrackingRadiusHandle(i, 
                    glGetUniformLocationARB( shader_program_handle, (const char*)declaration_char_string ) );

            declaration.str("");
            declaration << "point" << i << "_position";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            stabilizing_state->setPointPositionHandle(i,
                    glGetUniformLocationARB( shader_program_handle, (const char*)declaration_char_string ) );

            declaration.str("");
            declaration << "point" << i << "_enabled";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            stabilizing_state->setPointEnabledHandle(i, 
                    glGetUniformLocationARB( shader_program_handle, (const char*)declaration_char_string ) );

            declaration.str("");
            declaration << "point" << i << "_sensitivity";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            stabilizing_state->setPointSensitivityHandle(i, 
                    glGetUniformLocationARB( shader_program_handle, (const char*)declaration_char_string ) );

            // Create a texture to store the pattern being tracked
            createEmpty2DTexture(stabilizing_state->getPointTextureIdPtr(i), GL_RGBA, 
                                 STABILIZING_POINT_TEXTURE_WIDTH, STABILIZING_POINT_TEXTURE_WIDTH );

        }
        
        glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, image_layer->textureData.getTexId() );
    glEnable(GL_TEXTURE_2D);

    TextureImage* image_texture = &image_layer->textureData;

    float2 image_size;
    image_size.x = image_texture->getImageWidth();
    image_size.y = image_texture->getImageHeight();

    int image_layer_width_div_2 = int(image_size.x / 2.0f);
    int image_layer_height_div_2 = int(image_size.y / 2.0f);

    float2 image_texture_ratio;
    image_layer->getTextureData().getTextureRatios(image_texture_ratio);

    float2 image_texture_step = image_texture_ratio / image_size;

    float2 point_texture_ratio;
    // Point textures must be power of 2 squares
    point_texture_ratio.x = 1.0;
    point_texture_ratio.y = 1.0;

    float2 point_texture_size;
    point_texture_size.x = float( STABILIZING_POINT_TEXTURE_WIDTH );
    point_texture_size.y = float( STABILIZING_POINT_TEXTURE_WIDTH );

    float2 point_texture_step = point_texture_ratio / point_texture_size;

    float2 screen_size;
    screen_size.x = core->getRenderWidth();
    screen_size.y = core->getRenderHeight();

    float2 image_lower_left_corner = screen_size - image_size;
    image_lower_left_corner = image_lower_left_corner / 2.0f;
 
    bool point_enabled[MAX_STABILIZING_POINTS];
    JahLayer* point_layer[MAX_STABILIZING_POINTS];

    size_t i;

    glUseProgramObjectARB(m_shader_program_handle);

    m_shader_program->setTextureHandle("image_texture", 0);
    glUniform2fARB(stabilizing_state->getImageLowerLeftCornerHandle(), image_lower_left_corner.x, image_lower_left_corner.y);
    glUniform2fARB(stabilizing_state->getPointTextureStepHandle(), point_texture_step.x, point_texture_step.y);
    glUniform2fARB(stabilizing_state->getImageTextureStepHandle(), image_texture_step.x, image_texture_step.y);
    glUniform2fARB(stabilizing_state->getImageTextureRatioHandle(), image_texture_ratio.x, image_texture_ratio.y);
    glUniform2fARB(stabilizing_state->getPointTextureRatioHandle(), point_texture_ratio.x, point_texture_ratio.y);
    glUniform2fARB(stabilizing_state->getPointTextureSizeHandle(), point_texture_size.x, point_texture_size.y); 

    for (i = 0; i < MAX_STABILIZING_POINTS; i++)
    {
        GLuint texture_id = stabilizing_state->getPointTextureId(i) ;

        glActiveTextureARB(GL_TEXTURE1_ARB + i);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glEnable(GL_TEXTURE_2D);

        point_layer[i] = stabilizing_state->getPointLayer(i);

        glUniform2fARB( stabilizing_state->getPointScreenPositionHandle(i), stabilizing_state->getPointScreenPosition(i).x, 
                        stabilizing_state->getPointScreenPosition(i).y );
        glUniform1fARB( stabilizing_state->getPointRadiusHandle(i), stabilizing_state->getPointRadius(i) );
        glUniform1fARB( stabilizing_state->getPointTrackingRadiusHandle(i), stabilizing_state->getPointSearchRadius(i) );
        glUniform1fARB( stabilizing_state->getPointSensitivityHandle(i), stabilizing_state->getPointSensitivity(i) ); 

        point_enabled[i] = point_layer[i]->layerStatus;
        glUniform1fARB(stabilizing_state->getPointEnabledHandle(i), point_enabled[i]);

        std::ostringstream point_name;
        point_name << "point" << i << "_texture";
        std::string point_name_string = point_name.str();
        char* point_name_cstring = (char*)( point_name_string.c_str() );
        m_shader_program->setTextureHandle(point_name_cstring, i + 1);
    }

    core->bindPbuffer();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    {
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f); 
        glVertex3f(-image_layer_width_div_2, -image_layer_height_div_2, 0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, image_texture_ratio.x, 0.0f);           
        glVertex3f(image_layer_width_div_2, -image_layer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, image_texture_ratio.x, image_texture_ratio.y);           
        glVertex3f(image_layer_width_div_2, image_layer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, image_texture_ratio.y);           
        glVertex3f(-image_layer_width_div_2, image_layer_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    core->unbindPbuffer();

    glUseProgramObjectARB(0);

    for (i = 0; i < MAX_STABILIZING_POINTS; i++)
    {
        glActiveTextureARB(GL_TEXTURE1_ARB + i);
        glDisable(GL_TEXTURE_2D);
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
//#endif
}


void
GLWorld::doStretchCrop(JahLayer* image_layer)
{
    int image_width = image_layer->getTextureData().getImageWidth();
    int image_height = image_layer->getTextureData().getImageHeight();

	motionNode* sliders = getCurrentEffectLayer()->layernodes->m_node;

	int left = int( sliders->rx );
	int right = int( sliders->ry );
	int bottom = int( sliders->rz );
	int top = int( sliders->tx );
	float horizontal_offset = float(sliders->ty) / float(image_width);
	float vertical_offset = float(sliders->tz) / float(image_height);
	float stretch = ( float( (sliders->sx - 1.0f) * 100.0f) / float(image_width) ) + 1.0f;
	float zoom = ( float( (sliders->sy - 1.0f) * 100.0f) / float(image_width) );

	float2 lower_left_texture_coords;
	float2 upper_right_texture_coords;

	float2 texture_ratio;
	texture_ratio.x = image_layer->textureData.texwidthratio;
	texture_ratio.y = image_layer->textureData.texheightratio;

	float2 lower_left_vertex_coordinates;
	float2 upper_right_vertex_coordinates;

	lower_left_vertex_coordinates.x = float(image_width) / 2.0f;
	lower_left_vertex_coordinates.y = float(image_height) / 2.0f;
	upper_right_vertex_coordinates = lower_left_vertex_coordinates;

	float2 lower_left_displacement;
	lower_left_displacement.x = float(left) / float(image_width);
	lower_left_displacement.y = float(bottom) / float(image_height);
	lower_left_displacement *= stretch;

	lower_left_texture_coords = lower_left_displacement;
	lower_left_texture_coords.x += horizontal_offset;
	lower_left_texture_coords.y += vertical_offset;
	lower_left_texture_coords *= texture_ratio;
	lower_left_texture_coords += zoom;

	float2 upper_right_displacement;
	upper_right_displacement.x = float(right) / float(image_width);
	upper_right_displacement.y = float(top) / float(image_height);
	upper_right_displacement *= stretch;

	upper_right_texture_coords.x = 1.0f - (upper_right_displacement.x);
	upper_right_texture_coords.y = 1.0f - (upper_right_displacement.y);
	upper_right_texture_coords.x += horizontal_offset;
	upper_right_texture_coords.y += vertical_offset;
	upper_right_texture_coords *= texture_ratio;
	upper_right_texture_coords -= zoom;

	image_layer->bindBaseOrCompositeTexture();
    glEnable(GL_TEXTURE_2D);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glBegin(GL_QUADS);	
	{
		glTexCoord2f(lower_left_texture_coords.x, lower_left_texture_coords.y);           
		glVertex3f(-lower_left_vertex_coordinates.x, -lower_left_vertex_coordinates.y,  0.0f);

		glTexCoord2f(upper_right_texture_coords.x,  lower_left_texture_coords.y);   
		glVertex3f( upper_right_vertex_coordinates.x, -lower_left_vertex_coordinates.y,  0.0f);

		glTexCoord2f(upper_right_texture_coords.x,  upper_right_texture_coords.y);                    
		glVertex3f( upper_right_vertex_coordinates.x,  upper_right_vertex_coordinates.y,  0.0f);

		glTexCoord2f(lower_left_texture_coords.x, upper_right_texture_coords.y);  
		glVertex3f(-lower_left_vertex_coordinates.x,  upper_right_vertex_coordinates.y,  0.0f);
	}
	glEnd();

    glBindTexture(GL_TEXTURE_2D, image_layer->getTextureComposite().texID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (core->getRenderWidth() - image_width) / 2,
        (core->getRenderHeight() - image_height) / 2, image_width, image_height);

    if (Globals::getGlVendor().find("ATI")!= std::string::npos)
    {
        // Workaround for ATI driver bug
        image_layer->bindBaseOrCompositeTexture();
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }

}

