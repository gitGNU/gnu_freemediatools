/*******************************************************************************
**
** The header file for the Jahshaka jahcalc module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef FORM1_H
#define FORM1_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QFrame;
class QLineEdit;
class QPushButton;

class JahCalc : public QDialog
{
    Q_OBJECT

public:
    JahCalc( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~JahCalc();

    float returnResult;
	void setValue( int value );
	void setFPS( int value );

protected slots:
    virtual void languageChange();
    
    void returnAccept( void );
    void returnCancel( void );
    void clearAll( void );
    
    void select0( void );
    void select1( void );
    void select2( void );
    void select3( void );
    void select4( void );
    void select5( void );
    void select6( void );
    void select7( void );
    void select8( void );
    void select9( void );
    
    void selectAdd( void );
    void selectSubtract( void );
    void selectMultiply( void );
    void selectModulo( void );
    void selectDivide( void );
    void selectTotal( void );
    void doMath( void );

private:
	int m_fps;
    QString runningTotal;

    QString memoryTotal;
    QString memoryOpp;

    QFrame* frame3;
    
    QPushButton* calc0;
    QPushButton* calc1;
    QPushButton* calc2;
    QPushButton* calc3;
    QPushButton* calc4;
    QPushButton* calc5;
    QPushButton* calc6;
    QPushButton* calc7;
    QPushButton* calc8;
    QPushButton* calc9;
    
    QLineEdit* calcdisplay;
    
    QPushButton* opModulo;
    QPushButton* opDivide;
    QPushButton* opMultiply;
    QPushButton* opSubtract;
    QPushButton* opAdd;
    QPushButton* calcdecimal;
    QPushButton* opequals;
    QPushButton* clear;
    
    QPushButton* accept;
    QPushButton* cancel;
};

#endif // FORM1_H
