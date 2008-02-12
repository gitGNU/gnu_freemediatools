/*******************************************************************************
**
** The header file for the Jahshaka Wireup Widget
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef WIREWIDGET_H
#define WIREWIDGET_H

#include "qwidget.h"

#include "WireupCanvas.h"
#include "Gadget.h"

#include "assetexchange.h"

#include "jahpreferences.h" //needed to find images


//////////////////////////////////////////////////////////
// a wrapper class used to backport the 3.0 wireup
// into 2.0

class WireWidget : public QWidget {

    Q_OBJECT

public:
    WireWidget(QWidget* parent=0, const char* name=0, WFlags f=0);
    ~WireWidget();

	void initalize();

	void addNodes(int number, QString name, QString image);
	void addNodes(int number, QString name);

	void updateNodes(int number, QString name, QImage image);
	void updateNodes(int number, assetData asset);

signals:

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	//JahTrace			* jtrace;

    //BAZ CHANGED WireupCanvas	*canvas;
	WireupWidget	*wireup;

	//used to find media
	QString JahBasePath;

    WireupCanvas thecanvas;

	int nodeCounter;

	QList<Gadget> _gadgetList;
};


#endif
