#include "imageitem.h"
#include "filmstrip.h"

#include <qpainter.h>

ImageItem::ImageItem( assetData full, QImage img, QCanvas *canvas, QString& clipname )
    : QCanvasRectangle( canvas ), 
      image(img), 
      currentframe( full.startframe + full.frameOffset )
{
    setSize( image.width() +2, image.height() +2 ); // increased by 2 to deal with die size :P
    _type = ITEM_IMAGE;

    #ifndef _WS_QWS_
    pixmap.convertFromImage(image, OrderedAlphaDither);
    #endif

    name     = clipname;
    itemdata = full;

    imagewidth  = image.width();
    imageheight = image.height();
 
    // now force die size to these values.
    dieheight = image.height() + 2;
    diewidth = image.width() + 2;
    
    diewidthgap = diewidth - width();
    dieheightgap = dieheight - height();

    unscaled_width = img.width( );
	unscaled_height = img.height( );

    enableResizeHandle( true );

    filmstrip = 0;

}

void ImageItem::deleteFilmStrip()
{
    if ( filmstrip == 0 )
        return;

    filmstrip->hide();
    filmstrip->setCanvas(0);
    delete filmstrip; 
    filmstrip = 0;
	itemdata.filmstrip = false;
}

void ImageItem::createFilmStrip( int stripwidth )
{
    // (tried to do some trickery here to avoid crashes with deleted filmstrip).
    if ( filmstrip != 0 && !filmstrip->isVisible() )
    {   filmstrip->hide();
        filmstrip->setCanvas(0);
        delete filmstrip; 
    }

    if ( filmstrip != 0 )
        return;

    filmstrip = new FilmStrip ( this, canvas(), stripwidth ); 
    //filmstrip->move( x(), y() );
    filmstrip->show();
	itemdata.filmstrip = true;
}

void ImageItem::setSize( int w, int h )
{   
	ASSERT(w > 0);
	ASSERT(h > 0);
	if((w <= 0) || (h <= 0))
		return;

    imageheight = h;
    imagewidth = w;        


    QCanvasRectangle::setSize( w, h );
}

void ImageItem::resize ( int nw, int nh )
{
    float ratio = (double)(unscaled_width) / (double)(unscaled_height);
	int w = nw;
	int h = int( w / ratio );

	if ( h > nh )
	{
		h = nh;
		w = int( nh * ratio );
	}

    if ( width() == w && height() == h )
        return;
    
    diewidth = w + diewidthgap;
    dieheight = h + dieheightgap;

    setSize( w, h );
    // Note, the image is not scaled here. Rather, we assume the calling funciton will reload it, 
    // (and re-scale it) if it wants to.
    // Note also, the border etc are drawn by the figure editor (canvasview), we cant change it from here.
    canvas()->update();
}

void ImageItem::moveBy(double dx, double dy)
{
    if ( filmStrip() != 0 )
    {    filmstrip->moveFramesBy( dx, 0 ); 
         QCanvasRectangle::moveBy( 0, dy ); // previously, did not have this line, didnt move strip as whole.
    }
    else 
        QCanvasRectangle::moveBy( dx, dy );
	
}

void ImageItem::drawShape( QPainter &p )
{
    //first figure out offset so we can center image on die
    int xoffset = 0;
    int yoffset = 0;

    if (imagewidth<diewidth)   { xoffset = ((diewidth  - imagewidth)/2);  }
    if (imageheight<dieheight) { yoffset = ((dieheight - imageheight)/2); }

    // On Qt/Embedded, we can paint a QImage as fast as a QPixmap,
    // but on other platforms, we need to use a QPixmap.
    #ifdef _WS_QWS_
    p.drawImage( int(x()), int(y()), image, 0, 0, -1, -1, OrderedAlphaDither );
    #else
    p.drawPixmap( int(x())+xoffset, int(y())+yoffset, pixmap );
    #endif

    // draw progress bar, but only if its a clip or media (not audio for now).
    if ( itemdata.theCategory == VideoCategory::CLIP || itemdata.theCategory == VideoCategory::MEDIA )
    {
        int xborder = 4;
        int yborder = 8;
        int thickness = 6; 
        p.setPen( QColor ("darkgreen")  );
        p.setBrush( Qt::green );

        int maxw = itemdata.getNumFrames();

        if ( maxw > 1 ) 
        {
            maxw--;
        }

        int current_frame_offset = currentFrameNumber() - itemdata.startframe;
        int full_width = width();
        float progress_bar_width = current_frame_offset * ( full_width - (2 * xborder) ) / maxw; 

        if ( progress_bar_width < 0.00001 ) // if weird rounding things going on.
        {
            progress_bar_width = 0.0;
        }
        else if ( progress_bar_width > ( full_width - (2*xborder) )) // shouldnt happen, but bug in asset sequence code means it can. 
        {
            progress_bar_width = full_width - (2*xborder);
        }

        p.setBrush( Qt::NoBrush );
        p.drawRect ( (int)x() + xborder, (int)y() +height() + dieHeightGap()/2 - yborder, full_width - (2*xborder), thickness  );	
        p.setBrush( Qt::green );
        p.drawRect ( (int)x() + xborder, (int)y() +height() + dieHeightGap()/2 - yborder, int( progress_bar_width ), thickness );

       // draw the frame number text. (TODO font metrics?)
#if 0
        QString txt;
        txt.setNum( currentFrameNumber() );
        p.setPen( QColor ("black")  );
        p.drawText( (int)x() + width() - 40 -1, (int)y() + height() - yborder -8 -1 , txt ); // 'shadow' 
        p.setPen( QColor ("white")  );
        p.drawText( (int)x() + width() - 40, (int)y() +height() - yborder - 8, txt );
#endif
	}

        // Draw the resize handle in the lower right of the frame. 
        // Note, we cant draw on the 'border' since thats outside our bounding rect.
    if ( enableresizehandle && !image.hasAlphaBuffer() ){ // TODO fix for non-square items with no border area :(   
        p.setPen( QColor(10,10,10));
        int handlesize = 9; 
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - handlesize -1, (int)y() + int(dieHeightGap()/2) + height() -1, (int)x() + width() + int(diewidthgap/2) -1, (int)y() + dieHeightGap()/2 + height() - handlesize -1 );
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - 2*handlesize/3 -1, (int)y() + int(dieHeightGap()/2) + height() -1, (int)x() + width() + int(diewidthgap/2) -1, (int)y() + dieHeightGap()/2 + height() - 2*handlesize/3 -1);
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - handlesize/3 -1, (int)y() + int(dieHeightGap()/2) + height() -1, (int)x() + width() + int(diewidthgap/2) -1, (int)y() + dieHeightGap()/2 + height() - handlesize/3 -1 );
        p.setPen( QColor(100,100,100));
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - handlesize , (int)y() + int(dieHeightGap()/2) + height(), (int)x() + width() + int(diewidthgap/2), (int)y() + int(dieHeightGap()/2) + height() - handlesize );
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - 2*handlesize/3, (int)y() + int(dieHeightGap()/2) + height(), (int)x() + width() + int(diewidthgap/2), (int)y() + int(dieHeightGap()/2) + height() - 2*handlesize/3 );
        p.drawLine ( (int)x() + width() + int(diewidthgap/2) - handlesize/3, (int)y() + int(dieHeightGap()/2) + height(), (int)x() + width() + int(diewidthgap/2), (int)y() + int(dieHeightGap()/2) + height() - handlesize/3 );    
    }
}

bool ImageItem::hit( const QPoint &p ) const 
{
    int ix = p.x()-int(x());
    int iy = p.y()-int(y());
    if ( !image.valid( ix , iy ) )
        return FALSE;
    QRgb pixel = image.pixel( ix, iy );
    return qAlpha( pixel ) != 0;
}

void ImageItem::setImage( QImage img )
{
	image = img;

	// scaling code same as in desktop.cpp (where item images are first loaded).
    // Maybe inefficient?
    float wratio = ((diewidth-diewidthgap)/(float)img.width() );
    float hratio = ((dieheight-dieheightgap)/(float)img.height() );
	QImage imgx;
    imgx = img.scale( int(img.width()*wratio), int(img.height()*hratio), QImage::ScaleMin );
	image = imgx;

	#ifndef _WS_QWS_
    pixmap.convertFromImage(image, OrderedAlphaDither);
    #endif

	update();
	canvas()->update();
	//debug( "in setImage" );
}
