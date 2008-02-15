/*******************************************************************************
**
** The header file for the Jahshaka core effects module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef COREEFFECTS_H
#define COREEFFECTS_H


////////////////////////////////////////////////////////////
//the effect layer class
#include "gpumathlib.h"
#include <qgl.h>

class JahLayer;
class JahKeyframeObject;
class motionNode;
class QListViewItem;

#include <vector>

//can we move these into the effects routine?

enum FxCategory        { JAHPLUGIN, TRANSLATE, NOFX, NEGATIVE, GREY, EDGE, SELECTCOLORS, CONTRAST,
                         BRIGHTNESS, FISHEYE, MATRIXSHARPEN, MATRIXBLUR, MATRIXEMBOSS, CPUCOLORIZE,
                         EARTHQUAKE_NV30_GPU, GPUCOLORIZE,CHARCOAL_NV30_GPU, LIGHTING_NV30_GPU,

                        FISHEYERT, SWIRLRT, RIPPLERT, FLAGRT, CONTRASTRT, POLAR,
                        FLARES, GAUSSIANBLUR, BLUR, SHARPEN, COLORMATRIX, MIRROR, RADIALINES,
                        SWAPRGB, RTCOLORS,RTCOLORLUM };



class EffectInfo
{
public:
	enum EFFECT_CATEGORY
	{
		CPU_CATEGORY,
		GPU_CATEGORY,
		MESH_CATEGORY,
		GPGPU_CATEGORY,
		NOT_A_CATEGORY
	};

    static std::string&  getEffectCategoryName(EFFECT_CATEGORY category);

	// Should replace the FxCategory above
	enum EFFECT_TYPE
	{
		CPU_TYPE_START,
		GREY_CPU_TYPE,
		EDGE_DETECT_CPU_TYPE,
		SELECT_COLORS_CPU_TYPE,
		CONTRAST_CPU_TYPE,
		BRIGHTNESS_CPU_TYPE,
		MATRIX_SHARPEN_CPU_TYPE,
		MATRIX_BLUR_CPU_TYPE,
		MATRIX_EMBOSS_CPU_TYPE,
		GAUSSIAN_BLUR_CPU_TYPE,
		MIRROR_CPU_TYPE,
		SWAP_RGB_CPU_TYPE,
		NEGATIVE_CPU_TYPE,
		COLORIZE_CPU_TYPE,
        CHROMA_KEYER_CPU_TYPE,

		MESH_TYPE_START,
		FLAG_MESH_TYPE,
		FISHEYE_MESH_TYPE,
		RIPPLE_MESH_TYPE,
		SWIRL_MESH_TYPE,

		GPU_TYPE_START,
		STRETCH_CROP_GPU_TYPE,
        CHROMA_KEYER_GLSL_TYPE,
        COLORIZE_GPU_TYPE,
		EARTHQUAKE_GPU_TYPE,
		LIGHTING_GPU_TYPE,
		CHARCOAL_GPU_TYPE,

		EFFECT_TYPE_END,

		JAH_PLUGIN_TYPE,
		FIRE_3D_GPU_TYPE,  // Useless
        CHROMA_KEYER_GLSL_SEPARATE_ALPHA_TYPE, // Only works in the keyer module
		NOT_A_TYPE
	};

    static std::string&  getEffectTypeName(EFFECT_TYPE type);

protected:
	EFFECT_TYPE			m_type;	
	JahLayer*			m_jah_layer;

    static std::vector<std::string>				m_name_table;

public:

    EffectInfo();
    EffectInfo(EFFECT_TYPE type);

public:
	static EFFECT_CATEGORY  getCategory(EFFECT_TYPE type);
	static EFFECT_CATEGORY  getCategory(std::string& name);
	static std::string*		getName(EFFECT_TYPE type);
	static std::string	    getStringName(EFFECT_TYPE type);
	static EFFECT_TYPE      getType(std::string& name);
	EFFECT_TYPE	            getType() ;
	void				    setType(EFFECT_TYPE type);
	JahLayer*			    getJahLayer() ;
	void                    setJahLayer(JahLayer* layer) ;
};

class EffectLayer
{

protected:
    JahLayer*                       m_jah_layer;
	EffectInfo						m_info;
    int								m_pluginnumber;
	std::string						m_guid;
    std::vector<std::string>        m_slider_name_vector;


public:
	EffectInfo*			        getEffectInfo() { return &m_info; }
    EffectInfo::EFFECT_TYPE     getType() { return ( getEffectInfo()->getType() ); }
    EffectInfo::EFFECT_CATEGORY getCategory() { return ( getEffectInfo()->getCategory( getType() ) ); }
    void                        setType(EffectInfo::EFFECT_TYPE type) { getEffectInfo()->setType(type); }
    int                         getPluginNumber() { return m_pluginnumber; }
    void                        setPluginNumber(int number) { m_pluginnumber = number; }
    JahLayer*                   getJahLayer() { return m_jah_layer; }
    void                        setJahLayer(JahLayer* layer) { m_jah_layer = layer; }
    bool                        isACpuEffect() { return( getCategory() == EffectInfo::CPU_CATEGORY ); }
    bool                        isAMeshEffect() { return( getCategory() == EffectInfo::MESH_CATEGORY ); }
    bool                        isAGpuEffect() { return( getCategory() == EffectInfo::GPU_CATEGORY ); }
	void						setGuid(std::string& guid) { m_guid = guid; }
    std::string&				getGuid() { return m_guid; }
    void                        setSliderName(std::string name) { m_slider_name_vector.push_back(name); }
    std::string&                getSliderName(int slider_number) { return m_slider_name_vector[slider_number]; }
    std::vector<std::string>*   getSliderNameVector() { return &m_slider_name_vector; }

    EffectLayer(void);

    ~EffectLayer(void);

    QString							effectname;

	EffectInfo::EFFECT_TYPE			objtype;

    bool							selected;
    bool							layerStatus;
    bool							isOn;
    float							x,y,z;
    float							r;


	//need to add a node object to this
	//for effect keyframes
    motionNode*						node;
    JahKeyframeObject *				fxnodes;


};

static const unsigned int MAX_STABILIZING_POINTS = 4;
static const int MAX_STABILIZING_HISTORY = 4;
static const float MAX_STABILIZING_CIRCLE_RADIUS = 31;
static const unsigned int STABILIZING_POINT_TEXTURE_WIDTH = static_cast<int>( (MAX_STABILIZING_CIRCLE_RADIUS + 1.0f) * 2 );
static const float MAX_STABILIZING_RADIUS = 80;

class GlslShader;
class GlslProgram;

class StabilizingState
{
protected:
    GLint           m_point_radius_handle[MAX_STABILIZING_POINTS];
    GLint           m_point_tracking_radius_handle[MAX_STABILIZING_POINTS];
    GLint           m_point_position_handle[MAX_STABILIZING_POINTS];
    GLint           m_point_enabled_handle[MAX_STABILIZING_POINTS];
    GLint           m_point_sensitivity_handle[MAX_STABILIZING_POINTS];
    GLint           m_point_screen_position_handle[MAX_STABILIZING_POINTS];

    GLint           m_image_lower_left_corner_handle;
    GLint           m_point_texture_step_handle;
    GLint           m_image_texture_step_handle;
    GLint           m_image_texture_ratio_handle;
    GLint           m_point_texture_ratio_handle;
    GLint           m_point_texture_size_handle;

    GLuint          m_point_texture_id[MAX_STABILIZING_POINTS];
    float           m_point_radius[MAX_STABILIZING_POINTS];
    JahLayer*       m_base_layer;
    JahLayer*		m_point_layer[MAX_STABILIZING_POINTS];
    bool            m_point_enabled[MAX_STABILIZING_POINTS];
    float2          m_point_screen_position[MAX_STABILIZING_POINTS];
    float2          m_point_image_position[MAX_STABILIZING_POINTS];
    float2          m_point_previous_position[MAX_STABILIZING_POINTS];
    float2          m_point_previous_delta[MAX_STABILIZING_POINTS];
    float2          m_point_lower_left_corner_screen_coords[MAX_STABILIZING_POINTS];
    float2          m_point_lower_left_corner_image_coords[MAX_STABILIZING_POINTS];
    float           m_point_search_radius[MAX_STABILIZING_POINTS];
    float           m_point_sensitivity[MAX_STABILIZING_POINTS];
    int             m_point_checksum_tolerance[MAX_STABILIZING_POINTS];
    unsigned char*  m_point_pixel_buffer[MAX_STABILIZING_POINTS];
    int             m_point_texture_width;
    float2          m_image_lower_left_corner;
    float2          m_point_time_zero_position[MAX_STABILIZING_POINTS];
    float2          m_point_reference_position[MAX_STABILIZING_POINTS];
    int             m_point_reference_frame[MAX_STABILIZING_POINTS];


    GlslShader*                 m_fragment_shader;
    GlslProgram*                m_shader_program;
    GLhandleARB                 m_shader_program_handle;

public:
    std::vector<GlslShader*>    m_shader_vector;

public:
    GLint   getPointRadiusHandle(int point_number) { return m_point_radius_handle[point_number]; }
    void    setPointRadiusHandle(int point_number, GLint value) { m_point_radius_handle[point_number] = value; }
    GLint   getPointTrackingRadiusHandle(int point_number) { return m_point_tracking_radius_handle[point_number]; }
    void    setPointTrackingRadiusHandle(int point_number, GLint value) { m_point_tracking_radius_handle[point_number] = value; }
    GLint   getPointPositionHandle(int point_number) { return m_point_position_handle[point_number]; }
    void    setPointPositionHandle(int point_number, GLint value) { m_point_position_handle[point_number] = value; }
    GLint   getPointEnabledHandle(int point_number) { return m_point_enabled_handle[point_number]; }
    void    setPointEnabledHandle(int point_number, GLint value) { m_point_enabled_handle[point_number] = value; }
    GLint   getPointSensitivityHandle(int point_number) { return m_point_sensitivity_handle[point_number]; }
    void    setPointSensitivityHandle(int point_number, GLint value) { m_point_sensitivity_handle[point_number] = value; }
    GLint   getPointScreenPositionHandle(int point_number) { return m_point_screen_position_handle[point_number]; }
    void    setPointScreenPositionHandle(int point_number, GLint value) { m_point_screen_position_handle[point_number] = value; }

    GLint   getImageLowerLeftCornerHandle() { return m_image_lower_left_corner_handle; }
    void    setImageLowerLeftCornerHandle(GLint handle) { m_image_lower_left_corner_handle = handle; }
    GLint   getPointTextureStepHandle() { return m_point_texture_step_handle; }
    void    setPointTextureStepHandle(GLint handle) { m_point_texture_step_handle = handle; }
    GLint   getImageTextureStepHandle() { return m_image_texture_step_handle; }
    void    setImageTextureStepHandle(GLint handle) { m_image_texture_step_handle = handle; }
    GLint   getImageTextureRatioHandle() { return m_image_texture_ratio_handle; }
    void    setImageTextureRatioHandle(GLint handle) { m_image_texture_ratio_handle = handle; }
    GLint   getPointTextureRatioHandle() { return m_point_texture_ratio_handle; }
    void    setPointTextureRatioHandle(GLint handle) { m_point_texture_ratio_handle = handle; }
    GLint   getPointTextureSizeHandle() { return m_point_texture_size_handle; }
    void    setPointTextureSizeHandle(GLint handle) { m_point_texture_size_handle = handle; }

    GlslShader*   getFragmentShader() { return m_fragment_shader; }
    void          setFragmentShader(GlslShader* shader) { m_fragment_shader = shader; }
    GlslProgram*  getShaderProgram() { return m_shader_program; }
    void          setShaderProgram(GlslProgram* program) { m_shader_program = program; }
    GLhandleARB   getShaderProgramHandle() { return m_shader_program_handle; }
    void          setShaderProgramHandle(GLhandleARB handle) { m_shader_program_handle = handle; }

    unsigned char*  getPointPixelBuffer(int index) { return m_point_pixel_buffer[index]; }
    int             getPointChecksumTolerance(int index) { return m_point_checksum_tolerance[index]; }
    void            setPointChecksumTolerance(int index, int value) { m_point_checksum_tolerance[index] = value; }
    float           getPointSensitivity(int index) { return m_point_sensitivity[index]; }
    void            setPointSensitivity(int index, float value) { m_point_sensitivity[index] = value; }
    GLuint          getPointTextureId(int index) { return m_point_texture_id[index]; }
    GLuint*         getPointTextureIdPtr(int index) { return &(m_point_texture_id[index]); }
    JahLayer*       getBaseLayer() { return m_base_layer; }
    GLuint          getBaseLayerTextureId();
    void            setBaseLayer(JahLayer* layer) { m_base_layer = layer; }
    JahLayer*       getPointLayer(int index) { return m_point_layer[index]; }
    void			setPointLayer(int index, JahLayer* layer) { m_point_layer[index] = layer; }
    bool            getPointEnabled(int index) { return m_point_enabled[index]; }
    void            setPointEnabled(int index, bool value) { m_point_enabled[index] = value; }
    int             getSelectedTrackerPointNumber();
    float2&         getPointLowerLeftCornerScreenCoords(int point_number) { return m_point_lower_left_corner_screen_coords[point_number]; }
    void            setPointLowerLeftCornerScreenCoords(int point_number, float2& position) { m_point_lower_left_corner_screen_coords[point_number] = position; }
    float2&         getPointLowerLeftCornerImageCoords(int point_number) { return m_point_lower_left_corner_image_coords[point_number]; }
    void            setPointLowerLeftCornerImageCoords(int point_number, float2& position) { m_point_lower_left_corner_image_coords[point_number] = position; }
    float2&         getPointScreenPosition(int point_number) { return m_point_screen_position[point_number]; }
    void            setPointScreenPosition(int point_number, float2& position) { m_point_screen_position[point_number] = position; }
    float2&         getPointImagePosition(int point_number) { return m_point_image_position[point_number]; }
    void            setPointImagePosition(int point_number, float2& position) { m_point_image_position[point_number] = position; }
    float2&         getPointPreviousPosition(int point_number) { return m_point_previous_position[point_number]; }
    void            setPointPreviousPosition(int point_number, float2& position) { m_point_previous_position[point_number] = position; }
    float2&         getPointPreviousDelta(int point_number) { return m_point_previous_delta[point_number]; }
    void            setPointPreviousDelta(int point_number, float2& delta) { m_point_previous_delta[point_number] = delta; }
    int             getPointTextureWidth() { return m_point_texture_width; }
    void            setPointTextureWidth(int width) { m_point_texture_width = width; }
    void            setPointSearchRadius(int point_number, float value) { m_point_search_radius[point_number] = value; }
    float           getPointSearchRadius(int point_number) { return m_point_search_radius[point_number]; }
    void            setPointRadius(int point_number, float value) { m_point_radius[point_number] = value; }
    float           getPointRadius(int point_number) { return m_point_radius[point_number]; }
    void            setTrackerPoint(int point_number);
    float2&         getImageLowerLeftCorner() { return m_image_lower_left_corner; }
    void            setImageLowerLeftCorner(float2& position) { m_image_lower_left_corner = position; }
    float2&         getPointTimeZeroPosition(int point_number) { return m_point_time_zero_position[point_number]; }
    void            setPointTimeZeroPosition(int point_number, float2& position) { m_point_time_zero_position[point_number] = position; }
    float2&         getPointReferenceScreenPosition(int point_number) { return m_point_reference_position[point_number]; }
    void            setPointReferenceScreenPosition(int point_number, float2& position) { m_point_reference_position[point_number] = position; }
    int             getPointReferenceFrame(int point_number) { return m_point_reference_frame[point_number]; }
    void            setPointReferenceFrame(int point_number, int frame) { m_point_reference_frame[point_number] = frame; } 

};

#endif // COREEFFECTS_H




















































