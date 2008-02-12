/***************************************************************************
                          giftSocket.h  -  subclass of QSocket for reading
                                           GiFT streams from a socket
                             -------------------
    begin                : Sun August 3 2003
    copyright            : (C) 2003 by Arend van Beelen jr.
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

#ifndef __GIFTSOCKET_H
#define __GIFTSOCKET_H

#include <qsocket.h>
#include <qstring.h>
#include <qtextcodec.h>

namespace Apollon
{

class giFTSocket : public QSocket
{
	Q_OBJECT

	public:
		giFTSocket(QObject *parent = 0, const char *name = 0);

		bool canReadCommandBlock();
		QString readCommandBlock();
		virtual Q_LONG writeBlock(QString data, Q_ULONG len);

	private:
		QTextCodec *m_codec;

	signals:

	public slots:
};

}

#endif
