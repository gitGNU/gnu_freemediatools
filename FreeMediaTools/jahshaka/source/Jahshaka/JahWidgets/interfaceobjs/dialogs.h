/*******************************************************************************
**
** The header file for the Jahshaka dialogs file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DIALOGS_H
#define DIALOGS_H

#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qstring.h>

#include <qimage.h>
#include <qstring.h>

class FancyPopup : public QLabel
{
    Q_OBJECT
public:
    FancyPopup( QWidget* parent =0, const char*  name=0, int width=100, int height=100 );

    void popup( QWidget* parent = 0, QString oldname = 0);
    void popup( int x_offset, int y_offset, QWidget* parent, QString oldname = 0);

protected:
    virtual void mouseMoveEvent( QMouseEvent * );
    virtual void mouseReleaseEvent( QMouseEvent * );
    virtual void closeEvent( QCloseEvent * );

signals:
    void    returnText(QString textval);

private:
    QWidget* popupParent;
    int moves;
    QString textval,returnval;
    QPushButton* tmpB;
    QLineEdit* tmpE;

};


#endif


