#ifndef JAHTEXTBROWSER_H
#define JAHTEXTBROWSER_H

#include <qurl.h>
#include <qhttp.h>
#include <qtextbrowser.h>
#include <string>
#include <qnetworkprotocol.h>
#include <qprogressbar.h>

#define JTB_ONLINE 1
#define JTB_OFFLINE 2
#define JTB_DISCONNECTED 3

class JahTextBrowser : public QTextBrowser {
	Q_OBJECT
public:
    JahTextBrowser(QProgressBar *, const QString &, const QString &, 
        const QString &, const QString &, 
        QWidget *parent =0, const char *name =0);

public slots:
    void switchLine();
    void home();
    //void reload();
    void stop();
    void setText(const QString &txt);

signals:
    void modeChanged(int);

private slots:
    void fetch(const QString &);
    void done(bool);
    void imgdone(bool);

private:
    QProgressBar *pb;
    QUrl onlineu;
    QString onlinehost, onlinecontext, host, context;
    QString offlinepath, offlinep, disconnectedp;
	QHttp http, imghttp;
    int mode;
    bool stopped;

    QStringList urls;
    QString buf;
    QStringList::iterator urli;

    void fetch_online(const QString &);
    void fetch_offline(const QString &);
    void goOnline();
    void goOffline();
};

#endif // JAHTEXTBROWSER_H

