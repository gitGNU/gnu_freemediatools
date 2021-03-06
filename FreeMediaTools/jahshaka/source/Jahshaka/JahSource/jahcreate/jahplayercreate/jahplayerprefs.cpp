/*******************************************************************************
**
** The source file for the Jahshaka jahcontrol module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayercreate.h"
#include "anime.h"
#include <qslider.h>
#include <qcheckbox.h>

////////////////////////////////////////////////////////
//Jahshaka Prefernces Routines
//need to break these out into a separate file...


void JahControl::setStoragePath(void) 
{
    int status = QMessageBox::critical( 0, "Setting the Storage Path",
                                        "You are about to change the Storage Path!\n\n"
                                        "do so with caution, and only if you have\n"
                                        "copied your media folder to the new location!\n\n"

                                        "You will need to restart for changes to take effect.\n\n"
                                        "Do you still want to Proceed?\n",
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::No,
                                        QMessageBox::Cancel | QMessageBox::Escape );


    switch (status) {

    case QMessageBox::Yes    : {

		std::string thepath = StorageBox->text().data();
		std::string thepath2 = thepath + "media";

		QDir d( thepath2.data() );                        // "./example"

		if ( d.exists() )
		{
			JahMediaPath = thepath.data();

			JahPrefs& jprefs = JahPrefs::getInstance();
			jprefs.setMediaPath(JahMediaPath.data());
		}
		else
		{
				//let them know whats up
			std::string l_messg = "No media folder found at\n\n";
			l_messg += thepath;
			QMessageBox::information(0, "Error", l_messg.data() );

			//restore settings
			JahPrefs& jprefs = JahPrefs::getInstance();
			StorageBox->setText(jprefs.getMediaPath().data());
		}


                                break; }

    case QMessageBox::No     : { break; }

    case QMessageBox::Cancel : { break; }

    default :                  { break; }

}

}

void JahControl::getStoragePath(void)
{ 
        std::string s = QFileDialog::getExistingDirectory(
							QDir::homeDirPath (),
							0,
							"Set Jahshaka Storage Space",
							"Choose a Directory for Media & Scene Storage" ).data();


		std::string s2 = s + "media";
        QDir d( s2.data() );                        // "./example"

        if ( d.exists() )
        {
                StorageBox->setText(s.data());
                std::string thepath = StorageBox->text().data();

                JahPrefs& jprefs = JahPrefs::getInstance();
                jprefs.setMediaPath(JahMediaPath.data());
        }
        else
        {
			std::string l_messg = "No media folder found at\n\n";
			l_messg += s;

                QMessageBox::information(0, "Error", l_messg.data() );
        
        }
}

void JahControl::setGlobalRenderQuality(void)
{

    //read the slider setting
    int quality = renderqualityslider->value();

    if ( quality<0) { jahrenderquality=2; }
    else if ( quality>2) { jahrenderquality=2; }
    else { jahrenderquality = quality; }

    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    jprefs.setRenderQuality(jahrenderquality);


    //update all the modules now
    setModulesRenderQuality();


}

//change the name here to set stored settings for prefs
void JahControl::setModulesRenderQuality(void) 
{
    //update all the modules with render quality settings and videohead settings
    JahAnimationMod->setRenderQuality(jahrenderquality);
   

}

//change the name here to get stored settings for prefs
void JahControl::getGlobalPreferences(void)
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    jahrenderquality  = jprefs.getRenderQuality();

    rememberprefs     = jprefs.getRememberGeometry();

    dualheadprefs     = jprefs.getDualhead();

    videohead         = jprefs.getVideohead();

    menuprefs         = true;

    renderer          = jprefs.getRenderer();

    widescreenprefs        = jprefs.getWidescreen();

    jahStyleprefs     = jprefs.getStyle();

}


void JahControl::setStyleOption(void)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    jahStyleprefs = Pref10->isChecked();

    jprefs.setStyle(jahStyleprefs);
}

void JahControl::setGlobalPositionSize(void)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    rememberprefs = PositionPrefs->isChecked();

    jprefs.setRememberGeometry(rememberprefs);
}

void JahControl::setVideoHeadOption(void)
{
#if 0
    JahPrefs& jprefs  = JahPrefs::getInstance();

    videohead = VideoHead->isChecked();

    jprefs.setVideohead(videohead);

    if (videohead)
    {
        JahPlayerMod->videohead = true;
        JahPlayerMod->launchVideoHead();
        JahPlayerMod->activateVideoHead();

        //displayVideo2startupImage();

        updateModulesVideohead(true);
    }
    else
    {
        JahPlayerMod->deactivateVideoHead();
        updateModulesVideohead(false);
    }
#endif
}

void JahControl::setWidescreenOption(void)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    widescreenprefs = WideScreenBox->isChecked();

    jprefs.setWidescreen(widescreenprefs);

    //widescreen = widescreenprefs;
}


void JahControl::setDualHeadOption(void)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    dualheadprefs = DualHead->isChecked();

    jprefs.setDualhead(dualheadprefs);
}

void JahControl::setMenuOption(void)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    menuprefs = MenuOption->isChecked();

    jprefs.setMenubars(menuprefs);
}

void JahControl::setRenderer(int renval)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

    jprefs.setRenderer(renval);
}

void JahControl::setLanguage(const QString &renval)
{
    JahPrefs& jprefs  = JahPrefs::getInstance();
    JahTranslate &jt = *JahTranslate::getInstance();

    jprefs.setLanguage(renval.ascii());
    jt.setLanguage(renval.ascii());
}

void JahControl::setResolution(int renval)
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    jprefs.setJahResolution(renval);	
}


///////////////////////////////////////////////////////////////////
//save and restore moudles iu
void JahControl::saveModulesUI(void)
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    for (int i=0; i<module_counter; i++)
    {
        jprefs.setModulesUI(i,  MainModuleControlerLeftFrameWidth[i],
                                MainModuleControlerCenterFrameWidth[i],
                                MainModuleControlerRightFrameWidth[i]);

    }
}

void JahControl::restoreModulesUI(void)
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs  = JahPrefs::getInstance();

    for (int i=0; i<module_counter; i++)
    {
        jprefs.getModulesUI(i,  MainModuleControlerLeftFrameWidth[i],
                                MainModuleControlerCenterFrameWidth[i],
                                MainModuleControlerRightFrameWidth[i]);

    }
}

