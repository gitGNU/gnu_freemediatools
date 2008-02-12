/*******************************************************************************
**
** The header file for the Jahshaka Jah module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHPLAYER_H
#define JAHPLAYER_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include <qstring.h>
#include <qdir.h>
#include <qsettings.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QFrame;
class QSplitter;
class QWidgetStack;
class QWidget;
class QCheckBox;
class QPushButton;
class QSlider;
class QLCDNumber;

#ifdef NEWOPENOBJECTLIB
#include <GL/glew.h>
#else
#include "glew.h"
#endif

//jahshaka includes
#include "jahplayercreate.h"

//jahshaka library includes
#include "jahtracer.h"
#include "jahpreferences.h"
#include "jahtranslate.h"
#include "jahstats.h"

//openlibraries includes
#include "opencore.h"
#include "gpumathlib.h"

//for spaceball under linux
#ifdef SPACEBALL
#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <xdrvlib.h>
#endif
#endif

class Jahplayer : public QMainWindow
{
    Q_OBJECT

public:
    Jahplayer( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~Jahplayer();

    //initailizes the main interface
	bool setBasePath(void);
    void exitError(void);
  
	void checkJahExtensions(void);
    void enableJahExtensions(void);
    
	void storeGlobalPositionSize(void);
    void restoreGlobalPositionSize(void);

    void setupInterface( QWidget * );
	void setupMyCustomColors();

	void keyPressEvent( QKeyEvent *k );
	void keyReleaseEvent( QKeyEvent *k );
    void contextMenuEvent ( QContextMenuEvent * );

protected:
	bool event( QEvent *e );

public slots:

    //variable used to hide the ui
    void shrinkUI(bool value);
    void setStyleColor(QPalette pal);

signals:

    void sendchangeModeText(const QString & text);
    void sendsetProgressValue(int progress);

private:
    JahTrace			* jtrace;

	//this is the core jahshaka class
    JahControl *jahapp;

    bool dualheadOption;

public:
    QFrame* header;
    QFrame* menubox;
    QFrame* modebox;
    QFrame* statusbox;
    QSplitter* center;
    QFrame* masterworldspace;
    QFrame* lefttoolbar;
    QFrame* worldspace;

    QFrame* righttoolbar;
    QWidgetStack* controller;
    QWidget* controllermodules;
    QFrame* forceplayframe;
    QCheckBox* forceplay;
    QFrame* controllerframe;

    QWidget* controllerdesktop;
    QFrame* modeselectframe;

    QFrame* desktopbuttons;

    QFrame* desktopselectframe;
    QPushButton* desktopselectbutton;
    QFrame* modules;
    QFrame* modules_buttons;
    QFrame* modules_controls;
    QFrame* footer;
    QFrame* footer_menus;
    QFrame* footer_modes;
    QFrame* footer_status;

protected:
    QGridLayout* JahshakaLayout;
    QVBoxLayout* masterlayout;
    QHBoxLayout* headerLayout;
    QSpacerItem* menuspacer1;
    QSpacerItem* menuspacer2;
    QVBoxLayout* masterworldspaceLayout;
    QHBoxLayout* worldlayout;
    QGridLayout* worldspaceLayout;
    QHBoxLayout* controllermodulesLayout;
    QHBoxLayout* moduleslayout;

    QHBoxLayout* controllerdesktopLayout;
    QHBoxLayout* desktoplayout;

    QHBoxLayout* modeselectframeLayout;
    QHBoxLayout* desktopbuttonsLayout;
    QGridLayout* modulesLayout;
    QHBoxLayout* modules_layout;
    QHBoxLayout* footerLayout;
    QSpacerItem* footerspacer1;
    QSpacerItem* footerspacer2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // JAHSHAKA_H







