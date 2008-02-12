
#include <qevent.h>
#include <qpainter.h>

#include "jahprogressbar.h"

JahProgressBar::JahProgressBar(int totalSteps, QWidget *parent, const char *name)
    : QProgressBar(totalSteps, parent, name )
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(21);
    setFrameShape(Box);
    setFrameShadow(Sunken);
}

JahProgressBar::JahProgressBar( QWidget *parent, const char *name)
    : QProgressBar( parent, name )
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(21);
    setFrameShape(Box);
    setFrameShadow(Sunken);
    setTotalSteps( 100 );
}

JahProgressBar::~JahProgressBar()
{
}

void JahProgressBar::paintEvent( QPaintEvent *e )
{
    QPixmap buf( size() ); // double buffer
    QColor bgcol( QColor ( 40,40,40) );
    buf.fill( bgcol );
    QPainter painter( &buf );

    if ( totalSteps() == 0 )
    {
        return;
    }

    int pwid = int ( width() * progress()/totalSteps() );

    if ( progress() == totalSteps() )
    {
        pwid = width();
    }
       
    // loop down the frame.
    for ( int yy = 0; yy < height(); yy++ )
    {   
        QColor color( blue );
        int h, s, v;
        color.getHsv( h, s, v);

        if ( yy == 0 ) color.setRgb( 68, 106, 188 );
        else if ( yy > 0 && yy <= 2  ) color.setRgb( 156, 178, 236 );
        else if ( yy > 2 && yy <= 5 ) color.setRgb( 68, 94, 156 );
        else if ( yy > 5  && yy <= 7 ) color.setRgb( 76, 106, 164 );
        else if ( yy > 7 && yy <= 9) color.setRgb( 92, 120, 180 );
        else if ( yy > 9 && yy <= 11 ) color.setRgb( 100, 126, 188 );
        else if ( yy > 11 && yy <= 13 ) color.setRgb( 108, 134, 196 );
        else if ( yy > 13 && yy <= 15 ) color.setRgb( 124, 150, 212 );
        else if ( yy > 15 && yy <= 18 ) color.setRgb( 140, 166, 228 );
        else if ( yy > 17 && yy <= 21 ) color.setRgb( 148, 174, 228 );
        else if ( yy == 21 || yy >= 22) color.setRgb( 60, 78, 116 );

        if ( yy == 0 )
            painter.setPen(QPen( QColor( 68, 106, 188 ) ));
        else
            painter.setPen(QPen( QColor( 156, 178, 236 ) ));
        painter.drawLine( 0, yy, 2, yy );
        
        painter.setPen(QPen( color ));
        painter.drawLine( 2, yy, 2+ pwid -2, yy );
      
        painter.setPen(QPen( QColor( 60, 78, 116 ) ));
        painter.drawLine( 2 + pwid -2 , yy, 2 + pwid, yy );
    }
    
    // Add the text. Why doesnt this work?
    //painter.setPen( Qt::white );
    //painter.drawText(0, 0, "hello");

    // finished, so blit screen
    painter.end();
    bitBlt( this, e->rect().x(), e->rect().y(), &buf, e->rect().x(), e->rect().y(), e->rect().width(), e->rect().height() );
}


void JahProgressBar::resizeEvent(QResizeEvent *)
{
    repaint();
}

QSize JahProgressBar::sizeHint() const
{
    return QSize(350, 21);
}

