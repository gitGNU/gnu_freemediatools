/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GADGET_H
#define GADGET_H

#include <qcanvas.h>
#include <qpointarray.h>
#include <qstring.h>
#include <qimage.h>
#include <qpixmap.h>

class WireupWidget;
#include "Socket.h"
#include "Wire.h"

#ifdef V3UI  // had to also comment out stuff for v2, it confuses the moc.
/*#include "Operator.h" // in the authoring model
#include "FootageOperator.h" // in the authoring model
#include "EffectOperator.h" // in the authoring model
#include "PropertyBag.h" // in the authoring model
#include "Property.h" // in the authoring model
namespace authoringmodel = jahshaka::authoringModel;
#include "Observer.h" // in the authoring model
class authoringmodel::Observee;
*/
#endif

// RTTI helper
static const int gadgetRTTI = 850001;


/*!  Describes a 'gadget' on the canvas, which represents an operator, ie an effect or a piece of footage etc.
    Gadgets have InputSockets and OutputSockets, which can be connected using Wires to other Gadgets' Sockets.
    Note: you need to call show() after creating a Gadget.
*/
#ifdef V3UI
//class Gadget : public QCanvasRectangle, public authoringmodel::Observer
#endif

class Gadget : public QCanvasRectangle
{
    friend class WireupWidget;  // it needs to set our state, when moving etc. Dont want those public.

public:
	// Constructs a Gadget on the specified canvas.
#ifdef V3
//	Gadget( QCanvas * canvas, authoringmodel::operator_ptr op = authoringmodel::operator_ptr() );
//    Gadget( QCanvas * canvas, QImage &image, authoringmodel::operator_ptr op = authoringmodel::operator_ptr() );
#else
    Gadget( QCanvas * canvas );
    Gadget( QCanvas * canvas, QImage &image );
#endif
	
	// TODO reimplement base hide(), since we have multiple items.
	// TODO void hide();
	
	// Destructor. 
	~Gadget();

    enum GADGET_TYPE { DEFAULT_GADGET, EFFECT_GADGET, WIDE_GADGET, CIRCULAR_GADGET };
    enum GADGET_DRAW_LEVEL { DRAW_NORMAL, DRAW_NOBOX, DRAW_NOIMAGE, DRAW_MINIMAL };
    
    //! Returns the type of gadget we are.
    GADGET_TYPE type() { return _gadgettype; };
    //! Sets the gadget type. Forces repaint.
    void setGadgetType( GADGET_TYPE type );
    //! Returns the draw level.
    GADGET_DRAW_LEVEL drawLevel() { return _drawlevel; };
    //! Sets the draw level type. Forces repaint.
    void setGadgetDrawLevel( GADGET_DRAW_LEVEL level );

    // We must implement this because we are an Observer.
#ifdef V3UI
//    void onChangeNotification( authoringmodel::observee_ptr pSubject ) throw();
#endif
    static int gadget_counter;
	int rtti () const { return gadgetRTTI; };
	
	// We reimplement this base function so that the component items move too.
	void moveBy(double, double);
    void move( double, double );


    void setOriginalImage( QImage &img ); 
    void setOriginalImage( QString filename );





    void setTitleText( QString text );
    QString titleText() { return _titletext; };

    //! Sets the main color of the gadgets bg box. Overides a previous setting caused by choosing gadget type.
    /*! Triggers a canvas update, if the 'update' argument is true.
    */
    void setBoxColor( QColor color, bool update = true );

    // Override base class, so we can deal with other items we own.
    void setZ( double value );
    // Overide base class, so we can deal with redrawing ourselves after resize. Does not trigger canvas update.
    /*! Although public, you should not normally want to call this since the size and shape depend on zoom level etc,
        and updateSize() and zoomChangedRedraw().
    */
    void setSize ( int w, int h );
    // If the zoom level changes, we need to know to draw and reset our size.
    void zoomChangedRedraw();

    // Override canvasitem base class.
    void setSelected( bool yet );
    bool isSelected();

    // Sets the zoom level for *all* gadgets. Does not trigger repaint, external fn should do that. 
    //void setZoom( double level = 1.0 ); 

    // Adds a socket to the gadget.
    Socket* addSocket( Socket::SOCKET_TYPE type );
    Socket* addOutputSocket(); 
    Socket* addInputSocket(); 

    //! Removes the socket and any wires. Tells other sockets about the change, if they were connected to the same wire.
    void removeSocket( Socket* sock );
    //! Removes all sockets. See also removeSocket().
    void removeAllSockets();

    //! Returns shallow copy of the socket lists. You can perform most operations without accessing the list.
    QList<Socket> outputSockets() { return _outputsockets; };
    QList<Socket> inputSockets() { return _inputsockets; };

    bool isMoving() { return _ismoving; };
    bool isEnabled() { return _isenabled; };
    bool isLocked() { return _islocked; };
    bool isActive() { return _isactive; }; // needed?
    bool hasMouseOver() { return _hasmouseover; };  // NOT IMPLEMENTED YET. NOT NEEDED? now using setEnabled.

    // Used for mouse over. We reimpl this because we want the sockets to have mouse over effect along with us.
    void setEnabled( bool yes );

    static void setVerticalOrientation( bool set );
    bool verticalOrientation() const { return _orientvertical; };

    // Returns a suitable font size based on the gadget zoom. The title text is drawn at this size.
    int suitableFontSize();
	
    //! Returns pointer to the actual authoring model data we represent.
    /*! TODO too ugly?
    */
#ifdef V3UI
//    authoringmodel::operator_ptr authoringOperator() { return _authoringoperator; };
#endif

    //! Returns a reference to the authoring model property bag for the operator we are representing.
#ifdef V3UI
//    authoringmodel::propertybag_ptr propertyBag() { return _authoringoperator->propertyBag(); };
#endif

    //TODO void addInEdge( EdgeItem *edge ) { inList.append( edge ); }
    // TODO void addOutEdge( EdgeItem *edge ) { outList.append( edge ); }

	//void showOutputPort();
//	void showInputPort();

	
public slots:
	/*
	//! A slot that calls hide(). NB base class is not a QObject, hence we must add new slot for this.
	void hideSlot() { hide(); };
	
	//! A slot that colors oursevles bright, or some similar 'highlighted' effect.
	void highlight(); 
	
	//! A slot that colors ourselves grey, or some similar 'disabled' effect.
	void greyOut(); 
	*/

    // Updates our info in the authoring model's property bag for the Operator we represent.
#ifdef V3UI
//    void updatePropertyBag();
#endif

signals:
	
	
protected:
	
	//! Sets up static vars etc. Called from constr.
	void init();
    // Append extra data to the authoring model's property bag for the Operator we represent.
#ifdef V3UI 
    void initPropertyBag();
#endif


    // overide the base class, so we can draw some extra stuff.
    void drawShape( QPainter & painter );

    //! Updates the gadget size based on current zoom, oritentation, type and draw level. Does not trigger redraw.
    void updateSize();

    
    
    bool _mainpixmapupdaterequired; 
   // TO REMOVE void generateMainPixmap();
    void generateMainPixmaps();
    QPixmap* _mainpixmaps; // NEW array of main pixmaps

    bool _fadedpixmapupdaterequired; // faded image created on demand on first click. After resize, this will be 'true'. 
    
    // Reduces the alpha channel to create a faded pixmap. OK is set to true if succeeded. 
    // Will fail if the image it low colour depth, or if the alpha channel cant be processed for some reason.
    QPixmap generateFadedPixmap( QPixmap &pm, bool &ok, int alpha = 128 );
    QPixmap generateFadedPixmap( QImage &img, bool &ok, int alpha = 128 );
      
   // static bool _boxpixmapupdaterequired; // will trigger update of shared box pixmap.    
    // called on demand, if a new bg pixmap is needed. TODO caching of sizes/styles?

    static QPixmap* _boxpixmaps; // NEW array of box pixmaps

    // Aligns socket to gadget. Used when sockets are created or have to be moved due to gadget size change.
    void alignSocket( Socket* sock );
	
	// Keep a list of all other canvas items that we own.
	// We need this for various reasons, e.g. to drag them around or delete them.
	QList<QCanvasItem> _items; // USED?
    QList<Socket> _inputsockets; // NEW
    QList<Socket> _outputsockets; // NEW

    static QPixmap * _boxpixmap; 
    QImage _originalimage; // large size image, from which all scaled versions are generated.
    QPixmap _mainpixmap; // the main image to show in our gadget.
    QPixmap _mainpixmapfaded; // a faded copy of the main image, created on demand (first click).

    // This is only set by friend class WireupWidget, since only it knows if we are moving. 
    void setIsMoving( bool status ) { _ismoving = status; } ; 

    // === Relation to the authoring model. TODO should we just observer? NO, we need to set the property bag.
#ifdef V3UI
//    authoringmodel::operator_ptr _authoringoperator;
#endif

    // === Basic properties ===========================================================

    int _basez;
    static double _gadgetzoom;
    static int _gadgetzoomlevel;
    static double _defaultimagexborder; // default borders
    static double _defaultimageyborder; 
    double _imagexborder; // our border between central image and outermost edge
    double _imageyborder;
    static double _defaultminimalheight;

    QString _titletext;
    bool _islocked; // cant be deleted or moved if this set. Can be grouped?
    bool _isenabled; 
    bool _hasmouseover;
    bool _ismoving;
    bool _isselected;
    bool _isactive; // needed? i.e. selected, but could be active as part of a group.
    static const int _defaultwidth;
    static const int _defaultheight;

    // Sets the orientation for all gadgets. NB WireupWidget is best placed to set this. 
    static bool _orientvertical;

    // This is duplicate of whats in WireupWidget, to simple way to share it?
    static const int _numberofzoomlevels = 11;
    double _zoomlevels[_numberofzoomlevels];

    static QColor _selectioncolor;
    static QColor _defaultgadgetcolor;
    static QColor _effectgadgetcolor;
    static QColor _widegadgetcolor;
    static QColor _layergadgetcolor;

    QColor _boxcolor;

    GADGET_DRAW_LEVEL _drawlevel;
    GADGET_TYPE _gadgettype;

	
};

#endif
