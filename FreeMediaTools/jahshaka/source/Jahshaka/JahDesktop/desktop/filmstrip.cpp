#include "filmstrip.h"

#include <qtimer.h>
#include <qpainter.h>

#include <cmath>

#include "imageitem.h"


FilmFrame::FilmFrame( ImageItem* im, QCanvasPixmapArray * a, QCanvas * canvas ) 
    : QCanvasSprite( a, canvas ),
      imageitem( im )
{
    // nothing else to do.
}

FilmFrame::~FilmFrame()
{
    // nothing else to do
}

void FilmFrame::draw( QPainter &painter )
{
    int yborder = 8;
    QCanvasSprite::draw( painter );
    QString txt;
    txt.setNum( fnum );
    painter.setPen( QColor ("black")  );
    painter.drawText( (int)x() + width() - 20 -1, (int)y() + height() - yborder -1 , txt ); // 'shadow' 
    painter.setPen( QColor ("white")  );
    painter.drawText( (int)x() + width() - 20, (int)y() +height() - yborder, txt );
}

FilmStrip::FilmStrip( ImageItem *image, QCanvas *canvas, int stripwidth, int zoom )
	: QCanvasRectangle( canvas )
	, imageitem ( image )
	, sticky(false)
	, framespacex(2)
	, zoomlevel(zoom)
	, framespacey(5)
	, xvelocity(0.0)
	, timerinterval( 40 ) // ftoshow( framestoshow)
{
    ourexchange = new assetExchange;
    
    zoomlevel = 1;

    ftoshow = int( stripwidth / imageitem->width() );
    //printf ("ftoshow is %i \n", ftoshow);

    setPen( QColor( Qt::black ) );
    setBrush( QColor( Qt::black ) );
    
    move( imageitem->x(), imageitem->y() );

    // set our rectangle bounding size. this surrounds all frames in the frameholder.
    setSize( (ftoshow+1) * ((imageitem->width() + framespacex) ), imageitem->height() + (2*framespacey)  ); 
    stripwidth = width(); // this should stay constant, else problems!
    
    blackstrip = new QCanvasRectangle( canvas );
    blackstrip->setPen( QColor( Qt::black ) );
    blackstrip->setBrush( QColor( Qt::black ) );

    moveBy( -ftoshow*(imageitem->width() + framespacex) , -framespacey -1 );
    setZ( image->z() - 1);
    zframeheight = z();
    
    frameinitdone = false;
    initFrames();
    frameinitdone = true; 
}

FilmStrip::~FilmStrip()
{
	// frameholder gets deleted.
	delete ourexchange; 
}

void FilmStrip::initFrames()
{
    // loads the frameholder
    rightmostframe = 0;
    leftmostframe = 0;
    framewidth = imageitem->width();
    rightmostframenum = 0; 
    leftmostframenum = 0;
    bool firstcheck = true;
    leftmostframex = 0; 
    rightmostframex = 0;
    int imgframe = imageitem->currentFrameNumber();

    //for ( int framenum = -ftoshow; framenum < ftoshow + 1 && framenum*zoomlevel < imageitem->maxFrames(); framenum++ )
    for ( int framenum = -ftoshow; framenum < ftoshow + 1 && (imgframe + framenum*zoomlevel) <= imageitem->maxFrames(); framenum++ )
    {   
        int frametoget = imgframe + framenum*zoomlevel;

        if ( frametoget >= imageitem->getClip( ).startframe && frametoget <= imageitem->maxFrames()  ) 
        {   QCanvasSprite *frame = getNewFrame( frametoget );

             // move frames into a line, with 2 pixel space inbetween
             frame->move( imageitem->x() + ( (framenum * (framewidth+framespacex) ) ), imageitem->y() );
             frame->setZ( zframeheight );
             frame->show();
             frameholder.append( frame );
             if ( firstcheck || frame->x() < leftmostframex ) 
             {    leftmostframex = int( frame->x() );
                  leftmostframenum = frametoget; 
                  leftmostframe = frame;
                  firstcheck = false;
             }
             else if ( firstcheck || frame->x() > rightmostframex )
             {  rightmostframex = int( frame->x() );
                rightmostframenum = frametoget; 
                rightmostframe = frame;
                firstcheck = false;
             }
             if ( framenum == 0 )
                 centralframex = int( frame->x() );
        }
    }

    updateBlackStrip();
}

QCanvasSprite * FilmStrip::getNewFrame( int frametoget )
{

    QImage theimage( imageitem->width(), imageitem->height(), 32 );
    if ( frametoget >= imageitem->getClip( ).startframe && frametoget <= (imageitem->maxFrames() ) )  // was *zoomlevel 
         theimage = getFrameImage( frametoget ); //was *zoomlevel 
    else
        theimage.fill( QColor("gray20").rgb() );
    
    FilmFrame *frame = createFrameCanvasSprite( theimage );
    frame->setNumber( frametoget );
    return frame;
}

void FilmStrip::hide()  // (hmm, badly named since we do more than hide!)
{
    QCanvasRectangle::hide();
    blackstrip->hide();
    // delete our frames too
    
    QListIterator<QCanvasItem> it( frameholder );
    QCanvasItem * item;  
    while (( item = it.current() )) 
	{
		item->hide();
        item->setCanvas(0);
		delete item;
	    ++it;
    }
}

void FilmStrip::drawShape ( QPainter & )
{
    // This draws the black rectangle around the whole strip. Disable if you like, but note that 
    // this still defines the bounding region for clicking the clip.
    // DISABLE FOR NOW: QCanvasRectangle::drawShape( p );

    // Can do some fancy bg painting here, if you really want.
   /* e.g.  QPixmap pm( "Pixmaps/fancybg.png" );
            p.drawPixmap ( this->rect(), pm );
    */
}

void FilmStrip::updateBlackStrip()
{
    if ( blackstrip != 0 )
    {
        blackstrip->move( leftmostframex - framespacey, y() );
       // blackstrip->setZ( z() );
        int w = rightmostframex - leftmostframex  + (2*framespacey) + framewidth;
        blackstrip->setSize( w, height() );
    }
    blackstrip->show();

}

void FilmStrip::moveFramesBy( double dx, double )
{
    if ( !frameinitdone )
        return;

    // If we are 'sticky' at this point, snap to the sticky point. Remain stuck if mouse movement slow.
    if ( sticky && std::fabs(dx) < 3.0 )
    {   dx = -stickysnap;
        stickysnap = 0.0;
    }
    else
        sticky = false; 

    // Prevent larger than framewidth jumps
    if ( dx > (framewidth -5 ) )
        dx = framewidth -5;
    else if ( -dx < -(framewidth - 5 ) )
        dx = -( framewidth -5 ); 
    
    // prevent moving past ends. Skip this if frame just deleted?
   // if ( !justdeletedframe ) TODO: decide what goes wrong sometimes
    {   if ( leftmostframenum == 1  && leftmostframex >= int(imageitem->x()) && dx > 0 ) 
            return; 
        else if ( rightmostframenum >= ( imageitem->maxFrames()-1 ) && rightmostframex <= int(imageitem->x()) && dx < 0 )
            return;
        if ( leftmostframe && dx>0 && (leftmostframex+dx) > imageitem->x() ) // (leftmostframe=0 if deletion occured last time)
            return;
        if ( rightmostframe && dx<0 && (rightmostframex+dx) < imageitem->x() )  // (rightmostframe=0 if deletion occured last time)
            return;
    }

    QListIterator<QCanvasItem> it( frameholder );
	
	QCanvasItem * item;
    bool firstcheck = true;
    int leftmost = 0;
    int rightmost = 0;
    QCanvasItem *leftmostp = 0;
    QCanvasItem *rightmostp = 0;

    // move the bounding rect in y only.
    QCanvasRectangle::move( 0, imageitem->y() -framespacey -1  );

	while (( item = it.current() )) 
	{
        int ydisp = int( imageitem->y() - item->y() );
        item->moveBy(dx, ydisp );
		++it;
 
    	// If drag movement is slow and we are within a few pixels of centre frame, behave 'sticky' next time. 
    	// Ignore if velocity move. NB we dont know which frame is currently 'near' centre frame, we have to check all...
    	if ( !sticky && xvelocity < 0.05 && std::fabs(dx) < 3.0 && std::fabs(item->x()- centralframex ) < 8.0 )  
    	{    
			sticky = true;
         	stickysnap = item->x()- centralframex; 
    	}

        if ( firstcheck || int(item->x()) < leftmost ) 
        {   leftmost = int(item->x());
            leftmostp = item;
            firstcheck = false;
        }
        if ( firstcheck || int(item->x()) > rightmost )
        {   rightmost = int(item->x());
            rightmostp = item;
            firstcheck = false;
        }
	}

    leftmostframex = leftmost;
    rightmostframex = rightmost;
    
    leftmostframe = leftmostp;
    rightmostframe = rightmostp;

    // trigger new frames if required.
    int frametoget = -1;
    int pos = 0; 
    if ( leftmostframex > int(x( )) - 2 * framewidth && leftmostframenum >= imageitem->getClip( ).startframe + zoomlevel ) 
    {    
		 frametoget = leftmostframenum - zoomlevel;
         leftmostframe -= zoomlevel;
         pos = leftmostframex - framewidth - framespacex;
         leftmostframex = pos;
         leftmostframenum = frametoget; 
    }
    else if ( rightmostframex + framewidth < ( width() - x( ) ) && rightmostframenum <= (imageitem->maxFrames() - zoomlevel) )
    {   
        frametoget = rightmostframenum + zoomlevel;
        rightmostframe += frametoget;
        pos = rightmostframex + framewidth + framespacex;
        rightmostframex = pos;
        rightmostframenum = frametoget; 
    }

    if ( frametoget >= imageitem->getClip( ).startframe  ) // ? && frametoget < imageitem->maxFrames() )
    {   
        //printf("frametoget %i \n", frametoget );
        QCanvasSprite *frame = getNewFrame( frametoget );

        frame->move( pos, imageitem->y() ); 
        frame->setZ( zframeheight );
        frame->show();
        frameholder.append( frame );
    }

    // trigger frame deletion if required.
    justdeletedframe = false;
    if ( frametoget != leftmostframenum && leftmostframex <  ( int(x()) - 3 * framewidth ) )
    {    
         delete leftmostframe;
         frameholder.remove( leftmostframe );
         leftmostframe = 0;
         leftmostframenum  += zoomlevel;
         leftmostframex += (framewidth + framespacex);
         justdeletedframe = true;
    }
    else if ( frametoget != rightmostframenum && rightmostframex > ( int(x() + width() + 3 * framewidth ) ))
    {   
         delete rightmostframe;
         frameholder.remove( rightmostframe );
         rightmostframe = 0;
         rightmostframenum -=zoomlevel;
         rightmostframex -= (framewidth + framespacex);
         justdeletedframe = true;
    }

    updateBlackStrip();
}

void FilmStrip::setVelocity ( double value )
{ 
    if ( std::fabs( value ) >= 1 )
       xvelocity = value;
    else
        xvelocity = 0.0;

    if (value > 25.0)
            xvelocity =25.0;
    else if ( value < -25 )
            xvelocity =-25.0;
}

void FilmStrip::velocityMove()
{
    //printf("velocityMove %f\n", xvelocity );

    if ( std::fabs(xvelocity) < 1.0 )
    {   xvelocity = 0.0;
        return;
    }

    moveFramesBy( xvelocity, 0.0 );

    // slow down for next time
    //xvelocity /= 1.2;
   	QTimer::singleShot( timerinterval, this, SLOT( velocityMove() ) );
}

void FilmStrip::setZ( double value )
{
    setBlackBackZ( value );

    setFramesZ( value  +1);
}

void FilmStrip::setBlackBackZ( double value )
{
    //QCanvasItem::setZ( value );
    blackstrip->setZ( value );
}

void FilmStrip::setFramesZ( double value )
{
    zframeheight = value;
    QListIterator<QCanvasItem> it( frameholder );
	
	QCanvasItem * item;
	while (( item = it.current() )) 
	{
		item->setZ( value );
		++it;
	}
}

void FilmStrip::moveBy(double dx, double dy)
{
    QCanvasRectangle::moveBy( dx, dy );
    // (NB doesnt move frames)
}


FilmFrame * FilmStrip::createFrameCanvasSprite( QImage theimage )
{
	QCanvasPixmapArray *px1 = createScaledPixmapArray( theimage); // TODO MEM LEAK?!
	return new FilmFrame( imageItem(), px1, canvas() );
}

QCanvasPixmapArray * FilmStrip::createScaledPixmapArray ( QImage theimage )
{
    QImage imgxscaled = theimage.scale( imageitem->width(), imageitem->height(), QImage::ScaleMin );

	QPixmap pm( imgxscaled ); // maybe inefficient, but we need pixmap.
	
	QValueList<QPixmap> m_list;
	m_list.append( imgxscaled );
	return new QCanvasPixmapArray( m_list ); // TODO MEM LEAK?
}

QImage FilmStrip::getFrameImage( int framenum )
{
	QImage theimage;
	assetData itemdata = imageitem->getClip();
    theimage = ourexchange->getStreamImage( itemdata, framenum, false );
	return theimage;
}

