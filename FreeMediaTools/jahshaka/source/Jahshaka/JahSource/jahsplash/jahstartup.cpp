/*******************************************************************************
**
** The source file for the Jahshaka startup module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahstartup.h"

//This class creates and deletes the startup splash screen
//this will be the user-logon and profile manager later on

//the most important part is the getGraphicsInfo() routine
//as it gets GL info and sends back to the main app for startup
//so this is not just pretty - its functional

#include <qlayout.h>
#include <qlabel.h>
#include <qprogressbar.h>
#include <qapplication.h>

startup::startup( )
#ifdef DEBUG
: QDialog( 0, "Splash", false, Qt::WStyle_Customize | Qt::WStyle_Splash  ) //cant be on top for debugging
#else
: QDialog( 0, "Splash", false, Qt::WStyle_Customize | Qt::WStyle_Splash | Qt::WStyle_StaysOnTop )
#endif
  ,
  version(0),
  vendor(0),
  renderer(0),
  extensions(0),
  glsplash(0),
  plabel(0),
  pbar(0)
  
{

	//should be smaller was 300x200 for the gl intro alone
	//either resize images or resize this object to fiot...
	setGeometry ( 0, 0, 422, 312 );

	QFrame * startupframe = new QFrame (this);

	startupframe->setFrameStyle( QFrame::Box | QFrame::Plain );
	startupframe->setLineWidth( 1 );
	startupframe->setGeometry ( 0, 0, 422, 312 );       //42 extra

	QVBoxLayout *top = new QVBoxLayout( startupframe, 10 );
	QHBoxLayout *hb = new QHBoxLayout();
	top->addLayout( hb, 5 );
	hb->addSpacing( 10 );

	// Add gl frame
	QLabel *l = new QLabel( startupframe );
	l->setGeometry ( 1, 1, 420, 270 );

	// Create an OpenGL widget in splashscreen
	glsplash = new GLTexobj( l, "glbox1" );

	connect( glsplash,  SIGNAL(introFinished()), this, SLOT(introIsFinished())  );
	glsplash->setGeometry ( 1, 1, 420, 270 );

	// add progressbar and text info
	QFrame* theframe = new QFrame (startupframe);
	theframe->setGeometry ( 1, 275, 420, 35 );

	hb = new QHBoxLayout( theframe );
	top->addWidget( theframe );

	// Add text label
	plabel = new QLabel( theframe );
	plabel->setAlignment( QLabel::AlignLeft );
	plabel->setGeometry ( 5, 0, 65, fontMetrics().height()+6 );
	plabel->setFixedSize( 65, fontMetrics().height()+6 );
	plabel->setText( "Loading" );
	hb->addWidget( plabel );

	// Add progress bar
	pbar = new JahProgressBar( 1, theframe );
	pbar->setFixedHeight( fontMetrics().height()+6 );
	pbar->setGeometry ( 80, 0, 345, fontMetrics().height()+6 );
	pbar->setFixedSize( 345, fontMetrics().height()+6 );
	pbar->setProgress( 0 );
	hb->addWidget( pbar );

	top->activate();

	//position it on the screen
	QRect qRect(QApplication::desktop()->screenGeometry());
	int iXpos=qRect.width()/2-this->width()/2;
	int iYpos=qRect.height()/2-this->height()/2;
	this->move(iXpos,iYpos);

	show(); raise();
	repaint( true );

	//wait until opengl intro is finished
	inIntro = true;

	while (inIntro) 
	{ 
	    qApp->processEvents(); 
	}

	//get graphics card info and return
	getGraphicsInfo();

}

startup::~startup()
{
  delete version;
  delete vendor;
  delete renderer;
  delete extensions;
  delete glsplash;
  delete plabel;
  delete pbar ;
}

void startup::updateProgressValue( ) 
{ 
    pbar->setProgress( pbar->progress( ) + 1 ); 
    qApp->processEvents();
}

void startup::addProgressSteps( int v ) 
{ 
    pbar->setTotalSteps( pbar->totalSteps( ) + v ); 
    qApp->processEvents();
}

void startup::setProgressText( const QString &s )
{
    plabel->setText( s );
    qApp->processEvents();
}

void startup::getGraphicsInfo(void)
{
  //get the graphics card info so we can send it back to the core
  vendor     = new QString (*glsplash->vendor);
  renderer   = new QString (*glsplash->renderer);
  version    = new QString (*glsplash->version);
  extensions = new QString (*glsplash->extensions);

}
