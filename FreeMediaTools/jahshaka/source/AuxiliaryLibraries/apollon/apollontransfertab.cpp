/***************************************************************************
                          Apollontransfertab.cpp  -  description
                             -------------------
    begin                : Tue Apr 2 2002
    copyright            : (C) 2002 by Robert Wittams
    email                : robert@wittams.com en Je moeder
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <assert.h>


#include <qdir.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qtimer.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvaluelist.h>

#include "giftconnection.h"

#include "apollontransfertab.h"
#include "apollontransferviewitem.h"
#include "apollonutils.h"


ApollonTransferTab::ApollonTransferTab(QWidget *parent, const char *name, giFTConnection *gcn) :
  ApollonTab(parent, name, gcn)//,
  //m_playerTab(0)
{
//	KIconLoader *loader = KGlobal::iconLoader();

	connect(m_gcn, SIGNAL(giftAddDownloadReply(QString)), this, SLOT(addDownloadViewItem(QString)));
	connect(m_gcn, SIGNAL(giftAddUploadReply(QString)),   this, SLOT(addUploadViewItem(QString)));
	connect(m_gcn, SIGNAL(giftChgUploadReply(QString)),   this, SLOT(updateUploadView(QString)));
	connect(m_gcn, SIGNAL(giftChgDownloadReply(QString)), this, SLOT(updateDownloadView(QString)));
	connect(m_gcn, SIGNAL(giftDelDownloadReply(QString)), this, SLOT(delDownloadViewItem(QString)));
	connect(m_gcn, SIGNAL(giftDelUploadReply(QString)),   this, SLOT(delUploadViewItem(QString)));
	connect(m_gcn, SIGNAL(giftAddSourceReply(QString)),   this, SLOT(addSourceItem(QString)));
	connect(m_gcn, SIGNAL(giftDelSourceReply(QString)),   this, SLOT(delSourceItem(QString)));
	connect(m_gcn, SIGNAL(giftItemReply(QString)),        this, SLOT(itemReply(QString)));
	connect(m_gcn, SIGNAL(disconnected()),                this, SLOT(disconnected()));

	//QSplitter *splitter = new QSplitter(this);
	//splitter->setOrientation(QSplitter::Vertical);

	//setMinimumWidth(400);
	QVBox *downBox = new QVBox(this);

	// toolbar for downloads
    /*
	KToolBar *toolbar = new KToolBar((QMainWindow*) 0, (QWidget*) downBox, false, "transfer_toolbar", true, true);
	m_actionPreview = new KAction(i18n("&Preview"), "multimedia", "", this, SLOT(previewFile()),
		(KActionCollection *) 0, "action_preview");
	m_actionPreview->plug(toolbar);
	m_actionPreview->setEnabled(false);
	m_actionPause = new KAction(i18n("P&ause"), "player_pause", "", this, SLOT(pauseDownload()),
		(KActionCollection *) 0, "action_pause");
	m_actionPause->plug(toolbar); 
	m_actionPause->setEnabled(false);
	m_actionResume = new KAction(i18n("&Resume"), "player_fwd", "", this, SLOT(unpauseDownload()),
		(KActionCollection *) 0, "action_resume");
	m_actionResume->plug(toolbar); 
	m_actionResume->setEnabled(false);
	m_actionFindMore = new KAction(i18n("&Find more sources"), "viewmag+", "", this, 
		SLOT(sourceSearch()), (KActionCollection *) 0, "action_findmore");
	m_actionFindMore->plug(toolbar); 
	m_actionFindMore->setEnabled(false);
	m_actionCancel = new KAction(i18n("&Cancel Download"), "stop", "", this, SLOT(cancel()),
		(KActionCollection *) 0, "action_cancel");
	m_actionCancel->plug(toolbar);
	m_actionCancel->setEnabled(false);

	toolbar->insertSeparator();
	toolbar->insertWidget(-1, 0, new KToolBarSeparator(Horizontal, true, toolbar, "separator1"), -1);
	toolbar->insertSeparator();
	
	m_actionBrowseUser = new KAction(i18n("Browse user"), "find", "", this, SLOT(browseUser()),
		(KActionCollection *) 0, "action_browse");
	m_actionBrowseUser->plug(toolbar);
	m_actionBrowseUser->setEnabled(false);
	m_actionRemoveSource = new KAction(i18n("Remove this source user"), "editdelete", "", this, 
		SLOT(removeSourceitem()), (KActionCollection *) 0, "action_removesource");
	m_actionRemoveSource->plug(toolbar);
	m_actionRemoveSource->setEnabled(false);

	toolbar->insertSeparator();
	toolbar->insertWidget(-1, 0, new KToolBarSeparator(Horizontal, true, toolbar, "separator2"), -1);
	toolbar->insertSeparator();
	
	m_actionClearFinished = new KAction(i18n("C&lear completed"), "edittrash", "", this,
		 SLOT(clearFinished()), (KActionCollection *) 0, "action_clearfinished");
	m_actionClearFinished->plug(toolbar);
	m_actionClearFinished->setEnabled(false);
	*/

	new QLabel("Downloads", downBox);
	m_downList = new ApollonListView(downBox);
	connect(m_downList, SIGNAL(selectionChanged()), this, SLOT(slotClickedDownListView()));

	m_downList->setSelectionMode(QListView::Extended);

	m_downList->addColumn("Name");
	m_downList->setColumnWidthMode(0, QListView::Manual);
	m_downList->setColumnWidth(0, 200);

	m_downList->addColumn("Size");
	m_downList->setColumnAlignment(1, Qt::AlignRight);

	m_downList->addColumn("Transferred");
	m_downList->setColumnAlignment(2, Qt::AlignRight);

	m_downList->addColumn("Bandwidth");
	m_downList->setColumnAlignment(3, Qt::AlignRight);

	m_downList->addColumn("Progress");
	m_downList->setColumnAlignment(4, Qt::AlignRight);

	m_downList->addColumn("ETA");
	m_downList->setColumnAlignment(5, Qt::AlignRight);

	m_downList->setAllColumnsShowFocus(true);
	m_downList->setRootIsDecorated(true);
	m_downList->setShowSortIndicator(true);

	m_downMenu = new QPopupMenu(m_downList);
	m_downMenu->insertItem("&Preview",                 this, SLOT(previewFile()),    Key_Return, 0, 0);
	m_downMenu->insertItem("P&ause",                   this, SLOT(pauseDownload()),  Key_P, 1, 1);
	m_downMenu->insertItem("&Find more sources",       this, SLOT(sourceSearch()),   Key_Plus, 2, 2);
	m_downMenu->insertItem("&Cancel Download",         this, SLOT(cancel()), Key_Delete, 3, 3);
	m_downMenu->insertSeparator();
	m_downMenu->insertItem("C&lear Finished/Canceled", this, SLOT(clearFinished()));

	m_browseUserMenu = new QPopupMenu(m_downList);

	connect(m_downList, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this, SLOT(downloadPopup(QListViewItem*, const QPoint&, int)));
	connect(m_downList, SIGNAL(expanded(QListViewItem *)), this, SLOT(updateDownloadSources(QListViewItem *)));
/*
	QVBox *upBox = new QVBox(splitter);

	new QLabel(i18n("Uploads"), upBox);

	m_upList = new ApollonListView(upBox);
	m_upList->setSelectionMode(QListView::Extended);
	m_upList->addColumn(i18n("Name"));
	m_upList->setColumnWidthMode(0, QListView::Manual);
	m_upList->setColumnWidth(0, 200);
	m_upList->addColumn(i18n("Size"));
	m_upList->setColumnAlignment(1, Qt::AlignRight);

	m_upList->addColumn(i18n("Transferred"));
	m_upList->setColumnAlignment(2, Qt::AlignRight);

	m_upList->addColumn(i18n("Bandwidth"));
	m_upList->setColumnAlignment(3, Qt::AlignRight);

	m_upList->addColumn(i18n("Progress"));
	m_upList->setColumnAlignment(4, Qt::AlignRight);

	m_upList->addColumn(i18n("ETA"));
	m_upList->setColumnAlignment(5, Qt::AlignRight);

	m_upList->setAllColumnsShowFocus(true);
	m_upList->setRootIsDecorated(true);
	m_upList->setShowSortIndicator(true);

	m_upMenu = new QPopupMenu(m_upList);
	m_upMenu->insertItem(loader->loadIcon("stop",          KIcon::Small), i18n("&Cancel Upload"),           this, SLOT(cancel()));
	m_upMenu->insertSeparator();
	m_upMenu->insertItem(loader->loadIcon("trashcan_full", KIcon::Small), i18n("C&lear Finished/Canceled"), this, SLOT(clearFinished()));

	connect(m_upList, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)), this, SLOT(uploadPopup(QListViewItem *, const QPoint &, int)));
	connect(m_upList, SIGNAL(expanded(QListViewItem *)), this, SLOT(updateUploadSources(QListViewItem *)));
	connect(m_upList, SIGNAL(selectionChanged()), this, SLOT(slotClickedUpListView()));
	*/

	//	splitter->setResizeMode(upBox, QSplitter::KeepSize);
	//QValueList<int> valuelist;
	//valuelist.append(200);
	//valuelist.append(75);

    //splitter->setSizes(valuelist);
	//splitter->show();
	m_showPopup = 1;

	//KConfig *config = KGlobal::config();
	//m_downList->restoreLayout(config, "TransferTabDown");
	//m_upList->restoreLayout(config, "TransferTabUp");

}


ApollonTransferTab::~ApollonTransferTab()
{
//	KConfig *config = KGlobal::config();
//	m_downList->saveLayout(config, "TransferTabDown");
//	m_upList->saveLayout(config, "TransferTabUp");
//	config->sync();
}

void ApollonTransferTab::slotClickedDownListView() 
{
    debug("clicked download");

	m_downList->blockSignals(true); 
	m_whichList = 0; 
	checkToolbarActions(); 
	m_downList->blockSignals(false);
}
		
void ApollonTransferTab::slotClickedUpListView() 
{ 
	m_upList->blockSignals(true); 
	m_whichList = 1; 
	checkToolbarActions(); 
	m_upList->blockSignals(false);
}

void ApollonTransferTab::addDownloadViewItem(QString line)
{
	int     ID =       giFTConnection::searchKey("ADDDOWNLOAD", line).toInt();
	int     transmit = giFTConnection::searchKey("transmit", line).toInt();
	int     size =     giFTConnection::searchKey("size", line).toInt();
	QString hash =     giFTConnection::searchKey("hash", line);
	QString file =     giFTConnection::searchKey("file", line);
	QString protocol_name = protocolName(line);

	float   progress = 100 * ((float) transmit / (float) size);

	ApollonTransferViewItem *item = new ApollonTransferViewItem(m_downList, "download", ID, protocol_name);

	item->setHash(hash);
	item->setName(file);
	item->setSize(size);
	item->setTransmit(transmit);
	//item->setStatus(QString("%1%").arg(progress, 0, 'f', 1));
	item->setProgress(progress);
	item->setBandwidth("--");

	int numberofsources = line.contains("SOURCE {", true);

	for(int i = 0; i < numberofsources; i++)
	{
		QString source = line.section("SOURCE {", i + 1, i + 1);
		ApollonTransferViewItem *sourceitem = new ApollonTransferViewItem(item);

		transmit = giFTConnection::searchKey("transmit", source).toInt();
		progress = 100 * ((float) transmit / (float) size);

		sourceitem->setName(giFTConnection::searchKey("user", source));

		sourceitem->setTransmit(transmit);
		sourceitem->setStart(giFTConnection::searchKey("start", source).toInt());
		sourceitem->setStatus(giFTConnection::searchKey("status", source));
		sourceitem->setProgress(progress);
		sourceitem->setUrl(giFTConnection::searchKey("url", source));
		sourceitem->setID(ID);
	}
	emit sigSendHash(hash);
}

void ApollonTransferTab::delDownloadViewItem(QString line)
{
	int ID = giFTConnection::searchKey("DELDOWNLOAD", line).toInt();
	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_downList, ID);
	if(item == 0)
	{
		debug("Received an invalid delDownloadView request.");
		return;
	}

	if(item->size() == item->transmit())
	{
		item->setStatus("Completed");
		item->setProgress(-1);


		//KIconLoader *loader = KGlobal::iconLoader();
		//QMimeSourceFactory::defaultFactory()->setPixmap("Apollon",loader->loadIcon("apollon", KIcon::MainToolbar));
		QString statustext;

		if(m_showPopup==1)
                {
                        //statustext = "<br><img source = \"Apollon\"><br><strong>" + "Download finished:" + "</strong> <br>" + item->name() +"<br>";
                        statustext = "Download finished:" + item->name() +"<br>";
                        //QPassivePopup::message(statustext, m_systemTray);
                }

		if(m_showPopup==2)
		{
			//statustext = "<img source = \"Apollon\"><br><strong>" + "Download finished:" + "</strong> <br>" + item->name();
			statustext = "Download finished:" + item->name();
                        //KPassivePopup::message(statustext,  m_systemTray);
		}
	}
	else
	{
		item->setStatus("Canceled");
		item->setProgress(-1);
	}
//	->setEnabled(true);
}

void ApollonTransferTab::addUploadViewItem(QString line)
{
	int     ID =       giFTConnection::searchKey("ADDUPLOAD", line).toInt();
	int     transmit = giFTConnection::searchKey("transmit", line).toInt();
	int     size =     giFTConnection::searchKey("size", line).toInt();
	QString file =     giFTConnection::searchKey("file", line);
	QString protocol_name = protocolName(line);
	float   progress = 100 * ((float) transmit / (float) size);

	ApollonTransferViewItem *item = new ApollonTransferViewItem(m_upList, "upload", ID, protocol_name);

	item->setName(file);
	item->setSize(size);
	item->setTransmit(transmit);
	//item->setStatus(QString("%1 %").arg(progress, 0, 'f', 1) );
	item->setProgress(progress);
	int numberofsources = line.contains("SOURCE {", true);

	for(int i = 0; i < numberofsources; i++)
	{
		QString source = line.section("SOURCE {", i + 1, i + 1);

		transmit = giFTConnection::searchKey("transmit", source).toInt();
		progress = 100 * ((float) transmit / (float) size);

		ApollonTransferViewItem *sourceitem = new ApollonTransferViewItem(item);

		sourceitem->setName(giFTConnection::searchKey("user", source));
		sourceitem->setSize(transmit);
		sourceitem->setStatus(giFTConnection::searchKey("status", source));
		sourceitem->setProgress(progress);
	}
}

void ApollonTransferTab::delUploadViewItem(QString line)
{
	int ID = giFTConnection::searchKey("DELUPLOAD", line).toInt();

	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_upList, ID);
	if(item == 0)
	{
//		kdDebug() << "Received an invalid delUploadViewItem request." << endl;
		return;
	}

	if(item->size() == item->transmit())
	{
		item->setStatus("Completed");
		item->setProgress(-1);
	}
	else
	{
		item->setStatus("Canceled");
		item->setProgress(-1);
	}
	clearFinished(item);
}

void ApollonTransferTab::updateDownloadView(QString line)
{
	int     ID =        giFTConnection::searchKey("CHGDOWNLOAD", line).toInt();
	int     transmit =  giFTConnection::searchKey("transmit", line).toInt();
	int     through =   giFTConnection::searchKey("throughput", line).toInt();
	int     elapsed =   giFTConnection::searchKey("elapsed", line).toInt();
	int     size =      giFTConnection::searchKey("size", line).toInt();
	if(transmit == through) through = 0;
	QString bandwidth = ApollonUtils::calcBandwidth(through, elapsed);
	QString protocol_name = protocolName(line);

	float   progress =  100 * ((float) transmit / (float) size);

	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_downList, ID);
	if(item == 0)
	{
		debug("Received an invalid updateDownloadView request.");
		return;
	}
	item->updateIcon(protocol_name, "download");
	item->setTransmit(transmit);
	item->setBandwidth(bandwidth);
	item->setProgress(progress);
	item->setETA(through == 0 ? -1 : (int) ((size - transmit) / 1000 * ((float) elapsed / through)));

	item->setUpdateString(line);

	if(item->isOpen())
	{
		updateDownloadSources(item);
	}

	countActives();
}

void ApollonTransferTab::updateDownloadSources(QListViewItem *item)
{
	QString line = dynamic_cast<ApollonTransferViewItem *>(item)->updateString();

	int     elapsed =   giFTConnection::searchKey("elapsed", line).toInt();

	QString source = line.section("SOURCE {", 0, 0);
	line.remove(0, source.length());

	ApollonTransferViewItem *sourceItem = dynamic_cast<ApollonTransferViewItem *>(item)->firstChild();

	while(source = line.section("SOURCE {", 1, 1), !source.isEmpty())
	{
		if(sourceItem == 0)
		{
			sourceItem = new ApollonTransferViewItem(item);
		}

		QString sourceName =   giFTConnection::searchKey("user", source);
		QString sourceStatus = giFTConnection::searchKey("status", source);

		sourceItem->setName(sourceName);
		sourceItem->setStatus(sourceStatus);
		sourceItem->setStart(giFTConnection::searchKey("start", source).toInt());
		if(sourceStatus == "Active")
		{
			int sourceTransmit = giFTConnection::searchKey("transmit", source).toInt();
			int sourceSize = giFTConnection::searchKey("total", source).toInt();
			int aux = sourceTransmit - sourceItem->transmit();

			sourceItem->setTransmit(sourceTransmit);
			sourceItem->setBandwidth(ApollonUtils::calcBandwidth(aux, elapsed));
			sourceItem->setProgress(100 * ((float) sourceTransmit / (float) sourceSize));
		}
		else
		{
			sourceItem->setTransmit(-1);
			sourceItem->setBandwidth("--");
			sourceItem->setProgress(-1);
		}

		sourceItem = sourceItem->nextSibling();

		line.remove(0, source.length() + 8);
	}
}

void ApollonTransferTab::updateUploadView(QString line)
{
	int     ID =       giFTConnection::searchKey("CHGUPLOAD", line).toInt();
	int     transmit = giFTConnection::searchKey("transmit", line).toInt();
	int     through =  giFTConnection::searchKey("throughput", line).toInt();
	int     elapsed =  giFTConnection::searchKey("elapsed", line).toInt();
	int     size =     giFTConnection::searchKey("size", line).toInt();
	float   progress = 100 * ((float) transmit / (float) size);

	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_upList, ID);
	if(item == 0)
	{
//		kdDebug() << "Received an invalid updateUploadView request." << endl;
		return;
	}

	item->setTransmit(transmit);
	item->setBandwidth(ApollonUtils::calcBandwidth(through, elapsed));
	item->setProgress(progress);
	item->setETA(through == 0 ? -1 : ((int) ((size - transmit) / 1000 * ((float) elapsed / through))));

	item->setUpdateString(line);

	updateUploadSources(item);
}

void ApollonTransferTab::updateUploadSources(QListViewItem *item)
{
	QString line = dynamic_cast<ApollonTransferViewItem *>(item)->updateString();

	QString source = line.section("SOURCE {", 0, 0);
	line.remove(0, source.length());

	ApollonTransferViewItem *sourceItem = dynamic_cast<ApollonTransferViewItem *>(item)->firstChild();

	while(source = line.section("SOURCE {", 1, 1), !source.isEmpty())
	{
   		if(sourceItem == 0)
		{
			sourceItem = new ApollonTransferViewItem(item);
		}

		QString sourceName =   giFTConnection::searchKey("user", source);
		QString sourceStatus = giFTConnection::searchKey("status", source);

		sourceItem->setName(sourceName);
		sourceItem->setStatus(sourceStatus);

		if(sourceStatus == "Active")
		{
			int sourceTransmit = giFTConnection::searchKey("transmit", source).toInt();
			int sourceSize = giFTConnection::searchKey("total", source).toInt();

			sourceItem->setTransmit(sourceTransmit);
			sourceItem->setProgress(100 * ((float) sourceTransmit / (float) sourceSize));
		}
		else
		{
			sourceItem->setTransmit(-1);
			sourceItem->setProgress(-1);
		}

		sourceItem = sourceItem->nextSibling();

		line.remove(0, source.length() + 8);
	}
}

void ApollonTransferTab::addSourceItem(QString line)
{
	int ID = giFTConnection::searchKey("ADDSOURCE", line).toInt();

	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_downList, ID);
	QString protocol_name = protocolName(line);
	item->updateIcon(protocol_name, "download");

	if(item == 0)
	{
//		kdDebug() << "Received an invalid addSourceItem request." << endl;
		return;
	}

	ApollonTransferViewItem *sourceItem = new ApollonTransferViewItem(item);

	sourceItem->setName(giFTConnection::searchKey("user", line));
	sourceItem->setStatus(giFTConnection::searchKey("status", line));
	sourceItem->setProgress(-1);
	sourceItem->setUrl(giFTConnection::searchKey("url", line));
	sourceItem->setID(ID);
}


void ApollonTransferTab::delSourceItem(QString line)
{
        int ID = giFTConnection::searchKey("DELSOURCE", line).toInt();

	ApollonTransferViewItem *item = ApollonTransferViewItem::getItem(m_downList, ID);
	if(item == 0)
	{
//		kdDebug() << "Received an invalid addSourceItem request." << endl;
		return;
	}

	bool deleted = false;
	QString url = giFTConnection::searchKey("url", line);
	ApollonTransferViewItem *sourceItem = static_cast<ApollonTransferViewItem*>(item->firstChild());
	while (!deleted) {

	  if (sourceItem)
	    if (sourceItem->url() == url) {
	      item->takeItem(sourceItem);
	      delete sourceItem;
	      deleted = true;
	    }
	    else
	      sourceItem = sourceItem->nextSibling();
	  else
	    deleted = true;
	}

}

void ApollonTransferTab::disconnected()
{
	clearAll();
}

void ApollonTransferTab::downloadPopup(QListViewItem *item, const QPoint &point, int)
{

    debug("download popup1");

    m_current = 0;

	if(!item)
		return;

//	KIconLoader *loader = KGlobal::iconLoader();
	ApollonTransferViewItem* transview = (ApollonTransferViewItem*) item;
	m_browseUserMenu->clear();

	if (transview->parent() == 0)  // click on a file
	{
		m_current = transview->id();

		if (transview->isPaused())
		{
			m_downMenu->removeItem(1);
			m_downMenu->insertItem("&Resume", this, SLOT(unpauseDownload()),    Key_R,1,1);
		}
		else
		{
			m_downMenu->removeItem(1);
			m_downMenu->insertItem("P&ause", this, SLOT(pauseDownload()),    Key_P,1,1);

        }

		m_downMenu->popup(point);
	}
	else   // click on a source
	{
		m_current = transview->parent()->id();
		m_userIP =  transview->name();

        m_browseUserMenu->insertItem("Browse user: " + m_userIP, this, SLOT(browseUser()));
		m_browseUserMenu->insertItem("Remove this source user: " + m_userIP, this, SLOT(removeSourceitem()));
		m_browseUserMenu->popup(point);
	}

}

void ApollonTransferTab::uploadPopup(QListViewItem *item, const QPoint &point, int)
{
	m_current = 0;

	if(!item)
	{
		return;
	}

	ApollonTransferViewItem *transview = (ApollonTransferViewItem*) item;

	if(transview->parent() == 0) // click on file
	{
		m_current = transview->id();
	}
	else  // click on source
	{
		m_current = transview->parent()->id();
	}

	m_upMenu->popup(point);
}

void ApollonTransferTab::cancel() 
{
	if ( m_whichList == 0) 
		cancelFiles(m_downList);
	else if ( m_whichList == 1)
		cancelFiles(m_upList);
}

void ApollonTransferTab::clearFinished()
{
	if ( m_whichList == 0) 
		clearFinishedFiles(m_downList);
	else if ( m_whichList == 1)
		clearFinishedFiles(m_upList);
}

void ApollonTransferTab::cancelFiles(ApollonListView* list)
{
    debug("canceling...");

	QStringList strlist;
	ApollonTransferViewItem* item = (ApollonTransferViewItem *) list->firstChild();
	QValueList<ApollonTransferViewItem*> itemList;
	while(item != 0)
	{
		if(item->isSelected())
		  {
			itemList <<  item;
			strlist += item->text(0);
		}
		item = item->nextSibling();
	}
/*
	int answer = KMessageBox::questionYesNoList (this, i18n("Do you want to delete these items?"),  strlist, i18n("Deletion"));
	if (answer == KMessageBox::Yes)
	{
    */
		for (QValueList<ApollonTransferViewItem*>::Iterator it = itemList.begin(); it != itemList.end(); ++it) {
			m_gcn->makeTransferRequest((*it)->id(), "cancel");
			emit sigDeleteHash( (*it)->hash() );
			delete (*it);
		}
        /*
	}
    */
	checkToolbarActions();
}

void ApollonTransferTab::removeSourceitem()
{
    debug("removing source");

	QString del_url;
	int finish=0;
	QStringList strlist;
	int del_id;
	ApollonTransferViewItem* item = (ApollonTransferViewItem *) m_downList->firstChild();
	while(item != 0)
		{
			while(!item->firstChild())
			{
				 if ( !item->nextSibling())
				{
					finish=1; 
					break;
				 }
				 else 
				 {
				 	item=item->nextSibling();
				}
			 }
			if (finish==1)
				break;
			item=item->firstChild();
			while(item != 0)
			{
				if(item->isSelected())
				{
					strlist += item->text(0);
				}
				if(item->nextSibling()) 
					item=item->nextSibling();
				else break;
			}
			item=item->parent();
			item=item->nextSibling();
		}
/*
    int answer = KMessageBox::questionYesNoList (this, i18n("Do you want to remove these sources?"),  strlist, i18n("Deletion"));
	if (answer == KMessageBox::Yes)
	{
*/
		 item = (ApollonTransferViewItem *) m_downList->firstChild();
		 while(item != 0)
		{
			while(!item->firstChild())
			{
				 if ( !item->nextSibling())
				{
					finish=1; 
					break;
				 }
				else 
				{
					item=item->nextSibling();
				}
			 }
			if (finish==1)
				break;
			item=item->firstChild();
			while(item != 0)
			{
				if(item->isSelected())
				{
					del_id = item->id();
					del_url = item->url();
					m_gcn->makeDelSourceRequest(del_id,del_url);
				}
				if(item->nextSibling()) item=item->nextSibling();
				else break;
			}
			item=item->parent();
			item=item->nextSibling();
		}
        /*
	}
    */
}

void ApollonTransferTab::pauseDownload()
{
	ApollonTransferViewItem* item = (ApollonTransferViewItem *) m_downList->firstChild();
	while(item != 0)
	{
		if(item->isSelected())
		{
			m_current = item->id();
			item->pauseItem();
			m_gcn->makeTransferRequest(m_current, "pause");
		}
		item = item->nextSibling();
	}
	checkToolbarActions();
}

void ApollonTransferTab::unpauseDownload()
{
	ApollonTransferViewItem* item = (ApollonTransferViewItem *) m_downList->firstChild();
	while(item != 0)
	{
		if(item->isSelected())
		{
			m_current = item->id();
			item->unpauseItem();
			m_gcn->makeTransferRequest(m_current, "unpause");
		}
		item = item->nextSibling();
	}
	checkToolbarActions();
}

void ApollonTransferTab::clearFinishedFiles(ApollonListView* list)
{
	clearFinished( (ApollonTransferViewItem *) list->firstChild() );
//	m_actionClearFinished->setEnabled(false);
}

void ApollonTransferTab::clearFinished(ApollonTransferViewItem *item)
{
	ApollonTransferViewItem* temp;
	while(item)
	{
		temp = item->nextSibling();
		if(item->status() == "Canceled" || item->status() == "Completed")
		{
			delete item;
		}
		item = temp;
	}
}

void ApollonTransferTab::clearAll()
{
	ApollonTransferViewItem *p = (ApollonTransferViewItem *) m_downList->firstChild(), *q;
	while(p)
	{
		q = p->nextSibling();
		delete p;
		p = q;
	}

	p = (ApollonTransferViewItem *) m_upList->firstChild();
	while(p)
	{
		q = p->nextSibling();
		delete p;
		p = q;
	}
	checkToolbarActions();
}

void ApollonTransferTab::previewFile()
{
//	assert(m_playerTab != 0);
	//assert(m_preferences != 0);

	QString url;

	if(((ApollonTransferViewItem *) m_downList->currentItem())->status() == "Completed")
	{
//		url = QString("file:") + m_preferences->completedPath() + "/" + ((ApollonTransferViewItem *) m_downList->currentItem())->name();
	//	m_playerTab->play(url);
	}
	else
	{
	//	QDir dir(m_preferences->incomingPath());
		//url = QString("file:") + dir.path() + "/" + dir.entryList().grep(((ApollonTransferViewItem *) m_downList->currentItem())->name())[0];
		//m_playerTab->play(url, ((ApollonTransferViewItem *) m_downList->currentItem())->name());
	}
}

void ApollonTransferTab::autoSourceSearch()
{
	sourceSearch(true);
}

void ApollonTransferTab::sourceSearch(bool autoSearch)
{
	ApollonTransferViewItem* item = (ApollonTransferViewItem*) m_downList->firstChild();
	ApollonTransferViewItem* sourcesearchitem;

	while(item)
	{
		sourcesearchitem = item;
		if((autoSearch || sourcesearchitem->isSelected()) &&
		   sourcesearchitem->status() != "Canceled" && sourcesearchitem->status() != "Completed")
		{
			int ID = m_gcn->getNewID();
			m_sourceSearchIDs.append(ID);
			m_gcn->makeLocateRequest(ID, sourcesearchitem->hash());
		}
		item = item->nextSibling();
	}
}

void ApollonTransferTab::itemReply(QString line)
{
	line = line.stripWhiteSpace();

	int receivedItemID = giFTConnection::searchKey("ITEM", line).toInt();

	if(m_sourceSearchIDs.contains(receivedItemID) > 0)
	{
		// Should there be a test here to see if this reply is already in the list ?

		if(line.find("file(") > -1)
		{
			QString user = giFTConnection::searchKey("user", line);
			QString hash = giFTConnection::searchKey("hash", line);
			QString size = giFTConnection::searchKey("size", line);
			QString url =  giFTConnection::searchKey("url",  line);

			QString temp = giFTConnection::searchKey("file", line);
			QString save = temp.right(temp.length() - temp.findRev("/") - 1);

			m_gcn->makeAddRequest(user, hash, size, url, save);
		}
		else
		{
			QString endsearch = "ITEM(" + QString::number(receivedItemID) + ");";

			if(QString::compare(endsearch, line) == 0)
			{
				m_sourceSearchIDs.remove(receivedItemID);
			}
		}
	}
}

void ApollonTransferTab::browseUser()
{
	emit browseUser(m_userIP);
}

void ApollonTransferTab::getUserIP(int id)
{
	m_userIP = m_browseUserMenu->text(id);
}

void ApollonTransferTab::countActives()
{
	// sends a signal to the systemtray, which displays the number of downloads in the QToolTip.
	int actives = 0;
	float sumrate = 0;
	QString s;
	ApollonTransferViewItem *item = (ApollonTransferViewItem *) m_downList->firstChild();
	while(item)
	{
		s = item->bandwidth();
		if(s.find("KB/s") != -1)
		{
			actives++;
			sumrate += (s.remove(" KB/s")).toFloat();
		}
		item = item->nextSibling();
	}
	emit statsUpdate(actives, sumrate);
}

void ApollonTransferTab::checkToolbarActions()
{
	bool canResume = false, canPause = false, canPreview = false, canSearch = false, canCancel = false;
	bool canRemove = false, canBrowse = false;
	bool canClear = false;
	
	// pause (resume) is active when there is at least 1 selected file which is resumed (paused)
	// cancel & search more are active when there is at least 1 selected file
	// preview is active when there is exactly 1 selected file
	// clear is active when there is at least 1 file which is cancelled or completed
	
	if ( m_whichList == 0 ) // downllist
	{
		ApollonTransferViewItem* item = (ApollonTransferViewItem *) m_downList->firstChild();
		while(item != 0)
		{
			if(item->isSelected())
			{
				if (!canSearch) // no selected items found yet
					canPreview = true;
				else canPreview = false;
				
				if (!(item->isPaused())) canPause = true;
				if (item->isPaused()) canResume = true;
				canSearch = true;
				canCancel = true;
			}
			canClear = canClear || (item->status() == "Canceled") || (item->status() == "Completed");
	
			item = item->nextSibling();
		}
	
		// 'remove source' is active when there is at least 1 selected source, and no files are selected
		// 'browse user' is active when there is exactly 1 selected source, and no files are selected
		
		int finish = 0;
		item = (ApollonTransferViewItem *) m_downList->firstChild();
		
		if (!canSearch)  // canSearch == at least 1 file is selected
		while (item != 0)
		{
			while (!item->firstChild())
			{
				if (!item->nextSibling())
				{
					finish = 1;
					break;
				}
				else item = item->nextSibling();
			}
			if (finish) break;
			
			item = item->firstChild();
			while (item != 0)
			{
				if (item->isSelected())
				{
					if (!canRemove)
						canBrowse = true;
					else canBrowse = false;
					canRemove = true;
				}
				if (item->nextSibling()) item = item->nextSibling();
				else break;
			}
			item = item->parent()->nextSibling();
		}
		
	}
	else if ( m_whichList == 1) // uplist
	{
		ApollonTransferViewItem* item = (ApollonTransferViewItem *) m_upList->firstChild();
		while(item != 0)
		{
			if(item->isSelected())
			{
				canCancel = true;
			}
			canClear = canClear || (item->status() == "Canceled") || (item->status() == "Completed");
	
			item = item->nextSibling();
		}
	}
	
//	m_actionPreview->setEnabled(canPreview);
//	m_actionFindMore->setEnabled(canSearch);
//	m_actionCancel->setEnabled(canCancel);
//	m_actionResume->setEnabled(canResume);
//	m_actionPause->setEnabled(canPause);
//	m_actionBrowseUser->setEnabled(canBrowse);
//	m_actionRemoveSource->setEnabled(canRemove);
//	m_actionClearFinished->setEnabled(canClear);
}	
	

void ApollonTransferTab::setTray(ApollonSystemTray *tray)
{
	m_systemTray = tray;
}

void ApollonTransferTab::setShowPopup(int show)
{
	m_showPopup = show;
}

void ApollonTransferTab::autoSearchInterval(int i)
{
	m_autoSearchInterval = i;
	m_sourceSearchTimer = new QTimer;
	m_sourceSearchTimer->start(m_autoSearchInterval * 60000); // m_autoSearchInterval is in minutes
	connect(m_sourceSearchTimer, SIGNAL(timeout()), this, SLOT(autoSourceSearch()));
}

void ApollonTransferTab::contextMenuEvent(QContextMenuEvent *e)
{
	e->consume();
}

QString ApollonTransferTab::protocolName(QString line)
{
	QString protocolName = giFTConnection::searchKey("url", line);

	if(protocolName == "")
		protocolName = giFTConnection::searchKey("hash", line);

	if(protocolName.isEmpty() || protocolName.isNull())
		protocolName = " ";

	protocolName = protocolName.section(":", 0, 0);

	if(protocolName == "FTH")
		protocolName = "FastTrack";

	return protocolName;
}
