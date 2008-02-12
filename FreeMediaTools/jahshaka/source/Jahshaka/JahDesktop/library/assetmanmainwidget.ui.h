/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qtable.h>
#include <qheader.h>
#include <qvaluelist.h>
#include <qsplitter.h>
#include <qcolor.h>
#include <qdir.h>
#include <qfileinfo.h> 
#include <qprogressbar.h>
#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qpopupmenu.h>
#include <qiconview.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qlineedit.h>

#include "assetmanitem.h" // from uic
#include "dirview.h"
#include "embedded-images.h" // from qembed script
#include "AssetManager.h" // from openassetlib

#include "loginform1.h"
#include "iconprovider.h" // TODO if def...

#include "jahpreferences.h"
#include "jahtracer.h"

void AssetManMainWidget::init()
{ 
     //hide unused widgets (TODO delete)
     driveCombo->hide();
     
	 //closeButton->hide(); // maybe useful if dialog box.
	 //pathCombo->hide();

	 //central main media space
     fileview->viewImage(); // default to thumbnail view.
     fileview->setFrameShape( QFrame::NoFrame );

	 //the left column - local database
     dirlist->setFrameShape( QFrame::NoFrame );
     dirlist->setShowDirsOnly( true );
     dirlist->setSorting( 99, true); // sorting enabled only by clicking column
    
	 //the right column
     filelist->setFrameShape( QFrame::NoFrame );
     filelist->setShowDirsOnly( true );
     filelist->setSorting( 99, true); // sorting enabled only by clicking column
     filelist->setDir( "/" );

    assetTable->horizontalHeader()->hide();
    assetTable->verticalHeader()->hide();
    assetTable->setVScrollBarMode (QScrollView::AlwaysOn);
    assetTable->setLeftMargin(0);
    assetTable->setTopMargin(0);

    assetTable->setColumnStretchable(0, true ); // so it fits the whole width
    //assetTable->setFocusStyle( QTable::FollowStyle );


    AssetManItem *assetitem = new AssetManItem( this ); // currently, re-use this single item
    assetitem->setPalette(this->palette() );

    populateDirView();
     
    assetTable->setNumRows(  assetTable->numRows()  + 1 );
    //ORIG    assetTable->setCellWidget(  assetTable->numRows()-1, 0, item);  
    assetTable->setCellWidget(  0, 0, assetitem);  
    // No!     assetTable->setRowStretchable(assetTable->numRows()-1, true ); // so it fits the whole height
    assetitem->assetNumber =  assetTable->numRows()-1;
    //ORIG    assetTable->setRowHeight( assetTable->numRows() -1,  200 ); 
    assetTable->setRowHeight( 0,  200 );
    assetitem->show();

    dirlist->setDir( "/" );

    populateDrivesCombo();
    
    progress->setProgress(100);

    //construct the views popups
    QPopupMenu *pop = new QPopupMenu( this );
    pop->insertItem("View small icons", fileview, SLOT( viewSmall()));
    pop->insertItem("View large icons", fileview, SLOT( viewLarge()));
    pop->insertItem("View thumbnails", fileview, SLOT ( viewImage()));
	pop->insertItem("View as browser", fileview, SLOT ( viewBrowser()));
    viewsButton->setPopup( pop );
    QPopupMenu *pop2 = new QPopupMenu( this );
    pop2->insertItem("Show media files only", fileview, SLOT( showMediaFilesOnly()));
    pop2->insertItem("Show image media only", fileview, SLOT( showImageFilesOnly()));
    pop2->insertItem("Show video media only", fileview, SLOT ( showVideoFilesOnly()));
    pop2->insertItem("Show audio media only", fileview, SLOT ( showAudioFilesOnly()));
    pop2->insertItem("Show all files and folders", fileview, SLOT ( showAllFiles()));
    mediaViewsButton->setPopup( pop2 );
    
    QIconSet upset( qembed_findImage("up_folder" ));
    upset.setPixmap( qembed_findImage("up_folder_d"), QIconSet::Automatic,  QIconSet::Disabled, QIconSet::Off );
    upButton->setIconSet( upset ); 
    QIconSet backset ( qembed_findImage( "arrowleft_green_16_h"));
    backset.setPixmap( qembed_findImage("arrowleft_green_16_d"), QIconSet::Automatic,  QIconSet::Disabled, QIconSet::Off );
    backButton->setIconSet( backset ); 
  
    // TODO these could go in ui. 
    connect( advancedSearchForm1->cancelAdvancedSearchButton, SIGNAL (clicked()), this, SLOT( advancedSearchCancelPressed()));
    
    connect( fileview, SIGNAL ( currentChanged(QIconViewItem* )), assetitem, SLOT( setInfoFromIconViewItem( QIconViewItem* )));
    connect( fileview, SIGNAL ( selectionChanged(QIconViewItem* )), assetitem, SLOT( setInfoFromIconViewItem( QIconViewItem* )));
    
    // connect the advanced search button. NB had problems doing this in ui file due to types. 
    connect( advancedSearchForm1, SIGNAL( hitsToShow( std::list<Asset*> )), 
            fileview, SLOT( readAssets( std::list<Asset*> )));

    // TODO need to track when the user hits return in the path/drive combos.
}



void AssetManMainWidget::populateDrivesCombo()
{
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
   // CAN INSERT EXTRA items driveCombo->insertItem("My Media");
}


void AssetManMainWidget::changeWinDir( int drive )
{
    QString thedrive = driveCombo->text(drive);
    
    //printf("changeWinDir to %s \n", thedrive.latin1() );

   
    dirlist->setDir( thedrive);
    
    // DO WE REALLY WANT ALL THIS? dont want to delete root and add roots...
    if ( root ) // (should always be a root anyway)
        delete root;
    root = new Directory( dirlist, thedrive );
    root->setOpen( TRUE );

    changePath(thedrive);
    comboTextWasChanged(thedrive);
}


void AssetManMainWidget::changePath( const QString &path )
{
    if ( QFileInfo( path ).exists() )
        fileview->setDirectory( path );
    else
        setPathCombo( path );
}


void AssetManMainWidget::comboTextWasChanged( const QString &thedir )
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


void AssetManMainWidget::setPathCombo( QString path)
{
    QString dir = path;
    int i = 0;
    bool found = FALSE;
    for ( i = 0; i < pathCombo->count(); ++i ) 
    {
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


void AssetManMainWidget::directoryChanged( const QString & dir )
{
    //setCaption( dir );
    setPathCombo( dir );
}


void AssetManMainWidget::slotStartReadDir( int dirs )
{
    //progressLabel->setText( " Loading..." );
    //progressLabel->show();
    qApp->setOverrideCursor ( QCursor( Qt::WaitCursor ) ); 

    progress->reset();
    progress->show();
    progress->setTotalSteps( dirs );
}


void AssetManMainWidget::slotReadNextDir()
{
    //progressLabel->setText( tr( " Loading..." ) + progress->progressString() ); // Never gets update :(
    int p = progress->progress();
    progress->setProgress( ++p );
   
}


void AssetManMainWidget::slotReadDirDone()
{
    qApp->restoreOverrideCursor();
    //printf("slotreaddirdone ))) \n");
    //progressLabel->setText( tr( " Ready" ) );
    progress->setProgress( progress->totalSteps() );
}


void AssetManMainWidget::loadThisFile( QString )
{
}


void AssetManMainWidget::setTheFileName( QString )
{
}

void AssetManMainWidget::populateDirView()
{
    // TODO root should not be a data member?
    // Should we store localdb, trash etc??

    Directory *dir, *localdb;

/* JPN
    IconProvider *wip;
//#ifdef WIN32
    // Linux and Mac cannot use the windows icon provider, they'll need their own or hardcoded icons.
    wip = new IconProvider(); // TODO keep one?
//#endif    
*/
    dir = new Directory( dirlist, "Trash", Directory::TrashCan );
    dir->setExpandable( false );
    dir->setOpen( true );
    QPixmap *pm = new QPixmap( qembed_findImage("trash") );
    dir->setPixmap( pm );

    QString dsname;
    dir = new Directory( dirlist, "Search results", Directory::SearchRoot );
    dir->setPixmap( new QPixmap( qembed_findImage("search-mini") ));
    dsname = "www.jahshaka.net";
    dir = new Directory( dirlist, dsname, Directory::RemoteDataSource);
    dir->setPixmap( new QPixmap( qembed_findImage("minidb-16w") ));
    dir->setReadable( false ); // its not connected yet.
   
    dsname = "Jahshaka Libraries";
    localdb = new Directory( dirlist, dsname, Directory::LocalDataSource);
    localdb->setPixmap( new QPixmap( qembed_findImage("minidb-16w") ));
    DataSource *ds = olib::openassetlib::AssetManager::instance().createDataSource(DataSource::LocalDatabase, dsname.latin1() );

	QString jahdblocation = locateAssetDatabase().c_str();
	QFileInfo dbinfo( jahdblocation );

    QString connectstring = "file://"  + jahdblocation;
    try
    {   ds->connect( connectstring.latin1() );
    }
    catch(std::exception& e)
    {   QMessageBox::information( this, "Error" , e.what(), "OK"  );
    }
    catch ( ... )
    {   QMessageBox::information( this, "Error" , "Problem connecting to local database.", "OK"  );
    }

  /* TO REMOVE
    // Create 'All media' collection first time round.
    if ( !dbexisted ) 
    {    try
        {   ds->createCollection( "All media" );
            printf("just created All Media collection \n" );
        }
        catch(std::exception& e)
        {   printf("create collection failed for All Media\n");
            QMessageBox::information( this, "Error" , e.what(), "OK" );
        }
    }
    */

   //printf(" localdb text %s \n", localdb->text(0).latin1() );
   if ( ds != 0 )
   {   localdb->setDataSource( ds );
       localdb->populateCollections();
   }
   else
       QMessageBox::warning( this, "Database error", "Unable to open or connect to local database.\n" );


	/////////////////////////////////////////////////////////////////////
	// Now deal with the fileview drives.    
     root = new Directory( filelist, "My Computer", Directory::ComputerRoot );
     root->setSelected( false );
     filelist->setSelected( root, false );
     // root->setPixmap( wip->hardDriveSmallPixmap() );
      QDir d;

    const QFileInfoList* roots = QDir::drives();
    QPtrListIterator<QFileInfo> i(*roots);
    i.toLast();
    QFileInfo* fi;
    
    while ( (fi = *i) ) {
        --i;
        if (fi->isDir())
        {
     		new Directory( root, fi->filePath(),  Directory::DriveRoot);
        }
    }

	/////////////////////////////////////////////////////////////////////
	// Now deal with the media drive.
    JahPrefs& jprefs = JahPrefs::getInstance();
    QString JahMediaPath  = jprefs.getMediaPath().c_str();

     mediaroot = new Directory( filelist, "Media", Directory::ComputerRoot );
     mediaroot->setSelected( false );
     filelist->setSelected( mediaroot, false );
     // root->setPixmap( wip->hardDriveSmallPixmap() );
     new Directory( mediaroot, JahMediaPath+"/media",  Directory::FileFolder);

	 ////////////////////////////////////////////////////////////////////
	 //now clear ?
    fileview->clear();

}


void AssetManMainWidget::advandedSearchClicked()
{
    if ( advancedSearchButton->text().contains("Advanced") )
    {    widgetStack1->raiseWidget( advancedSearchStackPage );
         advancedSearchButton->setText( "Simple" );
    }
    else
    {   advancedSearchCancelPressed();
    }


}


void AssetManMainWidget::advancedSearchCancelPressed()
{
    widgetStack1->raiseWidget( dirlistStackPage );
    advancedSearchButton->setText( "Advanced" );
}


void AssetManMainWidget::executeSimpleSearch()
{
    fileview->clear();
    QString searchstr = searchLineEdit->text().stripWhiteSpace();
    if ( searchstr.length() == 0 )
        return;
    // We dont actually use wildcards yet...
    searchstr = searchstr.replace( "*", "" );

    QListViewItem *sitem = dirlist->selectedItem ();
    Directory* dir = dynamic_cast<Directory*>(sitem);
    if ( !dir )
        return;
    DataSource *ds = dir->dataSource();
    if ( !ds )
        return;

    // Could combine search for collection name also?
    DataSource::Query::Condition* condition = new DataSource::Query::Condition( "Filename", searchstr.latin1(), DataSource::Query::Condition::Contains ); 
    DataSource::Query query( condition );
    std::list<Asset*> hits = ds->search( query );
    //printf("Hits found = %i \n", hits.size() );
    delete condition;
    fileview->readAssets( hits );

}


void AssetManMainWidget::cdUp()
{
    QDir dir = fileview->currentDir();
    dir.cd( ".." );
    fileview->setDirectory( dir );
}


void AssetManMainWidget::enableUp()
{
    upButton->setEnabled( true );
    QDir dir = fileview->currentDir();
    dir.cd( ".." );
    QToolTip::add( upButton, QString( "Up to " ) + dir.absPath() );
}


void AssetManMainWidget::disableUp()
{
    upButton->setEnabled( false );
    QToolTip::remove( upButton );
}


void AssetManMainWidget::connectToServerDialog()
{
    ConnectDialog *con = new ConnectDialog(0);
    con->exec();
}


void AssetManMainWidget::enableBack()
{
    //printf("in enableBack\n" );
    backButton->setEnabled( true );
    QToolTip::add( backButton, QString("Back to " ) + fileview->previousDir().absPath() );
}


void AssetManMainWidget::disableBack()
{
    backButton->setEnabled( false );
    QToolTip::remove( backButton );
}


void AssetManMainWidget::cdBack()
{
   //printf("going back to %s \n", fileview->previousDir().dirName().latin1() );
   fileview->setDirectory( fileview->previousDir() );
}



std::string AssetManMainWidget::locateAssetDatabase()
{
	/////////////////////////////////////////////////////
	//the jahplayer uses a different database for jahshaka to make sure
	//we dont have issues with people trying to access the same
	//database at the sme time from different apps
    //need to check this here as we dont have a JahScenesPAth pref yet
	#ifndef JAHPLAYER
    QString databasename = "jahshaka.db";
	#else
	QString databasename = "jahplayer.db";
	#endif

    /////////////////////////////////////////////////////
    //check to see if database has been relocated
    JahPrefs& jprefs = JahPrefs::getInstance();
    QString JahBasePath   = jprefs.getBasePath().c_str();
    QString JahMediaPath  = jprefs.getMediaPath().c_str();
	JahTrace *jtrace = JahTrace::getInstance();	//set up tracer

    QDir d( JahMediaPath+"/database/" );                        // "./example"
    
    if ( d.exists() )
    {
        jtrace->info( "Initializing Jahshaka Asset Database externally");
        jtrace->debug("Database at:",JahMediaPath+"/database/"+databasename);
        return (JahMediaPath+"/database/"+databasename).ascii();
    }
 
    jtrace->info( "Initializing Jahshaka Asset Database locally" );
    jtrace->debug("Database at:",JahBasePath+"database/"+databasename);
    return (JahBasePath+"database/"+ databasename).ascii();
}
