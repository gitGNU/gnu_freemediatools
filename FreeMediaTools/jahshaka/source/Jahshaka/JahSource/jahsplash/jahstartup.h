/*******************************************************************************
**
** The header file for the Jahshaka startup module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef STARTUP_H
#define STARTUP_H

#include "qframe.h"
#include "qvbox.h"
#include "qlabel.h"
#include "qpixmap.h"
#include "qrect.h"
#include "qcolor.h"
#include "qtimer.h"
#include "qdialog.h"
#include <qapplication.h>

#include <qdialog.h>
#include <qpixmap.h>
#include <qstring.h>
#include <jahprogressbar.h>
#include <qlabel.h>

#include "jahstartupgl.h"

class startup : public QDialog
{
  Q_OBJECT
public:
  startup( );
  ~startup();

  GLTexobj* glsplash;

  QString * vendor, * renderer, * version, * extensions;

  bool inIntro;

  void getGraphicsInfo(void);

public slots:
  void     updateProgressValue( );
  void     addProgressSteps( int );
  void     setProgressText( const QString & );
  void     introIsFinished () { inIntro = false; }

private:
  JahProgressBar *pbar;
  QLabel       *plabel;
};




#endif


