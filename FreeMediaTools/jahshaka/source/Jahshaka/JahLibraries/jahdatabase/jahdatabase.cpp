/*******************************************************************************
**
** The source file for the Jahshaka desktop database file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahdatabase.h"
#include <sqlite3.h>

static const unsigned char databaseVersionMajor		= 2;
static const unsigned char databaseVersionMinor		= 1;
static const unsigned char databaseVersionRevision	= 2;


//is this callback ever used?
//probably for dbase error reporting?

static int callback(void *, int argc, char **argv, char **azColName)
{
	QString data;
	for(int i=0; i<argc; i++)
	{
		data  = QString(azColName[i]) + " = " +  QString(argv[i]);
		debug(data);
	}
	return 0;
}

JahDatabase::JahDatabase()
{
    //set up tracer
    jtrace = JahTrace::getInstance();	//set up tracer
    jtrace->debug("Setting up the Database Library");
}

JahDatabase::~JahDatabase()
{
}

std::string JahDatabase::locateDatabase()
{
	/////////////////////////////////////////////////////
	//the jahplayer uses a different database for jahshaka to make sure
	//we dont have issues with people trying to access the same
	//database at the sme time from different apps
    //need to check this here as we dont have a JahScenesPAth pref yet
	#ifndef JAHPLAYER
    QString databasename = "jahdesktop.db";
	#else
	QString databasename = "jahplayerdesktop.db";
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
        jtrace->info( "Initializing Jahshaka Database externally");
        jtrace->debug("Database at:",JahMediaPath+"/database/"+databasename);
        return (JahMediaPath+"/database/"+databasename).ascii();
    }
 
    jtrace->info( "Initializing Jahshaka Database locally" );
    jtrace->debug("Database at:",JahBasePath+"/database/"+databasename);
    return (JahBasePath+"/database/"+ databasename).ascii();
}

////////////////////////////////////////////////////////
//Initialize and Open the database
void JahDatabase::dropDatabase()
{
    char*	zErrMsg = 0;

	jtrace->info("Dropping desktop table");
	sqlite3_exec(db, 
				"drop table desktop", 
				callback, 
				0, 
				&zErrMsg);
	free(zErrMsg);
	zErrMsg = 0;

	jtrace->info("Dropping videodata table");
	sqlite3_exec(db, 
				"drop table videodata", 
				callback, 
				0, 
				&zErrMsg);
	free(zErrMsg);
	zErrMsg = 0;

	jtrace->info("Dropping version table");
	sqlite3_exec(db, 
				"drop table version", 
				callback, 
				0, 
				&zErrMsg);
	free(zErrMsg);
	zErrMsg = 0;
}

void JahDatabase::createDatabase()
{
	int		rc;
    char*	zErrMsg = 0;

	jtrace->info("Creating new version table...");
	rc = sqlite3_exec(	db, 
						"create table version(major integer not null, minor integer, revision integer)", 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to create version table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		exit(1);
	}
	jtrace->info("Done");


	jtrace->info("Inserting version information into version table...");
	rc = sqlite3_exec(	db, 
						QString("insert into version values(%1,%2,%3)").arg(databaseVersionMajor).arg(databaseVersionMinor).arg(databaseVersionRevision),
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to insert row into version table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		exit(1);
	}
	jtrace->info("Done");


	jtrace->info("Creating new desktop table...");
	rc = sqlite3_exec(	db, 
						"create table desktop(name varchar(55), numclips integer)", 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to create desktop table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		exit(1);
	}
	jtrace->info("Done");


	jtrace->info("Inserting default desktop entry into desktop table...");
	rc = sqlite3_exec(	db, 
						"insert into desktop values(\"desktop\", 2)", 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to insert row into desktop table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		exit(1);
	}
	jtrace->info( "Done" );


	jtrace->info("Creating videodata table...");
	rc = sqlite3_exec(	db, 
						"create table videodata(Category varchar(12), Cliptype varchar(12), Videoext varchar(9), MediaName varchar(128), MediaNumber int, FileLocation varchar(255), FileName varchar(255), FileExtension varchar(12), StartFrame int, EndFrame int, xpos int, ypos int, xsize int, ysize int, lock tinyint, filmstrip tinyint, frameOffset int, xNativeSize int, yNativeSize int, desktop varchar(55))", 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to create desktop table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		exit(1);
	}
	jtrace->info("Done");
}

bool JahDatabase::addDesktop(const char* /* desktopName */)
{
	int		rc;
    char*	zErrMsg = 0;

	jtrace->info("Inserting desktop entry into desktop table...");
	rc = sqlite3_exec(	db, 
						"insert into desktop values(\"desktop\", 2)", 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to insert row into desktop table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		return false;
	}
	jtrace->info( "Done" );
	return true;
}

bool JahDatabase::removeDesktop(const char* desktopName)
{
	int		rc;
    char*	zErrMsg = 0;

	jtrace->info("Deleting desktop entry from desktop table...");
	rc = sqlite3_exec(	db, 
						QString("delete from desktop where name = \"%1\"").arg(desktopName), 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to delete row from desktop table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		return false;
	}

	jtrace->info("Removing assets relating to desktop entry on videodata table...");
	rc = sqlite3_exec(	db, 
						QString("update videodata set category = 9 where desktop = \"%1\"").arg(desktopName),
						//QString("delete from videodata where desktop = \"%1\"").arg(desktopName), 
						callback, 
						0, 
						&zErrMsg);
	if( rc!=SQLITE_OK )
	{
		jtrace->error("Failed to delete rows from videodata table: ", zErrMsg);
		free(zErrMsg);
		zErrMsg = 0;
		jtrace->debug("Note: The database may now contain orphaned assets");
	}

	jtrace->info( "Done" );
	return true;
}

void JahDatabase::InitializeDatabase()
{
    QString jahdatabase = locateDatabase().c_str();

    //execute the sql commnad to open the database
    char *zErrMsg = 0;
	if(sqlite3_open(jahdatabase.ascii(), &db) != SQLITE_OK)
	{
        jtrace->error( "Unable to open the database");
        jtrace->error( "Error",sqlite3_errmsg(db));
        exit(1);
    }
    else
    {
		char**	table;
		int		rc,
				max_rows,
				max_cols;

		rc = sqlite3_get_table(	db, 
								"select major, minor, revision from version", 
								&table, 
								&max_rows, 
								&max_cols, 
								&zErrMsg);
	    if(rc == SQLITE_OK)
		{
			unsigned short dbVersionMajor		= QString(table[3]).toUShort();
			unsigned short dbVersionMinor		= QString(table[4]).toUShort();
			unsigned short dbVersionRevision	= QString(table[5]).toUShort();

			jtrace->info(QString("Database version %1.%2.%3").arg(dbVersionMajor).arg(dbVersionMinor).arg(dbVersionRevision));

			// Check this database is of the correct version
			if	(	(dbVersionMajor != databaseVersionMajor)
				||	(dbVersionMinor != databaseVersionMinor)
				||	(dbVersionRevision != databaseVersionRevision) )
			{
				// Different, therefore database needs upgrading!
				jtrace->warn(QString("Expecting database version %1.%2.%3").arg(databaseVersionMajor).arg(databaseVersionMinor).arg(databaseVersionRevision));
				dropDatabase();		// At some stage we may want to do specific changes on a db schema update,
				createDatabase();	// but for now just drop and recreate
			}
		}
		else
		{
			jtrace->warn("Check to find out what database version number failed: ", zErrMsg);
			free(zErrMsg);
			zErrMsg = 0;

			dropDatabase();
			createDatabase();
		}

        jtrace->debug("Initialized Database");
    }
}

int JahDatabase::TestDatabase(void)
{
    jtrace->info("Testing the database");
    int numclip = OpenDesktopTable();		//gets number of entries in database
    int counter = GetDatabaseClipNumber();
    if (counter==numclip)
        jtrace->debug("Database is ok");
    else
    {
        jtrace->warn("database got issues");
        if(counter > numclip) 
        {
            jtrace->warn("Extra assets! Salvaging missing assets");
            numclip = counter;
        }
        else if(counter < numclip)
        {
            jtrace->warn("Missing assets! Resetting asset clipnumber");
            numclip = counter;
        }
		//rewrite desktop table data in case we crash again!
    }
    return numclip;
}

void JahDatabase::CloseDatabase(int clipnum)
{
    jtrace->info("Closing database");

    // update the database clipnumber...
    char *zErrMsg = 0;
    sqlite3_exec(db, 
				QString("update desktop set numclips=%1").arg(clipnum), 
				callback, 
				0, 
				&zErrMsg);
	free(zErrMsg);

    sqlite3_close(db);   
    jtrace->debug("Database closed");
}


bool JahDatabase::WipeDatabase(void) 
{
    jtrace->info("Wiping Jahshaka Database...");

    int  rc;
    char *zErrMsg = 0;
    // Set the database clipnumber to zero
    rc = sqlite3_exec(	db, 
						"update desktop set numclips=0", 
						callback, 
						0, 
						&zErrMsg);
    if(rc != SQLITE_OK)
	{
		QString msg = QString("Failed to update destop table: %1").arg(zErrMsg);
		jtrace->warn(msg);
		if(zErrMsg != 0)
		{
			free(zErrMsg);
			zErrMsg = 0;
		}
		return false;
	}

    // Delete all rows in the videodata table
    rc = sqlite3_exec(	db, 
						QString("delete from videodata"), 
						callback, 
						0, 
						&zErrMsg);

    if( rc!=SQLITE_OK )
	{
		QString msg = QString("Failed to delete all rows from videodata table: %1").arg(zErrMsg);
		jtrace->warn(msg);
		if(zErrMsg != 0)
		{
			free(zErrMsg);
			zErrMsg = 0;
		}
		return false;
	}
    jtrace->debug("Done");
    return true;
}

//update the database with new information from
//videData on a clips position once it has been moved
//need some error checking done here

void JahDatabase::updateClipPosition(assetData data)
{
    //build and execute the sql command to update xpos
    QString sqlCmd = QString("update videodata set xpos=%1, ypos=%2 where MediaNumber=%3")
                        .arg(data.xpos)
						.arg(data.ypos)
                        .arg(data.clipnumber);
    char* zErrMsg = 0;
    sqlite3_exec(db, sqlCmd, callback, 0, &zErrMsg);
	free(zErrMsg);
}

//update the database with new information from
//videData on a clips position once it has been moved
//need some error checking done here

void JahDatabase::updateClipSize(assetData data)
{
    //build and execute the sql command to update xsize
    QString sqlCmd = QString("update videodata set  xsize = %1,ysize = %2 where MediaNumber = %3")
                        .arg(data.xsize)
                        .arg(data.ysize)
                        .arg(data.clipnumber);
    char* zErrMsg = 0;
    sqlite3_exec(db, sqlCmd, callback, 0, &zErrMsg);
	free(zErrMsg);
}

void JahDatabase::updateLockStatus(assetData data)
{
    //build and execute the sql command to update xpos
    QString sqlCmd = QString("update videodata set lock=%1 where MediaNumber=%2")
						.arg(data.locked ? 1 : 0)
						.arg(data.clipnumber);
    char* zErrMsg = 0;
    sqlite3_exec(db, sqlCmd, callback, 0, &zErrMsg);
	free(zErrMsg);
}

void JahDatabase::updateFilmstripStatus(assetData data)
{
    //build and execute the sql command to update xpos
	QString sqlCmd = QString("update videodata set filmstrip=%1 where MediaNumber = %2")
						.arg(data.filmstrip ? 1 : 0)
						.arg(data.clipnumber);
    char* zErrMsg = 0;
    sqlite3_exec(db, sqlCmd, callback, 0, &zErrMsg);
	free(zErrMsg);
}

void JahDatabase::updateFrameOffset(assetData data)
{
    //build and execute the sql command to update xpos
	QString sqlCmd = QString("update videodata set frameOffset=%1 where MediaNumber = %2")
						.arg(data.frameOffset)
						.arg(data.clipnumber);
    char* zErrMsg = 0;
    sqlite3_exec(db, sqlCmd, callback, 0, &zErrMsg);
	free(zErrMsg);
}

//this command reads in the number of clips 
//from the desktop database table

int JahDatabase::OpenDesktopTable()
{
    jtrace->debug("Accessing Desktop Table");

    char** data;
    char* zErrMsg = 0;
    int	rc,
		max_rows,
		max_cols;
    rc  = sqlite3_get_table(	db, 
							"select name, numclips from desktop", 
							&data, 
							&max_rows, 
							&max_cols, 
							&zErrMsg);
    if(rc != SQLITE_OK)
    { 
		jtrace->error(QString("Failed to retrieve desktop table: %1").arg(zErrMsg)); 
		free(zErrMsg);
        return -1; 
    }
    jtrace->debug(QString("Desktop name=%1, numclips=%2").arg(data[2]).arg(data[3]));
	return QString(data[3]).toInt();
}



//this is the main startup routine that rebuilds the desktop
//from the database by reading in all assets
//break this out into separate routines to make it easier to deal with...
//make this return bool for success to trap errors

///////////////////////////////////////////////////////////
// gets the number of each desktop clip from the database
// not the desktop so we can verify in case there was a crash and recover

int JahDatabase::GetDatabaseClipNumber()
{
    jtrace->debug("Retrieving number of clips stored in database clip");

    char** data;
    char* zErrMsg = 0;
    int rc,
		max_rows,
		max_cols;
    rc  = sqlite3_get_table(	db, 
							"select count(*) from videodata", 
							&data, 
							&max_rows, 
							&max_cols, 
							&zErrMsg);
    if(rc != SQLITE_OK)
    {
        jtrace->error("GetDatabaseClipNumber","SQL Error in database"); 
		free(zErrMsg);
        return -1;
    }
    QString msg = QString("The number returned was %1").arg(data[1]);
    jtrace->debug(msg);
    return QString(data[1]).toInt();
}


//need to clean this up and add new data types such as SEQUENCE and videoext, keyext, etc

void JahDatabase::addImage2database( assetData thedata )
{
    jtrace->debug("JahDatabase::","Adding asset to database");

    char* zErrMsg = 0;
	QString sqlCmd = QString("insert into videodata %1 values(%2)")
							.arg("(Category, Cliptype, Videoext, MediaName, MediaNumber, FileLocation, FileName, FileExtension, StartFrame, EndFrame, xpos, ypos, xsize, ysize, filmstrip, lock, frameOffset, xNativeSize, yNativeSize, desktop)")
							.arg(	QString("\"%1\",\"%2\",\"%3\",\"%4\",%5,\"%6\",\"%7\",\"%8\"")
											.arg(thedata.theCategory)
											.arg(thedata.theType)
											.arg(thedata.videoext)
											.arg(thedata.clipname)
											.arg(thedata.clipnumber)
											.arg(thedata.location)
											.arg(thedata.filename)
											.arg(thedata.extension)
									+ QString(",%1,%2,%3,%4,%5,%6,%7,%8,%9")
											.arg(thedata.startframe)
											.arg(thedata.endframe)
											.arg(thedata.xpos)
											.arg(thedata.ypos)
											.arg(thedata.xsize)
											.arg(thedata.ysize)
											.arg(thedata.filmstrip ? 1 : 0)
											.arg(thedata.locked ? 1 : 0)
											.arg(thedata.frameOffset)
									+ QString(",%1,%2,\"%3\"")
											.arg(thedata.xNativeSize)
											.arg(thedata.yNativeSize)
											.arg(thedata.desktop)
											);
    int rc = sqlite3_exec(	db, 
							sqlCmd, 
							callback, 
							0, 
							&zErrMsg);
	if(rc != SQLITE_OK)
	{
		QString msg = QString("Failed to insert asset to database: %1").arg(zErrMsg);
		jtrace->debug("JahDatabase::",msg);
		free(zErrMsg);
	}
}


void JahDatabase::removeImagefromdatabase( int imagenumber ) 
{
    //we need to remove the data from the database
    //for the case where clipnumber = imagenumber
    // which is field MediaNumer in table videodata

    //for now we will just de-activate clips
    //until we can figure this out properly
    //we do this by setting category to 9

    jtrace->debug("JahDatabase::","Removing asset" );

    //build and execute the sql command to update xpos
    char* zErrMsg = 0;
    int rc = sqlite3_exec(	db, 
							QString("update videodata set category = 9 where MediaNumber = %1").arg(imagenumber), 
							callback, 
							0, 
							&zErrMsg);
	if(rc != SQLITE_OK)
	{
		QString msg = QString("Failed to remove asset from database: %1").arg(zErrMsg);
		jtrace->warn("JahDatabase::",msg);
		free(zErrMsg);
	}
}

void JahDatabase::deleteImagefromdatabase( int imagenumber ) 
{
    jtrace->debug( "JahDatabase::","deleting asset" );
    char* zErrMsg = 0;
    int rc = sqlite3_exec(	db, 
							QString("delete from videodata where MediaNumber = %1").arg(imagenumber), 
							callback, 
							0, 
							&zErrMsg);
	if(rc != SQLITE_OK)
	{
		QString msg = QString("Failed to delete asset from database: %1").arg(zErrMsg);
		jtrace->warn("JahDatabase::",msg);
		free(zErrMsg);
	}
}

void JahDatabase::renameImageindatabase( int imagenumber, QString newname) 
{
    //we need to remove the data from the database
    //for the case where clipnumber = imagenumber
    // which is field MediaNumer in table videodata

    //for now we will just de-activate clips
    //until we can figure this out properly
    //we do this by setting category to 9
    jtrace->debug( "JahDatabase::","rename asset in database" );

    //build and execute the sql command to update xpos
    QString sqlCmd = QString("update videodata set  MediaName = \"%1\" where MediaNumber = %2")
                        .arg(newname)
                        .arg(imagenumber);

    char* zErrMsg = 0;
    int rc = sqlite3_exec(	db, 
							sqlCmd, 
							callback, 
							0, 
							&zErrMsg);
	if(rc != SQLITE_OK)
	{
		QString msg = QString("Failed to rename asset: %1").arg(zErrMsg);
		jtrace->warn("JahDatabase::",msg);
		free(zErrMsg);
	}
}

///////////////////////////////////////////////////////////
// gets the number of each desktop clip from the database

assetData JahDatabase::getImageFromDatabase(int clipnumber)
{
    assetData thedata;
    if(!doesAssetExistindatabase(clipnumber))
    {
        thedata.theCategory = (VideoCategory::TYPE)-1;
        return thedata;
    }

    //Build & execute the SQL command
    int rc,
		max_rows,
		max_cols;
    char* zErrMsg = 0;
    char** data = NULL;
    rc  = sqlite3_get_table(	db, 
							QString("select %1 from videodata where MediaNumber=%2")
								.arg(QString("Category,ClipType,Videoext,MediaName,MediaNumber,FileLocation,FileName,FileExtension,StartFrame,EndFrame,xpos,ypos,xsize,ysize,lock,filmstrip,frameOffset,xNativeSize,yNativeSize,desktop"))
								.arg(clipnumber), 
							&data, 
							&max_rows, 
							&max_cols, 
							&zErrMsg);
    if(rc != SQLITE_OK)
    {
		QString msg = QString("Failed to query the videodata table for MediaNumber=%1: %2").arg(clipnumber).arg(zErrMsg);
        jtrace->debug(msg);
		free(zErrMsg);
        thedata.theCategory = (VideoCategory::TYPE)-1;
        return thedata;
    }

	// The first 20 indexes are the column names, the next 20 is the data, we start at 20
	const unsigned char INDEX_Category		= 20;
	const unsigned char INDEX_ClipType		= 21;
	const unsigned char INDEX_Videoext		= 22;
	const unsigned char INDEX_MediaName		= 23;
	const unsigned char INDEX_MediaNumber	= 24;
	const unsigned char INDEX_FileLocation	= 25;
	const unsigned char INDEX_FileName		= 26;
	const unsigned char INDEX_FileExtension	= 27;
	const unsigned char INDEX_StartFrame	= 28;
	const unsigned char INDEX_EndFrame		= 29;
	const unsigned char INDEX_xpos			= 30;
	const unsigned char INDEX_ypos			= 31;
	const unsigned char INDEX_xsize			= 32;
	const unsigned char INDEX_ysize			= 33;
	const unsigned char INDEX_lock			= 34;
	const unsigned char INDEX_filmstrip		= 35;
	const unsigned char INDEX_frameOffset	= 36;
	const unsigned char INDEX_xNativeSize	= 37;
	const unsigned char INDEX_yNativeSize	= 38;
	const unsigned char INDEX_desktop		= 39;

	thedata.theCategory		= (VideoCategory::TYPE)QString("%1").arg(data[INDEX_Category]).toInt();
    thedata.theType			= (VideoType::TYPE)QString("%1").arg(data[INDEX_ClipType]).toInt();
    thedata.videoext		= QString(data[INDEX_Videoext]);
	thedata.clipname		= QString(data[INDEX_MediaName]);
    thedata.clipnumber		= QString("%1").arg(data[INDEX_MediaNumber]).toInt();
    thedata.location		= QString(data[INDEX_FileLocation]);
	thedata.filename		= QString(data[INDEX_FileName]);
    thedata.extension		= QString(data[INDEX_FileExtension]);
    thedata.startframe		= QString("%1").arg(data[INDEX_StartFrame]).toInt();
	thedata.endframe		= QString("%1").arg(data[INDEX_EndFrame]).toInt();
    thedata.xpos			= QString("%1").arg(data[INDEX_xpos]).toDouble();
    thedata.ypos			= QString("%1").arg(data[INDEX_ypos]).toDouble();
    thedata.xsize			= QString("%1").arg(data[INDEX_xsize]).toDouble();
    thedata.ysize			= QString("%1").arg(data[INDEX_ysize]).toDouble();
	thedata.frameOffset		= QString("%1").arg(data[INDEX_frameOffset]).toInt();
	thedata.locked			= QString("%1").arg(data[INDEX_lock]).toInt() ? true : false;
	thedata.filmstrip		= QString("%1").arg(data[INDEX_filmstrip]).toInt() ? true : false;
    thedata.xNativeSize		= QString("%1").arg(data[INDEX_xNativeSize]).toDouble();
    thedata.yNativeSize		= QString("%1").arg(data[INDEX_yNativeSize]).toDouble();
    thedata.desktop			= QString("%1").arg(data[INDEX_desktop]);

    switch (thedata.theCategory)
	{
	case VideoCategory::IMAGE:
		thedata.videotex = FALSE;
		break;
	case VideoCategory::CLIP:
		thedata.videotex = TRUE;
		break;
	case VideoCategory::AUDIO:
		thedata.videotex = FALSE;
		break;
	case VideoCategory::MEDIA:
		thedata.videotex = TRUE;
		break;
	case VideoCategory::CLIPKEY:
		thedata.videotex = TRUE;
		break;
	default:  //KEY, CLIPKEY not used
		thedata.theCategory = (VideoCategory::TYPE)-1;
		break;
    }
    return thedata;
}

bool JahDatabase::doesAssetExistindatabase(int clipnum)
{
    //Build & execute SQL command
    int rc,
		max_rows,
		max_cols;
    char *zErrMsg = 0;
    char  **data;
    rc  = sqlite3_get_table(	db, 
							QString("select count(*) from videodata where MediaNumber = %1").arg(clipnum), 
							&data, 
							&max_rows, 
							&max_cols, 
							&zErrMsg);
    if( rc!=SQLITE_OK )
    {
		QString msg = QString("Failed to retrieve the number of assets with clipnum=%1: %2")
						.arg(clipnum)
						.arg(zErrMsg);
        jtrace->warn(msg); 
		free(zErrMsg);
        return false;
    }

    jtrace->debug(QString("Number of assets with clipnum=%1 is %2").arg(clipnum).arg(data[1]));
    return QString("%1").arg(data[1]).toInt();
}

///////////////////////////////////////////////////////////
// gets the number of each desktop clip from the database

assetData JahDatabase::restoreImageFromDatabase(int row)
{
    assetData thedata;

    int rc,
		max_rows,
		max_cols;
    char* zErrMsg = 0;
    char** data = NULL;
    rc = sqlite3_get_table(	db, 
							QString("select %1 from videodata where ROWID=%2")
								.arg(QString("Category,ClipType,Videoext,MediaName,MediaNumber,FileLocation,FileName,FileExtension,StartFrame,EndFrame,xpos,ypos,xsize,ysize,lock,filmstrip,frameOffset,xNativeSize,yNativeSize,desktop"))
								.arg(row), 
							&data, 
							&max_rows, 
							&max_cols, 
							&zErrMsg);
    if(rc != SQLITE_OK)
    {
		QString msg = QString("Failed to query the videodata table for ROWID=%1: %2").arg(row).arg(zErrMsg);
        jtrace->debug(msg);
		free(zErrMsg);
        thedata.theCategory = (VideoCategory::TYPE)-1;
        return thedata;
    }

	// The first 20 indexes are the column names, the next 20 is the data, we start at 20
	const unsigned char INDEX_Category		= 20;
	const unsigned char INDEX_ClipType		= 21;
	const unsigned char INDEX_Videoext		= 22;
	const unsigned char INDEX_MediaName		= 23;
	const unsigned char INDEX_MediaNumber	= 24;
	const unsigned char INDEX_FileLocation	= 25;
	const unsigned char INDEX_FileName		= 26;
	const unsigned char INDEX_FileExtension	= 27;
	const unsigned char INDEX_StartFrame	= 28;
	const unsigned char INDEX_EndFrame		= 29;
	const unsigned char INDEX_xpos			= 30;
	const unsigned char INDEX_ypos			= 31;
	const unsigned char INDEX_xsize			= 32;
	const unsigned char INDEX_ysize			= 33;
	const unsigned char INDEX_lock			= 34;
	const unsigned char INDEX_filmstrip		= 35;
	const unsigned char INDEX_frameOffset	= 36;
	const unsigned char INDEX_xNativeSize	= 37;
	const unsigned char INDEX_yNativeSize	= 38;
	const unsigned char INDEX_desktop		= 39;

	thedata.theCategory		= (VideoCategory::TYPE)QString("%1").arg(data[INDEX_Category]).toInt();
    thedata.theType			= (VideoType::TYPE)QString("%1").arg(data[INDEX_ClipType]).toInt();
    thedata.videoext		= QString(data[INDEX_Videoext]);
	thedata.clipname		= QString(data[INDEX_MediaName]);
    thedata.clipnumber		= QString("%1").arg(data[INDEX_MediaNumber]).toInt();
    thedata.location		= QString(data[INDEX_FileLocation]);
	thedata.filename		= QString(data[INDEX_FileName]);
    thedata.extension		= QString(data[INDEX_FileExtension]);
    thedata.startframe		= QString("%1").arg(data[INDEX_StartFrame]).toInt();
	thedata.endframe		= QString("%1").arg(data[INDEX_EndFrame]).toInt();
    thedata.xpos			= QString("%1").arg(data[INDEX_xpos]).toDouble();
    thedata.ypos			= QString("%1").arg(data[INDEX_ypos]).toDouble();
    thedata.xsize			= QString("%1").arg(data[INDEX_xsize]).toDouble();
    thedata.ysize			= QString("%1").arg(data[INDEX_ysize]).toDouble();
	thedata.frameOffset		= QString("%1").arg(data[INDEX_frameOffset]).toInt();
	thedata.locked			= QString("%1").arg(data[INDEX_lock]).toInt() ? true : false;
	thedata.filmstrip		= QString("%1").arg(data[INDEX_filmstrip]).toInt() ? true : false;
    thedata.xNativeSize		= QString("%1").arg(data[INDEX_xNativeSize]).toDouble();
    thedata.yNativeSize		= QString("%1").arg(data[INDEX_yNativeSize]).toDouble();
    thedata.desktop			= QString("%1").arg(data[INDEX_desktop]);

	//start by setting the in and out point to SOM and EOM
	thedata.clipinframe = thedata.startframe;
	thedata.clipoutframe = thedata.endframe;

	//the frameoffset has to be inside the file!
	if(thedata.frameOffset < 0)
	{
		thedata.frameOffset = 0;
	}
	if((thedata.startframe + thedata.frameOffset) > thedata.endframe )
	{
		thedata.frameOffset = 0;
	}

    switch (thedata.theCategory)
	{
	case VideoCategory::IMAGE:
		thedata.videotex = FALSE;
		break;
	case VideoCategory::CLIP:
		thedata.videotex = TRUE;
		break;
	case VideoCategory::AUDIO:
		thedata.videotex = FALSE;
		break;
	case VideoCategory::MEDIA:
		thedata.videotex = TRUE;
		break;
	case VideoCategory::CLIPKEY:
		thedata.videotex = TRUE;
		break;
	default:  //KEY, CLIPKEY not used
		{
			QString msg = QString("Unknown asset category: %1").arg(data[INDEX_Category]);
			jtrace->warn(msg);
			thedata.theCategory = (VideoCategory::TYPE)-1;
		}
		break;
    }
	
	//TO DO just for debug
	thedata.validate();

    return thedata;
}


/////////////////////////////////////////////////////
// called on exit in order to purge deactivated clips

//the cool part here is when erase doesnt actuall erase the clips... just hides them
//so you just purge the database when youre running out of storage
//instead of a index we can erase all hidden clips and renumber the database from start to finish
// ie set medianumber from 1 to xxx after removing all dead entries and deleting the files
//then we can set globalclipnumber back...

// old notes here

// but first we really need a index field since the clinumber is also part of
// the pathname for rendered clips
// but the general idea is to go through all clips
// erase the ones with a 9
// delete associated files
// and then reindex the clips

void JahDatabase::purgeDatabase( int ) {
/*

    char *zErrMsg = 0;
    int   rc,max_rows,max_cols;
    char  **desktopdata;

    //build the sql command here
    char   *sql; sql = "select * from desktop;"; //where desktop number = 1 for multiple desktops

    rc  = sqlite_get_table(db, sql, &desktopdata, &max_rows, &max_cols, &zErrMsg);

    if( rc!=SQLITE_OK ){ fprintf(stderr, "SQL error: %s\n", zErrMsg); }
                        else { debug("Opened desktop table");  }

    //object data
    QString desktopname    = desktopdata[2];  debug(desktopname);
    QString numberofclips  = desktopdata[3];  debug(numberofclips);

    int numclip = numberofclips.toInt();

    //now we know howmany clips are in the database
    QString conversion;
    QString theclipnum       = conversion.setNum(numclip);

    int counter = GetDatabaseClipNumber();
    QString theclipcounter   = conversion.setNum(counter);

    debug("the clip number read is "+theclipnum);
    debug("the clip counter read   "+theclipcounter);




    int counter;

    int loop=1; //how about bool status here outside the loop

    while ( loop ) { //fix this to keep looping until it breaks

        //build the sql command
        QString theclipnum   = conversion.setNum(loop);  //enum

        //workaround - if theres no name theres no clip
        QString sqlcom = "select MediaName from videodata where MediaNumber="+theclipnum;

        //better way to do this ?
        const char* fontchar =  sqlcom;
        char*  sqlcommand    =  new char[ sqlcom.length() ];
        strcpy(sqlcommand,fontchar);

        char  **videodata;
        int rc,max_rows,max_cols;
    char *zErrMsg = 0;


        rc  = sqlite_get_table(db, sqlcommand, &videodata, &max_rows, &max_cols, &zErrMsg);

        QString clipnum;
        bool status;

        if( rc==SQLITE_OK )  { clipnum = videodata[0];
            if (clipnum == "MediaName") { status=true; } else { status=false; }	
        } else { status=false; }

        if (status) { debug("true");  counter = loop; loop++; } else { break; }
    }

    return counter;

*/

}
