/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "Socket.h"
#include "Wire.h"
#include "wireup-embedded-images.h"

#include <qcanvas.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qfont.h>

Socket::Socket( QCanvas *canvas, SOCKET_TYPE type ) : QCanvasEllipse(canvas), _type(type), _label( 0 )
{
	init(); 	
}

void Socket::init()
{	
    setSize( 30, 30 );
    setBrush( QColor( "gray50" ) );
    setPen( Qt::NoPen );

    // make the glow effect sprite. TODO STATIC!
    QPixmap pm( qembed_findImage( "green-glow" ) );	
	QValueList<QPixmap> m_list;
	m_list.append( pm );
	QCanvasPixmapArray *pma = new QCanvasPixmapArray( m_list ); 
    _glow = new QCanvasSprite ( pma, canvas() );
    _glow->hide();

    
    _default_input_color = QColor( "DarkGreen" );
    _default_output_color = QColor( "DarkGreen" );
}

Socket::~Socket()
{
	// QtCanvas deletes the canvas items for us -- 
	// we only need to worry about extra memory we have used.
}

void Socket::moveBy(double dx, double dy)
{
    QCanvasItem::moveBy( dx, dy );

    QList<Wire>::iterator it =  _wires.begin();
    while ( it != _wires.end() )
    {
        Wire * wire = (*it);

        // Tell the wires to move their ends too. TODO COULD MOVE ALL POINTS IF TRANS/SCALE CANVAS?
        // TODO could avoid calc of bezier intermediate point if the 2 sockets move/scale together?
         if ( _type == INPUT_SOCKET )
             wire->stretchBezierEnd( QPoint(x(), y() ) );//+ width()) );
         else if ( _type == OUTPUT_SOCKET )
            wire->stretchBezierStart( QPoint(x(), y() ) ); //- width()/2) ); 
        
         it++;
    }

    if ( _glow->visible() )
        _glow->moveBy( dx, dy );

    if ( _label )
        _label->moveBy( dx, dy );

    // (no canvas updated needed).
}

int Socket::numWiresConnected()
{ 
    return _wires.count();
}

/*
int Socket::numInputWiresConnected()
{ 
    return _inputwires.count();
}
*/

void  Socket::connectToWire( Wire * wire )
{
    // For now we assume only one wire per socket. 
    // Certainly cant connect the same wire twice.
    if ( numWiresConnected() == 0 && !_wires.contains( wire ) ) 
        _wires.append( wire );
}

void Socket::disconnectFromWire( Wire *wire )
{
    // NOTE: Need to be careful here. Socket at other end of wire must be informed of change.

    if ( _wires.contains( wire ) )
    {  
        if ( type() == OUTPUT_SOCKET )
            wire->setOutSocket( 0 );
        else if ( type() == INPUT_SOCKET )
            wire->setInSocket( 0 );
        
        setEnabled( false );
        //wire->setEnabled( false );
        wire->hide();
        wire->setCanvas( 0 );
        _wires.remove( wire );

        // Dont leave it hanging off other socket (that only happens if we're dragging a live wire).
        if ( wire->outSocket() && wire->outSocket() != this )
            wire->outSocket()->disconnectFromWire( wire );
        if ( wire->inSocket() && wire->inSocket() != this )
            wire->inSocket()->disconnectFromWire( wire );
        
     //??? TODO: delete CRASHES. usual qcanvas issue?   
        //delete wire;
    }
}

void Socket::disconnectAllWires()
{   
    QList<Wire> currentwires = _wires;
    QList<Wire>::iterator it =  currentwires.begin();
    
    while ( it != currentwires.end() )
    {      
        disconnectFromWire( (*it ) );
        ++it;
    }
}

void Socket::drawShape( QPainter & painter )
{
    setBrush( QColor( "gray30" ) );
    QCanvasEllipse::drawShape(painter);

    // Previous code drew a coloured band on a rectangle
    /* painter.setBrush(QColor( Qt::green ));

    int bandsize = 5; 
    if ( width() < 15 )
        bandsize = 2;

    if ( _type == OUTPUT_SOCKET )
        painter.drawRect( x(), y()+ height() - bandsize - 2, width(), bandsize );
    else if ( _type == INPUT_SOCKET )
        painter.drawRect( x(), y() + 2, width(), bandsize );
    */

    // New code draws a coloured blob at our centre
    int smallsize = int( width()/4 );
    int reflsize = 5;
    QColor thecolor;
    type() == INPUT_SOCKET ? thecolor = _default_input_color : thecolor = _default_output_color;
    if ( numWiresConnected() > 0 )
        thecolor = thecolor.light( 200 );
    painter.setBrush( thecolor );
    painter.drawEllipse( x() - smallsize, y() - smallsize, smallsize*2, smallsize*2 );

    // Painter a 'highlight'. Disable if too naff ;)
    /*
    if ( width()> 20 )
    {
        painter.setBrush( painter.brush().color().light() );
        int dfy = type() == INPUT_SOCKET ? -reflsize : +reflsize -1;
        painter.drawEllipse( x() - reflsize, y() + dfy, reflsize, reflsize );
    }
    */
}

void Socket::beginGlow()
{
    int dy = 0; 
    if ( _type == OUTPUT_SOCKET )
        dy = +5;
    else if ( _type == INPUT_SOCKET )
        dy = -5;
    int glowrad = int( _glow->width()/2 );
    //_glow->move( x() - glowrad, y() + height()/2 - glowrad + dy); 
    _glow->move( x() - glowrad, y() - glowrad );
    _glow->show();
    _glow->setZ(z() + 0.75 ); //(sockets are 0.5 below gadgets).
  //???  canvas()->update();
}

void Socket::endGlow()
{
    _glow->hide();
    canvas()->update(); // required.
}

void Socket::setLabelText( QString label )
{ 
    if ( !_label )
    {   printf( "in setLabelText %s \n", label.latin1() );
        QFont lfont( QFont( "Arial", 12 ) );
        lfont.setItalic( true );
        _label = new QCanvasText( canvas() );
        _label->setFont( lfont );
        _label->setColor( "gray65" ); // TODO dont hardcode...
        _label->setZ(z() + 0.75 );
    }

    _label->setText( label );
    _label->move( x() + width() , y() - 3 );
}

void Socket::setZ( double value )
{
    QCanvasEllipse::setZ( value );
    if ( _label )
        _label->setZ( value + 0.75 );

    // Set z for wires.
    QList<Wire>::iterator wireit =  _wires.begin();
    while ( wireit != _wires.end() )
    {
        // Is this right? This makes it behind both sockets, not in front of other items :(
        /*double zin  = (*wireit)->inSocket() ? (*wireit)->inSocket()->z() : value - 0.5;
        double zout = (*wireit)->outSocket() ? (*wireit)->outSocket()->z() : value - 0.5;
        double newz = zin;
        if ( zout < zin )
            newz = zout;
            */

        int newz = value - 0.5; // TODO need to fix? - makes it in front of other socket
        (*wireit)->setZ( newz );
        ++wireit;
    }
}

void Socket::setEnabled( bool yes )
{
    if ( _label ) 
    {   yes ? _label->show() : _label->hide();
    }
    
    // dont hide for now:  yes ? this->show() : this->hide();
    QCanvasItem::setEnabled( yes );
}




