/***************************************************************************
                          giftconnection.h  -  description
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

#ifndef GIFTCONNECTIONLIN_H
#define GIFTCONNECTIONLIN_H

#include <qsocket.h>
#include <qstring.h>
#include <qptrqueue.h>
#include <sys/types.h>

#include "giftsocket.h"

using namespace Apollon;

class giFTConnection : public QObject
{
	Q_OBJECT

	public:
		giFTConnection();
		~giFTConnection();

		const QString &host()             { return m_host; }
		int            port()             { return m_port; }
		bool           remoteConnection() { return m_remoteConnection; }

		bool connectToServer();
		bool isConnected();

		void setParams(const QString &host, int port, bool remote = false);
		void disconnect();

		int getNewID();
		static QString searchKey(const QString &key, const QString &searchstr);
		void startgiFT();

		void setDebugOutput(bool) { m_debugOutput = true; };

	public slots:
		void sockReadReady();
		void sockClosed();
		void sockError(int);

		void makeAttachRequest();
		void makeStatsRequest();
		void makeSearchRequest(int ID, int index, QString text);
		void makeCancelSearchRequest(int);
		void makeCancelBrowseRequest(int);
		void makeLocateRequest(int ID, QString hash);
		void makeAddRequest(QString user, QString hash, QString size, QString url, QString file);
		void makeTransferRequest(int ID, QString action);
		void makeDelSourceRequest(int ID, QString url);
		void makeShareRequest(QString action);
		void makeBrowseRequest(int ID, QString user);
		void makeQuitRequest();

		void addRequest(QString &req);
		void sockWrite();

		QString escapeStr(QString &sendstr);
		void restartDaemon();

	signals:
		void connected();
		void disconnected();
		void socketError(int error);
		void giftStatusReply(QString);
		void giftItemReply(QString);
		void giftAddDownloadReply(QString);
		void giftAddUploadReply(QString);
		void giftAddSourceReply(QString);
		void giftDelSourceReply(QString);
		void giftChgUploadReply(QString);
		void giftChgDownloadReply(QString);
		void giftDelDownloadReply(QString);
		void giftDelUploadReply(QString);
		void giftShareReply(QString);

#ifndef WIN32
		void sigGiFTPID(pid_t);
#endif

	private:
		giFTSocket socket;
		bool paramsSet;
		bool m_debugOutput;
		int ID;

		QString  m_host;
		int      m_port;
		bool     m_remoteConnection;

#ifndef WIN32
		pid_t giFT_pid;
#endif

		QPtrQueue<QString> pending;
};

#endif
