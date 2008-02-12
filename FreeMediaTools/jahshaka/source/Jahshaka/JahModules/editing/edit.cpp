/*******************************************************************************
**
** The source file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2005 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "edit.h"
#include <InputLCD.h>
#include <coreobject.h>
//./source/Jahshaka/


void GLEdit::initializeObjects() 
{
    m_pJTrace->info( "Initializing GLEdit Class");
    
    //make the current opengl context current
    makeCurrent();

	//initialize the timeline object
    theeditdesktop = new EditDesktop( this, moduleOptionsUI[4], "editdesk" );
    theeditdesktop->setFixedSize( QSize( 980, 600 ) );    //was 955,486

	configureLayer ( "World", false, false, false, true );
    buildLayer(

	       getLayer(), // new parameter !
	       "Background", LayerCategory::LAYER, ObjectCategory::BACKDROP );

	//setup/customize listviewitem for layer 1
	initializeListview(0,false);

    //hook up menu and sliders and default tool
    setObjectTab(getLayer()); 
    
    updateSliders();

    //turn forceplay on here
    setForcePlay(true);

}

//////////////////////////////////////////////////
// the new mediatable is initalized here
void GLEdit::initializeMediaTable( QHBox* parentbox)
{

    mediatable = 0;

    // the constructor does not work! new JahDesktopSideTable( 0, parentbox );



    //connect ( mediatable, SIGNAL(currentChanged ( int, int )), this, SLOT( m_bSelectSideTableRow(int,int)) );

    //connect ( editor, SIGNAL(itemRenamed( int, QString )), mediatable, SLOT( renameAsset(int, QString))); 
    //sidetableSelect = -1;
}


void GLEdit::updateUiSettings()
{
    ////////////////////////////////////////////////////////////////////////////////////
    // all other ui settings
    //for transparency

    //update video frames here
    FramesLcd->setValue   ( getActiveJahLayer()->asset.getNumFrames() );
    inFramesLcd->setValue ( getActiveJahLayer()->m_inframe );
    outFramesLcd->setValue( getActiveJahLayer()->m_outframe );

    KeyFramesLcd->setValue   ( getActiveJahLayer()->keyasset.getNumFrames() );
    inKeyFramesLcd->setValue ( getActiveJahLayer()->m_keyinframe );
    outKeyFramesLcd->setValue( getActiveJahLayer()->m_keyoutframe );

    extendHeadTail->setChecked ( getActiveJahLayer()->showEdges );
    toggleTheLoop->setChecked ( getActiveJahLayer()->loop );
    togglePing->setChecked ( getActiveJahLayer()->ping );

    //extended values here need to add everything new
    GridSelect->setChecked(m_bGridVal);
    if (m_pTextSafe)
      {
	TextSelect->setChecked(m_pTextSafe->layerStatus);
      }

    ClipSelect->setChecked(getActiveJahLayer()->drawtheLayer);
    KeySelect->setChecked(getActiveJahLayer()->drawtheKey);

}




/// edit 


/*
  adding the linking function
*/
void GLEdit::glworldUpdate()
{

  m_pJTrace->info( "GLEdit::glworldUpdate" );

}


void GLEdit::addListViewItem(bool, bool)
{  };

void GLEdit::updateanimframe(int)
{  };


void 
GLEdit::updateEditanimframe(int)
{  };

void 
GLEdit::updateSetinFrames(int, int)
{  };




GLEdit::GLEdit( 
	       GLCore* m_pCore, 
	       QHBox*, 
	       QHBox*, 
	       const char* name,
	       QHBox* controls, 
	       QHBox* controller, 
	       QHBox* leftcontroller,
	       QHBox* rightcontroller,
	       int* globalm_nClipnumber )
        :  GLWorld(m_pCore,name,controller,globalm_nClipnumber ) 
{
    buildInterface(controls);
    buildInterfaceMenus(leftcontroller,rightcontroller);

    initializeObjects();
};


/*
  moved out from the 
*/
GLEdit::~GLEdit() 
{ 

 }
