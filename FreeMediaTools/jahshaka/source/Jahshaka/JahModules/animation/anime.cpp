/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "anime.h"
#include "eyeCheckListItem.h"
#include "jahcreate.h"
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qslider.h>
#include "glworldlists.h"
#include <InputLCD.h>
#include <jahfilewindow.h>
#include <assetexchange.h>

void GLAnime::start( )
{
	// We need to ensure plugins are loaded here when in quick start mode
	if ( JahPrefs::getInstance().getQuickStart( ) )
		initializePlugins( );

#ifndef JAHPLAYER
	buildInterface(m_controls);
	buildInterfaceMenus(m_leftcontroller,m_rightcontroller);
#else
	initializePlugins( );
	buildPlayerInterfaceMenus(m_leftcontroller,m_rightcontroller);
#endif

	initializeObjects();

	// Update sliders to reflected m_bSelected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

void GLAnime::initializeObjects()
{
    m_pJTrace->info( "Initializing GLAnimation ModuleAnime");

    //make the current opengl context current
    makeCurrent();

    //if we are doing playback initalize the modes
    //can we do this at the end of this routine?
#ifdef JAHPLAYER
    initializePlayer();
#endif

    ///////////////////////////////////////////////
    //this creates the default layers

    JahLayer* jah_layer = configureLayer ( "World", false, true, false, true );
    buildLayer(jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );
    m_pCamera = jah_layer;
    jah_layer->setCameraLayer(m_pCamera);
    m_pCamera->layerStatus = false;
    m_pCamera->layernodes->m_node->layer_enabled = false;

    setModuleUsesLighting(true);

    //setup/customize listviewitem for layer 0
    initializeListview( 0, FALSE );

    //hook up menu and sliders and default tool
#ifndef JAHPLAYER
    setObjectTab( getLayerList()->last()->getJahLayer() );	//not sure what this does

    updateSliders();							//update out ui

    toolTranslate();							//set the m_bSelection tool

    setForcePlay(true);                         //turn forceplay on here

#else
    noTool();
#endif


#ifndef JAHPLAYER
    //grab a instace of the pluginlib
    //the plyaer init does this for us
    m_pJPlugIn = JahPluginLib::getInstance();
    m_pBufferSelectCheckBox->toggle();
#endif

	QCheckListItem* m_pCamera_q_checklist_item = (QCheckListItem*)m_pCamera->getCheckListItem();
	m_pCamera_q_checklist_item->setOn(false);

	// Update sliders to reflected m_bSelected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

////////////////////////////////////////////////////////
// the layer m_bSelection routine is overiden for
// the animation module
// original routine is located in glworld

void 
GLAnime::layerClicked( QListViewItem* item )
{
    // This needs to be done, because otherwise each changed slider will 
    // cause the scene to be redrawn.  For example, when a layer is clicked.
    setDontPaint(true);

    if (!item) 
    {
        return;
    }

    LayerListEntry* layer_list_entry = findLayerListEntry(item);

    if (!layer_list_entry)
    {
        return;
    }

    //first we need to dem_bSelect the current active layer
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        parent->selected = false;
    }
    else
    {	
        active_layer->selected = false;
    }

    JahLayer* jah_layer = layer_list_entry->thelayer;
    setActiveJahLayer(jah_layer);
    JahLayer* image_layer;
    motionNode* motion_node = getActiveJahLayer()->layernodes->m_node;

    EyeCheckListItem* check_list_item = (EyeCheckListItem*)getActiveJahLayer()->getCheckListItem();
    motion_node->layer_enabled = check_list_item->isOn();

    jah_layer->setIsSelectedVisible( check_list_item->eyeState() );

    //and then we m_bSelect the new layer
    jah_layer->selected = true;

    if (jah_layer->objtype == ObjectCategory::JAHTEXT)
    {
        jah_layer->getCheckListItem()->setText( 0, jah_layer->text->getText() );
    }

    if (jah_layer->objtype == ObjectCategory::EFFECT)
    {
        jah_layer->getParent()->setEffectsSlidersHaveChanged(true);
        image_layer = jah_layer->getParent();
        //hideAllLightingSliders();
    }
    else
    {
        jah_layer->setEffectsSlidersHaveChanged(true);
        image_layer = jah_layer;
        //showAllLightingSliders();
    }

    jah_layer->setCompositeTextureUpdatedByLighting(false);

#ifndef JAHPLAYER
    if ( m_pBufferSelectCheckBox->isOn() != image_layer->getUsePbuffer() )
    {
        m_pBufferSelectCheckBox->toggle();
    }
#endif

    if (jah_layer->objtype == ObjectCategory::EFFECT)
    {
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
    else if ( m_aJahSliders[9] )
    {
        // Save the current value of the node, so that it doesn't get reset to 100
        float current_value = motion_node->Alpha;
		m_aJahSliders[9]->setMaxValue(100);
		m_aJahSliderLCDs[9]->setMaxInt(100);
		m_aJahSliders[9]->setMinValue(0);
		m_aJahSliderLCDs[9]->setMinInt(0);
		m_aJahSliders[9]->setValue(current_value);

    }

    if ( ((QCheckListItem*)item)->isOn() )
    {
        jah_layer->layerStatus = true;

        if (jah_layer->objtype == ObjectCategory::EFFECT)
        {
            EffectInfo::EFFECT_TYPE effect_type = jah_layer->getEffectLayer()->getType();

            if (effect_type == EffectInfo::CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE)
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

#ifndef JAHPLAYER
    if ( jah_layer->getLightingEnabled() != getUseLightingCheckBox()->isOn() )
    {
        // Can't access protected member setOn() directly, so have to do this annoying workaround
        getUseLightingCheckBox()->toggle();
    }
#endif

    //clicked on layer, update keyframes
    //should only do this if the layer has changed...
    updateKeyframeDisplay();   
    updateCompositeButton( jah_layer->getCompositeType() );

    //now update the sliders
#ifndef JAHPLAYER
    updateSliders();
#endif

    ////////////////////////////////////////////////////////
    //customization here for the animation module

    // Find the parent layer number if there is one
    JahLayer* parent_layer = jah_layer;

#ifndef JAHPLAYER
    if (jah_layer->objtype == ObjectCategory::EFFECT)
    {
        parent_layer = jah_layer->getParent();

        EffectLayer* effect_layer = jah_layer->getEffectLayer();

        if (effect_layer->getType() == EffectInfo::JAH_PLUGIN_TYPE)
        {
            setPluginAxisSliderLabels(effect_layer);
        }
        else
        {
            //setDefaultAxisSliderLabels();
        }
    }
    else
    {
        setDefaultAxisSliderLabels();
    }

    // Set up the OBJECT tab to be correct for the layer
    setObjectTab(parent_layer);

    if (jah_layer->layertype == LayerCategory::TEXT)
    {
        // Restore the present text in the text entry box
        m_pTextEd->setText(jah_layer->text->text);
    }
#endif

    item->setSelected(true);
    m_pLayerListView->setCurrentItem(item);

    setDontPaint(false);
    updateGL();
}

/////////////////////////////////////////////////
// connect ui add effects routines to m_pCore

void
GLAnime::addCpuEffectFromTopMenu()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if ( canAcceptEffects(getActiveJahLayer()->objtype) )
    {
        QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[ANIMATION];
        chooseNewCPUEffect( getActiveLayer(), (QWidget*)jah_control_menu_options, 1, 1 );
        //hideAllLightingSliders();
    }

    updateSliders();
}

void
GLAnime::addCpuEffect()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if ( canAcceptEffects(getActiveJahLayer()->objtype) )
    {
        chooseNewCPUEffect( getActiveLayer(),AddCpuEffectButton );
        //hideAllLightingSliders();
    }

    updateSliders();
}

void 
GLAnime::addGpuEffectFromTopMenu()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if ( canAcceptEffects(getActiveJahLayer()->objtype) )
    {
        QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[ANIMATION];
        chooseNewGPUEffect( getActiveLayer(), (QWidget*)jah_control_menu_options, 1, 1 );
        //hideAllLightingSliders();
    }

    updateSliders();
}

void 
GLAnime::addGpuEffect()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if ( canAcceptEffects(getActiveJahLayer()->objtype) )
    {
        chooseNewGPUEffect( getActiveLayer(),AddGpuEffectButton );
        //hideAllLightingSliders();
    }

    updateSliders();
}

void 
GLAnime::addMeshEffectFromTopMenu()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if (getActiveJahLayer()->objtype == ObjectCategory::MESHLAYER)
    {
        QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[ANIMATION];
        chooseNewMeshEffect( getActiveLayer(), (QWidget*)jah_control_menu_options, 1, 1 );
    }

    updateSliders();
}

void 
GLAnime::addMeshEffect()
{
    JahLayer* active_layer = getActiveJahLayer();

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        JahLayer* parent = getActiveJahLayer()->getParent();
        setActiveJahLayer(parent);
    }

    active_layer->getCheckListItem()->setSelected(false);

    if (getActiveJahLayer()->objtype == ObjectCategory::MESHLAYER)
    {
        chooseNewMeshEffect( getActiveLayer(),AddMeshEffectButton );
        //hideAllLightingSliders();
    }

    updateSliders();
}

//////////////////////////////////////////////////////
// new experimental routine to locate missing media
// needs to be built out into a pop-up window that shows all necessary data
// to help the user... and needs to moved into the m_pCore

//ask charles how to initialize the loader in single item mode vs multi item mode

void GLAnime::locateMissingMedia(void)
{
    QString result;
    bool accepted;

    FileMainWindow *  theloader = new FileMainWindow (this,"theloader",TRUE);
    theloader->fileView()->setDirectory( m_qsJahMediaPath+"/media/" ); 

    if ( theloader->exec() == QDialog::Accepted ) 
    {
        QValueList < QString > list = theloader->m_selected_items;
        QValueList < QString >::iterator it;

        for ( it = list.begin( ); it != list.end( ); it ++ )
            //loadJahAsset( *it );

            //result = theloader->returnFilename;
            result = *it;

        accepted = true;
    }
    else
    {
        accepted = false;
    }

    delete theloader;

    //do some extra error checking here
    if (accepted) 
    {
        if (result)
        { 
            assetExchange * exchange = new assetExchange;
            assetData newAsset;

            bool success;
            success=exchange->loadAsset(result, false, newAsset);

            if (success)
            {
                //debug("success");
                addClip(newAsset); 

                FindMediaText->setText( newAsset.getFullFileName() );
            }

			delete exchange;
        }
    }
}


void 
GLAnime::slotChooseLightColorAmbient()
{
    QColor newcolor;
    QColor color = QColorDialog::getColor( newcolor, 0, 0 );
    getLightingSliderPtr(0)->setValue( color.red() );
    getLightingSliderPtr(1)->setValue( color.green() );
    getLightingSliderPtr(2)->setValue( color.blue() );
}

void 
GLAnime::slotChooseLightColorDiffuse()
{
    QColor newcolor;
    QColor color = QColorDialog::getColor( newcolor, 0, 0 );
    getLightingSliderPtr(3)->setValue( color.red() );
    getLightingSliderPtr(4)->setValue( color.green() );
    getLightingSliderPtr(5)->setValue( color.blue() );
}

void 
GLAnime::slotChooseLightColorSpecular()
{
    QColor newcolor;
    QColor color = QColorDialog::getColor( newcolor, 0, 0 );
    getLightingSliderPtr(6)->setValue( color.red() );
    getLightingSliderPtr(7)->setValue( color.green() );
    getLightingSliderPtr(8)->setValue( color.blue() );
}

void 
GLAnime::slotLightColorAmbient(QColor color) 
{
    m_ambient_color_button->setPaletteBackgroundColor(color); 
}

void 
GLAnime::slotLightColorDiffuse(QColor color) 
{
    m_diffuse_color_button->setPaletteBackgroundColor(color); 
}

void 
GLAnime::slotLightColorSpecular(QColor color) 
{
    m_specular_color_button->setPaletteBackgroundColor(color); 
}

