/*******************************************************************************
**
** The source file for the Jahshaka editing module - qtable hack
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_TABLE_H
#define JAHNLE_TABLE_H

#include <qtable.h>

namespace jahnle
{
	class Table : public QTable
	{
		Q_OBJECT

		public:
			Table( QWidget *parent, const char *name ) :
				QTable( 0, 3, parent, name )
			{
				setColumnStretchable( 0, false );
				//setColumnReadOnly( 0, true );
				hideColumn( 2 );
				verticalHeader( )->hide( );
				horizontalHeader( )->hide( );
				setLeftMargin( 0 );
				setTopMargin( 0 );
				setFocusPolicy( QWidget::ClickFocus );
				setFocusStyle( QTable::FollowStyle );
				setHScrollBarMode( QScrollView::AlwaysOff );
				setVScrollBarMode( QScrollView::AlwaysOn );
				setSelectionMode( QTable::SingleRow );
			}

			void setRow( int row, QString name, QString value, QString type = "" )
			{
				if ( row >= numRows( ) )
					setNumRows( row + 1 );

				// Temporary abuse of type 
				if ( type == "Color:rgba" && value.find( "/" ) != -1 )
					value = value.section( '/', -1 );

				setText( row, 0, name );
				setText( row, 1, value );
				setText( row, 2, type );
				setColumnWidth( 0, width( ) / 3 );
				setColumnWidth( 1, 2 * width( ) / 3 - 25 );
			}

			QString getName( int row )
			{
				return text( row, 0 );
			}

			QString getValue( int row )
			{
				return text( row, 1 );
			}

		protected:
			void keyPressEvent( QKeyEvent *k )
			{
				if ( hasFocus( ) && k->state( ) == 0 )
					QTable::keyPressEvent( k );
				else
					k->ignore( );
			}
	};
}

#endif
