/*******************************************************************************
**
** The header file for the Jahshaka network module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "networking.h"
#include "qregexp.h"
#include "apollonutils.h"
#include <qlineedit.h>
#include "giftconnection.h"
#include <qtabwidget.h>
#include <qcombobox.h>
#include "apollonqueryview.h"
#include "apollontransfertab.h"

//used for the p2p engine
#ifdef JAHGIFT

void JahNet::initializeGift()
{


    qDebug("initializing gift");

    jahGift = new giFTConnection();

    jahGift->setParams("localhost", 1213, true);

    //set up some variables
    kqv = static_cast<ApollonQueryView*>(0);

    m_hashList = new QValueList<QString>();

    connect(jahGift, SIGNAL(giftItemReply(QString)), this,SIGNAL(giftItemReply(QString)));

    setupTransferTab();


    connect(jahGift, SIGNAL(connected()),              this, SLOT(connected()) );
    //connect(jahGift, SIGNAL(disconnected()),           this, SLOT(disconnected()));
    connect(jahGift, SIGNAL(socketError(int)),         this, SLOT(giftsocketError(int)));
    connect(jahGift, SIGNAL(giftStatusReply(QString)), this, SLOT(giftStatusReply(QString)));

    m_timer = new QTimer(this);

}

void JahNet::startDaemon()
{
    qDebug("starting gift");

    emit switchToNetP2P();

    //this will lauch the demon if its not already running
    // jahGift->startgiFT();
    //sleep(1); // this is necessary, giFT needs this time

    jahGift->connectToServer();

}

// tries to connect to a remote gift daemon
void JahNet::retryRemoteConnection()
{
    jahGift->connectToServer();
}

// we've just been connected with the giFT daemon
void JahNet::connected()
{
    qDebug("connected to gift...");

    // update stats and enable stats timer.
    requestStats();

    m_numStatUpdates = 0;
    m_tries = 0;
    m_timer->start(5 * 1000); // 5 seconds

    m_timer->disconnect(this, SLOT(retryRemoteConnection()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(requestStats()));
}

// requests a stats update from giFT
void JahNet::requestStats()
{

    jahGift->makeStatsRequest();

    if(m_numStatUpdates < 20)
    {
        if(++m_numStatUpdates == 20) // after one minute the interval is changed to ...
        {
            m_timer->changeInterval(60 * 1000); // ... 1 minute
        }
    }

}

// displays new stats in the info box
void JahNet::giftStatusReply(QString status)
{

    QString statustext;
    QString networkstatus;
    QString size;
    int pos = 0;
    int tbl_cnt = 0;
    unsigned long totalUsers = 0;
    unsigned long totalFiles = 0;
    unsigned long totalSize = 0;

    //KIconLoader *loader = KGlobal::iconLoader();

    QRegExp rx("(\\w+) \\{ (?:users\\(([\\d]+)\\) )?files\\(([\\d]+)\\) size(.*)\\(([\\.\\d]+)\\) \\}");
    rx.setMinimal(true);

    while((pos = rx.search(status, pos)) != -1)
    {
        if(rx.cap(4).isEmpty())
            size = "GB";
        else
            size = rx.cap(4).remove("]").remove("[");

        if(rx.cap(1) == "giFT")
        {

        }
        else
        {
            if(tbl_cnt == 0)
//				statustext += "<table width=\"100%\" border=\"0\">";

            if(rx.cap(2) != "0")
                networkstatus = "Online";
            else
                networkstatus = "Connecting...";

            if (rx.cap(1) == "OpenFT")
            {
                //QMimeSourceFactory::defaultFactory()->setPixmap( "OpenFT",  loader->loadIcon("openft", KIcon::MainToolbar));

                if(tbl_cnt % 2 == 0)
                    statustext += "<tr>";
                tbl_cnt++;
                statustext += "OpenFT" + rx.cap(1);
            }
            else if (rx.cap(1) == "FastTrack")
            {
                //QMimeSourceFactory::defaultFactory()->setPixmap( "FastTrack",  loader->loadIcon("kazaa", KIcon::MainToolbar));
                //if(tbl_cnt % 2 == 0)
                //	statustext += "<tr>";
                tbl_cnt++;
                statustext += "FastTrack" + rx.cap(1);
            }
            else if (rx.cap(1) == "Gnutella")
            {
                //QMimeSourceFactory::defaultFactory()->setPixmap( "Gnutella",  loader->loadIcon("gnutella", KIcon::MainToolbar));
                if(tbl_cnt % 2 == 0)
                    statustext += "<tr>";
                tbl_cnt++;
                statustext += "Gnutella" + rx.cap(1);
            }
            else if (rx.cap(1) == "OpenNap")
            {
                //QMimeSourceFactory::defaultFactory()->setPixmap( "OpenNap",  loader->loadIcon("napster", KIcon::MainToolbar));
                if(tbl_cnt % 2 == 0)
                    statustext += "<tr>";
                tbl_cnt++;
                statustext += "OpenNap" + rx.cap(1);
            }
            else if (rx.cap(1) == "SoulSeek")
            {
                //QMimeSourceFactory::defaultFactory()->setPixmap( "SoulSeek",  loader->loadIcon("soulseek", KIcon::MainToolbar));
                if(tbl_cnt % 2 == 0)
                    statustext += "<tr>";
                tbl_cnt++;
                statustext += "SoulSeek" + rx.cap(1);
            }
            else
            {
                //if(tbl_cnt % 2 == 0)
                //	statustext += "<tr>";
                tbl_cnt++;
                //statustext += "<td><h3><b>" + rx.cap(1) + ":</b></h3>";
            }


            gidtstatusbox->setText(networkstatus); //correctamundo

            statustext = ApollonUtils::numberHuman(rx.cap(2).toULong());
            usersonlinebox->setText(statustext);

            totalUsers += rx.cap(2).toULong();
            totalFiles += (unsigned long) rx.cap(3).toDouble();
            totalSize += (unsigned long) rx.cap(5).toDouble();

            statustext = ApollonUtils::numberHuman(totalSize) + " GB";
            localsharesbox->setText(statustext);

        }

        statustext = ApollonUtils::numberHuman((unsigned long) rx.cap(3).toDouble())+" files";
        totalsharedbox->setText(statustext); //correctamundo


        statustext += "Total Size Shared:" + ApollonUtils::numberHuman((unsigned long) rx.cap(5).toDouble()) + " " + size;

        pos += rx.matchedLength();
    }

}

void JahNet::giftsocketError(int error)
{

    if(error == 0)
    {
        if(jahGift->remoteConnection())
        {
            qDebug("connection error");
        /*
            m_infoBox->setText(i18n("<h3>Connection error</h3>") +
                            i18n("<p>I can't connect to the giFT daemon, which is responsible for connecting to the"
                                    "  various networks, on the remote host.</p>"
                                    "<p>This means the daemon either isn't running or is rejecting our connections"
                                    "  because we're not in the allowed hosts list. If the latter is the case, open the"
                                    "  ~/.giFT/giftd.conf file <i>on the remote host</i> and edit the hosts_allow"
                                    "  variable so that we are an allowed host.</p>") +
                            i18n("<p>In the meantime, I'll try to reconnect once a minute to see whether the daemon"
                                    "  becomes available.</p>"));
*/
            m_timer->start(60 * 1000); // 1 minute

            m_timer->disconnect(this, SLOT(requestStats()));
            connect(m_timer, SIGNAL(timeout()), this, SLOT(retryRemoteConnection()));
        }
        else
        {
            // connection refused, probably the daemon is down, so let's start it
            if(m_tries < 3)
            {
                m_tries++;
                startDaemon();
            }

            // okay, now I'm giving up
            else
            {
                QString statustext;
                qDebug("connection error giving up");
/*
                statustext = i18n("<h3>Connection error</h3>") +
                            i18n("<p>The giFT daemon, which is responsible for connecting to the various networks,"
                                "  appears to be down and I don't seem to able to start it.</p>"
                                "<p>Probably your giFT installation is corrupted. Try starting giFT manually by"
                                "  typing \"giftd\" on the command line. If this gives errors, there's likely"
                                "  some internal error in giFT or in your giFT configuration. Refer to the giFT"
                                "  homepage at <a href=\"http://giftproject.org\">http://giftproject.org</a>"
                                "  for help.</p>");
*/
                m_tries = 0;

                //m_infoBox->setText(statustext);
            }
        }
    }
    else
    {
        QString statustext;
        qDebug("connection error");
        /*
        statustext = i18n("<h3>Connection error</h3>") +
                    i18n("<p>An unhandled error was returned when trying to open a connection with the giFT"
                        "  daemon, which is responsible for connecting to the various networks. The reported"
                        "  error was:</p>"
                        "<blockquote>"
                        "  %1 (%2)"
                        "</blockquote>").arg(strerror(error)).arg(error);
*/
    //	m_infoBox->setText(statustext);
    }

}


void JahNet::searchGiftNetwork()
{

    // find out which realm is selected
    //and what text is searched for
    QString text = searchforlineedit->text();

    int realm = mediatypebox->currentItem();

    if (realm>3) realm = 0;

    startQuery( realm, text );

}

void JahNet::startQuery(const int &index, const QString &text)
{

    int ID = jahGift->getNewID();

    jahGift->makeSearchRequest(ID, index, text);

    if(index == 6)
    {
        //browseUser(text);
        //return;
    }
    else
    {
        kqv = new ApollonQueryView(viewtab, ID, index, text, jahGift);
        kqv->setHashList(m_hashList);
        connect(this, SIGNAL(sigDeleteHash(QString )), kqv, SLOT(slotDeleteHash(QString)));
    }

    QString tabtext = text;

    unsigned int MAX_TABTEXT_CHARS = 32;

    tabtext = (tabtext.length() < MAX_TABTEXT_CHARS ) ? tabtext : tabtext.left(MAX_TABTEXT_CHARS - 3).append("...");

    QPixmap icon;
    QString number;

    viewtab->addTab(kqv, icon, tabtext+" "+number.setNum(0)+" results");

    viewtab->showPage(kqv);
    viewtab->polish();


    connect(this, SIGNAL(giftItemReply(QString)),                                      kqv,   SLOT(addItem(QString)));
    connect(kqv,  SIGNAL(giftAddRequest(QString, QString, QString, QString, QString)), jahGift, SLOT(makeAddRequest(QString, QString, QString, QString, QString)));
    connect(kqv,  SIGNAL(giftCancelSearchRequest(int) ),                               jahGift, SLOT(makeCancelSearchRequest(int)));
    connect(kqv,  SIGNAL(closetabs()),                                                 this,  SLOT(closealltabs()));


}



void JahNet::slotGetHash(QString hash)
{
    *m_hashList << hash;
    emit sigSendHash(hash);
}


void JahNet::slotDeleteHash(QString hash)
{
    if( m_hashList->remove(hash) )
        emit sigDeleteHash(hash);
}


void JahNet::getGiftNetworkStats()
{
    jahGift->makeStatsRequest();

}

void JahNet::setupTransferTab()
{
    m_transTab      = new ApollonTransferTab(  incomingtraffic,  "ApollonTransferTab", jahGift);

    //connect(m_transTab,       SIGNAL(browseUser(QString)),           this,            SLOT(showSearches(QString)));
    //connect(m_transTab,       SIGNAL(statsUpdate(int, float)),       this,            SLOT(statsUpdate(int, float)));

    connect(m_transTab,       SIGNAL(sigSendHash(QString )), this,     SLOT(slotGetHash(QString)));
    connect(m_transTab,       SIGNAL(sigDeleteHash(QString )), this,     SLOT(slotDeleteHash(QString)));

    m_transTab->polish();

}

#else


void JahNet::initializeGift()
{
}

void JahNet::startDaemon()
{
}

// tries to connect to a remote gift daemon
void JahNet::retryRemoteConnection()
{
}

// we've just been connected with the giFT daemon
void JahNet::connected()
{
}

// requests a stats update from giFT
void JahNet::requestStats()
{
}

// displays new stats in the info box
void JahNet::giftStatusReply(QString status)
{
}

void JahNet::giftsocketError(int )
{
}

void JahNet::searchGiftNetwork()
{
}

void JahNet::startQuery(const int &, const QString &)
{
}

void JahNet::slotGetHash(QString hash)
{
}

void JahNet::slotDeleteHash(QString hash)
{
}

void JahNet::getGiftNetworkStats()
{
}

void JahNet::setupTransferTab()
{
}

#endif


