/*******************************************************************************
**
** The source file for the Jahshaka animation selections module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "character.h"
#include "anime.h"
#include "jahcreate.h"
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlistview.h>
#include <glworldlists.h>
#include <openobjectlib.h>

void 
GLChar::addLayer(void)
{

//add on demand layers...
    QPopupMenu *menu = new QPopupMenu( 0 );

//maxobjects are crashing on sgi build
    menu->insertItem( "Light", 0, 0 );
    menu->insertItem( "Textured", 1, 0 );
    menu->insertItem( "3d Extrude", 2, 0 );
    menu->insertItem( "Polygon", 3, 0 );
    menu->insertItem( "Outline", 4, 0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( m_pAddButton->mapToGlobal( QPoint( 0, m_pAddButton->height() + 1 ) ) );

	if ( id != -1 ) 
	{
		LAYER_TYPE type;

        if (id > 0)
        {
            type = LAYER_3D_TEXT;
        }
        else
        {
            type = LAYER_LIGHT;
        }

		createALayer(type, getActiveJahLayer() );

        switch(id) 
        {
            case 4  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FOUTLINE;    break;
            case 3  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FPOLYGON;    break;
            case 2  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FEXTRUDE;    break;
            case 1  :  
            {
                getActiveJahLayer()->text->fontstyle = FontDrawstyle::FTEXTURE;    
                
                if ( !getActiveJahLayer()->layerAssetStatus )
                {
                    // Adds whatever is selected on the desktop as a texture.  This is needed to
                    // make textured text appear visible.  A better fix would be to set up a 
                    // default asset and use that, because if nothing on the desktop is selected 
                    // it will do nothing
                    JahControl* jah_control = Globals::getJahControl();
                    jah_control->GrabClipCG();
                }

                break;
            }
            default : break;
        }

	}

    delete menu;
    updateGL(); 
}

void
GLChar::addLightFromTopMenu(void)
{
	LAYER_TYPE type;
	type = LAYER_LIGHT;

	createALayer(type, getActiveJahLayer() );

    updateGL(); 
}

void 
GLChar::addLayerFromTopMenu(int val)
{

	LAYER_TYPE type;
	type = LAYER_3D_TEXT;

	createALayer(type, getActiveJahLayer() );

    switch(val) 
    {
        case 4  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FOUTLINE;    break;
        case 3  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FPOLYGON;    break;
        case 2  :  getActiveJahLayer()->text->fontstyle = FontDrawstyle::FEXTRUDE;    break;
        case 1  :  
        {
            getActiveJahLayer()->text->fontstyle = FontDrawstyle::FTEXTURE;    
            
            if ( !getActiveJahLayer()->layerAssetStatus )
            {
                // Adds whatever is selected on the desktop as a texture.  This is needed to
                // make textured text appear visible.  A better fix would be to set up a 
                // default asset and use that, because if nothing on the desktop is selected 
                // it will do nothing
                JahControl* jah_control = Globals::getJahControl();
                jah_control->GrabClipCG();
            }

            break;
        }
        default : break;
    }


    updateGL(); 
}

