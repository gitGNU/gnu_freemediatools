/***************************************************************************
                          apollonqueryview.cpp  -  description
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

#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qregexp.h>
#include <qstring.h>
#include <qtabwidget.h>
#include <qtooltip.h>

//#include <kconfig.h>
//#include <kdeversion.h>
#include <qlistview.h>
//#include <kglobal.h>
//#include <klocale.h>
//#include <kmimetype.h>

#include "apollonqueryview.h"
#include "apollonsearchviewitem.h"
#include "apollonutils.h"

ApollonQueryView::ApollonQueryView(QTabWidget *parent, int ID, int index, const QString &text, giFTConnection *gcn):
  QVBox(parent),
  m_parent(parent),
  m_id(ID),
  m_index(index),
  m_filterColumn(ApollonSearchListView::columnName),
  m_filterMode(0),
  m_query(text),
  m_gcn(gcn),
  m_foundItem(false)
{
	setName("queryTab");

	QHBox *box = new QHBox(this);
	QHBox *box2 = new QHBox(this);

	m_iconLabel = new QLabel(box);
	m_summaryLabel = new QLabel(box);

	QString realm;

	m_textSearchString = "Results for <i>%1</i> with keywords <i>%2</i> - Searching...";
	m_textCompleteString = "Results for <i>%1</i> with keywords <i>%2</i> - Search completed!";

	m_summaryLabel->setText(m_textSearchString);

	// initialize Search More button
	m_moreButton = new QPushButton(box);
	m_moreButton->setText(("Search More"));
	m_moreButton->setFixedHeight(m_moreButton->sizeHint().height());
	m_moreButton->setFixedWidth(m_moreButton->sizeHint().width());
	connect(m_moreButton, SIGNAL(clicked()), this, SLOT(moreSearch()));
	QToolTip::add(m_moreButton, ("Try to find more sources for this query"));
	m_moreButton->setEnabled(false);

	// initialize Stop Search button
	m_stopButton = new QPushButton(box);
	m_stopButton->setText(("Stop Search"));
	m_stopButton->setFixedHeight(m_stopButton->sizeHint().height());
	m_stopButton->setFixedWidth(m_stopButton->sizeHint().width());
	connect(m_stopButton, SIGNAL(clicked()), this, SLOT(stopSearch()));
	QToolTip::add(m_stopButton, ("Stop search (esc)"));

	// initialize Close button
	QPushButton *closeButton = new QPushButton(box);
//	closeButton->setPixmap(m_loader->loadIcon("fileclose", KIcon::Small));
	closeButton->setFixedHeight(closeButton->sizeHint().height());
	closeButton->setFixedWidth(closeButton->sizeHint().width());
	connect(closeButton, SIGNAL(clicked()), this, SLOT(deleteThis()));
	QToolTip::add(closeButton, ("Close tab (Ctrl-w)"));

	box->setStretchFactor(m_summaryLabel, 4);
	box->setStretchFactor(closeButton, 0);

	QComboBox *filterColumnCombo = new QComboBox(box2);
	filterColumnCombo->insertItem(("File Name"), 0);
	filterColumnCombo->insertItem(("User"), 1);
	filterColumnCombo->insertItem(("Artist"), 2);
	filterColumnCombo->insertItem(("Album"), 3);
	filterColumnCombo->insertItem(("Title"), 4);
	filterColumnCombo->insertItem(("Duration"), 5);
	filterColumnCombo->insertItem(("File Type"), 6);
	filterColumnCombo->insertItem(("Bitrate"), 7);
	filterColumnCombo->insertItem(("Category"), 8);
	filterColumnCombo->insertItem(("Directory"), 9);
	connect(filterColumnCombo, SIGNAL(activated(int)), this, SLOT(updateFilterColumn(int)));

	QComboBox *filterModeCombo = new QComboBox(box2);
	filterModeCombo->insertItem(("contains all of"), 0);
	filterModeCombo->insertItem(("contains any of"), 1);
	filterModeCombo->insertItem(("doesn't contain"), 2);
	connect(filterModeCombo, SIGNAL(activated(int)), this, SLOT(updateFilterMode(int)));

	m_findBox = new QLineEdit(box2);

	m_clearButton = new QPushButton(box2);
	m_clearButton->setText(("Clear"));
	m_clearButton->setEnabled(false);

	connect(m_findBox,     SIGNAL(textChanged(const QString &)), this, SLOT(findItems(const QString &)));
	connect(m_clearButton, SIGNAL(clicked()),                    this, SLOT(clearFindbox()));

	// initialize the results listview
	m_searchResults = new ApollonSearchListView(this);

	connect(m_searchResults, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(itemDoubleClicked(QListViewItem *)));
	connect(m_searchResults, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this, SLOT(queryPopup(QListViewItem*, const QPoint&, int)));

	// right-mouse-button menu for parent items
	m_itemMenu = new QPopupMenu(m_searchResults);

	// submenu of menu
	m_browseUserMenu = new QPopupMenu(m_searchResults);
//	m_itemMenu->insertItem(m_loader->loadIcon("2downarrow", KIcon::Small), ("Download"), this, SLOT(download()));
//	m_itemMenu->insertItem(m_loader->loadIcon("folder", KIcon::Small), ("Browse user:"), m_browseUserMenu);

	// right-mouse-button menu for child items
	m_sourceMenu = new QPopupMenu(m_searchResults);

//	KConfig *config = KGlobal::config();
//	m_searchResults->restoreLayout(config, "QueryView_" + QString::number(m_index));
}

ApollonQueryView::~ApollonQueryView()
{
//	KConfig *config = KGlobal::config();

//	m_searchResults->saveLayout(config, "QueryView_" + QString::number(m_index));
//	config->sync();
}

void ApollonQueryView::stopSearch()
{
	emit giftCancelSearchRequest(m_id);
}

void ApollonQueryView::moreSearch()
{
	m_id = m_gcn->getNewID();
	m_gcn->makeSearchRequest(m_id, m_index, m_query);
	m_summaryLabel->setText(("Searching More..."));
	m_parent->changeTab(this, m_query + (" (searching more)"));
	m_moreButton->setEnabled(false);
	m_stopButton->setEnabled(true);
}

void ApollonQueryView::deleteThis()
{
	emit giftCancelSearchRequest(m_id);

	delete this;
}

void ApollonQueryView::itemDoubleClicked(QListViewItem *item)
{
	if(item)
	{
		ApollonSearchViewItem *svi = static_cast<ApollonSearchViewItem*>(item);
		if(svi->isChild())
		{
			item = item->parent();
			itemDoubleClicked(item);
			return;
		}

		QString protocolName;
		if(svi->childCount() == 0)
		{
			protocolName = svi->protocol();
			emit giftAddRequest(svi->user(), svi->hash(), svi->size(), svi->url(), svi->fileName());
		}
		else
		{
			ApollonSearchViewItem *child = svi->firstChild();
			protocolName = child->protocol();
			while(child)
			{
//				child->setPixmap(0, m_loader->loadIcon("2downarrow", KIcon::Small));
				emit giftAddRequest(child->user(), child->hash(), child->size(), child->url(), child->fileName());
				child = child->nextSibling();
			}
		}

		svi->setDownloadIcon();
	}
}

void ApollonQueryView::download()
{
	QListViewItem *item = m_searchResults->firstChild();
	while(item)
	{
		if(item->isSelected() && item->isVisible())
		{
			itemDoubleClicked(item);
		}
		item = item->nextSibling();
	}
}

void ApollonQueryView::queryPopup(QListViewItem *item, const QPoint &point, int)
{
	m_current = "";

	if(!item)
		return;

	ApollonSearchViewItem *svi = dynamic_cast<ApollonSearchViewItem*>(item);

	if(svi->isChild())
	{
		m_sourceMenu->clear();
		m_current = svi->user();
//		m_sourceMenu->insertItem(m_loader->loadIcon("folder", KIcon::Small), i18n("Browse user: %1").arg(m_current), this, SLOT(browse()));
		m_sourceMenu->popup(point);
	}
	else
	{
		// loop through the children to get the user names out and put them in the menu
		m_browseUserMenu->clear();

		if(svi->childCount() == 0)
		{
//			m_browseUserMenu->insertItem(m_loader->loadIcon("connect_established", KIcon::Small), svi->user(), this, SLOT(browse(int)));
		}
		else
		{
			ApollonSearchViewItem *child = svi->firstChild();
			while(child)
			{
				QString user = child->user();
			//	m_browseUserMenu->insertItem(m_loader->loadIcon("connect_established", KIcon::Small), user, this, SLOT(browse(int)));
				child = child->nextSibling();
			}
		}

		m_itemMenu->popup(point);
	}
}

//#include <kdebug.h>
void ApollonQueryView::addItem(QString searchReply)
{
	searchReply = searchReply.stripWhiteSpace();

	QString recievedSearchID = giFTConnection::searchKey("ITEM", searchReply);

	if(recievedSearchID.toInt() == m_id)
	{
		if(searchReply.find("file(") > -1)
		{
			// see if there is already a ListViewItem with this hash and
			// make a child if there is
			QString hash = giFTConnection::searchKey("hash", searchReply);

			ApollonSearchViewItem *p = (hash.isEmpty() ? 0 : m_hashes2items[hash]);

			if(p == 0)
			{
				// we have a new unique hash make a parent
				p = new ApollonSearchViewItem(m_searchResults);
				if(!hash.isEmpty())
				{
					m_hashes2items.insert(hash, p);
				}

				QString url = giFTConnection::searchKey("url", searchReply);
				QString protocolName = url.section(":", 0, 0);
				
				p->setProtocol(protocolName);
				
				if ( m_hashList->find(hash) == m_hashList->end())  // a hash which is in transfertab was not found
				{
					p->setIcon();
					m_foundItem = true;
				}
				else		// same file is already in progress
					p->setDownloadIcon();
					
				p->setUrl(url);
				p->setFileName(giFTConnection::searchKey("file", searchReply).section('/', -1, -1));
				p->setSize(giFTConnection::searchKey("size", searchReply));
				p->setHash(giFTConnection::searchKey("hash", searchReply));
				p->setAlbum(giFTConnection::searchKey("album", searchReply));
				p->setDuration(giFTConnection::searchKey("duration", searchReply));
				p->setBitrate(giFTConnection::searchKey("bitrate", searchReply));
				p->setUser(giFTConnection::searchKey("user", searchReply));
				p->setOpenSlots(giFTConnection::searchKey("availability", searchReply));
				p->setArtist(giFTConnection::searchKey("artist", searchReply));
				p->setTitle(giFTConnection::searchKey("title", searchReply));
				p->setCategory(giFTConnection::searchKey("category", searchReply));
				p->setDirectory(giFTConnection::searchKey("directory", searchReply));

				QString mime = giFTConnection::searchKey("mime", searchReply);
				QString mimeIcon;// = type->icon(mime, 1);
				p->setVisible(itemVisible(p));
			}
			else
			{
				// not a unique item, make a child
				if(giFTConnection::searchKey("user", searchReply) != p->user())
				{
					if(p->childCount() == 0)
					{
						int openSlots = p->openSlots();
						p->setOpenSlots("0");
						p->addSource(p->user(), p->fileName(), p->size(), p->hash(), p->url(),
						             openSlots, p->album(), p->duration(), p->bitrate(), p->artist(),
						             p->title(), p->category(), p->directory(), p->mimeTypeComment());
						p->setUser("");
					}
					p->addSource(searchReply);
				}
			}
		}
		else
		{
			QString endSearch = "ITEM(" + QString::number(m_id) + ");";

			if(QString::compare(endSearch, searchReply) == 0)
			{
//				m_summaryLabel->setText(m_textCompleteString + (" (1 result)",
	//			                                                    " (%n results)", m_searchResults->childCount()));
				m_parent->changeTab(this, m_query + (" (finished)"));
				m_moreButton->setEnabled(true);
				m_stopButton->setEnabled(false);
			}
		}
	}
}

void ApollonQueryView::browse()
{
	emit giftBrowseRequest(m_current);
}

void ApollonQueryView::browse(int index)
{
	m_current = m_browseUserMenu->text(index);

	emit giftBrowseRequest(m_current);
}

void ApollonQueryView::closeAllTabs()
{
	emit closetabs();
}

void ApollonQueryView::findItems(const QString &s)
{
	QListViewItem *svi = m_searchResults->firstChild();

	switch(m_filterMode)
	{
		case 0: // contains all of
			while(svi)
			{
				svi->setVisible(ApollonUtils::containsMultiple(svi->text(m_filterColumn), s, true));
				svi = svi->nextSibling();
			}
			break;

		case 1: // contains any of
			while(svi)
			{
				svi->setVisible(ApollonUtils::containsMultiple(svi->text(m_filterColumn), s, false));
				svi = svi->nextSibling();
			}
			break;

		case 2: // doesn't contain
			while(svi)
			{
				svi->setVisible(s.isEmpty() ||
				                !ApollonUtils::containsMultiple(svi->text(m_filterColumn), s, false));
				svi = svi->nextSibling();
			}
			break;
	}
	m_searchResults->triggerUpdate();
	m_clearButton->setEnabled(!s.isEmpty());
	update();
}

void ApollonQueryView::clearFindbox()
{
	m_filterColumn = ApollonSearchListView::columnName;
	m_findBox->clear();
}

void ApollonQueryView::updateFilterColumn(int index)
{
	switch(index)
	{
		case 0:
			m_filterColumn = ApollonSearchListView::columnName;
			break;
		case 1:
			m_filterColumn = ApollonSearchListView::columnUser;
			break;
		case 2:
			m_filterColumn = ApollonSearchListView::columnArtist;
			break;
		case 3:
			m_filterColumn = ApollonSearchListView::columnAlbum;
			break;
		case 4:
			m_filterColumn = ApollonSearchListView::columnTitle;
			break;
		case 5:
			m_filterColumn = ApollonSearchListView::columnDuration;
			break;
		case 6:
			m_filterColumn = ApollonSearchListView::columnMimeType;
			break;
		case 7:
			m_filterColumn = ApollonSearchListView::columnBitrate;
			break;
		case 8:
			m_filterColumn = ApollonSearchListView::columnCategory;
			break;
		case 9:
			m_filterColumn = ApollonSearchListView::columnDirectory;
			break;
	}

	findItems(m_findBox->text());
}

void ApollonQueryView::updateFilterMode(int index)
{
	m_filterMode = index;

	findItems(m_findBox->text());
}

bool ApollonQueryView::itemVisible(QListViewItem *item)
{
	switch(m_filterMode)
	{
		case 0: // contains all of
			return ApollonUtils::containsMultiple(item->text(m_filterColumn), m_findBox->text(), true);

		case 1: // contains any of
			return ApollonUtils::containsMultiple(item->text(m_filterColumn), m_findBox->text(), false);

		case 2: // doesn't contain
			return m_findBox->text().isEmpty() ||
			       !ApollonUtils::containsMultiple(item->text(m_filterColumn), m_findBox->text(), false);
	}

	return true;
}

void ApollonQueryView::keyPressEvent(QKeyEvent *e)
{
	switch(e->state())
	{
		case ControlButton:
			if(e->key() == Key_W)
			{
				deleteThis();
			}
			break;

		case NoButton:
			if(e->key() == Key_Escape)
			{
				stopSearch();
			}
			break;

		default:
			break;
	}
}

void ApollonQueryView::slotDeleteHash(QString hash)
{
	if (m_foundItem)
	{
		ApollonSearchViewItem *p = (hash.isEmpty() ? 0 : m_hashes2items[hash]);
		if (p)
			p->setIcon();
	}
}

