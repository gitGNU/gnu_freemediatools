/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <stdio.h>

#include "effect.h"
#include "pbuffer.h"
#include <timer.h>
#include <jahrender.h>


void GLEffect::paintGL()
{
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
    
    drawAllLayers(0, getRenderOnlyToSelectedEffect() );

    if (m_pTextSafe->layerStatus) 
    {  
        m_pRenderSpace->drawLayer(m_pTextSafe, true);  
    }
    
    m_bSlidersHaveChanged = false;

    glPopMatrix();
}
