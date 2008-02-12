/*******************************************************************************
**
** The header file for the Jahshaka Jah module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_HEADER_H
#define JAH_HEADER_H

// qt
#include <qwidget.h>

class QMenuBar;
class QSize;

//! A simple widget to ensure the menubar is laid out correctly.
class JahHeader : public QWidget
{
    Q_OBJECT;

public:
    JahHeader( QWidget* parent, const char* name = "JahHeader" );
    
    QMenuBar* menuBar() const;
    QWidget* moduleSelectionPlaceholder() const;
    QWidget* feedbackPlaceholder() const;

private slots:
    void onMenuContentsChanged();

private:
    QMenuBar* m_menuBar;
    QWidget* m_menuPlaceholder;
    QWidget* m_moduleSelectionPlaceholder;
    QWidget* m_feedbackPlaceholder;
};

#endif // JAH_HEADER_H
