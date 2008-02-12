
#include "edit.h"
#include <qhbox.h>
#include <mediatable.h>
#include <desktop.h>
#include <jahpreferences.h>
#include <assetexchange.h>

//////////////////////////////////////////////////
// the new mediatable is initalized here
void GLEdit::initializeMediaTable( QHBox* parentbox)
{
    mediatable = new JahDesktopSideTable( 0, parentbox );
	mediatable->setAssetTracking( false );
    connect ( mediatable, SIGNAL( assetSelected( assetData ) ), this, SLOT( assetSelected( assetData ) ) );
}

void GLEdit::addAssetToEffectsTable(QString thedir, QString fxfilename)
{
	static int fxnumber = 0;
                    
    assetExchange exchange;
    assetData     theasset;
    
    //split filename into filename and extension
	int i = fxfilename.findRev(".");
    
    QString extension   = fxfilename.right(fxfilename.length()-(i));
    QString filename  = fxfilename.left(fxfilename.length() - (extension.length()) );                   
	QString shortName = filename;
	QString pathString = thedir+"/";

	bool itWorked;
	theasset = exchange.loadJahMediaAsset( filename, shortName,
										    pathString, extension, 
										    fxnumber, itWorked );
		
	//these were passed over to the table in addAsset								   
	//need to understand what they do
	int index = fxnumber; 
	int globaltablenumber = 1;

	QImage theimage = exchange.getImage(theasset);
	mediatable->addAsset( index,  globaltablenumber, theasset, theimage );
	
    fxnumber ++;
}
		
					
void GLEdit::initializeEffects(void)
{
    QString JahBasePath;
    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath  = jprefs.getBasePath().data();

    ///////////////////////////////////////////////////////////////////////////
    //we loop through all effects in the plugin directory

    QString jaheditingfxpath = JahBasePath+"plugins/editingfx";

    //the extension we are looking for
    QString plExt = "jfx";

    ///////////////////////////////////////////////////////////////////
    //we iterate through all the folders in the plugins directory
    //and scan each folder for plugins
    //this can be cleaned up now that it works
    
    QString fxfilename; //,loadfile;

	//first we grab the directory to scan
    QDir dd(jaheditingfxpath);
    if (!dd.exists())
        return;

	//then we set up the iterator
    const QFileInfoList *dlist = dd.entryInfoList();
    QFileInfoListIterator dit( *dlist );
    QFileInfo *dfi;

	//now we scan all files and folders
    while ( (dfi = dit.current()) != 0 )
    {
        if (!dfi->isDir())
        {
            QString thedir = jaheditingfxpath;
            fxfilename = dfi->fileName();

            if (fxfilename.endsWith(plExt))
				addAssetToEffectsTable(thedir, fxfilename);
        }
        ++dit;
    }
}

void GLEdit::assetSelected( assetData asset )
{
	AssetTables::getInstance( )->setSelectedAsset( asset );
}
