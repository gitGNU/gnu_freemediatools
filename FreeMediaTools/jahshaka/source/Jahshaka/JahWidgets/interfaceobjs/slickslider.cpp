
#include <qpainter.h>
#include <qfont.h>
#include <qtimer.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qpalette.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "slickslider.h"

SlickSlider::SlickSlider( QWidget* parent, const char* name, int thevalue )
	: QFrame( parent, name )
{
	mustGenerateBackground = true;
	dragging = false;

	drawsolidblock = false;
	drawdarkstyle = true;
	drawsunken = false; 
	drawsunken2 = true;
	drawfancybg = true;
	drawtriangles = true;

	usecustombarcolor = true;
	customBarColor = QColor( Qt::black ); 

	if ( drawdarkstyle ) 
	{   
		uppercol = QColor( "gray70" );
		lowercol = QColor( "gray65" );
	}
	else
	{
		uppercol = QColor( "gray95" ); 
		lowercol = QColor( "gray90" ); 
	}

	// set palette to (hopefully) hint to Qt not to use stupid colors for wipes before paint event.
	QPalette p = this->palette();
	QColorGroup cg;
	basestore = cg.color( QColorGroup::Light );
	p.setColor(QColorGroup::Base, uppercol ); //  
	p.setColor(QColorGroup::Background, uppercol); 
	p.setColor(QColorGroup::Foreground, uppercol); 
	this->setPalette(p);

	setValue( thevalue );
}

void SlickSlider::genBackground()
{
	bg = QPixmap( size() );
	bg.fill(colorGroup().base() ); 
   
	QPainter paint( &bg );	

	if ( drawfancybg )
	{   
		paint.setBrush( uppercol );
		paint.setPen( Qt::NoPen );
		paint.drawRect ( 1, 1, width() - 2, height()/2 );

		// lower half rect
		paint.setBrush( lowercol  );
		paint.drawRect ( 1, height()/2, width() -2, height()/2  );

		// slanted lines at top corners
		QPen pen;
		pen.setColor( lowercol );
		paint.setBrush( lowercol );
		pen.setWidth ( 3 );
		paint.setPen( pen  );
		paint.drawLine( 1, 1, 2, height()/2 +1  );
		paint.drawLine( 1, 1, 4, height()/2 +1 );
		paint.drawLine( width()-1, 1, width()-2, height()/2 +1 );
		paint.drawLine( width()-1, 1, width()- 5, height()/2 +1 );
	}

	// draw tick marks
	int duration = int( std::abs( maxValue() - minValue() ) );
	double tickgap = 1.0;
	int nticks = 0;

	if ( duration >= 1 )
	{
		tickgap = double( width( ) - 2 ) / duration;
		while ( tickgap < 5.0 )
		{
			tickgap *= 5;
			nticks /= 5;
		}
		nticks = int( ( width( ) / tickgap ) + 0.5 );
	}

	if ( nticks > 0 )
	{   
		paint.setPen( QColor("gray30")  );
		paint.setBrush( Qt::NoBrush  );

		for ( int i = 1; i <= nticks; i++ )
		{
			double w1 = i * tickgap + 0.5;
			paint.drawLine( int(w1), height()-4, int(w1), height() - 1);
			paint.drawLine( int(w1), 1, int(w1), 3);
		}
	}

	if ( drawsunken2 )
	{   
		//paint.drawRect ( 0, 0, width(), height() );
		paint.setPen( QColor( "gray30" ) );
		paint.drawRect ( 1, 1, width()-2, height()-2 ); // rect inside other one
	}
	
	// basic rect or sunken frame
	paint.setPen( QColor( "gray20" ));
	paint.setBrush( Qt::NoBrush  );
	paint.drawRect ( 0, 0, width(), height() );
 
	paint.end();
	mustGenerateBackground = false;
}

void SlickSlider::mousePressEvent( QMouseEvent *e )
{
	emit sliderPressed( );

	double duration = std::abs( maxValue( ) - minValue( ) );
	double position = std::abs( value( ) - minValue( ) );

	if ( position < 0 ) position = 0;
	if ( position >= duration ) position = duration;

	int wid1 = 5;
   
	if ( duration > 0 )
		wid1 = int( ( position / duration ) * ( width( ) - 10 ) + 5 );

	if ( e->x( ) >= wid1 - 10 && e->x( ) <= wid1 + 10 )
		dragging = true;
}

void SlickSlider::mouseReleaseEvent( QMouseEvent * )
{
	dragging = false;
	emit sliderReleased( );
}

void SlickSlider::mouseDoubleClickEvent( QMouseEvent * )
{
}

void SlickSlider::mouseMoveEvent( QMouseEvent *e )
{
	if ( dragging == true )
	{
		double offset = double( e->x( ) ) / double( width( ) );
		double duration = std::abs( maxValue( ) - minValue( ) );
		double position = offset * duration;

		if ( position < 0 ) position = 0;
		if ( position >= duration ) position = duration;

		setValue( int( position ) + minValue( ) );
		emit sliderMoved( value( ) );
	}
}

void SlickSlider::paintEvent ( QPaintEvent * )
{
	if ( mustGenerateBackground )
		genBackground();

	// For now, just let qt clear our widget, we'll paint directly (seems fine for non-fancy bg rect)
	QPainter paint( this );

	// paint the pre-drawn bg onto ourselves.
	paint.drawPixmap( rect(), bg );

	double duration = std::abs( maxValue( ) - minValue( ) );
	double position = std::abs( value( ) - minValue( ) );

	if ( position < 0 ) position = 0;
	if ( position >= duration ) position = duration;

	double wid1 = 5;
   
	if ( duration > 0 )
		wid1 = ( position / duration ) * ( width( ) - 10 ) + 5;

	// new: if its not a thick block, we draw a grey box bg
	// NB since doing backdrop seperately we dont get quite the same effect, our block
	// is drawn over the bg... e.g. over tickmarks. Could tidy I guess, do trickery to fix it.
	if ( !drawsolidblock)
	{
		int bbw = 5;
		paint.setPen ( QColor ( "gray50" ) );
		paint.setBrush( QColor( "gray50" ) );
		paint.drawRect ( int( wid1 - bbw ), 1, bbw*2 +1, height() -2  );
	}

	// This draws a thin or thick slider block.  
	int blockwid = 3; 
	int blockshifthalf = 1; //
	if ( drawsolidblock )
	{	
		blockwid = 5; // keep odd number, so ticks line up 
		blockshifthalf = 2;
	}

	if ( usecustombarcolor )
		paint.setBrush( customBarColor );
	else
		paint.setBrush( QColor( "gray45" )  );

	int yshift = 0;
	if ( !drawsolidblock )
		yshift = 4;
	if ( !drawtriangles )
		yshift = 0;
	paint.setPen( Qt::NoPen );
	paint.drawRect ( int( wid1 - blockshifthalf ), 1 + yshift, blockwid, height()/2 );
	if ( !usecustombarcolor )
		paint.setBrush( QColor( "gray30" )  );
	paint.drawRect ( int( wid1 - blockshifthalf ), height()/2, blockwid, height()/2 - yshift  );
	

	// if not solid block, draw triangular dragger.
	if ( !drawsolidblock )
	{   
		int tril = 5; // triangle size
		int wid = int(wid1);

		if  ( usecustombarcolor ) // to contrast with custom bar color.
		{   
			paint.setPen( customBarColor  );
			paint.setBrush( customBarColor );
		}
		else if ( drawfancybg ) // subtle color change
		{   
			paint.setPen( QColor("gray45")  );
			paint.setBrush( QColor("gray45")  );
		}
		else
		{  
		   	paint.setPen( QColor("gray30")  );
			paint.setBrush( QColor("gray30")  );
		}

		if ( drawtriangles )
		{
			QPointArray tri( 0 );
			tri.putPoints( 0, 4,   wid, tril,  wid-tril, 0,   wid+tril, 0,	wid, tril );
			paint.drawPolygon( tri, false, 0, 4 );
			QPointArray tri2( 0 );
			tri2.putPoints( 0, 4,   wid, height()-tril -1,  wid-tril, height() -1,   wid+tril, height()-1,	wid, height()-tril-1 );
			paint.drawPolygon( tri2, false, 0, 4 );
		}
	}

	paint.end();
}

int SlickSlider::value( )
{
	return m_range.value( );
}

void SlickSlider::setValue( int val )
{   
	if ( val == value() ) return;

	m_range.setValue( val );
	emit valueChanged( value( ) );	

	if ( dragging )
		emit repaint( );
	else
		emit update( );
}

void SlickSlider::setRange ( int minval, int maxval )
{
	if ( minval == minValue() && maxval == maxValue() ) return;
	m_range.setRange( minval, maxval );
	mustGenerateBackground = true;
	emit update();
}

int SlickSlider::minValue()
{
	return m_range.minValue();
}

int SlickSlider::maxValue()
{
	return m_range.maxValue();
}

void SlickSlider::setMinValue ( int val )
{
	if ( val == minValue() ) return;
	int current = m_range.value( );
	m_range.setMinValue( val );
	if ( current != m_range.value( ) )
		emit valueChanged( m_range.value( ) );	
	mustGenerateBackground = true;
	emit update( );
}

void SlickSlider::setMaxValue ( int val )
{
	if ( val == maxValue() ) return; 
	int current = m_range.value( );
	m_range.setMaxValue( val );
	if ( current != m_range.value( ) )
		emit valueChanged( value( ) );	
	mustGenerateBackground = true;
	emit update( );
}

void SlickSlider::resizeEvent( QResizeEvent * )
{
	mustGenerateBackground = true;
	emit update( );
}

