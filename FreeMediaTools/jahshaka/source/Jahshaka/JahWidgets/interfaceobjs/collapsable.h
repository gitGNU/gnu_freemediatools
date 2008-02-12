/*******************************************************************************
**
** The source file for the Jahshaka jahcontrol module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef COLLAPSABLE_H
#define COLLAPSABLE_H

// qt
#include <qframe.h>

// std
#include <memory>

class QWidget;
class QTabWidget;
class QString;

//! Provides a container with a collapse button
//! in the top right corner, to allow for the 
//! contents of the widget to be quickly hidden.
/// A basic widget which can quickly collapse and 
/// hide its contents.
/// The widget contains a layout, to which 
/// the child widgets may be added.
/// Upon pressing the collapse button, all children
/// will be hidden and the widget will reduce to a 
/// minimum size.
/// In appearance this will be like a group box, 
/// and it is possible to set a title
///
/// N.B. if a tabwidget is the immediate child of this
/// widget, the behaviour is special:
/// - the close button (if present) will become 
/// a child of the tabwidget
///
/// N.B. this widget already has a layout by default: 
/// a VBoxLayout. This is to allow a space at the top
///
/// N.B. the add methods must be used to add top level 
/// child widgets
class Collapsable : public QFrame
{
    Q_OBJECT

public:
    Collapsable( QWidget* p, const char* name = 0 );
    ~Collapsable();

    void add( QWidget* );
    void add( QTabWidget* );
    void setTitle( const QString& );

protected:
    virtual void paintEvent( QPaintEvent* );
    virtual void enabledChange( bool oldEnabled );

private slots:
    void toggleState();

private:
    struct PrivateCollapsable;
    std::auto_ptr< PrivateCollapsable > m_d;
};

#endif // COLLAPSABLE_H
