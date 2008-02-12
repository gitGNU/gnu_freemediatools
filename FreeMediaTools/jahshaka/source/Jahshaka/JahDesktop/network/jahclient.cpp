/*******************************************************************************
**
** The source file for the Jahshaka network client module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#include "networking.h"
#include <qsocket.h>

#include <jahcreate.h>
#include <desktop.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextview.h>


void JahNet::createNewClient() 
{
	QString hostname = serveredit->text() ;
	//qDebug("host :" + hostname);

	socket->connectToHost( hostname, 4242 );

}


void JahNet::createNewClient(QHostAddress) {

	//we create the chat client here on port 4242
	if (socket) { qDebug("already a socket..."); return; }
	if ( (socket->isOpen() ) && ( socket->state() != QSocket::Connecting) ) { qDebug("already connected"); return; }


	//we create the chat client here on port 4242
	socket = new QSocket( this );

	connect( socket, SIGNAL(connected()), this, SLOT(socketConnected()) );
	connect( socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()) );
	connect( socket, SIGNAL(readyRead()), SLOT(socketReadyRead()) );
	connect( socket, SIGNAL(error(int)), SLOT(socketError(int)) );

	// connect to the server
	//infoText->append( tr("Trying to connect to the server\n") );

	//get hostname from fields here
	QString hostname = serveredit->text() ;
	qDebug("host :" + hostname);

	socket->connectToHost( hostname, 4242 );

}



void JahNet::closeConnection()
{
    if (socket->state() == QSocket::Idle)
        {
        clienttextfeedback->append("Not connected.\n"  );
        return;
        }
    clienttextfeedback->append("Connection closed.\n"  );
    socket->close();
}

void JahNet::sendToServer() {

    if (socket->state() != QSocket::Connected)
        {
        clienttextfeedback->append("Not connected.\n"  );
        return;
        }

    clienttextfeedback->append(  texteditbox->text() +"\n"  );

    QTextStream os(socket);
    os << texteditbox->text() << "\n";

    texteditbox->setText( "" );

}

void JahNet::sendMediaToServer( assetData clip, int clipnumber ) {

  //////////////////////////////////////////////////////////////
  //first we open up a new socket separate from the converstaion

    currentclip = clip;
    currentclipnumber = clipnumber;

  //get hostip and port from fields here
  QString hostname = serveredit->text() ;
  QString porttext = "4243" ;  //the image server is on a different port...
  int portnumber= porttext.toInt();

  qDebug("host :" + hostname + " port:" + porttext);

  //need to do some debugging here in case we cant connect...
  datasocket->connectToHost( hostname, portnumber );

}

void JahNet::datasocketConnected()
{
    clienttextfeedback->append("Sending clip...\n"  );

  QTextStream os(datasocket);
  QString write, parse;

  assetData &clip = currentclip;
  write=clip.clipname;                           os << write << "\n";
  write=clip.clipnumber;                         os << write << "\n";
  write=clip.filename;                           os << write << "\n";
  write=clip.extension;                          os << write << "\n";
  write=clip.videoext;                           os << write << "\n";
  write=parse.setNum(clip.startframe);           os << write << "\n";
  write=parse.setNum(clip.endframe);             os << write << "\n";
  write=parse.setNum(clip.theCategory);          os << write << "\n";
  write=parse.setNum(clip.theType);              os << write << "\n";
  write=parse.setNum(clip.xsize);          os << write << "\n";
  write=parse.setNum(clip.ysize);              os << write << "\n";

  QString debugdata,readfile;

    int filestosend;

    if (clip.theType == VideoType::SEQUENCE)
        {
        filestosend = clip.endframe - clip.startframe + 1;
        if (filestosend <= 0) filestosend = 1;
        }
    else
        {
        filestosend = 1;
        }

    write=parse.setNum(filestosend);
    os << write << "\n";

    for (int i = 0; i < filestosend; ++i)
        {
        // work on videoext: use the filetosend arg

        QString ap;
        if (clip.theType == VideoType::SEQUENCE)
            {
            QString tmp;
            tmp.setNum(clip.startframe+i); // e.g. 1
            ap = tmp.rightJustify(clip.videoext.length(), '0');
            }
        else
            {
            ap = "";
            }
        
        readfile =  clip.location+clip.filename+ap+clip.extension;
        qDebug("Readfile is:"+readfile);

        write = clip.filename+ap+clip.extension;
        os << write << "\n";

        QFile file( readfile);

        file.open( IO_ReadOnly );

        QByteArray ba = file.readAll();
        write = parse.setNum(ba.size());
        os << write << "\n";
        datasocket->writeBlock(ba.data(), ba.size());

        file.close();
        }

  datasocket->close();

  clienttextfeedback->append( "Clip sent.\n" );

}

void JahNet::sendClipToServer()
{
    bool clipstatus = false;
    assetData newclip = jahcontrol->JahDesktopMod->getVideoData(clipstatus);

    if (clipstatus)
        {
        sendMediaToServer(newclip,jahcontrol->globalclipnumber);
        }
    else
        {
        clienttextfeedback->append( "No clip selected.\n" );
        }
}


void JahNet::socketReadyRead() {

	// read from the server
	while ( socket->canReadLine() ) {
	    //infoText->append( socket->readLine() );
	}

}

void JahNet::socketConnected()    { clienttextfeedback->append( tr("Connected to peer\n") ); }

void JahNet::socketConnectionClosed()    {
    clienttextfeedback->append( tr("Connection closed by peer\n") );
    socket->close();
}

void JahNet::socketError(int e)
{
    switch (e)
        {
    case QSocket::ErrConnectionRefused:
        clienttextfeedback->append( tr("Connection refused\n"));
        break;
    case QSocket::ErrHostNotFound:
        clienttextfeedback->append( tr("Host not found\n"));
        break;
    case QSocket::ErrSocketRead:
        clienttextfeedback->append( tr("Network error\n"));
        }
}

void JahNet::datasocketError(int e)
{
    switch (e)
        {
    case QSocket::ErrConnectionRefused:
        clienttextfeedback->append( tr("Connection refused\n"));
        break;
    case QSocket::ErrHostNotFound:
        clienttextfeedback->append( tr("Host not found\n"));
        break;
    case QSocket::ErrSocketRead:
        clienttextfeedback->append( tr("Network error\n"));
        }
}

