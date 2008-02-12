/***************************************************************************
                          giftconnection.cpp  -  description
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

#include <stdlib.h>

#ifndef WIN32
#include <unistd.h>
#endif

#include <signal.h>

//#include <kdebug.h>

#include "giftconnection.h"

#include "config.h"

using namespace Apollon;

giFTConnection::giFTConnection()
{
	paramsSet = FALSE;

	connect(&socket, SIGNAL( connected() ), 	this, SIGNAL( connected() ) ) ;
	connect(&socket, SIGNAL( readyRead() ),		this, SLOT( sockReadReady() ) );
	connect(&socket, SIGNAL( connectionClosed() ),	this, SLOT( sockClosed() ) );
	connect(&socket, SIGNAL( error(int) ), 		this, SLOT( sockError(int) ) );

	ID = 900;
#ifndef WIN32
	giFT_pid = 0;
#endif
	m_debugOutput = false;
}

giFTConnection::~giFTConnection()
{
}


void giFTConnection::startgiFT()
{
#ifndef WIN32
	giFT_pid = fork();

	if(giFT_pid == 0)
	{
		//char *program[2] = { "giftd", "-d" };

        //execlp("giftd", "giftd", '\0');
	}
	else
	{
		emit sigGiFTPID(giFT_pid);
	}
#endif
}

// make a connection to the giFT daemon.
bool giFTConnection::connectToServer()
{
	if(paramsSet != true && isConnected() != true)
		return false;

    debug("connecting...");

	socket.connectToHost(m_host, (Q_UINT16) m_port);

	makeAttachRequest();

	return true;
}

void giFTConnection::setParams(const QString &host, int port, bool remote)
{
	m_host = host;
	m_port = port;
	m_remoteConnection = remote;
	paramsSet = true;
}

void giFTConnection::sockReadReady()
{
	while( socket.canReadCommandBlock() )
	{
		QString line = socket.readCommandBlock();

		if(m_debugOutput && line.find("CHGDOWNLOAD") == -1 && line.find("STATS") == -1)
			debug( "=> from daemon: " + line );

		if ( line.find("STATS") > -1 && line.find("STATS") < 5)
		{
            debug(line);
			emit giftStatusReply( line );
		}
		if ( line.find("ITEM") > -1 && line.find("ITEM") < 5)
		{
			emit giftItemReply( line );
		}
		if ( line.find("ADDDOWNLOAD") > -1 && line.find("ADDDOWNLOAD") < 5)
		{
			emit giftAddDownloadReply( line );
		}
		if ( line.find("ADDUPLOAD") > -1 && line.find("ADDUPLOAD") < 5)
		{
			emit giftAddUploadReply( line );
		}
		if ( line.find("ADDSOURCE") > -1 && line.find("ADDSOURCE") < 5)
		{
			emit giftAddSourceReply( line );
		}
		if ( line.find("DELSOURCE") > -1 && line.find("DELSOURCE") < 5)
		{
			emit giftDelSourceReply( line );
		}
		if ( line.find("CHGDOWNLOAD") > -1 && line.find("CHGDOWNLOAD") < 5)
		{
			emit giftChgDownloadReply( line );
		}
		if ( line.find("CHGUPLOAD") > -1 && line.find("CHGUPLOAD") < 5)
		{
			emit giftChgUploadReply( line );
		}
		if ( line.find("DELDOWNLOAD") > -1 && line.find("DELDOWNLOAD") < 5)
		{
			emit giftDelDownloadReply( line );
		}
		if ( line.find("DELUPLOAD") > -1 && line.find("DELUPLOAD") < 5)
		{
			emit giftDelUploadReply( line );
		}
		if ( line.find("SHARE") > -1 && line.find("SHARE") < 5)
		{
			emit giftShareReply( line );
		}
	}
}

void giFTConnection::sockClosed()
{
	emit disconnected();
}

void giFTConnection::sockError(int error)
{
	emit socketError(error);
}

bool giFTConnection::isConnected()
{
	return (socket.state() == QSocket::Connection);
}


void giFTConnection::restartDaemon()
{
	//kill(giFT_pid, 1);
	system("killall giftd");
}

void giFTConnection::makeAttachRequest()
{
	QString request;

	/*
	  request = "ATTACH client(giFTcurs";
	//request.append(PACKAGE);
	request.append(") version(0.6.0");
	//request.append(VERSION);
	request.append(") profile(mkoebele);");
	*/
	request = "ATTACH client(";
	request.append(PACKAGE);
	request.append(") version(");
	request.append(VERSION);
	request.append(") profile(mkoebele);");

	addRequest( request );
}

void giFTConnection::makeStatsRequest()
{
	QString request;

	request = "STATS;";

	addRequest( request );
}

void giFTConnection::makeSearchRequest( int ID, int index, QString text )
{
	QString request, realm;

	//QString strID(ID);

	//realm can be: everything, audio, video, images, text documents, software

	switch (index)
	{
		case 0:
			realm = "everything";
			break;
		case 1:
			realm = "audio";
			break;
		case 2:
			realm = "video";
			break;
		case 3:
			realm = "image";
			break;
		case 4:
			realm = "text";
			break;
		case 5:
			realm = "application";
			break;
		/*
		These are not allowed as realm
		case 6:
			realm = "user";
			break;
		case 7:
			realm = "hash";
			break;
		*/

	}

	request = "SEARCH (" + QString::number(ID) + ") query (" + escapeStr(text) + ") realm (" + escapeStr(realm) + ");";

	addRequest( request );
}

void giFTConnection::makeCancelSearchRequest( int ID )
{
	QString request;

	request = "SEARCH (" + QString::number(ID) + ") action (cancel);";

	addRequest( request );
}

void giFTConnection::makeCancelBrowseRequest( int ID )
{
	QString request;

	request = "BROWSE (" + QString::number(ID) + ") action (cancel);";

	addRequest( request );
}

void giFTConnection::makeLocateRequest( int ID, QString hash )
{
	QString request;

	if (!hash.contains("null"))
	  request = "LOCATE (" + QString::number(ID) + ") query (" + escapeStr(hash) + ");";

	addRequest( request );
}

void giFTConnection::makeTransferRequest( int ID, QString action )
{
	QString request;

	request = "TRANSFER (" + QString::number(ID) + ") action(" + escapeStr(action) + ");";

	addRequest( request );
}

void giFTConnection::makeShareRequest( QString action )
{
	QString request;

	request = "SHARE action(" + escapeStr(action) + ");";

	addRequest( request );
}

void giFTConnection::makeBrowseRequest( int ID, QString user )
{
	QString request;

	request = "BROWSE ("+ QString::number(ID) + ") query(" + escapeStr(user) + ");";

	addRequest( request );

}

void giFTConnection::makeAddRequest( QString user, QString hash, QString size, QString url, QString file )
{
	QString request;

	request = "ADDSOURCE user(" + escapeStr(user) + ") hash(" + escapeStr(hash) + ") size(" + escapeStr(size) + ") url(" + escapeStr(url) + ") save(" + escapeStr(file) + ");";

	addRequest( request );

}

void giFTConnection::makeQuitRequest()
{
	QString request;

	request = "QUIT;";

	addRequest( request );
}

void giFTConnection::makeDelSourceRequest(int ID, QString url)
{
	QString request;

	request = "DELSOURCE (" + QString::number(ID) + ") url(" + escapeStr(url) + ");";
	/*cout<<request<<endl;*/
	addRequest( request );
}

void giFTConnection::addRequest( QString& req )
{
	pending.enqueue(&req);

	if (m_debugOutput && req.find("STATS") == -1)
	  debug("<= to the daemon: " + req);

	sockWrite();
}

void giFTConnection::sockWrite()
{
	QString pendingrequest;

	while( !pending.isEmpty() )
	{
		// For each pending request
		pendingrequest = *(pending.dequeue());
		// write it to the socket
		socket.writeBlock(pendingrequest, pendingrequest.length());
	}
}

void  giFTConnection::disconnect()
{
	socket.close();
	emit disconnected();
}

int giFTConnection::getNewID()
{
	return ID++;
}

QString giFTConnection::escapeStr( QString& sendstr )
{
	for(uint i = 0; i < sendstr.length(); i++)
	{
		if(sendstr[(int)i] == '[' || sendstr[(int)i] == ']' || sendstr[(int)i] == '(' || sendstr[(int)i] == ')' ||
		   sendstr[(int)i] == '{' || sendstr[(int)i] == '}' || sendstr[(int)i] == ';' || sendstr[(int)i] == '\\')
		{
			sendstr = sendstr.insert(i, '\\');
			i++;
		}
	}

	return sendstr;
}

QString giFTConnection::searchKey(const QString &key, const QString &_searchstr)
{
	QString searchstr = _searchstr.stripWhiteSpace();

	if(searchstr.find(key + "(") == -1)
	  return "";

	QString retval = searchstr.mid(searchstr.find(key + "(") + key.length() + 1);

	for(uint i = 1; i < retval.length(); i++)
	{
		if(retval[(int)i] == ')' && retval[(int)i - 1] != '\\')
		{
			retval = retval.left(i);
			break;
		}

		if((retval[(int)i] == '[' || retval[(int)i] == ']' || retval[(int)i] == '(' || retval[(int)i] == ')' ||
		    retval[(int)i] == '{' || retval[(int)i] == '}' || retval[(int)i] == ';' || retval[(int)i] == '\\') &&
		   retval[(int)i - 1] == '\\')
		{
			retval = retval.remove(i - 1, 1);

			if(retval[(int)i - 1] != '\\')
				i--;
		}
	}

	return retval;
}
