/*******************************************************************************
**
** The source file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahfileframe.h"


static const char* cdtoparent_xpm[]={
    "15 13 3 1",
    ". c None",
    "* c #000000",
    "a c #ffff99",
    "..*****........",
    ".*aaaaa*.......",
    "***************",
    "*aaaaaaaaaaaaa*",
    "*aaaa*aaaaaaaa*",
    "*aaa***aaaaaaa*",
    "*aa*****aaaaaa*",
    "*aaaa*aaaaaaaa*",
    "*aaaa*aaaaaaaa*",
    "*aaaa******aaa*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "***************"};

static const char* newfolder_xpm[] = {
    "15 14 4 1",
    " 	c None",
    ".	c #000000",
    "+	c #FFFF00",
    "@	c #FFFFFF",
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

static const char* viewtext_xpm[]={
    "15 13 3 1",
    ". c None",
    "* c #000000",
    "a c #ffff99",
    "***************",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "*aa**aa******a*",
    "*aaaaaaaaaaaaa*",
    "*aa**aa******a*",
    "*aaaaaaaaaaaaa*",
    "*aa**aa******a*",
    "*aaaaaaaaaaaaa*",
    "*aa**aa******a*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "***************"};
    
static const char* viewimage_xpm[]={
    "15 13 3 1",
    ". c None",
    "* c #000000",
    "a c #ffff99",
    "***************",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaa*aaaaa*",
    "*aa*aaa*a*aaaa*",
    "*a*a*a*a*a*aaa*",
    "**aaa*aaaaa*aa*",
    "*aaa*aaaaaaa*a*",
    "***************",
    "*aaaaaaaaaaaaa*",
    "***************"};
    
    static const char* viewicon_xpm[]={
    "15 13 3 1",
    ". c None",
    "* c #000000",
    "a c #ffff99",
    "***************",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "*aa*********aa*",
    "*aa*aaaaaaa*aa*",
    "*aa*aaaaaaa*aa*",
    "*aa*aaaaaaa*aa*",
    "*aa*aaaaaaa*aa*",
    "*aa*aaaaaaa*aa*",
    "*aa*********aa*",
    "*aaaaaaaaaaaaa*",
    "*aaaaaaaaaaaaa*",
    "***************"};

FileMainFrame::FileMainFrame( QWidget* parent, const char* name, bool, WFlags )
    : QWidget( parent, name )

{

    mainlibraryLayout = new QHBoxLayout( parent, 11, 6, "mainworldLayout");
    thelibrary = new QFrame( parent, "library" );
    mainlibraryLayout->addWidget( thelibrary );

    //create main grid layout
    Form1Layout = new QGridLayout( thelibrary, 1, 1, 1, 1, "Form1Layout"); // 11,6

    //titlebar = new QFrame( this, "titlebar" );
    titlebar = new QFrame( thelibrary, "titlebar" );
    titlebar->setMaximumSize( QSize( 32767, 32 ) );
    titlebar->setFrameShape( QFrame::StyledPanel );
    titlebar->setFrameShadow( QFrame::Raised );

    titlebarLayout = new QGridLayout( titlebar, 1, 1, 1, 1, "titlebarLayout"); 

	//////////////////////////////////////////////////////
	//we can use this for a history on linux?
    driveCombo = new QComboBox( titlebar, "comboBox3" );
    driveCombo->setEditable( TRUE );
    driveCombo->setName("Drives" );
    driveCombo->setMinimumSize( QSize( 80, 28 ) );
	
	//connect this widget to a slot to deal with changes in it
    connect( driveCombo,  SIGNAL(activated(int)), this, SLOT(changeWinDir(int))  );

    titlebarLayout->addWidget( driveCombo, 1, 1 );


    QLabel * textLabel1 = new QLabel( titlebar, "textLabel1" );
    textLabel1->setMinimumSize( QSize( 64, 0 ) );
    textLabel1->setMaximumSize( QSize( 64, 32767 ) );
    textLabel1->setText( "Path:" );
    titlebarLayout->addWidget( textLabel1, 1, 2 );

    //pathCombo = new QComboBox( TRUE, titlebar, "comboBox3" );
    pathCombo = new QComboBox( titlebar, "comboBox3" );
    pathCombo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, pathCombo->sizePolicy().hasHeightForWidth() ) );
    pathCombo->setEditable( TRUE );
    //pathCombo->setAutoCompletion( TRUE );
    //pathCombo->setMinimumSize( QSize( 400, 28 ) );

    //connect pathCombo return to a slolt...
    
    titlebarLayout->addWidget( pathCombo, 1, 3 );
    
    QSpacerItem* spacer = new QSpacerItem( 35, 20, QSizePolicy::Fixed, QSizePolicy::Minimum );
    titlebarLayout->addItem( spacer, 1, 4 );

    QPixmap pix;

    pix = QPixmap( cdtoparent_xpm );
    upButton = new QToolButton( titlebar, "One directory up" );
    upButton->setIconSet(pix);
    upButton->setMinimumSize( QSize( 32, 0 ) );
    upButton->setMaximumSize( QSize( 32, 32767 ) );
    titlebarLayout->addMultiCellWidget( upButton, 0, 1, 5, 5 );

    pix = QPixmap( newfolder_xpm );
    mkdirButton = new QToolButton( titlebar, "New Folder" );
    mkdirButton->setIconSet(pix);    
    mkdirButton->setMinimumSize( QSize( 32, 0 ) );
    mkdirButton->setMaximumSize( QSize( 32, 32767 ) );
    titlebarLayout->addMultiCellWidget( mkdirButton, 0, 1, 6, 6 );
    
    connect( upButton, SIGNAL( clicked() ),    this, SLOT( cdUp() ) );
    //connect( mkdirButton, SIGNAL( clicked() ), this, SLOT( newFolder() ) );
    
    //the view modes
    pix = QPixmap( viewicon_xpm );
    viewIcon = new QToolButton( titlebar, "view icons" );
    viewIcon->setIconSet(pix);
    viewIcon->setMinimumSize( QSize( 32, 0 ) );
    viewIcon->setMaximumSize( QSize( 32, 32767 ) );
    titlebarLayout->addMultiCellWidget( viewIcon, 0, 1, 7, 7 );

    pix = QPixmap( viewimage_xpm );
    viewImage = new QToolButton( titlebar, "view images" );
    viewImage->setIconSet(pix);    
    viewImage->setMinimumSize( QSize( 32, 0 ) );
    viewImage->setMaximumSize( QSize( 32, 32767 ) );
    titlebarLayout->addMultiCellWidget( viewImage, 0, 1, 8, 8 );

    pix = QPixmap( viewtext_xpm );
    viewText = new QToolButton( titlebar, "view text" );
    viewText->setIconSet(pix);    
    viewText->setMinimumSize( QSize( 32, 0 ) );
    viewText->setMaximumSize( QSize( 32, 32767 ) );
    titlebarLayout->addMultiCellWidget( viewText, 0, 1, 9, 9 );

    Form1Layout->addMultiCellWidget( titlebar, 0, 0, 0, 1 );
    
    QSplitter *preview = new QSplitter( thelibrary, "splitter" );
//    QSplitter *preview = new QSplitter( this, "splitter" );
    preview->setFrameShape( QFrame::StyledPanel );
    preview->setFrameShadow( QFrame::Raised );

    dirlist = new DirectoryView( preview, "dirlist", TRUE );
    dirlist->addColumn( "Name" );
    dirlist->addColumn( "Type" );
    dirlist->setMinimumSize( QSize( 220, 0 ) );
    dirlist->hide();

	////////////////////////////////////////////////
	// on windows its the current dir...
    JahPrefs& jprefs = JahPrefs::getInstance();
    QString JahMediaPath = jprefs.getMediaPath().data();

	//now we need to truncate the path to / on linux and C: or D: on windows

    root = new Directory( dirlist, JahMediaPath );

    root->setOpen( TRUE );
    preview->setResizeMode( dirlist, QSplitter::KeepSize );

    fileview = new QtFileIconView( preview,"preview",JahMediaPath );
    //fileview = new QtFileIconView( "/", preview );
    fileview->setSelectionMode( QIconView::Extended );
    
    //add the preview pane
    Form1Layout->addWidget( preview, 1, 1 );

    connect( dirlist,  SIGNAL( folderSelected( const QString & ) ),   fileview, SLOT ( setDirectory( const QString & ) ) );
    connect( fileview, SIGNAL( directoryChanged( const QString & ) ), this, SLOT( directoryChanged( const QString & ) ) );
    connect( fileview, SIGNAL( startReadDir( int ) ), this, SLOT( slotStartReadDir( int ) ) );
    connect( fileview, SIGNAL( readNextDir() ),       this, SLOT( slotReadNextDir() ) );
    connect( fileview, SIGNAL( readDirDone() ),       this, SLOT( slotReadDirDone() ) );
    
    connect( viewIcon,  SIGNAL( clicked() ),    fileview, SLOT( viewLarge() ) );
    connect( viewImage, SIGNAL( clicked() ),    fileview, SLOT( viewImage() ) );
    connect( viewText,  SIGNAL( clicked() ),    fileview, SLOT( viewSmall() ) );

    //need to track when the user hits return here
    //its not working properly
    connect( pathCombo, SIGNAL( textChanged(const QString &) ),       this, SLOT( textWasChanged(const QString &) ) );
         
    feedback = new QFrame( thelibrary, "feedback" );
//    feedback = new QFrame( this, "feedback" );
    feedback->setMaximumSize( QSize( 32767, 32 ) );
    feedback->setFrameShape( QFrame::StyledPanel );
    feedback->setFrameShadow( QFrame::Raised );

  
    //create main grid layout
    Form2Layout = new QGridLayout( feedback, 1, 4, 1, 1, "Form1Layout"); 

    label = new QLabel( feedback );
    Form2Layout->addWidget( label, 1, 1 );

    progress = new QProgressBar( feedback );
    Form2Layout->addWidget( progress, 1, 2 );
    
    loadbutton = new QPushButton( feedback );
    loadbutton->setText( "Load" );
    Form2Layout->addWidget( loadbutton, 1, 3 );
    connect( loadbutton, SIGNAL( clicked() ),    this, SLOT( returnFile() ) );

    closebutton = new QPushButton( feedback );
    closebutton->setText( "Close" );
    Form2Layout->addWidget( closebutton, 1, 4 );
    connect( closebutton, SIGNAL( clicked() ),    this, SLOT( returnNull() ) );

       
    Form1Layout->addMultiCellWidget( feedback, 2, 2, 0, 1 );
    
    connect( fileview, SIGNAL( enableUp() ),    this, SLOT( enableUp() ) );
    connect( fileview, SIGNAL( disableUp() ),   this, SLOT( disableUp() ) );
    connect( fileview, SIGNAL( enableMkdir() ), this, SLOT( enableMkdir() ) );
    connect( fileview, SIGNAL( disableMkdir() ),this, SLOT( disableMkdir() ) ); 
    
    //connect the double click ona file to exit and load routine
    connect( fileview, SIGNAL( loadthisfile(QString) ),this, SLOT( loadthisfile(QString) ) ); 
    connect( fileview, SIGNAL( setfilename(QString) ), this, SLOT( setthefilename(QString) ) );   

    languageChange();
    

	////////////////////////////////////////////////////
	//this routine loads up the drive dropdown
	//only needed on windows

	//#ifdef WIN32
	//debug("directory stuff");

    QDir d;
        
    const QFileInfoList* roots = QDir::drives();
    QPtrListIterator<QFileInfo> i(*roots);
    QFileInfo* fi;
    while ( (fi = *i) ) {
        ++i;
		if (fi->isDir())
		{
			driveCombo->insertItem(fi->filePath());
			//debug(fi->filePath() );
		}
	}
	driveCombo->insertItem("Media");
	//need to connect driveCombo to slot to deal with this as well
	//#endif

	////////////////////////////////////////////////////
	//resize to fit window? layout should auto resize this for us
    //resize( QSize(745, 481).expandedTo(minimumSizeHint()) );
}

FileMainFrame::~FileMainFrame()
{
	delete fileview;
}

/*
void FileMainFrame::show()
{
    QMainWindow::show();
}
*/
void FileMainFrame::setup()
{
 
}

void FileMainFrame::changeWinDir(int drive)
{
    JahPrefs& jprefs = JahPrefs::getInstance();

	QString thedrive = driveCombo->text(drive);
	
	if (thedrive=="Media")
	{
		thedrive = jprefs.getMediaPath().data();
		thedrive += "/media/";
	}
	else
	{
		dirlist->setDir( thedrive);
	}

    delete root;
	root = new Directory( dirlist, thedrive );
	root->setOpen( TRUE );

	changePath(thedrive);
	textWasChanged(thedrive);

}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FileMainFrame::languageChange()
{
    setCaption( tr( "JahLoader" ) );
}

void FileMainFrame::setPathCombo()
{
    QString dir = caption();
    int i = 0;
    bool found = FALSE;
    for ( i = 0; i < pathCombo->count(); ++i ) {
	if ( pathCombo->text( i ) == dir) {
	    found = TRUE;
	    break;
	}
    }

    if ( found )
	pathCombo->setCurrentItem( i );
    else {
	pathCombo->insertItem( dir );
	pathCombo->setCurrentItem( pathCombo->count() - 1 );
    }

}

void FileMainFrame::textWasChanged(const QString & thedir)
{
    //make sure we are not checking against the slash as it causes problems on linux
    if (thedir.right(1)!="/")
    {
        //oly if text is a directory
        if ( QFileInfo( thedir ).exists() )
        {
            QDir currentdir = fileview->currentDir();
            QString olddir = currentdir.path();

            //before changing make sure its not the curent directory
            if (thedir != olddir )
            {
                fileview->setDirectory( thedir );

            }
        }
    }
    
}

void FileMainFrame::returnFile( void )
{ 
	QtFileIconView *files = fileView( );
	const QtFileIconViewItem *item = ( const QtFileIconViewItem * )files->firstItem( );
	m_selected_items.clear( );

	for ( item = ( const QtFileIconViewItem * )files->firstItem( ); 
		  item != NULL; 
		  item = ( const QtFileIconViewItem * )item->nextItem( ) )
		if ( item->isSelected( ) && QFileInfo( item->filename( ) ).exists( ) )
			m_selected_items.append( item->filename( ) );

	emit loadButton();
}

void FileMainFrame::returnNull( void )
{    
}

void FileMainFrame::loadthisfile( QString )
{
	returnFile( );
}

void FileMainFrame::setthefilename( QString )
{
}

void FileMainFrame::directoryChanged( const QString &dir )
{
    setCaption( dir );
    setPathCombo();
}

void FileMainFrame::slotStartReadDir( int dirs )
{
    label->setText( tr( " Reading Directory..." ) );
    progress->reset();
    progress->setTotalSteps( dirs );
}

void FileMainFrame::slotReadNextDir()
{
    int p = progress->progress();
    progress->setProgress( ++p );
}

void FileMainFrame::slotReadDirDone()
{
    label->setText( tr( " Reading Directory Done." ) );
    progress->setProgress( progress->totalSteps() );
}

void FileMainFrame::cdUp()
{
    QDir dir = fileview->currentDir();
    dir.cd( ".." );
    fileview->setDirectory( dir );
}

void FileMainFrame::newFolder()
{
    fileview->newDirectory();
}

void FileMainFrame::changePath( const QString &path )
{
    if ( QFileInfo( path ).exists() )
	fileview->setDirectory( path );
    else
	setPathCombo();
}

void FileMainFrame::enableUp()
{
    upButton->setEnabled( TRUE );
}

void FileMainFrame::disableUp()
{
    upButton->setEnabled( FALSE );
}

void FileMainFrame::enableMkdir()
{
    mkdirButton->setEnabled( TRUE );
}

void FileMainFrame::disableMkdir()
{
    mkdirButton->setEnabled( FALSE );
}
