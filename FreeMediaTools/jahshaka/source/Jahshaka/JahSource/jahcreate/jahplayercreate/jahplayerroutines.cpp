/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayercreate.h"
#include "anime.h"
#include <glcore.h>
#include <widget.h>

#include <qvaluelist.h>

////////////////////////////////////////////////////////////
// general module ui switching

void JahControl::raiseModule(Module mod)
{
	QString rere; rere.setNum(mod);
	int themod = 	ModulesID[mod];

	
	//WorldLayout->raiseWidget(themod);	
//	optionals->raiseWidget(themod);
	MainControler->raiseWidget(themod);

	MainLeftControler->raiseWidget(themod);
	MainRightControler->raiseWidget(themod);         //raise the desktop controller on startup

    //need to restore thesplitter
	QValueList<int> sizeList;

	sizeList.append(MainModuleControlerLeftFrameWidth[themod]);
    sizeList.append(MainModuleControlerCenterFrameWidth[themod]);	
    sizeList.append(MainModuleControlerRightFrameWidth[themod]);

	if ( splitter2->sizes() != sizeList ) // (flickers if we setSizes when we dont need to).
        splitter2->setSizes(sizeList);

	WorldLayoutLeft->raiseWidget((mod == LIBRARY) ? themod : 0);
    WorldLayoutRight->raiseWidget(themod);
}





///////////////////////////////////////////////////////////////////
//used when loading a file in to parse the module name in the file

//and automatically switch to the appropriate module
void JahControl::switchModule(QString modname)
{
	modname = modname.lower();

	if (modname=="animation") changeModeAnimation();
	if (modname=="desktop")   changeModeDesktop();

}

//this is to pass data down to the modules
/*
void JahControl::loadJahSceneFile(QString filename)
{
    jtrace->debug("loading",filename);
    JahAnimationMod->LoadAll(filename);
    changeModeAnimation();
}
*/ 

//used to load the file into the correct module
//this way ya need to be in animation to load a scene...
//the old way we switched ya over as part of the gig

void JahControl::loadModuleFile(QString fn)
{
    bool status = false;

	JahAnimationMod->activate();

    status = JahAnimationMod->SceneLoadName(fn);

    if (!status)
    {
        QMessageBox::information(0, "Could not read file  ", fn.data());
    }
    else
    {
        changeModeAnimation();
    }

}

void JahControl::loadModelFile(QString fn)
{
    bool status = false;

	JahAnimationMod->activate();

    status = JahAnimationMod->ModelLoadName(fn);

    if (!status)
    {
        QMessageBox::information(0, "Could not read file  ", fn.data());
    }
    else
    {
        changeModeAnimation();
    }

}

/////////////////////////////////////////////////////////////
// used to store and restore splitter values
void JahControl::saveSplitterValues() 
{    
	//////////////////////////////////////////////////////////////////////////
	//also we need to remember size of left/right ui's for when the application restarts

	int themod = 	ModulesID[module];

    QValueList<int> list = splitter2->sizes();
    QValueList<int>::Iterator it = list.begin();

    int v1 = *it;
    MainModuleControlerLeftFrameWidth[themod]=v1;
    
    ++it;
    int v2 = *it;
    MainModuleControlerCenterFrameWidth[themod]=v2;
    
    ++it;
    int v3 = *it;
    MainModuleControlerRightFrameWidth[themod]=v3;

   
    //if ( widescreen )
    if ( widescreenprefs )
    {
        int theval1 = MainModuleControlerLeftFrameWidth[themod];
        
        for (int i=0; i<module_counter; i++)
        {
            if (MainModuleControlerLeftFrameWidth[i]!=theval1)
            {
                int v1 = MainModuleControlerLeftFrameWidth[i];
                int v2 = MainModuleControlerCenterFrameWidth[i];
                int v3 = MainModuleControlerRightFrameWidth[i];

                int t1 = v1+v2+v3;  //total width before change

                int leftover = t1 - theval1;

                MainModuleControlerLeftFrameWidth[i] = theval1;
                
                MainModuleControlerCenterFrameWidth[i]=
                    leftover - MainModuleControlerRightFrameWidth[i];
            }
        }
    }
}
/////////////////////////////////////////////////////////////
// used to disconnect a module when switching

void JahControl::stopAnimation() 
{    
	//stop playback in other modules
  jtrace->debug("in stopAnimation");

    //heres a little hack since this is always called before switching to a new module
    saveSplitterValues();

     switch(module) {

	    case ANIMATION    : {   JahAnimationMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahAnimationMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahAnimationMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahAnimationMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahAnimationMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
  
        //added to stop asset plaing on the desktop when switching modules
	    case DESKTOP      : { if (!dualhead) 
                                JahDesktopMod->stopanimation();  
                                
                              break; }  //was StopPlayer

        case PLAYER       : {   JahPlayerMod->Stop();        
								
								break; }  //was StopPlayer

        default           : {
                              jtrace->debug("JahControl::","unknown module in stopAnimation()");
                              break;
                            }

      }

}


/////////////////////////////////////////////////////////
// utility routines

void JahControl::getClipFromLibrary()
{
	QValueList < QString > list = JahLibraryMod->m_selected_items;
	QValueList < QString >::iterator it;
	for ( it = list.begin( ); it != list.end( ); it ++ )
       	addStringToDesktop( *it );
	changeModeDesktop();
}

void JahControl::checkClearPlayer() 
{
    if (  (module == PLAYER) )
    { 
        JahPlayerMod->Stop(); 
    }
}

void JahControl::syncMediaTable( assetData data )
{
	JahDesktopMod->syncMediaTable( data );
}

void JahControl::notifyPlayerOfChange()
{
    bool clipstatus = false;

    assetData theclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
    {
        if (module == PLAYER)
        {
            JahPlayerMod->Stop();

            JahPlayerMod->LoadMySequence(theclip);
            JahPlayerMod->setFocus();

        }
    }
}

void JahControl::notifyPlayerOfNextItem( )
{
	JahDesktopMod->selectNextAsset( );
	notifyPlayerOfChange( );
}

//used to issue the rename dialog for desktop clips
void JahControl::nameClip() 
{  
    if (globalclipnumber>0) 
    {
		bool clipstatus = false;
		assetData data = JahDesktopMod->getVideoData(clipstatus);

		if (clipstatus)
		{
			QString oldname = JahDesktopMod->getClipname();
			popup2->popup(DesktopName,oldname);
		}
	}
}

//check to see if a clip is selected first or we crash!
void JahControl::renameDesktopAsset(QString item) 
{
    //grab the current desktop clip
    bool clipstatus = false;
    assetData data = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
    {
		if (item.length() > 0 )
		{
			//find out the clip number
			int clipnum = data.clipnumber;

			//if its a good number ie >0 continue
			if (clipnum>0)
			{
				JahDesktopMod->setClipname(item);

				//then update the database
				JahDesktopMod->renameImageindatabase( clipnum, item);

				//force a full refresh
				JahDesktopMod->doRefresh();
			}
		}
    }

}

///////////////////////////////////////////////////////
//used for debugging to get the modulename

QString JahControl::getModuleName(Module themodule)
{
      switch(themodule) {

			case ANIMATION	:  return "ANIMATION";		break;
			case EFFECTS	:  return "EFFECTS";		break;
			case EDITING	:  return "EDITING";		break;
			case TEXTCG		:  return "TEXTCG";			break;
			case PAINT		:  return "PAINT";			break;
			case COLORIZE	:  return "COLORIZE";		break;
			case KEYER		:  return "KEYER";			break;
			case TRACKER	:  return "TRACKER";		break;

			default			:  return "UNKNOWN MODULE"; break;
      }
}

///////////////////////////////////////////////////////////
//desktop menu items should be moved into desktop itself

void JahControl::switchMode(void)
{
   //add a new layer
   //issues when adding over a deleted layer -> need to reset the layer before a add is done...
   bool added = TRUE;
   //need a global mode variable to track the main mode...

   //add on demand layers...
   QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "Web",3,0 );
    menu->insertItem( "Audio",2,0 );
    menu->insertItem( "Video",1,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( modeselectbutton->mapToGlobal( QPoint( 0, modeselectbutton->height() + 1 ) ) );
    QString layNum,temp;

    if ( id != -1 ) 
    {
   
      switch(id) {
         case 1   :  { break; }
         case 2   :  { break; }
         case 3   :  { break; }
         default  :  {  break; }
      }
    } 
    else 
    { 
        added = FALSE;
    }

    delete menu;

}


void JahControl::switchDesktop(void)
{
   //add a new layer
   //issues when adding over a deleted layer -> need to reset the layer before a add is done...
   bool added = TRUE;
   //need a global mode variable to track the main mode...

   //add on demand layers...
   QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( ">> add new",2,0 );
    menu->insertItem( "Desktop 1",1,0 );

    menu->setMouseTracking( TRUE );
    int id = menu->exec( desktopselectbutton->mapToGlobal( QPoint( 0, desktopselectbutton->height() + 1 ) ) );
    QString layNum,temp;

    if ( id != -1 ) {
      //if (numberLayers==NUMBER_LAYERS) return;
      //numberLayers+=1; layNum.setNum(numberLayers);

      switch(id) {
         case 1  : { break; }
         case 2  : { break; }
         case 3  : { break; }
         default  :  {  break; }

      }
    } else { added = FALSE;}

    delete menu;

}







