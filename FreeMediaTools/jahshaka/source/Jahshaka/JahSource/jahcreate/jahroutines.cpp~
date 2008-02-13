/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahcreate.h"
#include <glcore.h>
#include "effect.h"
#include "paint.h"
#include "character.h"
#include "anime.h"
#include "edit.h"
#include "tracker.h"
#include "desktop.h"
#include "jahlibrary.h"
#include "MainPlayer.h"
#include "color.h"
#ifdef USE_NETWORK
#include "networking.h"
#endif
#include "keyer.h"
#include "MainEncoder.h"
#include "dialogs.h"
#include "stretchableButton.h"

#include <qvaluelist.h>
#include <widget.h>

////////////////////////////////////////////////////////////
// general module ui switching

void JahControl::raiseModule(Module mod)
{
	QString rere; rere.setNum(mod);
	int themod = 	ModulesID[mod];

	optionals->raiseWidget(themod);
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

	/////////////////////////////////////////////////
	//always raise the mediabar here!
	//unless you are in the library
	if (mod==LIBRARY)
		WorldLayoutLeft->raiseWidget(themod);
	else
		WorldLayoutLeft->raiseWidget(0);

	//raise the module part here
	//can use a flag to hide it if necessary for modules that
	//dont use it!
    WorldLayoutRight->raiseWidget(themod);

	///////////////////////////////////////////
	//switch between the different cores
	//currently paint has a different core
	if (mod==PAINT)
		    theCoreLayout->raiseWidget(1);
	else
		    theCoreLayout->raiseWidget(0);
	
	if (menuprefs)
		raiseMenu(mod);

	//used when we select the module via the module selection box
	selectMenuDropdown(mod);
}

void JahControl::raiseD2Module(Module mod)
{
	int themod = 	ModulesID[mod];

	//D2WorldLayout->raiseWidget(themod);
	D2optionals->raiseWidget(themod);
	D2MainControler->raiseWidget(themod);
	D2MainLeftControler->raiseWidget(themod);
	D2MainRightControler->raiseWidget(themod);         //raise the desktop controller on startup

    //will these work or are they in the wrong place?
    //need to restore these
	QValueList<int> sizeList;

	sizeList.append(MainModuleControlerLeftFrameWidth[themod]);
    sizeList.append(MainModuleControlerCenterFrameWidth[themod]);	
    sizeList.append(MainModuleControlerRightFrameWidth[themod]);

    if ( splitter2->sizes() != sizeList ) // (flickers if we setSizes when we dont need to).
	    splitter2->setSizes(sizeList);

    WorldLayoutLeft->raiseWidget(themod);
    WorldLayoutRight->raiseWidget(themod);
}



///////////////////////////////////////////////////////////////////
//used when loading a file in to parse the module name in the file

//and automatically switch to the appropriate module
void JahControl::switchModule(QString modname)
{
	modname = modname.lower();

	if (modname=="animation") changeModeAnimation();
	if (modname=="editing")   changeModeEditing();
	if (modname=="effects")   changeModeEffects();
	if (modname=="text")      changeModeText();
	if (modname=="text cg")      changeModeText();
	if (modname=="colorize")  changeModeColorize();
	if (modname=="desktop")   changeModeDesktop();
	if (modname=="paint")     changeModePaint();
#ifdef USE_NETWORK
	if (modname=="network")   changeModeNetworkCom(); //create a changeModeNetwork that calls changeModeNetworkCom as default
#endif
	if (modname=="keyer")     changeModeKeyer();
	if (modname=="tracker")   changeModeTracker();
	if (modname=="player")   changeModePlayer();
}



void JahControl::switchModule(Module modname)
{
	jtrace->debug("switching to",(int)modname);

	switch(modname) {

	    case ANIMATION    : {   changeModeAnimation();	break; }
        case EDITING      : {   changeModeEditing();	break; }
        case EFFECTS      : {   changeModeEffects();	break; }
        case TEXTCG       : {   changeModeText();		break; }
        case PAINT        : {   changeModePaint();		break; }
        case COLORIZE     : {   changeModeColorize();	break; }
        case TRACKER      : {   changeModeTracker();	break; }
        case KEYER        : {   changeModeKeyer();		break; }
        case PLAYER       : {   changeModePlayer();		break; }

        default           : {
                              jtrace->debug("switchModule()","unsupported module");
                              break;
                            }
	}
}

///////////////////////////////////////////////////////
// smart loader that loads media or scene files
//based on the module you are in
void JahControl::slotSmartLoader() 
{
	switch(module) {

	    case DESKTOP      : {   JahDesktopMod->slotLoadIconview();
                                break; }

		//this one needs to load the asset and then play the asset...
		//so asset needs to be selected after its loaded...
	    case PLAYER      : {   JahDesktopMod->slotLoadIconview();

								bool clipselected = false;
								assetData theclip = JahDesktopMod->getVideoData(clipselected);

								if (clipselected)
								{
									JahPlayerMod->LoadMySequence(theclip);
									JahPlayerMod->setFocus();
								} 

                                break; }

	    case ANIMATION    : {   JahAnimationMod->SceneLoad();
                                break; }
							
        case EDITING      : {   JahEditingMod->SceneLoad();
                                break; }
        case EFFECTS      : {   JahEffectsMod->SceneLoad();
                                break; }
        case TEXTCG       : {   JahTextMod->SceneLoad();
                                break; }
        //case PAINT        : {   JahAnimationMod->SceneLoad();
        //                        break; }
        case COLORIZE     : {   JahColorMod->SceneLoad();
                                break; }
        case TRACKER      : {   JahTrackerMod->SceneLoad();
                                break; }
        case KEYER        : {   JahKeyerMod->SceneLoad();
                                break; }
								
        default           : {
                              jtrace->debug("slotSmartLoader()","unsupported module");
                              break;
                            }
	}
}

///////////////////////////////////////////////////////
// smart loader that loads media or scene files
//based on the module you are in
void JahControl::slotSmartSaver() 
{
	switch(module) {

	    case DESKTOP      : {   //JahDesktopMod->slotLoadIconview();
                                break; }

		//this one needs to load the asset and then play the asset...
		//so asset needs to be selected after its loaded...
	    case PLAYER      : {   
                                break; }

	    case ANIMATION    : {   JahAnimationMod->sceneSave();
                                break; }
							
        case EDITING      : {   //JahEditingMod->sceneSave();
                                break; }
        case EFFECTS      : {   JahEffectsMod->sceneSave();
                                break; }
        case TEXTCG       : {   JahTextMod->sceneSave();
                                break; }
        case PAINT        : {   //JahPaintMod->SceneLoad();
                                break; }
        case COLORIZE     : {   JahColorMod->sceneSave();
                                break; }
        case TRACKER      : {   JahTrackerMod->sceneSave();
                                break; }
        case KEYER        : {   JahKeyerMod->sceneSave();
                                break; }
								
        default           : {
                              jtrace->debug("slotSmartSaver()","unsupported module");
                              break;
                            }
	}
}


//used to load the file into the correct module
void JahControl::loadModuleFile(QString fn)
{
    bool status = false;
	switch(module) {

	    case ANIMATION    : {   status=JahAnimationMod->SceneLoadName(fn);
                                break; }

        case EDITING      : {   status=JahEditingMod->SceneLoadName(fn);
                                break; }

        case EFFECTS      : {   status=JahEffectsMod->SceneLoadName(fn);
                                break; }

        case TEXTCG       : {   status=JahTextMod->SceneLoadName(fn);
                                break; }

        case PAINT        : {   status=JahPaintMod->SceneLoadName(fn);
                                break; }

        case COLORIZE     : {   status=JahColorMod->SceneLoadName(fn);
                                break; }

        case TRACKER      : {   status=JahTrackerMod->SceneLoadName(fn);
                                break; }

        case KEYER        : {   status=JahKeyerMod->SceneLoadName(fn);
                                break; }

        default           : {
                              jtrace->debug("loadModuleFile()","unsupported module");
                              break;
                            }
	}

    if (!status)
    {
        QMessageBox::information(0, "Could not read file  ", fn.data());
    }

}



//used to load the file into the correct module
//model loading only works in animation for now
void JahControl::loadModelFile(QString fn)
{
    bool status = false;
	switch(module) {

	    case ANIMATION    : {   status=JahAnimationMod->ModelLoadName(fn);
                                break; }

        default           : {
                              jtrace->debug("loadModelFile()","unsupported module");
                              break;
                            }
	}

    if (!status)
    {
        QMessageBox::information(0, "Could not read file  ", fn.data());
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
    //jtrace->debug("in stopAnimation");

    //heres a little hack since this is always called before switching to a new module
    saveSplitterValues();

     switch(module) {

	    case ANIMATION    : {   JahAnimationMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahAnimationMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahAnimationMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahAnimationMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahAnimationMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case EDITING      : {   JahEditingMod->stopanimation();     
                                QObject::disconnect(core, SIGNAL(paintit()), JahEditingMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahEditingMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahEditingMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahEditingMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                               break; }
        case EFFECTS      : {   JahEffectsMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahEffectsMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahEffectsMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahEffectsMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahEffectsMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case TEXTCG       : {   JahTextMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahTextMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahTextMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahTextMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahTextMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case PAINT        : {   JahPaintMod->stopanimation();        
                                QObject::disconnect(paintcore, SIGNAL(paintit()), JahPaintMod, SLOT(paintGL()) );
                                QObject::disconnect(paintcore, SIGNAL(mousePress(QMouseEvent*)),   JahPaintMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(paintcore, SIGNAL(mouseRelease(QMouseEvent*)), JahPaintMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(paintcore, SIGNAL(mouseMove(QMouseEvent*)),    JahPaintMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case COLORIZE     : {   JahColorMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahColorMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahColorMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahColorMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahColorMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case TRACKER      : {   JahTrackerMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahTrackerMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahTrackerMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahTrackerMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahTrackerMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
                                break; }
        case KEYER        : {   JahKeyerMod->stopanimation();
                                QObject::disconnect(core, SIGNAL(paintit()), JahKeyerMod, SLOT(paintGL()) );
                                QObject::disconnect(core, SIGNAL(mousePress(QMouseEvent*)),   JahKeyerMod, SLOT(mousePressEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseRelease(QMouseEvent*)), JahKeyerMod, SLOT(mouseReleaseEvent(QMouseEvent*)) );
                                QObject::disconnect(core, SIGNAL(mouseMove(QMouseEvent*)),    JahKeyerMod, SLOT(mouseMoveEvent(QMouseEvent*)) );
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

void 
JahControl::notifyEncoderOfChange()
{
    bool clipstatus = false;

    assetData theclip = JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
    {
        if (module == ENCODING)
        {
            JahEncoderMod->ClearOut();

            JahEncoderMod->LoadMySequence(theclip);
            JahEncoderMod->setFocus();
        }
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







