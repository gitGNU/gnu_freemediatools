/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef SOCKET_H
#define SOCKET_H

#include <qcanvas.h>
#include <qcolor.h>

// RTTI helper
static const int socketRTTI = 850002;
class Gadget;
class Wire;

/*!  Describes a socket on the canvas, which is owned by a Gadget. Wires connect to sockets.
*/
class Socket : public QCanvasEllipse
{
	// IS IT? Q_OBJECT
		
    friend class WireupWidget;

public:

    enum SOCKET_TYPE { INPUT_SOCKET, OUTPUT_SOCKET };

	//! Constructs a Gadget on the specified canvas.
	Socket( QCanvas * canvas, SOCKET_TYPE type );
	
	//! Destructor. 
	~Socket();
    
    // Returns number of wires connected. For outputs this is 0 or more (unlimited, in theory).
    int numWiresConnected();
    // Returns number of wires connected. For inputs this is 0 or 1.
 //   int numInputWiresConnected();

    //! Returns shallow copy of the list of wires. You can perform most operations without accessing the list.
    QList<Wire> wires() { return _wires; };

    // Connect a wire, and become responsible for it (although responsibility for a
    // wire if it is connected to sockets at both ends). If the wire is already connected, no action is taken. 
    void connectToWire( Wire * wire );
    
    // Disconnects a wire. If the wire is not connected, no action is taken.
    void disconnectFromWire( Wire *wire );

    // Disconnects all wires. 
    void disconnectAllWires();
    
	int rtti () const { return socketRTTI; };
	
	//! We reimplement this base function so that the component items move too.
	void moveBy(double, double);

    // Starts the glow effect.
    void beginGlow();
    // Stops the glow effect.
    void endGlow();

    
    // Used for mouse over. We reimpl this for mouse over effects.
    void setEnabled( bool yes );

    // Override base class, so we can deal with other items we own.
    void setZ( double value );

    //! Sets the text for the label. Does not show the label automatically.
    /*! NB for efficiency, labels are created on demand when this function is first called.
    */
    void setLabelText( QString label );

    //! Returns the label text, or 0 if no label set.
    QCanvasText *label() { return _label; } ;

    SOCKET_TYPE type() { return _type; };

	
protected:
	
	//! Sets up static vars etc. Called from constr.
	void init();

    // overide the base class, so we can draw some extra stuff.
    void drawShape( QPainter & painter );

    // A socket owns a glow effect, which is a separte canvassprite outside our bounding box. TODO STATIC?
    QCanvasSprite *_glow; 

    //! Label, shown on mouse over etc. Initialised on demand if setLabelText is called.
    QCanvasText *_label;

    // Gadget we are stuck to. Can only be one!
    //Gadget *_mygadget; // IS IT REALLY NEEDED? SOCKETS KNOW GADGETS

   // Wire* _wire; // TODO in/out/ list.

    QList<Wire> _wires;
   // QList<Wire> _inputwires; 
   // QList<Wire> _outputwires;

    SOCKET_TYPE _type;

    QColor _color;

    QColor _default_input_color; 
    QColor _default_output_color;

};



#endif
