/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DESKTOPCANVAS_H
#define DESKTOPCANVAS_H

#include <qcanvas.h>
#include <qpainter.h>
#include <qtimer.h>

#include <qapplication.h>

#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qtable.h>

#include <assetdata.h>

//#include "filmstrip.h"
//#include "imageitem.h"

class ImageItem;
class assetExchange;

class assetData;
struct sqlite3;

// RTTI for the player controls
static const int playcontrolRTTI = 980001; // used by play and stop.
static const int rewindcontrolRTTI = 980002;
static const int forwardcontrolRTTI = 980003;
static const int backcontrolRTTI = 980004;
static const int fforwardcontrolRTTI = 980005;
//const bool no_writing = FALSE;
static const int bgRTTI = 980020;


// Small set of subclasses for the player buttons, so they get their own RTTIs.
// Could use cursor position on image to work it out buttons pressed, but currently
// the player buttons are *below* the image, so we dont get click info from the main image.
class PlayControlSprite: public QCanvasSprite { 
public:

  PlayControlSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return playcontrolRTTI; }
};

class RewindControlSprite: public QCanvasSprite {
public:
  RewindControlSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return rewindcontrolRTTI; }
};

class BackControlSprite: public QCanvasSprite {
public:

  BackControlSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return backcontrolRTTI; }
};

class ForwardControlSprite: public QCanvasSprite {
public:

  ForwardControlSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return forwardcontrolRTTI; }
};

class FForwardControlSprite: public QCanvasSprite {
public:

  FForwardControlSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return fforwardcontrolRTTI; }

};

class BackgroundSprite: public QCanvasSprite {
public:
  BackgroundSprite( QCanvasPixmapArray * a, QCanvas * canvas );
  int rtti () const { return bgRTTI; }
  bool isValid ();

};

class FigureEditor : public QCanvasView {
  Q_OBJECT

public:
  FigureEditor(QCanvas*, QWidget* parent=0, const char* name=0, WFlags f=0);
  ~FigureEditor();
  void clear();
  void swapItem(ImageItem *);
  void clearItem();
  bool eraseItem(int clipnumber);
  void setItemtext(QString&);

  int getDesktopSelect();

  // returns true if the current clip is playing on the desktop.
  bool clipIsPlaying() { return clipplaying; } ;

  QString imagetext;
  ImageItem *globalitem;

  assetData selectedClip;

  //temporary workaround here as sgi dont check for infunc creation properly
  bool bordercreated;
  bool move;

  // General options can be set here
  enum PlayerControlsMode { Reveal, AlwaysShow, AlwaysHide };
  // Setting setSelectionAllowed (false) prevents any interaction at all.
  void setSelectionAllowed ( bool enable ) { selectionallowed = enable; }; 
  void setResizingAllowed ( bool enable ) { resizeallowed = enable; };
  void setMovingAllowed( bool enable ) { movingallowed = enable; };
  void setPlayerControlsMode ( FigureEditor::PlayerControlsMode mode );
  void setItemBorderEnabled ( bool enable ) { borderenabled = enable; };
  void setFilmStripAllowed( bool enable ) { filmstripallowed = enable; };
  bool filmStripAllowed() { return filmstripallowed; };

  // If an item is moved or resized externally, the selection border and text (drawn in this class)
  // wont know, so you might want to hide these if items move externally.
  void hideDecorations();

  // Shows a large background logo at the center of the canvas. Can be called again to re-center if canvas size changes.
  void showBackgroundSprite ( bool on = true );

  // brings the item to the front (z value). Doesnt deal with border or text etc, they are done when selecting only.
  // NB No canvas refresh is called. 
  void bringToFront( ImageItem* item );

  // Creates a filmstrip expanded around the current globalitem. Any previous filmstrip is deleted.
  void createFilmStrip(int stripWidth);
  void deleteFilmStrip();

public slots:

    // Slots for each of the player controls which drops down under clip items.
    void playControlClicked(); 
    void playControlStop(); 
    void playControlPlay();
    void forwardControlClicked(); 
    void fForwardControlClicked();
    void backControlClicked();
    void rewindControlClicked();

  // Advances the clip by 'dframe' frames, or continuously if 'keepplaying' is set.
  /*! Note, for now the slot triggers itself every 100ms --
    its not supposed to be an embedded movie player, just a small preview. 
  */
  void advanceClipFrame( int dframe = +1 , bool keepplaying = true );

  // Reveals the player controls below the current 'globalitem' (assumes globalitem is valid imageitem).
  /*! This slot calls itself until the controls are in the final position. The controls are
      still useable whilst they animate. SEE ALSO playercontrolsmode and setPlayerControlsMode to change behaviour.
  */
  void revealPlayerControls();

  // Moves the player controls on the canvas (NB play button is in centre when using MoveTo).
  void playerControlsMoveBy( int x, int y);
  void playerControlsMoveTo( int x, int y);

  // Shows or hides the player controls. Send show=false to hide.
  /*! Note, the playercontrolsmode overides any setting for 'show' here. See setPlayerControlsMode.
  */
  void showHidePlayerControls( bool show = true );

protected slots:

    // Used to control mouse inertia, e.g. for pulling filmstrip.
    void mouseStoppedSlot();

protected:
  void contentsMousePressEvent(QMouseEvent*);
  void contentsMouseReleaseEvent(QMouseEvent*);
  void contentsMouseMoveEvent(QMouseEvent*);
  void contentsMouseDoubleClickEvent ( QMouseEvent*e );

  // We overide resize event, to do some extra things on resize
  void resizeEvent( QResizeEvent * e );

  // Convenience function, since we do quite a few conversions from QImage to pixmaparray, to make QCanvasSprites.
  QCanvasPixmapArray* createScaledPixmapArray( QImage theimage );

  // Returns a string based on the clip category. Used when setting the text on top of selected clips etc.
  QString categoryName( VideoCategory::TYPE cat );

  //! Make sure the canvas bg fits in with our current theme/palette if it changes.
  void styleChange ( QStyle & ){ if (canvas() ) canvas()->setBackgroundColor( qApp->palette().color( QPalette::Active, QColorGroup::Mid )); };

signals:
  void status(const QString&);
  void updateClipPosition(assetData data);
  void updateClipSize(assetData data);
  void updateLockStatus(assetData data);
  void updateFilmstripStatus(assetData data);
  void signalUpdateFrameOffset( assetData );
  void activeFlagDesktop();

  void itemRenamed( int clipnumber, QString newname );
    
private:
  QCanvasItem* moving;
  QCanvasItem* scrub;
  QCanvasItem* resize;
  QPoint moving_start;
  int height;

  QCanvasRectangle  *border; // was base class, but need super-class function.
  QCanvasItem* borderframe;
  QCanvasText* itemtext;
  QCanvasText* itemtextshadow;

  // The drop down player control pixmaps.
  QCanvasPixmapArray *playerpixmap; 
  QCanvasPixmapArray *stoppixmap;
  QCanvasPixmapArray *backpixmap;
  QCanvasPixmapArray *rewindpixmap;
  QCanvasPixmapArray *forwardpixmap;
  QCanvasPixmapArray *fforwardpixmap;

  QCanvasPixmapArray *backgroundpixmap;

  // The player controls sprites.
  PlayControlSprite *playercontrols; // play and stop control are same button.
  RewindControlSprite *rewindcontrol;
  BackControlSprite *backcontrol;
  ForwardControlSprite *forwardcontrol;
  FForwardControlSprite *fforwardcontrol;

  BackgroundSprite *backgroundsprite;

  bool clipplaying;

  // general options
  bool resizeallowed;
  bool movingallowed;
  bool selectionallowed;
  bool borderenabled;
  bool filmstripallowed;
  PlayerControlsMode playercontrolsmode;

  assetExchange *ourexchange;

  QTimer mousemovetimer;

};



#endif




