/***************************************************************************
                          apollonsearchitemview.cpp  -  description
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

#include <qstring.h>
#include <qregexp.h>
#include <qpalette.h>
#include <qpainter.h>
#include <qpen.h>

//#include <klocale.h>

#include "apollonsearchlistview.h"
#include "apollonsearchviewitem.h"
#include "apollonutils.h"
#include "giftconnection.h"

ApollonSearchViewItem::ApollonSearchViewItem(QListView *parent, bool child) :
  QListViewItem(parent),
  m_child(child),
  m_availSlots(0)
{

}

ApollonSearchViewItem::ApollonSearchViewItem(QListViewItem *parent, bool child) :
  QListViewItem(parent),
  m_child(child)
{
	m_availSlots = (static_cast<ApollonSearchViewItem*>(parent))->availSlots();
	m_availSlots = 1;
}

ApollonSearchViewItem::~ApollonSearchViewItem()
{
}

QString ApollonSearchViewItem::text(int column) const
{
    QString conv,retval;

	switch(column)
	{
		case ApollonSearchListView::columnName:
			return m_file;

		case ApollonSearchListView::columnSize:
			return ApollonUtils::convertSize(m_size.toULong());

		case ApollonSearchListView::columnAvailability:
			{
            conv.setNum(m_availSlots);
            retval=conv+" slots";
            return (retval);

            }
//			return i18n("1 slot", "%n slots", m_availSlots);

		case ApollonSearchListView::columnUser:
			return (m_children.count() <= 1 ? m_user : QString::number(m_children.count()) +(" users"));

		case ApollonSearchListView::columnAlbum:
			return m_album;

		case ApollonSearchListView::columnDuration:
			if(!m_duration.isEmpty())
			{
				return ApollonUtils::timeHuman(m_duration.toULong());
			}
			else
			{
				return "";
			}

		case ApollonSearchListView::columnBitrate:
			if(!m_bitrate.isEmpty())
			{
				return ApollonUtils::bitrateHuman(m_bitrate.toULong());
			}
			else
			{
				return "";
			}

		case ApollonSearchListView::columnArtist:
			return m_artist;

		case ApollonSearchListView::columnTitle:
			return m_title;

		case ApollonSearchListView::columnCategory:
			return m_category;

		case ApollonSearchListView::columnDirectory:
			return m_directory;

		case ApollonSearchListView::columnMimeType:
			return m_mimeTypeComment;
	}

	return "";
}

QString ApollonSearchViewItem::key(int column, bool /*ascending*/) const
{
	switch(column)
	{
		case ApollonSearchListView::columnRoot:
			return m_url;

		case ApollonSearchListView::columnSize:
			return m_size.rightJustify(15, '0');

		case ApollonSearchListView::columnAvailability:
			return QString::number(m_availSlots).rightJustify(15, '0');

		case ApollonSearchListView::columnUser:
			return QString::number(m_children.count()).rightJustify(15, '0');

		case ApollonSearchListView::columnDuration:
			return m_duration.rightJustify(15, '0');

		case ApollonSearchListView::columnBitrate:
			return m_bitrate.rightJustify(15, '0');

		default:
			return text(column);
	}
}

void ApollonSearchViewItem::addSource(const QString &searchReply)
{
	QString user = giFTConnection::searchKey("user", searchReply);

	if(m_children.find(user) == 0)
	{
		// this is a unique child, add it to the listviewitem
		ApollonSearchViewItem *child = new ApollonSearchViewItem(this, true);
		m_children.insert(user, child);

		child->setFileName(giFTConnection::searchKey("file", searchReply).section('/', -1, -1));
		child->setSize(m_size);

		child->setUser(user);
		child->setHash(m_hash);
		child->setUrl(giFTConnection::searchKey("url", searchReply));

		child->setOpenSlots(giFTConnection::searchKey("availability", searchReply));
		m_availSlots += child->openSlots();

		child->setAlbum(giFTConnection::searchKey("album", searchReply));
		child->setDuration(giFTConnection::searchKey("duration", searchReply));
		child->setBitrate(giFTConnection::searchKey("bitrate", searchReply));

		child->setArtist(giFTConnection::searchKey("artist", searchReply));
		child->setTitle(giFTConnection::searchKey("title", searchReply));
		child->setCategory(giFTConnection::searchKey("category", searchReply));
		child->setDirectory(giFTConnection::searchKey("directory", searchReply));
		child->setMimeTypeComment(m_mimeTypeComment);
	}
}

void ApollonSearchViewItem::addSource(const QString &user, const QString &file, const QString &size, const QString &hash,
                                      const QString &url, int availability, const QString &album, const QString &duration,
	                                 const QString &bitrate, const QString &artist, const QString &title,
	                                 const QString &category, const QString &directory, const QString mimeTypeComment)
{
	if(m_children.find(user) == 0)
	{
		// this is a unique child, add it to the listviewitem
		ApollonSearchViewItem *child = new ApollonSearchViewItem(this, true);
		m_children.insert(user, child);

		child->setFileName(file);
		child->setSize(size);

		child->setUser(user);
		child->setHash(hash);
		child->setUrl(url);

		child->setOpenSlots(QString::number(availability));
		m_availSlots += child->openSlots();

		child->setAlbum(album);
		child->setDuration(duration);
		child->setBitrate(bitrate);

		child->setArtist(artist);
		child->setTitle(title);
		child->setCategory(category);
		child->setDirectory(directory);
		child->setMimeTypeComment(mimeTypeComment);
	}
}

void ApollonSearchViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align)
{
	const QColor grey(180, 180, 180);
	QColorGroup newCG(cg);
	newCG.setColor(QColorGroup::Text, grey);
	if(m_availSlots == 0)
		QListViewItem::paintCell(p, newCG, column, width, align);
	else
		QListViewItem::paintCell(p, cg, column, width, align);
}


void ApollonSearchViewItem::setIcon()
{
	
	if(m_protocol == "FastTrack")
	{
//		setPixmap(0, m_loader->loadIcon("kazaa", KIcon::Small));
	}
	else if(m_protocol == "OpenNap")
	{
	//	setPixmap(0, m_loader->loadIcon("napster", KIcon::Small));
	}
	else if(m_protocol == "Gnutella")
	{
	//	setPixmap(0, m_loader->loadIcon("gnutella", KIcon::Small));
	}
	else if(m_protocol == "OpenFT")
	{
	//	setPixmap(0, m_loader->loadIcon("openft", KIcon::Small));
	}
	else if(m_protocol == "SoulSeek")
	{
	//	setPixmap(0, m_loader->loadIcon("soulseek", KIcon::Small));
	}

}

void ApollonSearchViewItem::setDownloadIcon()
{
	if(m_protocol == "FastTrack")
	{
//		setPixmap(0, m_loader->loadIcon("kazaadown", KIcon::Small));
	}
	else if(m_protocol == "OpenNap")
	{
//		setPixmap(0, m_loader->loadIcon("napsterdown", KIcon::Small));
	}
	else if(m_protocol == "Gnutella")
	{
//		setPixmap(0, m_loader->loadIcon("gnutelladown", KIcon::Small));
	}
	else if(m_protocol == "OpenFT")
	{
//		setPixmap(0, m_loader->loadIcon("openftdown", KIcon::Small));
	}
	else if(m_protocol == "SoulSeek")
	{
//		setPixmap(0, m_loader->loadIcon("soulseekdown", KIcon::Small));
	}

}


