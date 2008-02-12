/*******************************************************************************
**
** The source file for the Jahshaka glworldlayer.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include <dialogs.h>
#include <qlistview.h>

////////////////////////////////////////////////////
// functions thats used bu the ui to set core flags


////////////////////////////////////////
// Toggle and Set routines

void GLWorld::setLayerName(JahLayer* jah_layer, const char* layer_name) 
{  
    QString layer_name_qstring = layer_name;
    jah_layer->getCheckListItem()->setText(0, layer_name_qstring);
}

void GLWorld::nameLayer(void) 
{  
    m_pNamePopUp->popup(
			m_pNameButton, 
			getActiveJahLayer()->getCheckListItem()->text(0));
}

void GLWorld::nameLayerFromTopMenu(void) 
{  
}

void GLWorld::setlayerName(QString name) 
{
    ObjectCategory::TYPE layer_category = getActiveJahLayer()->getCategory();

    if (   layer_category == ObjectCategory::MAINBACK
        || layer_category == ObjectCategory::BACKDROP
        || layer_category == ObjectCategory::EFFECT
       )
    {
        return;
    }

    QListViewItem* current_list_view_item;
    current_list_view_item = m_pLayerListView->currentItem();

    current_list_view_item->setText (0, name);
    m_pLayerListView->repaint(current_list_view_item);

    getActiveJahLayer()->layername = name;
}

void GLWorld::setGrid()   
{ 
    //toggle the grid value
    m_bGridVal = !m_bGridVal; 
    
    updateGL(); 
}

void GLWorld::setSmooth()
{
    //we only toggle this on if the hardware supports it
    if (m_bHWALIASING)
        m_bHWAliasing = !m_bHWAliasing;

    updateGL();
}


void GLWorld::setBlur() 
{   
    m_bUpdateEffect = true;

    getActiveJahLayer()->blur =  !getActiveJahLayer()->blur; 
    
    if (getActiveJahLayer()->negative || getActiveJahLayer()->swaprgb || getActiveJahLayer()->mirror || getActiveJahLayer()->blur ) 
    { 
        getActiveJahLayer()->layerEffect=true;
    }
    else
    { 
        getActiveJahLayer()->layerEffect=false;
    }
    
    updateGL(); 
}    //for motionblur not implemented yet...



void GLWorld::setTextSafe() 
{  
    m_pTextSafe->layerStatus =  !m_pTextSafe->layerStatus; 
    updateGL(); 
}

void GLWorld::toggleForegroundStatus() 
{  
    getActiveJahLayer()->foreground =  
		!getActiveJahLayer()->foreground; 
    updateGL(); 
}

void GLWorld::toggleDepthStatus() 
{  
    getActiveJahLayer()->depth =  
		!getActiveJahLayer()->depth; 
    updateGL(); 
}

void GLWorld::toggleLight(void)   
{ 
    getActiveJahLayer()->lighting = 
		!getActiveJahLayer()->lighting; 
    updateGL(); 
}

void GLWorld::toggleMesh(void)    
{ 
    getActiveJahLayer()->ObjectDisplayMode = 
		!getActiveJahLayer()->ObjectDisplayMode; 
    updateGL(); 
}

void GLWorld::toggleReflect(void) 
{ 
    getActiveJahLayer()->reflect = !getActiveJahLayer()->reflect; 
    updateGL(); 
}

void GLWorld::toggleSmooth(void) 
{ 
    getActiveJahLayer()->smooth = !getActiveJahLayer()->smooth; 
    updateGL(); 
}

void
GLWorld::slotShowLights()
{
    getShowLights();
}

void GLWorld::toggleShader1(void)
{
//    getActiveJahLayer()->pixelshader = !getActiveJahLayer()->pixelshader;
//    getActiveJahLayer()->initVertexShader(1);
    updateGL();

}

void GLWorld::toggleShader2(void)
{
//    getActiveJahLayer()->pixelshader = !getActiveJahLayer()->pixelshader;
//    getActiveJahLayer()->initVertexShader(2);
    updateGL();

}

//used to toggle the on screen display of core statistics via the jahstats object
void GLWorld::toggleStatsDisplay(void)
{
    m_bStatsShowStats = !m_bStatsShowStats;
    updateGL();
}

//used to hide or show extended frames on clips layers
void GLWorld::toggleExtendHeadTail(void) 
{    
    
    bool thestatus;
    
    thestatus = getActiveJahLayer()->showEdges;

    thestatus = !thestatus;
    
    getActiveJahLayer()->showEdges = thestatus;
    
    updateGL();
 
}

//used to toggle playback
void GLWorld::toggleLoop(void) 
{    
    
    bool thestatus;
    
    thestatus = getActiveJahLayer()->loop;

    thestatus = !thestatus;
    
    getActiveJahLayer()->loop = thestatus;
    
    updateGL();
 
}

void GLWorld::togglePingPong(void) 
{    
    
    bool thestatus;
    
    thestatus = getActiveJahLayer()->ping;

    thestatus = !thestatus;
    
    getActiveJahLayer()->ping = thestatus;
    
    updateGL();
 
}

