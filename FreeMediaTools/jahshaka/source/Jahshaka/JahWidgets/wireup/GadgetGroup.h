/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2005 Visual Media Fx Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qobject.h>
#include <qcanvas.h>
class WireupWidget;
class Gadget;

// A group of gadgets. Can be collapsed or expanded.
class GadgetGroup : public QObject, public QCanvasRectangle
{
	Q_OBJECT // QOBJ INHERITANCE MAYBE NOT NEEDED!
		
    friend class WireupWidget;  // it needs to set our state, when moving etc. Dont want those public.

public:
	//! Constructs a Gadget Group on the specified canvas.
	GadgetGroup( QCanvas * canvas );

    void init();

    // TODO. These fns need to know about whats on the wire up, perhaps fn needs to be in wireup class.
    void collapse() { /* TODO */ };
    void expand() { /* TODO */ };
	
	//! TODO reimplement base hide(), since we have multiple items.
	// TODO void hide();
	
	//! Destructor. 
	~GadgetGroup();

private:

    QList< Gadget* > gadgets_;
};
