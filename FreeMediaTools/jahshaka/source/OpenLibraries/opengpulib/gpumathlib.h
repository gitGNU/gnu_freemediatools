/*******************************************************************************
**
** The header file for the gpumathlib class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc..
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef GPUMATHLIB_H
#define GPUMATHLIB_H

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <math.h>
#include <qstring.h>
#include <string>
#include <vector>

#include "glsl_objects.h"

//class JahLayer;
class QImage;
class TextureImage;

//////////////////////////////////////////////////////////////////
//this is for gpgpu support, using pbuffers, currently disabled
//need to define this before calling gpumathlib as its also used in there
#ifdef NVIDIA_GPGPU
#ifndef WIN32
#include <GL/glx.h>
#endif
#endif

static const float YUV_SCALE_Y = 219.0 / 256.0;
static const float YUV_SCALE_UV = 224.0 / 256.0;
static const float YUV_SCALE_Y_INVERSE = 1.0 / YUV_SCALE_Y;
static const float YUV_SCALE_UV_INVERSE = 1.0 / YUV_SCALE_UV;

static const float PI = 3.14159265f;
static const float TWO_PI = PI * 2.0f;

static const float DEFAULT_TEXT_EXTRUDE_DEPTH = 20.0f;

//this is used to select playback color modes
enum colorMode        { D_RED, D_GREEN, D_BLUE, D_ALPHA, D_ALL };

namespace jahstd
{
    template <class T>
    T Max(T a, T b)
    {
        T result;
        result = (a > b)? a : b;
        return (result);
    }

    template <class T>
    T Min(T a, T b)
    {
        T result;
        result = (a < b)? a : b;
        return (result);
    }

    OPENGPULIB_API int numberOfDecimalDigits(int number);
    OPENGPULIB_API float Roundf(float number);
    OPENGPULIB_API bool glslSupport();
    OPENGPULIB_API int Even(int number);
    OPENGPULIB_API bool doesFileExist(std::string filename);
    OPENGPULIB_API bool copyBinaryFile(std::string dest_filename, std::string src_filename);
    OPENGPULIB_API void copyTextFile(std::string dest_filename, std::string src_filename);
    OPENGPULIB_API void glGetTextureSubImage(int x_offset, int y_offset, int buffer_width, int buffer_height, 
                            int texture_width, int texture_height, unsigned char* buffer);
};



class OPENGPULIB_API float4
{
public:
    float                x;
    float                y;
    float                z;
    float                w;

    float4();
    float4(float X, float Y, float Z, float W);
    float4(float scalar);
    void clamp();
    void clampToOne();
    float max4();

    float min4();
    float max3();
    float min3();
    static float dot(float4& vector1, float4& vector2);
    static float4 crossProduct(float4& vector1, float4& vector2);
    static float4 pow(float4& vector, float scalar);

    void operator = (float scalar);
    void operator = (float4 vector);
    void operator -= (float scalar);
    void operator += (float scalar);
    void operator *= (float scalar);
    void operator /= (float scalar);
    void operator += (float4& vector);
};

OPENGPULIB_API float4 operator - (float scalar, float4& vector);
OPENGPULIB_API float4 operator - (float4& vector, float scalar);
OPENGPULIB_API float4 operator + (float4& vector, float scalar);
OPENGPULIB_API float4 operator / (float4 vector, float scalar);
OPENGPULIB_API float4 operator - (float4& vector1, float4& vector2);
OPENGPULIB_API float4 operator + (float4& vector1, float4& vector2);

class OPENGPULIB_API color4 : public float4
{
public:
    float alpha();
    float blue();
    float green();
    float red();

    color4();
    color4(float red, float green, float blue, float alpha);
    void operator = (float4 vector);
    bool operator == (float4& vector);
};

class OPENGPULIB_API float2
{
public:
    float                x;
    float                y;

    float2();
    float2(float X, float Y)     ;
    float2(float V);
    float length();
    void clampToOne();
    void floor();
    void ceil();
    void round();
    float max2();
    float min2();
    float aspect();
    static float dot(float2& vector1, float2& vector2);
    void operator = (float scalar);
    void operator -= (float scalar);
    void operator += (float scalar);
    void operator += (float2& vector);
    void operator -= (float2& vector);
    void operator /= (float scalar);
    void operator *= (float2& vector);
    void operator *= (float scalar);
    void operator /= (float2& vector);
    bool operator == (float2& vector);
    bool operator != (float2& vector);
};

OPENGPULIB_API float2 operator - (float scalar, float2& vector);
OPENGPULIB_API float2 operator - (float2& vector, float scalar);
OPENGPULIB_API float2 operator + (float2& vector, float scalar);
OPENGPULIB_API float2 operator / (float2& vector, float scalar);
OPENGPULIB_API float2 operator / (float scalar, float2& vector);
OPENGPULIB_API float2 operator / (float2& vector1, float2& vector2);
OPENGPULIB_API float2 operator - (float2& vector1, float2& vector2);
OPENGPULIB_API float2 operator + (float2& vector1, float2& vector2);

static inline float
minf(float a, float b)
{
    return (a < b) ? a : b;
}

static inline float
maxf(float a, float b)
{
    return (a > b) ? a : b;
}

OPENGPULIB_API bool getPowerOfTwoTextureSize(int& texture_width, int& texture_height, int width, int height);

extern "C" {

//definitions at the top
#define PI                   3.14159265358979323846
#define TWO_PI               (PI * 2.0)

#define PIXEL_BUFFER_WIDTH          2048
#define PIXEL_BUFFER_HEIGHT         2048

//used for distortion, gradient and turbulence tables
#define DISTORTION_TABLE_SIZE           32
#define GRADIENT_TABLE_SIZE             ((DISTORTION_TABLE_SIZE * 2) + 2)
#define TURBULENCE_TEXTURE_SIZE         1024

//variables used in gpu functions

typedef float4 half4;
typedef float4 half3;
typedef float4 half2;
typedef float half;


struct OPENGPULIB_API Image
{
    unsigned long        sizeX;
    unsigned long        sizeY;
    unsigned char*       data;
};

//Vertex vertices[number_of_vertices];
struct OPENGPULIB_API Vertex
{
    float                   s, t;
    float                   x, y, z;
};

//dont think this is used anymore
struct OPENGPULIB_API pixel_t
{
    int        x;
    int        y;
};

////////////////////////////////////////////////////////////
//math utility routines

OPENGPULIB_API float sfrand(void);
OPENGPULIB_API float frand(void);

OPENGPULIB_API void normalize3f(float& x, float& y, float& z);
OPENGPULIB_API float normalizefloat4(float4& vector);
OPENGPULIB_API void get_cross_product_4( float4& result, float4 vector1, float4 vector2);
OPENGPULIB_API void normalize_vector4(float4& vector);
OPENGPULIB_API void get_vector_from_points(float4& vector, float4 point1, float4 point2);

OPENGPULIB_API void rgb_to_hsv(float4& hsv, float4& rgb);
OPENGPULIB_API void hsv_to_rgb(float4& rgb, float4& hsv);
OPENGPULIB_API void color_enhance(float4& rgb, float4& hsv, float4& enhance);
OPENGPULIB_API void color_enhance2(float4& rgb, float4& hsv, float4& enhance); // for new color corrector
OPENGPULIB_API void rgb_to_yuv(float4& yuv, float4& rgb);
OPENGPULIB_API void yuv_to_rgb(float4& rgb, float4& yuv);
OPENGPULIB_API void yuv_to_hsv(float4& hsv, float4& yuv);
OPENGPULIB_API void hsv_to_yuv(float4& yuv, float4& hsv);
OPENGPULIB_API void find_rgb_range_from_hsv(float4& hsv_low, float4& hsv_high, float4& rgb_low, float4& rgb_high);

OPENGPULIB_API void invertRGBA(int size, unsigned int* buffer);



////////////////////////////////////////////////////////////
//debugging
OPENGPULIB_API void check_gl(void);
OPENGPULIB_API void find_shader_program_error(unsigned char * source, char* program_name);
void debug_arbdata(void);

////////////////////////////////////////////////////////////
//data io routines

OPENGPULIB_API unsigned char* loadshaderfile(const char *name);
int ImageLoad(char* filename, Image* image);

//////////////////////////////////////////////////////////////
//texture generators
OPENGPULIB_API void initVertexNoiseConstants(int table_size, int*& permutation_table, float4*& gradient_table, int gradient_size);
OPENGPULIB_API void createRandomTexture( int width, int height,GLuint & random_texture );
OPENGPULIB_API void createContrastTexture( int width, int height, double density, double exponent, GLuint & contrast_texture );

OPENGPULIB_API void getRectangleSubImage( unsigned char* dest, unsigned char* source, 
                                         int x_offset, int y_offset,
                                         int sub_width, int sub_height,
                                         int full_width, int full_height, int texture_width, bool invert = false );

OPENGPULIB_API void readTextureSubImageUchar(
                    GLuint texture_id, int texture_width, int texture_height,
                    int lower_left_pixel_offset_x, int lower_left_pixel_offset_y,
                    unsigned char* buffer, int buffer_width, int buffer_height);

OPENGPULIB_API void createEmpty2DTexture(GLuint* texture_id_ptr, GLenum format, int width, int height);



//////////////////////////////////////////////////////////////
// OpenGL matrix tracking
void matrixInvert4x4(float* inverse, float* matrix);
void matrixSwapRows4x4(float* row1, float* row2);
void matrixPrint4x4(float* matrix, const char* name);
void matrixPrintTranspose4x4(float* matrix, const char* name);
void matrixMultiply4x4(float* matrix_out, float* matrix1, float* matrix2);
void vectorMultiply(float* matrix_out, int vector_length, float* vertical_vector, float* horizontal_vector);
OPENGPULIB_API void getMVPMatrices(float* modelview, float* projection, float* mvp, float* modelviewIT, float* texture);
void matrixTranspose4x4(float* matrix);


//////////////////////////////////////////////////////////////
//for arb support

OPENGPULIB_API void loadJahshakaBasicArb( GLint width, GLint height, float camera_distance,
                           unsigned char* vertex_program,
                           GLuint& vertex_program_handle );

int countARBFPInstructions(GLubyte* fragment_program);
bool isAnARBFPInstruction(GLubyte* string);

//loads the shader in and checks to see if complex arb is supported
OPENGPULIB_API bool checkComplexArbSupport(QString name, int max_fp_instructions, int max_vp_instructions);

//////////////////////////////////////////////////////////////
//for gpgpu support

#ifdef NVIDIA_GPGPU
void create_pbuffer(int width, int height);
#endif // NVIDIA_GPU

}

class JahControl;

/*
  TODO:
  making a static instance 
  is sometimes a better idea
*/
class OPENGPULIB_API Globals
{
public:
    Globals();
    static std::string&     getGlVendor() ;
    static void             setGlVendor(std::string& vendor);
    static std::string&     getGlRenderer() ;
    static void             setGlRenderer(std::string& renderer);
    static std::string&     getGlVersion();
    static void             setGlVersion(std::string& version);
    static int              getGlMaxTextureSize();
    static void             setGlMaxtextureSize(int size);

    static bool             isWin32();
    static void             setisWin32(bool flag) ;
    static void             setEnableScriptingButton(bool flag);
    static bool             getEnableScriptingButton() ;
    static void             setSuppressPopups(bool flag);
    static bool             getSuppressPopups() ;
    static void             setAutoSaveEnable(bool flag);
    static bool             getAutoSaveEnable();
    static void             setCameraDistance(float distance);
    static float            getCameraDistance() ;
    static void             setShowLights(bool flag);
    static bool             getShowLights() ;
    static void             setJahControl(JahControl* jah_control);
    static JahControl*      getJahControl() ;
    static void             setRendering(bool flag);
    static bool             getRendering() ;
    static float            getKeyframerZoomFactor() ;
    static void             setKeyframerZoomFactor(float value) ;
    static int              getKeyframerYOffset();
    static void             setKeyframerYOffset(int value);

protected:
    static std::string      m_gl_vendor;
    static std::string      m_gl_renderer;
    static std::string      m_gl_version;
    static int              m_gl_max_texture_size;
    static bool             m_is_win32;
    static bool             m_enable_scripting_button;
    static bool             m_suppress_popups;
    static bool             m_auto_save_enable;
    static float            m_camera_distance;
    static bool             m_show_lights;
    static JahControl*      m_jah_control;
    static bool             m_rendering;
    static float            m_keyframer_zoom_factor;
    static int              m_keyframer_y_offset;
};


#endif
