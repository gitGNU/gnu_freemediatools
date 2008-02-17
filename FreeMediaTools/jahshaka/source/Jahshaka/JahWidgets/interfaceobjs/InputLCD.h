/* flip - pageflipper for TIFF animations
 * Copyright (C) 1999 Mark B. Allan (mba@reptilelabour.com)
 *
 * "flip" is free software; you can redistribute it and/or use
 * it and/or modify it under the terms of the "Artistic License"
 */

#ifndef InputLCD_h
#define InputLCD_h

#include <qwidget.h>

// std
#include <memory>

class IntValueFormatter;
class QLCDNumber;

/**
 * LCD display
 */
//====================================================================
class InputLCD : public QWidget
{
    Q_OBJECT;
	
public:
    InputLCD (QWidget* parent = NULL, const char* name = NULL );
    virtual ~InputLCD();

    inline	int	getMinInt ()	;
    inline	int	getMaxInt ()	;
    
    void contextMenuEvent(QContextMenuEvent * e);


    // Enumerated type to specify whether a double click gravitates to 0, Min or Max
    typedef enum Default
    {
        None,
        Zero,
        Min,
        Max
    };

    // Allow default gravity to be specified
    void setDefault( Default val );

    // allow the int value to be formatted before display
    void setFormatter( const IntValueFormatter& f );

signals:
    void		valueChanged(int val);
    void		inputPressEvent( QMouseEvent * );
    void		inputPressEvent( );
    void		inputReleaseEvent( );
	
public slots:
    void		setValue	(int);
    void		setMinInt	(int val);	
    void		setMaxInt	(int val);	
    void		setMinMaxInt(int min, int max);	
    void		setActive	(bool status);
    
protected:
    bool		LCDactive;
    int			valueType;
    void 		mousePressEvent  (QMouseEvent *msEv);
    void 		mouseDoubleClickEvent  (QMouseEvent *msEv);
    void 		mouseMoveEvent ( QMouseEvent *msEv );
    void 		mouseReleaseEvent( QMouseEvent *msEv );
    
    int			lastXPos;
    
    int			val;
    int			minInt;
    int			maxInt;
    
    //for the timecode numbers we store the current value here
    int     lcdval;
    
private:
    void formatAndDisplay( int );

    Default m_default;
    std::auto_ptr< IntValueFormatter > m_formatter;

    QLCDNumber* m_lcd;
};

#endif // InputLCD_h

