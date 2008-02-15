#include "jhclient.h"
#include <qregexp.h>
#include <qfileinfo.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assetexchange.h>
#include <qbuffer.h>
#ifdef _WIN32
    #include <windows.h>
#else
#include <unistd.h>
#endif
#include <string>
#include <qstring.h>
#include <desktop.h>
#include <jahdatabase.h>
#include <imageitem.h>
#include <filmstrip.h>
#include <qapplication.h>
#include <qurl.h>

#ifdef JAHLINUX
#include <signal.h>
#endif

#include "assettable.h"

VideodataTable JHClient::results;

JHClient::JHClient(JahDesktop *desktop, QObject *parent, const char *name)
    : QSocket(parent, name), jt(*JahTrace::getInstance()),
        prefs(JahPrefs::getInstance()),
		m_desktop( desktop ),
		m_transferring( false )
{
#ifdef JAHLINUX
	signal(SIGPIPE, SIG_IGN);
#endif

    connect(this, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(this, SIGNAL(connectionClosed()), this, SLOT(closeConnection()));
	connect(this, SIGNAL(bytesWritten( int )), this, SLOT( bytesTransferred( int ) ));
}

void JHClient::closeConnection()
{
    close();
}

void JHClient::readClient()
{
    while (state( ) == QSocket::Connected && !m_transferring && canReadLine())
    {
        QString lineread = readLine();
        QString cmdline(lineread);
        jt.debug("Cmd line: " + cmdline.simplifyWhiteSpace( ));
        while (canReadLine() && lineread != "\r\n")
            lineread = readLine();
        
        QStringList tokens = QStringList::split(QRegExp("[ \r\n]+"), cmdline);
        if (tokens[0] == "GET")
            dealWithGet(tokens);
    }
}

#define REQ_UNKNOWN     0
#define REQ_KEYFRAME    1
#define REQ_MEDIA       2
#define REQ_INDEX       3
#define REQ_THUMB		4
#define REQ_GALLERY		5
#define REQ_DETAIL		6
#define REQ_PLAY		7

static int validate(const QString &s);

void JHClient::dealWithGet(QStringList &tokens)
{
    switch (validate(tokens[1]))
    {
        case REQ_INDEX:
            replyIndex();
            break;

        case REQ_KEYFRAME:
            replyKeyframe(tokens[1]);
            break;

        case REQ_MEDIA:
            replyMedia(tokens[1]);
            break;

        case REQ_THUMB:
            replyThumb(tokens[1]);
            break;

        case REQ_GALLERY:
            replyGallery(tokens[1]);
            break;

        case REQ_DETAIL:
            replyDetail(tokens[1]);
            break;

        case REQ_PLAY:
            replyPlay(tokens[1]);
            break;

        default:
            replyStatus("404", "Not found");
    }
}

static int validate(const QString &s)
{
    if (QRegExp("/thumb/\\d+/.*").exactMatch(s))
        return REQ_THUMB;
    if (QRegExp("/keyframe/\\d+/.*").exactMatch(s))
        return REQ_KEYFRAME;
    if (QRegExp("/(index.htm){0,1}l{0,1}").exactMatch(s) || QRegExp("/gallery/").exactMatch(s) )
        return REQ_GALLERY;
    if (QRegExp("/media/\\d+/.*").exactMatch(s))
        return REQ_MEDIA;
    if (QRegExp("/table/").exactMatch(s))
        return REQ_INDEX;
    if (QRegExp("/detail/\\d+").exactMatch(s))
        return REQ_DETAIL;
    if (QRegExp("/play/\\d+/.*").exactMatch(s))
        return REQ_PLAY;
    return REQ_UNKNOWN;
}

void JHClient::replyIndex()
{
   	getResultsFromDb();

    std::ostringstream ss;
    VideodataTable *iter;
    assetExchange ae;

    // XHTML HEADER
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
        << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n"
        << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\r\n";

    // HEAD
    ss << "<head>\r\n"
        << "<title>Jahshaka Web Desktop View</title>\r\n"
        << "</head>\r\n";

    // BODY
    ss << "<body>\r\n";
    ss << "<h1>Jahshaka: Table</h1>\r\n";

	ss << "<table>\r\n"
	   << "<tr><td valign=top>\r\n"
 	   << "<br><a href='/gallery/'>Gallery</a><br>Table<br>\r\n"
	   << "</td>\r\n"
	   << "<td>";

    // TABLE
    ss << "<table>\r\n";

    ss << "<thead align=left>\r\n<tr>\r\n"
        << "<th>Thumb</th>\r\n"
        << "<th>Name</th>\r\n"
        << "<th>Number</th>\r\n"
        << "<th>Location</th>\r\n"
        << "<th>Extension</th>\r\n"
        << "<th>Start</th>\r\n"
        << "<th>End</th>\r\n"
        << "</tr>\r\n</thead>\r\n";

    ss << "<tbody>\r\n";
    for (iter = JHClient::results.next; iter; iter = iter->next)
    {
		QString name = iter->medianame;
		QUrl::encode( name );
		name += QString( "," ) + QString::number( iter->startframe + iter->offsetframe );

        ss << "<tr>\r\n";
        ss << "<td>" << "<a href=\"/detail/" << iter->medianumber << "\"><img src=\"/thumb/" << iter->medianumber << "/" << std::string( name.data( ) ) << "\" /></a>" << "</td>\r\n";
        ss << "<td>" << iter->medianame << "</td>\r\n";
        ss << "<td>" << iter->medianumber << "</td>\r\n";
        ss << "<td>" << iter->filelocation << "</td>\r\n";
        ss << "<td>" << iter->fileextension << "</td>\r\n";
        ss << "<td>" << iter->startframe << "</td>\r\n";
        ss << "<td>" << iter->endframe << "</td>\r\n";
        ss << "</tr>\r\n";
    }
    ss << "</tbody>\r\n";
    ss << "</table>\r\n";
    ss << "</td>\r\n";
    ss << "</tr>\r\n";
    ss << "</table>\r\n";

    ss << "</body>\r\n";

    ss << "</html>";

	std::string page = ss.str();
    sendPageToClient(page);
}

void JHClient::replyDetail(QString &req)
{
    VideodataTable *i;
    QRegExp rx("\\d+");
    int num = 0;
	int ante = 0;

    if (rx.search(req) > -1)
        num = rx.cap().toInt();

    i = findEntry(num, ante);

    if (!i)
    {
        replyStatus("404", "Not found");
        return;
    }

	std::string ss;

	// Silly - quick hack
	char temp[ 20 ]; sprintf( temp, "%d/", i->medianumber );
	char prev[ 20 ]; sprintf( prev, "%d", ante );
	char next[ 20 ]; sprintf( next, "%d", i->next ? i->next->medianumber : 0 );

	QString name = i->medianame;
	QUrl::encode( name );
	name += QString( "," ) + QString::number( i->startframe + i->offsetframe );

    ss = "HTTP/1.0 200 Ok\r\n"
         "Content-Type: text/html; charset=\"utf-8\"\r\n"
         "\r\n";
    writeBlock(ss.c_str(), ss.size());

    // XHTML HEADER
    ss = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
         "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n"
         "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\r\n";
    writeBlock(ss.c_str(), ss.size());

    // HEAD
    ss = "<head>\r\n"
         "<title>Jahshaka Web Desktop View</title>\r\n"
         "</head>\r\n";
    writeBlock(ss.c_str(), ss.size());

	// Ugh
	std::string left = ante != 0 ? "<a href='/detail/" + std::string( prev ) + "'>Previous</a>" : "Previous";
	std::string right = i->next ? "<a href='/detail/" + std::string( next ) + "'>Next</a>" : "Next";

	QString url = QString(  i->medianame ) + QString( i->fileextension );
	QUrl::encode( url );

    // BODY
    ss = "<body>\r\n"
         "<h1>Jahshaka: Detail</h1>\r\n"
    	 "<table>\r\n"
         "<tbody>\r\n"
		 "<tr>"
		 "<td valign=top>"
		 "<br><a href='/gallery/'>Gallery</a><br><a href='/table/'>Table</a><br><br>\r\n"
		 + left + "<br>" + right + "<br><br>\r\n"
		 "<a href=\"/media/" + std::string( temp ) + std::string( url.ascii( ) ) + "\">Download</a><br>\r\n"
		 "<a href=\"/play/" + std::string( temp ) + std::string( url.ascii( ) ) + "\">Play</a><br>\r\n"
		 "</td>"
		 "<td align=center><img src=\"/keyframe/" + std::string( temp ) + std::string( name.data( ) ) + "\" /></td>\r\n"
		 "</tr>\r\n"
		 "<tr><td></td><td>" + i->medianame + "</td></tr>\r\n"
         "</tbody>\r\n"
         "</table>\r\n"
         "</body>\r\n"
         "</html>";
  	writeBlock(ss.c_str(), ss.size());

	close( );
}

void JHClient::replyGallery( QString & )
{
   	getResultsFromDb();

	std::string ss;
    VideodataTable *iter;

	int index = 0;

    ss = "HTTP/1.0 200 Ok\r\n"
         "Content-Type: text/html; charset=\"utf-8\"\r\n"
         "\r\n";
    writeBlock(ss.c_str(), ss.size());

    // XHTML HEADER
    ss = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
         "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\r\n"
         "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\r\n";
    writeBlock(ss.c_str(), ss.size());

    // HEAD
    ss = "<head>\r\n"
         "<title>Jahshaka Web Desktop View</title>\r\n"
         "</head>\r\n";
    writeBlock(ss.c_str(), ss.size());

    // BODY
    ss = "<body>\r\n"
         "<h1>Jahshaka: Gallery</h1>\r\n";
    writeBlock(ss.c_str(), ss.size());

	ss = "<table>\r\n"
		 "<tr><td valign=top>\r\n"
		 "<br>Gallery<br><a href='/table/'>Table</a>\r\n"
		 "</td>\r\n"
		 "<td>\r\n"
    	 "<table>\r\n"
         "<tbody>\r\n";
    writeBlock(ss.c_str(), ss.size());

    for (iter = JHClient::results.next; iter; iter = iter->next)
    {
		char temp[ 20 ];
		sprintf( temp, "%d", iter->medianumber );

		QString name = iter->medianame;
		QUrl::encode( name );
		name += QString( "," ) + QString::number( iter->startframe + iter->offsetframe );

		ss = "";

		if ( index % 10 == 0 )
        	ss = "<tr>\r\n";
        
		ss += "<td>"
		     "<a href=\"/detail/" + std::string( temp ) + "\"><img src=\"/thumb/" + std::string( temp ) + "/" + std::string( name.data( ) ) + "\" /></a><br>"
             "</td>\r\n";

		if ( ( ++ index ) % 10 == 0 || iter->next == NULL )
        	ss += "</tr>\r\n";

    	writeBlock(ss.c_str(), ss.size());
    }
    ss = "</tbody>\r\n"
         "</table>\r\n"
         "</td></tr></table>\r\n"
         "</body>\r\n"
         "</html>";

  	writeBlock(ss.c_str(), ss.size());

	close( );
}

void JHClient::replyKeyframe(QString &req)
{
    VideodataTable *i;
    QRegExp rx("\\d+");
    int num = 0;

    if (rx.search(req) > -1)
        num = rx.cap().toInt();

    i = findEntry(num);

    if (!i)
    {
        replyStatus("404", "Not found");
        return;
    }

	if ( i->keyframe.size() == 0 )
	{
		assetExchange ae;
		QImage original = ae.getImage(i->data, i->startframe + i->data.frameOffset);

		QBuffer keyframe(i->keyframe);
		keyframe.open(IO_WriteOnly);
		original.save(&keyframe, "JPEG", 50);
		keyframe.close();
	}

    uint sz = i->keyframe.size();
    sendHeaderForJpg(sz);
	writeBlock( i->keyframe.data( ), sz );
}

void JHClient::replyThumb(QString &req)
{
    VideodataTable *i;
    QRegExp rx("\\d+");
    int num = 0;

    if (rx.search(req) > -1)
        num = rx.cap().toInt();

    i = findEntry(num);

    if (!i)
    {
        replyStatus("404", "Not found");
        return;
    }

	if ( i->thumb.size() == 0 )
	{
		assetExchange ae;
		QImage original = ae.getImage(i->data, i->startframe + i->data.frameOffset);

   		float ratio = (double)( original.width( ) ) / (double)( original.height( ) );
		int w = 80;
		int h = int( 80 / ratio );

		if ( h > 80 )
		{
			h = 80;
			w = int( 80 * ratio );
		}

		QBuffer thumb(i->thumb);
		thumb.open(IO_WriteOnly);
		original.scale( w, h ).save(&thumb, "JPEG", 50);
		thumb.close();
	}

    uint sz = i->thumb.size();
    sendHeaderForJpg(sz);
	writeBlock( i->thumb.data( ), sz );
}

void JHClient::replyMedia(QString &req)
{
    VideodataTable *i;
    QRegExp rx("\\d+");
    int num = 0;
    uint sz;

    if (rx.search(req) > -1)
        num = rx.cap().toInt();
    i = findEntry(num);

    if (!i)
    {
        replyStatus("404", "Not found");
        return;
    }
    m_transfer.setName(assetExchange::resolveFullPath(i->data, 1).c_str());
    sz = QFileInfo(m_transfer).size();
    sendHeaderForBinary(sz);
    sendFile( );
}

void JHClient::replyPlay(QString &req)
{
    VideodataTable *i;
    QRegExp rx("\\d+");
    int num = 0;

    if (rx.search(req) > -1)
        num = rx.cap().toInt();
    i = findEntry(num);

	if ( i )
	{
		AssetTables::getInstance( )->setSelectedAsset( i->data );
		m_desktop->changeModePlayer( );
	}

	std::string response = "HTTP/1.0 204 No Content\r\n\r\n";
	writeBlock(response.c_str(), response.size());
}

VideodataTable * JHClient::findEntry(int n)
{
    VideodataTable *i;
    for (i = results.next; i; i = i->next)
        if (i->medianumber == n) 
			return i;
    return NULL;
}

VideodataTable * JHClient::findEntry(int n, int &prev)
{
    VideodataTable *i;
	prev = 0;
    for (i = results.next; i; prev = i->medianumber, i = i->next)
        if (i->medianumber == n) 
			return i;
    return NULL;
}

void JHClient::replyStatus(std::string code, std::string msg)
{
    std::ostringstream ss, msgs;

    msgs << "<html><body>" << code << " " << msg << "</body</html>";

    ss << "HTTP/1.0 " << code << " " << msg << "\r\n"
        << "Content-Type: text/html\r\n"
        << "Content-Length: " << msgs.str().size() << "\r\n"
        << "\r\n"
        << msgs.str();

    writeBlock(ss.str().data(), ss.str().size());
}

void JHClient::bytesTransferred( int )
{
	if ( m_transferring && state( ) == QSocket::Connected )
	{
		while ( m_transferring && bytesToWrite( ) < 40960 )
		{
			char buffer[ 8192 ];
			unsigned int count = m_transfer.readBlock( buffer, 8192 );

			if ( count != 0 )
			{
				blockSignals( true );
				count = writeBlock( buffer, count );
				blockSignals( false );
			}

			if ( count != 8192 || state( ) != QSocket::Connected )
			{
				m_transferring = false;
				m_transfer.close( );
			}
		}
	}
	else if ( m_transferring && state( ) != QSocket::Connected )
	{
		m_transferring = false;
		m_transfer.close( );
	}
}

void JHClient::sendFile( )
{
    m_transfer.open(IO_ReadOnly);
	m_transferring = true;
	bytesTransferred( 0 );
}

void JHClient::sendHeaderForJpg(int sz)
{
    std::ostringstream ss;
    ss << "HTTP/1.0 200 Ok\r\n"
        << "Content-Type: image/jpg\r\n"
        << "Content-Length: " << sz << "\r\n"
        << "Cache-Control: max-age=3600\r\n"
        << "\r\n";
    writeBlock(ss.str().data(), ss.str().size());
}

void JHClient::sendFooter()
{
    std::ostringstream ss;
    ss << "\r\n";
    writeBlock(ss.str().data(), ss.str().size());
}

void JHClient::sendHeaderForBinary(uint sz)
{
    std::ostringstream ss;
    ss << "HTTP/1.0 200 Ok\r\n"
        << "Content-Type: application/binary\r\n"
        << "Content-Length: " << sz << "\r\n"
        << "\r\n";
    writeBlock(ss.str().data(), ss.str().size());
}

static bool matches( assetData a1, assetData a2 )
{
	return a1.location == a2.location && a1.filename == a2.filename && a1.frameOffset == a2.frameOffset;
}

void JHClient::getResultsFromDb()
{
	VideodataTable *iter = &JHClient::results;
	VideodataTable *piter = iter;

    QCanvasItemList l= m_desktop->allItems();

	QValueList<assetData> list;
    for (QCanvasItemList::Iterator it= l.begin(); it!= l.end(); it ++ )
	{
        if ( (*it)->rtti() == imageRTTI )
		{
			ImageItem* pImageItem = ((ImageItem*)(*it));
			assetData asset = pImageItem->getClip();
			if(pImageItem->filmStrip() != 0)
				asset.frameOffset = pImageItem->filmStrip()->getCentralFrameNumber();
			else
				asset.frameOffset = pImageItem->currentFrameNumber();
			list.append( asset );
		}
	}

	qHeapSort( list );

    for (QValueList< assetData >::Iterator it= list.begin(); it!= list.end(); it ++ )
	{
		assetData asset = *it;

		piter = iter;

		if ( iter->next == NULL || !matches( iter->next->data, asset ) )
		{
			if ( iter->next == NULL )
			{
				iter->next = new VideodataTable;
				iter = iter->next;
				iter->next = NULL;
			}
			else
			{
				iter = iter->next;
			}

			iter->data = asset;
			
			iter->category = iter->data.theCategory;
			iter->cliptype = iter->data.theType;
			strcpy(iter->medianame, iter->data.clipname.latin1( ) );
			iter->medianumber = iter->data.clipnumber;

			strcpy(iter->filelocation, iter->data.location.latin1( ) );
			strcpy(iter->filename, iter->data.filename.latin1( ) );
			strcpy(iter->fileextension, iter->data.extension.latin1( ) );
			iter->startframe = iter->data.startframe;
			iter->endframe = iter->data.endframe;
			iter->offsetframe =  iter->data.frameOffset;

			iter->thumb.resize( 0 );
			iter->keyframe.resize( 0 );
		}
		else
		{
			iter = iter->next;
		}
	}

	if ( iter->next != NULL )
	{
		piter = iter;
		iter = iter->next;
		piter->next = NULL;

		while( iter != NULL )
		{
			piter = iter;
			iter = iter->next;
			delete piter;
		}
	}
}

void JHClient::sendPageToClient(std::string &page)
{
    std::ostringstream ss;
    ss << "HTTP/1.0 200 Ok\r\n"
        << "Content-Type: text/html; charset=\"utf-8\"\r\n"
        << "Content-Length: " << page.size() << "\r\n"
        << "\r\n";
    writeBlock(ss.str().data(), ss.str().size());
	writeBlock(page.c_str( ), page.size( ) );
}

void JHClient::freeResults()
{
    VideodataTable *b, *i = JHClient::results.next;
    while (i)
    {
        b = i;
        i = i->next;
        delete b;
    }
	JHClient::results.next = NULL;
}



AssetTables::AssetTables( )
{
}

AssetTables *AssetTables::getInstance( )
{
  static AssetTables *tables = NULL;
  if ( tables == NULL )
    tables = new AssetTables( );
  return tables;
}

void AssetTables::registerTable( AssetTable *table )
{
  m_tables.push_back( table );
}

assetData &AssetTables::getSelectedAsset( )
{
  return m_current_asset;
}

void AssetTables::setSelectedAsset( assetData &asset )
{
  m_current_asset = asset;
  for ( std::vector< AssetTable * >::iterator iter = m_tables.begin( ); iter != m_tables.end( ); iter ++ )
    ( *iter )->selectAsset( asset );
}


AssetTable::~AssetTable( ) 
{

}
