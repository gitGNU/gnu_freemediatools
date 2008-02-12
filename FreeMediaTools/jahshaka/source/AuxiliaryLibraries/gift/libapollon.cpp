/***************************************************************************
                          libapollon.cc  -  Apollon library
                             -------------------
    begin                : Tue Sept 23 2003
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

#ifndef WIN32
#include <unistd.h>
#endif

//#include <kdebug.h>

#include "libapollon.h"

#define PACKAGE "libjahgift"
#define VERSION "1.9.9"

namespace Apollon
{

const int GIFT_PORT = 1213;
const int FIRST_ID = 900;
const int MAX_SEARCH_TIME = 300; // seconds

SearchResult::SearchResult() :
  id(0),
  availability(1),
  size(0)
{
}

SearchResult SearchResult::operator=(const SearchResult &result)
{
	SearchResult dup;

	dup.id = result.id;
	dup.hash = result.hash;
	dup.filename = result.filename;
	dup.user = result.user;
	dup.node = result.node;
	dup.availability = result.availability;
	dup.mimetype = result.mimetype;
	dup.url = result.url;
	dup.size = result.size;
	dup.metadata = result.metadata;

	return dup;
}

Search::Search(uint /*id*/, Connection *connection) :
  id(id)
{
	if(connection)
		connect(this, SIGNAL(destroyMe(uint)), connection, SLOT(stopSearch(uint)));

	connect(&timer, SIGNAL(timeout()), this, SLOT(destroySearch()));
	timer.start(MAX_SEARCH_TIME * 1000, true);
}

void Search::destroySearch()
{
	emit destroyMe(id);
}

Search::~Search()
{
	for(ResultsVector::Iterator it = results.begin(); it != results.end(); ++it)
	{
		for(SourcesList::Iterator jt = (*it).begin(); jt != (*it).end(); ++jt)
		{
			delete (*jt);
		}
	}
}

Connection::Connection() :
  m_debugOutput(false),
  m_connected(false),
  m_restartTries(0),
  m_id(FIRST_ID)
{
	connect(&m_socket, SIGNAL(connected()),        this, SLOT(socketConnected()));
	connect(&m_socket, SIGNAL(readyRead()),        this, SLOT(socketReadyRead()));
	connect(&m_socket, SIGNAL(connectionClosed()), this, SLOT(socketClosed()));
	connect(&m_socket, SIGNAL(error(int)),         this, SLOT(socketError(int)));
}

Connection::~Connection()
{
	if(m_connected)
	{
		sentDetachRequest();
	}
}

void Connection::startDaemon()
{
	m_daemon.setArguments("giftd");
	m_daemon.start();

#ifndef WIN32
	sleep(1); // give the daemon some time to start up
#endif

	connectToDaemon("127.0.0.1", GIFT_PORT);
}

void Connection::connectToDaemon(const QString &host, uint port)
{
	if(!m_connected)
	{
		m_socket.connectToHost(host, (Q_UINT16) port);
		m_connectCompleted = false;

		connectToDaemon(host, port);
	}
}

void Connection::stopDaemon()
{
	if(m_connected)
	{
		sentDetachRequest();
		m_daemon.tryTerminate();
	}
}

void Connection::setDebugOutput(bool debugOutput)
{
	m_debugOutput = debugOutput;
}

int Connection::startSearch(const QString &keywords, const QString &realm,
                            const QString &excluding)
{
	debug("Connection::startSearch()");

	uint id = m_id++;

	m_searchDict[id] = new Search(id, this);

	if(realm != "user")
	{
		m_idDict[id] = idSearch;
		sentSearchRequest(id, keywords, realm, excluding);
	}
	else
	{
		m_idDict[id] = idBrowse;
		sentBrowseRequest(id, keywords);
	}

	return id;
}

void Connection::stopSearch(uint id)
{
	if(m_idDict[id] != idSearch && m_idDict[id] != idBrowse)
		return;

	sentCancelSearchRequest(id);

	delete m_searchDict[id];

	m_idDict.erase(id);
	m_searchDict.erase(id);
}

int Connection::startDownload(const SearchResult &/*searchResult*/)
{
	return 0;
}

void Connection::pauseDownload(uint /*id*/)
{
}

void Connection::resumeDownload(uint /*id*/)
{
}

void Connection::findSourcesForDownload(uint /*id*/)
{
}

void Connection::stopDownload(uint /*id*/)
{
}

void Connection::stopUpload(uint /*id*/)
{

}

// ------------------------------------------------------------------- //

void Connection::sentRequest(const QString &request)
{
	if(m_debugOutput)
		debug("<= to the daemon: " +request);

	m_socket.writeBlock(request, request.length());
}

void Connection::sentAttachRequest()
{
	sentRequest("ATTACH client(" + QString(PACKAGE) + ") version(" + QString(VERSION) + ") profile(mkoebele);");
}

void Connection::sentDetachRequest()
{
	sentRequest("DETACH;");
	m_socket.close();
	m_connected = false;
}

void Connection::sentStatsRequest()
{
	sentRequest("STATS;");
}

void Connection::sentSearchRequest(uint ID, const QString &text, const QString &realm,
                                   const QString &excluding)
{
	debug( "Connection::sentSearchRequest()" );

	QString request = "SEARCH (" + QString::number(ID) + ") query (" + escapeStr(text) + ")";

	if(excluding != QString::null)
		request += " exclude (" + excluding + ")";
	if(realm != "everything")
		request += " realm (" + realm + ")";

	request += ";";
	sentRequest(request);
}

void Connection::sentCancelSearchRequest(uint ID)
{
	sentRequest("SEARCH (" + QString::number(ID) + ") action (cancel);");
}

void Connection::sentBrowseRequest(uint ID, const QString &user)
{
	sentRequest("BROWSE (" + QString::number(ID) + ") query(" + escapeStr(user) + ");");

}
void Connection::sentCancelBrowseRequest(uint ID)
{
	sentRequest("BROWSE (" + QString::number(ID) + ") action (cancel);");
}

void Connection::sentLocateRequest(uint ID, const QString &hash)
{
	if(!hash.contains("null"))
	{
		sentRequest("LOCATE (" + QString::number(ID) + ") query (" + hash + ");");
	}
}

void Connection::sentTransferRequest(uint ID, const QString &action)
{
	sentRequest("TRANSFER (" + QString::number(ID) + ") action(" + action + ");");
}

void Connection::sentShareRequest(const QString &action)
{
	sentRequest("SHARE action(" + action + ");");
}

void Connection::sentAddSourceRequest(const QString &user, const QString &hash, const QString &size,
                                      const QString &url, const QString &file)
{
	sentRequest("ADDSOURCE user(" + escapeStr(user) + ") hash(" + hash + ") size(" + size + ") url(" + url + ") save(" + escapeStr(file) + ");");

}

QString Connection::escapeStr(QString string)
{
	for(uint i = 0; i < string.length(); i++)
	{
		if(string[(int)i] == '(' || string[(int)i] == ')' || string[(int)i] == '\\' ||
		   string[(int)i] == '{' || string[(int)i] == '}' || string[(int)i] == ';')
		{
			string = string.insert(i, '\\');
			i++;
		}
	}

	return string;
}

void Connection::socketConnected()
{
	m_connectCompleted = true;
	m_connected = true;

	sentAttachRequest();

	emit connected();
}

void Connection::socketReadyRead()
{
	debug("Connection::socketReadyRead()");

	QString block;

	while((block = m_socket.readCommandBlock()) != "")
	{
		Command command(block);

		if(m_debugOutput && command.name != "CHGDOWNLOAD" && command.name != "STATS")
			debug("=> from daemon: " + block);

		if(command.name == "STATS")
		{
			readStatsCommand(command);
		}
		else if(command.name == "ITEM")
		{
			readItemCommand(command);
		}

		/*if(line.find("ADDDOWNLOAD") > -1 && line.find("ADDDOWNLOAD") < 5)
		{
			emit giftAddDownloadReply( line );
		}
		if(line.find("ADDUPLOAD") > -1 && line.find("ADDUPLOAD") < 5)
		{
			emit giftAddUploadReply( line );
		}
		if(line.find("ADDSOURCE") > -1 && line.find("ADDSOURCE") < 5)
		{
			emit giftAddSourceReply( line );
		}
		if(line.find("DELSOURCE") > -1 && line.find("DELSOURCE") < 5)
		{
			emit giftDelSourceReply( line );
		}
		if(line.find("CHGDOWNLOAD") > -1 && line.find("CHGDOWNLOAD") < 5)
		{
			emit giftChgDownloadReply( line );
		}
		if(line.find("CHGUPLOAD") > -1 && line.find("CHGUPLOAD") < 5)
		{
			emit giftChgUploadReply( line );
		}
		if(line.find("DELDOWNLOAD") > -1 && line.find("DELDOWNLOAD") < 5)
		{
			emit giftDelDownloadReply( line );
		}
		if(line.find("DELUPLOAD") > -1 && line.find("DELUPLOAD") < 5)
		{
			emit giftDelUploadReply( line );
		}
		if(line.find("SHARE") > -1 && line.find("SHARE") < 5)
		{
			emit giftShareReply( line );
		}*/
	}
}

void Connection::readItemCommand(const Command &command)
{
	debug("Connection::readItemCommand()");

	Search *search = m_searchDict[command.value.toUInt()];
	SearchResult *result = new SearchResult;

	if(search == 0)
	{
		return;
	}

	for(CommandElementList::ConstIterator it = command.elements.begin(); it != command.elements.end(); ++it)
	{
		if((*it).name == "hash")
		{
			result->hash = (*it).value;
		}
		else if((*it).name == "file")
		{
			result->filename = (*it).value;
		}
		else if((*it).name == "user")
		{
			result->user = (*it).value;
		}
		else if((*it).name == "node")
		{
			result->node = (*it).value;
		}
		else if((*it).name == "availability")
		{
			result->availability = (*it).value.toUInt();
		}
		else if((*it).name == "mime")
		{
			result->mimetype = (*it).value;
		}
		else if((*it).name == "url")
		{
			result->url = (*it).value;
		}
		else if((*it).name == "size")
		{
			result->size = (*it).value.toUInt();
		}
		else if((*it).name == "META")
		{
			for(CommandElementList::ConstIterator jt = (*it).elements.begin(); jt != (*it).elements.end(); ++jt)
			{
				result->metadata[(*jt).name] = (*jt).value;
			}
		}
		else
		{
			if(m_debugOutput)
			{
				debug("Unknown key in ITEM command: " + (*it).name );
			}
		}
	}

	if(result->hash != QString::null)
	{
		for(Search::ResultsVector::Iterator it = search->results.begin(); it != search->results.end(); ++it)
		{
			if((*(*it).begin())->hash == result->hash)
			{
				result->id = (*(*it).begin())->id;
				(*it).append(result);
			}
		}

		emit searchResult(search->id, *result);

		return;
	}

	result->id = search->results.size();
	search->results.append(Search::SourcesList());
	(*search->results.end()).append(result);

	emit searchResult(search->id, *result);
}

void Connection::readStatsCommand(const Command &command)
{
	debug("Connection::readStatsCommand()");

	ProtocolStats stats;

	for(CommandElementList::ConstIterator it = command.elements.begin(); it != command.elements.end(); ++it)
	{
		stats.name = (*it).name;
		for(CommandElementList::ConstIterator jt = (*it).elements.begin(); jt != (*it).elements.end(); ++jt)
		{
			if((*jt).name == "users")
			{
				stats.users = (*jt).value.toUInt();
			}
			else if((*jt).name == "files")
			{
				stats.files = (*jt).value.toUInt();
			}
			else if((*jt).name == "size")
			{
				stats.size = (*jt).value.toUInt();
			}
			else
			{
				if(m_debugOutput)
				{
					debug("Unknown key in STATS command: " + (*jt).name );
				}
			}
		}
		m_protoStatsList.append(stats);
	}

	emit statsUpdate(m_protoStatsList);
}

void Connection::socketClosed(int error)
{
	m_connected = false;

	if(m_restartTries < 3)
	{
		m_restartTries++;
		startDaemon();
	}
	else
	{
		emit disconnected(error);
	}
}

void Connection::socketError(int error)
{
	m_connectCompleted = true;
	if(m_connected == true)
	{
		m_connected = false;

		emit errorConnecting(error);
	}
}

}
