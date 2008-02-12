/*******************************************************************************
**
** The source file for the Jahshaka editing module - page abstraction
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_PAGE_H
#define JAHNLE_PAGE_H

#include "edit.h"

namespace jahnle
{
    class Page : public QFrame
    {
        Q_OBJECT
        
    public:
        Page( QWidget *parent, GLEdit *editor ) 
            : QFrame( parent ),
              m_editor( editor )
        {
        }
        
        virtual ~Page( ) { }
        virtual void restart( ) = 0;
        virtual void refresh( ) = 0;
        virtual void setPosition( int ) = 0;
        virtual void processKeyEvent( QKeyEvent *k ) { k->ignore( ); }
		virtual bool dropAsset( assetData & ) { return false; }
        virtual bool acceptsSelection( ) { return false; }
        virtual void startSelection( double, double ) { }
        virtual void moveSelection( double, double ) { }
        virtual void setSelection( double, double ) { }
        virtual void consumerFrameShow( Mlt::Frame & ) { }
        
    protected:
        GLEdit *editor( ) { return m_editor; }
        Preview *preview( ) { return m_editor->preview( ); }
        Project *project( ) { return m_editor->project( ); }
        
    private:
        GLEdit *m_editor;
    };
}

#endif
