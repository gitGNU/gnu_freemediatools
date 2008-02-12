/***************************************************************************
                          apollonqueryview.h  -  description
                             -------------------
begin		: Dec 05 2003
copyright	: (C) 2002 by Bastiaan Naber, (C) 2002 by Robert Wittams
email		: bastiaan@ricardis.tudelft.nl
description	:

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef APOLLONQUERYVIEW_H
#define APOLLONQUERYVIEW_H

#include <qvbox.h>
#include <qdict.h>
#include <qpushbutton.h>
#include <qcursor.h>
#include <qvaluelist.h>

//#include <kiconloader.h>

#include "giftconnection.h"
//#include "ktabwidget.h"
#include "apollonlistview.h"
#include "apollonsearchlistview.h"
#include "apollonsearchviewitem.h"

//class KListView;

class QLabel;
class QListViewItem;
class QPopupMenu;
class QString;
class QStringList;
class QTabWidget;

class ApollonViewTab;
//class ApollonSearchViewItem;

class ApollonQueryView : public QVBox
{
	Q_OBJECT

	public:
		ApollonQueryView(QTabWidget *parent, int ID, int index, const QString &text, giFTConnection *gcn);
		~ApollonQueryView();

		QString  query() { return m_query; }
		int      index() { return m_index; }

	public:
		virtual void keyPressEvent ( QKeyEvent * e );
	
	public slots:
		void moreSearch();
		void stopSearch();
		void deleteThis();
		void queryPopup(QListViewItem *item, const QPoint &point, int);
		void itemDoubleClicked(QListViewItem *item);
		void addItem(QString searchReply);
		void browse();
		void browse(int index);
		void download();
		void closeAllTabs();
		void findItems(const QString &s);
		void clearFindbox();
		void setHashList(QValueList<QString> *list) { m_hashList = list; };
		void slotDeleteHash(QString hash); // hash was removed from list. if hash was marked as downloaded it will be makred as 'normal'

	private slots:
		void  updateFilterColumn(int index);
		void  updateFilterMode(int index);

		bool itemVisible(QListViewItem *item);
		
	signals:
		void giftAddRequest(QString, QString, QString, QString, QString);
		void giftCancelSearchRequest(int);
		void giftBrowseRequest(QString);
		void closetabs();

	private:
		QTabWidget      *m_parent;
		int              m_id;
		int              m_index;
		QLabel          *m_iconLabel;
		QLabel          *m_summaryLabel;
		QLineEdit       *m_findBox;
		QButton         *m_clearButton;
		QPushButton     *m_moreButton;
		QPushButton     *m_stopButton;
		QPopupMenu      *m_itemMenu;
		QPopupMenu      *m_sourceMenu;
		QPopupMenu      *m_browseUserMenu;
		QPopupMenu      *m_tabMenu;
//		KIconLoader     *m_loader;

		QDict<ApollonSearchViewItem>   m_hashes2items;
		ApollonSearchListView::Columns m_filterColumn;
		int                            m_filterMode;
		QString          m_current; // the item currently right-clicked
		QString          m_query;
		QString          m_textSearchString;
		QString          m_textCompleteString;

		ApollonSearchListView *m_searchResults;
		giFTConnection  *m_gcn;
		QValueList<QString> *m_hashList;
		bool m_foundItem;  // "where any items found which are already in the transfertab?"

	
};

#endif
