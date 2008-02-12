/***************************************************************************
                          giftSocket.cpp - subclass of QSocket for reading
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

#include "giftsocket.h"
//#include <kconfig.h>
//#include <kglobal.h>
//#include <klocale.h>

namespace Apollon
{

giFTSocket::giFTSocket(QObject *parent, const char *name) :
  QSocket(parent, name)
{
/*
    KGlobal::config()->setGroup("Codecs");
	QString TextCodec = KGlobal::config()->readEntry("TextCodec");
	m_codec = QTextCodec::codecForName(TextCodec);
	if(!m_codec)
	{
		m_codec = QTextCodec::codecForLocale();
	}
    */
    m_codec = QTextCodec::codecForLocale();

}

bool giFTSocket::canReadCommandBlock()
{
	bool canRead = false;
	int character;
	int prev_character = 0;
	QString buffer;

	while((character = getch()) != -1)
	{
		buffer += character;
		if(character == ';' && prev_character != '\\')
		{
			canRead = true;
			break;
		}
		prev_character = character;
	}

	while(!buffer.isEmpty())
	{
		
#ifndef WIN32
		ungetch(((const QString) buffer)[buffer.length() - 1]);
#endif
		buffer.truncate(buffer.length() - 1);
	}

	return canRead;
}

QString giFTSocket::readCommandBlock()
{
	int character;
	int prev_character = 0;
	QString buffer;

	while((character = getch()) != -1)
	{
		buffer += character;
		if(character == ';' && prev_character != '\\')
		{
			return m_codec->toUnicode(buffer);
		}
		prev_character = character;
	}

	while(!buffer.isEmpty())
	{
#ifndef WIN32
		ungetch(((const QString) buffer)[buffer.length() - 1]);
#endif
		buffer.truncate(buffer.length() - 1);
	}

	return QString("");
}

Q_LONG giFTSocket::writeBlock(QString data , Q_ULONG len)
{
	QCString localdata = m_codec->fromUnicode(data);

	return QSocket::writeBlock(localdata, len);
}

}
