/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "Gadget.h"
#include "wireup-embedded-images.h"
#include <string>
#include <qimage.h>
#include <qcanvas.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qfont.h>
#include <qlist.h>
#include <qfileinfo.h>
#include <qfontmetrics.h>
#include <qcolor.h>

// Initialise statics
int Gadget::gadget_counter = 0; // Incremental counter of all gadgets.
QPixmap * Gadget::_boxpixmap = 0;
double Gadget::_gadgetzoom =  0.40;
int Gadget::_gadgetzoomlevel( 2 );
const int Gadget::_defaultwidth = 266; // keep larger than required, scaling up always worse.
const int Gadget::_defaultheight = 229;
//bool Gadget::_boxpixmapupdaterequired = true;
double Gadget::_defaultimagexborder = 10; // borders are relative to defaultwidth etc, and scale with zoom
double Gadget::_defaultimageyborder = 20; 
double Gadget::_defaultminimalheight = 35;
bool Gadget::_orientvertical = true;
// Default colors
QColor Gadget::_selectioncolor = QColor( 131, 255, 244 );
QColor Gadget::_defaultgadgetcolor = QColor( 59,59,59 );
QColor Gadget::_effectgadgetcolor = QColor( 99, 147, 147 );
QColor Gadget::_widegadgetcolor = QColor( 59,59,59 );
QColor Gadget::_layergadgetcolor = QColor( 0, 0 ,0 ); // was pale red ( 119, 67, 67 );

QPixmap* Gadget::_boxpixmaps = 0;

#ifdef V3UI  // had to also comment these out for v2, it confuses the moc.
//Gadget::Gadget( QCanvas *canvas, authoringmodel::operator_ptr op ) : QCanvasRectangle(canvas), _authoringoperator(op), _titletext( "Unnamed" )
#else
Gadget::Gadget( QCanvas *canvas ) : QCanvasRectangle(canvas), _titletext( "Unnamed" )
#endif
{
	init(); 
}

#ifdef V3UI
//Gadget::Gadget( QCanvas *canvas, QImage &image, authoringmodel::operator_ptr op ) : QCanvasRectangle(canvas), _authoringoperator(op), _titletext( "Unnamed" )
#else
Gadget::Gadget( QCanvas *canvas, QImage &image ) : QCanvasRectangle(canvas), _titletext( "Unnamed" )
#endif
{
	init(); 	
    setOriginalImage( image );
}

void Gadget::init()
{	
    setGadgetType( DEFAULT_GADGET );
    setGadgetDrawLevel( DRAW_NORMAL );
    // Append to the operator's property bag
#ifdef V3UI
    initPropertyBag();
    // Observe the property bag.
    if ( authoringOperator() )
        authoringOperator()->propertyBag()->attachObserver( this );
#endif

     // init the zoom levels. NB these arent linear. This could all be shared/static?
    // NB also, this is duplicate of whats in WireupWidget, easy way to share?
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

	// Note: Z ordering. To keep items seperate in z order use static gadget_counter*5.
	// basez_ is the starting z value. Items in front of all gadgets must be given z > 800000.

    _islocked = false; 
    _isenabled = false; 
    _hasmouseover = false; 
    _ismoving = false; 
    _isselected = false;
    _isactive = false; 

    _imagexborder = _defaultimagexborder;
    _imageyborder = _defaultimageyborder;

    _boxcolor = QColor( 59,59,59 ); // FIX ME

    _fadedpixmapupdaterequired = true;
   // NO static _boxpixmapupdaterequired = true;
//TO JUNK    setSize( int(_defaultwidth * _gadgetzoom ), int ( _defaultheight * _gadgetzoom ) );
    updateSize();

   // if ( _boxpixmapupdaterequired )
     //   generateBoxPixmap();

    // NEW cached array of scaled pixmaps
 /// OLD   if ( _boxpixmaps == 0 )
   /// OLD     generateBoxPixmaps();
    _mainpixmaps = 0;  // created when originalimage is set.

	_basez = gadget_counter * 5;
	gadget_counter++;
	if ( gadget_counter > 800000 )  // just in case!
		gadget_counter = 0;
	
	setZ( _basez ); 

}

#ifdef V3UI
void Gadget::initPropertyBag()
{

    printf( "initPropertyBag \n" );

    if ( authoringOperator() )
    {   
        // test....
        std::string optype = authoringmodel::Operator::type( _authoringoperator->type() );
        printf(" _authoringoperator->type() %s \n", optype.c_str() );
        
        // Add the x and y position as non-keyframable properties.
        //const std::string prop1_name( "TestProperty1" );
        printf(" _titletext.ascii() %s \n", _titletext.ascii() );
        authoringOperator()->propertyBag()->addProperty( "X position", static_cast<double>( x() ), true, false, false );
        authoringOperator()->propertyBag()->addProperty( "Y position", static_cast<double>( y() ), true, false, false );

        //printf(" just adderd properties x y %d %d \n", x(), y() );
        std::string ssname = _titletext.ascii();
        authoringOperator()->propertyBag()->addProperty( "Display name", static_cast<std::string>( titleText().latin1() ), true, false, false );
        authoringOperator()->propertyBag()->addProperty( "Enabled", static_cast<bool>( false ), true, false, false );

    }

}
#endif

#ifdef V3UI
void Gadget::updatePropertyBag()
{

    //printf(" in Gadget::updatePropertyBag x=%f y=%f \n", x(), y() );
    if ( authoringOperator() )
    {
        // (make sure the bag is updated in 1 'pass', else we get observer signals coming back from each).
        authoringOperator()->propertyBag()->beginUpdate();
        authoringOperator()->propertyBag()->property( "X position" )->value( static_cast<double>( x() ));
        authoringOperator()->propertyBag()->property( "Y position" )->value( static_cast<double>( y() ));
        authoringOperator()->propertyBag()->property( "Enabled" )->value( static_cast<bool>( selected() ));
        authoringOperator()->propertyBag()->property( "Display name")->value( static_cast<std::string>( titleText() ));
        authoringOperator()->propertyBag()->endUpdate();
    }

}
#endif

Gadget::~Gadget()
{
	// QtCanvas deletes the canvas items for us -- 
	// we only need to worry about extra memory we have used.
}


void Gadget::setGadgetType( GADGET_TYPE type )
{
    qDebug( "setGadgetType %i \n" , type );

    if ( _gadgettype != type )
    {
        _gadgettype = type;

        if ( type == DEFAULT_GADGET )
        {
            setBoxColor( _defaultgadgetcolor, false );
        }
        else if ( type == EFFECT_GADGET )
        {
            setBoxColor( _effectgadgetcolor, false );
        }
        else if ( type == WIDE_GADGET )
        {
            setBoxColor( _widegadgetcolor, false );
        }
        else if ( type == CIRCULAR_GADGET )
        {
            setBoxColor( _layergadgetcolor, false );
            //setGadgetDrawLevel( DRAW_NOBOX );
        }

        canvas()->update();
    }
}

void Gadget::setGadgetDrawLevel( GADGET_DRAW_LEVEL level )
{
    printf(" setGadgetDrawLevel %i \n", level );
    if ( _drawlevel != level )
    {
        _drawlevel = level;
        
        if ( level == DRAW_NOBOX )
        {   delete [] _mainpixmaps;
            _mainpixmaps = 0;
            generateMainPixmaps();
            updateSize();
        }

        if ( level == DRAW_MINIMAL )
        {
            updateSize();
        }

        canvas()->update();
    }
}
#ifdef V3UI
void Gadget::onChangeNotification( authoringmodel::observee_ptr pSubject ) throw()
{
    // We only observer the property bag for the operator we 
    //printf(" **** Gadget::onChangeNotification\n");
    
    authoringmodel::propertybag_ptr propbag = boost::dynamic_pointer_cast<authoringmodel::PropertyBag>(pSubject);
    if ( propbag )
    {
        //printf("            -- its a property bag\n");
        // Read the bag and update if needed.
        double xproperty = propbag->property( "X position" )->value<double>();
        double yproperty = propbag->property( "Y position" )->value<double>();
        bool enabledproperty = propbag->property( "Enabled" )->value<bool>();
        std::string dispnameproperty = propbag->property( "Display name" )->value<std::string>();
        //printf( "  propbag x=%f y=%f \n", xproperty, yproperty );
        
        // To avoid moving just due to rounding errors, we compare as integer.
        if ( int(xproperty) != int( x() ) || int(yproperty) != int( y() ) )
        {   move( xproperty, yproperty );
            canvas()->update();
        }

        // A test of bool property. For now just select/deselect, since we dont have an 'enabled'...
        if ( enabledproperty != isSelected() )
        {
            setSelected( enabledproperty );
            canvas()->update();
        }

        if ( titleText().latin1() != dispnameproperty )
        {
            setTitleText( dispnameproperty.c_str() );
            canvas()->update();
        }

    }

}
#endif

void Gadget::setZ( double value )
{
    QCanvasRectangle::setZ( value );
    //TODO SOCKETS. keep this -0.5 below ourselves.
    // Wires? dealt with by sockets? or always behind?

     Socket *sock;
     QList<Socket>::iterator sockit =  _outputsockets.begin();
     while ( sockit != _outputsockets.end() )
     {  
         sock = (*sockit);
         sock->setZ( z() - 0.5 ); //?z?
         sockit++;
     }
     sockit =  _inputsockets.begin();
     while ( sockit != _inputsockets.end() )
     {  
         sock = (*sockit);
         sock->setZ( z() - 0.5 ); //?z?
         sockit++;
     }
}

void Gadget::setSize ( int w, int h )
{
    if ( w == width() && h == height() )
        return;

        _mainpixmapupdaterequired = true;    
    _fadedpixmapupdaterequired = true;
    //_boxpixmapupdaterequired = true; // the shared pixmap

      QCanvasRectangle::setSize( w, h );
 
}

void Gadget::zoomChangedRedraw() // bad name??
{
    //printf("In zoomChangedRedraw %i %i \n", int(_defaultwidth * _gadgetzoom ), int ( _defaultheight * _gadgetzoom ) );
// TO JUNK    setSize( int(_defaultwidth * _gadgetzoom ), int ( _defaultheight * _gadgetzoom ) );
    updateSize();
    _mainpixmapupdaterequired = true;    
    _fadedpixmapupdaterequired = true;
    //_boxpixmapupdaterequired = true; // the shared pixmap

    Socket *sock;
     QList<Socket>::iterator sockit =  _outputsockets.begin();
     while ( sockit != _outputsockets.end() )
     {  
         sock = (*sockit);
         alignSocket( sock );
         sock->setSize( 45*_gadgetzoom, 45*_gadgetzoom );
         sockit++;
     }
     sockit =  _inputsockets.begin();
     while ( sockit != _inputsockets.end() )
     {  
         sock = (*sockit);
         alignSocket( sock );
         sock->setSize( 45*_gadgetzoom, 45*_gadgetzoom );
         sockit++;
     }
}


void Gadget::setSelected( bool yes )
{
    QCanvasItem::setSelected( yes );
}

bool Gadget::isSelected()
{
    return QCanvasItem::isSelected();
}

void Gadget::moveBy(double dx, double dy)
{
    QCanvasPolygonalItem::moveBy( dx, dy );
    
    // Move our sockets too
    Socket *sock;
    QList<Socket>::iterator sockit =  _outputsockets.begin();     
    while ( sockit != _outputsockets.end() )
    {
         sock = (*sockit);
         sock->moveBy( dx, dy );
         sockit++;
    }
    sockit =  _inputsockets.begin();
    while ( sockit != _inputsockets.end() )
    {
         sock = (*sockit);
         sock->moveBy( dx, dy );
         ++sockit;
    }
	
}

void Gadget::move( double dx, double dy )
{
    //( Notes, currently we do prop bag update every step, possible overkill)
    QCanvasPolygonalItem::move( dx, dy );
#ifdef V3UI
    updatePropertyBag();
#endif
}

void Gadget::setOriginalImage( QImage &img )
{
    // original image is always stored at default size then scaled later
    _originalimage = img.smoothScale( _defaultwidth - (2* 1 *_imagexborder), _defaultheight - (2* 1 *_imageyborder), QImage::ScaleMin );
   //_originalimage.convertFromImage( scaledimage );
    printf("incomimg img size %i %i \n", img.width(), img.height() );
    printf("orig img size %i %i \n", _originalimage.width(), _originalimage.height() );
   _mainpixmapupdaterequired = true; // used?
   _fadedpixmapupdaterequired = true;    

   // (dont need to erase old pixmaps in _mainpixmaps, they get replaced below?)
   generateMainPixmaps();
}

void Gadget::setOriginalImage( QString filename )
{
    QImage im( filename );
    setOriginalImage( im );
    QFileInfo fi ( filename );
    setTitleText( fi.fileName() );
}

void Gadget::setTitleText( QString text )
{ 
    //QString tidytext = text;
    QFontMetrics fm( QFont( "Arial", suitableFontSize(), QFont::Bold ) );
    bool toolong = false;
    while ( fm.width( text ) > width() -15 ) 
    {   toolong = true;
        QString shortertext = text.left( text.length() - 1 ); 
        text = shortertext;
        //printf( "checking %s %s\n", text.latin1(), shortertext.latin1() );
    }
   
    if ( toolong ) 
           text += "...";
    _titletext = text;

    update();
    canvas()->update();
}

/* // OLD TO BE REMOVED
void Gadget::setMainPixmap( QImage &img )
{
    QImage scaledimage = img.smoothScale( _defaultwidth - (2*_imagexborder), _defaultheight - (2*_imageyborder), QImage::ScaleMin );
   _originalpixmap.convertFromImage( scaledimage );
    
   
    
    
    
 //       QImage scaledimage = img.smoothScale( width() - (2*_imagexborder), height() - (2*_imageyborder), QImage::ScaleMin );
    _mainpixmap.convertFromImage( scaledimage );   // , OrderedAlphaDither );
    _mainpixmapupdaterequired = false;
    _fadedpixmapupdaterequired = true; 

    //_mainpixmap = pm;       
}

void Gadget::setMainPixmap( QString filename )
{
    QImage im( filename );
    setMainPixmap( im );
    setTitleText( filename );

    // create a faded version if we can.
    // TODO only do this on first click, to keep cpu low on resize.
   /* bool ok = false; 
    QPixmap faded = generateFadedPixmap( pm, ok, 130 );
    if ( ok )
        _mainpixmapfaded = faded;
        */
//}

void Gadget::setBoxColor( QColor color, bool update )
{
    _boxcolor = color;
    if ( update )
        canvas()->update();
}

QPixmap Gadget::generateFadedPixmap( QPixmap &pm, bool &ok, int alpha )
{
    //printf("in generateFadedPixmap pix\n");
    QImage img = pm.convertToImage(); 
    QPixmap fadepm = generateFadedPixmap( img, ok, alpha ); 
    return fadepm;
}

QPixmap Gadget::generateFadedPixmap( QImage &img, bool &ok, int alpha )
{
    //printf("in generateFadedPixmap img\n");

    ok = false;
    QPixmap result; 

  //  QImage img = pm.convertToImage();

    if ( !img.isNull() && img.depth() == 32 ) 
        img.setAlphaBuffer( true ); // safe?!?

    if ( img.hasAlphaBuffer() && !img.isNull() && img.depth() == 32 ) // just to trickery if 32 bit with alpha. 
    {
        ok = true; 
        int pr, pg, pb, pa;
        int bright = 30;
        int newalpha;
        for ( int y=0; y<img.height(); y++ )
        {
            for ( int x=0; x<img.width(); x++ )
            {
                //this is the source and destination pixel
                QRgb *p = (QRgb *)img.scanLine(y) + x;

                
                pr=qRed(*p) + bright;
                pg=qGreen(*p) + bright;
                pb=qBlue(*p) + bright;
                pa = qAlpha(*p);
                if ( pr > 255 ) pr = 255;
                if ( pg > 255 ) pg = 255;
                if ( pb > 255 ) pb = 255;
                
                pa == 0 ? newalpha = pa : newalpha = pa - alpha;
                if ( newalpha < 0 ) newalpha = 0;
                *p = qRgba( pr,pg,pb, newalpha );
               
            }
        }
        
        result.convertFromImage( img );
    }
    _fadedpixmapupdaterequired = false;
    return result; 
}

/* // OLD TO BE REMOVED.
void Gadget::generateBoxPixmap()
{
    if ( !_boxpixmapupdaterequired )
        return;

    if ( _boxpixmap != 0 )
        delete _boxpixmap;
    
    _boxpixmap = new QPixmap();

    printf("generateBoxPixmap \n");

    QFileInfo fi( "supernewnode.png" );
    if ( !fi.exists() )
        printf("generateBoxPixmap file not found\n");

    QImage theimage( "supernewnode.png" ); // TODO store always as static, similar to _originalimage trickery?
    QImage scaledimage = theimage.smoothScale( width(), height(), QImage::ScaleMin );
    _boxpixmap->convertFromImage( scaledimage );   // , OrderedAlphaDither );
    _boxpixmapupdaterequired = false;
}
*/

void Gadget::generateMainPixmaps()
{
      if ( _mainpixmaps != 0 )
        return;

      _mainpixmaps = new QPixmap[12];

      if ( _originalimage.isNull() )
          return;

      // border is not used if drawlevel is DRAW_NOBOX
        int xborder = 0;
        int yborder = 0;
        if ( drawLevel() != DRAW_NOBOX )
        {   xborder = _imagexborder;
            yborder = _imageyborder;
        }
        printf( " xborder = %i yborder = %i \n", xborder, yborder );
        double tzoom; 
        for ( int i = 0; i <= 11; i++ )
        {
            tzoom = _zoomlevels[i];
            QImage rescaledimage = _originalimage.smoothScale( (_defaultwidth * tzoom) - (2*tzoom*xborder), (_defaultwidth * tzoom) - (2*tzoom*yborder), QImage::ScaleMin );
            //printf("making mainpixmap number %i \n", i );
            _mainpixmaps[i].convertFromImage( rescaledimage );   // , OrderedAlphaDither );
    }
}


void Gadget::drawShape ( QPainter & p )
{
    int pmz = _gadgetzoomlevel;
    // This next line was how we originally painted the box, using pixmaps. But, this was too inflexible, 
    // we switched to proper paint events, slightly less attracted but faster, + super-flexible with size/color etc.
    // p.drawPixmap( int(x()), int(y()), _boxpixmaps[pmz] );

    // shadow (NB drawing beyond edge of bounding box can leave artefects)
    p.setPen( QPen( QColor( 5,5,5 ), 2 ) ); 
    p.setBrush ( Qt::NoBrush );

    if ( drawLevel() != DRAW_NOBOX )
    {
        if ( type() == CIRCULAR_GADGET && !_ismoving )
            p.drawEllipse( x(), y(), width(), height() );
        else if ( !_ismoving )
            p.drawRoundRect( x(), y(), width(), height(), 15, 15 );
    }

    if ( isSelected() )
        p.setPen( _selectioncolor );
    else
        p.setPen( QPen( QColor( 75,75,75 ), 2 ) );
    
    p.setBrush ( _boxcolor );
    if ( drawLevel() != DRAW_NOBOX )
    {
        if ( type() == CIRCULAR_GADGET && !_ismoving )
            p.drawEllipse( x(), y(), width(), height() );
        else if ( !_ismoving )
            p.drawRoundRect( x(), y(), width(), height(), 15, 15 );
    }

    p.setPen ( _boxcolor );

    if ( drawLevel() != DRAW_NOIMAGE && _ismoving && _fadedpixmapupdaterequired ) //(generate on first click)
    {
        bool ok = false; 
        QPixmap faded = generateFadedPixmap( _mainpixmaps[pmz], ok, 130 );
        if ( ok )
            _mainpixmapfaded = faded;

    }

    // ORIG, when we used the box pixmaps
    /*int tocenter = ( _boxpixmaps[pmz].width() - _mainpixmaps[pmz].width() ) / 2;
    int toycenter = ( _boxpixmaps[pmz].height() -(2*_gadgetzoom*_imageyborder) - _mainpixmaps[pmz].height() ) / 2;
    */
    int tocenter = ( width() - _mainpixmaps[pmz].width() ) / 2;
    int toycenter = ( height() -(2*_gadgetzoom*_imageyborder) - _mainpixmaps[pmz].height() ) / 2;
 
    if ( tocenter < 0 )
        tocenter = 0;
    if ( toycenter < 10*_gadgetzoom ) // hack! TODO fix properly
        toycenter = 10*_gadgetzoom;
        
    int xi = x() + tocenter;
    int yi = y() + toycenter; // + int(_gadgetzoom*_imageyborder); // (not sure why we need extra 1, prob just rounding above)
    //int yi = int (y() ) + int(_gadgetzoom*_imageyborder);

    if ( drawLevel() != DRAW_NOIMAGE )
    {
        if ( _ismoving && !_mainpixmapfaded.isNull() ) 
        {    p.drawPixmap( QRect( xi, yi, _mainpixmapfaded.width(), _mainpixmapfaded.height() ) , _mainpixmapfaded );
        }
        else
        {   p.drawPixmap( QRect( xi, yi, _mainpixmaps[pmz].width(), _mainpixmaps[pmz].height() ) , _mainpixmaps[pmz] );
        }
    }
    int h = _mainpixmaps[pmz].height() - 1;
    int w = _mainpixmaps[pmz].width() - 1;

    // p.setPen( Qt::green ); // for testing

    // cut off the corner pixels 
    if ( drawLevel() == DRAW_NORMAL && !_ismoving )
    {   p.drawPoint(xi,yi); p.drawPoint(xi,yi+h); p.drawPoint(xi+w,yi+h); p.drawPoint(xi+w,yi); // 4 corners 
        p.drawPoint(xi+1,yi); p.drawPoint(xi+1,yi+h); p.drawPoint(xi+w-1,yi+h); p.drawPoint(xi+w-1,yi); // 4 x diags
        p.drawPoint(xi,yi+1); p.drawPoint(xi,yi+h-1); p.drawPoint(xi+w,yi+h-1); p.drawPoint(xi+w,yi+1); // 4 y diags 
    }

    // Text
    int fontsize =  suitableFontSize();
    if ( width()> 125 )
        p.setFont( QFont( "Arial", fontsize, QFont::Bold ) );
    else
        p.setFont( QFont( "Arial", fontsize ) );
    int yborder = 8; // TODO use other border stuff
    //QString tmp;
    //tmp.setNum( gadget_counter );
    //QString txt = "Image: " + _titletext;
    QString txt = _titletext;

    p.setPen( QColor ("black")  );
    // TODO Should cache some of these calcs to be even faster.
    p.drawText( (int)x() + int(_gadgetzoom*_imagexborder) + 2, (int)y() + height() - fontsize + 2 - int(0.5*_gadgetzoom*_imageyborder ), txt ); // 'shadow' 
    p.setPen( QColor ("gray90")  );
    p.drawText( (int)x() + int(_gadgetzoom*_imagexborder), (int)y() + height() - fontsize +1 - int(0.5*_gadgetzoom*_imageyborder), txt );

    if ( isSelected() )
    {   
        p.setPen( _selectioncolor );
        p.setBrush ( Qt::NoBrush );

        if ( drawLevel() != DRAW_NOBOX )
        {
            if ( type() == CIRCULAR_GADGET )
                p.drawEllipse( x(), y(), width(), height() );
            else
                p.drawRoundRect( x(), y(), width(), height(), 15, 15 );
        }
    }
}

void Gadget::updateSize()
{
    int width = int(_defaultwidth * _gadgetzoom );
    int height = int ( _defaultheight * _gadgetzoom );

    if ( drawLevel() == DRAW_MINIMAL )
    {
        // TEST!
        height = _defaultminimalheight;
    }

    if ( type() == WIDE_GADGET )
        verticalOrientation() ? width *=2 : height *= 2;
    if ( type() == CIRCULAR_GADGET )
    {   // hard code to be smaller than usual (for layer nodes really). Could add as option?
        width *= 0.65; 
        height *= 0.65; 
        width > height ?  width = height : height = width ;
    }
    setSize( width, height );
}

Socket* Gadget::addSocket( Socket::SOCKET_TYPE type )
{
    Socket *socket = new Socket( canvas(), type );
    if ( type == Socket::INPUT_SOCKET )
        _inputsockets.append( socket );
    if ( type == Socket::OUTPUT_SOCKET )
        _outputsockets.append( socket );

    socket->setSize( 40*_gadgetzoom, 40*_gadgetzoom );
    socket->setZ( z()-0.5 ); 
    socket->show(); // comment this out if you want mouseover effect

    // NO NEED TO ALIGN ALL SOCKETS alignSocket( socket );
    zoomChangedRedraw();
    // (no canvas update needed)
    return socket;
}

Socket* Gadget::addOutputSocket() 
{ 
    return addSocket( Socket::OUTPUT_SOCKET ); 
}

Socket* Gadget::addInputSocket() 
{    
    return addSocket( Socket::INPUT_SOCKET ); 
}

void Gadget::removeSocket( Socket* sock )
{
    sock->disconnectAllWires();
    sock->setEnabled( false );
    sock->hide();
    sock->setCanvas( 0 );
    // delete sock; TODO fixme! crash due to being in some list elsewhere.
}

void Gadget::removeAllSockets()
{
    Socket *sock;
     QList<Socket>::iterator sockit =  _outputsockets.begin();
     while ( sockit != _outputsockets.end() )
     {  
        sock = (*sockit);
        removeSocket( sock );
        sockit++;
     }
     sockit =  _inputsockets.begin();
     while ( sockit != _inputsockets.end() )
     {  
        sock = (*sockit);
        removeSocket( sock );
        sockit++;
     }
    
}

void Gadget::setEnabled( bool yes )
{
    // TEST. we could show/hide items based on this:
    
    QCanvasItem::setEnabled( yes );
    Socket *sock;
     QList<Socket>::iterator sockit =  _outputsockets.begin();
     while ( sockit != _outputsockets.end() )
     {  
         sock = (*sockit);
         sock->setEnabled( yes );
         sockit++;
     }
     sockit =  _inputsockets.begin();
     while ( sockit != _inputsockets.end() )
     {  
         sock = (*sockit);
         sock->setEnabled( yes );
         sockit++;
     }
    
}

void Gadget::setVerticalOrientation( bool set )
{ 
    printf (" Gadget::setVerticalOrientation %i \n", set );
    _orientvertical = set; 
}

int Gadget::suitableFontSize()
{
    return ( 6 + int(6*_gadgetzoom) );
}

void Gadget::alignSocket( Socket* sock )
{
    // Position depends on zoom level + number of sockets we have to draw.
    
    if ( sock->type() == Socket::OUTPUT_SOCKET )
    {   
        int scount = inputSockets().count();
        int disp;
        if ( verticalOrientation() )
            disp = int ( width()/(outputSockets().count()+1) );
        else 
            disp = int ( height()/(outputSockets().count()+1) );

        int num = 1 + outputSockets().find( sock ); //bit inefficient, its a list.
        
        if ( verticalOrientation() )
            sock->move( x() + ( disp*num ) ,  y() + height() );
        else
            sock->move( x() + width() ,  y() + ( disp*num ) );

        // Now arrange the labels so they dont clash
        if ( sock->label() )
        {   if ( verticalOrientation() )
                sock->label()->move( sock->x(), sock->y() + (num)*sock->label()->boundingRect().height() ) ;
            else 
                sock->label()->move( sock->x() + sock->width(), sock->y() );

            QFont lfont( QFont( "Arial", suitableFontSize() ) );
            lfont.setItalic( true );
            sock->label()->setFont( lfont );
        }
    }
    else if ( sock->type() == Socket::INPUT_SOCKET )
    {   int disp; 
        if ( verticalOrientation() )
            disp = int ( width()/(inputSockets().count()+1) );
        else
            disp = int ( height()/(inputSockets().count()+1) );

        int num = 1 + inputSockets().find( sock ); //bit inefficient, its a list.
        
        if ( verticalOrientation() )
            sock->move( x() + ( disp*num ), y() );
        else
            sock->move( x(), y() + ( disp*num)  );

        // Now arrange the labels so they dont clash
        if ( sock->label() )
        {   if ( verticalOrientation() )
                sock->label()->move( sock->x(), sock->y() - (num+1)*sock->label()->boundingRect().height() );
            else
                sock->label()->move( sock->x() - sock->label()->boundingRect().width() - sock->width(), sock->y() );
            QFont lfont( QFont( "Arial", suitableFontSize() ) );
            lfont.setItalic( true );
            sock->label()->setFont( lfont );
        }
  
    }
}





