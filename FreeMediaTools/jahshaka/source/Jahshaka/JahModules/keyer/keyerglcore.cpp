/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyer.h"
#include <glworldlists.h>
#include <glcore.h>
#include <timer.h>
#include <jahrender.h>

void GLKeyer::paintGL()
{
    if (m_pChromaSettingsLayer == NULL || m_pChromaKeyLayer == NULL || m_pChromaBaseLayer == NULL)
    {
        return;
    }

    if ( getAnimating() )
    {  
        updateAnimation( getCurrentFrame() );
        incrementCurrentFrame(1);
        
        if ( getCurrentFrame() > m_nMaxFrame )
        {
            setCurrentFrame(m_nMinFrame);
        }
    }
        
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    if (!m_bSelect)
    { 
        if ( isLayerVisible( getJahLayer(0) ) )
        {
            m_pRenderSpace->drawLayer(getLayerList()->at(0)->thelayer, true);
        }
        else
        {
            m_pRenderSpace->drawLayer(m_pTitleSafe, true);
        }
    }
        
    //draw the grid if its visible
    if (m_bGridVal) 
    {  
        m_pRenderSpace->drawLayer(m_pGrid, true); 
    }
    
    int buffer_width = m_pChromaBaseLayer->getXResolution();
    int buffer_height = m_pChromaBaseLayer->getYResolution();

    glGetFloatv(GL_COLOR_CLEAR_VALUE, &m_f4ClearColor.x); 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    int key_texture_width;
    int key_texture_height;
    
    getPowerOfTwoTextureSize(key_texture_width, key_texture_height, buffer_width, buffer_height);
    
    float key_texture_width_ratio = float(buffer_width) / float(key_texture_width);
    float key_texture_height_ratio = float(buffer_height) / float(key_texture_height);

    getCompositeSizeRatioPtr()->x = key_texture_width_ratio;
    getCompositeSizeRatioPtr()->y = key_texture_height_ratio;

    if (!m_bTexturesInitialized)
    {
        createEmpty2DTexture(&m_hBaseTextureID, GL_RGBA, key_texture_width, key_texture_height);
        createEmpty2DTexture(&m_hKeyTextureID, GL_RGBA, key_texture_width, key_texture_height);
        m_bTexturesInitialized = true;
    }

    motionNode* chroma_values = m_pChromaSettingsLayer->layernodes->m_node;

    float red_low = chroma_values->rx;
    float red_high = chroma_values->tx;
    float green_low = chroma_values->ry;
    float green_high = chroma_values->ty;
    float blue_low = chroma_values->rz;
    float blue_high = chroma_values->tz;
    float alpha_low = 0.0f;
    float alpha_high = 0.0f;
    float soften_value = ( (chroma_values->sx - 1.0f) / 5.0f * 255.0f );
    
    m_fRedLow = red_low / 255.0f;
    m_fRedHigh = red_high / 255.0f;
    m_fGreenLow = green_low / 255.0f;
    m_fGreenHigh = green_high / 255.0f;
    m_fBlueLow = blue_low / 255.0f;
    m_fBlueHigh = blue_high / 255.0f;
    m_fSoftenValue = (chroma_values->sx - 1.0f) / 5.0f;
    m_fAlphaLow = 0.0f;
    m_fAlphaHigh = 0.0f;
    
    int lower_left_pixel_offset_x = ( m_pCore->getRenderWidth() - buffer_width ) / 2;
    int lower_left_pixel_offset_y = ( m_pCore->getRenderHeight() - buffer_height ) / 2;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    m_pRenderSpace->drawLayer(m_pTitleSafe, true);
    
    if (m_pChromaBaseLayer->layerStatus)
    {
        m_pChromaBaseLayer->drawTexture();    
    }

    // Capture the current screen to use as the base layer
    glBindTexture(GL_TEXTURE_2D, m_hBaseTextureID);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, lower_left_pixel_offset_x,
        lower_left_pixel_offset_y, buffer_width, buffer_height);

    
    if ( jahstd::glslSupport() && getGpuSelect() && m_pChromaSettingsLayer->layerStatus)
    {   
        initializeMeshLayer(m_pChromaKeyLayer);
        m_pCurrentEffectLayer = m_pChromaSettingsLayer;
        glslChromaKeyer(m_pChromaKeyLayer, m_hBaseTextureID, m_hKeyTextureID);
    }
    else if (m_pChromaSettingsLayer->layerStatus)
    {
        int key_texture_width;
        int key_texture_height;
        
        getPowerOfTwoTextureSize(key_texture_width, key_texture_height, buffer_width, buffer_height);
        
        int buffer_width_div_2 = buffer_width / 2;
        int buffer_height_div_2 = buffer_height / 2;        
        
        int color_buffer_size = buffer_width * buffer_height * 4;
        int texture_size = key_texture_width * key_texture_height * 4;

        static int previous_texture_size = 0;
        static unsigned char* base_color_buffer = NULL;
        static unsigned char* key_color_buffer = NULL;

        if (texture_size != previous_texture_size)
        {
            base_color_buffer = new unsigned char[texture_size];
            key_color_buffer = new unsigned char[texture_size];
            previous_texture_size = texture_size;
        }
        
        float key_texture_width_ratio = float(buffer_width) / float(key_texture_width);
        float key_texture_height_ratio = float(buffer_height) / float(key_texture_height);
        
                
        if (m_bUseTexturesForReadback)
        {
            readTextureSubImageUchar(
                m_hKeyTextureID, key_texture_width, key_texture_height,
                lower_left_pixel_offset_x, lower_left_pixel_offset_y,
                base_color_buffer, buffer_width, buffer_height);
        }
        else
        {
            glReadPixels(
                lower_left_pixel_offset_x,
                lower_left_pixel_offset_y,
                buffer_width,
                buffer_height,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                base_color_buffer);
        }
        
        if (m_pChromaKeyLayer->layerStatus)
        {
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            m_pChromaKeyLayer->objtype = ObjectCategory::JAHLAYER;
            m_pRenderSpace->drawLayer(m_pTitleSafe, true);  
            m_pRenderSpace->drawLayer(m_pChromaKeyLayer, true);
           
            if (m_bUseTexturesForReadback)
            {
                readTextureSubImageUchar(
                    m_hKeyTextureID, key_texture_width, key_texture_height,
                    lower_left_pixel_offset_x, lower_left_pixel_offset_y,
                    key_color_buffer, buffer_width, buffer_height);
            }
            else
            {
                glReadPixels(
                    lower_left_pixel_offset_x,
                    lower_left_pixel_offset_y,
                    buffer_width,
                    buffer_height,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    key_color_buffer);
            }
            
            GLint clear_color[4];
            glGetIntegerv(GL_COLOR_CLEAR_VALUE, &clear_color[0]); 
            
            for (int i = 0; i < color_buffer_size; i += 4)
            {
                unsigned char* red      = &key_color_buffer[i];
                unsigned char* green    = &key_color_buffer[i + 1];
                unsigned char* blue     = &key_color_buffer[i + 2];
                unsigned char* alpha    = &key_color_buffer[i + 3];

                if (*blue > *green)
                {
                    if (soften_value >= *blue)
                    {
                        *blue = 0;
                    }
                    else
                    {
                        *blue -= int( soften_value );
                    }

                    if (*blue < *green)
                    {
                        *blue = *green;
                    }
                }
                
                bool clear = *red == clear_color[0] &&
                    *green == clear_color[1] &&
                    *blue == clear_color[2];
                
                // If the key layer pixel falls within the key color range, replace
                // it with the corresponding base image pixel
                if (m_bSelectColorsIndividually && m_pChromaSettingsLayer->layerStatus)
                {                
                    if (clear || (*red >= red_low && *red <= red_high) )
                    {
                        *red = base_color_buffer[i];
                    }
                    
                    if (clear || (*green != 0 && *green >= green_low && *green <= green_high) )
                    {
                        *green = base_color_buffer[i + 1];
                    }
                    
                    if (clear || (*blue != 0 && *blue >= blue_low && *blue <= blue_high) )
                    {
                        *blue = base_color_buffer[i + 2];
                    }
                    
                    if (clear || (*alpha != 0 && *alpha >= alpha_low && *alpha <= alpha_high) )
                    {
                        *alpha = base_color_buffer[i + 3];
                    }
                }
                else if (m_pChromaSettingsLayer->layerStatus)
                {
                    if (   
                        ( (*red >= red_low && *red <= red_high) &&
                        (*green >= green_low && *green <= green_high) &&
                        (*blue >= blue_low && *blue <= blue_high) ) || clear
                        )
                    {
                        *((unsigned int*)&key_color_buffer[i]) = 
                            *((unsigned int*)&base_color_buffer[i]);
                    }
                    else if ( m_pChromaSettingsLayer->getCreateChromaAlphaMask() )
                    {
                        unsigned int white = 0xffffffff;
                        *((unsigned int*)&key_color_buffer[i]) = white;
                    }
                }
            }
            
            glBindTexture(GL_TEXTURE_2D, m_hKeyTextureID); 
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer_width, buffer_height,
                GL_RGBA, GL_UNSIGNED_BYTE, key_color_buffer);
            
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            
            glBegin(GL_QUADS);
            {
                glTexCoord2f(0.0f, 0.0f);           
                glVertex3f(-buffer_width_div_2, -buffer_height_div_2, 0.0f);
                
                glTexCoord2f(key_texture_width_ratio, 0.0f);           
                glVertex3f(buffer_width_div_2, -buffer_height_div_2,  0.0f);
                
                glTexCoord2f(key_texture_width_ratio, key_texture_height_ratio);           
                glVertex3f(buffer_width_div_2, buffer_height_div_2,  0.0f);
                
                glTexCoord2f(0.0f, key_texture_height_ratio);           
                glVertex3f(-buffer_width_div_2, buffer_height_div_2,  0.0f);
            }
            glEnd(); // GL_QUADS
        }
    }
    else if (m_pChromaKeyLayer->layerStatus)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        m_pChromaKeyLayer->objtype = ObjectCategory::JAHLAYER;
        m_pRenderSpace->drawLayer(m_pTitleSafe, true);  
        m_pRenderSpace->drawLayer(m_pChromaKeyLayer, true);
    }

    
    m_pRenderSpace->makeLayerOutline();
    glClearColor(m_f4ClearColor.x, m_f4ClearColor.y, m_f4ClearColor.z, m_f4ClearColor.w);

    glPopMatrix();
    
    glFlush();
    
    if ( getAnimating() ) 
    {
        startSingleShotTimer();  
    }
    
    setSlidersHaveChanged(false);
}


void GLKeyer::setGpuSelect(void)
{
    
    m_bGPUSelect = m_use_gpu_qcheckbox->isChecked();
    updateGL();
}







