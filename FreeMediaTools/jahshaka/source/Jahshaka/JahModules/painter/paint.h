/*******************************************************************************
**
** The header file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef PAINT_H
#define PAINT_H

#include <qcheckbox.h>
#include <qtabwidget.h>

#include "glworld.h"
#include "coreobject.h"
#include "paintdefs.h"

#include "paintlists.h"

#include <vector>

static const unsigned int MAX_RESIDENT_TEXTURES = 5000;

class PaintPixelCoords
{
	public:
		PaintPixelCoords( int x1_, int y1_, int x2_, int y2_ )
			: x1( x1_ )
			, y1( y1_ )
			, x2( x2_ )
			, y2( y2_ )
		{ }

		PaintPixelCoords( int x1_, int y1_ )
			: x1( x1_ )
			, y1( y1_ )
			, x2( 0 )
			, y2( 0 )
		{ }

		bool operator !=( PaintPixelCoords &other )
		{
			return x1 != other.x1 || y1 != other.y1 || x2 != other.x2 || y2 != other.y2;
		}

		int x1, y1, x2, y2;
};

class GLPaint :  public  GLWorld {
  Q_OBJECT

private:
	  QFrame *m_mainworld;
	  QHBox *m_controls;
	  QHBox *m_leftcontroller;
	  QHBox *m_rightcontroller;

public:

    GLPaint( GLCore* core, const char* name, QFrame* mainworld,
             QHBox* controls, QHBox* controller, QHBox* leftcontroller, QHBox* rightcontroller,
             int* globalclipnumber, const QGLWidget* =0  )
        :  GLWorld(core,name,controller,globalclipnumber )
           , m_mainworld( mainworld )
           , m_controls( controls )
           , m_leftcontroller( leftcontroller )
           , m_rightcontroller( rightcontroller )
           , mousePressed(false)
           , m_history_list_ptr( NULL )
           , m_copy_list_ptr( NULL )
           , m_paint_effects_dispatch_table(NOT_A_PAINT_EFFECT)
           , m_dirty_frames_list( NULL )
	{
	}

	~GLPaint() { }

protected:
	void start( );
	bool usesKeyFrames( ) { return false; }

public:
    enum PaintSliders
    {
        PEN_ALPHA_SLIDER,
        FILL_ALPHA_SLIDER,
        BACKGROUND_ALPHA_SLIDER,
        BRUSH_SIZE_SLIDER,
        GRADIENT_SLIDER,
        BUCKET_RANGE_SLIDER,
        NOT_A_SLIDER
    };

    enum PAINT_EFFECT_TYPE
    {
        INVERT_EFFECT,
        FADE_EFFECT,
        INTENSIFY_EFFECT,
        EMBOSS_EFFECT,
        BLUR_EFFECT,
        SHARPEN_EFFECT,
        EDGE_DETECT_X_EFFECT,
        EDGE_DETECT_Y_EFFECT,
        A_OVER_B_COMPOSITE_EFFECT,
        B_OVER_A_COMPOSITE_EFFECT,
        A_IN_B_COMPOSITE_EFFECT,
        B_IN_A_COMPOSITE_EFFECT,
        A_OUT_B_COMPOSITE_EFFECT,
        B_OUT_A_COMPOSITE_EFFECT,
        A_ATOP_B_COMPOSITE_EFFECT,
        B_ATOP_A_COMPOSITE_EFFECT,
        A_XOR_B_COMPOSITE_EFFECT,
        SOBEL_EFFECT,
        LAPLACIAN_EFFECT,
        LAPLACIAN2_EFFECT,
        LAP_OF_GAUSS_EFFECT,
        NOT_A_PAINT_EFFECT
    };

	void buildInterface( QHBox* qtinterface);
	void buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller );
	void createMenuItem(QPopupMenu * themenu );
	void initializeObjects();

	// History functions.
	void incrementHistoryCount(bool add_a_frame = false);
	void historyRemoveCurrent();

	// Image Manipulation functions.
	void fadeEffect();
	void embossEffect();
	void intensifyEffect();
	void invertEffect();
    void blurEffect();
    void sharpenEffect();
    void edgeDetectXEffect();
    void edgeDetectYEffect();
    void aOverBCompositeEffect();
    void bOverACompositeEffect();
    void aInBCompositeEffect();
    void bInACompositeEffect();
    void aOutBCompositeEffect();
    void bOutACompositeEffect();
    void aAtopBCompositeEffect();
    void bAtopACompositeEffect();
    void aXorBCompositeEffect();
    void sobelEffect();
    void laplacianEffect();
    void laplacian2Effect();
    void lapOfGaussEffect();
    void glslDrawEraser(bool initialize = false);
    void glslAOverBComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslBOverAComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslAInBComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslBInAComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslAOutBComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslBOutAComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslAAtopBComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslBAtopAComposite(GLuint a_texture_id, GLuint b_texture_id);
    void glslAXorBComposite(GLuint a_texture_id, GLuint b_texture_id);
    void executeComposite(GLuint a_texture_id, GLuint b_texture_id, 
                          GLhandleARB shader_program_handle,  GlslProgram* shader_program);


	void convolute(const convolutionType type);

	void updateDisplayZoom();
	void CalcZoomValues();

    void updateImageTexture();
    bool pointIsInsideDrawingArea(int x, int y);


	// Return the given integer value limited within the range 0 to 255.
	int limit0_255(const int & val) { return val < 0 ? 0 : val > 255 ? 255 : val; }

	// Activate the given tool.
	void activateTool(CanvasTool toolNum); 
	void makeCheckImage(void);

	// Accessor functions.
	QColor penColor()        { return *m_pen_color; }
	QColor fillColor()       { return *m_fill_color; }
	QColor backgroundColor() { return *m_background_color; }
	int    brushSize()       { return m_brush_size; }
	int    gradientDegree()  { return m_gradient_degree; }

	// Modifier functions.
	void setPenColor (QColor newColor)       { *m_pen_color        = newColor; }
	void setFillColor(QColor newColor)       { *m_fill_color       = newColor; }
	void setBackgroundColor(QColor newColor);
	void setBrushSize(int newSize);
	void setGradientDegree(int newVal)       { m_gradient_degree  = newVal; }

	//the paint tools
	void drawToolPen( color4& pen_color );

	void drawToolLine( int m_brush_size,color4& pen_color_light, color4& pen_color_dark, GLint minPointSize,
				   int x1, int y1, int x2, int y2  );

	void drawToolRect( int m_brush_size,
				   color4& pen_color, color4& pen_color_light, color4& pen_color_dark, GLint minPointSize,
				   int x1, int y1, int x2, int y2  );

	void drawToolRectFilled( int m_brush_size,  GLint minPointSize,
						 color4& pen_color, color4& pen_color_light, color4& pen_color_dark,
						 color4& fill_color, color4& fill_color_light, color4& fill_color_dark,
						 int x1, int y1, int x2, int y2  );


	void drawToolCircle( color4& pen_color, GLfloat gradientDegree, int x1, int y1, int x2, int y2  );

	void drawToolCircleFilled( color4& pen_color, color4& fill_color, GLfloat gradientDegree, int x1, int y1, int x2, int y2  );

	void drawToolTri ( color4& pen_color, color4& pen_color_light, color4& pen_color_dark,
				   int x1, int y1, int x2, int y2  );

	void drawToolTriFilled( color4& pen_color, color4& pen_color_light, color4& pen_color_dark,
						color4& fill_color, color4& fill_color_light, color4& fill_color_dark,
						int x1, int y1, int x2, int y2  );

    void drawToolPaintBucket();

    void drawToolPaintBucketFillAll();

    void drawEraser( int x_position, int y_position );

    void drawToolForegroundColorPicker( color4& pen_color, int x_position, int y_position );

    void drawToolBackgroundColorPicker( color4& fill_color, int x_position, int y_position );

    void fillPixelColorRange(int x_position, int y_position, color4& color_range_low, color4& color_range_high, 
                             QRgb new_color, unsigned int* pixel_buffer);

    bool checkColorRangeAndPosition(int x_position, int y_position, unsigned int* pixel_buffer, 
                                    QRgb new_color, color4& color_range_low, color4& color_range_high);

    bool adjacentPixelSet(bool* image_map, int x_position, int y_position);

    void evaluateAndSetPixelColor(int x, int y, unsigned int* pixel_buffer, bool* image_map,
                                  QRgb new_color, color4& color_range_low, color4& color_range_high,
                                  bool& point_found);

    void captureDrawingArea();
    void dumpNonBlackPixels();
    void copyTextureToQimage(QImage& image, unsigned int texture_id);
    void copyQimageToTexture(unsigned int texture_id, QImage& image);

    void drawTexturedQuad(GLuint texture_id);

    void clearColorsAndAlpha();
    void refreshTexture(GLuint* texture_id_ptr);
    void refreshCurrentTexture();

    int  getFillBucketRange() { return m_fill_bucket_range; }
    void setFillBucketRange(int value) { m_fill_bucket_range = value; }

    void showPaintSlider(enum PaintSliders slider);
    void hidePaintSliders();

    PAINT_EFFECT_TYPE getCurrentCompositeType() { return m_current_composite_type; }
    void setCurrentCompositeType(PAINT_EFFECT_TYPE type) { m_current_composite_type = type; }


public:
	virtual void updatePosition( );
	virtual bool usesVideoHead( ) { return false; }
	virtual void schedulePosition( );

	void   drawWithActiveTool();
	//void   initializeGL();
	void   paintGL();

	// Overloaded QT functions.
	virtual void mousePressEvent  ( QMouseEvent* event);
	virtual void mouseReleaseEvent( QMouseEvent* event);
	virtual void mouseMoveEvent   ( QMouseEvent* event);

    QFrame* m_mainworld_qframe;

	int    m_current_history_frame;

	QColor *m_pen_color, *m_fill_color, *m_background_color;
	int    m_brush_size, m_active_tool, m_gradient_degree, m_fade_degree;
	float		m_point_brush_size;
    int         m_point_brush_size_int;
	float		m_line_brush_size;
	int    x1, y1, x2, y2;
	bool   mousePressed, openPic;

    int    m_fill_bucket_range;

	QButtonGroup  *bgDrawingTools;

	QString			myProgramDirectory; // Directory where this program resides.
	QString			myWorkingPath;      // Path in which to look for files.
	QString			myAuthorText;

	bool			changedFrame;
	bool			frameEdited;

    int             m_lower_left_x;
    int             m_lower_left_y;
    int             m_upper_right_x;
    int             m_upper_right_y;

	float2          m_texture_ratio;

	int				m_texture_width;
	int				m_texture_height;

    PAINT_EFFECT_TYPE           m_current_composite_type;

    QPtrList<PaintHistory>*     m_history_list_ptr;
    QPtrList<PaintHistory>*     m_copy_list_ptr;

    GLuint          m_working_area_texture_id;

	bool            m_needs_clear;

    int             m_image_width_div_2;
    int             m_image_height_div_2;
    int             m_screen_width_div_2;
    int             m_screen_height_div_2;
    int             m_screen_center_x;
    int             m_screen_center_y;

    float           m_pen_alpha;
    float           m_fill_alpha;
    float           m_background_alpha;

	int	            m_frame_width;
    int             m_frame_height;

    bool            m_clip_is_still_image;

    unsigned int*   m_eraser_buffer;
    color4*         m_eraser_buffer_color4;
    bool*           m_eraser_circle_lookup_table;

    bool            m_use_gpu_eraser;

    typedef void (GLPaint::*PaintEffectFunction)();
    std::vector<PaintEffectFunction>     	m_paint_effects_dispatch_table;

    QPtrList<PaintHistory>*                 m_dirty_frames_list;


public:
    void            executePaintEffect(PAINT_EFFECT_TYPE effect_type);
    PaintEffectFunction     getPaintEffectFunction(int i) { return m_paint_effects_dispatch_table[i]; }

    bool            getOverwriteStillFrames();

    QPtrList<PaintHistory>* getHistoryListPtr() { return m_history_list_ptr; }
    QPtrList<PaintHistory>* getCopyListPtr() { return m_copy_list_ptr; }

    QPtrList<PaintHistory>* getDirtyFramesList() { return m_dirty_frames_list; }

    bool*           getEraserCircleLookupTable() { return m_eraser_circle_lookup_table; }
    unsigned int*   getEraserBuffer() { return m_eraser_buffer; }
    color4*         getEraserBufferColor4() { return m_eraser_buffer_color4; }
    GLuint          getImageLayerTextureId() { return getImageLayer()->getTextureData().getTexId(); }
    GLuint*         getImageLayerTextureIdPtr() { return &getImageLayer()->getTextureData().texID; }
    QFrame*         getMainworldQframe() { return m_mainworld_qframe; }
    void            setMainworldQframe(QFrame* qframe) { m_mainworld_qframe = qframe; }
    float           getPenAlpha() { return m_pen_alpha; }
    void            setPenAlpha(float value) { m_pen_alpha = value; }
    float           getFillAlpha() { return m_fill_alpha; }
    void            setFillAlpha(float value) { m_fill_alpha = value; }
    float           getBackgroundAlpha() { return m_background_alpha; }
    void            setBackgroundAlpha(float value) { m_background_alpha = value; }

    int             getImageWidthDiv2() { return m_image_width_div_2; }
    void            setImageWidthDiv2(int value) { m_image_width_div_2 = value; }
    int             getImageHeightDiv2() { return m_image_height_div_2; }
    void            setImageHeightDiv2(int value) { m_image_height_div_2 = value; }
    int             getScreenWidthDiv2() { return m_screen_width_div_2; }
    void            setScreenWidthDiv2(int value) { m_screen_width_div_2 = value; }
    int             getScreenHeightDiv2() { return m_screen_height_div_2; }
    void            setScreenHeightDiv2(int value) { m_screen_height_div_2 = value; }
    int             getScreenCenterX() { return m_screen_center_x; }
    void            setScreenCenterX(int value) { m_screen_center_x = value; }
    int             getScreenCenterY() { return m_screen_center_y; }
    void            setScreenCenterY(int value) { m_screen_center_y = value; }

    int             getLowerLeftX() { return m_lower_left_x; }
    int             getLowerLeftY() { return m_lower_left_y; }
    void            setLowerLeftX(int value) { m_lower_left_x = value; }
    void            setLowerLeftY(int value) { m_lower_left_y = value; }
    int             getUpperRightX() { return m_upper_right_x; }
    int             getUpperRightY() { return m_upper_right_y; }
    void            setUpperRightX(int value) { m_upper_right_x = value; }
    void            setUpperRightY(int value) { m_upper_right_y = value; }
    GLuint          getTextureId(int history_step);
    GLuint*         getTextureIdPtr(int history_step);
    GLuint*         getCurrentTextureIdPtr();
    GLuint          getCurrentTextureId();
    int             getCurrentHistoryFrame();
    float2&         getTextureRatio() { return m_texture_ratio; }
	int				getTextureWidth() { return m_texture_width; }
	int				getTextureHeight() { return m_texture_height; }
	bool			getNeedsClear() { return m_needs_clear; }
	void			setNeedsClear(bool flag) { m_needs_clear = flag; }
	void            setClearState();

    GLuint          getWorkingAreaTextureId() { return m_working_area_texture_id; }
    GLuint*         getWorkingAreaTextureIdPtr() { return &m_working_area_texture_id; }

    int             getFrameWidth() { return m_frame_width; }
    int             getFrameHeight() { return m_frame_height; }

    bool            getClipIsStillImage() { return m_clip_is_still_image; }
    void            setClipIsStillImage(bool flag) { m_clip_is_still_image = flag; }

    bool            getVideoFrameDirty(int frame_number);
    void            setVideoFrameDirty(int frame_number, bool flag);

    void            clearVideoFrameDirtyList();
    QLabel*         getCurrentColorLabel() { return m_current_color_label; }

    void            captureEraserBuffer();
    void            deleteEraserBuffer();
    void            buildEraserCircleLookupTable();
    inline bool     isInsideTheEraserCircle(int x, int y);

    void            copyFrames(int start_frame, int count);
    void            pasteFrames(int start_frame);
    void            deleteFrames(int start_frame, int count);
    bool            getAdvanceFrameAfterEffectApplication();
    void            advanceVideoFrameIfRequested();
    void            resetHistory();

    bool            getApplyEffectToAllFrames();
    bool            getAdvanceFrameAfterEffectApplied();

    void            executeEffect(PAINT_EFFECT_TYPE effect_type);

    PaintHistory*   getCurrentPaintHistory();

    bool            getUseGpuEraser();
    void            setUseGpuEraser(bool flag);



protected slots:
	void slotHistoryPrevious();
	void slotHistoryNext();
	void slotDelete();
	void slotCopyOne();
	void slotPaste();

	// Image Manipulation Slots.
	void slotInvert();
	void slotFade();
	void slotIntensify();
	void slotEmboss();
	void slotBlur();
	void slotSharpen();
	void slotEdgeDetectX();
	void slotEdgeDetectY();
	void slotSetCompositeType();
	void slotComposite();
	void slotSobel();
	void slotLaplacian();
	void slotLaplacian2();
	void slotLapOfGauss();
	void slotClear();
	void slotRefresh();

    void slotTurnOffAdvanceFrame();
    void slotTurnOffApplyEffectsToAllFrames();

	// Tool slots.
	void slotPen();
	void slotLine();
	void slotRectangle();
	void slotRectangleFilled();
	void slotCircle();
	void slotCircleFilled();
	void slotTriangle();
	void slotTriangleFilled();
    void slotPaintBucket();
    void slotPaintBucketFillAll();
    void slotForegroundColorPicker();
    void slotBackgroundColorPicker();
    void slotEraser();

	// Tool configuration slots.
	void slotPenColor();
	void slotFillColor();
	void slotBackgroundColor();
	void slotBrushSize(int value);
	void slotGradientDegree(int value);
	void slotFillBucketRange(int value);
    void slotSetPenAlpha(int value);
    void slotSetFillAlpha(int value);
    void slotSetBackgroundAlpha(int value);



public slots:
    void startanimation();

    void addLayer(void);
	void addClip(assetData clip);

	void clicked( QListViewItem* item );

	void changeParticle();
	void changeFontDraw();
	void changeObjectDraw();
	void keyCliplayer();
	void keyClipkey();
	void updateSliders();

	void resizePaintGL (void);
    void resetAll();
    void SceneLoad();
    void sceneSave();
	void clear(bool increment_history = true);
    //void resetZoom();
    //void changeZoom(int);

protected:
	void updateAnimation(int Frame, bool force = false);


signals:
	void    GrabClip(void);    //linked to main jahcontrol to talk to desktop
	void    GrabClipKey(void);    //linked to main jahcontrol to talk to desktop

private:

	std::vector <PaintPixelCoords> m_pixel_coords;

	///////////////////////////////////////////////////////////////////////
	//paint mouse tracking
    GLfloat mousex, mousey;

	///////////////////////////////////////////////////////////////////////
	//interface variables for the left and right world space menus
	///////////////////////////////////////////////////////////////////////
	
	QFrame			*glworldLeft;
	JahToolButton	*tooltranslate,	*toolscale,		*toolrotate;
	JahToolButton	*playerphoto;
    JahToolButton     *tooladdkey, *toolremovekey, *toolpreviouskey, *toolnextkey;

	QFrame			*glworldRight;
	JahToolButton	*toolzoomin,	*toolzoomout;
	QSlider			*zoom;



    int zoomPos_x;
    int zoomPos_y;

    double zoomScale_x;
    double zoomScale_y;
	///////////////////////////////////////////////////////////////////////
	//animation module ui options
	///////////////////////////////////////////////////////////////////////

	//declare all controllers that we are using here
	enum tabUI {SCENE, OBJECT, AXIS, CLIP, EFFECTS, KEYFRAMES, numTabs};
  
	///////////////////////////////////////////////////////////////////////
	//now the main widget variables

	//the first, main frame
	QFrame*			LayersFrame;
	QFrame*			EffectsFrame;

	///////////////////////////////////////////////////////////////////////
	//the tab frame to hold the tabs

	QTabWidget		*tabframe;

	//widegts for each of the tabs
	QWidget			*moduleOptionsUI[numTabs];

	//each module option ui can have up to 10 embedded ui's
    QFrame			*ControlFrameUI[numTabs][10];

    ////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// variables for the different tabs

	///////////////////////////////////////////////////////////////////////
	//set up the scene controls

	QPushButton*	ResetButton;             
	QPushButton*	SaveButton;    
	QPushButton*	LoadButton;              
	QPushButton*	ClearButton;
	QPushButton*	PackageButton;

	//lcdstartframe and endframe are initialized in world

	QCheckBox*		FogSelect;
	QSlider			*HFogSlider,		*VFogSlider;
	InputLCD		*HFogSlider_lcd,	*VFogSlider_lcd;

	//////////////////////////////////////////////////////////////////////
	//set up the different object controls, multiple layers are embedded

	//the world
	QCheckBox		*SmoothSelect;            
	QCheckBox		*TextSelect;
	QCheckBox		*GridSelect;
	QPushButton		*ResetWorldButton;

	//the layer types
	QPushButton		*ObjectLayer;
	QPushButton		*ObjectCube;
	QPushButton		*ObjectCylinder;
	QPushButton		*ObjectSphere;
	QPushButton		*ObjectBezier;
	QPushButton		*ObjectMesh;
	QPushButton		*ResetLayerButton;

	//the font options
	QLabel			*ScaleText_5;        
	QSlider			*ScaleX_5;         
	InputLCD		*ScaleXlcd_5;
	QLineEdit		*textEd;
	QPushButton		*textFont;           
	QPushButton		*textColor;        
	QPushButton		*textDraw;

	//the particle options
	QPushButton		*loadimage_3;
	QPushButton		*loadimage_4;
	QPushButton		*particlestyle;

	//the object options obj's
	QPushButton		*loadobject_1,	*objectstyle;
	QPushButton		*ResetObjectButton1;

	//the object options 3dsmax
	QPushButton		*loadmaxobject_1;
	QPushButton		*ResetObjectButton2;

	///////////////////////////////////////////////////////////////////////
	//set up the texture and clip control panel

	//clip options
	QCheckBox		*ClipSelect;
	QPushButton		*grabdesktopClip; 
	QPushButton		*keyimage_2;       
	QCheckBox		*KeySelect;
	QPushButton		*grabdesktopKey;
	QPushButton		*keyimage_3;
	QPushButton		*keyStatusBox;

	//video options
	QLabel			*VideoLabel;
	QToolButton		*lockicon;

	QCheckBox		*extendHeadTail;
	QCheckBox		*toggleTheLoop;
	QCheckBox		*togglePing;

	///////////////////////////////////////////////////////////////////////
	//set up the effects control panel

	QCheckBox		*ForegroundSelect;    
	QCheckBox		*DepthSelect;
	QCheckBox		*BlurSelect;
	QSlider			*HBlurSlider, *VBlurSlider;
	InputLCD		*HBlurSlider_lcd, *VBlurSlider_lcd;

	QCheckBox		*NegativeSelect, *SwapRGBSelect, *MirrorSelect; 
	JahToolButton	*lighticon;
	QCheckBox		*light1, *reflect1, *shader1, *shader2, *smooth1, *mesh1;
	QLabel			*Transparency_1;     
	QSlider			*Trans_1;         
	InputLCD		*Translcd_1;

	///////////////////////////////////////////////////////////////////////
	// Paint Module Specific need to weed out depreciated variables
	// and clean up the wacky names...

	//bool           EffectsStatus;

	//paint brushes
	QPushButton*	paintbrush1;
	QPushButton*	paintbrush2;
	QPushButton*	paintclip1;
	QPushButton*	paintclip1grab;
	QPushButton*	paintclip2;
	QPushButton*	paintclip2grab;
	QPushButton*	paintcolor;

	QPushButton		*m_pen_button, *m_line_button, *m_rectangle_button, *m_rectangle_filled_button,
  			  		*m_triangle_button, *m_triangle_filled_button, *m_circle_button, *m_circle_filled_button;

    QPushButton*    m_paintbucket_button;
    QPushButton*    m_paintbucket_fill_all_button;
    QPushButton*    m_foreground_color_picker_button;
    QPushButton*    m_background_color_picker_button;

    QPushButton*    m_eraser_button;

	QSlider			*m_brush_size_slider,    *ControlFrame1S2;
	InputLCD	    *m_brush_size_lcd, *ControlFrame1S2Lcd;
	QLabel			*ControlFrame1L1,    *m_brushes_label,    *m_brush_size_label;

	QPushButton		*m_background_color_button,  *m_pen_color_button,  *m_fill_color_button;
	QToolButton		*ControlFrame4B4, *ControlFrame4B5,  *ControlFrame4B6,  *ControlFrame4B7,  *ControlFrame4B8;
  					
	QSlider			*ControlFrame4S1, *m_gradient_slider;
	InputLCD		*ControlFrame4S1Lcd, *m_gradient_lcd;

    QSlider*        m_bucket_range_slider;
    InputLCD*       m_bucket_range_lcd;
    QLabel*         m_bucket_range_label;

    QSlider*        m_pen_alpha_slider;
    InputLCD*       m_pen_alpha_lcd;
    QLabel*         m_pen_alpha_label;

    QSlider*        m_fill_alpha_slider;
    InputLCD*       m_fill_alpha_lcd;
    QLabel*         m_fill_alpha_label;

    QSlider*        m_background_alpha_slider;
    InputLCD*       m_background_alpha_lcd;
    QLabel*         m_background_alpha_label;

	QLabel			*m_background_color_label, *m_gradient_label, *m_pen_color_label, *m_fill_color_label;
    QLabel*         m_current_color_label;

	QPushButton		*m_clear_bushbutton;
	QToolButton		*ControlFrame5B2,  *ControlFrame5B3,  *ControlFrame5B4;

	QToolButton		*ControlFrame7B1, *ControlFrame7B2, *ControlFrame7B3, *ControlFrame7B4,
  					*ControlFrame7B5, *ControlFrame7B6, *ControlFrame7B7, *ControlFrame7B8,
  					*ControlFrame7B9, *ControlFrame7B10;

	QPushButton		*effectInvert, *effectFade,  *effectIntensify, *effectBlur,     *effectSharpen,
					*effectEdgeX,  *effectEdgeY, *effectSobel,     *effectLapacian, *effectLapacian2, *effectGauss;

    QPushButton*    m_select_composite_button;
    QPushButton*    m_composite_button;
    QPushButton*    m_a_over_b_composite_button;
    QPushButton*    m_b_over_a_composite_button;
    QPushButton*    m_a_in_b_composite_button;
    QPushButton*    m_b_in_a_composite_button;
    QPushButton*    m_emboss_button;
    QCheckBox*      m_advance_frame_qcheckbox;
    QCheckBox*      m_apply_effect_to_all_frames_qcheckbox;
    QCheckBox*      m_overwrite_still_frames_qcheckbox;

    QCheckBox*      m_gpu_select_qcheckbox;

};



#endif // PAINT_H





