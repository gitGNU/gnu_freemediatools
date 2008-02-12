/*******************************************************************************
**
** The source file for the Jahshaka startup module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "loadingdialog.h"

//This class creates and deletes the startup splash screen
//this will be the user-logon and profile manager later on

//the most important part is the getGraphicsInfo() routine
//as it gets GL info and sends back to the main app for startup
//so this is not just pretty - its functional
/*
#include <qlayout.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qapplication.h>
*/
loadingDialog::loadingDialog( )
: QDialog( 0, "Loading", false, Qt::WStyle_Customize | Qt::WStyle_Splash )
{

  //should be smaller was 300x200 for the gl intro alone
  //either resize images or resize this object to fiot...
  setGeometry ( 0, 0, 422, 136 );

  QFrame * startupframe = new QFrame (this);

  startupframe->setFrameStyle( QFrame::Box | QFrame::Sunken );
  startupframe->setLineWidth( 3 );
  startupframe->setGeometry ( 0, 0, 422, 136 );       //42 extra

  QVBoxLayout *top = new QVBoxLayout( startupframe, 10 );
  QHBoxLayout *hb = new QHBoxLayout();
  top->addLayout( hb, 5 );
  hb->addSpacing( 10 );

  // Add gl frame
  QFrame *l = new QFrame( startupframe );
  l->setGeometry ( 1, 1, 420, 100 );

  //should put user text here
  //so they know what we are doing
  elabel1 = new QLabel( l );
  elabel1->setAlignment( QLabel::AlignLeft );
  elabel1->setFixedHeight( fontMetrics().height()+6 );
  elabel1->setGeometry ( 10, 10, 410, fontMetrics().height()+6 );

  elabel2 = new QLabel( l );
  elabel2->setAlignment( QLabel::AlignLeft );
  elabel2->setFixedHeight( fontMetrics().height()+6 );
  elabel2->setGeometry ( 10, 30, 410, fontMetrics().height()+6 );

  // add progressbar and text info
  hb = new QHBoxLayout();
  top->addLayout( hb );

  QFrame * theframe = new QFrame (startupframe);
  theframe->setGeometry ( 1, 100, 420, 35 );

  // Add text label
  plabel = new QLabel( theframe );
  plabel->setAlignment( QLabel::AlignLeft );
  plabel->setFixedHeight( fontMetrics().height()+6 );
  plabel->setGeometry ( 5, 0, 65, fontMetrics().height()+6 );
  plabel->setText( "Loading" );
  hb->addWidget( plabel );

  // Add progress bar
  pbar = new JahProgressBar( 130, theframe );
  pbar->setFixedHeight( fontMetrics().height()+6 );
  pbar->setGeometry ( 65, 0, 340, fontMetrics().height()+6 );
  pbar->setProgress( 0 );
  hb->addWidget( pbar );

  top->activate();

  //position it on the screen
  QRect qRect(QApplication::desktop()->screenGeometry());
  int iXpos=qRect.width()/2-this->width()/2;
  int iYpos=qRect.height()/2-this->height()/2;
  this->move(iXpos,iYpos);

  show(); 
  raise();
  repaint( true );

}

loadingDialog::~loadingDialog()
{
}

void loadingDialog::setProgressValue( int v ) 
{ 
    pbar->setProgress( v ); 
}

void loadingDialog::setProgressText( const QString &s )
{
  plabel->setText( s );
  //qApp->processEvents();
}

void loadingDialog::setDialog1Text( const QString &s )
{
  elabel1->setText( s );
  //qApp->processEvents();
}

void loadingDialog::setDialog2Text( const QString &s )
{
  elabel2->setText( s );
  //qApp->processEvents();
}

