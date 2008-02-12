/*******************************************************************************
**
** The header file for the Jahshaka loadingdialog module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef LOADINGSTARTUP_H
#define LOADINGSTARTUP_H

#include "qdialog.h"
#include "qstring.h"
#include "qlabel.h"
#include "qframe.h"
#include "qlayout.h"
#include "qapplication.h"

#include <jahprogressbar.h>

class loadingDialog : public QDialog
{
  Q_OBJECT
public:
  loadingDialog( );
  ~loadingDialog();

  void     setProgressValue( int );
  void     setProgressText( const QString & );
  void     setDialog1Text( const QString & );
  void     setDialog2Text( const QString & );

private:
  JahProgressBar *pbar;
  QLabel       *plabel, *elabel1, *elabel2;
};




#endif


