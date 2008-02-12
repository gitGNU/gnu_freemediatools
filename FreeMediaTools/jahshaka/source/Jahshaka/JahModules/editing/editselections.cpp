/*******************************************************************************
**
** The source file for the Jahshaka editing m_bSelections module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/


#include "edit.h"

#include <jahformatter.h>
#include <qslider.h>
#include <qpopupmenu.h>
#include <jahtranslate.h>
#include <jahpreferences.h>
#include <InputLCD.h>
#include <dialogs.h>
#include <qlistview.h>
#include <qwidgetstack.h>
#include <qhbox.h>
#include <qlayout.h>
#include <valueFormatters.h>
//LayerListEntry
//JahLayer
#include <coreobject.h>
#include <glworldlists.h>

#include <openobject.h>

void GLEdit::addLayer(void)
{
//add a new layer
//issues when adding over a deleted layer -> need to reset the layer before a add is done...
    bool added = TRUE;
    bool isvideo = FALSE;

//add on demand layers...
QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Graphic",7,0 );
    menu->insertItem( "Text",4,0 );
    menu->insertItem( "VidLayer",1,0 );
//menu->insertItem( "Desktop",8,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( m_pAddButton->mapToGlobal( QPoint( 0, m_pAddButton->height() + 1 ) ) );
    QString layNum,temp;

    if ( id != -1 ) {

    m_number_of_layers++; //layNum.setNum(m_number_of_layers);

    switch(id) {
        case 1  : {

                    //only add where we need to grab the clip from the desktop first...
                    //unless on dual head then we need to implement drag and drop

                    getLayerList()->append( 
					   new LayerListEntry(
							      m_number_of_layers, 
							      "VidLayer") );
                    getLayerList()->at(m_number_of_layers)->thelayer->blend          = TRUE;           
                    getLayerList()->at(m_number_of_layers)->thelayer->depth = FALSE;
                    getLayerList()->at(m_number_of_layers)->thelayer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer"; 
                    getLayerList()->at(m_number_of_layers)->thelayer->asset.extension = ".bmp";
                    buildLayer(getLayer(),"VidLayer", LayerCategory::LAYER, ObjectCategory::JAHLAYER );

                    getLayerList()->at(m_number_of_layers)->thelayer->showEdges = false;
                    getLayerList()->at(m_number_of_layers)->thelayer->foreground = true;

                    isvideo = TRUE;

                    //addDesktop();

                    break; }

        case 4  : { getLayerList()->append( new LayerListEntry(m_number_of_layers, "3DText") );
                    getLayerList()->at(m_number_of_layers)->thelayer->addTextObj();
                    getLayerList()->at(m_number_of_layers)->thelayer->text->text = "jahshaka";
                    getLayerList()->at(m_number_of_layers)->thelayer->text->font = m_qsJahBasePath+"fonts/arial.ttf";
                    getLayerList()->at(m_number_of_layers)->thelayer->layernodes->m_node->extrude = DEFAULT_TEXT_EXTRUDE_DEPTH;
                    getLayerList()->at(m_number_of_layers)->thelayer->text->fontstyle = FontDrawstyle::FEXTRUDE; //or FSOLID

                    getLayerList()->at(m_number_of_layers)->thelayer->layernodes->m_node->sx = 
						getLayerList()->at(m_number_of_layers)->thelayer->layernodes->m_node->sy = 
						getLayerList()->at(m_number_of_layers)->thelayer->layernodes->m_node->sz = 5.0;

                    getLayerList()->at(m_number_of_layers)->thelayer->lighting = TRUE; getLayerList()->at(m_number_of_layers)->thelayer->depth = TRUE;
                    getLayerList()->at(m_number_of_layers)->thelayer->reflect = FALSE;
                    buildLayer(getLayer(),"3DText", LayerCategory::TEXT, ObjectCategory::JAHTEXT);

                    break; }

        case 7  : { getLayerList()->append( new LayerListEntry(m_number_of_layers, "Layer") );
                    getLayerList()->at(m_number_of_layers)->thelayer->blend          = TRUE;           
                    getLayerList()->at(m_number_of_layers)->thelayer->depth = TRUE;
                    getLayerList()->at(m_number_of_layers)->thelayer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer"; 
                    getLayerList()->at(m_number_of_layers)->thelayer->asset.extension = ".bmp";
                    buildLayer(getLayer(),"Layer", LayerCategory::LAYER, ObjectCategory::JAHLAYER );

                    break; }

        default  :  {  break; }
        //not functional yet, switches to desktop for a clip m_bSelection and returns
        //case 7  : { debug("Desktop");  break; }
    }
    } 
    else 
    { 
        added = FALSE;
    }

    delete menu;

    if (added) 
    {
        getLayerList()->at(m_number_of_layers)->thelayer->layerAssetStatus = TRUE;
        addListViewItem(TRUE,TRUE); 
        setObjectTab(getLayer()); 
        updateSliders();

    //The is where we need to add a bar to editinterface
    //only do this for graphics and text ok, need a flag...
    //ie bool isvideo;


        if (!isvideo) 
        {
            //debug("in the m_bSelection");
            theeditdesktop->addEditLayer( m_number_of_layers,
                                        getLayerList()->at(m_number_of_layers)->thelayer->asset.videotex,
                                        getLayerList()->at(m_number_of_layers)->thelayer->asset.startframe,
                                        getLayerList()->at(m_number_of_layers)->thelayer->asset.endframe
                                        );
        }
        else
        {
            addDesktop();
        }
}

}

//cool now a layer is created and we have the videodata for it...
void GLEdit::addClipEdit(assetData newclip) 
{

    //extract video data before moving ahead!
    //debug("in addClipEdit");

    //first add the clip to the layer itself
    addClip(getActiveJahLayer(), newclip);

    //now add the clip to the timeline
    theeditdesktop->addEditLayer(m_number_of_layers, newclip );


}






void GLEdit::changeParticle() {


    ParticleCategory::TYPE cat = ParticleCategory::ATOMIC;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Atom",1,0 );
    menu->insertItem( "JetSpray",2,0 );
    menu->insertItem( "Waterfall",3,0 );
    menu->insertItem( "Rocket",4,0 );
    menu->insertItem( "Explosion",5,0 );
    menu->insertItem( "Swirl",6,0 );
    menu->insertItem( "Balloons",7,0 );
    menu->insertItem( "Shape",8,0 );
    menu->insertItem( "FireFlies",9,0 );
    menu->insertItem( "Shower",10,0 );
    menu->insertItem( "Fountain",11,0 );


    menu->setMouseTracking( TRUE );
    int id = menu->exec( loadimage_3->mapToGlobal( QPoint( 0, loadimage_3->height() + 1 ) ) );

    if ( id != -1 ) 
    {
        switch(id) 
        {
                case 1  :  cat = ParticleCategory::ATOMIC; break;
                case 2  :  cat = ParticleCategory::JETSPRAY; break;
                case 3  :  cat = ParticleCategory::WATERFALL; break;
                case 4  :  cat = ParticleCategory::ROCKET; break;
                case 5  :  cat = ParticleCategory::EXPLOSION; break;
                case 6  :  cat = ParticleCategory::SWIRL; break;
                case 7  :  cat = ParticleCategory::BALLOONS; break;
                case 8  :  cat = ParticleCategory::SHAPE; break;
                case 9  :  cat = ParticleCategory::FIREFLIES; break;
                case 10 :  cat = ParticleCategory::SHOWER; break;
                case 11 :  cat = ParticleCategory::FOUNTAIN; break;
        }

        getActiveJahLayer()->ParticleStyle = cat;
        getActiveJahLayer()->ParticleFirsttime = TRUE;

    }

    delete menu;
    updateGL();  //redraw scene
}

void GLEdit::changeFontDraw() {

    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Solid",1,0 );
    menu->insertItem( "Outline",2,0 );
    menu->insertItem( "3D",3,0 );
    //menu->insertItem( "Textured",4,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( textDraw->mapToGlobal( QPoint( 0, textDraw->height() + 1 ) ) );

    if ( id != -1 ) {
    switch(id) {
                case 1  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FPOLYGON;    break;
                case 2  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FOUTLINE;    break;
                case 3  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FEXTRUDE;    break;
            //   case 4  :  layers[m_active_layer].text->fontstyle = FontDrawstyle::FTEXTURED; break;
                default :                                                    break;
    }
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLEdit::changeObjectDraw() {

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


void GLEdit::keyCliplayer() {

    GLenum blendmode = GL_ZERO;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "GL_ZERO",1,0 );
    menu->insertItem( "GL_ONE",2,0 );
    menu->insertItem( "GL_DST_COLOR",3,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_COLOR",4,0 );


    menu->setMouseTracking( TRUE );
    int id = menu->exec( keyimage_2->mapToGlobal( QPoint( 0, keyimage_2->height() + 1 ) ) );

    if ( id != -1 ) {
        switch(id) {
                case 1  : { qDebug("clip: GL_ZERO"); blendmode = GL_ZERO; break; }
                case 2  : { qDebug("clip: GL_ONE"); blendmode = GL_ONE; break; }
                case 3  : { qDebug("clip: GL_DST_COLOR"); blendmode = GL_DST_COLOR; break; }
                case 4  : { qDebug("clip: GL_ONE_MINUS_DST_COLOR"); blendmode = GL_ONE_MINUS_DST_COLOR; break; }

        }

        getActiveJahLayer()->blendmode1 = blendmode;
    }

    delete menu;
    updateGL();  //redraw scene
}

void GLEdit::keyClipkey() {

    GLenum blendmode = GL_ZERO;
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "GL_ZERO",1,0 );
    menu->insertItem( "GL_ONE",2,0 );
    menu->insertItem( "GL_SRC_COLOR",3,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_COLOR",4,0 );
    menu->insertItem( "GL_SRC_ALPHA",5,0 );
    menu->insertItem( "GL_ONE_MINUS_SRC_ALPHA",6,0 );
    menu->insertItem( "GL_DST_ALPHA",7,0 );
    menu->insertItem( "GL_ONE_MINUS_DST_ALPHA",8,0 );


    menu->setMouseTracking( TRUE );
    int id = menu->exec( keyimage_2->mapToGlobal( QPoint( 0, keyimage_2->height() + 1 ) ) );

    if ( id != -1 ) {
    switch(id) {
                case 1  : { qDebug("key: GL_ZERO"); blendmode = GL_ZERO; break; }
                case 2  : { qDebug("key: GL_ONE"); blendmode = GL_ONE; break; }
                case 3  : { qDebug("key: GL_SRC_COLOR"); blendmode = GL_SRC_COLOR; break; }
                case 4  : { qDebug("key: GL_ONE_MINUS_SRC_COLOR"); blendmode = GL_ONE_MINUS_SRC_COLOR; break; }
                case 5  : { qDebug("key: GL_SRC_ALPHA"); blendmode = GL_SRC_ALPHA; break; }
                case 6  : { qDebug("key: GL_ONE_MINUS_SRC_ALPHA"); blendmode = GL_ONE_MINUS_SRC_ALPHA; break; }
                case 7  : { qDebug("key: GL_DST_ALPHA"); blendmode = GL_DST_ALPHA; break; }
                case 8  : { qDebug("key: GL_ONE_MINUS_DST_ALPHA"); blendmode = GL_ONE_MINUS_DST_ALPHA; break; }
    }

    getActiveJahLayer()->blendmode2 = blendmode;
    }

    delete menu;
    updateGL();  //redraw scene

}












