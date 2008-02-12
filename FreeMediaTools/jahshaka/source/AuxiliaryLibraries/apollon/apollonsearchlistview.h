/***************************************************************************
                          ApollonSearchListView.h  -  description
                             -------------------
    begin                : Sun December 14 2003
    copyright            : (C) 2003 by Arend van Beelen
    email                : arend@auton.nl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __APOLLONSEARCHLISTVIEW_H
#define __APOLLONSEARCHLISTVIEW_H

#include "apollonlistview.h"
#include <qtooltip.h>
#include <qtimer.h>
#include <qrect.h> 

//class KActionMenu;
//class KPopupMenu;

/**
 * This class provides a listview for search results. It allows the user
 * to select which columns should be hidden/shown.
 */
class ApollonSearchListView : public ApollonListView, QToolTip
{
	Q_OBJECT

	public:
		enum Columns { columnRoot = 0, columnName, columnSize, columnAvailability, columnUser,
		               columnArtist, columnAlbum, columnTitle, columnDuration, columnMimeType,
		               columnBitrate, columnCategory, columnDirectory };

		/**
		 * Constructor.
		 */
		ApollonSearchListView(QWidget *parent);

		/**
		 * Destructor.
		 */
		~ApollonSearchListView();

		/**
		 * Restores the layout from a previous session.
		 */
		//void restoreLayout(KConfig *config, const QString &group);

		/**
		 * Sets a new width for a column.
		 */
		virtual void setColumnWidth(int column, int w);

	public slots:

	signals:

	protected:
		virtual bool eventFilter(QObject *watched, QEvent *e);
		virtual void maybeTip(const QPoint&);

	private:
//		KPopupMenu *m_headerMenu;
//		KActionMenu *m_columnVisibleAction;
		QTimer* m_wakeUpTimer;
		int m_wakeUpDelay;
		void setWakeUpDelay (int i) { m_wakeUpDelay = i; }
		QString m_tipContent;
		QRect m_rect;

	private slots:
		void toggleColumnVisible(int column);
		void slotShowTip();
};

#endif
