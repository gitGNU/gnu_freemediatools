
#include <qpainter.h>
#include <qfont.h>
#include <qtimer.h>

#include <stdlib.h>
#include <iostream>

#include "sliderbutton.h"

//for calculator support!
#include "jahcalc.h"

SliderButton::SliderButton( QWidget* parent, const char* name, const char* tag, double min, double max, 
						   double value, bool integer, bool nolimits, bool drawbar, bool scalebar ) : QPushButton( parent, name ),
						   min( min), max (max ), value (value), isIntegerSlider_(integer), noLimits_(nolimits), 
						   drawBar(drawbar), scaleBar(scalebar)
{
	//connect (this, SIGNAL( clicked()), this, SLOT( clickedSlot()));

	dragging = false;
	dragvalue = value;
	drawFlat = false; //this should be true
	centered = true;
	theTag = "";

	// TODO update value text slot
	setMouseTracking( true );
	buildTag(dragvalue);

	//for some reason we lose the text color when we are in flat mode?

	drawFlat = true;

	if (drawFlat)
	{
		setFlat(true);

		QPalette pal = palette();
		pal.setColor(QColorGroup::ButtonText, QColor(0, 0, 0));
		pal.setColor(QColorGroup::Text, QColor(0, 0, 0));
		setPalette(pal);
	}
}

void SliderButton::paintEvent ( QPaintEvent * e)
{
	// Double buffer to avoid flicker.
	// NOTE: a large part of the flicker seems to be caused by the 
	// pallete button color (see Designer). If its not set to our
	// bg color then you see more flicker. I guess to do with clearing
	// the space before widget is drawn? (bitblt doesnt help there).

	QPushButton::paintEvent (e);
	QPainter paint( this );

	///////////////////////////////////////////////////////////
	//draw the button cell if in flat mode
	if (drawFlat)
	{
		//need to do it this way so its based on the current color scheme
		//QColor thebase = colorGroup().mid();
		//thebase = base.light();
		QColor thebase = QColor("grey");
		thebase = thebase.dark();

		paint.setPen( thebase  );
		paint.setBrush( thebase );
		paint.drawRect ( 0 , 0, width(), height() );
	}

	///////////////////////////////////////////////////////////
	// set up button
	QColor col;

	if (!drawFlat)
		col = QColor("grey");
	else
		col.setRgb(50,50,50); //= QColor("darkGrey"); //black

	QColor darkcol = col.dark();
	QColor lightcol = col.light();

	////////////////////////////////////////////////////////////
	// set up values and variables
	int xborder = 4;
	int yborder = 6;
	paint.setPen( darkcol  );
	paint.setBrush( col );

	float wid1;
	
	if (centered)
	wid1 = dragvalue * (width() ) / (max - min); 
	else 
	wid1 = dragvalue * (width() - (2*xborder) ) / (max - min); 
	
	if ( !centered && wid1 < 0.00001 )
		wid1 = 0.0;
	
	////////////////////////////////////////////////////////////
	//draws the slider rectangle box
	if (!drawFlat)
		paint.setBrush( Qt::NoBrush );
	
	paint.drawRect ( 0 + xborder, height() - 5, width() - 2*xborder, height() -2  );
	
	paint.setBrush( col );
	//float wid2 =  (width() - (2*xborder) ) / (max - min);

	////////////////////////////////////////////////////////////
	//draws the triangles

	//if (drawTriangles)
	int offset  = xborder; // triangle size
	int trisize = (height()/2)+(offset*2);

	//lefttri
	QPointArray triangleArrayLeft( 3 );
	triangleArrayLeft.setPoint ( 0, offset,trisize/2 ); 
	triangleArrayLeft.setPoint ( 1, trisize-offset,trisize-offset ); 
	triangleArrayLeft.setPoint ( 2, trisize-offset,offset );

	//righttri
	QPointArray triangleArrayRight( 3 );
	triangleArrayRight.setPoint ( 0, width()-offset,trisize/2 ); 
	triangleArrayRight.setPoint ( 1, width()-(trisize-offset),trisize-offset ); 
	triangleArrayRight.setPoint ( 2, width()-(trisize-offset),offset ); 
          
    paint.drawPolygon (triangleArrayLeft);
    paint.drawPolygon (triangleArrayRight);

	////////////////////////////////////////////////////////////
	col = QColor("cornflowerblue");
	darkcol = col.dark();
	lightcol = col.light();
	paint.setPen( darkcol  );
	paint.setBrush( col );

	////////////////////////////////////////////////////////////
	//draws the filler 
	//scalebar is cool for left-to-right effects


	if (scaleBar)
		paint.drawRect ( 0 + xborder, height() - 5, wid1, height() -1  );
	else
	{
		if (centered)
			paint.drawRect ( (( width()/2) - 3) + wid1, height() - 5, 6, height() -1  );
			//paint.drawRect ( (xborder+ (wid1-3) + width() )/2, height() - 5, 6, height() -1  );
		else
			paint.drawRect   (xborder+ (wid1-3), height() - 5, 6, height() -1  );
	}

	////////////////////////////////////////////////////////////
	// draw vertical line marker
	// we can show this while dragging by toggling drawBar
	if (drawBar)
	{
		paint.setPen( col );
		paint.drawLine( 0 + xborder + wid1, 0 + yborder, 0 + xborder + wid1, height() - yborder );
		paint.setPen( darkcol ); // for a 2nd line, slight shadow
		paint.drawLine( 0 + xborder + wid1 -1, 0 + yborder, 0 + xborder + wid1 -1, height() - yborder );
	} 

}


void SliderButton::clickedSlot()
{
//	if ( !isOn() )
//		fade();
}


void SliderButton::buildTag(double val)
{
	// TODO update value text slot
	QString ras;
	ras.setNum( val );
	
	//we are going to turn the extra tag value off for now
	//QString thetext = theTag + " " + ras + " "+ theType;
	QString thetext = ras + " "+ theType;
	setText( thetext );
}

void SliderButton::setValue ( int val )
{  // NB code duplication from right click event, could tidy up.
	value = val; 
	dragvalue = val;
	buildTag( value );
	repaint();
}

void SliderButton::setStepSensativity(bool val)        
{ 
	sensative = val;

	if (!sensative)
		step = 10;
	else
		step = 1;
}

void SliderButton::enterEvent( QEvent *e )
{	
	// repaint();
	//std::cout << "enter ev\n";
}

void SliderButton::leaveEvent( QEvent *e )
{
	//	repaint();
	//	std::cout << "leave ev\n";
}

void SliderButton::mousePressEvent(QMouseEvent* e)
{
	//we want to trap these events so they dont trigger the popup menu
	e->accept();

	if ( ( e->button() == Qt::RightButton  )  )
	{	
		//launch the calculator mon
		JahCalc *  thecalc = new JahCalc (this,"jahcalc",TRUE);

		if ( thecalc->exec() == QDialog::Accepted ) 
		{
			dragvalue = (int)thecalc->returnResult;
			buildTag( dragvalue );
			emit sliderMoved( dragvalue );
		}

		delete thecalc;
	}
	else
	{
		//set the step properly
		//need to see if this button can toggle as some times we dont want it to
		//switch back
		if ( (e->button() == Qt::ControlButton)   ) 
		{
			step = 1;
		}

		dragging = true;
		dragvalue = value;
		dragpoint = e->pos();
	}

	return;
}


void SliderButton::mouseReleaseEvent(QMouseEvent* e)
{
	e->accept();

	if ( e->button() == RightButton )
	{	
		//release here does nothing
	}
	else
	{
		if ( !sensative  ) 
		{
			step = 10;
		}

		dragging = false;
		value = dragvalue; 
	}

	return;
}

void SliderButton::mouseMoveEvent(QMouseEvent* e)
{
//	std::cout << "In mouse move\n";

    if ( dragging ) {
		// TEST
		
		if ( e->pos().x() < dragpoint.x() && dragvalue > min )
			dragvalue-=step;
		else if ( e->pos().x() > dragpoint.x() && dragvalue < max )
			dragvalue+=step;
		//std::cout << dragvalue << "\n";
	
		dragpoint = e->pos();

		buildTag( dragvalue );
		emit sliderMoved( dragvalue );
	
    }
}

void SliderButton::mouseDoubleClickEvent ( QMouseEvent*e )
{  // NB code duplication from right click event, could tidy up.
	
	if ( e->button() == Qt::LeftButton )
	{	
	    dragvalue = 0;
		buildTag( dragvalue );
		emit sliderMoved( dragvalue );
	}


	//double click to reset slider value
}



/////////////////////////////////////////////////////////
// this is here to try and get around the issues with the flat mode 
// so we can get the style correct
void SliderButton::drawButtonLabel( QPainter *paint )
{
    QStyle::SFlags flags = QStyle::Style_Default;

    if (isEnabled())
	flags |= QStyle::Style_Enabled;

    if (hasFocus())
	flags |= QStyle::Style_HasFocus;

    if (isDown())
	flags |= QStyle::Style_Down;

    if (isOn())
	flags |= QStyle::Style_On;

    if (! isFlat() && ! isDown())
	flags |= QStyle::Style_Raised;

    if (isDefault())
	flags |= QStyle::Style_ButtonDefault;

    style().drawControl(QStyle::CE_PushButtonLabel, paint, this,
			style().subRect(QStyle::SR_PushButtonContents, this),
			colorGroup(), flags); 
}
