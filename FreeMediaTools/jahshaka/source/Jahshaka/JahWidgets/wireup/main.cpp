#include "WireupWidget.h"
#include "WireupCanvas.h"
#include "Gadget.h"
#include "Wire.h"

#include <qapplication.h>
#include <qmainwindow.h>
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMainWindow mw(0);
    mw.setMinimumWidth( 100 );
    mw.setMinimumHeight( 100 );
    mw.resize( 600, 600 );

	//create the wireup here
    WireupCanvas thecanvas;
    thecanvas.resize( 2500, 2000 ); 
    WireupWidget *wireup = new WireupWidget( thecanvas, &mw );
    mw.setCentralWidget(wireup);


    // make some gadgets and connect their sockets.
    Gadget *mygadge = wireup->createGadget("dummy-img.png");
    Gadget *mygadge2 = wireup->createGadget("bg1.png");
  
	wireup->connect2gadgets(mygadge, mygadge2);

    Gadget *mygadge3 = wireup->createGadget("jahlogo.png");
	
	wireup->connect2gadgets(mygadge2, mygadge3);

    Gadget *mygadge4 = wireup->createGadget("jahlogo.png");
	
	wireup->connect2gadgets(mygadge3, mygadge4);


	// now display everythang
    mw.show();
    wireup->setZoom( 0.4 );
    Wire::setSimpleLines( false );
    wireup->show();

    // As usual in Qt apps, make sure we close down when the last window closes.
    QObject::connect( qApp, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()) );

    return app.exec();
}
