/*******************************************************************************
**
** The source file for the Jahshaka glworld dataio code
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#ifndef JAHPLAYER
#include "jahscript.h"
#endif

#include <qfiledialog.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <jahdataio.h>
#include <widget.h>

#include <string>
#include <assetexchange.h>

#include "eyeCheckListItem.h"


template < typename T > 
QCheckListItem* 
factory___( T* parent, QListViewItem* after, const QString& s )
{
    if ( after )
	return new EyeCheckListItem( parent, after, s, QCheckListItem::CheckBox );

    return new EyeCheckListItem( parent, s, QCheckListItem::CheckBox );
}


///////////////////////////////////////////////////////////////
// file load routine

void GLWorld::SceneLoad()
{
    QString file_name = QFileDialog::getOpenFileName( m_qsScenePath, "*.jsf", this );

    if ( file_name.isEmpty() )
    {
        return;
    }

    std::string file_name_string = file_name.ascii();
    char* scene_file_buffer;

    JahDataIO::readFileIntoBuffer(scene_file_buffer, (char*)file_name_string.c_str() );
    std::string scene_file_string = scene_file_buffer;

    //TODO : what a hack
    if (scene_file_string.find("<FREEMEDIATOOLS") != std::string::npos)
    {
       LoadAll(scene_file_string, true, false, file_name_string);
    }
}

void GLWorld::SceneAppend()
{
    QString file_name = QFileDialog::getOpenFileName( m_qsScenePath, "*.jsf", this );

    if ( file_name.isEmpty() )
    {
        return;
    }

    std::string file_name_string = file_name.ascii();
    char* scene_file_buffer;

    JahDataIO::readFileIntoBuffer(scene_file_buffer, (char*)file_name_string.c_str() );
    std::string scene_file_string = scene_file_buffer;


    if (scene_file_string.find("<FREEMEDIATOOLS") != std::string::npos)
    {
       LoadAll(scene_file_string, false, false, file_name_string);
    }
}

void GLWorld::importFx()
{
    QString file_name = QFileDialog::getOpenFileName( m_qsScenePath, "*.jsf", this );

    if ( file_name.isEmpty() )
    {
        return;
    }

    std::string file_name_string = file_name.ascii();
    char* scene_file_buffer;

    JahDataIO::readFileIntoBuffer(scene_file_buffer, (char*)file_name_string.c_str() );
    std::string scene_file_string = scene_file_buffer;


    if (scene_file_string.find("<FREEMEDIATOOLS") != std::string::npos)
    {
       LoadAll(scene_file_string, false, true, file_name_string);
    }
}


bool GLWorld::SceneLoadName(QString file_name)
{
    bool status = false;

    if ( file_name.isEmpty() )
    {
        return false;
    }

    std::string file_name_string = file_name.ascii();
    char* scene_file_buffer;

    JahDataIO::readFileIntoBuffer(scene_file_buffer, (char*)file_name_string.c_str() );
    std::string scene_file_string = scene_file_buffer;


    if (scene_file_string.find("<FREEMEDIATOOLS") != std::string::npos)
    {
       status = LoadAll(scene_file_string, true, false, file_name_string);
    }

    return status;
}

bool GLWorld::ModelLoadName(QString file_name)
{
    //first clear the module and reset variables
    ClearAll();

	//figure out filename and extension
    QString     objfilename,objextension;
            
    char l_file_without_extension[JahDataIO::c_max_path];
    JahDataIO::stripExtension ( l_file_without_extension, file_name.data() );
    objfilename = l_file_without_extension;
    
    char l_file_extension[JahDataIO::c_max_path];
    JahDataIO::getExtension ( l_file_extension, file_name.data() );
    objextension = ".";
    objextension += l_file_extension;

	//create the layer 
    LayerListEntry* new_layer_list_entry;
    JahLayer* new_layer = NULL;

    new_layer_list_entry = new LayerListEntry("3DObject");
    new_layer = new_layer_list_entry->thelayer;
    getLayerList()->append(new_layer_list_entry);

    new_layer->blend    = TRUE;
    new_layer->depth = TRUE;
    new_layer->lighting = TRUE;
    new_layer->objfilename = objfilename;
    new_layer->objextension = objextension;
	new_layer->loadObject = true;
   
	//and load the model in
	buildLayer(new_layer, "3DObject", LayerCategory::OBJECT, ObjectCategory::OBJECT );
	new_layer->setCameraLayer( getFirstJahLayer() );
	updateGL();

	#ifndef JAHPLAYER
    //now create the ui layer node for the listbox
    new_layer->layerAssetStatus = TRUE;
    clearSelection(m_pLayerListView);
    QCheckListItem* new_check_list_item = 
      (QCheckListItem*)addCheckListItem(
					m_pLayerListView, 
					TRUE, 
					false, 
					&factory___<QListView>);
    new_layer->setCheckListItem(new_check_list_item);
    new_check_list_item->setSelected(true);
    new_check_list_item->setOn(true);
    
	//now we can update the ui if not in the player
    setObjectTab( getLayerList()->last()->getJahLayer());
    updateSliders();
	setDefaultAxisSliderLabels();
    updateKeyframeDisplay();
	#endif

    return true;
}


///////////////////////////////////////////////////////////////
// file save routine
 
void 
GLWorld::sceneSave()
{
    //QString fn = QFileDialog::getSaveFileName( JahBasePath+"scenes", "*.jsf",  this );
    QString name = QFileDialog::getSaveFileName( m_qsScenePath, "*.jsf",  this );

    if ( getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "Auto save in progress - try again in a few seconds", name);
        return;
    }

    if ( !name.isEmpty() ) 
    {
        if (!name.endsWith(".jsf"))
        {
			name += ".jsf";
		}

		std::string file_name;
		file_name = name.ascii();
        setSceneSaveExport(false);
        saveSceneFile(file_name);
    }
    //else
    //{
    //    QMessageBox::information(0, "Saving aborted", name);
    //}
}

#ifndef JAHPLAYER
void
GLWorld::runScript(std::string& script_name)
{
    m_bAnimation = false;
    setStopScript(false);
    JahScript* script = new JahScript(script_name);
    script->executeScriptFileVector( getJahControl() );
    delete script;
}
#endif

void
GLWorld::stopScript()
{
    setStopScript(true);    
    getStopButton()->hide();
}

void  
GLWorld::getScript()
{
    QString first_load_dir = m_qsJahMediaPath + "/scripts";
    static std::string last_load_dir = first_load_dir.data();
    QString load_path = last_load_dir.data();

    QString name = QFileDialog::getSaveFileName( load_path, "*.jsc",  this );

    if ( getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "Auto save in progress - try again in a few seconds", name);
        return;
    }

    if ( name.isEmpty() ) 
    {
        QMessageBox::information(0, "No Script Specified", name);
		return;
    }

    std::string script_name = name.data();
    int last_slash = (int)script_name.find_last_of("/");
    last_load_dir = script_name.substr(0, last_slash);

#ifndef JAHPLAYER
    runScript(script_name);
#endif
}

//////////////////////////////////////////////////////////////
// routines to export a package to the desktop
// need to add the ability to 'tar' the finished folder

void 
GLWorld::saveAsExport()
{
    //QString fn = QFileDialog::getSaveFileName( JahBasePath+"scenes", "*.jsf",  this );
    QString name = QFileDialog::getSaveFileName( m_qsScenePath, "*.jsf",  this );

    if ( getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "Auto save in progress - try again in a few seconds", name);
        return;
    }

    if ( !name.isEmpty() ) 
    {
        if (!name.endsWith(".jsf"))
        {
			name += ".jsf";
		}

		std::string file_name;
		file_name = name.ascii();
        setSceneSaveExport(true);
        saveSceneFile(file_name);
    }
    else
    {
        QMessageBox::information(0, "Saving aborted", name);
    }
}

void GLWorld::PackageSave()
{
    QString name = QFileDialog::getSaveFileName( m_qsJahMediaPath+"/scenes", "*.*",  this );

    if ( getAutoSaveInProgress() )
    {
        QMessageBox::information(0, "Auto save in progress - try again in a few seconds", name);
        return;
    }

    if ( name.isEmpty() ) 
    {
        QMessageBox::information(0, "Saving aborted", name);
		return;
    }

    std::string name_string = name.data();
    int last_slash = (int)name_string.find_last_of("/");
    std::string directory_name = name_string.substr(0, last_slash);
    std::string package_name = name_string.substr(last_slash + 1, name_string.length() - last_slash);

    if ( package_name.length() > 4 )
    {
        std::string package_name_ending = package_name.substr(package_name.length() - 4, 4);

        if (package_name_ending == ".jsf")
        {
            package_name = package_name.substr(0, package_name.length() - 4);
            last_slash = (int)directory_name.find_last_of("/");
            directory_name = directory_name.substr(0, last_slash);
        }
    }

    directory_name = directory_name + "/" + package_name;
    std::string scene_file_name = directory_name + "/" + package_name + ".jsf";

    QString destination_dir = directory_name.data();

    QDir package_directory(destination_dir);

    bool everything_is_ok = false;

    if ( !package_directory.exists() )
    {
        if ( package_directory.mkdir(destination_dir) )
        {
            everything_is_ok = true;
        }
    }
    else
    {
        everything_is_ok = true;
    }

    if (everything_is_ok)
    {
        if ( scene_file_name.length() )
        {
            setSceneSaveExport(true);
			saveSceneFile(scene_file_name);
            SaveAllAssets(destination_dir);
        }
    }
    else
    {
        QMessageBox::information(0, "Error Packaging ", name);
    }

}

void GLWorld::SaveAllAssets(QString destination)
{
    QString assetname;
    assetExchange exchange;

    for (int i = 0; i < int( getLayerList()->count() ); i++) 
    {
		JahLayer* jah_layer = getJahLayer(i);

        if ( getLayerList()->at(i) )
        {
            //now write the objects information out
            if (jah_layer->layerAssetStatus)
            {
                //bool copyit = false;
                if (exchange.isMediaValid(jah_layer->asset))
                {
                    exchange.getMediaFilename(jah_layer->asset, 0, assetname);
                    exchange.ExportAsset( jah_layer->asset, destination);
                    //printf("GLWorld::SaveAllAssets exported %s to %s \n", assetname.data(), destination.data() );
                }
            }

            if (jah_layer->keyAssetStatus)
            {
                //bool copyit = false;
                if (exchange.isMediaValid(jah_layer->keyasset))
                {
                    exchange.getMediaFilename(jah_layer->keyasset, 0, assetname);
                    exchange.ExportAsset( jah_layer->keyasset, destination);
                    printf("GLWorld::SaveAllAssets exported %s to %s \n", assetname.data(), destination.data() );
                }
            }

            if (jah_layer->objtype == ObjectCategory::JAHTEXT)
            {
                std::string font_path = jah_layer->text->font.data();
                int last_slash = (int)font_path.find_last_of("/");
                std::string font_name = font_path.substr(last_slash, font_path.length() - last_slash);
                std::string dest_path = destination.data() + font_name;
                jahstd::copyBinaryFile(dest_path, font_path);
                printf("GLWorld::SaveAllAssets copied %s to %s \n", font_path.data(), dest_path.data() );
            }

            if (jah_layer->objtype == ObjectCategory::OBJECT)
            {
                std::string model_path = jah_layer->objfilename.data();
                model_path = model_path + jah_layer->objextension.data();
                int last_slash = (int)model_path.find_last_of("/");
                std::string model_name = model_path.substr(last_slash, model_path.length() - last_slash);
                std::string dest_path = destination.data() + model_name;
                jahstd::copyBinaryFile(dest_path, model_path);
                printf("GLWorld::SaveAllAssets copied %s to %s \n", model_path.data(), dest_path.data() );
            }
        }
    }
 
}  // save scene files to disk



void GLWorld::locateLoadAsset(assetData& asset_data, std::string const& file_name_string)
{
    QString assetname;
    assetExchange exchange;

    //bool copyit = false;
    if (exchange.isMediaValid(asset_data))
    {
		//debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MEDIA VALID");  
		return;
    }


	///////////////////////////////////////////////////////////////
	// media is invalid lets try to find it

	//grab this here in case we need to restore it
	QString asset_location = asset_data.location;

		//debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MEDIA INVALID");

		//we need to try to locate the media before returning

		//dont know how to do this with std::string so we use QString for now
		QString scene_file_location(file_name_string.c_str( ));
		//debug(scene_file_location);
		//this is c:/folder/folder/filename.jsf

		////////////////////////////////////////////////////////////////////
		// 1. check the folder the folder the scene file is in first
		// take scene_file_location and remove the load file to get dir
		int i;
		
		i = scene_file_location.findRev( "/" );  //-1 means couldnt find it

		//need to figure this out for some filesystems
		if (i == -1)
			i = scene_file_location.findRev( "\\" );  

		//means we cant figure out the directory location
		if (i == -1) return;

		//we want the /
		i += 1;

		QString new_location = scene_file_location.left( i );   
		//debug(new_location);

		////////////////////////////////////////////////////////////////////
		// 2. update asset location and test
		asset_data.location = new_location;

		if (exchange.isMediaValid(asset_data))
		{
			//debug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>MEDIA VALID");  
			return;
		}
	
		////////////////////////////////////////////////////////////////////
		// 3. test both JahBasePAth and m_qsJahMediaPath

		////////////////////////////////////////////////////////////////////
		// 4. possibly ask user to locate media or build list of
		//    missing files?

		////////////////////////////////////////////////////////////////////
		// 5. cant find the media so restore old asset location and return 
		asset_data.location = asset_location;

}  
