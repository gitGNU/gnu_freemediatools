/*******************************************************************************
**
** The header file for the Jahshaka spaceball module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

//main loader modified to accept spacemouse events
//later on we will add wacom support here

//libraries for spaceball under linux
#ifdef Q_WS_X11
#ifndef __sgi
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <xdrvlib.h>
#endif
#endif

//we override the qApp x11eventfilter in order to trap and handle
//the spaceball events and pass them on to the core module
//need to create a windows version in this class
//ie winEventFilter

class App: public QApplication {
  public:
    App(int argc, char **argv): QApplication(argc, argv) { }

#ifdef  Q_WS_X11
#ifndef __sgi
   //x11 spacemouse handler
    bool x11EventFilter(XEvent *xe) {

      bool eventval = false;

      switch (xe->type) {

           //key press events
           case 2 : {  eventval = true; break; }
           case 3 : {  eventval = true; break; }

          //we only care about these types of events!
          case ClientMessage : {

            MagellanFloatEvent MagellanEvent;
            QWidget * w = QWidget::find( (WId)(xe->xany.window) );


            switch( MagellanTranslateEvent  ( w->x11AppDisplay(), xe, &MagellanEvent, 1.0, 1.0 ) )
            {
               case MagellanInputMotionEvent : {

                        MagellanRemoveMotionEvents( w->x11AppDisplay() );

                        QCustomEvent *event = new QCustomEvent(1001);
                        event->setData(new MagellanFloatEvent(MagellanEvent));

                        QApplication::postEvent(w, event);
                        QApplication::sendPostedEvents();

                        eventval = true; break; }

                case MagellanInputButtonPressEvent : {

                        QCustomEvent *event = new QCustomEvent(1002);
                        event->setData(new int(MagellanEvent.MagellanButton));

                        QApplication::postEvent(w, event);
                        QApplication::sendPostedEvents();

                        eventval = true; break; }

      	     }
         }
      }

  return eventval;
  }
#endif
#endif

};





