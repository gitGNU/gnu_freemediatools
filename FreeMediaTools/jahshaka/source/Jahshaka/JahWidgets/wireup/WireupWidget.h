/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef WIREUPWIDGET_H
#define WIREUPWIDGET_H

#include <qcanvas.h>
#include <qsize.h>
#include <qtimer.h>
#include <string>
#include "Gadget.h"
#include "WireupOverview.h"

#ifdef V3UI
#include "oelplayer.h"

#include "Composite.h" // in the authoring model
namespace authoringmodel = jahshaka::authoringModel;
#include "Observer.h" // in the authoring model
class authoringmodel::Observee;
#endif

// TODO namespace vmfx : jahshaka : ui ?
class Wire;

// WONT COMPILE YET #include "Composite.h"

//! This is the top level widget that displays the view of the wire up (i.e. view of a Composite project).
/*! Notes: we use the setEnabled() feature of QCanvasView to deal with mouse over and the setSelected() to 
    deal with selection (Qt does not implement either of these, its left to use to use them if we want).
*/
#ifdef V3UI // had to also comment out stuff for v2, it confuses the moc.
//class WireupWidget : public QCanvasView , public authoringmodel::Observer
#else
class WireupWidget : public QCanvasView
#endif
{
    Q_OBJECT
public:
    WireupWidget( QCanvas& canv, QWidget *parent = 0, const char* name = 0, WFlags f = 0);
#ifdef V3UI
//    WireupWidget( authoringmodel::composite_ptr composite, QCanvas& canv, QWidget *parent = 0, const char* name = 0, WFlags f = 0);
#endif

    ~WireupWidget();

#ifdef V3UI
    //! We must implement this because we are an Observer.
    void onChangeNotification( authoringmodel::observee_ptr pSubject ) throw();
#endif

    virtual QSize sizeHint () const; 
    virtual QSize minimumSizeHint() const; 
    virtual QSizePolicy sizePolicy() const;

    // Brings the item to the front. Does not do a canvas update.
    void bringToFront( QCanvasItem *item );

    //enum WireupOrientation { Vertical, Horizontal };
    //WireupOrientation wireupOrientation() const { return _orientation; };
    bool verticalOrientation() const { return _orientvertical; };
#ifdef V3UI
	void setPlayer(oel_player* player) {	_player = player;	};
#endif

signals:

    //! Emitted when item(s) dropped onto ourselves, e.g. files for opening into workspace.
    void droppedItemsOntoWireup( QStringList );

#ifdef V3UI 
    //! Emitted when any gadget is clicked.
    //void currentOperator( authoringmodel::operator_ptr  );
#endif

public slots:

    //! Various overloads and functions to create Gadgets. 
    /*! You need to call show() yourself, e.g. after moving to start position.
        See autoPositionGadget() to line it up with other gadgets.
    */
    //! TODO remove this one? no image etc.
    Gadget* createGadget();
    //! Creates a gadget with image but no filename (remove this one?)
    Gadget* createGadget( QImage &image );
    //! Creates a gadget for a 'footage operator'. 
    Gadget* createGadget( QString filename );
    //! Creates a gadget representing an effect operator. 
    Gadget* createEffectGadget( std::string name, QImage &image );

    //! Attempts to move the gadget to some sensible position. 
    void autoPositionGadget( Gadget *gadg );

    //! Removes the gadget and its sockets, disconnecting any wires associated with it.
    void removeGadget( Gadget *mygadget );
    void removeAllGadgets();

    // Sets the zoom level. See _zoomlevel. 
    void setZoomLevel( int level = 1 ); 

    void zoomIn() { setZoomLevel( _zoomlevel + 1 ); };
    void zoomOut(){ setZoomLevel ( _zoomlevel - 1 ); };

    // Sets the orientation. Note, will trigger full re-draw.
    void setVerticalOrientation( bool set );
    void toggleOrientation() { setVerticalOrientation( !_orientvertical ); };
    
    //! Deletes gadgets which are selected. canvas update may be required.
    void deleteSelectedGadgets();

protected:

    // performs basic initialization.
    void initCanvasView();

    // === MOUSE and KEYBOARD FUNCTIONS / EVENTS ======================================

    void contentsMousePressEvent( QMouseEvent* e );
    void contentsMouseReleaseEvent( QMouseEvent* e );
    void contentsMouseMoveEvent( QMouseEvent* e );
    void contentsMouseDoubleClickEvent ( QMouseEvent* e );
    void wheelEvent ( QWheelEvent * event );

    void keyPressEvent ( QKeyEvent * e );
    void keyReleaseEvent ( QKeyEvent * e );

    void contentsContextMenuEvent ( QContextMenuEvent * e );

    void resizeEvent ( QResizeEvent * e );

    // === Drag and Drop =========================================================
    void contentsDragEnterEvent ( QDragEnterEvent *e );
    void contentsDropEvent( QDropEvent *e );

    // === SELECTION and GROUPING FUNCTIONS ======================================

    // Creates rubberband (if it did not exist), and moves to the position chosen, with zero size. See stretchRubberband.
    // The rubberband is move to front. A canvas update is done triggered. 
    void makeRubberband( QPoint topleft );
    // Moves the lower right corner of the rubber band box to the given position.
    void stretchRubberband( QPoint bottomright );
    // Removes rubber band (actually hides, then does canvas update). To get selected items, use selectFromRubberband first!
    void removeRubberband();
    // Selects all Gadgets or GadgetGroup items selected within rubberband. Does not remove the rubberband.
    // Any previous selection is ignored (TODO SHIFT KEY!). Use selectedGadgets to get the list. 
    void selectFromRubberband();
    //! Selected all gadgets. Canvas update may be required to see gadgets as selected.
    void selectAllGadgets();
    // Returns list of previous selection. 
    // Items could in theory be removed since that selection, but those fns should clear selection.
    QCanvasItemList selectedGadgets(){ return _selectedgadgets; };
    void clearSelection( bool updatecanvas = true );
    void selectionMoveBy( int x, int y );
    void allGadgetsMoveBy( int x, int y );

    // Removes Wires from a canvas item list.
  //?  QCanvasItemList canvasItemListWithoutWires( QCanvasItemList alist );

private slots:

    // === SPECIAL EFFECTS SLOTS ================================================

    // Waits a period of time and then tells the socket to stop glowing.
    void triggerSocketGlow( Socket *socket);
    // Slot that ends any glowing item.
    void removeGlows(); 

    // This is triggered if the cursor is stationary for a short time, so we dont check for mouse overs
    // during steps of a mouse move.
    void mouseOverEffects();

    //! For any extra stuff we need when the view is scrolled around.
    void contentsMovingSlot( int x, int y );
     
private:
  //WONT COMPILE YET  Composite *_composite;

    static const int _numberofzoomlevels = 11;
    double _zoomlevels[_numberofzoomlevels];

    double _zoom; // the actual zoom.
    int _zoomlevel; // entry in _zoomlevels

    double _front; // frontmost z value of canvas items.

    QPoint _movingstartpt;
    QCanvasItem* _movingitem; // Gadget* only?

    bool _overviewismoving;

    QPoint _translateviewstartpt; 
    QPoint _scaleviewstartpt;
    QPoint _lastclickpt;
 
    QCanvasItemList _selectedgadgets;
 
    Socket *_socketbeingwired; // Canvasitem* enough?
    QPoint _wirestartpt;  // needed? or use socket pos?
    Wire * _movingwire;
    QPoint _rubberbandstartpt; // for selection
    
    // To keep redraws to a minimum we use 4 lines for selection, not a rectangle.
    // Note, Qt4 has a QRubberBand class.
    QCanvasLine *_rubberbandline1;
    QCanvasLine *_rubberbandline2;
    QCanvasLine *_rubberbandline3;
    QCanvasLine *_rubberbandline4;

  //  WireupOrientation _orientation;
    static bool _orientvertical; 
    Gadget* _clickedgadget;
    QCanvasItem* _mouseoveritem;

    // remembers key states. see keyPressEvent and keyReleaseEvent
    bool _shiftkeydown;
    bool _controlkeydown;

    // for special effects
    Socket* _glowingsocket;
    QTimer _mouseovertimer;
    bool _mouseoversenabled;

    // The overview of the wireup. 
    WireUpOverview *_overview;

    // The actual authoring model data we are representing.
#ifdef V3UI
    authoringmodel::composite_ptr _compositemodel;

	oel_player* _player;
#endif
   
};

#endif
