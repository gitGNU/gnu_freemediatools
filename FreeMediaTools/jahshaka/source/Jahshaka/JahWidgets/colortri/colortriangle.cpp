/***************************************************************************
**
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

// Notes: be aware that most data is store as double, but signal/slots may be using int (e.g. from a slider).
// One must look out for situations where a double changes here, then emits a (rounded) int signal to a slider which is 
// later used in a slot where it could be converted back to double.
//


#include "colortriangle.h"
#include <qmap.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qevent.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qframe.h>
#include <qtooltip.h>

#include <math.h>


/*!
    Constructs a color triangle widget with the given \a parent and
    called \a name.
*/
ColorTriangle::ColorTriangle(QWidget *parent, const char *name)
    : QFrame(parent, name, WNoAutoErase), bg(sizeHint(), 32), selMode(Idle)
{
    themode = ColorTriangleMode; // default mode. See setMode

    setLineWidth(1);
    setMidLineWidth(1);
    setFrameShape(Box);
    setFrameShadow(Sunken);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFocusPolicy(StrongFocus);

    mustGenerateBackground = true;

    outerDonutHueShift = 0; // TODO: memb list instn.
    outerDonutSaturation = int( 255 * 0.75 ); // 75% arbitrary, means we can show more/less sat too.

    QColor tmp;
    tmp.setHsv(76, 184, 206);
    setColor(tmp);

    curColor = QColor(255, 255, 255 );
    curHue = 0; 
}

/*!
  Destructs the color triangle.
*/
ColorTriangle::~ColorTriangle()
{
}

/*!
    \internal

    Generates the first background image.
*/
void ColorTriangle::polish()
{   //debug( "In polish()" );
    outerRadius = (contentsRect().width() - 1) / 2;
    if ((contentsRect().height() - 1) / 2 < outerRadius)
	outerRadius = (contentsRect().height() - 1) / 2;

    penWidth = (int) floor(outerRadius / 50.0);
    ellipseSize = (int) floor(outerRadius / 12.5);

    double cx = (double) contentsRect().center().x();
    double cy = (double) contentsRect().center().y();

    pa = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
    pb = DoublePoint(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
    pc = DoublePoint(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));
    pd = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 10.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 10.0))));

    if ( themode == ColorTriangleMode )
    {   // Find the current position of the selector
        selectorPos = pointFromColor(curColor);
    }
    else if ( themode == ColorCorrectorMode )
    {    moveSelector( DoublePoint(cx, cy) );
        // The selector starts in the center, so we dont have a valid vector to move along for 
        // increasing hue strength etc, so we just start with vector along x (green).
        lastNoncenterHueVectX = selectorPos.x +2;
        lastNoncenterHueVectY = selectorPos.y +0; 
    }

    update();
}

/*! \reimp
 */
QSize ColorTriangle::sizeHint() const
{
    return QSize(50, 50);
}


void ColorTriangle::drawDonut ( QImage &bg, double innerRadius, bool saturationdonut )
{
    // The border is the distance from the edge of the contents rect
    // to the outer edges of the hue donut.
    double border = 1;

    int donutradius = outerRadius;
    if ( saturationdonut )
        donutradius-= outerRadius /5; 
        

    // Draw the hue donut
    QColor color;
    for (int y = 0; y < bg.height(); ++y) {

        QRgb *rgb = reinterpret_cast<QRgb *>(bg.scanLine(y));
        for (int x = 0; x < bg.width(); ++x) {
            // For each point, find the radius and determine wether it
            // is inside the inner and outer edges of the hue donut.
            DoublePoint point((double) x, (double) y);
            double radius = radiusAt(point, bg.rect());
            
            if ( radius < 0.5 ) // deal with very centre as a special case since angle etc badly defined.
            {
              // Draw
                QColor color(white);
              *rgb++ = color.rgb();
            }
            else if (radius < donutradius && radius > innerRadius) {

                // Find the angle of this point, and use it to find
                // the appropriate hue value at this point.
                double angle2 = angleAt(point, bg.rect());
                if (angle2 < 0) angle2 += TWOPI;

                // Set the color to draw to the calculated hue, and
                // with maximum value and saturation.
                double angle = angle2 - DPI/2.0;
                if (angle < 0) angle += TWOPI;

                // Outer donut color, taking into account the hue shift.
                int hue = 360 - outerDonutHueShift - (int) ((angle * 360.0) / TWOPI);
                if ( hue > 360 ) hue-=360;
                else if ( hue < 0 ) hue+= 360;

                color.setHsv( hue, outerDonutSaturation, 255);

                if ( saturationdonut )
                {  
                    int hue = 360 - (int) ((angle * 360.0) / TWOPI);
                    int sat = 0;
                    if ( (donutradius - innerRadius ) > 1 )
                        sat = int( 0.75 * 255 * radius / ( donutradius - innerRadius ) ); // 75% arbitrary, but looks better and means we can show more/less sat too.
                    color.setHsv( hue, sat, 255 );
                }

                // Shade the color depending on where on the donut
                // this point is.
                if (radius < innerRadius + border || radius > donutradius - border) {
                    QColor bc = colorGroup().mid();
                    int r = (color.red() + bc.red()) / 2;
                    int g = (color.green() + bc.green()) / 2;
                    int b = (color.blue() + bc.blue()) / 2;

                    double n = angle2 + DPI/4;
                    if (n < 0) n += TWOPI;

                    double fact;
                    if (radius > donutradius - border)
                        fact = (1.5 - cos(n)/2);
                    else
                        fact = (1.5 + cos(n)/2);

                    r = (int) ((double)r * fact);
                    g = (int) ((double)g * fact);
                    b = (int) ((double)b * fact);

                    if (r > 255) r = 255;
                    if (g > 255) g = 255;
                    if (b > 255) b = 255;

                    color = QColor(r, g, b);
                }

                // Draw
                *rgb++ = color.rgb();
            } else 
            {    // Skip
                ++rgb;
            }
        }
    }
}

QColor ColorTriangle::colorFromDonut(const DoublePoint &p) const
{   // Saturation donut only.

    QColor color;
    int donutradius = outerRadius - outerRadius /5;
    int innerRadius = 0; // (saturation donut).
    double radius = radiusAt( p, contentsRect() );
    double angle2 = angleAt( p , contentsRect() );
    if (angle2 < 0) angle2 += TWOPI;
    double angle = angle2 - DPI/2.0;
    if (angle < 0) angle += TWOPI;

//    int hue = 360 - (int) ((angle * 360.0) / TWOPI);
    int hue = 360 - (int) ((angle * 360.0) / TWOPI);
    if ( hue > 360 ) hue-=360;
        else if ( hue < 0 ) hue+= 360;
    int sat;
    if ( radius < 0.001 )
        sat = 0;
    else sat = int ( 255 * radius / ( donutradius - innerRadius) );
    color.setHsv( hue, sat, 255 );
    
    //QString conv;
    //debug( " colorfromdonut " + conv.setNum( hue ) +  " " +  conv.setNum( sat ) );

    return color;

}

void ColorTriangle::genBackground()
{
    // Create an image of the same size as the contents rect.
    bg = QImage(contentsRect().size(), 32);

    // Fill it with a background color.
    bg.fill(colorGroup().mid().rgb());

    // Find the inner radius of the hue donut.
    double innerRadius = outerRadius - outerRadius / 5;

    // Draw the outer hue donut
    drawDonut ( bg, innerRadius );

    // If ColorCorrectorMode, draw inner area
    if ( themode == ColorCorrectorMode )
        drawDonut( bg, 0, true );

}

/*!
    \internal

    Selects new hue or saturation/value values, depending on where the
    mouse button was pressed initially.
*/
void ColorTriangle::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->state() & LeftButton) == 0)
        return;

    DoublePoint depos((double) e->pos().x(), (double) e->pos().y());
    bool newColor = false;

    if (selMode == SelectingHue) { // (selecting outer donut this applies for both modes)
	// If selecting hue, find the new angles for the points a,b,c
	// of the triangle. The following update() will then redraw
	// the triangle.
	a = angleAt(depos, contentsRect());
	b = a + TWOPI / 3.0;
	c = b + TWOPI / 3.0;
	if (b > TWOPI) b -= TWOPI;
	if (c > TWOPI) c -= TWOPI;

	double am = a - DPI/2;
	if (am < 0) am += TWOPI;

	curHue = 360 - (int) (((am) * 360.0) / TWOPI);
	int h,s,v;
	curColor.getHsv(&h, &s, &v);
    if (curHue != h) {
	    newColor = true;
	    curColor.setHsv(curHue, s, v); // (for ALL modes, else outer donut rotation breaks?)
    }

    // For ColorCorrector mode: tell the hue shift to change, and hence rotate the outer donut.
    if ( themode == ColorCorrectorMode )
    {
        setCorrectorHueShift( correctorHueShift() + curHue - h );
        emit correctorHueShiftChanged( correctorHueShift() );
    }

	double cx = (double) contentsRect().center().x();
	double cy = (double) contentsRect().center().y();

	pa = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
	pb = DoublePoint(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
	pc = DoublePoint(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));
	pd = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 10.0))),
			 cy - (sin(a) * (outerRadius - (outerRadius / 10.0))));

    if ( themode == ColorTriangleMode )
    	selectorPos = pointFromColor(curColor);

    } // ( end of if block for selMode == SelectingHue ) 
    else  // (not selecting hue in outer donut)
    {
	TVertex aa(black, pa);
	TVertex bb(black, pb);
	TVertex cc(black, pc);

	TVertex *p1 = &aa;
	TVertex *p2 = &bb;
	TVertex *p3 = &cc;
	if (p1->point.y > p2->point.y) swap(&p1, &p2);
	if (p1->point.y > p3->point.y) swap(&p1, &p3);
	if (p2->point.y > p3->point.y) swap(&p2, &p3);

    if ( themode == ColorTriangleMode )
	    selectorPos = movePointToTriangle(depos.x, depos.y, aa, bb, cc);
    else
    { // let the point move anywhere within the inner donut
        double prevradius = (radiusAt( selectorPos, bg.rect()) );
        selectorPos = DoublePoint( depos.x, depos.y );
        double radius = radiusAt( depos, bg.rect());
        double angledeg = 360.0 * angleAt( depos, bg.rect() ) / TWOPI;
        if ( radius > ( outerRadius - outerRadius/5 ) )
            setCorrectorHueStrength ( 100 ); // beyond donut, set it to be max strength.
        if ( int(prevradius) != int(radius) ) // (NB the signal/slots for sliders work in ints).
            emit correctorHueStrengthChanged( int( correctorHueStrength() ) );
       // if ( int(prevangledeg) != int(angledeg) )
         //   emit correctorHueOffsetChanged( int(angledeg) ); // NB an int signal for offset, which is really a double quantity.
                 
            emit correctorHueOffsetChanged( int (angledeg*10.0 ));

            // Best to pick up signal with both changes (avoid 2 updates).
            emit correctorWheelChanged( int (angledeg*10.0 ), int( correctorHueStrength() ) );
    }

   if ( themode == ColorTriangleMode )
    {   QColor col = colorFromPoint(selectorPos);
	    if ( col != curColor) 
        {
	        // Ensure that hue does not change when selecting
	        // saturation and value.
	        int h,s,v;
	        col.getHsv(&h, &s, &v);
	        curColor.setHsv(curHue, s, v);
	        newColor = true;
        }
    }
    else if ( themode == ColorCorrectorMode )
    {   QColor col = colorFromDonut( selectorPos );
        if ( col != curColor )
            newColor = true;
    }
    }
    if (newColor)
    {  
    	emit colorChangedInternal(curColor);
        emit colorChangedExternal(curColor); // BAZ NEW
    }

    update();
}

/*!
    \internal

    When the left mouse button is pressed, this function determines
    what part of the color triangle the cursor is, and from that it
    initiates either selecting the hue (outside the triangle's area)
    or the saturation/value (inside the triangle's area).
*/
void ColorTriangle::mousePressEvent(QMouseEvent *e)
{
    // Only respond to the left mouse button.
    if (e->button() != LeftButton)
	return;

    DoublePoint depos((double) e->pos().x(), (double) e->pos().y());
    double rad = radiusAt(depos, contentsRect());
    bool newColor = false;

    // As in mouseMoveEvent, either find the a,b,c angles or the
    // radian position of the selector, then order an update.
    if (rad > (outerRadius - (outerRadius / 5))) {
	
    selMode = SelectingHue;

	a = angleAt(depos, contentsRect());
	b = a + TWOPI / 3.0;
	c = b + TWOPI / 3.0;
	if (b > TWOPI) b -= TWOPI;
	if (c > TWOPI) c -= TWOPI;

	double am = a - DPI/2;
	if (am < 0) am += TWOPI;

	curHue = 360 - (int) ((am * 360.0) / TWOPI);
	int h,s,v;
	curColor.getHsv(&h, &s, &v);

	if (h != curHue) {
	    newColor = true;
	    curColor.setHsv(curHue, s, v);
	}

	double cx = (double) contentsRect().center().x();
	double cy = (double) contentsRect().center().y();

	pa = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
	pb = DoublePoint(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
	pc = DoublePoint(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
			 cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));
	pd = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 10.0))),
			 cy - (sin(a) * (outerRadius - (outerRadius / 10.0))));

    // Move the selector and emit colorChanged only if color chooser/triangle mode.
    if ( themode == ColorTriangleMode )
    {	selectorPos = pointFromColor(curColor);
        emit colorChangedInternal(curColor); // (NB no signal if colorcorrector)
    }
    else if ( themode == ColorCorrectorMode )
    {   
        // (TODO something here?)
    }

    } else {
	selMode = SelectingSatValue;

	TVertex aa(black, pa);
	TVertex bb(black, pb);
	TVertex cc(black, pc);

	TVertex *p1 = &aa;
	TVertex *p2 = &bb;
	TVertex *p3 = &cc;
	if (p1->point.y > p2->point.y) swap(&p1, &p2);
	if (p1->point.y > p3->point.y) swap(&p1, &p3);
	if (p2->point.y > p3->point.y) swap(&p2, &p3);

    if ( themode == ColorTriangleMode )
    	selectorPos = movePointToTriangle(depos.x, depos.y, aa, bb, cc);
    else if ( themode == ColorCorrectorMode )
    {   selectorPos = depos; 
        emit correctorHueStrengthChanged( int( correctorHueStrength() ) );         
        emit correctorHueOffsetChanged( correctorHueOffset() );
        emit correctorWheelChanged( correctorHueOffset(), int( correctorHueStrength() ) );
    }

    QColor col;
    if ( themode == ColorTriangleMode )
    	col = colorFromPoint(selectorPos);
    else if ( themode == ColorCorrectorMode )
        col = colorFromDonut( selectorPos ); 
	if (col != curColor) {
	    curColor = col;
	    newColor = true;
	}
    }

    if (newColor)
    {	emit colorChangedInternal(curColor);
        emit colorChangedExternal(curColor); // BAZ NEW. needed
    }

    update();
}

/*!
    \internal

    Stops selecting of colors with the mouse.
*/
void ColorTriangle::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == LeftButton)
	selMode = Idle;
}

/*!
    \internal
*/
void ColorTriangle::keyPressEvent(QKeyEvent *e)
{   // TODO: fix the 359/360 insonsistencies
    // TODO FIX FOR COLOR CORRECTOR, THE CODE HERE APPLIES FOR CHOOSER ONLY
    switch (e->key()) {
	case Key_Left: {
	    --curHue;
	    if (curHue < 0) curHue += 360;
	    int h,s,v;
	    curColor.getHsv(&h, &s, &v);
	    QColor tmp;
	    tmp.setHsv(curHue, s, v);
	    setColor(tmp);
	}
	    break;
	case Key_Right: {
	    ++curHue;
	    if (curHue > 359) curHue -= 360;
	    int h,s,v;
	    curColor.getHsv(&h, &s, &v);
	    QColor tmp;
	    tmp.setHsv(curHue, s, v);
	    setColor(tmp);
	}
	    break;
	case Key_Up: {
	    int h,s,v;
	    curColor.getHsv(&h, &s, &v);
	    QColor tmp;
	    if (e->state() & ShiftButton) {
		if (s > 5) s -= 5;
		else s = 0;
	    } else {
		if (v > 5) v -= 5;
		else v = 0;
	    }
	    tmp.setHsv(curHue, s, v);
	    setColor(tmp);
	}
	    break;
	case Key_Down: {
	    int h,s,v;
	    curColor.getHsv(&h, &s, &v);
	    QColor tmp;
	    if (e->state() & ShiftButton) {
		if (s < 250) s += 5;
		else s = 255;
	    } else {
		if (v < 250) v += 5;
		else v = 255;
	    }
	    tmp.setHsv(curHue, s, v);
	    setColor(tmp);
	}
	    break;
    };
}

/*!
    \internal

    Regenerates the background image and sends an update.
*/
void ColorTriangle::resizeEvent(QResizeEvent *)
{
    outerRadius = (contentsRect().width() - 1) / 2;
    if ((contentsRect().height() - 1) / 2 < outerRadius)
	outerRadius = (contentsRect().height() - 1) / 2;

    penWidth = (int) floor(outerRadius / 50.0);
    ellipseSize = (int) floor(outerRadius / 12.5);

    double cx = (double) contentsRect().center().x();
    double cy = (double) contentsRect().center().y();

    pa = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
    pb = DoublePoint(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
    pc = DoublePoint(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));
    pd = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 10.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 10.0))));

    // Find the current position of the selector
    selectorPos = pointFromColor(curColor);

    mustGenerateBackground = true;
    update();
}

/*! \reimp

First copies a background image of the hue donut and its
    background color onto the frame, then draws the color triangle,
    and finally the selectors.
*/
void ColorTriangle::drawContents(QPainter *p)
{
    if (mustGenerateBackground) {
	genBackground();
	mustGenerateBackground = false;
    }

    // Blit the static generated background with the hue gradient onto
    // the double buffer.
    QImage buf = bg.copy();

    // Draw the trigon
    int h,s,v;
    curColor.getHsv(&h, &s, &v);

    // Find the color with only the hue, and max value and saturation
    QColor hueColor;
    hueColor.setHsv(curHue, 255, 255);

    // Draw the triangle
    if ( themode == ColorTriangleMode )
        drawTrigon(&buf, pa, pb, pc, hueColor);

    // Slow step: convert the image to a pixmap
    QPixmap pix;
    pix.convertFromImage(buf);
    QPainter painter(&pix);

    int ri, gi, bi;
    hueColor.getRgb(&ri, &gi, &bi);
    QColor contrastcol;
    if ((ri * 30) + (gi * 59) + (bi * 11) > 12800)
        painter.setPen(QPen(black, penWidth));
    else
	painter.setPen(QPen(white, penWidth));

    // Only draw ellipse on outer donut if in color selector mode.
    if ( themode == ColorTriangleMode )
        painter.drawEllipse((int) (pd.x - ellipseSize / 2.0),
	    		(int) (pd.y - ellipseSize / 2.0),
	    		ellipseSize, ellipseSize);

    curColor.getRgb(&ri, &gi, &bi);




    // Find a color for painting the selector based on the brightness
    // value of the color.
    // TODO this is prob broken, esp for colorcorrector
    if ((ri * 30) + (gi * 59) + (bi * 11) > 12800)
	painter.setPen(QPen(black, penWidth));
    else
	painter.setPen(QPen(white, penWidth));

    // Colorcorrector text lables and line
    if ( themode == ColorCorrectorMode )
    {   painter.setPen(QPen(black, penWidth ));
        painter.drawLine( contentsRect().center().x(), contentsRect().center().y(), int( selectorPos.x ), int( selectorPos.y ) );
        painter.drawText( int( 1+ selectorPos.x + ellipseSize ), int( 1+ selectorPos.y + ellipseSize ), "M" ); // shadow for master text
        painter.setPen(QPen(white, penWidth ));
        painter.drawText( int( selectorPos.x + ellipseSize ), int( selectorPos.y + ellipseSize ), "M" ); // master text
    }
   
    if ( themode == ColorTriangleMode )  // Draw the selector ellipse.
    {   painter.drawEllipse((int) (selectorPos.x - ellipseSize / 2.0),
			(int) (selectorPos.y - ellipseSize / 2.0),
			ellipseSize, ellipseSize);
    }
    else if ( themode == ColorCorrectorMode ) // or the corrector ellipse.
    { 
        painter.setBrush( Qt::SolidPattern );
        painter.setPen(QPen(black, penWidth));    
        painter.setBrush( painter.pen().color() );
        painter.drawChord((int) (selectorPos.x - ellipseSize / 2.0),
			(int) (selectorPos.y - ellipseSize / 2.0),
			ellipseSize, ellipseSize, 0,  16*360);
        painter.setPen(QPen(white, penWidth));    
        painter.setBrush( painter.pen().color() );
        painter.drawChord((int) (1+ selectorPos.x - ellipseSize / 2.0),
			(int) (1+ selectorPos.y - ellipseSize / 2.0),
			ellipseSize, ellipseSize, 0,  16*360);
        painter.setBrush( Qt::NoBrush );

    }

    // Blit
    p->drawPixmap(contentsRect().topLeft(), pix);
}

/*! \internal

Draws a trigon (polygon with three corners \a pa, \a pb and \a pc
    and three edges), using \a painter.

    Fills the trigon with a gradient, where the \a pa point has the
    color \a color, \a pb is black and \a bc is white. Bilinear
    gradient.
*/
void ColorTriangle::drawTrigon(QImage *buf, const DoublePoint &pa,
			       const DoublePoint &pb, const DoublePoint &pc,
			       const QColor &color)
{
    // Create three TVertex objects. A TVertex contains a double-point
    // coordinate and a color.
    // pa is the tip of the arrow
    // pb is the black corner
    // pc is the white corner
    TVertex aa(color, pa);
    TVertex bb(black, pb);
    TVertex cc(white, pc);

    // Sort. Make p1 above p2, which is above p3 (using y coordinate).
    // Bubble sorting is fastest here.
    TVertex *p1 = &aa;
    TVertex *p2 = &bb;
    TVertex *p3 = &cc;
    if (p1->point.y > p2->point.y) swap(&p1, &p2);
    if (p1->point.y > p3->point.y) swap(&p1, &p3);
    if (p2->point.y > p3->point.y) swap(&p2, &p3);

    // All the three y deltas are >= 0
    double p1p2ydist = p2->point.y - p1->point.y;
    double p1p3ydist = p3->point.y - p1->point.y;
    double p2p3ydist = p3->point.y - p2->point.y;
    double p1p2xdist = p2->point.x - p1->point.x;
    double p1p3xdist = p3->point.x - p1->point.x;
    double p2p3xdist = p3->point.x - p2->point.x;

    // The first x delta decides wether we have a lefty or a righty
    // trigon.
    bool lefty = p1p2xdist < 0;

    // Left and right colors and X values. The key in this map is the
    // y values. Our goal is to fill these structures with all the
    // information needed to do a single pass top-to-bottom,
    // left-to-right drawing of the trigon.
    QMap<int, DoubleColor> leftColors;
    QMap<int, DoubleColor> rightColors;
    QMap<int, double> leftX;
    QMap<int, double> rightX;

    // Scan longy - find all left and right colors and X-values for
    // the tallest edge (p1-p3).
    DoubleColor source;
    DoubleColor dest;
    double r, g, b;
    double rdelta, gdelta, bdelta;
    double x;
    double xdelta;
    int y1, y2;

    // Initialize with known values
    x = p1->point.x;
    source = p1->color;
    dest = p3->color;
    r = source.r;
    g = source.g;
    b = source.b;
    y1 = (int) floor(p1->point.y);
    y2 = (int) floor(p3->point.y);

    // Find slopes (notice that if the y dists are 0, we don't care
    // about the slopes)
    xdelta = p1p3ydist == 0.0 ? 0.0 : p1p3xdist / p1p3ydist;
    rdelta = p1p3ydist == 0.0 ? 0.0 : (dest.r - r) / p1p3ydist;
    gdelta = p1p3ydist == 0.0 ? 0.0 : (dest.g - g) / p1p3ydist;
    bdelta = p1p3ydist == 0.0 ? 0.0 : (dest.b - b) / p1p3ydist;

    // Calculate gradients using linear approximation
    int y;
    for (y = y1; y < y2; ++y) {
	if (lefty) {
	    rightColors.insert(y, DoubleColor(r, g, b));
	    rightX.insert(y, x);
	} else {
	    leftColors.insert(y, DoubleColor(r, g, b));
	    leftX.insert(y, x);
	}

	r += rdelta;
	g += gdelta;
	b += bdelta;
	x += xdelta;
    }

    // Scan top shorty - find all left and right colors and x-values
    // for the topmost of the two not-tallest short edges.
    x = p1->point.x;
    source = p1->color;
    dest = p2->color;
    r = source.r;
    g = source.g;
    b = source.b;
    y1 = (int) floor(p1->point.y);
    y2 = (int) floor(p2->point.y);

    // Find slopes (notice that if the y dists are 0, we don't care
    // about the slopes)
    xdelta = p1p2ydist == 0.0 ? 0.0 : p1p2xdist / p1p2ydist;
    rdelta = p1p2ydist == 0.0 ? 0.0 : (dest.r - r) / p1p2ydist;
    gdelta = p1p2ydist == 0.0 ? 0.0 : (dest.g - g) / p1p2ydist;
    bdelta = p1p2ydist == 0.0 ? 0.0 : (dest.b - b) / p1p2ydist;

    // Calculate gradients using linear approximation
    for (y = y1; y < y2; ++y) {
	if (lefty) {
	    leftColors.insert(y, DoubleColor(r, g, b));
	    leftX.insert(y, x);
	} else {
	    rightColors.insert(y, DoubleColor(r, g, b));
	    rightX.insert(y, x);
	}

	r += rdelta;
	g += gdelta;
	b += bdelta;
	x += xdelta;
    }

    // Scan bottom shorty - find all left and right colors and
    // x-values for the bottommost of the two not-tallest short edges.
    x = p2->point.x;
    source = p2->color;
    dest = p3->color;
    r = source.r;
    g = source.g;
    b = source.b;
    y1 = (int) floor(p2->point.y);
    y2 = (int) floor(p3->point.y);

    // Find slopes (notice that if the y dists are 0, we don't care
    // about the slopes)
    xdelta = p2p3ydist == 0.0 ? 0.0 : p2p3xdist / p2p3ydist;
    rdelta = p2p3ydist == 0.0 ? 0.0 : (dest.r - r) / p2p3ydist;
    gdelta = p2p3ydist == 0.0 ? 0.0 : (dest.g - g) / p2p3ydist;
    bdelta = p2p3ydist == 0.0 ? 0.0 : (dest.b - b) / p2p3ydist;

    // Calculate gradients using linear approximation
    for (y = y1; y < y2; ++y) {
	if (lefty) {
	    leftColors.insert(y, DoubleColor(r, g, b));
	    leftX.insert(y, x);
	} else {
	    rightColors.insert(y, DoubleColor(r, g, b));
	    rightX.insert(y, x);
	}

	r += rdelta;
	g += gdelta;
	b += bdelta;
	x += xdelta;
    }

    // Inner loop. For each y in the left map of x-values, draw one
    // line from left to right.
    QMap<int, double>::const_iterator it = leftX.begin();
    while (it != leftX.end()) {
	int y = it.key();
	double lx = it.data();
	double rx = rightX[y];

	int lxi = (int) floor(lx);
	int rxi = (int) floor(rx);
	DoubleColor rc = rightColors[y];
	DoubleColor lc = leftColors[y];

        // if the xdist is 0, don't draw anything.
	double xdist = rx - lx;
	if (xdist != 0.0) {
            double r = lc.r;
            double g = lc.g;
            double b = lc.b;
            double rdelta = (rc.r - r) / xdist;
            double gdelta = (rc.g - g) / xdist;
            double bdelta = (rc.b - b) / xdist;

            QRgb *scanline = reinterpret_cast<QRgb *>(buf->scanLine(y));
            scanline += lxi;

            // Inner loop 2. Draws the line from left to right.
            for (int i = lxi; i < rxi; ++i) {
                *scanline++ = qRgb((int) r, (int) g, (int) b);
                r += rdelta;
                g += gdelta;
                b += bdelta;
            }
        }

	++it;
    }
}

/*! \internal

Sets the color of the triangle to \a col.
 */
void ColorTriangle::setColor(const QColor &col)
{
    //qDebug( " setColor \n" );

    setColorSilently( col );

    emit colorChangedInternal(curColor);

    // The only place the external signal is emitted.
    emit colorChangedExternal(curColor);
}

void ColorTriangle::setColorSilentlyConstantHue( const QColor &col )
{   // used to ensure triangle does not change hue...

    if (col == curColor)
	return;

   

    int h, s, v;
    curColor.getHsv(&h, &s, &v);

    int hc, sc, vc;
    col.getHsv( hc, sc, vc );

    curColor.setHsv( h, sc, vc );
    setColorSilently( curColor );

}

void ColorTriangle::setColorSilently( const QColor &col )
{
    //qDebug( "    ->setColorSilently \n" );
    if (col == curColor)
	return;

    curColor = col;

    int h, s, v;
    curColor.getHsv(&h, &s, &v);

    // Never use an invalid hue to display colors
    if (h != -1 ) 
    	curHue = h;

    a = (((360 - curHue) * TWOPI) / 360.0);
    a += DPI / 2.0;
    if (a > TWOPI) a -= TWOPI;

    b = a + TWOPI/3;
    c = b + TWOPI/3;

    if (b > TWOPI) b -= TWOPI;
    if (c > TWOPI) c -= TWOPI;

    double cx = (double) contentsRect().center().x();
    double cy = (double) contentsRect().center().y();

    pa = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
    pb = DoublePoint(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
    pc = DoublePoint(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
		     cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));
    pd = DoublePoint(cx + (cos(a) * (outerRadius - (outerRadius / 10.0))),
		     cy - (sin(a) * (outerRadius - (outerRadius / 10.0))));

    selectorPos = pointFromColor(curColor);
    update();
}

/*! \internal

Returns the current color of the triangle.
 */
QColor ColorTriangle::color() const
{
    return curColor;
}

/*!
    \internal

  Returns the distance from \a pos to the center of \a rect.
*/
double ColorTriangle::radiusAt(const DoublePoint &pos, const QRect &rect) const
{
    double mousexdist = pos.x - (double) rect.center().x();
    double mouseydist = pos.y - (double) rect.center().y();
    return sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
}

/*!
    \internal

  With origin set to the center of \a rect, this function returns
    the angle in radians between the line that starts at (0,0) and
    ends at (1,0) and the line that stars at (0,0) and ends at \a pos.
*/
double ColorTriangle::angleAt(const DoublePoint &pos, const QRect &rect) const
{
    double mousexdist = pos.x - (double) rect.center().x();
    double mouseydist = pos.y - (double) rect.center().y();
    double mouserad = sqrt(mousexdist * mousexdist + mouseydist * mouseydist);
    if (mouserad == 0.0)
        return 0.0;

    double angle = acos(mousexdist / mouserad);
    if (mouseydist >= 0)
	angle = TWOPI - angle;

    return angle;
}

/*! \internal

Returns a * a.
 */
inline double qsqr(double a)
{
    return a * a;
}

/*! \internal

Returns the length of the vector (x,y).
 */
inline double vlen(double x, double y)
{
    return sqrt(qsqr(x) + qsqr(y));
}

/*! \internal

Returns the vector product of (x1,y1) and (x2,y2).
 */
inline double vprod(double x1, double y1, double x2, double y2)
{
    return x1 * x2 + y1 * y2;
}

/*! \internal

Returns true if the point cos(p),sin(p) is on the arc between
    cos(a1),sin(a1) and cos(a2),sin(a2); otherwise returns false.
*/
bool angleBetweenAngles(double p, double a1, double a2)
{
    if (a1 > a2) {
	a2 += TWOPI;
	if (p < DPI) p += TWOPI;
    }

    return p >= a1 && p < a2;
}

/*! \internal

    A line from a to b is one of several lines in an equilateral
    polygon, and they are drawn counter clockwise. This line therefore
    has one side facing in and one facing out of the polygon. This
    function determines wether (x,y) is on the inside or outside of
    the given line, defined by the "from" coordinate (ax,ay) and the
    "to" coordinate (bx,by).

    The point (px,py) is the intersection between the a-b line and the
    perpendicular projection of (x,y) onto that line.

    Returns true if (x,y) is above the line; otherwise returns false.

    If ax and bx are equal and ay and by are equal (line is a point),
    this function will return true if (x,y) is equal to this point.
*/
static bool pointAbovePoint(double x, double y, double px, double py,
			    double ax, double ay, double bx, double by)
{
    bool result = false;

    if (floor(ax) > floor(bx)) {
	if (floor(ay) < floor(by)) {
	    // line is draw upright-to-downleft
	    if (floor(x) < floor(px) || floor(y) < floor(py))
		result = true;
	} else if (floor(ay) > floor(by)) {
            // line is draw downright-to-upleft
	    if (floor(x) > floor(px) || floor(y) < floor(py))
		result = true;
	} else {
	    // line is flat horizontal
	    if (y < ay) result = true;
	}
    } else if (floor(ax) < floor(bx)) {
	if (floor(ay) < floor(by)) {
	    // line is draw upleft-to-downright
	    if (floor(x) < floor(px) || floor(y) > floor(py))
		result = true;
	} else if (floor(ay) > floor(by)) {
	    // line is draw downleft-to-upright
	    if (floor(x) > floor(px) || floor(y) > floor(py))
		result = true;
	} else {
	    // line is flat horizontal
	    if (y > ay)
                result = true;
	}
    } else {
	// line is vertical
	if (floor(ay) < floor(by)) {
	    if (x < ax) result = true;
	} else if (floor(ay) > floor(by)) {
	    if (x > ax) result = true;
	} else {
	    if (!(x == ax && y == ay))
		result = true;
	}
    }

    return result;
}

/*! \internal

if (ax,ay) to (bx,by) describes a line, and (x,y) is a point on
    that line, returns -1 if (x,y) is outside the (ax,ay) bounds, 1 if
    it is outside the (bx,by) bounds and 0 if (x,y) is within (ax,ay)
    and (bx,by).
*/
static int pointInLine(double x, double y, double ax, double ay,
		       double bx, double by)
{
    if (ax > bx) {
	if (ay < by) {
	    // line is draw upright-to-downleft

	    // if (x,y) is in on or above the upper right point,
	    // return -1.
	    if (y <= ay && x >= ax)
		return -1;

	    // if (x,y) is in on or below the lower left point,
	    // return 1.
	    if (y >= by && x <= bx)
		return 1;
	} else {
	    // line is draw downright-to-upleft

	    // If the line is flat, only use the x coordinate.
	    if (floor(ay) == floor(by)) {
		// if (x is to the right of the rightmost point,
		// return -1. otherwise if x is to the left of the
		// leftmost point, return 1.
		if (x >= ax)
		    return -1;
		else if (x <= bx)
		    return 1;
	    } else {
		// if (x,y) is on or below the lower right point,
		// return -1.
		if (y >= ay && x >= ax)
		    return -1;

		// if (x,y) is on or above the upper left point,
		// return 1.
		if (y <= by && x <= bx)
		    return 1;
	    }
	}
    } else {
	if (ay < by) {
	    // line is draw upleft-to-downright

            // If (x,y) is on or above the upper left point, return
	    // -1.
	    if (y <= ay && x <= ax)
		return -1;

	    // If (x,y) is on or below the lower right point, return
	    // 1.
	    if (y >= by && x >= bx)
		return 1;
	} else {
	    // line is draw downleft-to-upright

	    // If the line is flat, only use the x coordinate.
	    if (floor(ay) == floor(by)) {
		if (x <= ax)
		    return -1;
		else if (x >= bx)
		    return 1;
	    } else {
		// If (x,y) is on or below the lower left point, return
		// -1.
		if (y >= ay && x <= ax)
		    return -1;

		// If (x,y) is on or above the upper right point, return
		// 1.
		if (y <= by && x >= bx)
		    return 1;
	    }
	}
    }

    // No tests proved that (x,y) was outside [(ax,ay),(bx,by)], so we
    // assume it's inside the line's bounds.
    return 0;
}

/*! \internal

    \a a, \a b and \a c are corner points of an equilateral triangle.
    (\a x,\a y) is an arbitrary point inside or outside this triangle.

    If (x,y) is inside the triangle, this function returns the double
    point (x,y).

    Otherwise, the intersection of the perpendicular projection of
    (x,y) onto the closest triangle edge is returned, unless this
    intersection is outside the triangle's bounds, in which case the
    corner closest to the intersection is returned instead.

    Yes, it's trigonometry.
*/
DoublePoint ColorTriangle::movePointToTriangle(double x, double y, const TVertex &a,
					       const TVertex &b, const TVertex &c) const
{
    // Let v1A be the vector from (x,y) to a.
    // Let v2A be the vector from a to b.
    // Find the angle alphaA between v1A and v2A.
    double v1xA = x - a.point.x;
    double v1yA = y - a.point.y;
    double v2xA = b.point.x - a.point.x;
    double v2yA = b.point.y - a.point.y;
    double vpA = vprod(v1xA, v1yA, v2xA, v2yA);
    double cosA = vpA / (vlen(v1xA, v1yA) * vlen(v2xA, v2yA));
    double alphaA = acos(cosA);

    // Let v1B be the vector from x to b.
    // Let v2B be the vector from b to c.
    double v1xB = x - b.point.x;
    double v1yB = y - b.point.y;
    double v2xB = c.point.x - b.point.x;
    double v2yB = c.point.y - b.point.y;
    double vpB = vprod(v1xB, v1yB, v2xB, v2yB);
    double cosB = vpB / (vlen(v1xB, v1yB) * vlen(v2xB, v2yB));
    double alphaB = acos(cosB);

    // Let v1C be the vector from x to c.
    // Let v2C be the vector from c back to a.
    double v1xC = x - c.point.x;
    double v1yC = y - c.point.y;
    double v2xC = a.point.x - c.point.x;
    double v2yC = a.point.y - c.point.y;
    double vpC = vprod(v1xC, v1yC, v2xC, v2yC);
    double cosC = vpC / (vlen(v1xC, v1yC) * vlen(v2xC, v2yC));
    double alphaC = acos(cosC);

    // Find the radian angles between the (1,0) vector and the points
    // A, B, C and (x,y). Use this information to determine which of
    // the edges we should project (x,y) onto.
    double angleA = angleAt(a.point, contentsRect());
    double angleB = angleAt(b.point, contentsRect());
    double angleC = angleAt(c.point, contentsRect());
    double angleP = angleAt(DoublePoint(x, y), contentsRect());

    // If (x,y) is in the a-b area, project onto the a-b vector.
    if (angleBetweenAngles(angleP, angleA, angleB)) {
	// Find the distance from (x,y) to a. Then use the slope of
	// the a-b vector with this distance and the angle between a-b
	// and a-(x,y) to determine the point of intersection of the
	// perpendicular projection from (x,y) onto a-b.
	double pdist = sqrt(qsqr(x - a.point.x) + qsqr(y - a.point.y));

        // the length of all edges is always > 0
	double p0x = a.point.x + ((b.point.x - a.point.x) / vlen(v2xB, v2yB)) * cos(alphaA) * pdist;
	double p0y = a.point.y + ((b.point.y - a.point.y) / vlen(v2xB, v2yB)) * cos(alphaA) * pdist;

	// If (x,y) is above the a-b line, which basically means it's
	// outside the triangle, then return its projection onto a-b.
	if (pointAbovePoint(x, y, p0x, p0y, a.point.x, a.point.y, b.point.x, b.point.y)) {
	    // If the projection is "outside" a, return a. If it is
	    // outside b, return b. Otherwise return the projection.
	    int n = pointInLine(p0x, p0y, a.point.x, a.point.y, b.point.x, b.point.y);
	    if (n < 0)
		return a.point;
	    else if (n > 0)
		return b.point;

	    return DoublePoint(p0x, p0y);
	}
    } else if (angleBetweenAngles(angleP, angleB, angleC)) {
	// If (x,y) is in the b-c area, project onto the b-c vector.
	double pdist = sqrt(qsqr(x - b.point.x) + qsqr(y - b.point.y));

        // the length of all edges is always > 0
        double p0x = b.point.x + ((c.point.x - b.point.x) / vlen(v2xC, v2yC)) * cos(alphaB) * pdist;
	double p0y = b.point.y + ((c.point.y - b.point.y) / vlen(v2xC, v2yC)) * cos(alphaB) * pdist;

	if (pointAbovePoint(x, y, p0x, p0y, b.point.x, b.point.y, c.point.x, c.point.y)) {
	    int n = pointInLine(p0x, p0y, b.point.x, b.point.y, c.point.x, c.point.y);
	    if (n < 0)
		return b.point;
	    else if (n > 0)
		return c.point;
	    return DoublePoint(p0x, p0y);
	}
    } else if (angleBetweenAngles(angleP, angleC, angleA)) {
	// If (x,y) is in the c-a area, project onto the c-a vector.
	double pdist = sqrt(qsqr(x - c.point.x) + qsqr(y - c.point.y));

        // the length of all edges is always > 0
        double p0x = c.point.x + ((a.point.x - c.point.x) / vlen(v2xA, v2yA)) * cos(alphaC) * pdist;
	double p0y = c.point.y + ((a.point.y - c.point.y) / vlen(v2xA, v2yA)) * cos(alphaC) * pdist;

	if (pointAbovePoint(x, y, p0x, p0y, c.point.x, c.point.y, a.point.x, a.point.y)) {
	    int n = pointInLine(p0x, p0y, c.point.x, c.point.y, a.point.x, a.point.y);
	    if (n < 0)
		return c.point;
	    else if (n > 0)
		return a.point;
	    return DoublePoint(p0x, p0y);
	}
    }

    // (x,y) is inside the triangle (inside a-b, b-c and a-c).
    return DoublePoint(x, y);
}

/*! \internal

    Given the color \a col, this function determines the point in the
    equilateral triangle defined with (pa, pb, pc) that displays this
    color. The function assumes the color at pa has a hue equal to the
    hue of \a col, and that pb is black and pc is white.

    In this certain type of triangle, we observe that saturation grows
    from the black-color edge towards the black-white edge. The value
    grows from the black corner towards the white-color edge. Using
    the intersection of the saturation and value points on the three
    edges, we are able to determine the point with the same saturation
    and value as \a col.
*/
DoublePoint ColorTriangle::pointFromColor(const QColor &col) const
{
    // Simplifications for the corner cases.
    if (col == black)
	return pb;
    else if (col == white)
	return pc;

    // Find the x and y slopes
    double ab_deltax = pb.x - pa.x;
    double ab_deltay = pb.y - pa.y;
    double bc_deltax = pc.x - pb.x;
    double bc_deltay = pc.y - pb.y;
    double ac_deltax = pc.x - pa.x;
    double ac_deltay = pc.y - pa.y;

    // Extract the h,s,v values of col.
    int hue,sat,val;
    col.getHsv(&hue, &sat, &val);

    // Find the line that passes through the triangle where the value
    // is equal to our color's value.
    double p1 = pa.x + (ab_deltax * (double) (255 - val)) / 255.0;
    double q1 = pa.y + (ab_deltay * (double) (255 - val)) / 255.0;
    double p2 = pb.x + (bc_deltax * (double) val) / 255.0;
    double q2 = pb.y + (bc_deltay * (double) val) / 255.0;

    // Find the line that passes through the triangle where the
    // saturation is equal to our color's value.
    double p3 = pa.x + (ac_deltax * (double) (255 - sat)) / 255.0;
    double q3 = pa.y + (ac_deltay * (double) (255 - sat)) / 255.0;
    double p4 = pb.x;
    double q4 = pb.y;

    // Find the intersection between these lines.
    double a = (q2 - q1) / (p2 - p1);
    double c = (q4 - q3) / (p4 - p3);
    double b = q1 - a * p1;
    double d = q3 - c * p3;

    double x = (d - b) / (a - c);
    double y = a * x + b;

    return DoublePoint(x, y);
}

/*! \internal

    Determines the color in the color triangle at the point \a p. Uses
    linear interpolation to find the colors to the left and right of
    \a p, then uses the same technique to find the color at \a p using
    these two colors.
*/
QColor ColorTriangle::colorFromPoint(const DoublePoint &p) const
{
    // Find the outer radius of the hue gradient.
    int outerRadius = (contentsRect().width() - 1) / 2;
    if ((contentsRect().height() - 1) / 2 < outerRadius)
	outerRadius = (contentsRect().height() - 1) / 2;

    // Find the center coordinates
    double cx = (double) contentsRect().center().x();
    double cy = (double) contentsRect().center().y();

    // Find the a, b and c from their angles, the center of the rect
    // and the radius of the hue gradient donut.
    DoublePoint pa(cx + (cos(a) * (outerRadius - (outerRadius / 5.0))),
		   cy - (sin(a) * (outerRadius - (outerRadius / 5.0))));
    DoublePoint pb(cx + (cos(b) * (outerRadius - (outerRadius / 5.0))),
		   cy - (sin(b) * (outerRadius - (outerRadius / 5.0))));
    DoublePoint pc(cx + (cos(c) * (outerRadius - (outerRadius / 5.0))),
		   cy - (sin(c) * (outerRadius - (outerRadius / 5.0))));

    // Find the hue value from the angle of the 'a' point.
    double angle = a - DPI/2.0;
    if (angle < 0) angle += TWOPI;
    double hue = (360.0 * angle) / TWOPI;

    // Create the color of the 'a' corner point. We know that b is
    // black and c is white.
    QColor color;
    color.setHsv(360 - (int) floor(hue), 255, 255);

    // See also drawTrigon(), which basically does exactly the same to
    // determine all colors in the trigon.
    TVertex aa(color, pa);
    TVertex bb(black, pb);
    TVertex cc(white, pc);

    // Make sure p1 is above p2, which is above p3.
    TVertex *p1 = &aa;
    TVertex *p2 = &bb;
    TVertex *p3 = &cc;
    if (p1->point.y > p2->point.y) swap(&p1, &p2);
    if (p1->point.y > p3->point.y) swap(&p1, &p3);
    if (p2->point.y > p3->point.y) swap(&p2, &p3);

    // Find the slopes of all edges in the trigon. All the three y
    // deltas here are positive because of the above sorting.
    double p1p2ydist = p2->point.y - p1->point.y;
    double p1p3ydist = p3->point.y - p1->point.y;
    double p2p3ydist = p3->point.y - p2->point.y;
    double p1p2xdist = p2->point.x - p1->point.x;
    double p1p3xdist = p3->point.x - p1->point.x;
    double p2p3xdist = p3->point.x - p2->point.x;

    // The first x delta decides wether we have a lefty or a righty
    // trigon. A lefty trigon has its tallest edge on the right hand
    // side of the trigon. The righty trigon has it on its left side.
    // This property determines wether the left or the right set of x
    // coordinates will be continuous.
    bool lefty = p1p2xdist < 0;

    // Find whether the selector's y is in the first or second shorty,
    // counting from the top and downwards. This is used to find the
    // color at the selector point.
    bool firstshorty = (p.y >= p1->point.y && p.y < p2->point.y);

    // From the y value of the selector's position, find the left and
    // right x values.
    double leftx;
    double rightx;
    if (lefty) {
        if (firstshorty) {
            leftx = p1->point.x;
            if (floor(p1p2ydist) != 0.0) {
                leftx += (p1p2xdist * (p.y - p1->point.y)) / p1p2ydist;
            } else {
                leftx = QMIN(p1->point.x, p2->point.x);
            }
        } else {
            leftx = p2->point.x;
            if (floor(p2p3ydist) != 0.0) {
                leftx += (p2p3xdist * (p.y - p2->point.y)) / p2p3ydist;
            } else {
                leftx = QMIN(p2->point.x, p3->point.x);
            }
        }

        rightx = p1->point.x;
        rightx += (p1p3xdist * (p.y - p1->point.y)) / p1p3ydist;
    } else {
        leftx = p1->point.x;
        leftx += (p1p3xdist * (p.y - p1->point.y)) / p1p3ydist;

        if (firstshorty) {
            rightx = p1->point.x;
            if (floor(p1p2ydist) != 0.0) {
                rightx += (p1p2xdist * (p.y - p1->point.y)) / p1p2ydist;
            } else {
                rightx = QMAX(p1->point.x, p2->point.x);
            }
        } else {
            rightx = p2->point.x;
            if (floor(p2p3ydist) != 0.0) {
                rightx += (p2p3xdist * (p.y - p2->point.y)) / p2p3ydist;
            } else {
                rightx = QMAX(p2->point.x, p3->point.x);
            }
        }
    }

    // Find the r,g,b values of the points on the trigon's edges that
    // are to the left and right of the selector.
    double rshort = 0, gshort = 0, bshort = 0;
    double rlong = 0, glong = 0, blong = 0;
    if (firstshorty) {
        if (floor(p1p2ydist) != 0.0) {
            rshort  = p2->color.r * (p.y - p1->point.y) / p1p2ydist;
            gshort  = p2->color.g * (p.y - p1->point.y) / p1p2ydist;
            bshort  = p2->color.b * (p.y - p1->point.y) / p1p2ydist;
            rshort += p1->color.r * (p2->point.y - p.y) / p1p2ydist;
            gshort += p1->color.g * (p2->point.y - p.y) / p1p2ydist;
            bshort += p1->color.b * (p2->point.y - p.y) / p1p2ydist;
        } else {
            if (lefty) {
                if (p1->point.x <= p2->point.x) {
                    rshort  = p1->color.r;
                    gshort  = p1->color.g;
                    bshort  = p1->color.b;
                } else {
                    rshort  = p2->color.r;
                    gshort  = p2->color.g;
                    bshort  = p2->color.b;
                }
            } else {
                if (p1->point.x > p2->point.x) {
                    rshort  = p1->color.r;
                    gshort  = p1->color.g;
                    bshort  = p1->color.b;
                } else {
                    rshort  = p2->color.r;
                    gshort  = p2->color.g;
                    bshort  = p2->color.b;
                }
            }
        }
    } else {
        if (floor(p2p3ydist) != 0.0) {
            rshort  = p3->color.r * (p.y - p2->point.y) / p2p3ydist;
            gshort  = p3->color.g * (p.y - p2->point.y) / p2p3ydist;
            bshort  = p3->color.b * (p.y - p2->point.y) / p2p3ydist;
            rshort += p2->color.r * (p3->point.y - p.y) / p2p3ydist;
            gshort += p2->color.g * (p3->point.y - p.y) / p2p3ydist;
            bshort += p2->color.b * (p3->point.y - p.y) / p2p3ydist;
        } else {
            if (lefty) {
                if (p2->point.x <= p3->point.x) {
                    rshort  = p2->color.r;
                    gshort  = p2->color.g;
                    bshort  = p2->color.b;
                } else {
                    rshort  = p3->color.r;
                    gshort  = p3->color.g;
                    bshort  = p3->color.b;
                }
            } else {
                if (p2->point.x > p3->point.x) {
                    rshort  = p2->color.r;
                    gshort  = p2->color.g;
                    bshort  = p2->color.b;
                } else {
                    rshort  = p3->color.r;
                    gshort  = p3->color.g;
                    bshort  = p3->color.b;
                }
            }
        }
    }

    // p1p3ydist is never 0
    rlong  = p3->color.r * (p.y - p1->point.y) / p1p3ydist;
    glong  = p3->color.g * (p.y - p1->point.y) / p1p3ydist;
    blong  = p3->color.b * (p.y - p1->point.y) / p1p3ydist;
    rlong += p1->color.r * (p3->point.y - p.y) / p1p3ydist;
    glong += p1->color.g * (p3->point.y - p.y) / p1p3ydist;
    blong += p1->color.b * (p3->point.y - p.y) / p1p3ydist;

    // rshort,gshort,bshort is the color on one of the shortys.
    // rlong,glong,blong is the color on the longy. So depending on
    // wether we have a lefty trigon or not, we can determine which
    // colors are on the left and right edge.
    double rl, gl, bl, rr, gr, br;
    if (lefty) {
	rl = rshort; gl = gshort; bl = bshort;
	rr = rlong; gr = glong; br = blong;
    } else {
	rl = rlong; gl = glong; bl = blong;
	rr = rshort; gr = gshort; br = bshort;
    }

    // Find the distance from the left x to the right x (xdist). Then
    // find the distances from the selector to each of these (saxdist
    // and saxdist2). These distances are used to find the color at
    // the selector.
    double xdist = rightx - leftx;
    double saxdist = p.x - leftx;
    double saxdist2 = xdist - saxdist;

    // Now determine the r,g,b values of the selector using a linear
    // approximation.
    double r, g, b;
    if (xdist != 0.0) {
	r = (saxdist2 * rl / xdist) + (saxdist * rr / xdist);
	g = (saxdist2 * gl / xdist) + (saxdist * gr / xdist);
	b = (saxdist2 * bl / xdist) + (saxdist * br / xdist);
    } else {
	// In theory, the left and right color will be equal here. But
	// because of the loss of precision, we get an error on both
	// colors. The best approximation we can get is from adding
	// the two errors, which in theory will eliminate the error
	// but in practise will only minimize it.
	r = (rl + rr) / 2;
        g = (gl + gr) / 2;
	b = (bl + br) / 2;
    }

    // Now floor the color components and fit them into proper
    // boundaries. This again is to compensate for the error caused by
    // loss of precision.
    int ri = (int) floor(r);
    int gi = (int) floor(g);
    int bi = (int) floor(b);
    if (ri < 0) ri = 0;
    else if (ri > 255) ri = 255;
    if (gi < 0) gi = 0;
    else if (gi > 255) gi = 255;
    if (bi < 0) bi = 0;
    else if (bi > 255) bi = 255;

    // Voila, we have the color at the point of the selector.
    return QColor(ri, gi, bi);
}

void ColorTriangle::setCorrectorHueShift( int hueshift )
{
    if ( outerDonutHueShift == hueshift ) // doesnt apply to chooser/triangle
        return;

    if ( hueshift < -180 ) hueshift+=360;
    else if ( hueshift > 180 ) hueshift-=360;
    outerDonutHueShift = hueshift;
  
    mustGenerateBackground = true; // trigger creation of new bg.
    update();
}

void ColorTriangle::setCorrectorSaturation( int sat )
{
    if ( outerDonutSaturation == sat )
        return;

    if ( sat < 0 ) sat = 0;
    else if ( sat > 255 ) sat = 255;
    
    outerDonutSaturation = sat;

    mustGenerateBackground = true; // trigger creation of new bg.
    update();
}

void ColorTriangle::setMode( Mode mode )
 {   
     if ( themode == mode ) return;

     themode = mode; 
 }

 void ColorTriangle::setCorrectorHueOffset( int angle )
 {
     // NB units are 1/10th degrees to avoid rounding issues when using just integer degrees.

    if ( angle > 3600 )
        angle-=3600;
    else if ( angle < 0 )
        angle+= 3600;

    DoublePoint depos = selectorPos;
    double huestr = correctorHueStrength();
    double radius = radiusAt(depos, bg.rect() );

    /*QString conv, conv2; 
    conv.setNum( huestr );
    conv2.setNum( radius );
    debug( "in hoffset, str = " + conv + " rad = " + conv2 );
    */

    double cx = (double) contentsRect().center().x();
	double cy = (double) contentsRect().center().y();

    double a = TWOPI * angle/ (360.0 * 10.0 );
	
    DoublePoint mp = DoublePoint(cx + (cos(a) * (radius)),
			 cy - (sin(a) * (radius)));


    moveSelector( mp );
    setCorrectorHueStrength( huestr ); // force hue strength to be same as before angle change.   
 }

 int ColorTriangle::correctorHueOffset()
 {
     // NB units are 1/10th degrees, sent back as integer.
    return int ( 10.0 * 360.0 * angleAt(selectorPos, contentsRect() ) / TWOPI );
 }

 void ColorTriangle::setCorrectorHueStrength( int rad )
 {
    setCorrectorHueStrength( double(rad ));
 }

 void ColorTriangle::setCorrectorHueStrength( double rad )
 {
    if ( rad < 0 || rad > 100 )
         return;
    double radius = radiusAt(selectorPos, contentsRect() );

/*    QString conv;
    conv.setNum( rad );
    debug( "setCorrectorHueStrength rad " + conv );
    conv.setNum( radius );
    debug( "    In setCorrectorHueStrength. Radius is " + conv );
*/
    double sdx = selectorPos.x;
    double sdy = selectorPos.y;
    double dx, dy;

    if ( radius < 1 ) // not a good vector to move along, use the last know good one.
    {    sdx = lastNoncenterHueVectX;
         sdy = lastNoncenterHueVectY;
         radius = radiusAt( DoublePoint( sdx, sdy), contentsRect() ); // get the last know good radius.
    }
    else
    {   lastNoncenterHueVectX = selectorPos.x;
        lastNoncenterHueVectY = selectorPos.y;
    }

    dx = sdx - (double) contentsRect().center().x();
    dy = sdy - (double) contentsRect().center().y();   

    dx/= radius / (outerRadius - outerRadius/5);
    dy/= radius / (outerRadius - outerRadius/5);
    
    double scalef = rad / 100.0;
    DoublePoint newp = DoublePoint( contentsRect().center().x() + dx*scalef, contentsRect().center().y() + dy*scalef );

    moveSelector( newp );
 }

double ColorTriangle::correctorHueStrength()
 {
    double strength = 100.0 *  radiusAt(selectorPos, contentsRect() ) / (outerRadius - outerRadius/5);
    if ( strength > 99.5 ) // (to avoid rounding problems around 99-100 at max strength).
        strength = 100.0;
    return strength;
 }

 void ColorTriangle::moveSelector ( DoublePoint pos )
 {
     /*QString conv, conv2;
     conv.setNum( pos.x );
     conv2.setNum( pos.y ); 
     debug( "moveSelector to " + conv + " " + conv2 );
     */
     selectorPos = pos;

    bool newColor = false;

    QColor col;
    if ( themode == ColorTriangleMode )
    {    col = colorFromPoint(selectorPos);
         if ( col != curColor) {
	        // Ensure that hue does not change when selecting
	        // saturation and value.
	        int h,s,v;
	        col.getHsv(&h, &s, &v);
	        curColor.setHsv(curHue, s, v);
	        newColor = true;
         }
	}
    else if ( themode == ColorCorrectorMode )
    {    col = colorFromDonut( selectorPos );
         if ( col != curColor )
         {    curColor = col;
             newColor = true;
         }
    }
	
   
    if (newColor)
	emit colorChangedInternal(curColor);

    update();
}

