/*******************************************************************************
**
** The source file for the Jahshaka color module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"
#include <coreobject.h>
#include <jahrender.h>
#include "pbuffer.h"
#include <qtimer.h>

void GLColorize::paintGL()
{
     if ( m_bAnimation )
    {  
        updateAnimation( getCurrentFrame() );
        incrementCurrentFrame(1);
        
        if ( getCurrentFrame() > m_nMaxFrame )
        {
            setCurrentFrame(1);
        }
    }
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    if (!m_bSelect)
    { 
        if ( isLayerVisible( getJahLayer(0) ) )
        {
            m_pRenderSpace->drawLayer( getJahLayer(0), true );
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
    
    JahLayer* base_layer = getJahLayer(1);

    if (base_layer) 
    {
        if ( base_layer->layerStatus )
        {
            buildEffects(base_layer);
            m_pRenderSpace->drawLayer(base_layer, true);
        }
    }

    glPopMatrix();
    
    if (m_pTextSafe->layerStatus) 
    {  
        m_pRenderSpace->drawLayer(m_pTextSafe, true);  
    }
    
    glFlush();
    
    if ( m_bAnimation ) 
    {
        m_pTimer->start( getRedrawDelay(), TRUE );
    }
    
    m_bSlidersHaveChanged = false;
}





