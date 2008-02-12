/*******************************************************************************
**
** The header file for the Jahshaka keyer module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef TRACKER_H
#define TRACKER_H

#include "glworld.h"

///////////////////////////////////////////////
//Then Main Class Definitions

#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qstylefactory.h>
#include <stdlib.h>

#include "jahnodes.h"
#include "jahtimer.h"
#include "coreobject.h"

//for dynamic plugin support
#include "jahpluginlib.h"

////////////////////////////////////////////////////////////
//for gpu support
#include "gpumathlib.h"

class TrackerData
{
protected:
    int             m_frame_number;
    bool            m_point_enabled[MAX_TRACKING_POINTS];
    float2          m_point_image_position[MAX_TRACKING_POINTS];

public:
    int             getFrameNumber() { return m_frame_number; }
    void            setFrameNumber(int frame_number) { m_frame_number = frame_number; }
    float2&         getPointImagePosition(int point_number) { return m_point_image_position[point_number]; }
    void            setPointImagePosition(int point_number, float2& position) { m_point_image_position[point_number] = position; }
    bool            getPointEnabled(int point_number) { return m_point_enabled[point_number]; }
    void            setPointEnabled(int point_number, bool flag) { m_point_enabled[point_number] = flag; }

    TrackerData(int frame_number, float2* position, bool* enabled);
    void            setPointImagePosition(float2* position);
    void            setPointEnabled(bool* enabled);

};

class GLTracker :  public  GLWorld 
{
    Q_OBJECT

    enum TRACKER_LAYERS
    {
        TRACKER_NULL_LAYER,
        TRACKER_BASE_LAYER,
    };


protected:
    GLuint          m_point_texture_id[MAX_TRACKING_POINTS];
    float           m_point_radius[MAX_TRACKING_POINTS];
    bool            m_point_enabled[MAX_TRACKING_POINTS];
    float2          m_point_screen_position[MAX_TRACKING_POINTS];
    float2          m_point_screen_position_unrounded[MAX_TRACKING_POINTS];
    float2          m_point_initial_screen_position[MAX_TRACKING_POINTS];
    float2          m_point_image_position[MAX_TRACKING_POINTS];
    float2          m_point_previous_position[MAX_TRACKING_POINTS];
    float2          m_point_previous_delta[MAX_TRACKING_POINTS];
    float2          m_point_lower_left_corner_screen_coords[MAX_TRACKING_POINTS];
    float2          m_point_lower_left_corner_image_coords[MAX_TRACKING_POINTS];
    float           m_point_search_radius[MAX_TRACKING_POINTS];
    float           m_point_sensitivity[MAX_TRACKING_POINTS];
    int             m_point_checksum_tolerance[MAX_TRACKING_POINTS];
    unsigned char*  m_point_pixel_buffer[MAX_TRACKING_POINTS];
    int             m_point_texture_width;
    float2          m_image_lower_left_corner;
    float2          m_point_time_zero_position[MAX_TRACKING_POINTS];
    float2          m_point_reference_position[MAX_TRACKING_POINTS];
    int             m_point_reference_frame[MAX_TRACKING_POINTS];
    bool            m_stabilize;

    std::map<int,TrackerData*>        m_keyframe_data;

public:
    void            dumpPointPixelBuffer(int index);
    unsigned char*  getPointPixelBuffer(int index);
    int             getPointChecksumTolerance(int index);
    void            setPointChecksumTolerance(int index, int value);
    float           getPointSensitivity(int index) ;
    void            setPointSensitivity(int index, float value);
    GLuint          getPointTextureId(int index) ;
    GLuint*         getPointTextureIdPtr(int index);
    GLuint          getBaseLayerTextureId();

    // removed inline functions
    void            setBaseLayer(JahLayer* layer);
    JahLayer*       getPointLayer(int index);
    void	    setPointLayer(int index, JahLayer* layer);
    bool            getPointEnabled(int index) ;
    void            setPointEnabled(int index, bool value);
    int             getSelectedTrackerPointNumber();
    float2&         getPointLowerLeftCornerScreenCoords(int point_number);
    void            setPointLowerLeftCornerScreenCoords(int point_number, float2& position);
    float2&         getPointLowerLeftCornerImageCoords(int point_number);
    void            setPointLowerLeftCornerImageCoords(int point_number, float2& position);
    float2&         getPointScreenPosition(int point_number);
    void            setPointScreenPosition(int point_number, float2& position);
    float2&         getPointScreenPositionUnrounded(int point_number);
    void            setPointScreenPositionUnrounded(int point_number, float2& position);
    float2&         getPointInitialScreenPosition(int point_number);
    void            setPointInitialScreenPosition(int point_number, float2& position);
    float2&         getPointImagePosition(int point_number);
    void            setPointImagePosition(int point_number, float2& position);
    float2&         getPointPreviousPosition(int point_number);
    void            setPointPreviousPosition(int point_number, float2& position);
    float2&         getPointPreviousDelta(int point_number) ;
    void            setPointPreviousDelta(int point_number, float2& delta);
    int             getPointTextureWidth() ;
    void            setPointTextureWidth(int width);
    void            setPointSearchRadius(int point_number, float value);
    float           getPointSearchRadius(int point_number) ;
    float           getPointSearchDiameter(int point_number);
    void            setPointRadius(int point_number, float value);
    float           getPointRadius(int point_number) ;
    void            setTrackerPoint(int point_number);
    float2&         getImageLowerLeftCorner();
    void            setImageLowerLeftCorner(float2& position) ;
    float2&         getPointTimeZeroPosition(int point_number) ;
    void            setPointTimeZeroPosition(int point_number, float2& position);
    float2&         getPointReferenceScreenPosition(int point_number) ;
    void            setPointReferenceScreenPosition(int point_number, float2& position);
    int             getPointReferenceFrame(int point_number);
    void            setPointReferenceFrame(int point_number, int frame);
    bool            getStabilize() ;
    void            setStabilize(bool flag);
    int             getNumberOfEnabledPoints();
    bool            getUseGpu() ;

    void            glslTracker(JahLayer* base_layer);
    void            capturePointPattern(int point_number);
    void            createTrackerPointLayers();
    void            showSliders(bool flag);

    std::map<int,TrackerData*>*  getKeyframeData();

private:
	QHBox *m_pControls;
	QHBox *m_pLeftController;
	QHBox *m_pRightController;

public:

    GLTracker( GLCore* core, const char* name, QHBox* controls,
        QHBox* controller,QHBox* leftcontroller, QHBox* rightcontroller,
	       int* globalclipnumber );
    ~GLTracker();

protected:
	void start( );

public:
    //object and variable initialization routines
    void initializeVariables(void);
    void initializeObjects();

    //ui initialization routines
    void buildInterface( QHBox* qtinterface);
    void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );
	void createMenuItem(QPopupMenu * themenu );

    //used to load the plugins in
    JahPluginLib * jplugin;

    //used for the node view
    //JahNodes*               thenodeframer;

    //these arent slots are they? they should just be public
    void updateSliders(motionNode* keyframe);
    void updateUiSettings();
    void updateUiOptions();

public slots:
    void addClip(assetData clip);
    void layerClicked( QListViewItem* item );

    void setStabilize(void);
    void setGpuSelect(void);

    //option selection routines
    void setOption1(void);
    void setOption2(void);
    void setOption3(void);
    void setOption4(void);

    void captureTrackerPoint(int);


        ////////////////////////////////////////////
        // routines to draw effects


public:
    JahLayer*               getChromaBaseLayer() ;
    JahLayer*               getChromaKeyLayer() ;
    JahLayer*               getChromaSettingsLayer() ;
    bool                    getSelectColorsIndividually() ;
    bool                    getGpuSelect() ;
    GLuint*                 getBaseTextureIDPtr();
    GLuint                  getBaseTextureID() ;
    GLuint*                 getKeyTextureIDPtr();
    GLuint                  getKeyTextureID();
    bool                    getTexturesInitialized();

protected:    
    JahLayer*               m_chroma_base_layer;
    JahLayer*               m_chroma_key_layer;
    JahLayer*               m_chroma_settings_layer;


signals:
    void    GrabClip(void);    //linked to main jahcontrol to talk to desktop

protected:

    void objectMenu(int menu);
    void paintGL();
    void buildEffect();


private:

    ////////////////////////////////////////////////////////////////////////////
    // effect based variables

    QImage      sourceimage,savedimage;

    bool        rebuildStatus;
    int         effectobj;


    int         iScreen;

    float       z_offset;
    float       camera_distance;

    ////////////////////////////////////////////////////////////////////////////
    //interface variables
    ///////////////////////////////////////////////////////////////////////
    //interface variables for the left and right world space menus
    ///////////////////////////////////////////////////////////////////////

    JahToolButton   *tooltranslate, *toolscale,     *toolrotate;
    JahToolButton   *playerphoto,   *switchtheview;

    ///////////////////////////////////////////////////////////////////////
    //animation module ui options
    ///////////////////////////////////////////////////////////////////////

    //declare all controllers that we are using here
    enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, KEYFRAMES, numTabs};

    ///////////////////////////////////////////////////////////////////////
    //now the main widget variables

    QSlider*        m_x_position_slider;
    QSlider*        m_y_position_slider;
    QSlider*        m_point_radius_slider;
    QSlider*        m_point_search_radius_slider;
    QSlider*        m_point_sensitivity_slider;
    QSlider*        m_point_checksum_tolerance_slider;

    InputLCD*       m_x_position_lcd;
    InputLCD*       m_y_position_lcd;
    InputLCD*       m_point_radius_lcd;
    InputLCD*       m_point_search_radius_lcd;
    InputLCD*       m_point_sensitivity_lcd;
    InputLCD*       m_point_checksum_tolerance_lcd;
 
    QLabel*         m_x_position_label;
    QLabel*         m_y_position_label;
    QLabel*         m_point_radius_label;
    QLabel*         m_point_search_radius_label;
    QLabel*         m_point_sensitivity_label;
    QLabel*         m_point_checksum_tolerance_label;

    //the first, main frame
    QFrame*         LayersFrame;
    QFrame*         EffectsFrame;

    //for the control panel
    QPushButton*    AddRTButton;
    QPushButton*    AddGpuButton;
    //    QPushButton*    AddPluginButton;

    ///////////////////////////////////////////////////////////////////////
    //the tab frame to hold the tabs

    QTabWidget      *tabframe;

    //widegts for each of the tabs
    QWidget         *moduleOptionsUI[numTabs];

    //each module option ui can have up to 10 embedded ui's
    QFrame          *ControlFrameUI[numTabs][10];

    ///////////////////////////////////////////////////////////////////////
    // variables for the different tabs

    ///////////////////////////////////////////////////////////////////////
    //set up the scene controls

    QPushButton*    ResetButton;             
    QPushButton*    SaveButton;    
    QPushButton*    ExportButton;    
    QPushButton*    LoadButton;              
    QPushButton*    ClearButton;
    QPushButton*    PackageButton;
    QCheckBox*      VideoRenderSelect;
    QCheckBox*      m_use_gpu_qcheckbox;
    QCheckBox*      m_stabilize_qcheckbox;

    //lcdstartframe and endframe are initialized in world

    QCheckBox*      FogSelect;
    QSlider         *HFogSlider,        *VFogSlider;
    InputLCD        *HFogSlider_lcd,    *VFogSlider_lcd;

    //////////////////////////////////////////////////////////////////////
    //set up the different object controls, multiple layers are embedded

    //the world
    QCheckBox       *SmoothSelect;            
    QCheckBox       *TextSelect;
    QCheckBox       *GridSelect;
    QPushButton     *ResetWorldButton;

    //the layer types
    QPushButton     *ObjectLayer;
    QPushButton     *ObjectCube;
    QPushButton     *ObjectCylinder;
    QPushButton     *ObjectSphere;
    QPushButton     *ObjectBezier;
    QPushButton     *ObjectMesh;
    QPushButton     *ResetLayerButton;

    //the font options
    //the particle options

    //the object options obj's
    QPushButton     *loadobject_1,  *objectstyle;
    QPushButton     *ResetObjectButton1;

    //the object options 3dsmax
    QPushButton     *loadmaxobject_1;
    QPushButton     *ResetObjectButton2;

    ///////////////////////////////////////////////////////////////////////
    //set up the texture and clip control panel

    //clip options
    QCheckBox       *ClipSelect;
    QPushButton     *grabdesktopClip; 
    QPushButton     *keyimage_2;       
    QCheckBox       *KeySelect;
    QPushButton     *grabdesktopKey;
    QPushButton     *keyimage_3;
    QPushButton     *keyStatusBox;

    //video options
    QLabel          *VideoLabel;
    QToolButton     *lockicon;

    QCheckBox       *extendHeadTail;
    QCheckBox       *toggleTheLoop;
    QCheckBox       *togglePing;

    ///////////////////////////////////////////////////////////////////////
    //set up the effects control panel

    QCheckBox       *ForegroundSelect;    
    QCheckBox       *DepthSelect;
    QCheckBox       *BlurSelect;
    QSlider         *HBlurSlider, *VBlurSlider;
    InputLCD        *HBlurSlider_lcd, *VBlurSlider_lcd;

    QCheckBox       *NegativeSelect, *SwapRGBSelect, *MirrorSelect; 
    JahToolButton   *lighticon;
    QCheckBox       *light1, *reflect1, *shader1, *shader2, *smooth1, *mesh1;
    QLabel          *Transparency_1;     
    QSlider         *Trans_1;         
    InputLCD        *Translcd_1;


};



#endif // TRACKER_H





