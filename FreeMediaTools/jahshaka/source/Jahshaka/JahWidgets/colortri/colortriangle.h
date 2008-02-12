/***************************************************************************
** >>>> NOTE: WE ARE IN PROCESS OF REPLACING ALL PREVIOUS COLORTRI CODE WITH NEW CODE.  <<<<<<<
** >>>> ALL COLOR CORRECTOR CODE IS NEW <<<<<
**
**
** Original qt color triangle code copyright: 
** Copyright (C) 2003-2004 Trolltech AS.  All rights reserved.
**
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Solutions License Agreement provided
** with the Solution.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** Please email sales@trolltech.com for information
** about Qt Solutions License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*/
#ifndef COLORTRIANGLE_H
#define COLORTRIANGLE_H
#include <qwidget.h>
#include <qframe.h>
#include <qimage.h>
#include <qcolor.h>

class ColorTriangle;
class ColorDisplay;

/*
#if defined(Q_WS_WIN)
#  if !defined(QT_QTCOLORTRIANGLE_EXPORT) && !defined(QT_QTCOLORTRIANGLE_IMPORT)
#    define QT_QTCOLORTRIANGLE_EXPORT
#  elif defined(QT_QTCOLORTRIANGLE_IMPORT)
#    if defined(QT_QTCOLORTRIANGLE_EXPORT)
#      undef QT_QTCOLORTRIANGLE_EXPORT
#    endif
#    define QT_QTCOLORTRIANGLE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTCOLORTRIANGLE_EXPORT)
#    undef QT_QTCOLORTRIANGLE_EXPORT
#    define QT_QTCOLORTRIANGLE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTCOLORTRIANGLE_EXPORT
#endif
*/
//class QT_QTCOLORTRIANGLE_EXPORT QtColorTriangle : public QWidget

struct DoublePoint
{
    double x;
    double y;

    DoublePoint() : x(0), y(0) {}
    DoublePoint(double xx, double yy) : x(xx), y(yy) {}
    DoublePoint(const DoublePoint &c) : x(c.x), y(c.y) {}
};

struct DoubleColor
{
    double r, g, b;

    DoubleColor() : r(0.0), g(0.0), b(0.0) {}
    DoubleColor(double red, double green, double blue) : r(red), g(green), b(blue) {}
    DoubleColor(const DoubleColor &c) : r(c.r), g(c.g), b(c.b) {}
};



struct TVertex {
    DoubleColor color;
    DoublePoint point;

    TVertex(const DoubleColor &c, const DoublePoint &p) : color(c), point(p) {}
    TVertex(const QColor &c, const DoublePoint &p)
	: color(DoubleColor((double) c.red(), (double) c.green(),
			    (double) c.blue())), point(p) {}
};

const double DPI = 3.14159265358979323846264338327950288419717;
const double TWOPI = 2.0*DPI;

/*
    Used to store color values in the range 0..255 as doubles.
*/
/*
    Used to store x-y coordinate values as doubles.
*/

/*
    Used to store pairs of DoubleColor and DoublePoint in one structure.
*/

/*! \internal

Swaps the TVertex at *a with the one at *b.
 */
inline void swap(TVertex **a, TVertex **b)
{
    TVertex *tmp = *a;
    *a = *b;
    *b = tmp;
}



class ColorTriangle : public QFrame
{
    Q_OBJECT

public:
    ColorTriangle(QWidget *parent = 0, const char *name = 0);
    ~ColorTriangle();

    QSize sizeHint() const;

    void polish();

    QColor color() const;

    // The mode changes the behaviour between color triangle chooser and color corrector.
    // TODO? I soon begin to think there should be a common base class, since all the if/else blocks are
    // getting too much really :(
    enum Mode { ColorTriangleMode, ColorCorrectorMode } themode;
    void setMode( Mode mode );

    // TODO: store sets of hueshift and saturation for each tonal range. TODO use these! 
    enum TonalRange { Master, Shadows, Midtones, Highlights } thelevel;

    // Returns the saturation for the outer donut in ColorCorrector mode only
    int correctorSaturation() { return outerDonutSaturation; };

    // Returns hue tint. This is the hue angle, with red at zero degrees. Forced to be in range 0-360 degrees.
    int correctorHueOffset();

    // Returns hue strength in colorcorrector mode. This is the hue vector length, in range 0-100 (double precision to avoid rounding issues). 
    double correctorHueStrength();

    // Returns the hue shift, in range 0-360 degrees. The hue shift affects the outer donut in ColorCorrector mode only.
    int correctorHueShift() { return outerDonutHueShift; };

signals:
    // Used to tell spin boxes etc to update, internal to our ui. 
    void colorChangedInternal(const QColor &col);
    // Use this signal for external use, e.g. keyer. It avoids cyclical signal/slots.
    void colorChangedExternal( const QColor &col );

    // emitted when the user drags the outter wheel in ColorCorrectorMode.
    void correctorHueShiftChanged( int correctorHueShift );

    // emitted when user changes the angle of the selector in ColorCorrectorMode. The angle is the hue offset (or 'tint'),
    // reported in units of 1/10th degrees. Hence max is 3600.
    void correctorHueOffsetChanged( int correctorHueOffset );
   
     // emitted when user changes the radius of the selector in ColorCorrectorMode.
    void correctorHueStrengthChanged( int correctorHueStrength );

    // emitted when user changes either the offset angle or the strength of the tint in ColorCorrectorMode.
    // NB if the user is moving the selector manually, the offset and strength will almost always change 
    // together, hence its better to avoid catching 2 separate signals and doing unnecessary updates for both changes.
    void correctorWheelChanged( int offset, int strength );

public slots:
    void setColor(const QColor &col);
    void setColorSilently( const QColor &col ); // as setColor but emits no signals after setting color.
    // As above but guarantees only S and V change. The problem is that near the corners of the triangle the color
    // goes to black (for example), which means we forget our HUE when moving back into the triangle. 
    void setColorSilentlyConstantHue( const QColor &col );

    // shifts the hue by the given amount. Forces hue to be in range 0-360 after any change. 
    // This hue shift affects the outer donut in ColorCorrector mode only.
    void setCorrectorHueShift( int hueshift );

    // Sets the saturation for the outer donut in ColorCorrector mode only.
    // If sat <0 it is set to 0. If sat is > 255 it is set to 255.
    void setCorrectorSaturation( int sat );
  
    // Sets the hue offset (aka 'tint'). For accuracy, angle is meausre in 1/10th of a degree, with red at zero degrees. 
    // Max angle is therefore 3600. The selector moves accordingly.
    void setCorrectorHueOffset( int angle );

    // Sets the hue strength. Values outside range 0-100 are ignored The selector moves accordingly.
    // Note, int or double overloads, the latter avoids rounding issues.
    void setCorrectorHueStrength( int rad );
    void setCorrectorHueStrength( double rad );

    // Moves the selector to arbitrary point and calls for a screen update. 
    // The calling function should check the point is valid for its needs.
    void moveSelector( DoublePoint pos );

protected:
    void drawContents(QPainter *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);
    void resizeEvent(QResizeEvent *);
    void drawTrigon(QImage *p, const DoublePoint &a, const DoublePoint &b,
		    const DoublePoint &c, const QColor &color);


private:
    double radiusAt(const DoublePoint &pos, const QRect &rect) const;
    double angleAt(const DoublePoint &pos, const QRect &rect) const;
    DoublePoint movePointToTriangle(double x, double y, const TVertex &a,
				    const TVertex &b, const TVertex &c) const;

    DoublePoint pointFromColor(const QColor &col) const;

    // For color triangle only (re-name?). See also colorFromDonut.
    QColor colorFromPoint(const DoublePoint &p) const;

    // For saturation donut
    QColor colorFromDonut(const DoublePoint &p) const;

    // Used by genBackground to draw the color donuts. If saturationdonut is 
    // set then the saturation is scaled by radius (usually one sets zero innerRadius for this).
    // Else, a hue donut is drawn, with max saturation and value, but hue shifted by the 
    // member outerDonutHueShift (shift starts at 0, so top-most color in wheel is red).
    // Image is drawn onto the given bg image.
    void drawDonut ( QImage &bg, double innerRadius, bool saturationdonut = false );

    // If ColorTriangleMode mode then draws single outer HUE donut, 
    // If ColorCorrectorMode then draw outer HUE donut and inner (static)
    // saturation donut.
    void genBackground();
    
    QImage bg;
    double a, b, c;
    DoublePoint pa, pb, pc, pd;

    // If the selector is moved to the exact centre we dont have a valid vector to move
    // along for increasing the hue strength, hence we store the last known non-center X, Y here. 
    double lastNoncenterHueVectX, lastNoncenterHueVectY;

    QColor curColor;
    int curHue;

    bool mustGenerateBackground;
    int penWidth;
    int ellipseSize;

    int outerRadius;
    DoublePoint selectorPos;

    int outerDonutHueShift;
    int outerDonutSaturation;

    enum SelectionMode {
	Idle,
	SelectingHue,
	SelectingSatValue
    } selMode;
};




#endif
