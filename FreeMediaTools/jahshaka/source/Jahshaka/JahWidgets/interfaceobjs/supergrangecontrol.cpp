/****************************************************************************
**
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*****************************************************************************/

#include "supergrangecontrol.h"

// local
#include "valueFormatters.h"
#include "jahcalc.h"

// qt
#include <qpainter.h>
#include <qdrawutil.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qsizepolicy.h>

// std
#include <cmath>
#include <memory>


SupergRangeControl::SupergRangeControl( QWidget *parent, const char *name )
    : QWidget( parent, name ), 
      QRangeControl( -500, 500, 1, 10, 0 ),
      track( true ),
      mousePressed( false ),
      shift( false ),
      m_default( None ),
      leftarrowpressed( false ),
      rightarrowpressed( false ),
      m_textWidth( 0 )
{
    setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed ) );
    setMouseTracking( true ); // required unless we only track on focus.
    setFocusPolicy( WheelFocus );
    setBackgroundMode( Qt::NoBackground );

    mousehold = new QTimer( this );

    connect( mousehold, SIGNAL( timeout() ), this, SLOT( mouseHoldHelperSlot() ) );
}

SupergRangeControl::~SupergRangeControl()
{
}

void SupergRangeControl::setFormatter( const IntValueFormatter& f )
{
    m_formatter.reset( f.clone() );
	update( );
}

void SupergRangeControl::setTracking( bool )
{
    // We now do tracking regardless where the focus is. 
    track = true;
}

bool SupergRangeControl::tracking() const
{
    return track;
}

void SupergRangeControl::setRange( int min, int max )
{
    QRangeControl::setRange( min, max );
    update();
}

void SupergRangeControl::setMax( int max )
{
    QRangeControl::setMaxValue( max );
    update();    
}

void SupergRangeControl::setMin( int min )
{
    QRangeControl::setMinValue( min );
    update();    
}

void SupergRangeControl::setValue( int v )
{
    QRangeControl::setValue( v );
    update();
}

void SupergRangeControl::setDefault( Default v )
{
    m_default = v;
}

void SupergRangeControl::valueChange()
{
    repaint( FALSE );
    emit valueChanged(value());
}

/*!
  \reimp
*/

void SupergRangeControl::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() ) {
    case Key_Left:
        subtractLine();
	break;
    case Key_Right:
        addLine();
	break;
    case Key_Up:
        subtractLine();
	break;
    case Key_Down:
        addLine();
	break;
    case Key_PageUp:
	subtractPage();
	break;
    case Key_PageDown:
	addPage();
	break;
    case Key_Home:
	setValue( minValue() );
	break;
    case Key_End:
	setValue( maxValue() );
	break;
    case Key_Shift:
        shift = true;
    break;
    default:
	e->ignore();
	return;
    };
}

void SupergRangeControl::keyReleaseEvent( QKeyEvent *e )
{
    switch ( e->key() ) {
    case Key_Shift:
        shift = false;
        break;
    default:
	e->ignore();
	return;
    };
}

void SupergRangeControl::mousePressEvent( QMouseEvent *e )
{
    emit inputPressEvent();
    emit inputPressEvent( e );

    e->accept();

    if ( e->button() == LeftButton ) 
    {
	mousePressed = TRUE;
        
        if ( e->pos().x() < height() )
        {
            // left control pressed
            subtractLine();
            leftarrowpressed = true;
            mousehold->start( 250, TRUE );
            update();
        }
        else if ( e->pos().x() > ( width() - height() ) ) 
        {
            // right control pressed
            addLine();
            rightarrowpressed = true;
            mousehold->start( 250, TRUE );
            update();
        }        
        else
        { 
            // central region pressed
            pressedAt = e->pos();
        }
    }
    else if ( e->button() == RightButton )
    {
        std::auto_ptr< JahCalc > thecalc( new JahCalc( this, "jahcalc", TRUE ) );
        thecalc->setValue( value() );

        if ( thecalc->exec() == QDialog::Accepted ) 
        {
            setValue( (int)thecalc->returnResult );
        }
    }
}

void SupergRangeControl::mouseHoldHelperSlot()
{
    if ( leftarrowpressed )
        subtractLine();
    else if ( rightarrowpressed )
        addLine();
    else
        return;

    // set up another shot, mouse still down. NB this repeat is faster than initial one.
    mousehold->start( 75, TRUE );
}

void SupergRangeControl::mouseReleaseEvent( QMouseEvent *e )
{
    emit inputReleaseEvent();

    if ( leftarrowpressed || rightarrowpressed )
    {   
        leftarrowpressed = false;
        rightarrowpressed = false;
        mousePressed = false;
        update();
        return;
    }

    if( e->button() == LeftButton && e->pos().x() > width() && e->pos().x() < ( width() - height() ) )
    {
        QPoint movedTo = e->pos();
        setValue( value() + ( movedTo - pressedAt ).x() );
        pressedAt = movedTo;
    }

    mousePressed = false;
    update();
}

void SupergRangeControl::mouseMoveEvent( QMouseEvent *e )
{
    if ( !mousePressed  || leftarrowpressed || rightarrowpressed )
	return;

    if ( track ) 
    {
	QPoint movedTo = e->pos();
        
        if ( !shift )
        {
            setValue( value() + ( movedTo - pressedAt ).x() );
        }
        else
        {   
            if ( movedTo.x() > pressedAt.x() )
                setValue( value() + 1 );
            else if ( movedTo.x() < pressedAt.x() )
                setValue( value() -1 );
        }
	pressedAt = movedTo;
    }
}

void SupergRangeControl::mouseDoubleClickEvent ( QMouseEvent* e )
{   
    if ( e->button() == LeftButton && e->pos().x() > height() && e->pos().x() < ( width() - height() ) )
    { 
        switch( m_default )
        {
        case Min:
            setValue( minValue() );
            break;

        case Max:
            setValue( maxValue() );
            break;

        case Zero:
            setValue( 0 );
            break;

        case None:
        default:
            return;
            break;
        }

        valueChange();
    }
}

void SupergRangeControl::wheelEvent( QWheelEvent *e )
{
    int step = ( e->state() & ControlButton ) ? lineStep() : pageStep();
    if ( !shift )
        setValue( value() + e->delta()*step/120 );
    else if ( e->delta() < 0 )
        setValue( value() - 1 );
    else if ( e->delta() > 0 )
        setValue( value() + 1 );
    e->accept();
}

void SupergRangeControl::paintEvent( QPaintEvent* e )
{
    QWidget::paintEvent( e );

    QPixmap pix( width(), height() ); // double buffer
    QPainter pt( &pix );
    pt.setFont( font() );

    QBrush brush;

    // main bg
    brush = QBrush( colorGroup().background() );
    pt.fillRect( rect(), brush );
  
    // Draw our value text.
    pt.setPen( colorGroup().text()  );

    QString conv;
    conv.setNum( value() );
    
    if ( m_formatter.get() )
    {
        conv = m_formatter->convert( value() );
    }

    // make the bounding rect _not_ depend on the current value
    // as we get shake...
    QRect b( pt.fontMetrics().boundingRect( conv ) );
    m_textWidth = std::max( m_textWidth, b.width() );

    pt.drawText( (width() - m_textWidth)/2, (height() + b.height())/2, conv );

    // Draw the arrow controls
    const int tsize = std::max( b.height() * 2/3, 8 );
    const int inset = tsize * 2 / 3;

    pt.setPen( Qt::NoPen );
    pt.setBrush( colorGroup().text() );
    paintTriangle( pt, QPoint( inset, height()/2 ), tsize, WEST );
    paintTriangle( pt, QPoint( width() - inset, height()/2 ), tsize, EAST );

    pt.end();

    bitBlt( this, 0, 0, 
            &pix, 0, 0, width(), height(), 
            Qt::CopyROP );
}

void SupergRangeControl::paintTriangle( QPainter &painter, const QPoint& tip, int size, ArrowDirection d ) const
{
    QPointArray tri( 3 );

    static const double COS_30 = 0.866;
    static const double SIN_30 = 0.5;

    switch ( d )
    {
    case EAST:
        tri.putPoints( 0, 3, 
                       tip.x(), tip.y(), 
                       tip.x() - (int)(COS_30*size), tip.y() + (int)(SIN_30*size),
                       tip.x() - (int)(COS_30*size), tip.y() - (int)(SIN_30*size) );
        break;

    case WEST:
        tri.putPoints( 0, 3, 
                       tip.x(), tip.y(), 
                       tip.x() + (int)(COS_30*size), tip.y() + (int)(SIN_30*size),
                       tip.x() + (int)(COS_30*size), tip.y() - (int)(SIN_30*size) );
        break;

    default:
        break;
    }

    painter.drawPolygon( tri );
}

QSize SupergRangeControl::sizeHint() const
{
    return minimumSizeHint();
}

QSize SupergRangeControl::minimumSizeHint() const
{
    constPolish();

    // we need to reserve the size of 2* the height 
    // (for arrows), and whatever the longest
    // formatted string is
    QString minStr = QString::number( minValue() );
    QString maxStr = QString::number( maxValue() );

    if ( m_formatter.get() )
    {
        minStr = m_formatter->convert( minValue() );
        maxStr = m_formatter->convert( maxValue() );
    }

    QString longer( minStr.length() > maxStr.length() ? minStr : maxStr );
    QFontMetrics fm( font() );
    QRect bounding = fm.boundingRect( longer );
    
    static const double PADDING = 1.8;
    QSize res( bounding.width() + (bounding.height() * 4), (int)( PADDING * bounding.height() ) );
    
    return res;
}
