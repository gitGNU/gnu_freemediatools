/*******************************************************************************
**
** The source file for the Jahshaka jahcalc module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahcalc.h"

#include <qvariant.h>
#include <qframe.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvalidator.h>

/* 
 *  Constructs a JahCalc as a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
JahCalc::JahCalc( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )

{
    if ( !name ) setName( "JahCalc" );
    
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 235, 225 ) );
    setMaximumSize( QSize( 235, 225 ) );

    frame3 = new QFrame( this, "frame3" );
    frame3->setGeometry( QRect( 0, 0, 235, 225 ) );
    frame3->setMinimumSize( QSize( 235, 225 ) );
    frame3->setMaximumSize( QSize( 235, 225 ) );
    frame3->setFrameShape( QFrame::StyledPanel );
    frame3->setFrameShadow( QFrame::Raised );

    //the numbers
    calc0 = new QPushButton( frame3, "calc0" );
    calc0->setGeometry( QRect( 70, 155, 50, 30 ) );
    connect( calc0, SIGNAL( clicked() ), this, SLOT( select0() ) );

    calc1 = new QPushButton( frame3, "calc1" );
    calc1->setGeometry( QRect( 10, 120, 50, 30 ) );
    connect( calc1, SIGNAL( clicked() ), this, SLOT( select1() ) );

    calc2 = new QPushButton( frame3, "calc2" );
    calc2->setGeometry( QRect( 70, 120, 50, 30 ) );
    connect( calc2, SIGNAL( clicked() ), this, SLOT( select2() ) );
    
    calc3 = new QPushButton( frame3, "calc3" );
    calc3->setGeometry( QRect( 130, 120, 50, 30 ) );
    connect( calc3, SIGNAL( clicked() ), this, SLOT( select3() ) );
   
    calc4 = new QPushButton( frame3, "calc4" );
    calc4->setGeometry( QRect( 10, 85, 50, 30 ) );
    connect( calc4, SIGNAL( clicked() ), this, SLOT( select4() ) );
    
    calc5 = new QPushButton( frame3, "calc5" );
    calc5->setGeometry( QRect( 70, 85, 50, 30 ) );
    connect( calc5, SIGNAL( clicked() ), this, SLOT( select5() ) );

    calc6 = new QPushButton( frame3, "calc6" );
    calc6->setGeometry( QRect( 130, 85, 50, 30 ) );
    connect( calc6, SIGNAL( clicked() ), this, SLOT( select6() ) );
    
    calc7 = new QPushButton( frame3, "calc7" );
    calc7->setGeometry( QRect( 10, 50, 50, 30 ) );
    connect( calc7, SIGNAL( clicked() ), this, SLOT( select7() ) );

    calc8 = new QPushButton( frame3, "calc8" );
    calc8->setGeometry( QRect( 70, 50, 50, 30 ) );
    connect( calc8, SIGNAL( clicked() ), this, SLOT( select8() ) );
    
    calc9 = new QPushButton( frame3, "calc9" );
    calc9->setGeometry( QRect( 130, 50, 50, 30 ) );
    connect( calc9, SIGNAL( clicked() ), this, SLOT( select9() ) );

    //the display
    calcdisplay = new QLineEdit( frame3, "calcdisplay" );
    //calcdisplay = new QSpinBox( frame3, "calcdisplay" );
    calcdisplay->setGeometry( QRect( 10, 10, 170, 31 ) );
    //calcdisplay->setValidator( new QIntValidator( calcdisplay ) );
    calcdisplay->setText( "0" );
	calcdisplay->setReadOnly( true );
    connect( calcdisplay, SIGNAL( returnPressed() ), this, SLOT( returnAccept() ) );

    //the operators
    opModulo = new QPushButton( frame3, "opModulo" );
    opModulo->setGeometry( QRect( 185, 190, 45, 30 ) );
    connect( opModulo, SIGNAL( clicked() ), this, SLOT( selectModulo() ) );

    opDivide = new QPushButton( frame3, "opDivide" );
    opDivide->setGeometry( QRect( 185, 155, 45, 30 ) );
    connect( opDivide, SIGNAL( clicked() ), this, SLOT( selectDivide() ) );

    opMultiply = new QPushButton( frame3, "opMultiply" );
    opMultiply->setGeometry( QRect( 185, 120, 45, 30 ) );
    connect( opMultiply, SIGNAL( clicked() ), this, SLOT( selectMultiply() ) );

    opSubtract = new QPushButton( frame3, "opSubtract" );
    opSubtract->setGeometry( QRect( 185, 85, 45, 30 ) );
    connect( opSubtract, SIGNAL( clicked() ), this, SLOT( selectSubtract() ) );

    opAdd = new QPushButton( frame3, "opAdd" );
    opAdd->setGeometry( QRect( 185, 50, 45, 30 ) );
    connect( opAdd, SIGNAL( clicked() ), this, SLOT( selectAdd() ) );

    calcdecimal = new QPushButton( frame3, "calcdecimal" );
    calcdecimal->setGeometry( QRect( 10, 155, 50, 30 ) );

    opequals = new QPushButton( frame3, "opequals" );
    opequals->setGeometry( QRect( 130, 155, 50, 30 ) );
    connect( opequals, SIGNAL( clicked() ), this, SLOT( selectTotal() ) );

    clear = new QPushButton( frame3, "clear" );
    clear->setGeometry( QRect( 185, 10, 45, 30 ) );
    connect( clear, SIGNAL( clicked() ),    this, SLOT( clearAll() ) );

    //return modes
    cancel = new QPushButton( frame3, "cancel" );
    cancel->setGeometry( QRect( 95, 190, 85, 30 ) );
    connect( cancel, SIGNAL( clicked() ),    this, SLOT( returnCancel() ) );

    accept = new QPushButton( frame3, "accept" );
    accept->setGeometry( QRect( 10, 190, 85, 30 ) );
    connect( accept, SIGNAL( clicked() ),    this, SLOT( returnAccept() ) );
    
    //default return is zero
    returnResult  = 0.0;
    runningTotal  = "0";
    memoryTotal   = "";
    memoryOpp     = "";
    
    languageChange();
    resize( QSize(235, 225).expandedTo(minimumSizeHint()) );
    
    //grab the keyboard focus
    calcdisplay->setFocus();
}

/*
 *  Destroys the object and frees any allocated resources
 */
JahCalc::~JahCalc()
{
    // no need to delete child widgets, Qt does it all for us
}

void JahCalc::setValue( int value )
{
	runningTotal = "";
    memoryOpp   = "";
	memoryTotal.setNum( value );
	calcdisplay->setText( memoryTotal );
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void JahCalc::languageChange()
{
    setCaption( tr( "JahCalc" ) );
    
    calc0->setText( tr( "0" ) );
	calc0->setAccel( tr( "0" ) );
    calc1->setText( tr( "1" ) );
	calc1->setAccel( tr( "1" ) );
    calc2->setText( tr( "2" ) );
	calc2->setAccel( tr( "2" ) );
    calc3->setText( tr( "3" ) );
	calc3->setAccel( tr( "3" ) );
    calc4->setText( tr( "4" ) );
	calc4->setAccel( tr( "4" ) );
    calc5->setText( tr( "5" ) );
	calc5->setAccel( tr( "5" ) );
    calc6->setText( tr( "6" ) );
	calc6->setAccel( tr( "6" ) );
    calc7->setText( tr( "7" ) );
	calc7->setAccel( tr( "7" ) );
    calc8->setText( tr( "8" ) );
	calc8->setAccel( tr( "8" ) );
    calc9->setText( tr( "9" ) );
	calc9->setAccel( tr( "9" ) );
    
    opModulo->setText( tr( "mod" ) );
    opModulo->setAccel( tr( "%" ) );
    opDivide->setText( tr( "/" ) );
    opDivide->setAccel( tr( "/" ) );
    opMultiply->setText( tr( "*" ) );
    opMultiply->setAccel( tr( "*" ) );
    opSubtract->setText( tr( "-" ) );
    opSubtract->setAccel( tr( "-" ) );
    opAdd->setText( tr( "+" ) );
    opAdd->setAccel( tr( "+" ) );
    calcdecimal->setText( tr( "." ) );
    calcdecimal->setAccel( tr( "." ) );
    opequals->setText( tr( "=" ) );
    opequals->setAccel( tr( "=" ) );
    clear->setText( tr( "C" ) );
    clear->setAccel( Qt::Key_Backspace );
    
    accept->setText( tr( "accept" ) );
	accept->setAccel( Qt::Key_Return );
    cancel->setText( tr( "cancel" ) );
	cancel->setAccel( Qt::Key_Escape );
}

void JahCalc::returnAccept( void )
{
	if ( memoryOpp != "" )
		doMath( );
	returnResult = runningTotal == "" ? memoryTotal.toInt() : runningTotal.toInt( );
	QDialog::accept();
}

void JahCalc::returnCancel( void )
{    
   //closes dialog and returns a cancel
   QDialog::reject();

}

void JahCalc::clearAll( void )
{    
   runningTotal = "";
   memoryOpp = "";
   calcdisplay->setText( memoryTotal );
}

//the numeric slots here
void JahCalc::select0( void )
{    
   runningTotal += "0";
   calcdisplay->setText(runningTotal);
}

void JahCalc::select1( void )
{    
   runningTotal += "1";
   calcdisplay->setText(runningTotal);
}

//the numeric slots here
void JahCalc::select2( void )
{    
   runningTotal += "2";
   calcdisplay->setText(runningTotal);
}

//the numeric slots here
void JahCalc::select3( void )
{    
   runningTotal += "3";
   calcdisplay->setText(runningTotal);
}

//the numeric slots here
void JahCalc::select4( void )
{    
   runningTotal += "4";
   calcdisplay->setText(runningTotal);
}

//the numeric slots here
void JahCalc::select5( void )
{    
   runningTotal += "5";
   calcdisplay->setText(runningTotal);
}

void JahCalc::select6( void )
{    
   runningTotal += "6";
   calcdisplay->setText(runningTotal);
}

void JahCalc::select7( void )
{    
   runningTotal += "7";
   calcdisplay->setText(runningTotal);
}

void JahCalc::select8( void )
{    
   runningTotal += "8";
   calcdisplay->setText(runningTotal);
}

void JahCalc::select9( void )
{        
   runningTotal += "9";
   calcdisplay->setText(runningTotal);
}

///////////////////////////////////////////
//the operation slots here

void JahCalc::selectAdd( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

	memoryOpp    = "+";
    runningTotal = "";
}

void JahCalc::selectSubtract( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

    memoryOpp    = "-";
    runningTotal = "";
}

void JahCalc::selectMultiply( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

    memoryOpp    = "*";
    runningTotal = "";
}

void JahCalc::selectModulo( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

    memoryOpp    = "%";
    runningTotal = "";
}

void JahCalc::selectDivide( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

    memoryOpp    = "/";
    runningTotal = "";
}

void JahCalc::selectTotal( void )
{   
    if (memoryOpp=="")
        memoryTotal  = runningTotal != "" ? runningTotal : memoryTotal;
    else
        doMath();

    memoryOpp    = "";
	runningTotal = "";
}

void JahCalc::doMath( void )
{
	if ( runningTotal == "" )
	{
        calcdisplay->setText(memoryTotal);
		return;
	}

    int total  = runningTotal.toInt();
    int mtotal = memoryTotal.toInt();
    int thetotal;

    //execute previous command   
    if (memoryOpp=="+")
    {
        thetotal = mtotal + total;
        memoryTotal = memoryTotal.setNum(thetotal);
        runningTotal = "";
        calcdisplay->setText(memoryTotal);
    } 
	else if (memoryOpp=="-")
    {
        thetotal = mtotal - total;
        memoryTotal = memoryTotal.setNum(thetotal);
        runningTotal = "";
        calcdisplay->setText(memoryTotal);
    } 
	else if (memoryOpp=="*")
    {
        thetotal = mtotal * total;
        memoryTotal = memoryTotal.setNum(thetotal);
        runningTotal = "";
        calcdisplay->setText(memoryTotal);
    } 
	else if (memoryOpp=="/")
    {
		if ( total != 0 )
		{
        	thetotal = mtotal / total;
        	memoryTotal = memoryTotal.setNum(thetotal);
        	runningTotal = "";
        	calcdisplay->setText(memoryTotal);
		}
    }
	else if (memoryOpp=="%")
    {
		if ( total != 0 )
		{
        	thetotal = mtotal % total;
        	memoryTotal = memoryTotal.setNum(thetotal);
        	runningTotal = "";
        	calcdisplay->setText(memoryTotal);
		}
    }
}
