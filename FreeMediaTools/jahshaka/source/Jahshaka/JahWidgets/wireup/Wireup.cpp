/*******************************************************************************
**
** The source file for the Jahshaka Wireup Widget
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "Wireup.h"


#include "WireupWidget.h"
/*
#include "Wire.h"
#include "Gadget.h"
#include "Socket.h"
#include <qwmatrix.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <math.h>
*/

//struct Wire

// TODOs: too many canvas updates?? needed?
WireWidget::WireWidget(QWidget* parent, const char*, WFlags )
{
    //set the base path for relative pixmap location
    JahPrefs& jprefs = JahPrefs::getInstance();    
    JahBasePath = jprefs.getBasePath( ).data();

	nodeCounter = 0;

	//create the wireup here    
    thecanvas.resize( 2500, 2000 );

    wireup = new WireupWidget( thecanvas,parent );
    //mw.setCentralWidget(wireup);


	//initalize();

	// now display everythang
    //mw.show();
// Old. not needed?   wireup->setZoom( 0.4 );
   
    wireup->show();
	
    wireup->setHScrollBarMode( QCanvasView::AlwaysOff );
    wireup->setVScrollBarMode( QCanvasView::AlwaysOff );


}


void WireWidget::initalize(void)
{
    // make some gadgets and connect their sockets.
    Gadget *mygadge = wireup->createGadget( JahBasePath+"Pixmaps/wireup/dummy-img.png"); // "dummy",
    Gadget *mygadge2 = wireup->createGadget( JahBasePath+"Pixmaps/wireup/bg1.png"); // "bg1"
  
// V2 TO FIX	wireup->connect2gadgets(mygadge, mygadge2);

    Gadget *mygadge3 = wireup->createGadget( JahBasePath+"Pixmaps/wireup/jahlogo.png"); //"jahlogo"
	
// V2 TO FIX	wireup->connect2gadgets(mygadge2, mygadge3);

    Gadget *mygadge4 = wireup->createGadget( JahBasePath+"Pixmaps/wireup/jahlogo.png" ); //"jahlogo"
	
// V2 TO FIX	wireup->connect2gadgets(mygadge3, mygadge4);
}

////////////////////////////////////////////////////////
//jahshaka 2.0 access routines
//adds a hard coded vertical node tree
void WireWidget::addNodes(int number, QString name, QString image)
{
	if (nodeCounter<=0) //its the source image
	{
		// create a blank gadget
		Gadget *mygadge = wireup->createGadget(image); // 'name'
		_gadgetList.append(mygadge);
	}
	
	nodeCounter++;
}

void WireWidget::addNodes(int number, QString name)
{
	if (nodeCounter>0)//its a effect... so its not theprimary node and needs to be connected
	{
		// create a blank gadget
		Gadget *mygadge = wireup->createGadget(name);//  'false'
		_gadgetList.append(mygadge);

		//find the parent
		Gadget *oldgadge = _gadgetList.at(nodeCounter-1);

		//do the connection
// V2 TO FIX		wireup->connect2gadgets(oldgadge, mygadge);
	}
	
	nodeCounter++;
}


void WireWidget::updateNodes(int number, assetData asset)
{
	assetExchange exchange;

	QImage theimage = exchange.getImage(asset, 1);

	updateNodes(number, asset.filename,theimage );
}

void WireWidget::updateNodes(int number, QString name, QImage image)
{
	//update a existing gadget
	Gadget *mygadge = _gadgetList.at(number);

	mygadge->setOriginalImage(image);
	mygadge->setTitleText(name);
// Old: replace/fix?	mygadge->updateItem();
	
	wireup->canvas()->update();
	//canvas->update();

}


WireWidget::~WireWidget()
{
}


