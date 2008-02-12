#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "openmedialib.h"
#include <qcanvas.h>

class FilmStrip;
static const int imageRTTI = 984376;

class ImageItem: public QCanvasRectangle {
public:

  ImageItem( assetData full, QImage img, QCanvas *canvas, QString& clipname );
  bool operator < ( const ImageItem &other ) const { return itemdata.clipnumber < other.itemdata.clipnumber; }
  int rtti () const { return imageRTTI; }
  bool hit( const QPoint&) const;
  int Type () const { return _type; }

  assetData getClip() { return itemdata; }

  QString getText() { return name; }
  void setText(QString& newname) { name=newname;}

  // Sets the image.
  void setImage( QImage img );

  void setCurrentFrameNumber( int num ) { currentframe = num; } ;
  int currentFrameNumber() { return currentframe;} ;

  // Convenience function, returns the asset's max frames.
  int maxFrames() { return itemdata.getMaxFrames(); };

  // Use this function to resize -- it will set the diesize and image size correctly.
  void resize( int w, int h );
  
  // Overide of base class, to keep image size correct.
  void setSize( int w, int h );

  void moveBy( double dx, double dy );
  //void move( double dx, double dy );

  // Show or hide the resize handle, painted over the top of the lower-right corner.
  void enableResizeHandle( bool enable = true ) { enableresizehandle = enable; };

  // Returns the die width (can be larger than the image on the die)
  int dieWidth() { return diewidth; };
  // Returns the die height (can be larger than the image on the die)
  int dieHeight() { return dieheight; };
  // Returns the gap between the die size and the image size.
  // Note: would be nice to not need this ugly function, but the way the image is drawn is 
  // slightly odd, i.e. the die and image size in relation to the canvasitem size, the the 
  // canvasview needs it to determine the exact mouse click position.
  int dieWidthGap() { return diewidthgap; };
  int dieHeightGap() { return dieheightgap; };

  // creates the filmstrip. stripwidth hints at the approximate screen width we want to fill.
  void createFilmStrip( int stripwidth );
  void deleteFilmStrip();
  assetData* getItemData() { return &itemdata; }

  FilmStrip* filmStrip() { return filmstrip; };

  // Actually it'd be better to implement it using RTTI (dynamic_cast)
  // but jahshaka gives segfault on dynamic_cast for me ;-(
  enum {ITEM_IMAGE, ITEM_AUDIO};
protected:
  void drawShape( QPainter & );
  int _type;
private:
  QImage image;
  //QImage fullres;
  QPixmap pixmap;
  QString name;

  // since assetData does not have the concept of a 'current frame', store it here in the ImageItem.
  // [22-09-2005] assetData does now have a frameOffset.
  int currentframe;

  assetData itemdata;

  //stores the images width and height for justification
  int imagewidth, imageheight;
  //stores to die's width and
  int diewidth, dieheight;
  
  // Store initial values.
  int diewidthgap, dieheightgap;
  int unscaled_width, unscaled_height;

  bool enableresizehandle;

  FilmStrip* filmstrip;

};

#endif 
