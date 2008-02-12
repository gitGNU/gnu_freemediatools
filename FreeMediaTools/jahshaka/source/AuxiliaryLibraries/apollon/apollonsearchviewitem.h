/***************************************************************************
                          apollonsearchitemview.h  -  description
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

#ifndef APOLLONSEARCHVIEWITEMS_H
#define APOLLONSEARCHVIEWITEMS_H

//#include <kiconloader.h>

#include <qlistview.h>
#include <qstring.h> 
#include <qdict.h>

class ApollonSearchViewItem : public QListViewItem
{
	public:
		ApollonSearchViewItem(QListView *parent, bool child = false);
		ApollonSearchViewItem(QListViewItem *parent, bool child = false);
		~ApollonSearchViewItem();

		inline ApollonSearchViewItem *firstChild() { return (ApollonSearchViewItem *) QListViewItem::firstChild(); }
		inline ApollonSearchViewItem *nextSibling() { return (ApollonSearchViewItem *) QListViewItem::nextSibling(); }

		QString text(int col) const;

		QString key(int column, bool ascending) const;

		inline const QString &user()     { return m_user; }
		inline const QString &hash()     { return m_hash; }
		inline const QString &size()     { return m_size; }
		inline const QString &url()      { return m_url; }
		inline const QString &fileName() { return m_file; }
		inline const QString &album()    { return m_album; }
		inline const QString &duration() { return m_duration; }
		inline const QString &bitrate()  { return m_bitrate; }
		inline const QString &artist()   { return m_artist; }
		inline const QString &title()    { return m_title; }
		inline const QString &category() { return m_category; }
		inline const QString &directory(){ return m_directory; }
		inline const QString &mimeTypeIcon() { return m_mimeTypeIcon; } //contains the path
		inline const QString &mimeTypeComment() { return m_mimeTypeComment; }
		inline bool isChild()            { return m_child; }
		inline int openSlots()           { return m_openSlots.toInt(); }
		inline int availSlots()          { return m_availSlots; }
		inline int childCount()          { return m_children.count(); }
		inline const QString &protocol() { return m_protocol; }

		void addSource(const QString &searchReply);
		void addSource(const QString &user, const QString &file, const QString &size, const QString &hash,
		               const QString &url, int availability, const QString &album, const QString &duration,
		               const QString &bitrate, const QString &artist, const QString &title,
		               const QString &category, const QString &directory, const QString mimeTypeComment);

		virtual void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

		inline void setUser(QString user)           { m_user = user; }
		inline void setHash(QString hash)           { m_hash = hash; }
		inline void setSize(QString size)           { m_size = size; }
		inline void setUrl(QString url)             { m_url = url; }
		inline void setFileName(QString file)       { m_file = file; }
		inline void setDuration(QString duration)   { m_duration = duration; }
		inline void setOpenSlots(QString openSlots) { m_openSlots = openSlots;
		                                              m_availSlots = openSlots.toInt(); }
		inline void setAlbum(QString album)         { m_album = album; }
		inline void setBitrate(QString bitrate)     { m_bitrate = bitrate; }
		inline void setArtist(QString artist)       { m_artist = artist; }
		inline void setTitle(QString title)         { m_title = title; }
		inline void setCategory(QString category)   { m_category = category; }
		inline void setDirectory(QString directory) { m_directory = directory; }
		inline void setMimeTypeIcon(QString mimeTypeIcon)   { m_mimeTypeIcon = mimeTypeIcon; } //contains the path
		inline void setMimeTypeComment(QString mimeTypeComment)   { m_mimeTypeComment = mimeTypeComment; }
		inline void setProtocol(QString prot) { m_protocol = prot; }
		
		void setIcon();
		void setDownloadIcon();

	protected:
		QString m_user;
		QString m_hash;
		QString m_size;
		QString m_url;
		QString m_file;
		QString m_duration;
		QString m_openSlots;
		QString m_album;
		QString m_bitrate;
		QString m_artist;
		QString m_title;
		QString m_category;
		QString m_directory;
		QString m_mimeTypeIcon;
		QString m_mimeTypeComment;
		QString m_protocol;
		QDict<ApollonSearchViewItem> m_children;

	private:
		bool m_child;
		int m_availSlots;
//		KIconLoader     *m_loader;
};

#endif
