/*******************************************************************************
 **
 ** The source file for the Jahshaka jahcreate module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the main class that creates and launches the jahshaka interface
// and sets up and connects all the modules and interface objects

#include "jahcreate.h"

#include "effect.h"
#include "paint.h"
#include "character.h"
#include "anime.h"
#include "edit.h"
#include "tracker.h"

#include "desktop.h"
#include "jahlibrary.h"
#include "MainPlayer.h"
#include "dialogs.h"
#include "glworld.h"
#include "color.h"
#include "networking.h"
#include "keyer.h"
#include "jahtranslate.h"
#include "jahheader.h"

//for the encoder module
#include "MainEncoder.h"

void JahControl::buildMenuBar( JahHeader* h )
{
    if ( !h )
    {
        return;
    }

    menu = h->menuBar();

    /////////////////////////////////////////////////
    //build the menus in the menubox
    buildDropDownMenus( menu );
    
    /////////////////////////////////////////////////
    //create the mode box
    buildModeSelector( h->moduleSelectionPlaceholder() );
    
    /////////////////////////////////////////////////
    //create feedbackbar
    CreateDesktopFeedbackBox( h->feedbackPlaceholder() );
}

void JahControl::buildModeSelector( QWidget* parent )
{
    modeSelector = new QComboBox( FALSE, parent, "modeSelector" );

    //these have to be in order :) :(
    modeSelector->insertItem("Desktop");
    modeSelector->insertItem("Animation");
    modeSelector->insertItem("Effects");
    modeSelector->insertItem("Editing");
    modeSelector->insertItem("Paint");
    modeSelector->insertItem("Text CG");
    modeSelector->insertItem("Colorize");
    modeSelector->insertItem("Keyer");
    modeSelector->insertItem("Tracker");
    modeSelector->insertItem("Player");
    //modeSelector->insertItem("Encoding");
    //modeSelector->insertItem("Library");
    
    modeSelector->setCurrentItem(0); //DESKTOP
    connect( modeSelector,  SIGNAL(activated(int)), this, SLOT(switchModule(int))  );

    if ( !parent->isHidden() )
    {
        modeSelector->show();
    }
}

//this is a bad hack but it works
void JahControl::switchModule(int)
{
    QString modText = modeSelector->currentText();
    switchModule(modText);
}

void JahControl::buildDropDownMenus( QMenuBar* menu )
{
    
    //set up translator
    //JahTranslate& jt = JahTranslate::getInstance();
    
    ////////////////////////////////////////////////////////////////
    //ok.. so file and edit should be on all screens
    menufile = new QPopupMenu( menu );
    createFileMenu( menufile );
    menu->insertItem( "&"+jt->tr("File"), menufile );
    
    menuedit = new QPopupMenu( menu );
    createEditMenu( menuedit );
    menu->insertItem( "&"+jt->tr("Edit"), menuedit );
    
    /////////////////////////////////////////////////////////////////
    // menus for the modules
    /////////////////////////////////////////////////////////////////
    
    //have the modules create their own menus from now on!
    //we can also consolidate this code in the jahmodules.cpp file...
    
    //////////////////////////////////////////////////////////////
    //ui code for menu items
    menuOptions[DESKTOP] = new QPopupMenu( menu );
    JahDesktopMod->createMenuItem(menuOptions[DESKTOP]); //pass this as function ?
    menuOptionNames[ DESKTOP ] = JahDesktopMod->getModuleName();
    
    menuOptions[ANIMATION] = new QPopupMenu( menu );
    JahAnimationMod->createMenuItem(menuOptions[ANIMATION]); //pass this as function ?
    menuOptionNames[ ANIMATION ] = JahAnimationMod->getModuleName();

    menuOptions[EFFECTS] = new QPopupMenu( menu );
    JahEffectsMod->createMenuItem(menuOptions[EFFECTS]); //pass this as function ?
    menuOptionNames[ EFFECTS ] = JahEffectsMod->getModuleName();    

    menuOptions[EDITING] = new QPopupMenu( menu );
    JahEditingMod->createMenuItem(menuOptions[EDITING]); //pass this as function ?
    menuOptionNames[ EDITING ] = JahEditingMod->getModuleName();    

    menuOptions[PAINT] = new QPopupMenu( menu );
    JahPaintMod->createMenuItem(menuOptions[PAINT]); //pass this as function ?
    menuOptionNames[ PAINT ] = JahPaintMod->getModuleName();    

    menuOptions[TEXTCG] = new QPopupMenu( menu );
    JahTextMod->createMenuItem(menuOptions[TEXTCG]); //pass this as function ?
    menuOptionNames[ TEXTCG ] = JahTextMod->getModuleName();

    menuOptions[COLORIZE] = new QPopupMenu( menu );
    JahColorMod->createMenuItem(menuOptions[COLORIZE]); //pass this as function ?
    menuOptionNames[ COLORIZE ] = JahColorMod->getModuleName();

    menuOptions[TRACKER] = new QPopupMenu( menu );
    m_pJahTrackerMod->createMenuItem(menuOptions[TRACKER]); //pass this as function ?
    menuOptionNames[ TRACKER ] = m_pJahTrackerMod->getModuleName();    

    menuOptions[KEYER] = new QPopupMenu( menu );
    JahKeyerMod->createMenuItem(menuOptions[KEYER]); //pass this as function ?
    menuOptionNames[ KEYER ] = JahKeyerMod->getModuleName();

    menuOptions[PLAYER] = new QPopupMenu( menu );
    JahPlayerMod->createMenuItem(menuOptions[PLAYER]); //pass this as function ?
    menuOptionNames[ PLAYER ] = JahPlayerMod->getModuleName();

    menuOptions[LIBRARY] = new QPopupMenu( menu );
    JahLibraryMod->createMenuItem(menuOptions[LIBRARY]); //pass this as function ?
    menuOptionNames[ LIBRARY ] = JahLibraryMod->getModuleName();    

    menuOptions[ENCODING] = new QPopupMenu( menu );
    JahEncoderMod->createMenuItem(menuOptions[ENCODING]); //pass this as function ?
    menuOptionNames[ ENCODING ] = JahEncoderMod->getModuleName();

    ////////////////////////////////////////////////////
    //now add help menu
    menuhelp = new QPopupMenu( menu );
    createHelpMenu(menuhelp);
    menu->insertItem("&"+jt->tr("Help"), menuhelp );

    /// and select the desktop by default
    raiseMenu( DESKTOP );
}


/////////////////////////////////////////////////////
//some slots for the drop down menus

void JahControl::createHelpMenu(QPopupMenu * themenu)
{
    Q_CHECK_PTR(themenu);
    themenu->insertItem("&"+jt->tr("Help") );
}

void JahControl::createEditMenu(QPopupMenu * themenu)
{
    Q_CHECK_PTR(themenu);
    themenu->insertItem("&"+jt->tr("Edit") );
}

void JahControl::createFileMenu(QPopupMenu * themenu)
{
    
    
    /* XPM */
    static const char * p1_xpm[] = {
        "16 16 3 1",
        "       c None",
        ".      c #000000000000",
        "X      c #FFFFFFFF0000",
        "                ",
        "                ",
        "         ....   ",
        "        .XXXX.  ",
        " .............. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .XXXXXXXXXXXX. ",
        " .............. ",
        "                "};
    
    /* XPM */
    static const char * p2_xpm[] = {
        "16 16 3 1",
        "       c None",
        ".      c #000000000000",
        "X      c #FFFFFFFFFFFF",
        "                ",
        "   ......       ",
        "   .XXX.X.      ",
        "   .XXX.XX.     ",
        "   .XXX.XXX.    ",
        "   .XXX.....    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .XXXXXXX.    ",
        "   .........    ",
        "                ",
        "                "};
    
    /* XPM */
    static const char * p3_xpm[] = {
        "16 16 3 1",
        "       c None",
        ".      c #000000000000",
        "X      c #FFFFFFFFFFFF",
        "                ",
        "                ",
        "   .........    ",
        "  ...........   ",
        "  ........ ..   ",
        "  ...........   ",
        "  ...........   ",
        "  ...........   ",
        "  ...........   ",
        "  ...XXXXX...   ",
        "  ...XXXXX...   ",
        "  ...XXXXX...   ",
        "  ...XXXXX...   ",
        "   .........    ",
        "                ",
        "                "};
    
    //for the menus if they are activated
    //need to make this conditional
    //and need to add as a subroutine
    QPixmap p1( p1_xpm );
    QPixmap p2( p2_xpm );
    QPixmap p3( p3_xpm );
    
    Q_CHECK_PTR(themenu);
    themenu->insertItem(p1,"&"+jt->tr("Load"), this, SLOT(slotSmartLoader()), CTRL+Key_L,1 );
    
    //this should be greyed out unless in a DCC moudle
    themenu->insertItem(p3,"&"+jt->tr("Save"), this, SLOT(slotSmartSaver()), CTRL+Key_S,2 );
    themenu->setItemEnabled(2,false);
    themenu->insertItem(p3,"&"+jt->tr("Save As"), this, SLOT(slotSaveSceneAs()), CTRL+Key_S,3 );
    themenu->setItemEnabled(3,false);

    themenu->insertSeparator();
	
	//add export media option

    themenu->insertItem("&"+jt->tr("Append Scene"),this,SLOT( SceneAppendScene() ),CTRL+Key_S,4 );
    themenu->setItemEnabled(4,false);

    themenu->insertItem("&"+jt->tr("Export Scene"),this,SLOT( ExportMedia() ), CTRL+Key_S,5 );	
    themenu->setItemEnabled(5,false);

    themenu->insertSeparator();

	themenu->insertItem("&"+jt->tr("Export Media"),this,SLOT( PackageSaveScene() ), CTRL+Key_S,6 );	
    themenu->setItemEnabled(6,false);

    themenu->insertSeparator();

	themenu->insertItem(""+jt->tr("Preferences"), this, SLOT(changeModePref()), CTRL+Key_P,7);
    themenu->insertSeparator();
    themenu->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X,8);
}

//used to raise the menu
//the menu items should really correspond with the module number enums
//to automate this process
void JahControl::raiseMenu( Module m )
{
    menu->removeItem( moduleMenuId );

	// Temporary hack
	if ( m == NETWORK ) return;

    moduleMenuId = menu->insertItem( "&"+jt->tr( menuOptionNames[ m ] ), menuOptions[ m ] );
    
    switch( m ) 
    {
        case ANIMATION: 
        case EDITING: 
        case EFFECTS: 
        case TEXTCG: 
        case COLORIZE: 
        case TRACKER: 
        case KEYER: 
        { 
            menufile->setItemEnabled(2,true);
            break; 
        }

        case PAINT: 
		case PLAYER: 
        case LIBRARY: 
        case ENCODING: 
        case DESKTOP: 
        {  
            menufile->setItemEnabled(2,false);  //file save option
            break; 
        }
                    
        default: 
        {
            jtrace->debug("raiseMenu()","unsupported module");
            break;
        }
    }
}


void JahControl::selectMenuDropdown(Module mod)
{
    
    
    switch(mod) 
    {    
    case DESKTOP      : {  modeSelector->setCurrentItem(0); break; }
    case ANIMATION    : {  modeSelector->setCurrentItem(1); break; }
    case EDITING      : {  modeSelector->setCurrentItem(3); break; }
    case EFFECTS      : {  modeSelector->setCurrentItem(2); break; }
    case TEXTCG       : {  modeSelector->setCurrentItem(5); break; }
    case PAINT        : {  modeSelector->setCurrentItem(4); break; }
    case COLORIZE     : {  modeSelector->setCurrentItem(6); break; }
    case TRACKER      : {  modeSelector->setCurrentItem(8); break; }
    case KEYER        : {  modeSelector->setCurrentItem(7); break; }
    case PLAYER       : {  modeSelector->setCurrentItem(9); break; }
    case ENCODING     : {  modeSelector->setCurrentItem(10); break; }
    case LIBRARY      : {  modeSelector->setCurrentItem(11); break; }
        
    default           : {
        jtrace->debug("raiseMenu()","unsupported module");
        break;
    }
    }
}


