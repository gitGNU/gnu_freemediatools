/*******************************************************************************
**
** The source file for the Jahshaka dialogs module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "dialogs.h"

// This class is used for the popup input windows

#include <qapplication.h>
#include <qkeycode.h>
#include <qlayout.h>

FancyPopup::FancyPopup( QWidget* parent, const char*  name, int width, int height ):
    QLabel( parent, name, WType_Popup ){
        setFrameStyle( WinPanel|Raised );
        setAlignment( AlignCenter );
        resize(width,height);
        moves = 0;
        setMouseTracking( TRUE );

    textval=name;

}

void FancyPopup::mouseMoveEvent( QMouseEvent * )
{
/*
    moves++;
    QString s;
    s.sprintf("%d/%d", e->pos().x(), e->pos().y());
    if (e->state() & QMouseEvent::LeftButton)
        s += " (down)";
    setText(s);*/
}

void FancyPopup::mouseReleaseEvent( QMouseEvent * e)
{
    if  (rect().contains( e->pos() ) || moves > 5)
        close();
}

void FancyPopup::closeEvent( QCloseEvent *e )
{
    e->accept();
    moves = 0;
    if (!popupParent)
       return;

    returnval=tmpE->text();
    emit returnText(returnval);

    // remember that we (as a popup) might recieve the mouse release
    // event instead of the popupParent. This is due to the fact that
    // the popupParent popped us up in its mousePressEvent handler. To
    // avoid the button remaining in pressed state we simply send a
    // faked mouse button release event to it.
    QMouseEvent me( QEvent::MouseButtonRelease, QPoint(0,0), QPoint(0,0), QMouseEvent::LeftButton, QMouseEvent::NoButton);
    QApplication::sendEvent( popupParent, &me );
}

void 
FancyPopup::popup(int x_offset, int y_offset, QWidget* parent, QString oldname)
{
    popupParent = parent;

    setText(textval);

     tmpE = new QLineEdit( this );
	//tmpE->setFocus();
	 if (oldname.length() > 0)
	 {
		tmpE->setText(oldname);
	 }
     
	 connect( tmpE, SIGNAL( returnPressed() ), this, SLOT( close() ) );
     tmpE->setGeometry(10, 10, 230, 30);

     tmpB = new QPushButton("Accept", this );
     connect( tmpB, SIGNAL( clicked() ), this, SLOT( close() ) );
     tmpB->setGeometry(10, 90, 230, 30);

    if (popupParent)
	{
        move( popupParent->mapToGlobal( QPoint(x_offset, y_offset) ) );
	}

    show();
}

void 
FancyPopup::popup( QWidget* parent, QString oldname) 
{
    popupParent = parent;

    setText(textval);

     tmpE = new QLineEdit( this );
	//tmpE->setFocus();
	 if (oldname.length() > 0)
	 {
		tmpE->setText(oldname);
	 }
     
	 connect( tmpE, SIGNAL( returnPressed() ), this, SLOT( close() ) );
     tmpE->setGeometry(10, 10, 230, 30);

     tmpB = new QPushButton("Accept", this );
     connect( tmpB, SIGNAL( clicked() ), this, SLOT( close() ) );
     tmpB->setGeometry(10, 90, 230, 30);

    if (popupParent)
	{
        move( popupParent->mapToGlobal( popupParent->rect().bottomLeft() ) );
	}

    show();

}

