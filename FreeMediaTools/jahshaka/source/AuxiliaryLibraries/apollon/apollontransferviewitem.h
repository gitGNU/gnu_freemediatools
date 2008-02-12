/***************************************************************************
                          apollontransferviewitem.h  -  description
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

#ifndef APOLLONTRANSFERVIEWITEM_H
#define APOLLONTRANSFERVIEWITEM_H

#include <qlistview.h>
#include <qpixmap.h>

#include "apollonutils.h"

class QString;

class ApollonTransferViewItem : public QListViewItem
{
	public:
		enum Column
		{
			columnName = 0,
			columnSize,
			columnTransmit,
			columnBandwidth,
			columnStatus,
			columnETA
		};

		ApollonTransferViewItem(QListView *parent, const QString &updown, int ID = -1, const QString &protocolName = QString::null);
		ApollonTransferViewItem(QListViewItem *parent, int ID = -1);
		~ApollonTransferViewItem();

		// ID property
		inline void setID(int ID);
		inline int id() const;

		// hash property
		inline void setHash(const QString &hash);
		inline QString hash() const;

		// name property
		inline void setName(const QString &name);
		inline QString name() const;

		// url property
		inline void setUrl(const QString &url);
		inline QString url() const;

		// size property
		inline void setSize(int size);
		inline int size() const;

		// transmit property
		inline void setTransmit(int transmit);
		inline int transmit() const;

		// bandwidth property
		inline void setBandwidth(const QString &bandwidth);
		inline QString bandwidth() const;

		// status property
		void setStatus(QString status);
		inline QString status() const;

		// progress property
		inline void setProgress(float progress);
		inline float progress() const;

		// ETA property
		inline void setETA(int eta);
		inline int eta() const;

		//per source: set start
		inline void setStart(int start);
		inline int start() const;

		// string for updating sources
		inline void setUpdateString(const QString &updateString);
		inline QString updateString() const;

		inline void setProtocolName(QString);
		inline QString protocolName();

		inline ApollonTransferViewItem *parent() const;
		inline ApollonTransferViewItem *firstChild() const;
		inline ApollonTransferViewItem *nextSibling() const;

		QString key(int column, bool ascending) const;
		int compare(QListViewItem *item, int column, bool ascending) const;

		static ApollonTransferViewItem *getItem(const QListView *lv, int ID);

		void updateIcon(const QString &protocolName, const QString &updown);
		void pauseItem();
		void unpauseItem();

		inline bool isPaused() { return m_isPaused; }

		void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

	private:
		int      m_id;
		QString  m_hash;
		QString  m_name;
		QString  m_url;
		int      m_size;
		int      m_transmit;
		QString  m_bandwidth;
		QString  m_status;
		int      m_eta;
		QString  m_updateString;
		bool     m_updated;
		QString  m_icon;
		bool     m_isPaused;
		QPixmap  m_pausePix, m_pix;
		float    m_progress;
		int      m_lastWidth;
		QPixmap  m_gradient;
		QString  m_protocolName;
		int      m_start;
};

void ApollonTransferViewItem::setID(int ID)
{
	m_id = ID;
}

int ApollonTransferViewItem::id() const
{
	return m_id;
}

void ApollonTransferViewItem::setHash(const QString &hash)
{
	m_hash = hash;
}

QString ApollonTransferViewItem::hash() const
{
	return m_hash;
}

void ApollonTransferViewItem::setName(const QString &name)
{
	m_name = name;
	setText(columnName, m_name);
}

QString ApollonTransferViewItem::name() const
{
	return m_name;
}

void ApollonTransferViewItem::setUrl(const QString &url)
{
	m_url = url;
}

QString ApollonTransferViewItem::url() const
{
	return m_url;
}

void ApollonTransferViewItem::setSize(int size)
{
	m_size = size;
	setText(columnSize, (m_size == -1 ? QString("") : ApollonUtils::convertSize(m_size)));
}

int ApollonTransferViewItem::size() const
{
	return m_size;
}

void ApollonTransferViewItem::setTransmit(int transmit)
{
	m_transmit = transmit;
	setText(columnTransmit, (m_transmit == -1 ? QString("") : ApollonUtils::convertSize(m_transmit)));
}

int ApollonTransferViewItem::transmit() const
{
	return m_transmit;
}

void ApollonTransferViewItem::setBandwidth(const QString &bandwidth)
{
	m_bandwidth = bandwidth;
	setText(columnBandwidth, m_bandwidth);
}

QString ApollonTransferViewItem::bandwidth() const
{
	return m_bandwidth;
}

QString ApollonTransferViewItem::status() const
{
	return m_status;
}

float ApollonTransferViewItem::progress() const
{
	return m_progress;
}

void ApollonTransferViewItem::setProgress(float progress)
{
	m_progress = progress;
	repaint();
}

void ApollonTransferViewItem::setETA(int eta)
{
	m_eta = eta;
	setText(columnETA, (m_eta == -1 ? QString("") : ApollonUtils::timeHuman(m_eta)));
}

int ApollonTransferViewItem::eta() const
{
	return m_eta;
}

void ApollonTransferViewItem::setStart(int start) 
{
         m_start = start;
}


int ApollonTransferViewItem::start() const
{
         return m_start;
}

void ApollonTransferViewItem::setUpdateString(const QString &updateString)
{
	m_updateString = updateString;
}

QString ApollonTransferViewItem::updateString() const
{
	return m_updateString;
}


void ApollonTransferViewItem::setProtocolName(QString p)
{
         m_protocolName = p;
}

QString ApollonTransferViewItem::protocolName()
{
         return m_protocolName;
}

ApollonTransferViewItem *ApollonTransferViewItem::parent() const
{
	return (ApollonTransferViewItem *) QListViewItem::parent();
}

ApollonTransferViewItem *ApollonTransferViewItem::firstChild() const
{
	return (ApollonTransferViewItem *) QListViewItem::firstChild();
}

ApollonTransferViewItem *ApollonTransferViewItem::nextSibling() const
{
	return (ApollonTransferViewItem *) QListViewItem::nextSibling();
}



#endif
