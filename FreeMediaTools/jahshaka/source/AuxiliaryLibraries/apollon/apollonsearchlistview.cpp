/***************************************************************************
                          ApollonSearchListView.cpp  -  description
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
//#include <kiconloader.h>
//#include <kicontheme.h>
//#include <kaction.h>
//#include <kdebug.h>
//#include <klocale.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qheader.h>

#include "apollonsearchlistview.h"
#include "apollonsearchviewitem.h"
const int defaultColumnWidth = 50;

ApollonSearchListView::ApollonSearchListView(QWidget *parent) :
  ApollonListView(parent),
  QToolTip(viewport())
  //,m_headerMenu(NULL)
{
	// create all the columns
	addColumn("");
	setColumnWidthMode(0, QListView::Manual);
	setColumnWidth(0, 55);

	addColumn(("File Name"));
	setColumnWidthMode(1, QListView::Manual);
	setColumnWidth(1, 350);

	addColumn(("Size"));
	setColumnWidthMode(2, QListView::Manual);
#ifndef WIN32
	setColumnAlignment(2, Qt::AlignRight);
#endif
	setColumnWidth(2, 80);

	addColumn(("Availability"));
	setColumnWidthMode(3, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(3, Qt::AlignRight);
#endif
	setColumnWidth(3, 50);

	addColumn(("User"));
	setColumnWidthMode(4, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(4, Qt::AlignRight);
#endif
	setColumnWidth(4, 80);

	addColumn(("Artist"));
	setColumnWidthMode(5, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(5, Qt::AlignLeft);
#endif
	setColumnWidth(5, 80);

	addColumn(("Album"));
	setColumnWidthMode(6, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(6, Qt::AlignLeft);
#endif	
	setColumnWidth(6, 80);

	addColumn(("Title"));
	setColumnWidthMode(7, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(7, Qt::AlignLeft);
#endif
	setColumnWidth(7, 80);

	addColumn(("Duration"));
	setColumnWidthMode(8, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(8, Qt::AlignRight);
#endif
	setColumnWidth(8, 50);

	addColumn(("File Type"));
	setColumnWidthMode(9, QListView::Manual);
#ifndef WIN32
	setColumnAlignment(9, Qt::AlignLeft);
#endif
	setColumnWidth(9, 0);

	addColumn(("Bitrate"));
	setColumnWidthMode(10, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(10, Qt::AlignRight);
#endif
	setColumnWidth(10, 50);

	addColumn(("Category"));
	setColumnWidthMode(11, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(11, Qt::AlignLeft);
#endif	
	setColumnWidth(11, 0);

	addColumn(("Directory"));
	setColumnWidthMode(12, QListView::Manual);
#ifndef WIN32	
	setColumnAlignment(12, Qt::AlignLeft);
#endif	
	setColumnWidth(12, 0);

	setSorting(2, false);

	setSelectionMode(QListView::Extended);
	setShowSortIndicator(true);
	setAllColumnsShowFocus(true);
	setRootIsDecorated(true);

	// initialize the header menu
//	m_columnVisibleAction = new KActionMenu(("&Show Columns"), this, "showColumns");

//	m_headerMenu = m_columnVisibleAction->popupMenu();
//	m_headerMenu->insertTitle(("Show"));
//	m_headerMenu->setCheckable(true);

//	for(int i = columnName; i < header()->count(); ++i)
//	{
//		m_headerMenu->insertItem(header()->label(i), i - 1);
//		m_headerMenu->setItemChecked(i - 1, columnWidth(i) > 0);
//	}

//	connect(m_headerMenu, SIGNAL(activated(int)), this, SLOT(toggleColumnVisible(int)));
	
	setWakeUpDelay(800);
	m_wakeUpTimer = new QTimer();
	connect(m_wakeUpTimer, SIGNAL(timeout()), SLOT(slotShowTip()));
}

ApollonSearchListView::~ApollonSearchListView()
{
}

void ApollonSearchListView::maybeTip(const QPoint& p)
{
	ApollonSearchViewItem* item = (ApollonSearchViewItem*)itemAt(p);
	if (! item) {
		m_wakeUpTimer->stop();
		return;
	}
	
	m_rect = itemRect(item);
	if (! m_rect.isValid()) {
		m_wakeUpTimer->stop();
		return;
	}
	m_rect.setWidth(width());
	
//	KIconLoader *loader = KGlobal::iconLoader();
	//QMimeSourceFactory::defaultFactory()->setPixmap( "mimetypeicon",  loader->loadIcon(item->mimeTypeIcon(), KIcon::MainToolbar));
	/*
	m_tipContent = "<big><img source=\"mimetypeicon\"> " + item->fileName() + "</big> <hr>" +
		( item->title().isEmpty() ? "" :
			"<b>"+"Title" + ": &nbsp;</b>" + item->title() + "<br>" ) +
		( item->user().isEmpty() ? "" :
			"<b>"+"User" + ": &nbsp;</b>" + item->user() + "<br>" ) +
		( item->album().isEmpty() ? "" :
			"<b>"+"Album" + ": &nbsp;</b>" + item->album() + "<br>" ) +
		( item->artist().isEmpty() ? "" :
			"<b>"+"Artist" + ": &nbsp;</b>" + item->artist() + "<br>"  ) +
		( item->category().isEmpty() ? "" :
			"<b>"+"Category" + ": &nbsp;</b>" + item->category() + "<br>" ) +
		( item->size().isEmpty() ? "" :
			"<b>"+"Size" + ": &nbsp;</b>" + item->text(ApollonSearchListView::columnSize) + "<br>" ) +
		( item->duration().isEmpty() ?  "" :
			"<b>"+"Duration" + ": &nbsp;</b>" + item->text(ApollonSearchListView::columnDuration) + "<br>" ) +
		( item->bitrate().isEmpty() ? "" :
			"<b>"+"Bitrate" + ": &nbsp;</b>" + item->text(ApollonSearchListView::columnBitrate) + "<br>" ) +
		"<b>"+"File Type" + ": &nbsp;</b>" + item->mimeTypeComment() + "<br>";
		*/
	m_wakeUpTimer->start(m_wakeUpDelay);
}

void ApollonSearchListView::slotShowTip() 
{
	tip(m_rect, m_tipContent);
}
/*
void ApollonSearchListView::restoreLayout(KConfig *config, const QString &group)
{
	KListView::restoreLayout(config, group);

	for(int i = columnName; i < header()->count(); ++i)
	{
		m_headerMenu->setItemChecked(i - 1, columnWidth(i) > 0);
	}
}
*/

void ApollonSearchListView::setColumnWidth(int column, int w)
{
    QListView::setColumnWidth(column, w);

//	if(m_headerMenu)
	//	m_headerMenu->setItemChecked(column - 1, w > 0);
}

void ApollonSearchListView::toggleColumnVisible(int column)
{
	column++;

	if(columnWidth(column) == 0)
	{
		setColumnWidth(column, defaultColumnWidth);
	}
	else
	{
		setColumnWidth(column, 0);
	}
}

bool ApollonSearchListView::eventFilter(QObject *watched, QEvent *e)
{

	if(watched == header())
	{
		if(e->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *me = static_cast<QMouseEvent*>(e);

			switch(me->button())
			{
#ifdef WIN32
			case QEvent::RightButton:
#else
			case RightButton:
#endif		
	//			m_headerMenu->popup(QCursor::pos());
					return true;

				default:
					break;
			}
		}
	}

	return QListView::eventFilter(watched, e);

}
