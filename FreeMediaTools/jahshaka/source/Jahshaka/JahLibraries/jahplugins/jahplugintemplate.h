/*******************************************************************************
**
** The Jahshaka Plugin API
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** This is Open Source Software Released under the BSD License
**
*******************************************************************************/

#ifndef JAHPLUGINTEMPLATE_H
#define JAHPLUGINTEMPLATE_H

/////////////////////////////////////////////////////////////
//currently we need Qt to compile all jahshaka plugins

#include <qstring.h>
#include <qimage.h>

#include <GL/glew.h>

static const int MESH_WIDTH = 74;
static const int MESH_HEIGHT = 50;


/////////////////////////////////////////////////////////////
// The main Plugin Class

enum JAH_PLUGIN_CLASS
{
		JAH_PLUGIN_NONE,
        JAH_CPU_PLUGIN,
        JAH_GPU_PLUGIN,
        JAH_MESH_PLUGIN
};

class jahPlugin
{
    
public:
    
    //jahPlugin( ) {}
    virtual ~jahPlugin( ) {}
    
    /////////////////////////////////////////////////////////
    //main plugin information
    virtual QString& getName(void)             { return pluginName;   }
    virtual QString& getFamily(void)           { return pluginFamily; }
    virtual QString& getGroup(void)            { return pluginGroup;  }
    virtual QString& getGuid(void)             { return m_guid;       }
    virtual void setGuid(QString& guid)        { m_guid = guid;       }
    virtual JAH_PLUGIN_CLASS getClass(void)    { return pluginClass;  }
    
    //ui information
    virtual int     getSliders(void)           { return numsliders;  }
    virtual int     getButtons(void)           { return numoptions;  }
    virtual bool    useingLocalUI(void)        { return useLocalUI;  }
    
    //for image processing
    virtual void    setImage(QImage image)     
    {   vfximage = new QImage (image);
        //we can set these when image is sent over
        
        Height = image.height();
        Width  = image.width();
    }
    
    virtual QImage  getImage (void)            { return *vfximage; /*QImage theimage; return theimage;*/ }
    virtual void    processImage(void)         { };
    virtual void    initializePlugin(void) 	{ };
    
    //used to pass app user settings to the plugin
    //convert this into a array please
    virtual void    setSlider (int theslider, GLfloat value)  { slider[theslider] = value; }
    virtual void    setOption (int theoption, bool value) { option[theoption] = value; }
    virtual bool    getOption (int theoption) { return option[theoption]; }
    
    
    /////////////////////////////////////////////////////////////
    // utility routines
    
    //this is set when the plugin is initialized so it knows where resource files are
    QString JahBasePath;
    QString tempBasePath;
    
    virtual void    setBasePath(QString basepath)
    {
        JahBasePath = basepath;
    }
    
    bool checkResBounds(int &x, int &y)
    {
        bool inscope = true;
        
        if (y<0) { y = 0; inscope = false; }
        if (y>=vfximage->height()) { y = vfximage->height() -1; inscope = false; }
        
        if (x<0) { x = 0; inscope = false; }
        if (x>=vfximage->width()) { x = vfximage->width() -1; inscope = false; }
        
        return inscope;
    }
    
    void checkColorBounds(int &c)
    {
        if (c<0) c = 0;
        if (c>=255) c = 255;
        
    }
    
    void checkColorBounds(int &r, int &g, int &b)
    {
        if (r<0)    r = 0;
        if (r>=255) r = 255;
        
        if (g<0)    g = 0;
        if (g>=255) g = 255;
        
        if (b<0)    b = 0;
        if (b>=255) b = 255;
        
    }
    
    QRgb getColor(int x, int y)
    {
        int r,g,b;
        QRgb color;
        
        int xval = x;
        int yval = y;
        
        checkResBounds(xval,yval);
        
        //return value
        QRgb *p = (QRgb *)vfximage->scanLine(yval)+xval;
        r=qRed(*p); g=qGreen(*p); b=qBlue(*p);
        
        checkColorBounds(r);
        checkColorBounds(g);
        checkColorBounds(b);
        
        color = qRgb (r,g,b);
        return color;
    }
    
    /////////////////////////////////////////////
    // init the plugin
    
    void initSettings()
    {
        JahPluginVersion = "2.0";
        
        useLocalUI = 0;
        numsliders = 0;
        numoptions = 0;
        numlabels  = 0;
        
        pluginName    = "NULL";
        pluginFamily  = "NULL";
        pluginGroup   = "NULL";
        pluginClass   = JAH_PLUGIN_NONE;
        
        Width  = 0;
        Height = 0;
        
        uselabels = false;
        usesliders = false;
        
        for (int s=0; s<10; s++)
        {
            slider[s] = 0.0;
        }
        
        for (int o=0; o<4; o++)
        {
            option[o] = 0;
        }
        
        //this lets us know what the hardware is running
        nvsupport  = false;
        arbsupport = false;
        
        //this lets us know what the plugin supports
        HASNV       = false;
        HASARB      = false;
        
        gpuactive   = false;
        
        USENV       = false;
        USEARB      = false;
        
        //for meshfx
        usesMesh    = false;
    }
    
    
    //////////////////////////////////////////////////////////////////
    // important routine here!
    
    void initializeGpu()
    {
        //we need to initialize glew for all glew based plugins
        GLenum err = glewInit();
        
        if (GLEW_OK != err)
        {
            //problem: glewInit failed, something is seriously wrong
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
            gpuinitialized = false;
        }
        else
        {
            gpuinitialized = true;
            //debug("glew initialized");
        }
        
        /////////////////////////////////////////////////////////
        //figure out which version of the plugin code to use
        
        //if they are on nvidia and the plugin works on nvidia
        if (nvsupport && HASNV)
        {
            USENV = true;
        }
        else if (arbsupport && HASARB)
        {
            //if the card supports arb and the plugin supports arb
            USEARB = true;
        }
    }
    
    int getScreenWidth() { return m_screen_width; }
    int getScreenHeight() { return m_screen_height; }
    void setScreenWidth(int width) { m_screen_width = width; }
    void setScreenHeight(int height) { m_screen_height = height; }
    
    //////////////////////////////////////////////////////////////////
    // variables down here
    
    //for when we are using host sliders
    int numsliders;
    int numoptions;
    int numlabels;
    
    int m_screen_width;
    int m_screen_height;
    
    
    //user feedback values
    GLfloat  slider[10];
    bool   option[4];
    
    //we can use the host or set up a local ui
    bool useLocalUI;
    
    //important globals here - name of the plugin
    QString JahPluginVersion;          //name of plugin
    QString pluginFamily;              //name of plugin
    QString pluginGroup;               //name of plugin
    QString pluginName;                //name of plugin
    QString m_guid;
    JAH_PLUGIN_CLASS pluginClass;      //class of plugin ie CPU, GPU, MeshFX
    
    bool            uselabels;
    bool            usesliders;
    
    QString         *label;
    QString         *sliderlabel;
    QString         *button;
    
    //for image based effects
    QImage * vfximage;
    int Width;
    int Height;
    
    //for gpu based effects
    
    GLfloat  gpuwidth, gpuheight;
    GLfloat texwidthratio, texheightratio;
    int render_height, render_width;
    GLfloat camera_distance;
    GLuint  texid;
    
    //gpu preferences
    bool gpuinitialized;
    bool gpuactive;
    
    bool nvsupport;
    bool arbsupport;
    
    bool HASNV, HASARB;
    bool USENV, USEARB;
    
    //these are for the ARB and are set when the plugin is loaded
    int     max_vp_instructions;
    int     max_fp_instructions;
    
    
    virtual void    isComplexArbSupported(void) { };
    
    //these can only be done 1x ie the first time the plugin is called
    virtual void    setGpuImageSize(GLfloat w, GLfloat h)
    {
        gpuwidth   = w;
        gpuheight  = h;
    }
    
protected:
    GLuint          m_src_texture_id;
    GLuint          m_dest_texture_id;

public:

    //these can only be done 1x ie the first time the plugin is called
    virtual void    setTexId(GLuint t)                 { texid   = t; }

    virtual void    setSrcTextureId(GLuint id) { m_src_texture_id = id; }
    virtual void    setDestTextureId(GLuint id) { m_dest_texture_id = id; }
    virtual GLuint  getSrcTextureId() { return m_src_texture_id; }
    virtual GLuint  getDestTextureId() { return m_dest_texture_id; }
    
    virtual void    setTexRatios(GLfloat w, GLfloat h)
    {
        texwidthratio   = w;
        texheightratio  = h;
    }
    
    virtual void    setCoreSize(int w, int h)
    {
        render_height = h;
        render_width  = w;
    }
    
    virtual void    setCameraDistance(GLfloat cd)      { camera_distance = cd; }
    
    //////////////////////////////////////////////
    // for plugin mesh object support
    //need to make these dynamically assignable... ie use pointers
    GLfloat*         MeshXCoord; // should be 72,48 for 720 486
    GLfloat*         MeshYCoord;
    GLfloat*         MeshZCoord;
    
    int MESH_X_DIMENSION;
    int MESH_Y_DIMENSION;
    
    bool usesMesh;
    
    void setMeshXDimension(int value) { MESH_X_DIMENSION = value; }
    void setMeshYDimension(int value) { MESH_Y_DIMENSION = value; }
    
    //set up a pointer to the mesh array
    void setMeshXCoord(GLfloat *meshx) { MeshXCoord = meshx; }
    void setMeshYCoord(GLfloat *meshy) { MeshYCoord = meshy; }
    void setMeshZCoord(GLfloat *meshz) { MeshZCoord = meshz; }
    
    void getMeshCoord( int column, int row, GLfloat& x, GLfloat& y, GLfloat& z)
    {
        int index = (column * MESH_HEIGHT) + row;
        x = MeshXCoord[index];
        y = MeshYCoord[index];
        z = MeshZCoord[index];
    }
    
    void setMeshCoord( int column, int row, GLfloat& x, GLfloat& y, GLfloat& z)
    {
        int index = (column * MESH_HEIGHT) + row;
        MeshXCoord[index] = x;
        MeshYCoord[index] = y;
        MeshZCoord[index] = z;
    }
};



#endif 

