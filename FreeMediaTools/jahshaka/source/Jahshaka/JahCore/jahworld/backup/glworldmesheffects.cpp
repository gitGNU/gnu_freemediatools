/*******************************************************************************
**
** The source file for the Jahshaka effects objects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <jahpluginlib.h>

#define PI                   3.14159265358979323846
#define TWO_PI               (PI * 2.0)

/////////////////////////////////////////////////////////////////
// mesh based effects and opengl based effects

void 
GLWorld::chooseNewMeshEffect(LayerListEntry* parent, QWidget* thebutton, int x_offset, int y_offset)
{
    //add on demand layers...
    QPopupMenu *menu = new QPopupMenu( 0 );
   
    int total_effect_count = getNumberEmbeddedMeshEffects();
    int menu_item_number = 0;
    int first_effect_index = EffectInfo::MESH_TYPE_START + 1;

    for (int i = first_effect_index; i < EffectInfo::GPU_TYPE_START; i++)
    {
        EffectInfo* effect = m_mesh_effects_selection_vector[i];
        EffectInfo::EFFECT_TYPE effect_type = effect->getType();
        
        menu->insertItem( QString( (*effect->getName(effect_type)).data() ), menu_item_number, 0 );
        menu_item_number++;
    }
    
    // Add the plugins to the menu
    if (jplugin->vfxPluginsCounter > 0)
    {
        total_effect_count = addPluginsToMenu(JAH_MESH_PLUGIN, total_effect_count, menu);
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
	addMeshEffectLayer(effect_id, parent->getJahLayer(), menu->text(effect_id).data() );   
    delete menu;  
}

void                
GLWorld::addMeshEffectLayer(int effect_id, JahLayer* parent_jah_layer, const char* effect_name)
{
    //This QString should be a pointer, plugins need to be fixed FIXME
    QString title;
    EffectInfo::EFFECT_TYPE effect_type = EffectInfo::NOT_A_TYPE;
    int plugin_id = -1;
    int first_effect_index = EffectInfo::MESH_TYPE_START + 1;
    std::string guid = "";
    bool added = TRUE;

    if ( effect_id != -1 && parent_jah_layer->layerAssetStatus )
    {
        int vector_index = effect_id + first_effect_index;

        if ( vector_index < EffectInfo::GPU_TYPE_START )
        {
            effect_type = m_mesh_effects_selection_vector[vector_index]->getType();
            title = QString( (*m_mesh_effects_selection_vector[vector_index]->getName(effect_type) ).data() );
        }
        else
        {
            QString thename = effect_name;
            plugin_id = jplugin->getPluginId(thename, JAH_MESH_PLUGIN);
            
            if (plugin_id >= 0)
            {
                title = jplugin->vfxPlugins->at(plugin_id)->getPluginName();
                effect_type = EffectInfo::JAH_PLUGIN_TYPE;
                guid = std::string( jplugin->vfxPlugins->at(plugin_id)->getGuid().data() );
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

	m_sliders_have_changed = true;
}

void GLWorld::doSwirlRt(JahLayer* base_layer)
{

    GLfloat                 xx, yy, radius, original_angle, twisted_angle;
    GLfloat                 lensX, lensY;
    GLfloat                 delta_xx, delta_yy;
    GLfloat                 new_xx, new_yy;

    GLfloat                 TwistEffect = 2.4f;                // The amount of Twist
    GLfloat                 LensRadius;                       // Radius of the lens
 
    lensX = getCurrentEffectLayer()->layernodes->m_node->rx;
    lensY = getCurrentEffectLayer()->layernodes->m_node->ry; 
    LensRadius = (getCurrentEffectLayer()->layernodes->m_node->rz * 3.0) + 100.0;    
    TwistEffect = (getCurrentEffectLayer()->layernodes->m_node->tx / 10.0) + 1.0;

    for (int i = 0; i <= base_layer->JAHMESH_X_DIMENSION; i++) 
    {
        for (int j = 0; j <= base_layer->JAHMESH_Y_DIMENSION; j++) 
        {
            //layers[1].getMeshCoord(i, j, xx, yy, z_offset);
            base_layer->getMeshCoord(i, j, xx, yy, z_offset);

            delta_xx = xx - lensX;
            delta_yy = yy - lensY;
            radius = sqrt(((delta_xx) * (delta_xx)) + ((delta_yy) * (delta_yy)));  

            // If we're inside the unit circle
            if (radius < LensRadius) 
            { 
                // Get the radius of the current point    
                original_angle = atan2(-delta_yy, delta_xx);
                twisted_angle = original_angle + ((LensRadius - radius) / 100.0 * TwistEffect); 
                new_xx = lensX + (radius * ( cos(twisted_angle))); 
                new_yy = lensY + (radius * ( -sin(twisted_angle)));                  

                 //set x and y coords in mesh
                base_layer->setMeshCoord(i, j, new_xx, new_yy, z_offset);                   
            }
            else
            {
                base_layer->setMeshCoord(i, j, xx, yy, z_offset); 
            }
        }
    }
    
}

void GLWorld::doFlagRt(JahLayer* base_layer) {

    GLfloat                 zero_offset;
    GLfloat                 magnitude;
    GLfloat                 num_waves;

    GLfloat                 radians;

    int                     i, j;
    GLfloat                 xx, yy, zz;

    float                    image_width;
    float                    image_height;



    image_width = getCurrentEffectLayer()->getImageWidth();
    image_height = getCurrentEffectLayer()->getImageHeight();
    
    num_waves = (getCurrentEffectLayer()->layernodes->m_node->rx / 10.0) + 1; 
    zero_offset = getCurrentEffectLayer()->layernodes->m_node->ry / 100.0; 
    magnitude = (getCurrentEffectLayer()->layernodes->m_node->rz / 2.0) + 20.0;
    
    //num_waves =   (layers[effectobj].node->rx / 10.0) + 1; 
    
    for (i = 0; i <= base_layer->JAHMESH_X_DIMENSION; i++)
    {
        radians = (((float)i / (float)base_layer->JAHMESH_X_DIMENSION) - zero_offset) * TWO_PI * num_waves;

        for (j = 0; j <= base_layer->JAHMESH_Y_DIMENSION; j++) 
        {
            base_layer->getMeshCoord(i, j, xx, yy, z_offset);
            zz = z_offset - (magnitude * (float)sin(radians));

            //set x, y and z coords in mesh
            base_layer->setMeshCoord(i, j, xx, yy, zz);
        }
    }

    base_layer->generateMeshNormals();
}


void GLWorld::doRippleRt(JahLayer* base_layer)
{

    GLfloat                 ripple_radius;
    GLfloat                 wave_height;
    GLfloat                 half_wave_width;

    GLfloat                 radians;
    GLfloat                 current_radius;
    GLfloat                 current_radius_offset;

    GLfloat                 inner_limit;
    GLfloat                 outer_limit;

    int                     i, j;
    GLfloat                 xx, yy, zz;

    ripple_radius = (getCurrentEffectLayer()->layernodes->m_node->rx * 2.0) + 100.0; 
    wave_height = (getCurrentEffectLayer()->layernodes->m_node->ry / 2.0) + 10.0;

    half_wave_width = (PI / 2.0) * wave_height;

    inner_limit = ripple_radius - half_wave_width;
    outer_limit = ripple_radius + half_wave_width;

    for (i = 0; i <= base_layer->JAHMESH_X_DIMENSION; i++) 
    {
        for (j = 0; j <= (base_layer->JAHMESH_Y_DIMENSION + 1); j++) 
        {
            base_layer->getMeshCoord(i, j, xx, yy, z_offset);
            //layers[1].getMeshCoord(i, j, xx, yy, z_offset);
            
            current_radius = sqrt((xx * xx) + (yy * yy));

            if ((current_radius < outer_limit) && (current_radius > inner_limit))
            {
                current_radius_offset = current_radius - ripple_radius;

                radians = current_radius_offset / half_wave_width;
                zz = z_offset + ((wave_height * cos(radians)) * 5.0);
            }
            else
            {
                zz = z_offset;
            }

            //set x and y coords in mesh
            base_layer->setMeshCoord(i, j, xx, yy, zz); 
        }
    }

    base_layer->generateMeshNormals();
}

void GLWorld::doFisheyeRt(JahLayer* base_layer) 
{

    GLfloat                  xx, yy, radius, angle;
    GLfloat                  lensX, lensY;
    GLfloat                  delta_xx, delta_yy;

    GLfloat                  LensEffect;  LensEffect  = 0.4f;   
    GLfloat                  LensRadius;                       

    lensX = getCurrentEffectLayer()->layernodes->m_node->rx;
    lensY = getCurrentEffectLayer()->layernodes->m_node->ry; 
 
    LensRadius = (getCurrentEffectLayer()->layernodes->m_node->rz * 3.0) + 80.0;
 
    for (int i = 0; i <= base_layer->JAHMESH_X_DIMENSION; i++) 
    {
        for (int j = 0; j <= (base_layer->JAHMESH_Y_DIMENSION + 2); j++) 
        {
            //layers[1].getMeshCoord(i, j, xx, yy, z_offset);
            base_layer->getMeshCoord(i, j, xx, yy, z_offset);
                        
            delta_xx = xx - lensX;
            delta_yy = yy - lensY;

            radius = sqrt(((delta_xx) * (delta_xx)) + ((delta_yy) * (delta_yy)));  

            // If we're inside the unit circle
            if (radius < LensRadius) 
            { 
                // Get the radius of the current point    
                angle = atan2((yy - lensY),(xx - lensX));                
                radius = LensRadius * exp ( log (radius / LensRadius) * LensEffect ); 
                    
                //set x and y coords in mesh
                base_layer->setMeshCoord(i, j, 
                                                        (lensX + (radius * ( cos(angle))  )), 
                                                        (lensY + (radius * ( sin(angle))  )),
                                                         z_offset);
            }
            else
            {
                base_layer->setMeshCoord(i, j, xx, yy, z_offset);
            }
        }
    }

}


void GLWorld::doColorMatrixRt(JahLayer* base_layer) 
{
    //raise the color matrix
    glMatrixMode(GL_COLOR);
    glLoadIdentity();

    // Transform the luminance colors:
    GLfloat lumx = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->tx+100) / 100;
    GLfloat lumy = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->ty+100) / 100;
    GLfloat lumz = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->tz+100) / 100;

    glScalef(lumx,lumy,lumz);   // Scale r*=0.5, g*=2.0, b*=1.0

    // Transform the brightness colors:
    GLfloat brightx = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->rx) / 100;
    GLfloat brighty = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->ry) / 100;
    GLfloat brightz = ((GLfloat)getCurrentEffectLayer()->layernodes->m_node->rz) / 100;

    glTranslatef(brightx,brighty,brightz);   // Scale r*=0.5, g*=2.0, b*=1.0

    // Transform the saturation colors:
    GLfloat saturation = ((GLfloat)base_layer->layernodes->m_node->sx);
    
    GLfloat matrix[16];

    matrix[0]  = (1.0 - saturation ) * 0.3086 + saturation;
    matrix[1]  = (1.0 - saturation ) * 0.3086 ;
    matrix[2]  = (1.0 - saturation ) * 0.3086 ;
    matrix[3]  = 0.0 ;
    matrix[4]  = (1.0 - saturation ) * 0.6094 ;
    matrix[5]  = (1.0 - saturation ) * 0.6094 + saturation;
    matrix[6]  = (1.0 - saturation ) * 0.6094 ;
    matrix[7]  = 0.0 ;
    matrix[8]  = (1.0 - saturation ) * 0.0820 ;
    matrix[9]  = (1.0 - saturation ) * 0.0820 ;
    matrix[10] = (1.0 - saturation ) * 0.0820 + saturation;
    matrix[12] = 0.0 ;
    matrix[13] = 0.0 ;
    matrix[14] = 0.0 ;
    matrix[15] = 1.0 ;

    glMultMatrixf(matrix);

    glMatrixMode(GL_MODELVIEW);
}


//////////////////////////////////////////////////////////
// none of these functions work yet
// we need to clean these up

void GLWorld::doSharpenRt(JahLayer*)
{
    /*
    //we need to toggle this when ever its turned off...
    //int width = 9;

    //GLfloat convolveMat[3][3] = { -ColorMR, -ColorMR,     -ColorMR,
    //                              -ColorMR, 1+8*ColorMR,  -ColorMR,
    //                              -ColorMB ,-ColorMR,     -ColorMR };

    //glConvolutionFilter2D(GL_CONVOLUTION_2D, GL_LUMINANCE, 3,3, GL_LUMINANCE,
    //                        GL_FLOAT, convolveMat[0]);

    glMatrixMode(GL_COLOR);
    glPushMatrix();

    GLfloat convolve_2d[300][200];
    GLfloat convolve_1d[500];
    glMatrixMode(GL_COLOR);
    glConvolutionFilter2D(GL_CONVOLUTION_2D, GL_LUMINANCE, 300,200, GL_LUMINANCE,
                        GL_FLOAT, convolve_2d[]);

    glConvolutionFilter1D(GL_CONVOLUTION_1D, GL_LUMINANCE, 500, GL_LUMINANCE,
                        GL_FLOAT, convolve_1d);

    //glMatrixMode(GL_COLOR);
    //glPushMatrix();
    glLoadMatrixf(convolveMat);
    */

}

void GLWorld::doContrastRt(JahLayer*)    
{

}


void GLWorld::doBlurRt(JahLayer*) 
{
    /*
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glPushMatrix();
    // glConvolutionFilter2D(GL_CONVOLUTION_2D, GL_LUMINANCE,
    //               3, 3, GL_LUMINANCE, GL_FLOAT, horizontal);
    //glEnable(GL_CONVOLUTION_2D);
    //glDisable(GL_CONVOLUTION_2D);
    glPopMatrix();
    */

}

