/*******************************************************************************
**
** The source file for the Jahshaka glworldcontrols.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include <qwidgetstack.h>
#include <qmultilineedit.h>
#include <qslider.h>
#include "glworldlists.h"
#include "customlistviewitem.h"
#include <qfiledialog.h>
#include <jahdataio.h>
#include <openobjectlib.h>
#include <InputLCD.h>

#ifndef JAHPLAYER
#include "jahnodes.h"
#endif

///////////////////////////////////////////////////////////////////////
//used to parse a m_bSelection list
//need to pass over a QPtrList or return one with multiple copies of the calling data

// This function is deprecated and should be removed from the code FIXME
bool GLWorld::getSelectionTree(QListView* listv, int &layer, int &effect, bool &status)
//void GLAnime::drawSelectionTree(  )
{
    //create the iterator
    QListViewItemIterator it( listv );
    
    //clear out activelayers
    m_pActiveLayers->clearList();
    
    /////////////////////////////////////////////////
    //built in debugging variable
    //turn this on to see tree structure
    
    bool isSelected         = false;
    bool isSelectedLayer    = false;
    bool isSelectedEffect   = false;
    bool m_bSelectedStatus     = false;
    
    int  m_bSelectedLayer = 0;
    int  m_bSelectedEffect = 0;
    
    bool alayer     = false;
    bool aeffect    = false;
    bool ison = false;
    
    int layercounter    = 0;
    int fxcounter       = 0;
    
    QString isOn;
    QString isSelec;
    
    //list is for multiple m_bSelections
    QPtrList<selectionList> thelist;
    
    int           m_bSelectioncounter= 0;
    
    // iterate through the first ListView...
    for ( ; it.current(); ++it )
    {
        alayer = false;
        aeffect = false;
        ison = false;
        
        ///////////////////////////////////////////////////
        // basic stuff first
        if (   ( (QCheckListItem*)it.current())->isOn()   )
        {
            ison = true; isOn = "1 ";
        }
        else
        {
            ison = false; isOn = "0 ";
        }
        
        /////////////////////////////////////////////
        //Just checks to see if its m_bSelected...
        if ( it.current()->isSelected() )
        {
            isSelected = true; isSelec = " <Selected>";
        }
        else
        {
            isSelected = false; isSelec = "";
        }
        
        ///////////////////////////////////////////////////
        //this means its a layer without a child
        if ( !(it.current()->firstChild()) )
        {
            if ( !(it.current()->parent() ))
            {
                
                layercounter += 1;
                alayer = true;
            }
        }
        
        ////////////////////////////////////////////////////
        //this means its a layer and it has effects under it
        if ( it.current()->firstChild() )
        {
            
            layercounter    += 1;
            fxcounter       = 0;
            alayer          = true;
        }
        
        /////////////////////////////////////////////////
        //if this is true it means the item has a parent
        //so its a effect
        if ( it.current()->parent() )
        {
            fxcounter +=1;
            aeffect = true;
        }
        
        /////////////////////////////////////////////////
        //record the m_bSelection
        if (isSelected)
        {
            m_bSelectioncounter +=1;
            
            //only do this for effects...
            if(!aeffect)
            {
                thelist.append( new selectionList(layercounter-1, ison) );
            }
            
            m_bSelectedLayer  = layercounter;
            m_bSelectedEffect = fxcounter;
            
            m_bSelectedStatus = ison;
            
            if (aeffect)
                isSelectedEffect = true;
            
            if (alayer)
                isSelectedLayer = true;
        }
        
    }
    
    ////////////////////////////////////////////////////
    // print out and set up for effect or layer
    
    
    if (isSelectedLayer)
    {
        
        layer  = m_bSelectedLayer - 1;
        effect = -1;
        status = m_bSelectedStatus;
    }
    
    if (isSelectedEffect)
    {
        
        layer  = m_bSelectedLayer-1;
        effect = m_bSelectedEffect-1;
        status = m_bSelectedStatus;
    }
    
    
    ////////////////////////////////////////////
    // the list contains all m_bSelections and their status
    //need to merge in with m_pActiveLayers
    
    //this is the number of items in the listview
    if (isSelectedLayer && !isSelectedEffect)
    {
        
		m_pActiveLayers->setTotalLayers( getLayerList()->count() -  1);
        
        for (uint i=0; i < uint(m_bSelectioncounter); i++)
        {
            int ly = thelist.at(i)->layer;
            bool st = thelist.at(i)->status;
            
            QString thebool;
            if (st) thebool = " on"; else thebool = " off";
            
            
            //need to iterated m_bSelected layers and add them all here
            m_pActiveLayers->addLayer(ly);
            
        }
        
    }
    
    
    /////////////////////////////////////////////////////
    // defaults to true, but we can test for errors later
    
    return true;
}


////////////////////////////////////////////
//adds the current layer to the listbox and m_bSelects it

template < typename T > 
QCheckListItem* 
factory( T* parent, QListViewItem* after, const QString& s )
{
    if ( after )
	return new QCheckListItem( parent, after, s, QCheckListItem::CheckBox );

    return new QCheckListItem( parent, s, QCheckListItem::CheckBox );
}

//headtail is wether we append or prepend the new item
//default is to prepend, you have to specify if you want to append it on the bottom
template <typename T>
QCheckListItem*
GLWorld::addCheckListItem(T* parent, 
			  bool status, 
			  bool headtail, 
			  QCheckListItem* (*factory)(T*, QListViewItem*, const QString& s ))
{
    //clear all current m_bSelections
    m_pLayerListView->clearSelection();
    
    //create the new listview item, currently all are checkbox style
    QCheckListItem* new_q_check_list_item = NULL;
    
    //by default we add items at the top of the stack
    //this lets us add them at the bototm of the stack
    QListViewItem* after = NULL;
    if (headtail)
    {
	after = m_pLayerListView->lastItem();
    }

    new_q_check_list_item = factory( parent, 
				     after, 
				     getLayerList()->last()->thelayer->layername );

    new_q_check_list_item->setExpandable(true);
    
    //set the layer to visible, need to 'check' the checkbox
    new_q_check_list_item->setOn(status);
    
    m_pLayerListView->setCurrentItem(new_q_check_list_item);
    m_pLayerListView->setSelected(new_q_check_list_item, status);
    
    //update layer m_bSelection mode
    changeSelection( getLayerList()->count() - 1 );
    getLayerList()->last()->thelayer->setCheckListItem(new_q_check_list_item);
    
    setLayerVisible(getLayerList()->count() - 1, status);

    return new_q_check_list_item;   
}

template QCheckListItem* 
factory( QListViewItem* parent, QListViewItem* after, const QString& s );
template QCheckListItem* 
factory( QCheckListItem* parent, QListViewItem* after, const QString& s );
template QCheckListItem* 
factory( QListView* parent, QListViewItem* after, const QString& s );

template QCheckListItem*
GLWorld::addCheckListItem< QListViewItem >(QListViewItem* parent, bool status, bool headtail, QCheckListItem* (*fn)(QListViewItem*, QListViewItem*, const QString& ));
template QCheckListItem*
GLWorld::addCheckListItem< QCheckListItem >(QCheckListItem* parent, bool status, bool headtail, QCheckListItem* (*fn)(QCheckListItem*, QListViewItem*, const QString& ));
template QCheckListItem*
GLWorld::addCheckListItem< QListView >(QListView* parent, bool status, bool headtail, QCheckListItem* (*fn)(QListView*, QListViewItem*, const QString& ));

QListViewItem*
GLWorld::addEffectToUserInterface(QListViewItem* parent, QString& name, bool checked, bool m_bSelected)
{
    if (parent == NULL)
    {
        return NULL;
    }

    QListViewItem* current_child = parent->firstChild();
    QListViewItem* last_child = parent;

    while (current_child)
    {
        last_child = current_child;
        current_child = current_child->nextSibling();
    }

    parent->setOpen(true);

    QCheckListItem* new_item = 
        new QCheckListItem(parent, last_child, name, QCheckListItem::CheckBox);

    new_item->setOn(checked);
    
    // m_bSelected needs to be off when loading scene files.  Otherwise the first effect
    // loaded becomes the tree head FIXME
    if (m_bSelected)
    {
        m_pLayerListView->setCurrentItem(new_item);
        m_pLayerListView->setSelected(new_item, true);
        new_item->setSelected(true);
    }
    else
    {
        m_pLayerListView->setCurrentItem(parent);
        m_pLayerListView->setSelected(parent, true );
    }

    return new_item;
}


////////////////////////////////////////////
//adds the current effect to a layer in the listbox and m_bSelects it

//headtail is weather we append or prepend the new item
//default is to prepend, you have to specify if you want to append it on the bottom
void 
GLWorld::addSelectionFx(bool status, QString fxname, bool, bool set_m_bSelected)
{
//#ifndef JAHPLAYER
    
    //first we need to find the parent so we can add the child in the right place
    QListViewItemIterator it( m_pLayerListView );
    
    QListViewItem* item = NULL;
    QListViewItem* lastfx = NULL;
    QListViewItem* tree_head = NULL;
    
    bool gotParent = false;
    
    bool gotfx = false;
    
    for ( ; it.current(); it++)
    {
        if ( it.current()->isSelected() )
        {
            if( !it.current()->parent() )
            {
                gotParent = true;
                
                item = it.current();
                tree_head = item;
                
                int numChildren = item->childCount();
                
                //now we need to find the last child of item
                if (numChildren>0)
                {
                    gotfx = true;
                    lastfx = item->firstChild();
                    
					bool condition = true;

                    do 
                    {
                        if ( !lastfx->nextSibling() )
						{
                            break;
						}
                        
                        else
						{
                            lastfx = lastfx->nextSibling(); 
						}
                        
                        
                    } while (condition);
                }
            }
        }
    }
    
    if (gotParent)
    {
        clearSelection(m_pLayerListView);
        
        //got parnet, now we need to add after the last child of the parent
        item->setOpen(true);
        
        QCheckListItem * rasta;
        
        if (gotfx)
        {
            rasta = new QCheckListItem( item, lastfx, fxname , QCheckListItem::CheckBox );
        }
        else
        {
            rasta = new QCheckListItem( item, fxname , QCheckListItem::CheckBox );
        }
        
        rasta->setOn(status);
        
        // set_m_bSelected needs to be off when loading scene files.  Otherwise the first effect
        // loaded becomes the tree head
        if (set_m_bSelected)
        {
            m_pLayerListView->setCurrentItem (rasta);
            m_pLayerListView->setSelected ( rasta, status );
        }
        else
        {
            m_pLayerListView->setCurrentItem (tree_head);
            m_pLayerListView->setSelected ( tree_head, true );
        }
    }
    
//#endif
    
}

//this is a hard clear used to clear multim_bSelects
void GLWorld::clearSelection(QListView *them_pLayerListView) 
{
    QListViewItemIterator it( them_pLayerListView );
    
    for ( ; it.current(); it++)
    {
        it.current()->setSelected(false);
    }
    
}

void GLWorld::changeSelection(int layer_number) 
{ 
    if ( getActiveJahLayer() )
    {
        getActiveJahLayer()->selected = FALSE;
    }

    LayerListEntry* layer_list_entry = getLayerList()->at(layer_number);
    JahLayer* jah_layer = layer_list_entry->getJahLayer();
    setActiveJahLayer(jah_layer);
    jah_layer->selected=TRUE;
}

void GLWorld::setLayerVisible( int layer, bool status ) 
{
    QString data;
    
    if (getLayerList()->at(layer))
    {
        getLayerList()->at(layer)->thelayer->layerStatus = status;
    }
}


void 
#ifdef ADD_EFFECTS_TO_TOP_OF_STACK
GLWorld::moveLayerUp(void)
#else
GLWorld::moveLayerDown(void)
#endif
{
    //cant delete background
    if (getActiveJahLayer() == m_pCamera)
    {
        return;
    }

    QListViewItem* current_list_view_item = m_pLayerListView->currentItem();
    
    //nothing m_bSelected
    if (current_list_view_item == 0)  
    {
        return;
    }

    if ( activeLayerIsAnEffect() )
    {
        EffectLayer* effect_layer = getActiveEffectLayer();
        JahLayer* jah_layer = getActiveJahLayer();
        JahLayer* parent_jah_layer = jah_layer->getParent();
        QPtrList<EffectLayer>* effect_list = parent_jah_layer->getEffectsList();

        int list_position = effect_list->find(effect_layer);

        if ( list_position < int( effect_list->count() ) )
        {
            // Temporarily remove the item from the list
            effect_list->take();

            if ( list_position == int( effect_list->count() ) )
            {
                effect_list->append(effect_layer);
            }
            else
            {
                effect_list->insert(list_position + 1, effect_layer);
            }
        }

        parent_jah_layer->reorderListViewItems();
        parent_jah_layer->setEffectsSlidersHaveChanged(true);

        m_pLayerListView->setCurrentItem(current_list_view_item);

        updateGL();
    }
    else
    {
        JahLayer* jah_layer = getActiveJahLayer();
        LayerListEntry* layer_list_entry = jah_layer->getLayerListEntry();
        int list_position = getLayerList()->find(layer_list_entry);

        if (list_position > 1)
        {
            getLayerList()->take(list_position);

            list_position--;
            LayerListEntry* previous_layer_list_entry = getLayerList()->at(list_position);

            while ( previous_layer_list_entry && 
                    previous_layer_list_entry->getJahLayer() &&
                    previous_layer_list_entry->getJahLayer()->getCheckListItem() &&
                    previous_layer_list_entry->getJahLayer()->getCheckListItem()->parent() )
            {
                list_position--;
                previous_layer_list_entry = getLayerList()->at(list_position);
            }

            JahLayer* previous_jah_layer = previous_layer_list_entry->getJahLayer();

            if (previous_jah_layer)
            {
                QCheckListItem* previous_check_list_item = previous_jah_layer->getCheckListItem();
                getLayerList()->insert(list_position, layer_list_entry);

                jah_layer->getCheckListItem()->moveItem(previous_check_list_item);
            }
        }
    }
}

void 
#ifdef ADD_EFFECTS_TO_TOP_OF_STACK
GLWorld::moveLayerDown(void)
#else
GLWorld::moveLayerUp(void)
#endif
{
    //cant delete background
    if (getActiveJahLayer() == m_pCamera)
    {
        return;
    }

    QListViewItem* current_list_view_item = m_pLayerListView->currentItem();
    
    //nothing m_bSelected
    if (current_list_view_item == 0)  
    {
        return;
    }

    if ( activeLayerIsAnEffect() )
    {
        EffectLayer* effect_layer = getActiveEffectLayer();
        JahLayer* jah_layer = getActiveJahLayer();
        JahLayer* parent_jah_layer = jah_layer->getParent();
        QPtrList<EffectLayer>* effect_list = parent_jah_layer->getEffectsList();

        int list_position = effect_list->find(effect_layer);

        if ( list_position > 0 )
        {
            // Temporarily remove the item from the list
            effect_list->take();
            effect_list->insert(list_position - 1, effect_layer);
        }

        parent_jah_layer->reorderListViewItems();
        parent_jah_layer->setEffectsSlidersHaveChanged(true);

        m_pLayerListView->setCurrentItem(current_list_view_item);

        updateGL();
    }
    else
    {
        JahLayer* jah_layer = getActiveJahLayer();
        LayerListEntry* layer_list_entry = jah_layer->getLayerListEntry();
        int list_position = getLayerList()->find(layer_list_entry);

        if ( list_position < int( getLayerList()->count() - 1 ) )
        {
            getLayerList()->take(list_position);

            LayerListEntry* next_layer_list_entry = getLayerList()->at(list_position);

            while ( next_layer_list_entry && 
                    next_layer_list_entry->getJahLayer() &&
                    next_layer_list_entry->getJahLayer()->getCheckListItem() &&
                    next_layer_list_entry->getJahLayer()->getCheckListItem()->parent() )
            {
                list_position++;
                next_layer_list_entry = getLayerList()->at(list_position);
            }

            if (!next_layer_list_entry)
            {
                getLayerList()->append(layer_list_entry);
            }
            else
            {
                JahLayer* next_jah_layer = next_layer_list_entry->getJahLayer();

                if (next_jah_layer)
                {
                    QListViewItem* next_check_list_item = next_jah_layer->getCheckListItem();
                    getLayerList()->insert(list_position + 1, layer_list_entry);

                    // In a listview, the item after is the item below, which is the opposite
                    // direction of how the layers are stored in the qptrlist
                    next_check_list_item->moveItem( jah_layer->getCheckListItem() );
                }
            }
        }
    }
}

void  
GLWorld::deleteEffectsList(JahLayer* jah_layer)
{
    QPtrList<EffectLayer>* effects_list = jah_layer->getEffectsList();
    EffectLayer* effect_layer = effects_list->last();

    for ( ; effect_layer; effect_layer = effects_list->current() )
    {
        getLayerList()->remove( effect_layer->getJahLayer()->getLayerListEntry() );
        effects_list->remove(effect_layer);
    }
}

void  
GLWorld::deleteLightsList(JahLayer* jah_layer)
{
    QPtrList<JahLayer>* lights_list = jah_layer->getLightsList();
    JahLayer* light_layer = lights_list->last();

    for ( ; light_layer; light_layer = lights_list->current() )
    {
        getLayerList()->remove( light_layer->getLayerListEntry() );
        lights_list->remove(light_layer);
    }
}


void GLWorld::delLayer(void)
{
    JahLayer* jah_layer = getActiveJahLayer();
    LayerListEntry* active_layer = jah_layer->getLayerListEntry();
    QListViewItem* current_list_view_item = jah_layer->getCheckListItem();
    QListViewItem* list_view_item_below = NULL;

    if (m_qsModuleName == "Effects" && jah_layer->objtype != ObjectCategory::EFFECT)
    {
        return;
    }

    
    if (!current_list_view_item)  
    {
        return;
    }

    if ( getActiveJahLayer() )
    {
        QListViewItem* currently_m_bSelected_list_view_item = getActiveJahLayer()->getCheckListItem();

        if (currently_m_bSelected_list_view_item)
        {
            currently_m_bSelected_list_view_item->setSelected(false);
        }

        if ( getActiveJahLayer() == m_pCamera )
        {
            return;
        }
    }

    list_view_item_below = current_list_view_item->nextSibling();

    if (!list_view_item_below)
    {
        list_view_item_below = current_list_view_item->parent();
    }
   
    delete current_list_view_item;

    if ( jah_layer->getLightsList()->count() )
    {
        deleteLightsList(jah_layer);        
    }

    if ( jah_layer->getEffectsList()->count() )
    {
        deleteEffectsList(jah_layer);
    }

    if (active_layer) 
    {
        JahLayer* parent = jah_layer->getParent();

        if (parent != NULL && jah_layer->objtype == ObjectCategory::EFFECT)
        {            
            // Remove the effect layer from the parent's effect list
            QPtrList<EffectLayer>* effect_list = parent->getEffectsList();
            EffectLayer* effect_layer = jah_layer->getEffectLayer();

            if (effect_layer != NULL)
            {
                if (effect_list->find(effect_layer) >= 0)
                {
                    if (effect_layer->getType() == EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE)
                    {
                        parent->setIsChromaKeyLayer(false);
                    }

                    effect_list->remove(effect_layer);

                    #ifndef JAHPLAYER
                    #ifndef NEWWIREUPWIDGET
                    if (m_bHasNodeFramer)
                    {
                        int layer_number = getActiveLayerNumber() - 1;
						m_pNodeFramer->delNodes(layer_number); // node numbers different by 1.
                    }
                    #endif
                    #endif
                }
            }

            parent->setEffectsSlidersHaveChanged(true);
        }
        else if (parent != NULL && jah_layer->objtype == ObjectCategory::JAHLIGHT)
        {
            if ( parent->getLightsList() )
            {
                parent->getLightsList()->remove(jah_layer);
            }
        }
        else if( jah_layer->objtype == ObjectCategory::OBJECT )
        {
           jah_layer->jahobject->closeObject( );
        }

        getLayerList()->remove(active_layer ); 
        setActiveJahLayer( getFirstJahLayer() );
    }    
    
    //set it to the world
    if (m_qsModuleName == "Animation" || m_qsModuleName == "Text")
    {
        if (list_view_item_below)
        {
            layerClicked(list_view_item_below);
        }
        else
        {
            layerClicked( m_pCamera->getCheckListItem() );
        }
    }
    else
    {
        changeSelection(0);
    }
    //setLayerVisible(0, TRUE); 
    updateGL(); 
}


void GLWorld::setObjectTab(JahLayer* jah_layer)
{           
    LayerCategory::TYPE layertype = jah_layer->layertype;
    ObjectCategory::TYPE object = jah_layer->objtype;

    switch ( layertype ) 
    {
        case LayerCategory::LAYER :     
        {       
            switch ( object ) 
            {
                case ObjectCategory::BACKDROP : 
                { 
                    m_pObjectControlStack->raiseWidget(2); 
                    break; 
                }
                case ObjectCategory::JAHLAYER : 
                { 
                    m_pObjectControlStack->raiseWidget(0); 
                    break; 
                }
                default : 
                { 
                    m_pObjectControlStack->raiseWidget(0); 
                    break; 
                }
            }

            break; 
        }
        case LayerCategory::PARTICLE :     
        {   
            m_pObjectControlStack->raiseWidget(3); 
            break; 
        }	
        case LayerCategory::OBJECT :     
        {   
            m_pObjectControlStack->raiseWidget(4); 
            break; 
        }
        case LayerCategory::FRAME :     
        {   
            break; 
        }
        case LayerCategory::TEXT :     
        {   
#ifndef JAHPLAYER
            if ( object==ObjectCategory::JAHTEXT ) 
            {
                m_pObjectControlStack->raiseWidget(1); 
            }
#endif
            break; 
        }
        case LayerCategory::GRID :     
        {   
            break; 
        }
        case LayerCategory::LIGHT :     
        {   
            if (object == ObjectCategory::JAHLIGHT )     
            { 
                m_pObjectControlStack->raiseWidget(5); 
            }
                                               
            break; 
        }
        default :     
        {    
            m_pJTrace->error ("unknown layer type, unable to m_bSelect menus"); 
            break; 
        }
    }
}





/*
void GLWorld::spaceButtonEvent  ( int button )  {

 //we will use buttons 6/5/4 to lock on x/y/z axes any other button releases
 switch(button) {
 
  
   default   : {  xloxk = true; ylock; }
   
    }
    
     }
*/

void GLWorld::spaceEvent  ( double x, double y, double z, double a, double b, double c )  
{
    //user should be able to tune sensitivity
    int sensitivty = 70;
    
    if (getActiveJahLayer() != m_pCamera)// thelayer=0;
    {
        getActiveJahLayer()->layernodes->m_node->rx += (GLfloat)(a/sensitivty);
        getActiveJahLayer()->layernodes->m_node->ry += (GLfloat)(b/sensitivty);
        getActiveJahLayer()->layernodes->m_node->tx += (GLfloat)(c/sensitivty);
        
        getActiveJahLayer()->layernodes->m_node->tx += (GLfloat)(x/sensitivty);
        getActiveJahLayer()->layernodes->m_node->ty += (GLfloat)(y/sensitivty);
        getActiveJahLayer()->layernodes->m_node->tz += (GLfloat)(-z/sensitivty);
        
        updateGL(); 
    }
    else if (getActiveJahLayer() == m_pCamera)
    {
        m_pCamera->layernodes->m_node->rx += (GLfloat)(a/sensitivty);
        m_pCamera->layernodes->m_node->ry += (GLfloat)(b/sensitivty);
        m_pCamera->layernodes->m_node->tx += (GLfloat)(c/sensitivty);
        
        m_pCamera->layernodes->m_node->tx += (GLfloat)(x/sensitivty);
        m_pCamera->layernodes->m_node->ty += (GLfloat)(y/sensitivty);
        m_pCamera->layernodes->m_node->tz += (GLfloat)(-z/sensitivty);  
        
        updateGL();       
    }
}





////////////////////////////////////////////////
// 3d object loaders

//////////////////////////////////////////////////////////////
// popup object loaders

//need to move the load routines out of object code into m_pCore
//can pass the object over...

void GLWorld::loadObjObject(void) 
{
	
    //get a instance of the openobjectlib
    //so we can poll the extensions
    OpenObjectLib* oolib = OpenObjectLib::getInstance();
    QString formats = oolib->getFormats().data();
    
    QString     objfilename,objextension;

	static QString last_load_filename = m_qsJahMediaPath + "/models";
    
    QString filename = QFileDialog::getOpenFileName(last_load_filename,  formats);
    
    if ( !filename.isEmpty() ) 
    {
        if ( QFile::exists(filename) )
        {
            last_load_filename = filename;
            //do another check to see if it exists here
            
            char l_file_without_extension[JahDataIO::c_max_path];
            JahDataIO::stripExtension ( 
                l_file_without_extension, 
                filename.data() );
            objfilename = l_file_without_extension;
            
            char l_file_extension[JahDataIO::c_max_path];
            JahDataIO::getExtension ( 
                l_file_extension, 
                filename.data() );
            objextension = ".";
            objextension += l_file_extension;
            
			std::string layername_string = filename.data();
			int last_period = (int)layername_string.find_last_of(".");
			int last_slash = (int)layername_string.find_last_of("/", last_period);
			int length = last_period - (last_slash + 1);
			layername_string = layername_string.substr(last_slash + 1, length);
			QString layername = layername_string.data();

			getActiveJahLayer()->layername = layername;
			getActiveJahLayer()->getCheckListItem()->setText(0, layername);
            
            getActiveJahLayer()->objfilename = objfilename;
            getActiveJahLayer()->objextension = objextension;
            
            getActiveJahLayer()->loadObject = true;
            getActiveJahLayer()->objtype = ObjectCategory::OBJECT;
            
            updateGL();
            //getActiveJahLayer()->loadOBJ(objfilename+objextension);  
        }
    }
	
    
}

/////////////////////////////////////////////////////////////////
// routine thats called when a layer in the listview is m_bSelected

void 
GLWorld::layerClicked( QListViewItem* item )
{
    if (!item) 
    {
        return;
    }

    LayerListEntry* layer_list_entry = findLayerListEntry(item);

    if (!layer_list_entry)
    {
        return;
    }

    setActiveJahLayer( layer_list_entry->getJahLayer() );
	JahLayer* jah_layer = layer_list_entry->thelayer;

    QCheckListItem* check_list_item = (QCheckListItem*)getActiveJahLayer()->getCheckListItem();
    getActiveJahLayer()->layernodes->m_node->layer_enabled = check_list_item->isOn();

    // Tell the node framer to m_bSelect our node
    #ifndef JAHPLAYER
    #ifndef NEWWIREUPWIDGET
    if (m_bHasNodeFramer)
    {
        m_pNodeFramer->
	  selectNode( getActiveLayerNumber() - 1 ); // node numbers different by 1.
    }
    #endif
    #endif

    if (jah_layer->objtype == ObjectCategory::EFFECT)
    {
        jah_layer->getParent()->setEffectsSlidersHaveChanged(true);

        EffectLayer* effect_layer = jah_layer->getEffectLayer();
        EffectInfo::EFFECT_TYPE effect_type = effect_layer->getType();

        m_aJahSliders[9]->setMaxValue(500);
        m_aJahSliderLCDs[9]->setMaxInt(500);
        m_aJahSliders[9]->setMinValue(1);
        m_aJahSliderLCDs[9]->setMinInt(1);

        if (effect_type < EffectInfo::EFFECT_TYPE_END)
        {
            applySliderNames(effect_layer);
        }
    }
    else
    {
        setDefaultAxisSliderLabels();
        m_aJahSliders[9]->setMaxValue(100);
        m_aJahSliderLCDs[9]->setMaxInt(100);
        m_aJahSliders[9]->setMinValue(0);
        m_aJahSliderLCDs[9]->setMinInt(0);
    }

    if ( ((QCheckListItem*)item)->isOn() )
    {
        jah_layer->layerStatus = true;

        if (jah_layer->objtype == ObjectCategory::EFFECT)
        {
            EffectInfo::EFFECT_TYPE effect_type = jah_layer->getEffectLayer()->getType();

            if (   effect_type == EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE
                || effect_type == EffectInfo::CHROMA_KEYER_CPU_TYPE )
            {
                jah_layer->getParent()->setIsChromaKeyLayer(true);
            }
        }
    }
    else
    {
        jah_layer->layerStatus = false;

        if (jah_layer->objtype == ObjectCategory::EFFECT)
        {
            EffectInfo::EFFECT_TYPE effect_type = jah_layer->getEffectLayer()->getType();

            if (   effect_type == EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE
                || effect_type == EffectInfo::CHROMA_KEYER_CPU_TYPE )
            {
                jah_layer->getParent()->setIsChromaKeyLayer(false);
            }
        }
    }

    //forceupdatefxbuffer = true;
    setDontTrack(true);
	updateGL();
    setDontTrack(false);

    //clicked on layer, update keyframes
    //should only do this if the layer has changed...
    updateKeyframeDisplay();

    //now update the sliders
    updateSliders();

	if (jah_layer->layertype == LayerCategory::TEXT)
	{
		m_pTextEd->setText(jah_layer->text->text);
        m_pTextEd->show();
	}
    else if (m_pTextEd != NULL)
    {
        m_pTextEd->hide();
    }

    item->setSelected(true);
    m_pLayerListView->setCurrentItem(item);
}

/////////////////////////////////////////////////////////////////
// routine thats called when an effect node in the jahnodeeditor (canvasview) is m_bSelected

void 
GLWorld::effectNodeClicked( int layernumber )
{
    QString conv;
    conv.setNum( layernumber );
    //debug ( "effectnode clicked: " + conv );
    
   if (getLayerList()->at(layernumber))
    {
        conv = getLayerList()->at(layernumber)->name();
        //debug( "layer name is :" + conv ) ;
        conv.setNum( getLayerList()->at(layernumber)->layer() ); // NB possible bug: layer() often not init in layer constr?
        //debug( "layernumber " + conv ) ;

        if ( getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem() && getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem()->listView() )
        {   
		//debug ( "listviewitem ok" );

            // If list view item already m_bSelected we dont want to trigger more updates, we can return
            if ( getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem()->isSelected() )
                return;
         
        // Make sure no other list items are left m_bSelected
            QListView *listview = getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem()->listView();
            QListViewItemIterator it( listview );
            while ( it.current() ) {
                QListViewItem *item = it.current();
                item->setSelected( false );
                ++it;
            }

            // m_bSelect our item
            QListViewItem* ouritem = getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem();
            ouritem->setSelected( true );
            ouritem->listView()->setCurrentItem( ouritem ); // needs to be *current* and m_bSelected.
            ouritem->listView()->triggerUpdate();

            // treat same as when layers list view item clicked
            layerClicked( getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem() );
      
        }
    }
}

int
GLWorld::findLayer(JahLayer* layer)
{
    for (LayerListEntry* current = getLayerList()->first(); current; current = getLayerList()->next() )
    {
        if (current->thelayer == layer)
        {
            return getLayerList()->at();
        }
    }

    return -1;
}

void 
GLWorld::initializeListview(JahLayer* jah_layer, bool layer_status)
{
    QCheckListItem* new_q_check_list_item = addCheckListItem(m_pLayerListView, layer_status);
    jah_layer->setCheckListItem(new_q_check_list_item);
}

void
GLWorld::initializeListview(int layer_number, bool layer_status)
{
    initializeListview( getLayerList()->at(layer_number)->thelayer, layer_status );
}

bool                
GLWorld::multipleLayersSelectedInListView()
{
    LayerListEntry* layer_list_entry = getLayerList()->first();
    int count = 0;

    for(; layer_list_entry; layer_list_entry = getLayerList()->next() )
    {
        QListViewItem* list_view_item = layer_list_entry->getJahLayer()->getCheckListItem();

        if ( list_view_item && list_view_item->isSelected() )
        {
            count++;
        }
    }

    if (count > 1)
    {
        return true;
    }

    return false;
}

