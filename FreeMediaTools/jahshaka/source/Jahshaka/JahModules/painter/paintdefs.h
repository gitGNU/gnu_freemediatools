
//#include <qmainwindow.h>
#include <qgl.h>
#include <qpen.h>
#include <qpoint.h>
#include <qimage.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpointarray.h>
#include <qmenubar.h>
#include <qslider.h>
#include <qlabel.h>
#include <qbuttongroup.h>

#include <math.h>   //for drawing triangles and circles using trigonometry, etc.

class QMouseEvent;
class QResizeEvent;
class QPaintEvent;
class QToolButton;


 /*
 | a Canvas is a QGLwidget that you can draw on
*/
 //definitions used by Canvas : history
static const unsigned int PAINT_HISTORY_SIZE = 1000;
static const unsigned int INVALID_TEXTURE_ID = 0xffffffff;

 //definitions used by Canvas : mathematical
#define PI        3.14159265358979323846
#define twoPI     PI * 2
#define halfPI    PI / 2
#define quarterPI PI / 4
#define atanPI  atan(PI)
#define toolCirclePointsPerPI    24    //tool configuration

 //definitions used by Canvas : color data structures
typedef GLfloat color3[3];
typedef float color3f255[3];

 //definitions used by Canvas : convolutions
typedef float     float3[3];
typedef float3    kernel3x3[3];
typedef kernel3x3 convolutionKernels[10];
const convolutionKernels convolutionMatrix =
 { { { 0.0f, 0.0f, 0.0f },                      //fade
     { 0.0f, 1.0f, 0.0f },
     { 0.0f, 0.0f, 0.0f } },
   { { 0.0f, 0.0f, 0.0f },                      //intensify
     { 0.0f, 1.0f, 0.0f },
     { 0.0f, 0.0f, 0.0f } },
   { { 0.1111111f, 0.1111111f, 0.1111111f },    //blur
     { 0.1111111f, 0.1111111f, 0.1111111f },
     { 0.1111111f, 0.1111111f, 0.1111111f } },
   { { -0.117647f, -0.117647f, -0.117647f },    //sharpen
     { -0.117647f,  1.941176f, -0.117647f },
     { -0.117647f, -0.117647f, -0.117647f } },
   { { -0.05f, -0.07f, -0.05f  },               //Laplacian-of-Gaussian variant
     { -0.07f,  1.58f, -0.07f },
     { -0.05f, -0.07f, -0.05f  } },
   { {  0.0f,  0.0f,  0.0f },                   //edgeDetectX
     { -0.25f, 1.5f, -0.25f },
     {  0.0f,  0.0f,  0.0f  } },
   { { 0.0f, -0.25f, 0.0f },                    //edgeDetectY
     { 0.0f,  1.5f,  0.0f },
     { 0.0f, -0.25f, 0.0f } },
   { {  0.5f,  1.5f,  0.5f  },                  //sobel
     {  0.0f,  1.0f,  0.0f },
     { -0.5f, -1.5f, -0.5f  } },
   { { 0.75f,  -2.25f,  0.75f  },               //Laplacian
     { -2.25f,  4.0f,  -2.25f },
     { 0.75f,  -2.25f,  0.75f  } },
   { { 0.25f,  -0.75f,  0.25f  },               //Laplacian2
     { -0.75f,  1.0f,  -0.75f },
     { 0.25f,  -0.75f,  0.25f  } } };
enum convolutionType { fade, intensify, blur, sharpen, lapOfGauss,
                       edgeDetectX, edgeDetectY,
                       sobel, laplacian, laplacian2 };

//list of the tools supported by Canvas
enum CanvasTool { none, pen, line, rectangle, rectangleFilled, circle,
                  circleFilled, triangle, triangleFilled, paintBucket, paintBucketFillAll, eraser,
                  foregroundColorPicker, backgroundColorPicker };
