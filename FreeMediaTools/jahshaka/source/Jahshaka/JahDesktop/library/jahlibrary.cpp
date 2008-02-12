/*******************************************************************************
**
** The source file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahlibrary.h"

//custom icons
#include "jahlibrarywidgets.h"

#include "Asset.h" 
#include "assetmanmainwidget.h" // from uic

void JahLibrary::activate( )
{
	if ( !m_started )
	{
		m_started = true;
		fileview = new QtFileIconView( this );
		fileview->setDirectory( directory );
		buildInterface( 0 );
		buildInterfaceMenus( m_leftcontroller, m_rightcontroller );
		initializeModule( m_parent, m_controls, m_leftframe, m_rightframe );
	}
}

void JahLibrary::initializeModule(QWidget* parent, QWidget* userspace, QHBox* theleftframe,QHBox* therightframe)
{
	ammw = new AssetManMainWidget(parent,userspace,theleftframe,therightframe);
	connect( ammw, SIGNAL( loadButton() ), this, SLOT( returnFile() ) );
}


JahLibrary::~JahLibrary()
{
	delete dirlist;
	delete root;
}

/*
void JahLibrary::show()
{
    QMainWindow::show();
}
*/
void JahLibrary::setup()
{
 
}

void JahLibrary::changeWinDir(int drive)
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
void JahLibrary::languageChange()
{
    setCaption( tr( "JahLoader" ) );
}

void JahLibrary::setPathCombo()
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

void JahLibrary::textWasChanged(const QString & thedir)
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

void JahLibrary::returnFile( void )
{ 
	//debug("returning file");
	
	//the new code here grab a handle on the fileview wdiget 
	//we need to know if its a filesystem view or a asset view 
	QtFileIconView *files = ammw->fileView( );

	/* 
	//currently these do not work in QtFileIconViewItem
	bool isAsset() { return passet; };
	Asset* asset() { return passet; };
	*/

	//if its a filesystem view we would grab data this way 
	const QtFileIconViewItem *item = ( const QtFileIconViewItem * )files->firstItem( ); 
	m_selected_items.clear( );

	for ( item = ( const QtFileIconViewItem * )files->firstItem( ); 
		item != NULL; 
		item = ( const QtFileIconViewItem * )item->nextItem( ) ) 
	{


		if ( item->isSelected( ) && QFileInfo( item->filename( ) ).exists( ) ) 
		{
			m_selected_items.append( item->filename( ) );

			/* 
			//if (item->isAsset())
			debug("itsaasset");
			else
			debug("itsnotaasset");
			*/

		}

	}
	
	emit loadButton();

}

void JahLibrary::returnNull( void )
{    
}

void JahLibrary::loadthisfile( QString )
{
	returnFile( );
}

void JahLibrary::setthefilename( QString )
{
}

void JahLibrary::directoryChanged( const QString &dir )
{
    setCaption( dir );
    setPathCombo();
}

void JahLibrary::slotStartReadDir( int dirs )
{
    label->setText( tr( " Reading Directory..." ) );
    progress->reset();
    progress->setTotalSteps( dirs );
}

void JahLibrary::slotReadNextDir()
{
    int p = progress->progress();
    progress->setProgress( ++p );
}

void JahLibrary::slotReadDirDone()
{
    label->setText( tr( " Reading Directory Done." ) );
    progress->setProgress( progress->totalSteps() );
}

void JahLibrary::cdUp()
{
    QDir dir = fileview->currentDir();
    dir.cd( ".." );
    fileview->setDirectory( dir );
}

void JahLibrary::newFolder()
{
    fileview->newDirectory();
}

void JahLibrary::changePath( const QString &path )
{
    if ( QFileInfo( path ).exists() )
	fileview->setDirectory( path );
    else
	setPathCombo();
}

void JahLibrary::enableUp()
{
    upButton->setEnabled( TRUE );
}

void JahLibrary::disableUp()
{
    upButton->setEnabled( FALSE );
}

void JahLibrary::enableMkdir()
{
    mkdirButton->setEnabled( TRUE );
}

void JahLibrary::disableMkdir()
{
    mkdirButton->setEnabled( FALSE );
}

////////////////////////////////////////////////////
// new code here

//for communication with desktop module
void JahLibrary::clickedImportButton()
{
    emit importButton();
}


QString JahLibrary::getModuleName()
{
  return ModuleName;
}
