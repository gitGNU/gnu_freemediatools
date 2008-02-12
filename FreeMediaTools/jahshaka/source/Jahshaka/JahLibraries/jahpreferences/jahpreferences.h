/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHPREFS_H
#define JAHPREFS_H

#include <string>

#include    "qsettings.h"

class JahPrefs
{

    public:

        //for some reason we have to do this public on windows vc6++
        #ifdef WIN32
    ~JahPrefs() throw() ;
        #endif

    private:

        static JahPrefs _instance ;

        // Hide these 'cause this is a singleton.       
        JahPrefs() throw() ; 

        #ifndef WIN32
    ~JahPrefs() throw() ;    
        #endif

        JahPrefs( JahPrefs&);

        JahPrefs& operator = ( const JahPrefs&) ;

        /// the sole instance of this class
		std::string JahVersion;
        std::string BasePath;
        std::string MediaPath;

        int     JahRenderQuality;
        int     JahRenderer;

        int     jahResolution;


        bool    menuBars;
        bool    dualHead;

        bool    videoHead;
        int     videoHeadX;
        int     videoHeadY;

        std::string  language;

		bool	quickStart;

        bool    rememberGeometry;
        int     XPos;    
        int     YPos;     
        int     JahWidth;    
        int     JahHeight;    
		bool    JahMaximized;

        int     XPos2;    
        int     YPos2;     
        int     JahWidth2;    
        int     JahHeight2;    
		bool    JahMaximized2;

        bool    HwFog;
        bool    HwAliasing;
        
        bool    vertexshaders;
        bool    gpusupport;
        bool    nv_gpusupport;

        bool    autoLaunchserver;

        bool    foundPrefs;

        bool    wideScreen;

        bool    jahstyle;
		std::string     themestyle;
        int     themecolor;

        int     max_vertex_program_instructions_ARB;
        int     max_fragment_program_instructions_ARB;

        std::string jtbonlineurl;

        //for modules ui
        //currently hardcoded to 13
        int module_counter;
	    int	MainModuleControlerRightFrameWidth[13];
	    int	MainModuleControlerCenterFrameWidth[13];
	    int	MainModuleControlerLeftFrameWidth[13];
	    
        //each module can have its own default background color...
        //stored in RGB triplets
	    int	backgroundR[13], backgroundG[13], backgroundB[13];
	    

    public:
        //gets a instance of this object
        static JahPrefs& getInstance() throw();

        //the main prefs read and write routines
        void savePreferences(void) throw();
        void checkVersion(void) throw();
        void saveVideoheadPreferences(void) throw();
        void loadPreferences(void) throw();

        //routines to set varialbes
        void setJahVersion(std::string jahver);

        void setJahResolution ( int xres );
        //void setXRes ( int xres );
        //void setYRes ( int xres );
        //void setRatio( QString ratio );

        void setBasePath ( std::string path ); 
        void setMediaPath( std::string path );

        void setMenubars( bool value ); 
        void setDualhead( bool value );

        void setThemestyle( std::string value );
        void setThemecolor( int value );

        void setVideohead( bool value ); 
        void setVideoheadX( int value ); 
        void setVideoheadY( int value ); 

		bool getQuickStart( );
		void setQuickStart( bool value );

        void setRememberGeometry( bool value );
        void setGeometryX( int value ); 
        void setGeometryY( int value ); 
        void setGeometryWidth( int value ); 
        void setGeometryHeight( int value );
		void setGeometryMaximized( bool );

        void setGeometryX2( int value ); 
        void setGeometryY2( int value ); 
        void setGeometryWidth2( int value ); 
        void setGeometryHeight2( int value );
		void setGeometryMaximized2( bool );

        void setLanguage( std::string value );

        void setHwFog( bool value );
        void setHwAliasing( bool value ); 
        void setVShading( bool value );
        void setGpusupport (bool value );
        void setNVGpusupport (bool value );

        void setMaxVertexInstructionsARB(int value);
        void setMaxFragmentInstructionsARB(int value);

        void setRenderer( int value );
        void setRenderQuality( int value );

        void setWidescreen (bool value );

        void setStyle      (bool value );
        void setStyleTheme (int value );
        void setStyleColor (int value );

        void setAutoLaunchServer (bool value );

        void setJtbonlineurl(const std::string &);

    void setModulesUI(int module, int left, int center, int right);

        ///////////////////////////////////
        //routines to get variables

        bool getFoundPrefs(void);

        std::string getJahVersion( void );

        int getJahResolution( void );
        //int getXRes( void );
        //int getYRes( void );
        //QString getRatio( void );

        std::string getBasePath ( void ); 
        std::string getMediaPath( void ); 

        bool getMenubars( void ); 
        bool getDualhead( void );

		std::string  getThemestyle( void );
        int  getThemecolor( void );

        bool getVideohead( void ); 
        int  getVideoheadX( void ); 
        int  getVideoheadY( void ); 

        bool getRememberGeometry( void ); 
        int  getGeometryX( void ); 
        int  getGeometryY( void ); 
        int  getGeometryWidth( void ); 
        int  getGeometryHeight( void );
        bool getGeometryMaximized( void );

        int  getGeometryX2( void ); 
        int  getGeometryY2( void ); 
        int  getGeometryWidth2( void ); 
        int  getGeometryHeight2( void );
        bool getGeometryMaximized2( void );

        std::string  getLanguage( void );

        bool getHwFog( void ); 
        bool getHwAliasing( void ); 
        bool getVShading( void );
        bool getGpusupport( void );
        bool getNVGpusupport( void );

        int getMaxVertexInstructionsARB( void );
        int getMaxFragmentInstructionsARB( void );

        int  getRenderer( void );
        int  getRenderQuality( void );

        bool getWidescreen( void );

        bool getStyle (void );
		std::string  getStyleTheme (void );
        int  getStyleColor (void ); 

        bool getAutoLaunchServer (void );

        std::string getJtbonlineurl();

        void getModulesUI(int module, int &left, int &center, int &right); 

};

#endif

