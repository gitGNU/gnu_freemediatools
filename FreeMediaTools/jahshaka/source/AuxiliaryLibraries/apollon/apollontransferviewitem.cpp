/***************************************************************************
                          apollontransferviewitem.cpp  -  description
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

#include <assert.h>

//#include <kglobal.h>
//#include <kiconloader.h>
//#include <klocale.h>
#include <qpainter.h>
#include <qpen.h>
//#include <kpixmapeffect.h>

#include "apollontransferviewitem.h"

ApollonTransferViewItem::ApollonTransferViewItem(QListView *parent, const QString &updown, int ID, const QString &protocolName) :
  QListViewItem(parent),
  m_id(ID),
  m_hash(""),
  m_name(""),
  m_size(-1),
  m_transmit(-1),
  m_bandwidth(""),
  m_status(""),
  m_eta(-1),
  m_updateString(""),
  m_lastWidth(0)
{
	m_updated = false;
	updateIcon(protocolName, updown);
	m_updated = false;
	m_isPaused = false;
//	KIconLoader *loader = KGlobal::iconLoader();
//	m_pausePix = loader->loadIcon("player_pause", KIcon::Small);
	m_protocolName = protocolName;
}

ApollonTransferViewItem::ApollonTransferViewItem(QListViewItem *parent, int ID) :
  QListViewItem(parent),
  m_id(ID),
  m_hash(""),
  m_name(""),
  m_size(-1),
  m_transmit(-1),
  m_bandwidth(""),
  m_status(""),
  m_eta(-1),
  m_updateString("")
{
	m_updated = false;
	m_isPaused = false;
//	KIconLoader *loader = KGlobal::iconLoader();
//	m_pausePix = loader->loadIcon("player_pause", KIcon::Small);
}

ApollonTransferViewItem::~ApollonTransferViewItem()
{
}

void ApollonTransferViewItem::setStatus(QString status)
{
	m_status = status;

//	KIconLoader *loader = KGlobal::iconLoader();

	if(m_status == "Completed")
	{
		//setPixmap(0, loader->loadIcon("ok", KIcon::Small));
		m_bandwidth = "";
		m_eta = -1;
	}
	else if(m_status == "Canceled")
	{
		//setPixmap(0, loader->loadIcon("no", KIcon::Small));
		m_bandwidth = "";
		m_eta = -1;
	}
	m_lastWidth = -1; // this generates a new gradient for the progress bar
}

ApollonTransferViewItem *ApollonTransferViewItem::getItem(const QListView *lv, int ID)
{
	assert(lv != 0);

	ApollonTransferViewItem *item = (ApollonTransferViewItem *) lv->firstChild();
	while(item != 0)
	{
		if(item->id() == ID)
		{
			break;
		}
		item = item->nextSibling();
	}

	return item;
}


void ApollonTransferViewItem::updateIcon(const QString &protocolName, const QString &updown)
{
	if(m_updated)
	{
		return;
	}

//	KIconLoader *loader = KGlobal::iconLoader();

	if(updown == "download")
	{
		if(protocolName=="FastTrack")
		{
			m_icon = "kazaadown";
		}
		else if(protocolName=="Gnutella")
		{
			m_icon = "gnutelladown";
		}
		else if(protocolName=="OpenFT")
		{
			m_icon = "openftdown";
		}
		else if(protocolName=="OpenNap")
		{
			m_icon = "napsterdown";
		}
		else if(protocolName=="SoulSeek")
		{
			m_icon = "soulseekdown";
		}
		else
		{
			m_icon = "1downarrow";
		}
	}
	else if(updown == "upload")
	{
		if(protocolName=="FastTrack")
		{
			m_icon = "kazaaup";
		}
		else if(protocolName=="Gnutella")
		{
			m_icon = "gnutellaup";
		}
		else if(protocolName=="OpenFT")
		{
			m_icon = "openftup";
		}
		else if(protocolName=="SoulSeek")
		{
			m_icon = "soulseekup";
		}
		else
		{
			m_icon = "1uparrow";
		}
	}

//	m_pix = loader->loadIcon(m_icon, KIcon::Small);
//	setPixmap(0, m_pix);
	m_updated = true;
}

void ApollonTransferViewItem::pauseItem()
{
//	setPixmap(0, m_pausePix);
	m_isPaused = true;
	m_updated = true; // don't change the icon...
}

void ApollonTransferViewItem::unpauseItem()
{
//	setPixmap(0, m_pix);
	m_isPaused = false;
	m_updated = true;
}

QString ApollonTransferViewItem::key(int column, bool /*ascending*/) const
{
	QString temp1;
	switch(column)
	{
		case columnName:
			return m_name;

		case columnSize:
			temp1 = temp1.setNum(size());
			return temp1;

		case columnTransmit:
			temp1 = temp1.setNum(transmit());
			return temp1;

		case columnBandwidth:
			temp1 = bandwidth();
			if(temp1 == "--") return "0.00";
			temp1 = temp1.remove(" KB/s");
			return temp1;

		case columnETA:
			temp1 = temp1.setNum(eta());
			return temp1;

		case columnStatus:
			if (m_progress != -1)
				temp1.setNum((int) m_progress);
			else
				temp1 = m_status;
			return temp1;

		default:
			return text(column);
	}

	return "";
}

int ApollonTransferViewItem::compare(QListViewItem *item, int column, bool ascending) const
{
	if(column != columnName)
	{
		QString current = key(column, ascending);
		QString next = item->key(column, ascending);

		if((current.toDouble() - next.toDouble()) > 0)
			return 1;
		else
			return -1;
	}
	else
	{
		return QListViewItem::compare(item, column, ascending);
	}
}

void ApollonTransferViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align)
{
	// status bars
	if(column == columnStatus)
	{
		int fillwidth;
		p->setPen(cg.foreground());
		p->drawRect(1, 1, width - 2, height() - 2);
		p->setPen(QColor(255, 255, 255));
		p->drawRect(0, 0, width, height());
		p->drawRect(2, 2, width - 4, height() - 4);

		if(m_lastWidth != width)
		{
			m_lastWidth = width;
			m_gradient.resize(width, height());
			QColor color;

			if(m_progress != -1)
				//{ color = cg.highlight(); }
				{ color = QColor(0, 0, 255); /*color = cg.highlight();*/ }
			else
				{ color = cg.background();}

			//ApollonUtils::gradient((QPixmap &) m_gradient, color, color.light(), true);
			ApollonUtils::gradient(m_gradient, color, color.light(), true,1);
		}
		if(parent() == 0)
		{
			if(m_progress == -1)
				fillwidth = width - 6;
			else
				fillwidth = (int) (m_progress / 100.0 * (width - 6));

            bitBlt(p->device(), (int) p->worldMatrix().dx() + 3, (int) p->worldMatrix().dy() + 3, &m_gradient, 0, 0, fillwidth, height() - 6, Qt::CopyROP, false);

            p->fillRect(3 + fillwidth, 3, width - fillwidth - 6, height() - 6, QColor(255, 255, 255));

            //p->setPen(cg.foreground());
            p->setPen(QColor(0, 0, 0));
			if(m_progress != -1)
				p->drawText(0, 0, width, height(), Qt::AlignCenter, QString::number((int) m_progress) + "%");
			else
				p->drawText(0, 0, width, height(), Qt::AlignCenter, m_status);
		}
		else
		{
			fillwidth = (int) (m_progress / 100.0 * (width - 6));

            bitBlt(p->device(), (int) p->worldMatrix().dx() + 3, (int) p->worldMatrix().dy() + 3, &m_gradient, 0, 0, fillwidth, height() - 6, Qt::CopyROP, false);

            p->fillRect(3 + fillwidth, 3, width - fillwidth - 6, height() - 6, QColor(255, 255, 255));

            //p->setPen(cg.foreground());
            p->setPen(QColor(0, 0, 0));
			p->drawText(0, 0, width, height(), Qt::AlignCenter, m_status);
		}
		listView()->repaint();
	}
	// other cells
	else
	{
		QListViewItem::paintCell(p, cg, column, width, align);
	}
}
