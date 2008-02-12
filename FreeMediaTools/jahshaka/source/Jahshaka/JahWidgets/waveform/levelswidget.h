/****************************************************************************
** Form interface generated from reading ui file 'levelswidget.ui'
**
** Created: Thu 15. Sep 16:49:24 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

// WARNING! THIS FILE OUT OF SYNCH WITH UI. You can still use uic to generate layout code, 
// but there are extra functions pasted in...

#ifndef LEVELSWIDGET_H
#define LEVELSWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class LevelsPlot;
class QGroupBox;
class QCheckBox;
class QComboBox;
class QButtonGroup;
class QLabel;

#include "levelsplot.h"

class LevelsWidget : public QWidget
{
    Q_OBJECT

public:
    LevelsWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~LevelsWidget();

    LevelsPlot* levelsPlot;
    QGroupBox* groupBox1;
    QCheckBox* realTimeCheckBox;
    QComboBox* levelComboBox;
    QButtonGroup* buttonGroup1;
    QLabel* textLabel1;
    QLabel* meanLabel;
    QLabel* textLabel3;
    QLabel* medianLabel;

public slots:
    virtual void levelChooser( const QString & levelstring );
    void setRealTime( bool state ) { realTimeCheckBox->setChecked( state ); };
    void realTimeToggled( bool state ) { emit realTimeChanged( state ); }
    void setTheImage( QImage *image );

signals:
	void realTimeChanged( bool );

protected:
    QGridLayout* LevelsWidgetLayout;
    QSpacerItem* spacer1_2;
    QSpacerItem* spacer6;
    QSpacerItem* spacer1;
    QSpacerItem* spacer5;
    QHBoxLayout* groupBox1Layout;
    QSpacerItem* spacer3;
    QHBoxLayout* buttonGroup1Layout;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // LEVELSWIDGET_H
