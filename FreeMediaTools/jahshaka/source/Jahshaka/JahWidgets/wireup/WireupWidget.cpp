/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "WireupWidget.h"
#include "Wire.h"
#include "Gadget.h"
#include "Socket.h"

#ifdef V3UI
#include "PropertyBag.h" // from the authoring model
#include "Property.h" // from the authoring model
#include "Operator.h" // from the authoring model
#endif
#include "wireup-embedded-images.h"

#include <qwmatrix.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <math.h>
#include <qdragobject.h>
#include <qcursor.h>
#include <qfileinfo.h>
#include <qpopupmenu.h>
#include <qapplication.h>

#include <boost/shared_ptr.hpp>
#ifdef V3UI
#include <openeffectslib/fx/n_operator.hpp>
#include <openeffectslib/fx/playable.hpp>

namespace oel = olib::openeffectslib;
#endif


// init statics
bool WireupWidget::_orientvertical = true; 

WireupWidget::WireupWidget(QCanvas& canv, QWidget *parent, const char* name, WFlags f)
                        : QCanvasView(&canv, parent, name, f), _zoom( 1 )
{
   initCanvasView();
}
#ifdef V3UI
WireupWidget::WireupWidget(authoringmodel::composite_ptr composite, QCanvas& canv, QWidget *parent, const char* name, WFlags f)
                        : QCanvasView(&canv, parent, name, f), _zoom( 1 ), _compositemodel( composite )
{
   initCanvasView();
   _compositemodel->attachObserver( this );
}
#endif

WireupWidget::~WireupWidget()
{
    //removeAllGadgets();
    QCanvasItemList allitems = canvas()->allItems();
    for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
    {
        (*it)->hide();
        (*it)->setCanvas( 0 );
        delete (*it);
    }
}

void WireupWidget::initCanvasView()
{
    // init the zoom levels. NB these arent linear.
    _zoomlevels[0] = 0.30;
    _zoomlevels[1] = 0.35;
    _zoomlevels[2] = 0.40;
    _zoomlevels[3] = 0.45;
    _zoomlevels[4] = 0.50;
    _zoomlevels[5] = 0.60;
    _zoomlevels[6] = 0.70;
    _zoomlevels[7] = 0.80;
    _zoomlevels[8] = 0.95;
    _zoomlevels[9] = 1.10;
    _zoomlevels[10] = 1.25;
    _zoomlevels[11] = 1.40;


    _front = 1; 
    _movingitem = 0;
    _movingwire = 0;
    _rubberbandline1 = 0;
    _rubberbandline2 = 0;
    _rubberbandline3 = 0;
    _rubberbandline4 = 0;
    _clickedgadget = false;
    _mouseoveritem = 0;
    //_orientation = WireupWidget::Vertical;
    _shiftkeydown = false;
    _controlkeydown = false; 
    _translateviewstartpt = QPoint(0, 0); // in Qt terms this is 'Null'

    setFocusPolicy( QWidget::StrongFocus ); // we need all keyboard and mouse events
    // we need mouse events even if no buttons pressed (e.g. mouseovers).
    viewport()->setMouseTracking( true ); // for mouse event timing OR use enter/leave???  or not, not qwidgets!?
    setVScrollBarMode( QCanvasView::AlwaysOn ); 
    setHScrollBarMode( QCanvasView::AlwaysOn );
    setMargin(0);
	setFrameStyle( NoFrame );
    canvas()->setBackgroundColor( Qt::black ); // (canvas defines the bg color, not the view)
	QPixmap tile ( qembed_findImage( "grid-for-canvas") ) ;
	canvas()->setBackgroundPixmap ( tile );

    // Set viewport to take focus + drops.
    viewport()->setAcceptDrops( true );
    viewport()->setFocusPolicy( QWidget::StrongFocus );

    _overview = 0;
    // Comment out next line to disable overview.
    _overview = new WireUpOverview( canvas() );
    if ( _overview )
    {   _overview->move( contentsX() + 5, contentsY() + visibleHeight() - _overview->height() - 10 );
        _overview->setZ( 100000 );
        _overview->show();
    }
    _overviewismoving = false;
    connect( this, SIGNAL( contentsMoving( int, int )), this, SLOT( contentsMovingSlot(int, int )));

    _mouseoversenabled = true; // NB the timer still runs, just the mouseOverEffects fn that bails out.

    setZoomLevel( 2 );
}

#ifdef V3UI
void WireupWidget::onChangeNotification( authoringmodel::observee_ptr pSubject ) throw()
{
    printf(" ****WireupWidget::onChangeNotification\n");
    
    authoringmodel::composite_ptr composite = boost::dynamic_pointer_cast<authoringmodel::Composite>(pSubject);
    if ( composite )
    {
        printf("            -- its a composite\n");
        // DO SOMETHING...
    }

}
#endif

QSize WireupWidget::sizeHint () const
{
    return QSize( 400, 300 );
}


QSize WireupWidget::minimumSizeHint () const
{
    return QSize( 100, 100 );
}


QSizePolicy WireupWidget::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred, false );//  (no height for width requirement)
}

Gadget* WireupWidget::createGadget()
{
    printf( " creating gadget number %i \n", Gadget::gadget_counter );
    Gadget * gadg = new Gadget( canvas() );
    bringToFront( gadg );

    // TEST ONLY
    gadg->setOriginalImage("beck.bmp");
    gadg->addOutputSocket();
    gadg->addInputSocket();
    gadg->zoomChangedRedraw(); // TEST ONLY. Shouldnt normally need this...

    if ( _overview )
        _overview->regenerate();
    return gadg;
}

void WireupWidget::autoPositionGadget( Gadget *gadg )
{
    int fit = (Gadget::gadget_counter) % 10;
    gadg->moveBy( fit * 80 + Gadget::gadget_counter, fit * 40 +  Gadget::gadget_counter );
#ifdef V3UI
    gadg->updatePropertyBag();
#endif
}

Gadget* WireupWidget::createGadget( QString filename )
{
#ifdef V3UI
    authoringmodel::footage_operator_ptr newop;
    // TEMPORARY HACK: disabled author model 
    if ( 1 )
    {   if ( _compositemodel )
        {   
            /*printf(" _compositemodel->createFootageOperator \n ");
            authoringmodel::mediafile_ptr pmf = authoringmodel::MediaFile::create( fs::path(filename.latin1(), fs::native) ); 
            printf("            after MediaFile::create \n ");
            newop = _compositemodel->createFootageOperator( pmf );
            printf("        newop is %i \n ", newop );
*/

            {   
                printf(" _compositemodel->createFootageOperator \n ");
                try {
                    authoringmodel::mediafile_ptr pmf = authoringmodel::MediaFile::create( fs::path(filename.latin1(), fs::native) ); 
                    newop = _compositemodel->createFootageOperator( pmf );
					if(_player)
						_player->play(_compositemodel->playable());
                }
                catch ( const std::exception &e )
                {
                    printf( "%s \n", e.what() );
                }
                catch (...)
                {   printf( " unknown exception\n" );
                }
            }


        }
    }
    else 
        printf(" DISABLED _compositemodel->createFootageOperator \n ");
#endif

    QImage image( filename );
    if ( image.isNull() )
        printf(" Wireupwidget image for gadget is null \n" );
    
#ifdef V3UI
    Gadget * gadg = new Gadget( canvas(), image, newop );
#else
    Gadget * gadg = new Gadget( canvas(), image );
#endif
    gadg->move( -100.0, -100.0 );
    Socket* sock = gadg->addOutputSocket();
    sock->setLabelText( "Output" );
    bringToFront( gadg );
    if ( _overview )
        _overview->regenerate();

    QFileInfo fi( filename );
    gadg->setTitleText( fi.fileName() );
    return gadg;
}

Gadget* WireupWidget::createGadget( QImage &image )
{
    Gadget * gadg = new Gadget( canvas(), image );
    gadg->move( -100.0, -100.0 );
    Socket* sock = gadg->addOutputSocket();
    sock->setLabelText( "Output" );
    bringToFront( gadg );
    if ( _overview )
        _overview->regenerate();
    return gadg;  
}

Gadget* WireupWidget::createEffectGadget( std::string name, QImage &image )
{
    QString qname = name.c_str();
#ifdef V3UI
    authoringmodel::effect_operator_ptr newop;
    // TEMPORARY HACK: disabled author model 
    QString qname = name.c_str();
    if ( !qname.contains("Waves" ) && !qname.contains("Wipe" ) && !qname.contains("Corrector") && !qname.contains("Compositor")
        && !qname.contains("Output") )
    
    {
        if ( _compositemodel )
        {   printf(" _compositemodel->createEffectOperator \n ");
            try {
                newop = _compositemodel->createEffectOperator( name );
            }
            catch ( const std::exception &e )
            {
                printf( "%s \n", e.what() );
            }
            catch (...)
            {   printf( " unknown exception\n" );
            }
        }
    }
#endif

#ifdef V3UI
    Gadget * gadg = new Gadget( canvas(), image, newop );
#else
    Gadget * gadg = new Gadget( canvas(), image );
#endif
    gadg->setGadgetType( Gadget::EFFECT_GADGET );
    // HACK!
    if ( qname.contains("Compositor" ) )
        gadg->setGadgetType( Gadget::WIDE_GADGET );
    if ( qname.contains("Layer" ) )
    {   gadg->setGadgetType( Gadget::CIRCULAR_GADGET );
        gadg->setGadgetDrawLevel( Gadget::DRAW_NOIMAGE );
    }

    gadg->move( -200.0, -200.0 );

    // TODO FIX ME! We will need to interogate authoringmodel to ask how many connections an operator has.
    // For demo purposes, we do a horrible hack here.
    if ( name.find( "Output" ) )
    {   Socket* osock = gadg->addOutputSocket();
        osock->setLabelText( "Output" );

#ifdef V3UI
        // A fake property to TEST.
        if ( newop )
            gadg->authoringOperator()->propertyBag()->addProperty( "Quality (test)", static_cast<int>( 100 ), true, false, false );
#endif
    }

    Socket* isock = gadg->addInputSocket();
    isock->setLabelText( "Input" );

    if ( !name.find( "Compositor" ) )
    {   isock = gadg->addInputSocket();
        isock->setLabelText( "Input 2" );
        isock = gadg->addInputSocket();
        isock->setLabelText( "Input 3" );
    }

    bringToFront( gadg );
    if ( _overview )
        _overview->regenerate();
    return gadg;  
}

void WireupWidget::removeGadget( Gadget *mygadget )
{
    mygadget->removeAllSockets();
    mygadget->hide();
    mygadget->setCanvas( 0 );
#ifdef V3UI
    if ( _compositemodel )
    {   
        if ( mygadget->authoringOperator() )
        {
            printf("  deleting operator from authoring model\n" );
            _compositemodel->deleteOperator( mygadget->authoringOperator() );
        }
    }
#endif
    //delete mygadget; // TODO fixme! crash due to being in some list elsewhere.
}

void WireupWidget::removeAllGadgets()
{
    QCanvasItemList allitems = canvas()->allItems();
    for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI )
        {
            Gadget *gadge = dynamic_cast<Gadget*>(*it);
            removeGadget( gadge );
        }
    }
}

void WireupWidget::bringToFront( QCanvasItem *item )
{
    _front+=10;  
    if ( item->rtti() == gadgetRTTI ) // gadgets must set Z of their sockets
    {
        Gadget* gadge = (Gadget*)(item);
        gadge->setZ( _front );
    }
    else
        item->setZ( _front );
}

/* OLD
void WireupWidget::setWireupOrientation( WireupOrientation orient )
{
    if ( _orientation != orient )
    {   _orientation = orient;
        
        // bit messy, gadgets have a static bool for this.
        orient == Vertical ? Gadget::setVerticalOrientation( true ) : Gadget::setVerticalOrientation( false ); 
        // must force full redraw, sockets and wires all change.
        canvas()->setAllChanged();
        canvas()->update();
    }
}
*/

void WireupWidget::setVerticalOrientation( bool set )
{
    printf (" WireupWidget::setVerticalOrientation %i \n", set );
    if ( _orientvertical != set )
    {
        _orientvertical = set;
         // bit messy, gadgets and wires also have a static bool for this.
        Gadget::setVerticalOrientation( set );
        Wire::setVerticalOrientation( set );

        // must force full redraw, sockets and wires all change.
        
        QCanvasItemList allitems = canvas()->allItems();
        for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
        {
            if ( (*it)->rtti() == gadgetRTTI )
            {
                Gadget *gadge = dynamic_cast<Gadget*>(*it);
                // flips its x and y ...
                gadge->move( gadge->y(), gadge->x() );
                gadge->zoomChangedRedraw();
            }
        }

        QPoint position = mapFromGlobal( QCursor::pos() );
        // Disabled moving view until overview can deal with it:
        //ensureVisible( position.y(), position.x() ); // is this correct?
        
        //canvas()->setAllChanged();
        if ( _overview )
            _overview->regenerate();
        canvas()->update();
    }
    else
        printf( "tried to set orientation to same as current setting \n " );
}

void WireupWidget::deleteSelectedGadgets()
{
    //QCanvasItemList selectedgadgets = canvas()->allItems();
    for (QCanvasItemList::Iterator it=_selectedgadgets.begin(); it!=_selectedgadgets.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI ) 
        {   Gadget *gadge = dynamic_cast<Gadget*>(*it);
            removeGadget( gadge );
        }
    }
    _selectedgadgets.clear(); // dont call clearSelection(), the gadgets wont be there!
    if ( _overview )
        _overview->regenerate();
    canvas()->update();
}
    
void WireupWidget::setZoomLevel( int level )
{

    if ( level >= _numberofzoomlevels || level < 0  )
        return;
    //printf("Trying to set zoom to level %i = %f \n", level, _zoomlevels[level] );

    double oldzoom = _zoom;
    _zoom = _zoomlevels[level];
    _zoomlevel = level;
    //printf(" just set _zoom and _zoomlevel to %f, %i \n", _zoom, _zoomlevel ); 
  
    Gadget::_gadgetzoom = _zoom;
    Gadget::_gadgetzoomlevel = level;
    //printf ("Gadget::_gadgetzoom is %f \n", Gadget::_gadgetzoom );

    // Need to move gadgets based on zoom level and last click point.
    QWMatrix zmat;
    zmat.reset();
    zmat.scale( _zoom / oldzoom,  _zoom / oldzoom ); // is this correct?

    QCanvasItemList ilist = canvas()->allItems();
    for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
    {
        int iRTTI = (*it)->rtti();

        if ( iRTTI == gadgetRTTI && (*it)->isVisible() )
        {   Gadget* gadg = (Gadget*)(*it);

            QPoint pt( gadg->x(), gadg->y() );
            QPoint newpt = zmat.map( pt );
            gadg->move( newpt.x(), newpt.y() );

            gadg->zoomChangedRedraw();
        }
    }

    // Now recenter based on last click point.
    QPoint pt( _lastclickpt.x(), _lastclickpt.y() );
    QPoint newpt = zmat.map( pt );
    newpt -= pt;
    allGadgetsMoveBy( -newpt.x(), -newpt.y() );
    canvas()->update();
    
}

 // === MOUSE and KEYBOARD FUNCTIONS ======================================

void WireupWidget::contentsMousePressEvent( QMouseEvent* e )
{
    // **TODO: this function is too long! its actually quite simple, but too many if blocks.**

    // Note, due to large bounding box of beziers, we cannot detect a good mouseclick/mouseover on them: TODO code bezier check if required.

    _lastclickpt = e->pos(); // always store the last click position.

    QCanvasItemList ilist = canvas()->collisions(e->pos());

    bool bgclicked = true;
    int iRTTI = 0;
    //QCanvasItem *canvitem = 0;
    Gadget *gadget = 0;


    // Control key scale and rotate overide anything under cursor pos.
    if ( _controlkeydown && e->button() == Qt::LeftButton )
    {   _translateviewstartpt = e->pos();
    }
    else if ( _controlkeydown && e->button() == Qt::RightButton )
    {
        _scaleviewstartpt = e->pos();
    }
    else if ( e->button() == Qt::RightButton )
    {
        // ignored for now
        e->ignore();
        return;
    }
    else
    {
        for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
        {
            iRTTI = (*it)->rtti();
            printf("RTTI= %i \n", iRTTI );

            if ( iRTTI == overviewRTTI && (*it)->isVisible() )
            {
                // check where the click was
                if ( e->pos().y() - (*it)->y() < 8 ) // clicked at top
                {
                    _overviewismoving = true;
                    _movingstartpt = e->pos();
                    break;
                }
                else if ( _overview )
                {
                    // clicked in overview, move contents view. 
                    // DISABLED this until its working right:
                    /* // TODO FIX ME
                    QPoint newcpos = _overview->pixelToContentsPoint( e->pos() );
                    setContentsPos( newcpos.x(), newcpos.y() );
                    */

                    // TODO keep overview in same place...

                    //printf(" new contents pos %i %i \n", _overview->pixelToContentsPoint( e->pos() ).x(), 
                    //            _overview->pixelToContentsPoint( e->pos() ).y() );
                   
                    canvas()->update();
                    break;
                }
            }
            else if ( iRTTI == gadgetRTTI && (*it)->isVisible() )
            {   printf("gadget clicked \n");
                gadget = (Gadget*)(*it); 
                bringToFront( gadget );
                _movingstartpt = e->pos();
                _movingitem = (*it);
                gadget->setIsMoving( true );
                _clickedgadget = gadget;

                // add to selection
                if ( !_shiftkeydown && !_selectedgadgets.contains( gadget )  )
                  clearSelection();
                if ( !_selectedgadgets.contains( gadget ) )
                {   _selectedgadgets.append( gadget );
                    (*it)->setSelected( true );
                }
                canvas()->update(); // required
                bgclicked = false;
#ifdef V3UI
                // let other widgets know a new gadget is clicked, hence new current operator.
                if ( gadget->authoringOperator() )
                    emit currentOperator( gadget->authoringOperator() );
                else
                {   //?emit currentOperator( authoringmodel::FootageOperator() );
                    // We emit a blank pointer to let other widgets know that something else is clicked.
                    emit currentOperator( authoringmodel::operator_ptr() );
                }
#endif
                break;
           }
            else if ( iRTTI == socketRTTI && (*it)->isVisible() ) 
            {   printf("socket clicked \n");
                
                _wirestartpt = e->pos();
                Socket * sock = (Socket*)(*it);
                _socketbeingwired = sock;
                Socket *othersock = 0;
                // If already a wire, store its socket info and break the connection. 
                if ( sock->numWiresConnected() > 0 )  // (current thinking is that only one wire can go in a socket)
                {   Wire * curwire = sock->wires().first(); 
                
                    if ( sock->type() == Socket::OUTPUT_SOCKET )
                        othersock = curwire->inSocket();
                    else if ( sock->type() == Socket::INPUT_SOCKET )
                        othersock = curwire->outSocket();

                    sock->disconnectFromWire( curwire );
                    othersock->disconnectFromWire( curwire ); // we'll make a new one to replace it.
                    curwire->hide();
                    curwire->setCanvas(0);
                
                    // *** TODO *** mem leak: delete curwire BUT after we are done with it (see next block)
                  
                }
                // Either way, make a new wire
                _movingwire = new Wire( canvas() );

                QPointArray pts( 0 );
                int dx = 0; //sock->width()/2;
                int yd = +1.0; // y direction
                // Must set the in or out socket of the new wire
                if ( sock->type() == Socket::OUTPUT_SOCKET )
                {   if ( othersock )
                    {
                        _movingwire->setInSocket( othersock ); 
                         othersock->connectToWire( _movingwire );
                        _movingwire->stretchBezierEnd( QPoint( othersock->x(), othersock->y() ) );
                    }
                    else
                    {
                        _movingwire->setOutSocket( sock ); 
                    }
                    printf( "setting out socket \n" );
                    yd = +1.0;
                }
                else if ( sock->type() == Socket::INPUT_SOCKET )
                {   if ( othersock )
                    {
                        _movingwire->setOutSocket( othersock );
                        othersock->connectToWire( _movingwire );
                        _movingwire->stretchBezierStart( QPoint( othersock->x(), othersock->y() ) );
                    }
                    else
                    {    _movingwire->setInSocket( sock );
                    }    
                    printf( "setting in socket \n" );
                    yd = -1.0;
                }

                if ( !othersock )
                {   pts.putPoints(0, 4, int(sock->x() +dx) , int(sock->y() +0) , int(sock->x()+0 +dx), int(sock->y()+(yd*10)),
                    int(sock->x()+0 +dx),int(sock->y()+(yd*10)) ,  int(sock->x() + 0 +dx) , int(sock->y() + 0) );
    
                    _movingwire->setControlPoints( pts );
                    // Sockets know about thier wires. NB we need to disconnect later if the user doesnt connect the other end.
                    sock->connectToWire( _movingwire );
                }
                _movingwire->setIsMoving( true );
                _movingwire->show();
                
                //??canvas()->update(); // needed?
                bgclicked = false;
                break;
            }
            else
            {   // Something else.
                _clickedgadget = 0;
                if ( !_shiftkeydown )
                    clearSelection(); // why here?
#ifdef V3UI
                 // let other widgets know that no operator is selected. TODO This may change, sending 0 not great.
                emit currentOperator( authoringmodel::operator_ptr() );
#endif   
                ;
            }
        } //(end of if block for items)
        if ( bgclicked )
            makeRubberband( e->pos() );

    }
}

void WireupWidget::contentsMouseReleaseEvent( QMouseEvent* e )
{
    QCanvasItemList ilist = canvas()->collisions(e->pos());
    int iRTTI = 0;
    //QCanvasItem *canvitem = 0;
    Gadget *gadget = 0; // TODO FIX THIS. the one below is not this one...
    Socket *sock = 0;

    _overviewismoving = false; // do something more?
    if ( _overview )
        _overview->show();


    if ( _movingitem ) // TODO: rename? be careful if its not a Gadget later!
    {   Gadget *gadget = (Gadget*)(_movingitem); 
        gadget->setIsMoving( false );
        gadget->update(); // needs repaint after releasing mouse to swap the orig pixmap back.
        canvas()->update(); // required
        // Update our data in the authoring model's property bag for the operator.
#ifdef V3UI
        gadget->updatePropertyBag();
#endif
        _movingitem = 0;
  
    }
    else if ( _movingwire )
    {   
        for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
        {
            iRTTI = (*it)->rtti();
            if ( iRTTI == socketRTTI )
            {    sock = (Socket*)(*it );
                 continue;
            }
        }
        
        // Connect to the socket.
        if ( sock )
        {   // Check the connection is invalid, i.e. input-to-input or output-to-output
            bool oktoconnect = true; 
            if ( _movingwire->inSocket() && sock->type() == Socket::INPUT_SOCKET )
                oktoconnect = false;
            else if ( _movingwire->outSocket() && sock->type() == Socket::OUTPUT_SOCKET )
                oktoconnect = false; 
            // TODO Check the connection is not to ourselves...?

            // Check the socket doesnt already have a wire (for now assume only 1 wire can connect).
            if ( sock->numWiresConnected() > 0 )
                oktoconnect = false;
            
            if ( oktoconnect )
            {   sock->connectToWire( _movingwire );
                _movingwire->setIsMoving( false );
                // Tell the wire its new socket
                if ( sock->type() == Socket::INPUT_SOCKET )
                    _movingwire->setInSocket( sock );
                else if ( sock->type() == Socket::OUTPUT_SOCKET )
                    _movingwire->setOutSocket( sock );
                triggerSocketGlow( sock ); // glow!
                _movingwire->update(); // needs repaint to change its color

                // Authoring model: we must tell the data model to do the equivalent connection. 
                //  NB there is no concept of wires in the authoring model, just operators and pins.
            }
            else
            {   // before deleting wire, must tell socket at other end whats about to happen
                if ( _movingwire->inSocket() )
                    _movingwire->inSocket()->disconnectFromWire( _movingwire );
                else if ( _movingwire->outSocket() )
                    _movingwire->outSocket()->disconnectFromWire( _movingwire );
          

                _movingwire->hide(); // usual 'safe' canvas item removal
                _movingwire->setCanvas(0);
            }
        }
        else // dropped wire on something else, so we'll delete the wire.
        { 
            //disconnectAllWires
            _movingwire->hide(); // usual 'safe' canvas item removal
            _movingwire->setCanvas(0);
            // We must tell the originating socket its not connected either.
            _movingwire->connectedSocket()->disconnectFromWire( _movingwire );
            // TODO FIX deletion! I think prob is we get some mouse move events for wire after its gone... 
            //delete _movingwire;
        }

        
        canvas()->update();
        _movingwire = 0;
    }
    else if ( _rubberbandline1 )
    {
        selectFromRubberband();
        QCanvasItemList selection = selectedGadgets();
        QString conv; 
        conv.setNum( selection.size() );
        //if ( selection.size() > 0 )
        //    QMessageBox::information( this, "Wireup", "You selected " + conv + " gadget(s).\n" );
        removeRubberband();     
    }

    _translateviewstartpt = QPoint(0, 0 );
    _scaleviewstartpt = QPoint(0, 0 );

    // Update the overview.
    //NO_overview->setSize( visibleWidth()/5, visibleHeight()/5 );
    //_overview->setScaleFactor( 0.125 );
    if ( _overview )
    {   _overview->setContentsRect( contentsRect() );
        _overview->regenerate();
    }
}

void WireupWidget::resizeEvent ( QResizeEvent * e )
{
    QCanvasView::resizeEvent( e );
    if ( _overview )
    {   _overview->move( contentsX() + 5, contentsY() + visibleHeight() - _overview->height() - 15 );
        canvas()->update();
    }
}

void WireupWidget::contentsMouseMoveEvent( QMouseEvent* e )
{   
    // Any mouse movement triggers a timer. If it fires, we do mouseover effects.
    if ( !_mouseovertimer.isActive() )
    {    _mouseovertimer.start( 200 );
         connect( &_mouseovertimer, SIGNAL(timeout()), this, SLOT(mouseOverEffects()) );
         printf("_mouseovertimer.start\n");
    }
    else
    {
        _mouseovertimer.changeInterval( 300);
        //printf("_mouseovertimer.changeInterval\n");
    }
    /* NOT HERE?
    if ( _mouseoveritem )
    {   _mouseoveritem->setEnabled( false );
        _mouseoveritem = 0;
        canvas()->update();
    }
    */
    
    if ( _overviewismoving && _overview )
    {
        _overview->moveBy( e->pos().x() - _movingstartpt.x(),
                         e->pos().y() - _movingstartpt.y());
        _movingstartpt = e->pos();
        canvas()->update(); // required
    }
    else if ( _movingitem ) // Rename? these are Gadgets only.
    {
        selectionMoveBy( e->pos().x() - _movingstartpt.x(),
                         e->pos().y() - _movingstartpt.y());

        _movingstartpt = e->pos();
        
       canvas()->update(); // required
    }
    else if ( _movingwire )
    { 
        if ( _movingwire->connectedSocket()->type() == Socket::OUTPUT_SOCKET )
        {   _movingwire->setBezierPoint( 3, e->pos() ); // 3 is end point of bezier.
            _movingwire->stretchBezierEnd( e->pos() ); 
        }
        else if ( _movingwire->connectedSocket()->type() == Socket::INPUT_SOCKET )
        {   _movingwire->setBezierPoint( 0, e->pos() );
            _movingwire->stretchBezierStart( e->pos() ); 
        }
               
         
        
        canvas()->update(); 
    }
    else if ( _rubberbandline1 )
    {
        stretchRubberband( e->pos() );
        selectFromRubberband(); // selects as we move band, since mousetracking is true.
    }
    else if ( !_translateviewstartpt.isNull() )
    {   allGadgetsMoveBy( e->pos().x() - _translateviewstartpt.x(),
                         e->pos().y() - _translateviewstartpt.y() );
        canvas()->update(); 
        _translateviewstartpt = e->pos();
    }
    else if ( !_scaleviewstartpt.isNull() )
    {
        if ( e->pos().x() < _scaleviewstartpt.x() )
            zoomOut();
        else
            zoomIn();
        _scaleviewstartpt = e->pos();
    }
}

void WireupWidget::contentsMouseDoubleClickEvent ( QMouseEvent* e )
{
    if ( e->button() == Qt::RightButton )
    {
        // ignored for now
        e->ignore();
        return;
    }

    QCanvasItemList ilist = canvas()->collisions(e->pos());
    int iRTTI = 0;
    for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
    {
        iRTTI = (*it)->rtti();
        if ( iRTTI == gadgetRTTI )
        {
            Gadget *gadg = (Gadget *)(*it);
            QString title = gadg->titleText();
            //QMessageBox::information( this, "Wireup", "Double clicked " + title + " gadget.\n" );
            gadg->setGadgetDrawLevel( Gadget::DRAW_MINIMAL );
            break;
        }
    }
}

void WireupWidget::keyPressEvent ( QKeyEvent * e )
{
    if ( e->key() == Qt::Key_Shift )
    {   _shiftkeydown = true;
        printf("Shift key down.\n");
        e->accept();
    }
    else if ( e->key() == Qt::Key_Control )
    {   _controlkeydown = true;
        printf("Control key down.\n");
        e->accept();
    }
    else
        e->ignore();
}
void WireupWidget::keyReleaseEvent ( QKeyEvent * e )
{
    if ( e->key() == Qt::Key_Shift )
    {   _shiftkeydown = false;
        printf("Shift key release.\n");
        e->accept();
    }
    else if ( e->key() == Qt::Key_Control )
    {   _controlkeydown = false;
        printf("Control key release.\n");
        e->accept();
    }
    else if ( e->key() == Qt::Key_Delete )
    {   
        deleteSelectedGadgets();
        e->accept();
    }
    else if ( _controlkeydown = true && e->key() == Qt::Key_A ) 
    {
        selectAllGadgets();
        canvas()->update();
        e->accept();
    }
    else
        e->accept(); // REQUIRED else we lose focus?
}

void WireupWidget::contentsContextMenuEvent ( QContextMenuEvent * e )
{
    if ( _controlkeydown )
        return;

    QPopupMenu *menu = new QPopupMenu( this );

    QCanvasItemList ilist = canvas()->collisions(e->pos());
    int iRTTI = 0;
    QCanvasItemList::Iterator it; 
    for ( it=ilist.begin(); it!=ilist.end(); ++it)
    {
        iRTTI = (*it)->rtti();
        printf("RTTI= %i \n", iRTTI );

        if ( iRTTI == gadgetRTTI && (*it)->isVisible() )
        {   printf( "clicked a gadget \n");
            break;
        }
       
    }

    menu->move( mapToGlobal( contentsToViewport(e->pos() )) );

    //Popup menu for event clicked.
   if ( iRTTI == gadgetRTTI )  
   {
        menu->insertItem( "Delete", 1 );
        menu->setMouseTracking( TRUE );
        Gadget *gadge = dynamic_cast<Gadget*>(*it);
        
        int id = menu->exec();
        if ( id != -1 && gadge )
        {
            switch ( id )
            {
                case 1 :
                    {   printf("  delete this gadget \n" );
                        removeGadget( gadge );
                        break;
                    }
            }
        }
    }
    else if ( iRTTI == 0 ) // bg clicked
    {
       menu->insertItem( "Toggle orientation", 1 );
       menu->setMouseTracking( TRUE );
       int id = menu->exec();
       if ( id != -1 )
        {
            switch ( id )
            {
                case 1 :
                    {   printf("  toggle orientation \n" );
                        toggleOrientation();
                        break;
                    }
            }
        }
   }
   else // something else clicked
   {
       menu->insertItem( "No options available", 1 );
       menu->setMouseTracking( TRUE );
       int id = menu->exec();

   }
}

void WireupWidget::wheelEvent ( QWheelEvent * event )
{  
    // delta is +120 for pushing wheel away from user, -120 for pulling toward user.
    if ( _controlkeydown )
    {
        if ( event->delta() > 0 )
            zoomIn();
        else if ( event->delta() < 0 )
            zoomOut();
        event->accept();
    }
    else
        event->ignore();
}


void WireupWidget::contentsDragEnterEvent ( QDragEnterEvent *e )
{
    e->accept(); // important! else we can lose the Dnd event.
}

void WireupWidget::contentsDropEvent ( QDropEvent *e )
{
    printf( " dropEvent  \n" );
    
    if ( !QUriDrag::canDecode(e) ) {
        e->ignore();
        return;
    }

    QStringList lst;
    QUriDrag::decodeToUnicodeUris( e, lst );
    int counter = 0;
    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
    {
        QString filenameuri = *it;

        // Check the type (the bit before the ://) to see if its a file, or an effect etc.
        if ( filenameuri.contains( "file:" ) ) // (e.g. Dnd from desktop)
        {
            // QFile doesnt like the file:/// prefix, but we need to preserve one / on non-Windows systems.
#ifdef _WIN32
            QString filename = filenameuri.replace("file:///", "" ); 
#else
            QString filename = filenameuri.replace("file://", "" ); 
#endif
            QFileInfo fi( filename );
            printf(" %s \n", filename.latin1() );        
            if ( fi.exists() )
            {
                printf(" adding gadget for %s \n", filename.latin1() );
                Gadget *mygadge = this->createGadget( filename );
                mygadge->move( e->pos().x() + 20*_zoom*counter, e->pos().y() + 20*_zoom*counter ); // stagger them
            
                counter++;
                mygadge->show();
                mygadge->zoomChangedRedraw(); // without this, the sockets are not in place (tho I though they call this fn).
            }
        }
        else if ( filenameuri.contains( "effect:" ) )
        {
            QString effectnameorig = filenameuri.replace("effect://", "" );
            QString effectname = effectnameorig;
            printf(" effect name dropped is %s \n", effectname.latin1() );        
            // TODO finish this properly. Metadatapath was also used in framework effectsbrowser, seems 
            // bad to use another here... Anyway, should use image larger than the usual effects browser thumbs.
            QString metadatapath( "../metadata-for-effects/");
            QString fn = metadatapath + effectname.replace( " ", "" ).lower()  + "/" + effectname.replace( " ", "" ).lower() + "-frame2.png"; // mid way...
            printf( "  metadata filename for effect is %s \n", fn.latin1() ); 
            QFileInfo fi( fn );
            QImage img;
            if ( fi.exists() )
                img = QImage( fn );
            else 
            {   qDebug( "no image for effect, using default \n" ); 
                img = qembed_findImage( "default-effect-thumb" ); //(NB this in *our* embedded images file + framework embedded images.
            }

            if ( img.isNull() )
                qDebug( "img is null for effect gadget\n" );

            Gadget *mygadge = this->createEffectGadget( effectnameorig.latin1(), img );
            mygadge->setTitleText( effectnameorig );
            mygadge->move( e->pos().x() + 20*_zoom*counter, e->pos().y() + 20*_zoom*counter ); // stagger them
            mygadge->show();
            mygadge->zoomChangedRedraw(); // without this, the sockets are not in place (tho I though they call this fn).
        }
        else
            printf( " %s unknown type, ignoring \n", filenameuri.latin1() );
    }
}


// === SELECTION and GROUPING FUNCTIONS ======================================

void WireupWidget::makeRubberband( QPoint topleft )
{
    // Init and move the rubber band lines
    if ( _rubberbandline1 == 0 )
    {
        _rubberbandline1 = new QCanvasLine( canvas() );
        _rubberbandline2 = new QCanvasLine( canvas() );
        _rubberbandline3 = new QCanvasLine( canvas() );
        _rubberbandline4 = new QCanvasLine( canvas() );
        
        QPen pen;
        pen.setColor( qApp->palette().color( QPalette::Active, QColorGroup::Highlight ));
        //pen.setStyle( Qt::DashLine ); // try dashed if you like ;-)
        pen.setWidth( 1 );
       
        _rubberbandline1->setPen( pen );
        _rubberbandline2->setPen( pen );
        _rubberbandline3->setPen( pen );
        _rubberbandline4->setPen( pen );
    }

    _rubberbandline1->setPoints (topleft.x(), topleft.y(), topleft.x(), topleft.y() );
    _rubberbandline2->setPoints (topleft.x(), topleft.y(), topleft.x(), topleft.y() );
    _rubberbandline3->setPoints (topleft.x(), topleft.y(), topleft.x(), topleft.y() );     
    _rubberbandline4->setPoints (topleft.x(), topleft.y(), topleft.x(), topleft.y() );
   
    bringToFront( _rubberbandline1 );
    bringToFront( _rubberbandline2 );
    bringToFront( _rubberbandline3 );
    bringToFront( _rubberbandline4 );
    _rubberbandline1->show();
    _rubberbandline2->show();
    _rubberbandline3->show();
    _rubberbandline4->show();
    canvas()->update();
}


void WireupWidget::stretchRubberband( QPoint bottomright )
{
    if ( !_rubberbandline1 ) 
        return;
    
    QPoint topleft = _rubberbandline1->startPoint();
    _rubberbandline1->setPoints (topleft.x(), topleft.y(), bottomright.x(), topleft.y() );
    _rubberbandline2->setPoints (topleft.x(), topleft.y(), topleft.x(), bottomright.y() );
    _rubberbandline3->setPoints (bottomright.x(), topleft.y(), bottomright.x(), bottomright.y() );     
    _rubberbandline4->setPoints (topleft.x(), bottomright.y(), bottomright.x(), bottomright.y() );
    canvas()->update(); 
}

void WireupWidget::selectFromRubberband()
{  
    QCanvasItemList allitems = canvas()->collisions ( QRect( _rubberbandline1->startPoint(), _rubberbandline4->endPoint() ));
    if ( !_shiftkeydown )
        clearSelection( false );
     for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
        {
            if ( (*it)->rtti() == gadgetRTTI )
                if ( !_selectedgadgets.contains( (*it) ) )
                {    _selectedgadgets.append( (*it ));
                    (*it)->setSelected( true );
                }
        }
}

void WireupWidget::selectAllGadgets()
{
    QCanvasItemList allitems = canvas()->allItems();
    for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
        {
            if ( (*it)->rtti() == gadgetRTTI )
                if ( !_selectedgadgets.contains( (*it) ) )
                {    _selectedgadgets.append( (*it ));
                    (*it)->setSelected( true );
                }
        }
}

void WireupWidget::removeRubberband()
{
    _rubberbandline1->hide();
    _rubberbandline2->hide();
    _rubberbandline3->hide();
    _rubberbandline4->hide();

    delete _rubberbandline1;
    delete _rubberbandline2;
    delete _rubberbandline3;
    delete _rubberbandline4;
    _rubberbandline1 = 0; 
    _rubberbandline2 = 0; 
    _rubberbandline3 = 0;
    _rubberbandline4 = 0;
    canvas()->update();
}

void WireupWidget::clearSelection( bool updatecanvas )
{
    // call the gadgets to change their look etc.
    for (QCanvasItemList::Iterator it=_selectedgadgets.begin(); it!=_selectedgadgets.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI )
            (*it)->setSelected( false );
    }
    if ( updatecanvas ) 
        canvas()->update();
    _selectedgadgets.clear();
}

void WireupWidget::selectionMoveBy( int x, int y )
{
    //(Note, currently we do property bag updates every time (move()), poss disable cos its a bit of overkill)

    for (QCanvasItemList::Iterator it=_selectedgadgets.begin(); it!=_selectedgadgets.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI )
        {   Gadget* gadg = ( Gadget* )(*it); 
            (*it)->moveBy( x, y );
#ifdef V3UI
            gadg->updatePropertyBag();
#endif
        }
            
    }
}

void WireupWidget::allGadgetsMoveBy( int x, int y )
{
    QCanvasItemList allitems = canvas()->allItems();
    for (QCanvasItemList::Iterator it=allitems.begin(); it!=allitems.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI )
            (*it)->moveBy( x, y );
    }
}

void WireupWidget::triggerSocketGlow( Socket* socket )
{
    socket->beginGlow();
    _glowingsocket = socket;
    QTimer::singleShot( 300, this, SLOT( removeGlows() ) );
}

void WireupWidget::removeGlows()
{   // NOTES: possible (but unlikely) crash if glowing socket deleted before we reach here -- so keep glows very short!
    if ( _glowingsocket )
        _glowingsocket->endGlow();
}

void WireupWidget::mouseOverEffects()
{   // TODO this slot seems to still be called multiple times?

    if ( !_mouseoversenabled )
        return;

    //printf ("Mouseover effect triggered\n");
    _mouseovertimer.stop(); // OR use enter/leave event? or cannot, not a qwidget?!
    QPoint position = mapFromGlobal( QCursor::pos() );
    QCanvasItemList ilist = canvas()->collisions( position );

    int iRTTI = 0;
    Gadget *gadget = 0;

    // We keep ptr to the old item, we might keep it highlighted after checking mouseover pos.
    QCanvasItem *prevmouseover = _mouseoveritem;

    // NB wires are dealt with differently, because we mis-used the bounding box :(
    // For now, do mouse overs only if cursor is near a socket.  We ignore mouse overs on sockets themselves, 
    // we just look for gadgets and turn on the sockets at the same time.  
    // TODO we dont want mouseovers to 
    // disappear immediately the mouse moves to another item, since the user wants to see a wire + socket, for 
    // example. 
    for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
    {
        iRTTI = (*it)->rtti();
        if ( (*it)->isVisible() )
        {   if ( iRTTI == wireRTTI )
            {   // Check we're near a socket.
                bool nearsocket = false;
                Wire *thewire = (Wire*)(*it);
                Socket *insock = thewire->inSocket();
                Socket *outsock = thewire->outSocket();
                if ( insock )
                {
                    QPoint itempos( int(insock->x()), int(insock->y()) );
                    QPoint dist = itempos - position;
                    //printf( " Insock dist.x y %i %i \n",  dist.x(), dist.y() );
                    //printf( " Insock dist.manhattanLength %i \n",  dist.manhattanLength() );
                    if ( dist.manhattanLength() < 40 )
                        nearsocket = true;
                }
                if ( outsock )
                {
                    QPoint itempos( int(outsock->x()), int(outsock->y()) );
                    QPoint dist = itempos - position;
                    //printf( " Outsock dist.x y %i %i \n",  dist.x(), dist.y() );
                    //printf( " Outsock dist.manhattanLength %i \n",  dist.manhattanLength() );
                    if ( dist.manhattanLength() < 40 )
                        nearsocket = true;
                }
                if ( nearsocket ) // ok, we hope this wire is the one they wanted...
                {
                    //printf( "chosen wire. mouse over rtti is %i \n", iRTTI );
                    (*it)->setEnabled( true );
                    _mouseoveritem = (*it );
                    break;
                }
            }
            else if ( iRTTI == gadgetRTTI )
            {       //printf( "gadget mouse over rtti is %i \n", iRTTI );
                    (*it)->setEnabled( true );
                    _mouseoveritem = (*it );
                    break;
            }
            else    
            {
                // ignore items we havent coded for
                //printf( "ignored mouse over rtti is %i \n", iRTTI );
            }
   
        }
    }

     // Deal with any previous mouse over
    if ( prevmouseover && _mouseoveritem != prevmouseover )
    {
        prevmouseover->setEnabled( false );
    }

    canvas()->update();

}

void WireupWidget::contentsMovingSlot(int x, int y )
{
    if ( _overview )
    {    _overview->move( contentsX() + 5, contentsY() + visibleHeight() - _overview->height() - 10 );
        if ( _overview->visible() )
        {   _overview->hide();
            canvas()->update(); // jerks around too much if done every time :(
        }
    }
}

