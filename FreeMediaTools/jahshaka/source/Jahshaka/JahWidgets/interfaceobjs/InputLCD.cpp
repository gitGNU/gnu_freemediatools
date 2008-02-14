/* flip - pageflipper for TIFF animations
 * Copyright (C) 1999 Mark B. Allan (mba@reptilelabour.com)
 *
 * "flip" is free software; you can redistribute it and/or use
 * it and/or modify it under the terms of the "Artistic License"
 */

#include "InputLCD.h"
#include "jahcalc.h"
#include "valueFormatters.h"

#include <qdatetime.h>
#include <qlcdnumber.h>
#include <qlayout.h>

#include <string>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string>

InputLCD::InputLCD (QWidget* parent, const char* name )
    : QWidget( parent, name ),
      lcdval( 0 ),
      m_default( Zero )
{
    setCursor (sizeHorCursor);

    m_lcd = new QLCDNumber( 4, this );
    m_lcd->setMode( QLCDNumber::DEC );
    m_lcd->setLineWidth( 1 );
    m_lcd->setSegmentStyle( QLCDNumber::Flat );  

    QBoxLayout* l = new QVBoxLayout( this );
    l->addWidget( m_lcd );

    minInt		= -(INT_MAX-1);
    maxInt		=  (INT_MAX-1);
    LCDactive	= FALSE;

    formatAndDisplay( lcdval );
}

InputLCD::~InputLCD()
{}

void InputLCD::setFormatter( const IntValueFormatter& f )
{
    m_formatter.reset( f.clone() );
    
    // hack, to make timecodes work properly
    m_lcd->setNumDigits( 12 );

    formatAndDisplay( lcdval );
}

//----------------------------------------------------------------
void	InputLCD::setActive (bool status)
{
#ifndef JAHPLAYER
    LCDactive = status;
    if (LCDactive == FALSE)
    {
        formatAndDisplay( 0 );
    }
#endif
}	


//----------------------------------------------------------------
void InputLCD::setValue( int val )
{
  /*
TODO:
Conditional jump or move depends on uninitialised value(s)
   at 0x821665E: InputLCD::setValue(int) (InputLCD.cpp:74)
   by 0x8196721: GLEdit::buildInterface(QHBox*) (editinterface.cpp:236)
   m_nAStartFrame was not initialized
   by 0x8194601: GLEdit::GLEdit(GLCore*, QHBox*, QHBox*, char const*, QHBox*, QHBox*, QHBox*, QHBox*, int*) (edit.cpp:142)
   by 0x81F0DDD: JahControl::CreateEditingModule() (jahmodules.cpp:363)
   by 0x81E4044: JahControl::JahControl(QGLWidget*, QWidget*, JahHeader*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QWidget*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*, QFrame*) (jahcreate.cpp:308)
   by 0x81FFB2F: Jahshaka::Jahshaka(QWidget*, char const*, unsigned) (jahshaka.cpp:106)
   by 0x820053F: main (main.cpp:166)

*/
    LCDactive	= TRUE;
    
    //we turned off error checking here...
    //CY: and I turned it back on (and it's not 'error' - it's 'range'... sigh...)
    if ( val < minInt )	val = minInt;
    if ( val > maxInt )	val = maxInt;
    
    formatAndDisplay( val ); 
    lcdval = val;
}
//----------------------------------------------------------------
void	InputLCD::setMinInt (int val)
{
    minInt = val;
}
void	InputLCD::setMaxInt (int val)
{
    maxInt = val;
}

void	InputLCD::setMinMaxInt (int min, int max)
{
    minInt = min;
    maxInt = max;
}

//----------------------------------------------------------------
void InputLCD::mousePressEvent ( QMouseEvent *msEv )
{   
    emit inputPressEvent( );
    emit inputPressEvent( msEv );

    //ButtonState QMouseEvent::button () const
    if (msEv->button()== LeftButton)
    {
        lastXPos	= msEv->x();
    }
    else if (msEv->button()== RightButton)
    {
          
        int result;

        JahCalc *  thecalc = new JahCalc (this,"jahcalc",TRUE);
		thecalc->setValue( lcdval );

        if ( thecalc->exec() == QDialog::Accepted ) 
        {
            result = (int)thecalc->returnResult;

			lcdval = result;

            // The planners dreams gone wrong.. sigh... 
            // This is an abuse of the default handling but it keeps
            // compatability with the previous broken behaviour
            switch( m_default )
            {
            case Zero:
                emit valueChanged( result );
                formatAndDisplay( result );
                break;
                
            default:
                emit valueChanged( result );
                break;
            }
        }
        
        delete thecalc;
    }
}

void InputLCD::mouseDoubleClickEvent ( QMouseEvent * )
{ 
	switch( m_default )
	{
		case None:
			break;

		case Zero:
			// This is odd... why is there a display?
  			emit valueChanged( 0 );
			formatAndDisplay( 0 );
			break;

		case Min:
			emit valueChanged( minInt );
			break;

		case Max:
			emit valueChanged( maxInt );
			break;
	}
}


//----------------------------------------------------------------
void InputLCD::mouseMoveEvent ( QMouseEvent *msEv )
{   

    int tempInt;
    int newInt;

    if (LCDactive)
    { 
        tempInt = msEv->x()-lastXPos;
        
        switch (msEv->state())
        {
            case (LeftButton):
                break;
            case (LeftButton | ControlButton) :
                tempInt *= 10;
                break;
            default :
				break;
        }

        

        int thecurrentvalue;
        thecurrentvalue=lcdval;

        newInt = thecurrentvalue + tempInt;

        if (thecurrentvalue != newInt )
        {
            // Set the value according to the range
            setValue( newInt );
            
            // emit the value that is actually used
            emit valueChanged( lcdval );
        }
    
    }
    
    lastXPos = msEv->x();
}

void InputLCD::mouseReleaseEvent ( QMouseEvent * )
{   
	emit inputReleaseEvent( );
}

void InputLCD::formatAndDisplay( int v )
{
    QString s = QString::number( v );
    if ( m_formatter.get() )
    {
        s = m_formatter->convert( v );
    }
    
    m_lcd->display( s );
}
