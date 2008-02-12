/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef WIRE_H
#define WIRE_H

#include <qcanvas.h>

// RTTI helper
static const int wireRTTI = 850003;
class Socket;

/*!  Describes a wire on the canvas, which can be connected to either 1 or 2 Sockets.
     NB QCanvasSpline doesnt do what we need, we we derive from QCanvasRectangle and use a painter
     to draw the bezier. 
*/
class Wire : public QCanvasRectangle
{
	//Q_OBJECT  // DOES IT NEED TO BE?
		
    friend class WireupWidget;

public:
	//! Constructs a Gadget on the specified canvas. TODO should take socket ptr. See setInSocket etc.
	Wire( QCanvas * canvas );
	
	//! Destructor. 
	~Wire();

	int rtti () const { return wireRTTI; };
	
	//! We reimplement this base function so that the component items move too.
	void moveBy(double, double);

    // Sets the bezier points. This fn also updates the bounding rectangle and calls for an update().
    void setControlPoints(  QPointArray & a );

    //! Returns shallow copy of the bezier points. Usually you wont need these, see other functions for manipulating ends.
    QPointArray bezierPoints() { return pts; };

    // Sets a single bezier point. These fns also update the bounding rectangle and call for an update(), 
    // so try to avoid calling multiple times. See also setControlPoints(), so set all the points together.
    void setBezierPoint( uint index, int x, int y );
    // Overloaded form, taking a QPoint.
    void setBezierPoint( uint index, const QPoint &pt ); 

    // Moves the end point of the bezier to pt, and stretches the mid control points accordingly. First point is unchanged.
    void stretchBezierEnd( const QPoint &pt );
    void stretchBezierStart( const QPoint &pt );

    // Returns the input socket. Note, will be NULL if not connected.
    Socket* inSocket() { return _insocket; };

    // Returns the putput socket. Note, will be NULL if not connected.
    Socket* outSocket() { return _outsocket; };

    void setInSocket( Socket *sock ) { _insocket = sock; }; 
    void setOutSocket( Socket *sock ) { _outsocket = sock; }; 

    // Convenience function which returns the input socket OR the output socket, which ever is connected. 
    // If both are connected, it returns the input socket. Usually wires have at least one socket.
    Socket* connectedSocket();

    // Sets the thickness for all wires (static). Redraw required.
    void setThickness( int pixels ) { _thickness = pixels;} ;
    
    // Sets all beziers to be drawn with 3 straight lines.
    static void setSimpleLines( bool enable ) { _simplelines = enable; };

    // Override base class.
    void setZ( double value );

    static void setVerticalOrientation( bool set );
    bool verticalOrientation() const { return _orientvertical; };

protected:
	
	//! Sets up static vars etc. Called from constr.
	void init();

    // overide the base class, so we can draw the bezier curve etc.
    void drawShape( QPainter & painter );

    // We must define our bounding rectangle based on the bezier points. 
    // NB beziers dont normally pass thru the 2nd and 3rd control points, so our bounding rect will be
    // larger than needed. Its pretty inefficient anyway to use a bounding rect...
    void updateBoundingRect();

    // Sets the specified point in the cubic bezier. 0 is the start, 3 is the end. See also setControlPoints.
    // Note: these do NOT update the bounding rectangle or call an update(), unlike the public function setBezierPoint().
    void setPoint( uint index, int x, int y );
    // Overloaded form, taking a QPoint.
    void setPoint( uint index, const QPoint &pt );

    // This is only set by friend class WireupWidget, since only it knows if we are moving. 
    void setIsMoving( bool status );

    

    // TODO SOCKETS! Note, either of these may be NULL when disconnected (but both shouldnt normally be null at same time).
    Socket* _insocket; 
    Socket* _outsocket;

    // Basic properties
    // THIS IS NOT NAMED RIGHT. Is being connected, not moving.
    bool _ismoving;

    static int _thickness; 

    static bool _simplelines;

    // Sets the orientation for all wires. WireupWidget is best placed to set this.
    static bool _orientvertical;

private:

    //  array of 4 points describing the bezier.
    QPointArray pts;
    double _zprefered;

};



#endif
