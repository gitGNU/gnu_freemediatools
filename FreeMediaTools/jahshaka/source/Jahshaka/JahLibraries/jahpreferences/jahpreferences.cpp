/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

//#include <string>
//#include <iostream>

#include "jahpreferences.h"

/////////////////////////////////////////////////////////////////////
//we should make this routine save the prefs and load the prefs in 
//to make things a bit cleane rmon

JahPrefs JahPrefs::_instance ;

JahPrefs::JahPrefs() 
    throw()
{   
    //these dont actually need to be initialized   
    BasePath            = "invalid";
    MediaPath           = "invalid";

    JahRenderQuality    = 2;          //default is BMP
    JahRenderer         = 0;          //default is HARDREN

    //these guys default to qt rendering till we see if the new
    //hardware renderer works
	
	//#ifdef JAHIRIX
	//	JahRenderer = 1;
	//#endif

	#ifdef JAHDARWIN
		JahRenderer = 1;
	#endif

	//this becomes
	jahResolution = 3;		//default to D1 NTSC

	////////////////////////////////////////
	// ui settings
    dualHead            = false;
    wideScreen          = true;
    videoHead           = false;
    videoHeadX          = 0;
    videoHeadY          = 0;
	quickStart			= false;
    rememberGeometry    = true;
	menuBars			= true;
    
	XPos                = 0;    
    YPos                = 0;     
    JahWidth            = 1024;
    JahHeight           = 768;
	JahMaximized		= false;

    XPos2               = 0;    
    YPos2               = 0;     
    JahWidth2           = 1024;
    JahHeight2          = 768;
	JahMaximized2		= false;

    HwFog               = false;
    HwAliasing          = false;
    vertexshaders       = false;
    gpusupport          = false;
    nv_gpusupport       = false;

    foundPrefs          = FALSE;

    language            = "English";

    jahstyle            = FALSE;
    themestyle          = "NULL";
    themecolor          = -1;

	//for the jahnet server
	autoLaunchserver	= FALSE;

    jtbonlineurl = "http://www.jahshaka.org/jahguide/index.php";

    //for the modules ui
    //currently hardcoded to 13 modules
    module_counter = 13;
    for (int i=0; i<module_counter; i++)
    {
            MainModuleControlerLeftFrameWidth[i] = 0;
            MainModuleControlerCenterFrameWidth[i]=600;
            MainModuleControlerRightFrameWidth[i]=0;
    }


}

JahPrefs::~JahPrefs() 
    throw()
{

}

JahPrefs& JahPrefs::getInstance()
    throw()
{
    return (_instance);
}

void JahPrefs::savePreferences(void)
    throw()
{
    //debug( "Storing the global preferences" );

    QSettings settings;
    #ifndef __sgi
	#ifndef JAHPLAYER
    settings.setPath( "gnu.org", "FreeMediaTools" );
    //fprintf(stderr, "save: set the settings path\n");
	#else
    settings.setPath( "jahplayer.com", "jahplayer" );
	#endif
	#endif

    // <JAHSHAKA>
	#ifndef JAHPLAYER
    settings.beginGroup("/jahshaka");
    #else
    // <JAHPLAYER>
    settings.beginGroup("/jahplayer");
    #endif

    // <DESKTOP>
    settings.beginGroup("/desktop");
    settings.writeEntry( "/media_path",     MediaPath.c_str() );
    settings.writeEntry( "/render_quality",     JahRenderQuality );
    settings.writeEntry( "/quick_start",     quickStart );
    settings.writeEntry( "/remember_geometry",     rememberGeometry );
    settings.writeEntry( "/dual_head",     dualHead );
    settings.writeEntry( "/video_head",     videoHead );
    settings.writeEntry( "/style",     jahstyle );
    settings.writeEntry( "/resolution",     jahResolution );
    settings.writeEntry( "/menu_bars",     menuBars );
    settings.writeEntry( "/theme_style",     themestyle.c_str() );
    settings.writeEntry( "/theme_color",    themecolor );
    settings.writeEntry( "/language",    language.c_str() );
    settings.writeEntry( "/widescreen",    wideScreen );
    settings.writeEntry( "/reserved1",    false );  
    settings.writeEntry( "/auto_launch_server",    autoLaunchserver );
    settings.writeEntry( "/version",    JahVersion.c_str() );
    // </DESKTOP>
    settings.endGroup();
    // <GEOMETRY>
    settings.beginGroup("/geometry");
    //application geometry
    settings.writeEntry( "x",         XPos );
    settings.writeEntry( "y",         YPos );
    settings.writeEntry( "width",     JahWidth );
    settings.writeEntry( "height",    JahHeight );
    settings.writeEntry( "maximized", JahMaximized );
    //dualhead geometry
    settings.writeEntry( "x2",         XPos2 );
    settings.writeEntry( "y2",         YPos2 );
    settings.writeEntry( "width2",     JahWidth2 );
    settings.writeEntry( "height2",    JahHeight2 );
    settings.writeEntry( "maximized2", JahMaximized2 );
    //videohead geometry
    settings.writeEntry( "x3",     videoHeadX );
    settings.writeEntry( "y3",     videoHeadY );
    // </GEOMETRY>
    settings.endGroup();
    // <NETWORK>
    settings.beginGroup("/network");
    settings.writeEntry("/online_url", jtbonlineurl.c_str());
    // </NETWORK>
    settings.endGroup();
    
    // <MODULESUI>
    settings.beginGroup("/modulesui");
    settings.writeEntry("/module_counter", module_counter);

    for (int i=0; i<module_counter; i++)
    {
        QString ext,name; 
        ext.setNum(i);

        name="/module_left"+ext;
        settings.writeEntry(name, MainModuleControlerLeftFrameWidth[i]);
        name="/module_center"+ext;
        settings.writeEntry(name, MainModuleControlerCenterFrameWidth[i]);
        name="/module_right"+ext;
        settings.writeEntry(name, MainModuleControlerRightFrameWidth[i]);
    }
    // </MODULESUI>
    settings.endGroup();
    
    // </JAHSHAKA>
    settings.endGroup();
}

void JahPrefs::saveVideoheadPreferences(void)
    throw()
{
//    debug( "Storing the global preferences" );

    QSettings settings;
    #ifndef __sgi
	#ifndef JAHPLAYER
    settings.setPath( "jahshaka.com", "jahshaka" );
	#else
    settings.setPath( "jahplayer.com", "jahplayer" );
	#endif
	#endif

    // <JAHSHAKA>
	#ifndef JAHPLAYER
    settings.beginGroup("/jahshaka");
    #else
    // <JAHPLAYER>
    settings.beginGroup("/jahplayer");
    #endif

    // <JAHSHAKA>
    settings.beginGroup("/dualgeometry");

    settings.writeEntry( "/x3",     videoHeadX );
    settings.writeEntry( "/y3",     videoHeadY );

    settings.endGroup();
    settings.endGroup();

}

void JahPrefs::loadPreferences(void)
    throw()
{

    //debug( "loading the global preferences" );
    bool itworked;

    QSettings settings;
    #ifndef __sgi
	#ifndef JAHPLAYER
    settings.setPath( "jahshaka.com", "jahshaka" );
	#else
    settings.setPath( "jahplayer.com", "jahplayer" );
	#endif
	#endif

    int intval   = 0;
    bool boolval = 0;
    QString strval;

    // <JAHSHAKA>
	#ifndef JAHPLAYER
    settings.beginGroup("jahshaka");
    #else
    // <JAHPLAYER>
    settings.beginGroup("jahplayer");
    #endif

    // <DESKTOP>
    settings.beginGroup("desktop");
    strval = settings.readEntry( "/media_path", QString::null, &itworked );
    if (itworked) MediaPath = strval.ascii();

    intval = settings.readNumEntry( "/render_quality", intval, &itworked );
    if (itworked) JahRenderQuality = intval;

    boolval = settings.readBoolEntry( "/quick_start", boolval, &itworked );
    if (itworked) quickStart = boolval;

    boolval = settings.readBoolEntry( "/remember_geometry", boolval, &itworked );
    if (itworked) rememberGeometry = boolval;

    boolval = settings.readBoolEntry( "/dual_head", boolval, &itworked );
    if (itworked) dualHead = boolval;

    boolval = settings.readBoolEntry( "/video_head", boolval, &itworked );
    if (itworked) videoHead = boolval;

    ///////////////////////////////////////////////////////////////////////////
    //we can use these to save the style...

    boolval = settings.readBoolEntry ( "/style", boolval, &itworked );
    if (itworked) jahstyle = boolval;

    intval = settings.readNumEntry ( "/resolution", intval, &itworked );
    if (itworked) jahResolution = intval;

    boolval = settings.readBoolEntry( "/menu_bars", boolval, &itworked );
    if (itworked) menuBars = boolval;

    strval = settings.readEntry ( "/theme_style", QString::null, &itworked );
    if (itworked) themestyle = strval.ascii();

    intval = settings.readNumEntry ( "/theme_color", intval, &itworked );
    if (itworked) themecolor = intval;

    strval = settings.readEntry ( "/language", QString::null, &itworked );
    if (itworked) language = strval.ascii();

    boolval = settings.readBoolEntry ( "/widescreen", boolval, &itworked );
    if (itworked) wideScreen = boolval;

    boolval = settings.readBoolEntry ( "/auto_launch_server", boolval, &itworked );
    if (itworked) autoLaunchserver = boolval;

    // </DESKTOP>
    settings.endGroup();

    // <GEOMETRY>
    settings.beginGroup("/geometry");
    intval = settings.readNumEntry ( "/x", intval, &itworked );
    if (itworked) XPos = intval;

    intval = settings.readNumEntry ( "/y", intval, &itworked );
    if (itworked) YPos = intval;

    intval = settings.readNumEntry ( "/width", intval, &itworked );
    if (itworked) JahWidth = intval;

    intval = settings.readNumEntry ( "/height", intval, &itworked );
    if (itworked) JahHeight = intval;

    boolval = settings.readBoolEntry ( "/maximized", boolval, &itworked );
    if (itworked) JahMaximized = boolval;

    //for dualhead
    intval = settings.readNumEntry ( "/x2", intval, &itworked );
    if (itworked) XPos2 = intval;

    intval = settings.readNumEntry ( "/y2", intval, &itworked );
    if (itworked) YPos2 = intval;

    intval = settings.readNumEntry ( "/width2", intval, &itworked );
    if (itworked) JahWidth2 = intval;

    intval = settings.readNumEntry ( "/height2", intval, &itworked );
    if (itworked) JahHeight2 = intval;

    boolval = settings.readBoolEntry ( "/maximized2", boolval, &itworked );
    if (itworked) JahMaximized2 = boolval;

    settings.endGroup();

    //for videohead
    settings.beginGroup("/dualgeometry");
    
    intval = settings.readNumEntry ( "/x3", intval, &itworked );
    if (itworked) videoHeadX = intval;

    intval = settings.readNumEntry ( "/y3", intval, &itworked );
    if (itworked) videoHeadY = intval;

    // </GEOMETRY>
    settings.endGroup();

    // <NETWORK>
    settings.beginGroup("/network");

    strval = settings.readEntry("/online_url", QString::null, &itworked);
    if (itworked) jtbonlineurl = strval.ascii();

    // </NETWORK>
    settings.endGroup();


    // <MODULESUI>
    settings.beginGroup("/modulesui");

    intval = settings.readNumEntry ( "/module_counter", intval, &itworked );
    //if (itworked) module_counter = intval;

    for (int i=0; i<module_counter; i++)
    {
        QString ext,name; 
        ext.setNum(i);

        name="/module_left"+ext;
        intval = settings.readNumEntry ( name, intval, &itworked );
        if (itworked) MainModuleControlerLeftFrameWidth[i] = intval;

        name="/module_center"+ext;
        intval = settings.readNumEntry ( name, intval, &itworked );
        if (itworked) MainModuleControlerCenterFrameWidth[i] = intval;

        name="/module_right"+ext;
        intval = settings.readNumEntry ( name, intval, &itworked );
        if (itworked) MainModuleControlerRightFrameWidth[i] = intval;
    }

    // </MODULESUI>
    settings.endGroup();

    // </JAHSHAKA>
    settings.endGroup();

    //if it didnt work then we didnt find the preferences
    foundPrefs = true;

}

void JahPrefs::checkVersion(void)
    throw()
{
    bool itworked;
    QString strval;

    //initialize QSettings
    QSettings settings;
    #ifndef __sgi
	#ifndef JAHPLAYER
    settings.setPath( "jahshaka.com", "jahshaka" );
	#else
    settings.setPath( "jahplayer.com", "jahplayer" );
	#endif
	#endif

    //define applicaiton group
	#ifndef JAHPLAYER
    settings.beginGroup("jahshaka"); // <JAHSHAKA>
    #else
    settings.beginGroup("jahplayer"); // <JAHPLAYER>
    #endif

    // jump into desktop settings
    settings.beginGroup("desktop");

    //see whats stored
    strval = settings.readEntry( "/version", QString::null, &itworked );

    //back out
    settings.endGroup(); // </DESKTOP>
    settings.endGroup(); // </JAHSHAKA>


	//if versions dont match up then the prefs are old
	//and need to be updated before moving ahead
    if (strval.isEmpty() )
    {
        //nothing stored
		savePreferences();
    }
    else 
	if (JahVersion != strval.ascii() )
	{
        //versions dont match up
		savePreferences();
	}
}

/////////////////////////////////////////////////////////////////
//routines to set data start here

void JahPrefs::setJahVersion(std::string jahver)  { JahVersion       = jahver; }

void JahPrefs::setJahResolution(int xres)   { jahResolution         = xres;  }
//void JahPrefs::setXRes(int xres)          { XResolution         = xres;  }
//void JahPrefs::setYRes(int yres)          { YResolution         = yres;  }
//void JahPrefs::setRatio(QString ratio)    { Ratio               = ratio; }

void JahPrefs::setBasePath(std::string path)  { BasePath            =  path; }
void JahPrefs::setMediaPath(std::string path) { MediaPath           =  path; }

void JahPrefs::setMenubars(bool value)    { menuBars            = value; }
void JahPrefs::setDualhead(bool value)    { dualHead            = value; }
void JahPrefs::setWidescreen(bool value)  { wideScreen          = value; }

void JahPrefs::setThemestyle(std::string  value)  { themestyle          = value; }
void JahPrefs::setThemecolor(int  value)  { themecolor          = value; }

void JahPrefs::setVideohead(bool value)   { videoHead           = value; }
void JahPrefs::setVideoheadX(int  value)  { videoHeadX          = value; }
void JahPrefs::setVideoheadY(int  value)  { videoHeadY          = value; }

void JahPrefs::setQuickStart(bool value)    { quickStart = value; }
bool JahPrefs::getQuickStart(void) 			{ return quickStart; }

void JahPrefs::setRememberGeometry(bool value)    { rememberGeometry    = value; }
void JahPrefs::setGeometryX(int value)    { XPos                = value; }
void JahPrefs::setGeometryY(int value)    { YPos                = value; }
void JahPrefs::setGeometryWidth(int value)  { JahWidth          = value; }
void JahPrefs::setGeometryHeight(int value) { JahHeight         = value; }
void JahPrefs::setGeometryMaximized(bool value) { JahMaximized  = value; }

void JahPrefs::setGeometryX2(int value)    { XPos2                = value; }
void JahPrefs::setGeometryY2(int value)    { YPos2                = value; }
void JahPrefs::setGeometryWidth2(int value)  { JahWidth2          = value; }
void JahPrefs::setGeometryHeight2(int value) { JahHeight2         = value; }
void JahPrefs::setGeometryMaximized2(bool value) { JahMaximized2  = value; }

void JahPrefs::setLanguage(std::string value) { language            = value; }

void JahPrefs::setHwFog(bool value)       { HwFog               = value; }
void JahPrefs::setHwAliasing(bool value)  { HwAliasing          = value; }
void JahPrefs::setVShading(bool value)    { vertexshaders       = value; }
void JahPrefs::setGpusupport(bool value)  { gpusupport          = value; }
void JahPrefs::setNVGpusupport(bool value){ nv_gpusupport       = value; }

void JahPrefs::setMaxVertexInstructionsARB(int value) { max_vertex_program_instructions_ARB = value; }
void JahPrefs::setMaxFragmentInstructionsARB(int value) { max_fragment_program_instructions_ARB = value; }

void JahPrefs::setRenderer(int value)     { JahRenderer         = value; }
void JahPrefs::setRenderQuality(int value) { JahRenderQuality   = value; }

void JahPrefs::setStyle(bool value)        { jahstyle = value; }
void JahPrefs::setStyleTheme(int value)    { themestyle = value; }
void JahPrefs::setStyleColor(int value)    { themecolor = value; }

void JahPrefs::setAutoLaunchServer(bool value)    { autoLaunchserver = value; }

void JahPrefs::setJtbonlineurl(const std::string &s) { jtbonlineurl = s; }


void JahPrefs::setModulesUI(int module, int left, int center, int right) 
{

            MainModuleControlerLeftFrameWidth[module]  = left;
            MainModuleControlerCenterFrameWidth[module]= center;
            MainModuleControlerRightFrameWidth[module] = right;
}

/////////////////////////////////////////////////////////////////
//routines to get data start here

bool JahPrefs::getFoundPrefs(void)      { return foundPrefs;  }

std::string JahPrefs::getJahVersion(void)   { return JahVersion;  }

int JahPrefs::getJahResolution(void)    { return jahResolution; }
//int JahPrefs::getXRes(void)             { return XResolution; }
//int JahPrefs::getYRes(void)             { return YResolution; }
//QString JahPrefs::getRatio(void)        { return Ratio;       }

std::string JahPrefs::getBasePath(void)     { return BasePath;    }
std::string JahPrefs::getMediaPath(void)    { return MediaPath;   }

bool JahPrefs::getMenubars(void)        { return menuBars;    }
bool JahPrefs::getDualhead(void)        { return dualHead;    }
bool JahPrefs::getWidescreen(void)      { return wideScreen;  }

std::string  JahPrefs::getThemestyle(void)      { return themestyle;  }
int  JahPrefs::getThemecolor(void)      { return themecolor;  }

bool JahPrefs::getVideohead(void)       { return videoHead;   }
int  JahPrefs::getVideoheadX(void)      { return videoHeadX;  }
int  JahPrefs::getVideoheadY(void)      { return videoHeadY;  }

bool JahPrefs::getRememberGeometry(void) { return rememberGeometry; }
int  JahPrefs::getGeometryX(void)       { return XPos;        }
int  JahPrefs::getGeometryY(void)       { return YPos;        }
int  JahPrefs::getGeometryWidth(void)   { return JahWidth;    }
int  JahPrefs::getGeometryHeight(void)  { return JahHeight;   }
bool JahPrefs::getGeometryMaximized(void)  { return JahMaximized;   }

int  JahPrefs::getGeometryX2(void)      { return XPos2;       }
int  JahPrefs::getGeometryY2(void)      { return YPos2;       }
int  JahPrefs::getGeometryWidth2(void)  { return JahWidth2;   }
int  JahPrefs::getGeometryHeight2(void) { return JahHeight2;  }
bool JahPrefs::getGeometryMaximized2(void)  { return JahMaximized2;   }

std::string JahPrefs::getLanguage(void)        { return language;    }

bool JahPrefs::getHwFog(void)           { return HwFog;       }
bool JahPrefs::getHwAliasing(void)      { return HwAliasing;  }
bool JahPrefs::getVShading(void)        { return vertexshaders; }
bool JahPrefs::getGpusupport(void)      { return gpusupport;  }
bool JahPrefs::getNVGpusupport(void)    { return nv_gpusupport; }

int  JahPrefs::getMaxVertexInstructionsARB(void) { return max_vertex_program_instructions_ARB; }
int  JahPrefs::getMaxFragmentInstructionsARB(void) { return max_fragment_program_instructions_ARB; }

int  JahPrefs::getRenderer(void)        { return JahRenderer; }
int  JahPrefs::getRenderQuality(void)   { return JahRenderQuality; }

bool JahPrefs::getStyle(void)           { return jahstyle; }
std::string  JahPrefs::getStyleTheme(void)      { return themestyle; }
int  JahPrefs::getStyleColor(void)      { return themecolor; }

bool  JahPrefs::getAutoLaunchServer(void)      { return autoLaunchserver; }

std::string JahPrefs::getJtbonlineurl() { return jtbonlineurl; }

void JahPrefs::getModulesUI(int module, int &left, int &center, int &right) 
{

            left   = MainModuleControlerLeftFrameWidth[module];
            center = MainModuleControlerCenterFrameWidth[module];
            right  = MainModuleControlerRightFrameWidth[module];
}


