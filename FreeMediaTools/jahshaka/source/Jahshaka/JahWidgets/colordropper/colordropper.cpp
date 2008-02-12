#include "colordropper.h"

#include <qcursor.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qevent.h>
#include <qimage.h>
#include <qpen.h>
#include <qfontmetrics.h>

ColorDropper::ColorDropper()
    : QWidget( NULL, "ColorDropper", Qt::WStyle_Customize  | Qt::WStyle_StaysOnTop | Qt::WStyle_NoBorder )
{
    // position so that it is always in the top left of the application
    move( qApp->mainWidget()->mapToGlobal( QPoint( 0, 0 ) ) );

    // install outself as a global event filter
    qApp->installEventFilter( this );

    m_globalMouseTracking = QApplication::hasGlobalMouseTracking();
    QApplication::setGlobalMouseTracking( true );

    QApplication::setOverrideCursor( QCursor( colorDropperImage(), 0, 15 ) );
}

ColorDropper::~ColorDropper()
{
    QApplication::setGlobalMouseTracking( m_globalMouseTracking );
    QApplication::restoreOverrideCursor();
}
    
QPoint getAppPos( QMouseEvent* e )
{
    return e->globalPos() - qApp->mainWidget()->mapToGlobal( QPoint( 0, 0 ) );
}

bool ColorDropper::eventFilter( QObject* obj, QEvent* e )
{
    if ( e->type() == QEvent::MouseMove )
    {
        QMouseEvent* eAsMouseEvent = static_cast< QMouseEvent* >( e );
        m_pos = getAppPos( eAsMouseEvent );
        update();
        return true;
    }
    else if ( e->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent* eAsMouseEvent = static_cast< QMouseEvent* >( e );
        m_pos = getAppPos( eAsMouseEvent );
        emit colorPicked( QPixmap::grabWindow( 
                              qApp->mainWidget()->winId(), 
                              m_pos.x(), 
                              m_pos.y(), 
                              1, 1 
                              ).convertToImage().pixel( 0, 0 ) );
        deleteLater();
        return true;
    }
    else if ( obj == this )
    {
        return false;
    }

    return true;
}

QPixmap ColorDropper::colorDropperImage()
{
    /* XPM */
    static const char * dropper_small_xpm[] = {
        "16 16 3 1",
        " c #000000",
        "x c #ffffff",
        ". c None",
        "............  ..",
        "...........    .",
        "..........  xx  ",
        "........   x    ",
        ".........       ",
        "........       .",
        ".......  x    ..",
        "......  x  . ...",
        ".....  x  ......",
        "....  x  .......",
        "...  x  ........",
        "..  x  .........",
        ".  x  ..........",
        ".    ...........",
        "    ............",
        "  .............."
    };

    return QPixmap( dropper_small_xpm );
}

void ColorDropper::paintEvent( QPaintEvent* )
{
    QPixmap buffer( size() );
    QPainter p( &buffer );
    const int halfwidth = 15;
    const int halfheight = 15;

    // drag a small chunk of the app
    QPixmap section = QPixmap::grabWindow( qApp->mainWidget()->winId(), 
                                           m_pos.x() - halfwidth, 
                                           m_pos.y() - halfheight, 
                                           2*halfwidth + 1, 
                                           2*halfheight + 1 );
    
    // scale and draw
    QImage sectionAsImage = section.convertToImage();
    p.drawImage( 0, 0, sectionAsImage.scale( width(), height() ) );

    // draw a box for the pixel we are interested in
    float scaleW = ( float )width() / ( 2*halfwidth + 1 );
    float scaleH = ( float )height() / ( 2*halfheight + 1 );
    p.setPen( QPen( QColor( 255, 0, 0 ) ) );
    p.drawRect( int( halfwidth * scaleW ), int( halfheight * scaleH ), int( 2*scaleW ), int( 2*scaleH ) );
    QFontMetrics fm = p.fontMetrics();

    // and some text to indicatre the colour choice
    const int PADDING = 3;
    p.drawText( PADDING, height() - PADDING, QColor( sectionAsImage.pixel( halfwidth, halfheight ) ).name() );

    bitBlt( this, QPoint( 0, 0 ), &buffer, QRect( 0, 0, buffer.width(), buffer.height() ), Qt::CopyROP );
}
