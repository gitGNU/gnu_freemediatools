/***************************************************************************
                          libapollon.h  -  Apollon library
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

#ifndef __LIBAPOLLON_H
#define __LIBAPOLLON_H

#include <sys/types.h>

#include <qmap.h>
#include <qobject.h>
#include <qprocess.h>
#include <qstring.h>
#include <qtimer.h>
#include <qvaluelist.h>
#include <qvaluevector.h>

#include "giftparse.h"
#include "giftsocket.h"

namespace Apollon
{

extern const int GIFT_PORT;
extern const int START_ID;
extern const int MAX_SEARCH_TIME;

class Connection;

/**
 * Contains all information about search results as returned by @ref searchResult() signal
 */
struct SearchResult
{
	/**
	 * Constructor
	 */
	SearchResult();

	/**
	 * Simple assignment operator, returns a duplicate of the SearchResult
	 */
	SearchResult operator=(const SearchResult &result);

	/**
	 * ID of the search result. If you receive search results with identical ID's, the
	 * library found them to be identical, thus being multiple sources to the same file
	 */
	uint id;

	/**
	 * Hash of the file
	 */
	QString hash;

	/**
	 * Filename, including path
	 */
	QString filename;

	/**
	 * The user sharing the file
	 */
	QString user;

	/**
	 * Node sharing the file
	 */
	QString node;

	/**
	 * The number of available slots
	 */
	uint availability;

	/**
	 * Mimetype of the file
	 */
	QString mimetype;

	/**
	 * (Protocol-specific) URL of the file
	 */
	QString url;

	/**
	 * Filesize, in bytes
	 */
	uint size;

	/**
	 * Various meta-data
	 */
	typedef QMap<QString, QString> MetaDataDict;
	MetaDataDict metadata;
};

/**
 * This class is used internally for managing searches, you normally don't need to use
 * it directly.
 */
class Search : public QObject
{
	Q_OBJECT

	public:
		/**
		 * Constructor, sets the ID and initializes the timer.
		 */
		Search(uint id, Connection *connection = 0);

		/**
		 * Destructor, cleans up the sources from all source lists
		 */
		~Search();

		/**
		 * The ID of the search
		 */
		uint id;

		/**
		 * A 2 dimensional list with all results. The rows represent the unique results, where
		 * the columns represent the sources per unique result.
		 */
		typedef QValueList<SearchResult*> SourcesList;
		typedef QValueVector<SourcesList> ResultsVector;
		ResultsVector results;

		/**
		 * This timer is used to remove searches that exceeded MAX_SEARCH_TIME.
		 */
		QTimer timer;

	signals:
		/**
		 * This signal is emitted when the search exceeded MAX_SEARCH_TIME. If a connection
		 * was specified in the constructor, this signal is automatically connected to its
		 * stopSearch() slot.
		 */
		void destroyMe(uint id);

	private slots:
		void destroySearch();
};

/**
 * Contains statistics about downloads and uploads
 */
struct Stats
{
	/**
	 * Hash of the file
	 */
	QString hash;

	/**
	 * State of the transfer
	 */
	QString state;

	/**
	 * Number of transmitted bytes
	 */
	uint transmit;

	/**
	 * Filesize, in bytes
	 */
	uint size;

	/**
	 * Filename, excluding path
	 */
	QString filename;

	/**
	 * Marks whether the file is explicitly shared
	 */
	bool shared;

	/**
	 * Contains per source statistics
	 */
	struct SourceStats
	{
		/**
		 * The username
		 */
		QString user;

		/**
		 * (Protocol-specific) URL of the file on the user's computer
		 */
		QString url;

		/**
		 * Status of the current trunk
		 */
		QString status;

		/**
		 * Status of the current trunk (string provided by protocol)
		 */
		QString protocolStatus;

		/**
		 * The bytenumber on which the current trunk starts
		 */
		uint start;

		/**
		 * The number of bytes transmitted of the current trunk
		 */
		uint transmit;

		/**
		 * Total size of the current trunk
		 */
		uint total;
	};

	/**
	 * Stats about the separate sources
	 */
	typedef QValueList<SourceStats> SourcesList;
	SourcesList sources;
};

/**
 * contains statistics about the various protocols (including "giFT")
 */
struct ProtocolStats
{
	/**
	 * Name of the network or protocol.
	 */
	QString name;

	/**
	 * Number of users online.
	 */
	uint users;

	/**
	 * Number of files being shared.
	 */
	uint files;

	/**
	 * Total size of shared files, in GB.
	 */
	uint size;
};
typedef QValueList<ProtocolStats> ProtoStatsList;

/**
 * Handles all communication with the giFT daemon. No internal knowledge of the
 * daemon is required for the use of this class.
 */
class Connection : public QObject
{
	Q_OBJECT

	public:
		/**
		 * Constructor. The construct will not automatically try to connect
		 * to the daemon.
		 */
		Connection();

		/**
		 * Destructor
		 */
		~Connection();

		/**
		 * Starts the daemon and connects to it when started. If connecting succeeds,
		 * the signal @ref connected() will be emitted. On error, @ref errorConnecting()
		 * is emitted.
		 */
		void startDaemon();

		/**
		 * Attempts to connect to the daemon, without starting it.
		 * @param host the host where the daemon runs, by default this is localhost.
		 * @param port the port on which the daemon runs, by default this is 1213.
		 */
		void connectToDaemon(const QString &host = "127.0.0.1", uint port = GIFT_PORT);

		/**
		 * Kills the daemon. Does nothing if it wasn't running.
		 */
		void stopDaemon();

		/**
		 * Use this function to enable/disable debug output.
		 * @param debugOutput true if debug output should be enabled
		 */
		void setDebugOutput(bool debugOutput);

	public slots:
		/**
		 * Starts a search on the network.
		 * @param keywords the keywords to search for
		 * @param realm the realm to search, this can be "everything" (default),
		 *              "audio", "video", "images", "text documents", "software",
		 *              or "user"
		 * @param excluding if provided, these are keywords that may not occur in
		 *                  the results
		 * @return an ID for the search
		 */
		int startSearch(const QString &keywords, const QString &realm = "everything",
		                const QString &excluding = QString::null);

		/**
		 * Stops a search.
		 * @param id ID of the search to stop
		 */
		void stopSearch(uint id);

		/**
		 * Starts downloading a file that was previously returned as a search result.
		 * @param searchResult the search result to download
		 * @return an ID for the download
		 */
		int startDownload(const SearchResult &searchResult);

		/**
		 * Pauses a download.
		 * @param id ID of the download to pause
		 */
		void pauseDownload(uint id);

		/**
		 * Resumes a paused download.
		 * @param id ID of the download to resume
		 */
		void resumeDownload(uint id);

		/**
		 * Attempts to find more sources for a download.
		 * @param id ID of the download to search for more sources for
		 */
		void findSourcesForDownload(uint id);

		/**
		 * Cancels a download.
		 * @param id ID of the download to cancel, the ID will be invalid after this
		 *           function.
		 */
		void stopDownload(uint id);

		/**
		 * Cancels an upload.
		 * @param id ID of the upload to cancel, the ID will be invalid after this
		 *           function. The ID is provided by the @ref uploadStarted() signal.
		 * @see uploadStarted()
		 */
		void stopUpload(uint id);

	signals:
		/**
		 * Emitted as soon as a (new) connection comes available. Any jobs you sent
		 * to the daemon (like @ref startSearch(), @ref startDownload()) will be
		 * ignored.
		 */
		void connected();

		/**
		 * Emitted if an established connection is lost.
		 * @param error error code indicating why the connection was lost
		 */
		void disconnected(int error);

		/**
		 * Emitted if we tried to establish a connection, but failed.
		 * @param error error code indicating why the connection attempt failed.
		 */
		void errorConnecting(int error);

		/**
		 * Gives statistics per protocol (including "giFT").
		 */
		void statsUpdate(const ProtoStatsList &stats);

		/**
		 * Emitted when new results for a search action arrived.
		 * @param id ID of the search
		 * @param result contains all details about the search result
		 */
		void searchResult(uint id, const SearchResult &result);

		/**
		 * Emitted when a search completed. No more @ref searchResult() signals will
		 * be emitted anymore for the given search ID after this signal.
		 * @param id ID of the search that completed
		 * @see searchResult()
		 */
		void searchCompleted(uint id);

		/**
		 * Provides new statistics for a download.
		 * @param id ID of the download
		 * @param stats Holds all statistics about the download. Including sources.
		 */
		void downloadStatsUpdate(uint id, Stats &stats);

		/**
		 * Emitted when a download is stalled. This can be because the user paused
		 * the download, or because there are no sources available anymore.
		 * @param id ID of the stalled download
		 */
		void downloadStalled(uint id);

		/**
		 * Emitted when a stalled download is resumed. This can be because the user
		 * unpaused the download, or because there are new sources available.
		 * @param id ID of the resumed download
		 */
		void downloadResumed(uint id);

		/**
		 * Emitted when a download completed.
		 * @param id ID of the completed download
		 */
		void downloadCompleted(uint id);

		/**
		 * Emitted when a new upload is started. This signal provides the new ID
		 * of the upload.
		 * @param id new ID of the upload
		 */
		void uploadStarted(uint id);

		/**
		 * Provides new statistics for an upload.
		 * @param id ID of the upload
		 * @param stats Holds all statistics about the upload.
		 */
		void uploadStatsUpdate(uint id, Stats &stats);

		/**
		 * Emitted when an upload is stalled. Usually, this means the downloader was
		 * disconnected for whatever reason.
		 * @param id ID of the stalled upload
		 */
		void uploadStalled(uint id);

		/**
		 * Emitted when a stalled upload is resumed.
		 * @param id ID of the resumed upload
		 */
		void uploadResumed(uint id);

		/**
		 * Emitted when an upload is canceled.
		 * @param id ID of the canceled upload
		 */
		void uploadCanceled(uint id);

		/**
		 * Emitted when an upload completed.
		 * @param id ID of the completed upload
		 */
		void uploadCompleted(uint id);

	private:
		enum IdClasses { idNone = 0, idSearch, idBrowse, idDownload, idUpload };
		typedef QMap<uint, IdClasses> IdDict;
		typedef QMap<uint, Search *> SearchDict;

		bool             m_debugOutput;      // should debug output be generated?
		bool             m_connected;        // are we connected to the daemon?
		bool             m_connectCompleted; // flag whether a connection attempt completed
		QProcess         m_daemon;           // process of the daemon
		giFTSocket       m_socket;           // socket to communicate with the daemon
		int              m_restartTries;     // number of times we tried to restart the daemon
		uint             m_id;               // id for daemon communication
		IdDict           m_idDict;           // list that tracks the usage of id's
		SearchDict       m_searchDict;       // list that keeps track of all running searches
		ProtoStatsList   m_protoStatsList;   // contains statistics per protocol

		void sentRequest(const QString &request);
		void sentAttachRequest();
		void sentDetachRequest();
		void sentStatsRequest();
		void sentSearchRequest(uint ID, const QString &text, const QString &realm,
		                       const QString &excluding);
		void sentCancelSearchRequest(uint ID);
		void sentCancelBrowseRequest(uint ID);
		void sentLocateRequest(uint ID, const QString &hash);
		void sentTransferRequest(uint ID, const QString &action);
		void sentShareRequest(const QString &action);
		void sentBrowseRequest(uint ID, const QString &user);
		void sentAddSourceRequest(const QString &user, const QString &hash, const QString &size,
		                          const QString &url, const QString &file);

		void readItemCommand(const Command &command);
		void readStatsCommand(const Command &command);

		QString escapeStr(QString string);

	private slots:
		void socketConnected();
		void socketReadyRead();
		void socketClosed(int error = 0);
		void socketError(int error);
};

}
#endif
