#ifndef JHCLIENT_H
#define JHCLIENT_H

#include <qsocket.h>
#include <assetdata.h>

class JahDesktop;
class JahTrace;
class JahPrefs;

struct VideodataTable {
    assetData data;
    int category;
    int cliptype;
    char medianame[129];
    int medianumber;
    char filelocation[256];
    char filename[256];
    char fileextension[12];
    int startframe;
    int endframe;
    int offsetframe;
    QByteArray thumb;
    QByteArray keyframe;
    VideodataTable *next;
};

class JHClient : public QSocket {
    Q_OBJECT
public:
    JHClient(JahDesktop *, QObject * =0, const char * =0);
    static void freeResults();

private slots:
    void readClient();
    void closeConnection();
	void bytesTransferred( int );

private:
    JahTrace &jt;
    JahPrefs &prefs;

    void dealWithGet(QStringList &tokens);
    void replyIndex();
    void replyKeyframe(QString &);
    void replyThumb(QString &);
    void replyMedia(QString &req);
    void replyPlay(QString &req);
    void replyGallery(QString &req);
    void replyDetail(QString &req);
    VideodataTable * findEntry(int n);
    VideodataTable * findEntry(int n, int &prev);
    void replyStatus(std::string code, std::string msg);
    void sendHeaderForJpg(int sz);
    void sendHeaderForBinary(uint sz);
    void sendFile();
    void sendFooter();
    void getResultsFromDb();
    void sendPageToClient(std::string &page);

    static VideodataTable results; // head of list
	JahDesktop *m_desktop;

	// For the downloading state - the QSocket writeBuffer accepts any 
	// amount of data written to it - this causes a huge build up in the sockets
	// buffer when transferring large files, so the socket event 'bytesWritten'
	// is used to trigger more data to be written to the buffer. Note that 
	// the keep alive functionality in readClient is disabled when a media transfer
	// occurs.
	QFile m_transfer;
	bool m_transferring;
};

#endif // JHCLIENT_H
