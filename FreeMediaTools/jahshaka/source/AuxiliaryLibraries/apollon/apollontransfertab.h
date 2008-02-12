/***************************************************************************
                          Apollontransfertab.h  -  description
                             -------------------
    begin                : Tue Apr 2 2002
    copyright            : (C) 2002 by Robert Wittams
    email                : robert@wittams.com en Je Moeder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef APOLLONTRANSFERTAB_H
#define APOLLONTRANSFERTAB_H

//#include <kaction.h>
//#include <klistview.h>

#include <qevent.h>
#include <qvaluelist.h>

#include "apollontab.h"
#include "apollonlistview.h"

class QPopupMenu;
class QTimer;

class ApollonPlayerTab;
class ApollonPreferencesDialog;
class ApollonSystemTray;
class ApollonTransferViewItem;
class giFTConnection;

class ApollonTransferTab : public ApollonTab
{
	Q_OBJECT

	public:
		ApollonTransferTab(QWidget *parent, const char *name, giFTConnection *gcn);
		~ApollonTransferTab();

		void setTray(ApollonSystemTray *sysTray);
		inline void setPlayerTab(ApollonPlayerTab *playerTab);
		inline void setPreferences(ApollonPreferencesDialog *preferences);
		//virtual void keyPressEvent(QKeyEvent*);
		virtual void contextMenuEvent( QContextMenuEvent *);

	public slots:
		void addDownloadViewItem(QString line);
		void delDownloadViewItem(QString line);
		void addUploadViewItem(QString line);
		void delUploadViewItem(QString line);
		void updateDownloadView(QString line);
		void updateDownloadSources(QListViewItem *item);
		void updateUploadView(QString line);
		void updateUploadSources(QListViewItem *item);
		void addSourceItem(QString line);
		void delSourceItem(QString line);
		void itemReply(QString line);
		void setShowPopup(int);
		void disconnected();
		void autoSearchInterval(int);

	private slots:
		void checkToolbarActions();
		
		void previewFile();
		void autoSourceSearch();
		void sourceSearch(bool autoSearch = false);
		void browseUser();
		void getUserIP(int);
		void cancelFiles(ApollonListView*);
		void removeSourceitem();
		void pauseDownload();
		void unpauseDownload();
		void clearFinishedFiles(ApollonListView*);
		void clearAll();
		void downloadPopup(QListViewItem*, const QPoint&, int);
		void uploadPopup(QListViewItem*, const QPoint&, int col);

		void slotClickedDownListView();
		void slotClickedUpListView();

		void cancel();
		void clearFinished();
		
	signals:
		void browseUser(QString user);
		void statsUpdate(int activeDownloads, float rate);
		void sigSendHash(QString hash);
		void sigDeleteHash(QString hash);
	

	private:
		ApollonListView *m_upList;
		ApollonListView *m_downList;

		QPopupMenu *m_downMenu;
		QPopupMenu *m_upMenu;
		QPopupMenu *m_browseUserMenu;
		
//		KAction *m_actionPreview, *m_actionPause, *m_actionResume, *m_actionFindMore, *m_actionCancel;
//		KAction *m_actionRemoveSource, *m_actionBrowseUser;
//		KAction *m_actionClearFinished;

		int                       m_current; // the item currently right-clicked
		int                       m_autoSearchInterval; // minutes
		int                      m_showPopup;
		QString                   m_userIP;
		QValueList<int>           m_sourceSearchIDs;
		ApollonSystemTray        *m_systemTray;
		ApollonPlayerTab         *m_playerTab;
		ApollonPreferencesDialog *m_preferences;
		QTimer                   *m_sourceSearchTimer;

		QString  protocolName(QString line);
		void     clearFinished(ApollonTransferViewItem *item);
		void     countActives();
		
		int m_whichList; // == 0 if downlist, == 1 if uplist !!
		
};

void ApollonTransferTab::setPlayerTab(ApollonPlayerTab *playerTab)
{
	m_playerTab = playerTab;
}

void ApollonTransferTab::setPreferences(ApollonPreferencesDialog *preferences)
{
	m_preferences = preferences;
}

#endif
