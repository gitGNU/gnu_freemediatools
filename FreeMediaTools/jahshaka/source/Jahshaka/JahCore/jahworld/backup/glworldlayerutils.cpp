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
//used to parse a selection list
//need to pass over a QPtrList or return one with multiple copies of the calling data

// This function is deprecated and should be removed from the code FIXME
bool GLWorld::getSelectionTree(QListView* listv, int &layer, int &effect, bool &status)
//void GLAnime::drawSelectionTree(  )
{
    //create the iterator
    QListViewItemIterator it( listv );
    
    //clear out activelayers
    activeLayers->clearList();
    
    /////////////////////////////////////////////////
    //built in debugging variable
    //turn this on to see tree structure
    
    bool isSelected         = false;
    bool isSelectedLayer    = false;
    bool isSelectedEffect   = false;
    bool selectedStatus     = false;
    
    int  selectedLayer = 0;
    int  selectedEffect = 0;
    
    bool alayer     = false;
    bool aeffect    = false;
    bool ison = false;
    
    int layercounter    = 0;
    int fxcounter       = 0;
    
    QString isOn;
    QString isSelec;
    
    //list is for multiple selections
    QPtrList<selectionList> thelist;
    
    int           selectioncounter= 0;
    
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
        //Just checks to see if its selected...
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
        //record the selection
        if (isSelected)
        {
            selectioncounter +=1;
            
            //only do this for effects...
            if(!aeffect)
            {
                thelist.append( new selectionList(layercounter-1, ison) );
            }
            
            selectedLayer  = layercounter;
            selectedEffect = fxcounter;
            
            selectedStatus = ison;
            
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
        
        layer  = selectedLayer - 1;
        effect = -1;
        status = selectedStatus;
    }
    
    if (isSelectedEffect)
    {
        
        layer  = selectedLayer-1;
        effect = selectedEffect-1;
        status = selectedStatus;
    }
    
    
    ////////////////////////////////////////////
    // the list contains all selections and their status
    //need to merge in with activeLayers
    
    //this is the number of items in the listview
    if (isSelectedLayer && !isSelectedEffect)
    {
        
		activeLayers->setTotalLayers( getLayerList()->count() -  1);
        
        for (uint i=0; i < uint(selectioncounter); i++)
        {
            int ly = thelist.at(i)->layer;
            bool st = thelist.at(i)->status;
            
            QString thebool;
            if (st) thebool = " on"; else thebool = " off";
            
            
            //need to iterated selected layers and add them all here
            activeLayers->addLayer(ly);
            
        }
        
    }
    
    
    /////////////////////////////////////////////////////
    // defaults to true, but we can test for errors later
    
    return true;
}


////////////////////////////////////////////
//adds the current layer to the listbox and selects it

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
    //clear all current selections
    m_layer_listview->clearSelection();
    
    //create the new listview item, currently all are checkbox style
    QCheckListItem* new_q_check_list_item = NULL;
    
    //by default we add items at the top of the stack
    //this lets us add them at the bototm of the stack
    QListViewItem* after = NULL;
    if (headtail)
    {
	after = m_layer_listview->lastItem();
    }

    new_q_check_list_item = factory( parent, 
				     after, 
				     getLayerList()->last()->thelayer->layername );

    new_q_check_list_item->setExpandable(true);
    
    //set the layer to visible, need to 'check' the checkbox
    new_q_check_list_item->setOn(status);
    
    m_layer_listview->setCurrentItem(new_q_check_list_item);
    m_layer_listview->setSelected(new_q_check_list_item, status);
    
    //update layer selection mode
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
GLWorld::addEffectToUserInterface(QListViewItem* parent, QString& name, bool checked, bool selected)
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
    
    // selected needs to be off when loading scene files.  Otherwise the first effect
    // loaded becomes the tree head FIXME
    if (selected)
    {
        m_layer_listview->setCurrentItem(new_item);
        m_layer_listview->setSelected(new_item, true);
        new_item->setSelected(true);
    }
    else
    {
        m_layer_listview->setCurrentItem(parent);
        m_layer_listview->setSelected(parent, true );
    }

    return new_item;
}


////////////////////////////////////////////
//adds the current effect to a layer in the listbox and selects it

//headtail is weather we append or prepend the new item
//default is to prepend, you have to specify if you want to append it on the bottom
void 
GLWorld::addSelectionFx(bool status, QString fxname, bool, bool set_selected)
{
//#ifndef JAHPLAYER
    
    //first we need to find the parent so we can add the child in the right place
    QListViewItemIterator it( m_layer_listview );
    
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
        clearSelection(m_layer_listview);
        
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
        
        // set_selected needs to be off when loading scene files.  Otherwise the first effect
        // loaded becomes the tree head
        if (set_selected)
        {
            m_layer_listview->setCurrentItem (rasta);
            m_layer_listview->setSelected ( rasta, status );
        }
        else
        {
            m_layer_listview->setCurrentItem (tree_head);
            m_layer_listview->setSelected ( tree_head, true );
        }
    }
    
//#endif
    
}

//this is a hard clear used to clear multiselects
void GLWorld::clearSelection(QListView *them_layer_listview) 
{
    QListViewItemIterator it( them_layer_listview );
    
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
    if (getActiveJahLayer() == camera)
    {
        return;
    }

    QListViewItem* current_list_view_item = m_layer_listview->currentItem();
    
    //nothing selected
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

        m_layer_listview->setCurrentItem(current_list_view_item);

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
    if (getActiveJahLayer() == camera)
    {
        return;
    }

    QListViewItem* current_list_view_item = m_layer_listview->currentItem();
    
    //nothing selected
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

        m_layer_listview->setCurrentItem(current_list_view_item);

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

    if (ModuleName == "Effects" && jah_layer->objtype != ObjectCategory::EFFECT)
    {
        return;
    }

    
    if (!current_list_view_item)  
    {
        return;
    }

    if ( getActiveJahLayer() )
    {
        QListViewItem* currently_selected_list_view_item = getActiveJahLayer()->getCheckListItem();

        if (currently_selected_list_view_item)
        {
            currently_selected_list_view_item->setSelected(false);
        }

        if ( getActiveJahLayer() == camera )
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
                    if (hasnodeframer)
                    {
                        int layer_number = getActiveLayerNumber() - 1;
						thenodeframer->delNodes(layer_number); // node numbers different by 1.
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
    if (ModuleName == "Animation" || ModuleName == "Text")
    {
        if (list_view_item_below)
        {
            layerClicked(list_view_item_below);
        }
        else
        {
            layerClicked( camera->getCheckListItem() );
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
                    objectControlStack->raiseWidget(2); 
                    break; 
                }
                case ObjectCategory::JAHLAYER : 
                { 
                    objectControlStack->raiseWidget(0); 
                    break; 
                }
                default : 
                { 
                    objectControlStack->raiseWidget(0); 
                    break; 
                }
            }

            break; 
        }
        case LayerCategory::PARTICLE :     
        {   
            objectControlStack->raiseWidget(3); 
            break; 
        }	
        case LayerCategory::OBJECT :     
        {   
            objectControlStack->raiseWidget(4); 
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
                objectControlStack->raiseWidget(1); 
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
                objectControlStack->raiseWidget(5); 
            }
                                               
            break; 
        }
        default :     
        {    
            jtrace->error ("unknown layer type, unable to select menus"); 
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
    
    if (getActiveJahLayer() != camera)// thelayer=0;
    {
        getActiveJahLayer()->layernodes->m_node->rx += (GLfloat)(a/sensitivty);
        getActiveJahLayer()->layernodes->m_node->ry += (GLfloat)(b/sensitivty);
        getActiveJahLayer()->layernodes->m_node->tx += (GLfloat)(c/sensitivty);
        
        getActiveJahLayer()->layernodes->m_node->tx += (GLfloat)(x/sensitivty);
        getActiveJahLayer()->layernodes->m_node->ty += (GLfloat)(y/sensitivty);
        getActiveJahLayer()->layernodes->m_node->tz += (GLfloat)(-z/sensitivty);
        
        updateGL(); 
    }
    else if (getActiveJahLayer() == camera)
    {
        camera->layernodes->m_node->rx += (GLfloat)(a/sensitivty);
        camera->layernodes->m_node->ry += (GLfloat)(b/sensitivty);
        camera->layernodes->m_node->tx += (GLfloat)(c/sensitivty);
        
        camera->layernodes->m_node->tx += (GLfloat)(x/sensitivty);
        camera->layernodes->m_node->ty += (GLfloat)(y/sensitivty);
        camera->layernodes->m_node->tz += (GLfloat)(-z/sensitivty);  
        
        updateGL();       
    }
}





////////////////////////////////////////////////
// 3d object loaders

//////////////////////////////////////////////////////////////
// popup object loaders

//need to move the load routines out of object code into core
//can pass the object over...

void GLWorld::loadObjObject(void) 
{
	
    //get a instance of the openobjectlib
    //so we can poll the extensions
    OpenObjectLib* oolib = OpenObjectLib::getInstance();
    QString formats = oolib->getFormats().data();
    
    QString     objfilename,objextension;

	static QString last_load_filename = JahMediaPath + "/models";
    
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
// routine thats called when a layer in the listview is selected

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

    // Tell the node framer to select our node
    #ifndef JAHPLAYER
    #ifndef NEWWIREUPWIDGET
    if (hasnodeframer)
    {
        thenodeframer->selectNode( getActiveLayerNumber() - 1 ); // node numbers different by 1.
    }
    #endif
    #endif

    if (jah_layer->objtype == ObjectCategory::EFFECT)
    {
        jah_layer->getParent()->setEffectsSlidersHaveChanged(true);

        EffectLayer* effect_layer = jah_layer->getEffectLayer();
        EffectInfo::EFFECT_TYPE effect_type = effect_layer->getType();

        JahSliders[9]->setMaxValue(500);
        JahSlidersLCD[9]->setMaxInt(500);
        JahSliders[9]->setMinValue(1);
        JahSlidersLCD[9]->setMinInt(1);

        if (effect_type < EffectInfo::EFFECT_TYPE_END)
        {
            applySliderNames(effect_layer);
        }
    }
    else
    {
        setDefaultAxisSliderLabels();
        JahSliders[9]->setMaxValue(100);
        JahSlidersLCD[9]->setMaxInt(100);
        JahSliders[9]->setMinValue(0);
        JahSlidersLCD[9]->setMinInt(0);
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
		textEd->setText(jah_layer->text->text);
        textEd->show();
	}
    else if (textEd != NULL)
    {
        textEd->hide();
    }

    item->setSelected(true);
    m_layer_listview->setCurrentItem(item);
}

/////////////////////////////////////////////////////////////////
// routine thats called when an effect node in the jahnodeeditor (canvasview) is selected

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

            // If list view item already selected we dont want to trigger more updates, we can return
            if ( getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem()->isSelected() )
                return;
         
        // Make sure no other list items are left selected
            QListView *listview = getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem()->listView();
            QListViewItemIterator it( listview );
            while ( it.current() ) {
                QListViewItem *item = it.current();
                item->setSelected( false );
                ++it;
            }

            // select our item
            QListViewItem* ouritem = getLayerList()->at(layernumber)->getJahLayer()->getCheckListItem();
            ouritem->setSelected( true );
            ouritem->listView()->setCurrentItem( ouritem ); // needs to be *current* and selected.
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
    QCheckListItem* new_q_check_list_item = addCheckListItem(m_layer_listview, layer_status);
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

