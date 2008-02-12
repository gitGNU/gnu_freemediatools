/*******************************************************************************
**
** The header file for the Jahshaka jahcalc module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHDATABASE
#define JAHDATABASE

#include <qstring.h>

#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qtable.h>
#include <qdropsite.h>

#include "openmedialib.h"

#include "jahtracer.h"
#include "jahpreferences.h"

struct sqlite3;

class JahDatabase
{

public:
    JahDatabase();
    ~JahDatabase();


    ////////////////////////////////////////////////////////
    //Initialize and Open the database

    static std::string locateDatabase();
    void InitializeDatabase(void);
    void CloseDatabase(int clipnum);

    int  TestDatabase(void);
    bool WipeDatabase(void);

    void updateClipPosition(assetData data);
    void updateClipSize(assetData data);
	void updateLockStatus(assetData data);
	void updateFilmstripStatus(assetData data);
	void updateFrameOffset(assetData data);

    int OpenDesktopTable();

    int GetDatabaseClipNumber();

    void addImage2database( assetData thedata );

    void removeImagefromdatabase( int imagenumber );
    void deleteImagefromdatabase( int imagenumber );
    void renameImageindatabase( int imagenumber, QString newname);

    assetData getImageFromDatabase(int clipnumber);
    bool doesAssetExistindatabase(int clipnum);

    assetData restoreImageFromDatabase(int clipnumber);

    void purgeDatabase( int imagenumber );

    //int *clipnumber;

	sqlite3 *getDatabase( ) { return db; }

	bool addDesktop(const char* desktopName);
	bool removeDesktop(const char* desktopName);

private:
	void dropDatabase();
	void createDatabase();

	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

    /////////////////////////////////////////////////
    //the embedded database
    sqlite3*         db;

};

#endif // FORM1_H
