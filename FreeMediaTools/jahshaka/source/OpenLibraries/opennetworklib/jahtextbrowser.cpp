#include "jahtextbrowser.h"

#include <iostream>

#include <qdragobject.h>
#include <qprogressdialog.h>
#include <qurloperator.h>
#include <qmime.h>
#include <qapplication.h>
#include <qeventloop.h>
#include <qnetwork.h>
#include <qregexp.h>
#include <qstrlist.h>

JahTextBrowser::
JahTextBrowser(
        QProgressBar *pb,
        const QString &onlineurl,
        const QString &offlinepath,
        const QString &offlinepage,
        const QString &disconnectedpage,
        QWidget *parent,
        const char *name)
      : QTextBrowser(parent, name),
        pb(pb),
        onlineu(onlineurl),
        offlinepath(offlinepath),
        offlinep(offlinepage),
        disconnectedp(disconnectedpage),
        mode(JTB_OFFLINE)
{
    onlinehost = onlineu.host();
    onlinecontext = onlineu.path();
    host = onlinehost;
    context = onlinecontext;


    mimeSourceFactory()->setFilePath(offlinepath);
	connect(this, SIGNAL(sourceChanged(const QString&)),
		this, SLOT(fetch(const QString &)));
	connect(&http, SIGNAL(done(bool)),
		this, SLOT(done(bool)));
    connect(&imghttp, SIGNAL(done(bool)),
        this, SLOT(imgdone(bool)));

    setSource(offlinep);
}

void JahTextBrowser::
setText(const QString &txt)
{
    QPalette p = palette();
    /* These Values should be enabled only if needed: they are
        a hack which corrects background and text colours
        at loading time */
    /*
    p.setColor(QColorGroup::Base, Qt::white);
    p.setColor(QColorGroup::Background, Qt::white);
    p.setColor(QColorGroup::Foreground, Qt::white);
    */
    p.setColor(QColorGroup::Text, Qt::black); 
    this->setPalette(p);

    QBrush b(white);
    setPaper(b);

    QTextBrowser::setText(txt);
}

void JahTextBrowser::
fetch(const QString &page)
{
    switch (mode)
        {
        case JTB_ONLINE: fetch_online(page);
            break;
        case JTB_OFFLINE: fetch_offline(page);
        }
}

void JahTextBrowser::
fetch_online(const QString &page)
{
    // New implementation, using USER_AGENT line in header
	QString path;

	if (page.startsWith("/"))
		{
		path = context = page;
		}
	else
		{
		path = context + "/" + page;
		}
    QHttpRequestHeader head("GET", path);
    head.setValue("Host", host);    
    head.setValue("User-Agent", "FreeMediaTools/2.0");
	http.setHost(host);
    http.request(head);
}

void JahTextBrowser::
fetch_offline(const QString &)
{
    ;
}

void JahTextBrowser::
done(bool err)
{
    if (err)
        {
        // go offline and display error page
        mode = JTB_OFFLINE;
        setSource(disconnectedp);
        emit modeChanged(mode);
        return;
        }

    QString tmp(http.readAll());
    buf = tmp;

    QRegExp rx("(src=\")([^\"]+)(\")");

    urls.clear();
    int offset = 0;
    while (-1 != (offset = rx.search(buf, offset, QRegExp::CaretAtOffset)))
        {
        urls.append(rx.cap(2));
        offset += rx.matchedLength();
        }

    // QUrl may help us in this case.
    // images are relative to the host, which is set in the global host var
    // all we do is fetch the relative/absolute url
    // but we also need to set that url somewhere: compose using QUrl perhaps?

    if (urls.empty())
        {
        setText(buf);
        return;
        }

    stopped = false;
    pb->reset();
    pb->setTotalSteps(urls.size());
    pb->setProgress(0);

    imghttp.setHost(host);
    // use the extra http object with separate signals to
    // process all the pictures.
    // Once they have been stored, they will become available
    // and therefore the last done() slot will setText()
    urli = urls.begin();
    tmp = *urli;
    imghttp.get(tmp);
}

void JahTextBrowser::
imgdone(bool err)
{
    QString tmp;

    pb->setProgress(pb->progress()+1);

    if (err)
        {
        // ignore as no entry will be insterted into mimefactory
        }

    // process image: get data and add to factory
    // readAll() gives me back a byte array
    // convert it to ImageDrag?
    // add *urli to factory
    QImage tmpi(imghttp.readAll());
    tmp = *urli;
    mimeSourceFactory()->setImage(tmp, tmpi);

    ++urli;
    if (urli == urls.end() || stopped)
        {
        setText(buf);
        return;
        }

    tmp = *urli;
    imghttp.get(tmp); // next image
}

void JahTextBrowser::
switchLine()
{
    switch (mode)
        {
        case JTB_ONLINE: goOffline();
        break;
        case JTB_OFFLINE: goOnline();
        break;
        }
}

void JahTextBrowser::
home()
{
    switch (mode)
        {
        case JTB_OFFLINE:
            setSource("offline.html");
            break;
        case JTB_ONLINE:
            setSource("");
            break;
        }
}

void JahTextBrowser::
stop()
{
    stopped = true;
}

void JahTextBrowser::
goOnline()
{
    mode = JTB_ONLINE;
    setSource("");
    emit modeChanged(mode);
}

void JahTextBrowser::
goOffline()
{
    mode = JTB_OFFLINE;
    setSource(offlinep);
    emit modeChanged(mode);
}

