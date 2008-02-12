/*******************************************************************************
**
** The source file for the Jahshaka jahcreate module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the main class that creates and launches the jahshaka interface
// and sets up and connects all the modules and interface objects

#include "jahplayercreate.h"


void JahControl::buildMenuBar(QWidget *  parent)
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

    //set up translator
    //JahTranslate& jt = JahTranslate::getInstance();

    QMenuBar* menu = new QMenuBar ( parent );//menuBar();

    QPopupMenu* file = new QPopupMenu( menu );
    file->insertItem(p1,"&"+jt->tr("Load"), this, SLOT(slotLoadIconview()), CTRL+Key_L );
    //file->insertItem(p3,"&"+jt->tr("Save"), this, SLOT(slotSave()), CTRL+Key_S );
    file->insertSeparator();
    file->insertItem("&"+jt->tr("Import"), this, SLOT(slotLoadIconview()), CTRL+Key_I);
    file->insertItem("&"+jt->tr("Export"), this, SLOT(slotExport()), CTRL+Key_E);
    file->insertSeparator();
    file->insertItem(""+jt->tr("Preferences"), this, SLOT(changeModePref()), CTRL+Key_P);
    file->insertSeparator();
    file->insertItem("E&xit", qApp, SLOT(quit()), CTRL+Key_X);
    menu->insertItem("&"+jt->tr("File"), file);

    QPopupMenu* edit = new QPopupMenu( menu );
    edit->insertItem("&"+jt->tr("Desktop"), this, SLOT(changeModeDesktop()) );
    edit->insertItem("&"+jt->tr("Library"), this, SLOT(changeModeLibrary()) );
    edit->insertSeparator();
    edit->insertItem("&"+jt->tr("Animation"), this, SLOT(changeModeAnimation()) );
    edit->insertItem("&"+jt->tr("Player"), this, SLOT(changeModePlayer()) );
  
    menu->insertItem("&"+jt->tr("Modules"), edit);


    QPopupMenu* desk = new QPopupMenu( menu );
    desk->insertItem("&"+jt->tr("Load clip"), this, SLOT(slotLoadIconview()) );
    //desk->insertItem("&"+jt->tr("Save clip"), this, SLOT(slotSave()) );
    desk->insertSeparator();
    desk->insertItem(jt->tr("Play clip"), this, SLOT(changeModePlayer()) );
    desk->insertSeparator();
    desk->insertItem(jt->tr("Delete clip"), this, SLOT(clearItem()) );
    //desk->insertSeparator();
    //desk->insertItem("&"+jt->tr("View mode"), viewmode );
    desk->insertSeparator();
    desk->insertItem("&"+jt->tr("Tidy desktop"), this, SLOT(tidyDesktop()) );
    desk->insertItem("&"+jt->tr("Clear desktop"), this, SLOT(clearAll()) );
    //desk->insertItem("&Add desktop", this, SLOT(notImplemented()) );
    //desk->insertItem("&Switch desktop", this, SLOT(notImplemented()) );
    menu->insertItem("&"+jt->tr("Desktop"), desk);
}


/////////////////////////////////////////////////////
//some slots for the drop down menus
//since 'thedesktop' doesnt seem to work for them

void JahControl::slotLoadIconview(void)
{
    JahDesktopMod->slotLoadIconview();
}

void JahControl::slotExport(void)
{
    JahDesktopMod->slotExport();
}

void JahControl::slotImport(void)
{
    JahDesktopMod->slotImport();
}

void JahControl::clearItem(void)
{
    JahDesktopMod->clearItem();
}

void JahControl::clearAll(void)
{
    JahDesktopMod->clearAll();
}

void JahControl::tidyDesktop(void)
{
    JahDesktopMod->tidyDesktop();
}

