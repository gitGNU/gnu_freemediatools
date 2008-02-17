/*******************************************************************************
**
** The source file for the Jahshaka desktop io file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <iostream>

#include "desktop.h"

#include "jahfilewindow.h"
#include "jahfileiconview.h"
#include "openobjectlib.h"
#include <algorithm>
#include <sstream>
#include <mediatable.h>
#include <jahtracer.h>
#include <jahdatabase.h>


#include <FreeMediaTools.h>

#ifdef _MSC_VER
#pragma warning (disable : 4786) // no big deal, this is a cpp file
#endif

/////////////////////////////////////////////////////////////////////
//used for QString JahDesktop::findModuleName(QString loadfilename)
#include "jahbxmlnode.h"
/////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// asset loader routine for the desktop
void JahDesktop::slotLoadIconview() 
{
    QString result;

    FileMainWindow theloader(this,"theloader",TRUE);
    //std::string current_root_firectory = getCurrentRootDirectory().data();
    std::string current_load_firectory = getCurrentLoadDirectory().data();

	if ( getCurrentRootDirectory() != "" )
    {
		theloader.setPath( getCurrentRootDirectory() );
    }

	if ( getCurrentLoadDirectory() != "" )
    {
		theloader.setPath( getCurrentLoadDirectory() );
        theloader.dirList()->setDir( getCurrentLoadDirectory() );
        theloader.fileView()->setDirectory( getCurrentLoadDirectory() );
        //theloader.cdUp();
    }

    if ( theloader.exec() == QDialog::Accepted ) 
    {
		QValueList < QString > list = theloader.m_selected_items;
		QValueList < QString >::iterator it;

		for ( it = list.begin( ); it != list.end( ); it ++ )
		{
			loadJahAsset( *it );
		    if(qApp->hasPendingEvents())
        		qApp->processEvents();
		}
    }

	setCurrentRootDirectory( theloader.getRoot( ) );
	setCurrentLoadDirectory( theloader.getPath( ) );
}

//the old loader kept for backwards compatability
void JahDesktop::slotLoad() 
{

    //ask user for file now
    QString filename = QFileDialog::getOpenFileName( JahMediaPath+"media", "*.*" , this );
    filename=filename.stripWhiteSpace();

    //user didnt enter anything or cancelled
    if (filename) 
    { 
        loadJahAsset(filename); 
    }
}

///////////////////////////////////////////////////
// these routines are here so we can load scenes in at the desktop level
// however they dont load the scenes to the desktop
// rather tey load them directly into the appropriate module
// and then switch the users ui to the module

bool JahDesktop::isSceneExtension(QString & string)
{
    bool okval = false;

	if (string.lower().endsWith( ".jsf" )) { okval = true; } else
	if (string.lower().endsWith( ".jah" )) { okval = true; }

    return okval;
}

//currently being tested
bool JahDesktop::is3dExtension(QString & string)
{
	//checking for extension limited to 3 char
	QString ext = string.right(3);

	//find out what oolib supports
    OpenObjectLib* oolib = OpenObjectLib::getInstance();
    QString format = oolib->getFormats().data();

	//see if our extension is in the format string
	return format.find( ext, 0 ) != -1;    // i == 1
}

bool JahDesktop::handleJahSceneFile(QString filename ) 
{
    jtrace->debug( ">> handling the scene file:", filename );

	#ifdef JAHPLAYER
	QString theModule = findModuleName(filename);
	
	if (theModule=="Animation")
		emit loadMainModuleFile(filename);
	#endif

	#ifndef JAHPLAYER

		//before loading we need to switch to the respective module
		//but to do that we need to read the file header
		//and figure out what it is

		//1. find the module
		QString theModule = findModuleName(filename);

		if (theModule!="NULL")
        {
			jtrace->debug("found the module");
		    //2. switch to the module
		    emit switchMainModule(theModule);

		    jtrace->debug("loading the file",filename);
		    //3. load the scene file
		    emit loadMainModuleFile(filename);
        }
	#endif

    return true;
}

bool JahDesktop::handle3dFile(QString filename ) 
{
    jtrace->debug( ">> handling the 3d file:", filename );

	#ifdef JAHPLAYER
	emit loadMainModelFile(filename);
	#endif

	#ifndef JAHPLAYER

	//2. switch to the animaiton module
	emit switchMainModule("Animation");

	//3. load the scene file
	emit loadMainModelFile(filename);

	#endif

    return true;
}

///////////////////////////////////////////////////////
// asset loader routine for the desktop
bool JahDesktop::loadJahAsset(QString filename, bool ) 
{
    jtrace->info( ">> Loading Asset into Database:", filename );

    //user didnt enter anything or cancelled
    if (!filename)
        return false;

    //clean filename
    filename=filename.stripWhiteSpace();

    //////////////////////////////////////////////////////////////////
	// This is a hack to load scene files in
	//currently they are not supported on the desktop...
	bool scene_file = isSceneExtension( filename );

    if ( scene_file )
    {
        jtrace->info( "loading scene file:", filename );
        bool status = handleJahSceneFile(filename);

        return status;
    }
    
    //////////////////////////////////////////////////////////////////
	// This is a hack to load 3d files in
	//currently they are not supported on the desktop...
	bool model_file = is3dExtension( filename );

    if ( model_file )
    {
        jtrace->info( "loading 3d file:", filename );
        bool status = handle3dFile(filename);

        return status;
    }
	
	////////////////////////////////////////////////////////
    // deal with media files

	// initialize assetExchange
	assetExchange exchange;

    //get the extension and check to see if its supported
    //uses a duplicate of filename since getExtension also returns 
    //a extension
    bool validextension = exchange.checkExtension(filename);

    //////////////////////////////////////////////////////////////
    //if its not ok then its not supported
    if (!validextension)
    {
        QString errortext = "File type or sequence not supported:\n"+filename;
        errortext +="\n\nLooking for something like 'file.bmp', 'file_0000.BMP', etc.";
        jtrace->debug(errortext);

        QMessageBox::information(this, FMT_PROGRAM_TITLE, errortext);
        return false;
    }

	//////////////////////////////////////////////////
	// attempt to load the asset in now 
	assetData newAsset;

    //grab the clip number and increment
	int clipn; 
	clipn = *clipnumber;
	clipn++;

	bool success=exchange.loadAsset(filename, false, newAsset, clipn);

    if (success)
    {
    	QImage theimage = exchange.getImage(newAsset);
	    *clipnumber = clipn;
	    addAssetToDesktop(newAsset,clipn,true,theimage);
    }

    return success;
}


///////////////////////////////////////////////////////////
// routine to clear item from database

void JahDesktop::clearItem()
{
    //jtrace->info( "Clearing Item");

    ///////////////////////////////////////////////////////////////
    //first we need to make sure that something is selected!
    //trying to erase a null clip causes a crash
    bool clipstatus = false;
    assetData erase = getVideoData(clipstatus);

    if (clipstatus)
    {
        //first we remove the clip from the desktop

        //in order to clear the item from the desktop
        //we need to make usre its selected first!
        //since it could be selected in the table

        bool theeraser;
        theeraser = editor->eraseItem(erase.clipnumber);

        if (theeraser)
        {
        //editor->clearItem(); //should return a true for debugging if successful before moving ahead

        //then we remove it from the table

        //then we grab the clips information for the erase procedure
        //erase =  editor->selectedClip;
        //assetData  erase =  editor->selectedClip;

        //we needed to know what its index was to remove it from the table
        //int tbitem = erase.clipnumber;
        table->removeItem(  erase.clipnumber );

        // tell the media table to also remove item. Clumsy, but media table needs separate calls for now.
        mediatable->removeItem(  erase.clipnumber );

        //then we remove the image from the database
        //currently its just de-activated as we will be doing purging later on and erasing at that time
        theDatabase->removeImagefromdatabase(erase.clipnumber);
        QString conv;
        conv.setNum( erase.clipnumber );
        jtrace->debug("asset removed from database, clip number=" + conv );

        //for safety we dont erase any stock images only images in the renders directory
        //we need to add the option to remove from database if the location is outside of the database
        bool EraseFile = false;

        QString jahrenderpath = JahMediaPath + "/media/renders/"; //renders is renderpath as defined in glworld.cpp

        //we only check to make sure they are in the renders directory...
        //if (jahrenderpath.contains(erase.location))
        if (erase.location.contains(jahrenderpath))
        {
            EraseFile = true;
            jtrace->debug("nice location"+erase.location);
        }
        else
        {
            EraseFile = false; 
            jtrace->debug("bad location:"+erase.location);
        }


        //if we are clear to erase the clip we do so now
        if (EraseFile) 
        {
            if (eraseTheAsset(erase))
            {
                jtrace->debug("erased asset");
            }
        }

        }
    }
}


bool JahDesktop::clearTheDesktop() 
{
    ///////////////////////////////////////////////////////
    //caution using this routine! It wipes the datbase...

    //first we find out how many entries are in the database
    QString data;

    int counter = theDatabase->GetDatabaseClipNumber();
    jtrace->debug("Number of records in database:"+data.setNum(counter));

    //now we iterate through each one and erase the files
    //if they are in the renders directory
    //so we dont erase stuff we are not supposed to

    //we can also clear the desktop and wipe everything using the database...
    //good for corrupt databases
    assetData erase;
    bool EraseFile;

    QString jahrenderpath = JahMediaPath + "/media/renders/"; //renders is renderpath as defined in glworld.cpp

    for (int loop=1; loop<=counter; loop++)
    {
        jtrace->debug(">>erasing clip"+data.setNum(loop));
        EraseFile = false;

        //get the clip from the database
        //jtrace->debug("getting clip from database");
        erase = theDatabase->getImageFromDatabase(loop);

        //we only check to make sure they are in the renders directory...
        //if (jahrenderpath.contains(erase.location)) 
        if (erase.location.contains(jahrenderpath)) 
        { 
            EraseFile = true; 
            jtrace->debug("nice location"+erase.location);
        }
        else
        {
            EraseFile = false; 
            jtrace->debug("bad location:"+erase.location);
        }

        //if we are clear to erase the clip we do so now
        if (EraseFile) 
        {
            if (eraseTheAsset(erase))
            {
                jtrace->debug("erased asset");
            }
        }//end of if

    } //finish looping thorugh files


    //jtrace->debug("finished erasing");
    //////////////////////////////////////////////////////////////////
    //now we wipe the database and set global clipnumber back to 0

    bool wipe = theDatabase->WipeDatabase();

    //after wiping all the clips this clears the desktop
    //need to iterate thru desktop objects and erase them that way
    //this crashes system later on
    editor->clear();

    //need to also clear the desktop table...
    table->clearAll();

    // ... and clear the media table.
    mediatable->clearAll();

    //set globalclipnumber to 0
    *clipnumber = 0;

    return wipe;
}


///////////////////////////////////////////////////////
// asset loader routines for the desktop
// only supports new style xml files

//was based on guillaumes code...
//will eet work now???

QString JahDesktop::findModuleName(QString loadfilename)
{ 
	
    jtrace->debug("deskop triggered load a scene file...");

    std::string const & a_load_filename = loadfilename.data();

	FILE * l_file_pointer = fopen( a_load_filename.data( ) , "rt" ) ;
	if ( l_file_pointer == NULL ) 
	{ 
		jtrace->info( "GLWorld::LoadAll" , "open xml file failed" ) ; 
		return false ; 
	}

	bxmlnode::basicxmlnode * l_xml_root = NULL ;
	l_xml_root = bxmlnode::readbasicxmlnode( l_file_pointer ) ;
	fclose( l_file_pointer ) ;
	if ( l_xml_root == NULL ) 
	{ 
		jtrace->info( "GLWorld::LoadAll" , "read xml failed" ) ; 
		return false ; 
	}

	std::string l_attribute_value ;
	bool l_found_attribute ;
	l_found_attribute = bxmlnode::getAttributeFromThisTag ( 
		l_xml_root ,
		std::string ( "FMTFreeMediaTools" ) ,
		std::string ( "FMT_version" ) ,
		l_attribute_value ) ;

	//some error checking here
	if ( l_attribute_value.find ( "2.0" ) == std::string::npos )
	{
		//this means there was a problem close the file and let the user know
		QMessageBox::information(0, "Error loading Scene file","Sorry, we cant read this Scene file");
		bxmlnode::deletebasicxmlnode( l_xml_root ) ;
		return false ;
	}

	l_found_attribute = bxmlnode::getAttributeFromFirstLevelSubTagOccurence ( 
		l_xml_root ,
		std::string ( "SCENE_FILE" ) ,
		0 ,
		std::string ( "name" ) ,
		l_attribute_value ) ;
    jtrace->debug( "Scene file name",l_attribute_value.data());

	l_found_attribute = bxmlnode::getAttributeFromFirstLevelSubTagOccurence ( 
		l_xml_root ,
		std::string ( "MODULE" ) ,
		0 ,
		std::string ( "name" ) ,
		l_attribute_value ) ;
    jtrace->debug( "Module name",l_attribute_value.data());

	std::string l_module_name = l_attribute_value ;


    bxmlnode::deletebasicxmlnode( l_xml_root ) ;

    return l_module_name.data(); 
}

void JahDesktop::addAssetToDesktop(assetData thedata, int clipn, bool loadit, QImage image )
{
    //now add it to the system
	if(loadit)
	{
		double xsize = 0, ysize = 0;
		addItemToDesktopTable( clipn-1, thedata, image);
		addImage2(thedata, xsize, ysize, image);
		thedata.xsize = xsize;
		thedata.ysize = ysize;
		thedata.xNativeSize = image.width();
		thedata.yNativeSize = image.height();
		thedata.desktop = "desktop";
		theDatabase->addImage2database( thedata );
	}
	else
	{
		theDatabase->addImage2database( thedata );
		//delete from database - sets the clip to be off
		//so it wont reload if we restart
	}

    canvas->update();

}

//why arent we using the assetexchange to get the clip names here?
bool JahDesktop::eraseTheAsset(assetData erase)
{
    bool eraseStatus = false;

    switch(erase.theCategory) {

        case  VideoCategory::IMAGE:    { 
                            jtrace->debug("erasing image "+ erase.location + erase.filename + erase.extension);

                            QFile f(erase.location + erase.filename + erase.extension);
                            if ( f.remove() )  
                            { 
                                //jtrace->debug("erase sucessful");
                                eraseStatus = true; 
                            }
                            else 
                            {
                                //jtrace->debug("couldnt erase file");
                            }

                            break;
                        }

        case  VideoCategory::CLIP :    { 
                            jtrace->debug("erasing clip " + erase.filename);

                            int length, blanklength;
                            QString currentframe, erasefile;

                            //flag that makes sure
                            bool lasterase = false;
                            bool cleanerase = true;

                            for (int i = erase.startframe; i<=erase.endframe; i++) 
                            {

                                // This is where we create the extension for the sequence

                                //first figure out length of current frame
                                currentframe.setNum(i);
                                length = currentframe.length();  //was theblankframe

                                //then remove it from the blank template for the clip
                                QString blank(erase.videoext);
                                blank = blank.remove(0, length);

                                //now we need to build the sequence template
                                blanklength = blank.length();
                                QString theblank;
                                for (int ii=0; ii<blanklength; ii++) 
                                {
                                    theblank +="0";
                                }

                                blank = theblank+currentframe;

                                //then we build the path to the frame and open the file
                                erasefile = erase.location + erase.filename + blank + erase.extension;
                                //jtrace->debug("erasing file:"+erasefile);

                                QFile f(erasefile);

                                //now some error checking on the erase procedure
                                if ( f.remove() )	
                                { 
                                    //jtrace->debug("erase sucessful"); 
                                    lasterase = true; 
                                }
                                else
                                { 
                                    //jtrace->debug("couldnt erase file"); 
                                    cleanerase = false; 
                                }
                            }

                            if (cleanerase == true && lasterase == true) 
                            { 
                                jtrace->info("erased sequence successfully"); 
                                eraseStatus = true;

                                //now we erase the directory if its a clip
                                QString destDir;
                                destDir             = erase.location;

                                QDir d( destDir );                        // "./example"
                                if ( d.exists() )
                                {
                                    //jtrace->debug( "found asset directory", destDir );
                                    if (d.rmdir( destDir ))
                                    {
                                        jtrace->debug("erased sequence directory",destDir );

                                    }
                                    else
                                    {
                                        jtrace->debug("couldnt erase directory",destDir );

                                    }
                                }

                            }
                            break;
                    }

        default :   { 
                        jtrace->debug("dont know what it is"); 
                        break; 
                    }

        } //end of switch here

    return eraseStatus;
}


void JahDesktop::slotImport()
{
    jtrace->info( "Importing Item");

    QString filename;

    FileMainWindow *  theloader = new FileMainWindow (this,"theloader",TRUE);
    //theloader->fileView()->setDirectory( JahMediaPath+"/media/" ); QDir::homeDirPath ()
    theloader->fileView()->setDirectory( QDir::homeDirPath () ); 

    if ( theloader->exec() == QDialog::Accepted ) 
    {
    	QValueList < QString > list = theloader->m_selected_items;
		QValueList < QString >::iterator it;
		for ( it = list.begin( ); it != list.end( ); it ++ )
		{
    		QString filename = *it;
			assetData  theasset;

			//first we load the asset in
			//problem here is it gets added to the database but at the old location
			//not the new imported location in media/imported so we load it as flase
			//a hack so its in the database but not on the desktop and we
			//remove it from the database after were finihsed
			bool status = loadJahAsset(filename,false);

			if (status)
			{

				//grabs the asset, if its not there we get a -1 in theCategory
				theasset = theDatabase->getImageFromDatabase(*clipnumber);

				assetExchange exchange;
				QString assetname = theasset.clipname;
				//debug(assetname);

				if (exchange.isMediaValid(theasset))
				{
					//we actually 'export' the asset to the MediaPath
					//debug("exporting:"+filename+" to:"+JahMediaPath+"media/imported/");

					exchange.ExportAsset(theasset,JahMediaPath+"/media/imported/");

					//delete the old asset from the database
					theDatabase->deleteImagefromdatabase( *clipnumber );
				}
			}
		}
	}

    delete theloader;
}

void JahDesktop::slotExport()
{
    jtrace->info( "Exporting Item");

    //then we grab the clips information for the erase procedure
    assetData  theasset =  editor->selectedClip;


    int clipnum = theasset.clipnumber;

    //if its a good number ie >0 continue
    if (clipnum>0)
    {
        QString destination = QFileDialog::getExistingDirectory(
        QDir::homeDirPath (),
        //prefframe2,
        0,
        "Export Asset",
        "Choose a location to export asset to" );

        QDir d( destination );                        // "./example"

        if ( d.exists() )
        {
            assetExchange exchange;
            QString assetname;

            if (exchange.isMediaValid(theasset))
            {
                exchange.ExportAsset(theasset, destination);
            }
        }
        else
        {
            QMessageBox::information(0, "Error",
            "Invalid location:\n\n"+ destination);
        }
    }
}




/////////////////////////////////////////////////
// save doesnt do much right now

void JahDesktop::slotSave()
{
    //we need to add conversion utilities!!!
    //ie export a jpg sequence as tga...
    jtrace->debug("saving or exporting asset");

    assetData  exporter =  editor->selectedClip;

    int clipnum = exporter.clipnumber;

    //if its a good number ie >0 continue
    if (clipnum>0) 
    {

        QString filename = QFileDialog::getSaveFileName( getCurrentLoadDirectory().data() ,  
                                                        "*" + exporter.extension, 
                                                        this );

        if ( !filename.isEmpty() ) 
        {
            exportTheAsset(filename, exporter);
        }

    }

}

bool JahDesktop::exportTheAsset(QString exportto, assetData theasset)
{
    jtrace->info( "Exporting Asset");
    jtrace->info( "Exporting Asset to"+exportto);
    QString src_filename_qstring = theasset.location + theasset.filename + theasset.extension;;
    std::string src_filename;
    std::string dest_filename;

    if ( src_filename_qstring.data() )
    {
        src_filename = src_filename_qstring.data();
    }

    if ( exportto.data() )
    {
        dest_filename = exportto.data();
    }


    bool exportStatus = false;

    // categories are IMAGE, KEY, CLIP, CLIPKEY, AUDIO, MEDIA
    switch(theasset.theCategory) 
    {
        case  VideoCategory::IMAGE:    
        case  VideoCategory::MEDIA:    
        {
            jtrace->debug("exporting media:"+ theasset.location + theasset.filename + theasset.extension);
            std::string extension_string;

            if ( theasset.extension.data() )
            {
                extension_string = theasset.extension.data();
            }

            std::string dest_extension = 
                dest_filename.substr( dest_filename.length() - extension_string.length(), extension_string.length() );

            if (dest_extension != extension_string)
            {
                dest_filename += extension_string;
            }

            jahstd::copyBinaryFile(dest_filename, src_filename);
            break;
        }

        case  VideoCategory::CLIP :    
        { 
            jtrace->debug("copying clip " + theasset.location + theasset.filename + theasset.extension);
            int number_of_frames = theasset.endframe - theasset.startframe + 1;
            int number_of_numeric_digits = jahstd::numberOfDecimalDigits(number_of_frames);
            //std::string padding_string = 

            std::string original_filename_string = theasset.filename.data();
            std::string extension_string = theasset.extension.data();

            std::string dest_name;
            int last_slash = (int)dest_filename.find_last_of("/");
            int dest_name_length = (int)dest_filename.length() - 1 - last_slash;
            dest_name = dest_filename.substr(last_slash+ 1, dest_name_length);

            QDir dest_directory( dest_filename.data() );

            if ( !dest_directory.exists() )
            {
                dest_directory.mkdir( dest_filename.data() );
            }

            for (int frame_number = 1; frame_number <= number_of_frames; frame_number++)
            {
                int padding_length = number_of_numeric_digits - jahstd::numberOfDecimalDigits(frame_number);
                std::ostringstream padding_stringstream("");

                for (int i = 0; i < padding_length; i++)
                {
                    padding_stringstream << "0";
                }

                padding_stringstream << frame_number << extension_string;
                std::string destination_filename = dest_filename;
                destination_filename += "/";
                destination_filename += dest_name;
                destination_filename += "_";
                destination_filename += padding_stringstream.str();

                std::string source_filename = src_filename;
                int last_underscore = (int)source_filename.find_last_of("_");
                source_filename = source_filename.substr(0, last_underscore); 
                source_filename += "_";
                source_filename += padding_stringstream.str();

                // This would be good, but the data isn't actually saved anywhwere
                //std::string source_filename = theasset.getFrameNameVector()[frame_number];
                bool copied = jahstd::copyBinaryFile(destination_filename, source_filename);

                if (!copied)
                {
                    // Try adding an extra zero in to the name padding
                    int last_underscore = (int)source_filename.find_last_of("_");
                    source_filename.insert(last_underscore + 1, "0");
                    jahstd::copyBinaryFile(destination_filename, source_filename);
                }
            }
 
            break;
        }

        default :   
        { 
            jtrace->debug("dont know what it is"); 
            break; 
         }

    } //end of switch here

    return exportStatus;
}



