/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2005 Visual Media Fx Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "GadgetGroup.h"

// Incremental counter of all gadgets.
//int Gadget::gadget_counter = 0; // WHAT ABOUT COUNTER ?

GadgetGroup::GadgetGroup( QCanvas *canvas ) : QCanvasRectangle(canvas)
{
	init(); 	
}

void GadgetGroup::init()
{	
    // TODO.
	show();

}

GadgetGroup::~GadgetGroup()
{
	// QtCanvas deletes the canvas items for us -- 
	// we only need to worry about extra memory we have used.

    // TODO Deal with gadgets in our group.
}




