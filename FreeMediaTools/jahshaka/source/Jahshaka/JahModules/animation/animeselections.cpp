/*******************************************************************************
 **
 ** The source file for the Jahshaka animation m_bSelections module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

#include "anime.h"

#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qobject.h>

#include "jahcreate.h"
#include "eyeCheckListItem.h"
#include <glworldlists.h>
#include <dialogs.h>
#include <InputLCD.h>
#include <openobjectlib.h>

#include <FreeMediaTools.h>

template < typename T > 
QCheckListItem* 
factory_( T* parent, QListViewItem* after, const QString& s )
{
    if ( after )
	return new EyeCheckListItem( parent, after, s, QCheckListItem::CheckBox );

    return new EyeCheckListItem( parent, s, QCheckListItem::CheckBox );
}

void 
GLAnime::initializeListview(JahLayer* jah_layer, bool layer_status)
{
    QCheckListItem* new_q_check_list_item = addCheckListItem(m_pLayerListView, layer_status, FALSE, &factory_<QListView>);
    jah_layer->setCheckListItem(new_q_check_list_item);
}


JahLayer*
GLAnime::createALayer(LAYER_TYPE type, JahLayer* parent_jah_layer)
{
    LayerListEntry* new_layer_list_entry;
    JahLayer* new_layer = NULL;
    QCheckListItem* parent_check_list_item = NULL;

    switch(type) 
    {
        case LAYER_3D_OBJECT   : //create a 3d object
        {
            new_layer_list_entry = new LayerListEntry("3DObject");
            new_layer = new_layer_list_entry->thelayer;
            getLayerList()->append(new_layer_list_entry);

            new_layer->blend    = TRUE;
            new_layer->depth = TRUE;
            new_layer->lighting = TRUE;
            new_layer->objfilename = m_qsJahBasePath+"media/models/f-16";      
            new_layer->objextension = ".obj";
            buildLayer(new_layer, "3DObject", LayerCategory::OBJECT, ObjectCategory::OBJECT );
            break; 
        }

        case LAYER_PARTICLE  : //create a particle system
        { 
            new_layer_list_entry = new LayerListEntry("Particle");
            new_layer = new_layer_list_entry->thelayer;
            getLayerList()->append(new_layer_list_entry);

            new_layer->ParticleStyle = ParticleCategory::SWIRL;
            new_layer->blend    = TRUE;
            new_layer->depth = TRUE;
            new_layer->lighting = FALSE;
            buildLayer(new_layer, "Particle", LayerCategory::PARTICLE, ObjectCategory::JAHPARTICLE );
            break; 
        }

        case LAYER_LIGHT  : //create a light
        { 
            new_layer_list_entry = new LayerListEntry("Light");
            new_layer = new_layer_list_entry->getJahLayer();
            getLayerList()->append(new_layer_list_entry);
            new_layer->setLayerListEntry(new_layer_list_entry);
            new_layer->layernodes->m_node->tz = 50.0f;
            new_layer->depth = TRUE;

            if ( parent_jah_layer->getParent() )
            {
                parent_jah_layer = parent_jah_layer->getParent();
            }

            parent_jah_layer->getLightsList()->append(new_layer);

            buildLayer(new_layer, "Light", LayerCategory::LIGHT, ObjectCategory::JAHLIGHT );

            if (  parent_jah_layer->canAcceptLighting() )
            {
                parent_jah_layer->setLightingEnabled(true);
                parent_check_list_item = parent_jah_layer->getCheckListItem();
                new_layer->setParent(parent_jah_layer);
            }
            else if ( 
                        parent_jah_layer->objtype == ObjectCategory::EFFECT
                        || parent_jah_layer->objtype == ObjectCategory::JAHLIGHT
                     )
            {
                // Add it to the true parent
                parent_jah_layer = parent_jah_layer->getParent();
                parent_jah_layer->setLightingEnabled(true);
                parent_check_list_item = parent_jah_layer->getCheckListItem();
                new_layer->setParent( parent_jah_layer->getParent() );
            }
            else 
            {
                // Make it a global light
                parent_check_list_item = m_pCamera->getCheckListItem();
            }

            setActiveJahLayer(new_layer);
            break; 
        }

        case LAYER_3D_TEXT  : 
        {
            new_layer_list_entry =  new LayerListEntry("3DText");
            new_layer = new_layer_list_entry->thelayer;
            getLayerList()->append(new_layer_list_entry);

            new_layer->addTextObj();
            new_layer->text->text = FMT_PROGRAM_TITLE;
            new_layer->text->font = m_qsJahBasePath+"fonts/arial.ttf";
            new_layer->layernodes->m_node->extrude = DEFAULT_TEXT_EXTRUDE_DEPTH;
            new_layer->text->fontstyle = FontDrawstyle::FEXTRUDE; //or FSOLID
            new_layer->depth = TRUE;
            new_layer->reflect = FALSE;
            new_layer->Red = 1.0f;
            new_layer->Green = 1.0f;
            new_layer->Blue = 1.0f;
            //ScaleX_5->setValue( int(DEFAULT_TEXT_EXTRUDE_DEPTH) );
            //ScaleXlcd_5->setValue( int(DEFAULT_TEXT_EXTRUDE_DEPTH) );

            buildLayer(new_layer, "3DText", LayerCategory::TEXT, ObjectCategory::JAHTEXT);
            /*
            //fonts dont have assets by default
            //need to fix this?
            new_layer->asset.theCategory = VideoCategory::IMAGE;
            new_layer->asset.filename = m_qsJahBasePath+"media/images/earthtex";
            std::string temp_string = new_layer->asset.filename.data();
            new_layer->asset.extension = ".bmp";
            std::string layer_name = "3D Text";
            */
            new_layer->layertype = LayerCategory::TEXT;
            new_layer->objtype = ObjectCategory::JAHTEXT;
            new_layer->setLightingEnabled(true);
            //new_layer->get

            break; 
        }

        case LAYER_IMAGE_LARGE  : //creates a large layer
        { 
            new_layer_list_entry = new LayerListEntry("Matte Layer");
            new_layer = new_layer_list_entry->thelayer;

            getLayerList()->append(new_layer_list_entry);
            new_layer->blend          = TRUE;           
            new_layer->depth = TRUE;
            new_layer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer"; 
            new_layer->asset.extension = ".bmp";
            new_layer->layerAssetStatus = TRUE;
            buildLayer(new_layer, "Matte Layer", LayerCategory::LAYER, ObjectCategory::LARGE_LAYER);

            break; 
        }

        case LAYER_IMAGE  : //creates a layer
        { 
            new_layer_list_entry = new LayerListEntry("Layer");
            new_layer = new_layer_list_entry->thelayer;

            getLayerList()->append(new_layer_list_entry);
            new_layer->blend          = TRUE;           
            new_layer->depth = TRUE;
            new_layer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer"; 
            new_layer->asset.extension = ".bmp";
            new_layer->layerAssetStatus = TRUE;
            buildLayer(new_layer, "Layer", LayerCategory::LAYER, ObjectCategory::MESHLAYER );

            break; 
        }

        default  :  {  break; }
    }

    new_layer->setCameraLayer(m_pCamera);

    clearSelection(m_pLayerListView);


    QCheckListItem* new_check_list_item;
    
    if (parent_check_list_item)
    {
        new_check_list_item = (QCheckListItem*)addCheckListItem(parent_check_list_item, TRUE, FALSE, &factory_<QCheckListItem>); 
        parent_check_list_item->setOpen(true);
    }
    else
    {
        new_check_list_item = (QCheckListItem*)addCheckListItem(m_pLayerListView, TRUE, FALSE, &factory_<QListView>); 
    }

    new_layer->setCheckListItem(new_check_list_item);
    new_check_list_item->setSelected(true);
    new_check_list_item->setOn(true);

    setObjectTab( getLayerList()->last()->getJahLayer() );

    updateGL();

    updateSliders();


    setDefaultAxisSliderLabels();

    updateKeyframeDisplay();

    return new_layer;
}

void GLAnime::nameLayerFromTopMenu(void) 
{
    QPopupMenu* jah_control_menu_options = Globals::getJahControl()->menuOptions[ANIMATION];
    m_pNamePopUp->popup(0, 0, jah_control_menu_options, getActiveJahLayer()->getCheckListItem()->text(0)); 
}


void GLAnime::addLayerFromTopMenu(int val)
{
		LAYER_TYPE type = LAYER_TYPE(val);
		createALayer(type, getActiveJahLayer() );
}

void GLAnime::addLayer(void)
{
//add on demand layers...
    QPopupMenu *menu = new QPopupMenu( 0 );

//maxobjects are crashing on sgi build
    menu->insertItem( "3D Object", LAYER_3D_OBJECT, 0 );
    menu->insertItem( "Particle", LAYER_PARTICLE, 0 );
    menu->insertItem( "Light", LAYER_LIGHT, 0 );
    menu->insertItem( "3D Text", LAYER_3D_TEXT, 0 );
    menu->insertItem( "Matte Layer", LAYER_IMAGE_LARGE, 0 );
    menu->insertItem( "Layer", LAYER_IMAGE, 0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( m_pAddButton->mapToGlobal( QPoint( 0, m_pAddButton->height() + 1 ) ) );

	if ( id != -1 ) 
	{
		LAYER_TYPE type = LAYER_TYPE(id);
		createALayer(type, getActiveJahLayer() );
	}

    delete menu;
}

void GLAnime::changeParticle() {

    ParticleCategory::TYPE cat = ParticleCategory::ATOMIC;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Atom",1,0 );
    menu->insertItem( "JetSpray",2,0 );
    menu->insertItem( "Rocket",3,0 );
    menu->insertItem( "Explosion",4,0 );
    menu->insertItem( "Swirl",5,0 );
    menu->insertItem( "Balloons",6,0 );
    menu->insertItem( "Shape",7,0 );
    menu->insertItem( "FireFlies",8,0 );
    menu->insertItem( "Fountain",9,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( loadimage_3->mapToGlobal( QPoint( 0, loadimage_3->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 1  :  cat = ParticleCategory::ATOMIC;    break;
            case 2  :  cat = ParticleCategory::JETSPRAY;  break;
            case 3  :  cat = ParticleCategory::ROCKET;    break;
            case 4  :  cat = ParticleCategory::EXPLOSION; break;
            case 5  :  cat = ParticleCategory::SWIRL;     break;
            case 6  :  cat = ParticleCategory::BALLOONS;  break;
            case 7  :  cat = ParticleCategory::SHAPE;     break;
            case 8  :  cat = ParticleCategory::FIREFLIES; break;
            case 9  :  cat = ParticleCategory::FOUNTAIN;  break;

        }

        getActiveJahLayer()->ParticleStyle = cat;
        getActiveJahLayer()->ParticleFirsttime = TRUE;
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLAnime::changeParticleDraw() {

    ParticleDrawstyle::TYPE cat = ParticleDrawstyle::PARPOINT;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Point",1,0 );
    menu->insertItem( "Line",2,0 );
    menu->insertItem( "Texture",3,0 );
    menu->insertItem( "Clip",4,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( loadimage_3->mapToGlobal( QPoint( 0, loadimage_3->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 1  :  cat = ParticleDrawstyle::PARPOINT;   break;
            case 2  :  cat = ParticleDrawstyle::PARLINE;    break;
            case 3  :  cat = ParticleDrawstyle::PARGAUSS;   break;
            case 4  :  cat = ParticleDrawstyle::PARTEXTURE; break;
            default :                    break;

        }

        getActiveJahLayer()->ParticleDraw=cat;
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLAnime::changeFontDraw() {

    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Textured",  6,0 );
    menu->insertItem( "3d Extrude",5,0 );
    menu->insertItem( "Polygon",   4,0 );
    menu->insertItem( "Outline",   3,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( textDraw->mapToGlobal( QPoint( 0, textDraw->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 3  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FOUTLINE;    break;
            case 4  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FPOLYGON;    break;
            case 5  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FEXTRUDE;    break;
            case 6  :  
            {
                getActiveJahLayer()->text->fontstyle = FontDrawstyle::FTEXTURE;    
                
                if ( !getActiveJahLayer()->layerAssetStatus )
                {
                    // Adds whatever is m_bSelected on the desktop as a texture.  This is needed to
                    // make textured text appear visible.  A better fix would be to set up a 
                    // default asset and use that, because if nothing on the desktop is m_bSelected 
                    // it will do nothing
                    JahControl* jah_control = Globals::getJahControl();
                    jah_control->GrabClipAnim();
                }

                break;
            }
            default :                                                    break;
        }
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLAnime::changeObjectDraw() {
#ifdef NEWOPENOBJECTLIB
#else
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Plain",1,0 );
    menu->insertItem( "Color",2,0 );
    menu->insertItem( "Material",3,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( objectstyle->mapToGlobal( QPoint( 0, objectstyle->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 1  :  getActiveJahLayer()->jahobject->setMaterialMode(0); break;
            case 2  :  getActiveJahLayer()->jahobject->setMaterialMode(1); break;
            case 3  :  getActiveJahLayer()->jahobject->setMaterialMode(2); break;
            default :                                          break;
        }
    }

    delete menu;
    updateGL();  //redraw scene
#endif
}


void GLAnime::keyCliplayer() {

    GLenum blendmode = GL_ONE;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "GL_ZERO",1,0 );
    menu->insertItem( "GL_ONE",2,0 );
    menu->insertItem( "GL_SRC_COLOR",3,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_COLOR",4,0 );
    menu->insertItem( "GL_SRC_ALPHA",5,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_ALPHA",6,0 );
    menu->insertItem( "GL_DST_COLOR",7,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_COLOR",8,0 );
    menu->insertItem( "GL_DST_ALPHA",9,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_ALPHA",10,0 );


    menu->setMouseTracking( TRUE );
    int id = menu->exec( keyimage_2->mapToGlobal( QPoint( 0, keyimage_2->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 1  : { blendmode = GL_ZERO; break; }
            case 2  : { blendmode = GL_ONE; break; }
            case 3  : { blendmode = GL_SRC_COLOR; break; }
            case 4  : { blendmode = GL_ONE_MINUS_SRC_COLOR; break; }
            case 5  : { blendmode = GL_SRC_ALPHA; break; }
            case 6  : { blendmode = GL_ONE_MINUS_SRC_ALPHA; break; }
            case 7  : { blendmode = GL_DST_COLOR; break; }
            case 8  : { blendmode = GL_ONE_MINUS_DST_COLOR; break; }
            case 9  : { blendmode = GL_DST_ALPHA; break; }
            case 10 : { blendmode = GL_ONE_MINUS_DST_ALPHA; break; }
        }

        getActiveJahLayer()->blendmode1 = blendmode;
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLAnime::keyClipkey() {

    GLenum blendmode = GL_SRC_ALPHA;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "GL_ZERO",1,0 );
    menu->insertItem( "GL_ONE",2,0 );
    menu->insertItem( "GL_SRC_COLOR",3,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_COLOR",4,0 );
    menu->insertItem( "GL_SRC_ALPHA",5,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_ALPHA",6,0 );
    menu->insertItem( "GL_DST_COLOR",7,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_COLOR",8,0 );
    menu->insertItem( "GL_DST_ALPHA",9,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_ALPHA",10,0 );


    menu->setMouseTracking( TRUE );
    int id = menu->exec( keyimage_2->mapToGlobal( QPoint( 0, keyimage_2->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
            case 1  : { blendmode = GL_ZERO; break; }
            case 2  : { blendmode = GL_ONE; break; }
            case 3  : { blendmode = GL_SRC_COLOR; break; }
            case 4  : { blendmode = GL_ONE_MINUS_SRC_COLOR; break; }
            case 5  : { blendmode = GL_SRC_ALPHA; break; }
            case 6  : { blendmode = GL_ONE_MINUS_SRC_ALPHA; break; }
            case 7  : { blendmode = GL_DST_COLOR; break; }
            case 8  : { blendmode = GL_ONE_MINUS_DST_COLOR; break; }
            case 9  : { blendmode = GL_DST_ALPHA; break; }
            case 10 : { blendmode = GL_ONE_MINUS_DST_ALPHA; break; }
        }

        getActiveJahLayer()->blendmode2 = blendmode;
    }

    delete menu;
    updateGL();  //redraw scene

}











