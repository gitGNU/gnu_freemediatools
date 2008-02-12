/*******************************************************************************
**
** The source file for the Jahshaka jahdesktop code
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

////////////////////////////////////////////////////////////////
// This is the file that sets up  the desktop interface
// and connects it to the routines

#include "jahplayercreate.h"
#include <qcheckbox.h>
#include <qslider.h>
#include <widget.h>

void JahControl::CreateDesktopMiddleFrame(QHBox* frame) 
{
    //set up translator
    //JahTranslate& jt = JahTranslate::getInstance();

    page = new QWidget( frame, "page" );
    pageLayout = new QHBoxLayout( page, 0, 0, "pageLayout");
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout->addItem( spacer );

    modeselectframe = new QFrame( page, "modeselectframe" );
    modeselectframe->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, modeselectframe->sizePolicy().hasHeightForWidth() ) );
    modeselectframe->setMinimumSize( QSize( 127, 20 ) );
    modeselectframe->setMaximumSize( QSize( 127, 20 ) );
    modeselectframe->setFrameShape( QFrame::Box );
    modeselectframe->setFrameShadow( QFrame::Plain );

    //modeselectbutton
    lockbutton = new QPushButton( modeselectframe, "modeselectbutton" );
    lockbutton->setGeometry( QRect( 0, 0, 40, 20 ) );
    lockbutton->setMinimumSize( QSize( 40, 20 ) );
    lockbutton->setMaximumSize( QSize( 40, 20 ) );
    lockbutton->setText( tr( "lock"  ) );
    //connect( lockbutton,  SIGNAL(clicked()), SLOT( switchMode())  );

    expandbutton = new QPushButton( modeselectframe, "modeselectbutton" );
    expandbutton->setGeometry( QRect( 41, 0, 45, 20 ) );
    expandbutton->setMinimumSize( QSize( 45, 20 ) );
    expandbutton->setMaximumSize( QSize( 45, 20 ) );
    expandbutton->setText( tr( "expand"  ) );
    //expandbutton->setFrameShadow( QFrame::Raised );
    connect( expandbutton,  SIGNAL(clicked()), JahDesktopMod, SLOT( slotExpand())  );

    sharebutton = new QPushButton( modeselectframe, "modeselectbutton" );
    sharebutton->setGeometry( QRect( 87, 0, 40, 20 ) );
    sharebutton->setMinimumSize( QSize( 40, 20 ) );
    sharebutton->setMaximumSize( QSize( 40, 20 ) );
    sharebutton->setText( tr( "share"  ) );
    //connect( sharebutton,  SIGNAL(clicked()), SLOT( switchMode())  );


    pageLayout->addWidget( modeselectframe );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout->addItem( spacer_2 );

    /////////////////////////////////////////////////////////////////////////////
    // the main desktop buttons

    buttonframe = new QFrame( page, "buttonframe" );
    buttonframe->setMinimumSize( QSize( 336, 30 ) );
    buttonframe->setMaximumSize( QSize( 336, 30 ) );
    buttonframe->setFrameShape( QFrame::NoFrame );
    buttonframe->setFrameShadow( QFrame::Plain );

    DesktopName = new JahToolButton( buttonframe, "desktopnamebutton" );
    JahFormatter::addJahPlayerButton( DesktopName, 0, 3, 84, 26,   84, 26, JahBasePath+"Pixmaps/jahplayer/playername.png", 
                                                            JahBasePath+"Pixmaps/jahplayer/playernameon.png" );
    connect ( DesktopName,    SIGNAL(clicked()), this, SLOT( nameClip()) );
    popup2 = new FancyPopup( buttonframe, "Enter Clipname",250,130 );  //send size and position as well
    connect ( popup2,      SIGNAL( returnText(QString)), this, SLOT( renameDesktopAsset(QString)) );


    DesktopLoad = new JahToolButton( buttonframe, "desktopnamebutton" );
    JahFormatter::addJahPlayerButton( DesktopLoad, 84, 3, 84, 26,   84, 26, JahBasePath+"Pixmaps/jahplayer/playerload.png", 
                                                                JahBasePath+"Pixmaps/jahplayer/playerloadon.png" );
    connect( DesktopLoad,  SIGNAL(clicked()), JahDesktopMod, SLOT(slotLoadIconview())  );  //this slot is located in desktop.h

    DesktopPlay = new JahToolButton( buttonframe, "desktopnamebutton" );
    JahFormatter::addJahPlayerButton( DesktopPlay, 168, 3, 84, 26,   84, 26, JahBasePath+"Pixmaps/jahplayer/playerplay.png", 
                                                                JahBasePath+"Pixmaps/jahplayer/playerplayon.png" );
    connect( DesktopPlay,  SIGNAL(clicked()), SLOT(changeModePlayer())  );    //this is located in jahcontrol

    DeleteButton = new JahToolButton( buttonframe, "desktopnamebutton" );
    JahFormatter::addJahPlayerButton( DeleteButton, 252, 3, 84, 26,   84, 26, JahBasePath+"Pixmaps/jahplayer/playerdelete.png", 
                                                                JahBasePath+"Pixmaps/jahplayer/playerdeleteon.png" );
    connect ( DeleteButton,  SIGNAL(clicked()), JahDesktopMod, SLOT( clearItem()) );

    pageLayout->addWidget( buttonframe );

    ////////////////////////////////////////////////////////////////////

    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout->addItem( spacer_3 );

    desktopselectframe = new QFrame( page, "desktopselectframe" );
    desktopselectframe->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, desktopselectframe->sizePolicy().hasHeightForWidth() ) );
    desktopselectframe->setMinimumSize( QSize( 120, 20 ) );
    desktopselectframe->setMaximumSize( QSize( 120, 20 ) );
    desktopselectframe->setFrameShape( QFrame::StyledPanel );
    desktopselectframe->setFrameShadow( QFrame::Raised );


    desktopselectbutton = new QComboBox( FALSE, desktopselectframe, "renderercombo" );
    desktopselectbutton->setGeometry( QRect( 0, 0, 120, 20 ) );
    desktopselectbutton->insertItem("Desktop");
    desktopselectbutton->insertItem(">New Desktop");
    desktopselectbutton->insertItem(">Del Desktop");
    desktopselectbutton->setCurrentItem(0); //4:2:0
//  connect( desktopselectbutton,  SIGNAL(activated(int)), this, SLOT(setRenderer(int))  );

    pageLayout->addWidget( desktopselectframe );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    pageLayout->addItem( spacer_4 );

}

////////////////////////////////////////////////////////////////
//icon for desktop preferences locate jahpath
static const char* basefolder_xpm[] = {
    "15 14 4 1",
    "   c None",
    ".  c #000000",
    "+  c #FFFF00",
    "@  c #FFFFFF",
    "          .    ",
    "               ",
    "          .    ",
    "       .     . ",
    "  ....  . . .  ",
    " .+@+@.  . .   ",
    "..........  . .",
    ".@+@+@+@+@..   ",
    ".+@+@+@+@+. .  ",
    ".@+@+@+@+@.  . ",
    ".+@+@+@+@+.    ",
    ".@+@+@+@+@.    ",
    ".+@+@+@+@+.    ",
    "...........    "};

void JahControl::CreateDesktopPreferences(QHBox* frame)
{
    JahPrefs& jprefs    = JahPrefs::getInstance();
    //set up translator
    JahTranslate* jt = JahTranslate::getInstance();

    prefframe1 = new QFrame( frame, "prefframe1" );
    prefframe1->setGeometry( QRect( 0, 40, 860, 200 ) );
    //prefframe1->setFrameShape( QFrame::StyledPanel );
    //prefframe1->setFrameShadow( QFrame::Raised );

    //////////////////////////////////////////////////////////////////
    
	//add set style text here
    stylelabel = new QLabel( prefframe1, "storagelabel" );
    stylelabel->setGeometry( QRect( 19, 11, 151, 21 ) );
    stylelabel->setAlignment( int( QLabel::AlignCenter ) );
    stylelabel->setText( jt->tr( "Modify Style" ) );

    stylebutton = new QPushButton( prefframe1, "Pref1" );
    stylebutton->setGeometry( QRect( 19, 30, 145, 21 ) );
    stylebutton->setText( tr( "Set Style" ) );
    connect( stylebutton, SIGNAL(clicked()),  SLOT(setJahTheme()) );
    
    
	////////////////////////////////////////////////////////////////
    colorlabel = new QLabel( prefframe1, "storagelabel" );
    colorlabel->setGeometry( QRect( 19, 70, 151, 21 ) );
    colorlabel->setAlignment( int( QLabel::AlignCenter ) );
    colorlabel->setText( jt->tr( "Modify Color" ) );


    Pref5 = new QPushButton( prefframe1, "Pref5" );
    Pref5->setGeometry( QRect( 19, 91, 70, 21 ) );
    connect( Pref5, SIGNAL(clicked()),  SLOT(setstyleGrey()) );

    Pref6 = new QPushButton( prefframe1, "Pref6" );
    Pref6->setGeometry( QRect( 19, 121, 70, 21 ) );
    connect( Pref6, SIGNAL(clicked()),  SLOT(setstyleBlue()) );

    Pref7 = new QPushButton( prefframe1, "Pref7" );
    Pref7->setGeometry( QRect( 99, 91, 70, 21 ) );
    connect( Pref7,  SIGNAL(clicked()), SLOT(setstyleBlack()) );

    Pref8 = new QPushButton( prefframe1, "Pref8" );
    Pref8->setGeometry( QRect( 99, 121, 70, 21 ) );
    connect( Pref8,  SIGNAL(clicked()), SLOT(setstyleWhite()) );  //change to signal...?

	//this is the remember prefs option
    Pref10 = new QCheckBox( prefframe1, "Pref10" );
    Pref10->setGeometry( QRect( 19, 160, 140, 21 ) );
    Pref10->setChecked(jahStyleprefs);
    connect( Pref10,  SIGNAL(clicked()), this, SLOT(setStyleOption())  );


    uiframe1 = new QFrame( prefframe1, "uiframe1" );
    uiframe1->setGeometry( QRect( 190, 10, 2, 180 ) );
    uiframe1->setFrameShape( QFrame::StyledPanel );
    uiframe1->setFrameShadow( QFrame::Raised );

	////////////////////////////////////////////////////////

    StorageBox = new QLineEdit( prefframe1, "StorageBox" );
    StorageBox->setGeometry( QRect( 210, 40, 181, 21 ) );
    StorageBox->setText( JahMediaPath );

    StorageButton = new QPushButton( prefframe1, "StorageButton" );
    StorageButton->setGeometry( QRect( 210, 71, 70, 21 ) );
    connect( StorageButton, SIGNAL(clicked()),  SLOT(setStoragePath()) );

    QPixmap pix; pix = QPixmap( basefolder_xpm );

    StorageLocation = new QToolButton( prefframe1, "StorageLocation" );
    StorageLocation->setGeometry( QRect( 400, 40, 31, 21 ) );
    StorageLocation->setIconSet(pix);
    connect( StorageLocation , SIGNAL(clicked()),  SLOT(getStoragePath()) );

    storagelabel = new QLabel( prefframe1, "storagelabel" );
    storagelabel->setGeometry( QRect( 289, 11, 151, 21 ) );
    storagelabel->setAlignment( int( QLabel::AlignCenter ) );
    storagelabel->setText( jt->tr( "User Storage" ) );

    renderqualityslider = new QSlider( prefframe1, "renderqualityslider" );
    renderqualityslider->setGeometry( QRect( 280, 130, 140, 21 ) );
    renderqualityslider->setOrientation( QSlider::Horizontal );

    renderqualityslider->setOrientation( QSlider::Horizontal );
    renderqualityslider->setTickmarks( QSlider::Right );
    renderqualityslider->setMinValue( 0 );
    renderqualityslider->setMaxValue( 2 );

    //check prefs and read in renderquality
    renderqualityslider->setValue   ( jahrenderquality );
    connect( renderqualityslider,   SIGNAL(sliderReleased()),    SLOT(setGlobalRenderQuality()) );

    renderercombo = new QComboBox( FALSE, prefframe1, "renderercombo" );
    renderercombo->setGeometry( QRect( 280, 169, 141, 21 ) );
    renderercombo->insertItem("GL Renderer    (fast)");
    renderercombo->insertItem("Qt GL Renderer  (fast)");
    renderercombo->insertItem("Software        (slow)");
    renderercombo->insertItem("Qt Renderer Exp (fastest)");
    renderercombo->setCurrentItem(renderer); //4:2:0
    connect( renderercombo,  SIGNAL(activated(int)), this, SLOT(setRenderer(int))  );

    //we must add 3 options here for the 3 renereres

    renderlabel = new QLabel( prefframe1, "renderlabel" );
    renderlabel->setGeometry( QRect( 289, 111, 151, 21 ) );
    renderlabel->setAlignment( int( QLabel::AlignCenter ) );

    rendererlabel = new QLabel( prefframe1, "rendererlabel" );
    rendererlabel->setGeometry( QRect( 210, 169, 65, 21 ) );
    rendererlabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    renderqualitylabel = new QLabel( prefframe1, "renderqualitylabel" );
    renderqualitylabel->setGeometry( QRect( 210, 139, 65, 21 ) );
    renderqualitylabel->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );

    uiframe2 = new QFrame( prefframe1, "uiframe2" );
    uiframe2->setGeometry( QRect( 220, 105, 180, 2 ) );
    uiframe2->setFrameShape( QFrame::StyledPanel );
    uiframe2->setFrameShadow( QFrame::Raised );

    uiframe3 = new QFrame( prefframe1, "uiframe3" );
    uiframe3->setGeometry( QRect( 440, 9, 2, 180 ) );
    uiframe3->setFrameShape( QFrame::StyledPanel );
    uiframe3->setFrameShadow( QFrame::Raised );

    ////////////////////////////////////////////////////
    //insert http browsing preferences here

    httpdlabel = new QLabel( prefframe1, "renderlabel" );
    httpdlabel->setGeometry( QRect( 455, 10, 100, 21 ) );
    httpdlabel->setAlignment( int( QLabel::AlignCenter ) );

    // setup interface for Jah Httpd
    //need a qcheckbox for auto lunch here

    bindButton = new QPushButton("Start", prefframe1);
    bindButton->setGeometry( QRect( 450, 40, 70, 21 ) );

    //need to pull the default values from the preferences
    //instead of hardcoding them ie the "1" and "8080"
    backlogLineEdit = new QLineEdit("1", prefframe1);
    backlogLineEdit->setGeometry( QRect( 450, 70, 70, 21 ) );

    portLineEdit = new QLineEdit("8080", prefframe1);
    portLineEdit->setGeometry( QRect( 450, 100, 70, 21 ) );













    uiframe33 = new QFrame( prefframe1, "uiframe3" );
    uiframe33->setGeometry( QRect( 570, 9, 2, 180 ) );
    uiframe33->setFrameShape( QFrame::StyledPanel );
    uiframe33->setFrameShadow( QFrame::Raised );

    //////////////////////////////////////////////////////////////
    // ui settings here

    uisettingslabel = new QLabel( prefframe1, "uisettingslabel" );
    uisettingslabel->setGeometry( QRect( 580, 20, 120, 21 ) );
    uisettingslabel->setAlignment( int( QLabel::AlignCenter ) );

    PositionPrefs = new QCheckBox( prefframe1, "PositionPrefs" );
    PositionPrefs->setGeometry( QRect( 580, 50, 140, 20 ) );
    PositionPrefs->setChecked(rememberprefs);
    connect( PositionPrefs,  SIGNAL(clicked()), this, SLOT(setGlobalPositionSize())  );

    DualHead = new QCheckBox( prefframe1, "DualHead" );
    DualHead->setGeometry( QRect( 580, 80, 140, 20 ) );
    DualHead->setChecked(dualheadprefs);
    connect( DualHead,  SIGNAL(clicked()), this, SLOT(setDualHeadOption())  );

    VideoHead = new QCheckBox( prefframe1, "VideoHead" );
    VideoHead->setGeometry( QRect( 580, 110, 140, 20 ) );
    VideoHead->setChecked(videohead);
    connect( VideoHead,  SIGNAL(clicked()), this, SLOT(setVideoHeadOption())  );

    MenuOption = new QCheckBox( prefframe1, "MenuOption" );
    MenuOption->setGeometry( QRect( 580, 140, 140, 20 ) );
    MenuOption->setChecked(menuprefs);
    connect( MenuOption,  SIGNAL(clicked()), this, SLOT(setMenuOption())  );

    WideScreenBox = new QCheckBox( prefframe1, "WideScreen" );
    WideScreenBox->setGeometry( QRect( 580, 170, 140, 20 ) );
    WideScreenBox->setChecked(widescreenprefs);
    connect( WideScreenBox,  SIGNAL(clicked()), this, SLOT(setWidescreenOption())  );

    /////////////////////////////////////////////////////////////

    uiframe4 = new QFrame( prefframe1, "uiframe4" );
    uiframe4->setGeometry( QRect( 700, 9, 2, 180 ) );
    uiframe4->setFrameShape( QFrame::StyledPanel );
    uiframe4->setFrameShadow( QFrame::Raised );

    languagelabel = new QLabel( prefframe1, "languagelabel" );
    languagelabel->setGeometry( QRect( 710, 20, 120, 21 ) );
    languagelabel->setAlignment( int( QLabel::AlignCenter ) );

    LanguageCombo = new QComboBox( FALSE, prefframe1, "LanguageCombo" );
    LanguageCombo->setGeometry( QRect( 710, 45, 170, 21 ) );
    LanguageCombo->insertItem("English");
    LanguageCombo->insertItem("Spanish");
    LanguageCombo->insertItem("French");
    LanguageCombo->insertItem("Portuguese");
    LanguageCombo->insertItem("German");
    LanguageCombo->insertItem("Italian");
    LanguageCombo->setCurrentText(jprefs.getLanguage().c_str());
    connect( LanguageCombo,  SIGNAL(activated(const QString &)), this, SLOT(setLanguage(const QString &))  );
    jt->setLanguage(jprefs.getLanguage().c_str());

    jahreslabel = new QLabel( prefframe1, "languagelabel" );
    jahreslabel->setGeometry( QRect( 710, 75, 120, 21 ) );
    jahreslabel->setAlignment( int( QLabel::AlignCenter ) );

    JahresCombo = new QComboBox( FALSE, prefframe1, "JahresCombo" );
    JahresCombo->setGeometry( QRect( 710, 100, 170, 21 ) );

    projectData thedata;

    thedata.buildComboBox(JahresCombo);

    JahresCombo->setCurrentItem(JahDefaultRes); //should be languagepref
    connect( JahresCombo,  SIGNAL(activated(int)), this, SLOT(setResolution(int))  );

    uiframe6 = new QFrame( prefframe1, "uiframe6" );
    uiframe6->setGeometry( QRect( 730, 140, 130, 2 ) );
    uiframe6->setFrameShape( QFrame::StyledPanel );
    uiframe6->setFrameShadow( QFrame::Raised );

    PrefExit = new QPushButton( prefframe1, "PrefExit" );
    PrefExit->setGeometry( QRect( 760, 160, 70, 21 ) );
    connect( PrefExit,  SIGNAL(clicked()), SLOT(changeModePrefReturn())  );

    PreferenceslanguageChange();

}

void JahControl::PreferenceslanguageChange()
{

    Pref5->setText( jt->tr( "Grey" ) );
    Pref6->setText( jt->tr( "Blue" ) );
    Pref7->setText( jt->tr( "Black" ) );
    Pref8->setText( jt->tr( "White" ) );
    
    Pref10->setText( jt->tr( "Remember Style" ) );
    StorageLocation->setText( jt->tr( "Dir" ) );
    StorageButton->setText( jt->tr( "Set Path" ) );

    MenuOption->setText( jt->tr( "Use Menus" ) );
    DualHead->setText( jt->tr( "Dual Head" ) );
    VideoHead->setText( jt->tr( "Video Head" ) );
    WideScreenBox->setText( jt->tr( "Wide Screen" ) );
    PositionPrefs->setText( jt->tr( "Remember UI" ) );

    httpdlabel->setText( jt->tr( "Web Desktop" ) );
    PrefExit->setText( jt->tr( "RETURN" ) );

    //storagelabel->setText( jt->tr( "Media Storage" ) );
    renderlabel->setText( jt->tr( "Render Settings" ) );
    uisettingslabel->setText( jt->tr( "User Interface" ) );

    languagelabel->setText( jt->tr( "Language" ) );
    jahreslabel->setText( jt->tr( "Resolution" ) );

    rendererlabel->setText( jt->tr( "Renderer" ) );
    renderqualitylabel->setText( jt->tr( "Quality" ) );
}


