/*******************************************************************************
**
** The source file for the Jahshaka network server module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#include "httpd.h"


///////////////////////////////////////////////////////////
//this routes us to the correct routine based on the port
//data is coming in on... 4242 is conversation data
//and 4243 is media data

void ClientSocket::readClient()
{

   if (servport==4242)
   {
    readClientData();
   }
   else if (servport==4243)
   {
    readClientMedia();
   }

}

void ClientSocket::readClientData()        //sends this back to the client...
{

    //////////////////////////////////////////////////////
    //this is for text only conversations

    debug("LayerCategory::TEXT SERVER receiving data");

    QString echotext,temp,temp2;

    QTextStream os( this );

    while ( canReadLine() )
    {
        echotext = temp.setNum(line) + " : " + readLine();
        emit sendtext(echotext);
        line++;
    };

}

void ClientSocket::readClientMedia()
{
    buf.writeBlock(readAll());
}

void ClientSocket::connectionClosed()
{
    debug("we are closing the connection");

    QString text; debug("closing Connection on port :" + text.setNum(servport));
    if (servport==4242)
    {
        return;
    }

    buf.close();
    buf.open(IO_ReadOnly);

    uint filesize;
    QString clipname;
    int filestoreceive;
    int filesreceived;
    assetData * datafile;


    datafile = new assetData;
    QString read,conv;
    QTextStream is( &buf );

    read=is.readLine(); read=read.stripWhiteSpace();
    datafile->clipname   = read;
    read=is.readLine(); read=read.stripWhiteSpace();
    datafile->clipnumber = read.toInt();
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->filename   = read;
    datafile->location   = "media/share/"; //this would be jahpath+share...
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->extension  = read;
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->videoext   = read;
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->startframe = read.toInt();
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->endframe   = read.toInt();
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->theCategory = VideoCategory::TYPE(read.toInt());
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->theType = VideoType::TYPE(read.toInt());
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->xsize = read.toDouble();
    read=is.readLine(); read=read.stripWhiteSpace(); 
    datafile->ysize = read.toDouble();
    datafile->xpos       = 10;
    datafile->ypos       = 10;
    read=is.readLine(); read=read.stripWhiteSpace();
    filestoreceive = read.toInt();
    filesreceived = 0;

    for (filesreceived = 0; filesreceived < filestoreceive; ++filesreceived)
        {
        read = is.readLine(); read = read.stripWhiteSpace();
        clipname = "media/share/" + read;
        read = is.readLine(); read = read.stripWhiteSpace();
        filesize = read.toUInt();

        QFile file( clipname );
        if ( QFile::exists( clipname ) ) { file.remove(); }
        file.open( IO_WriteOnly );
        char *data = (char *)malloc(filesize);
        buf.readBlock(data, filesize);
        file.writeBlock(data, filesize);
        file.close();
        free(data);
    }

    buf.close();

    emit updateDesktop(*datafile);
}

/////////////////////////////////////////////////////////////
//This is the main server connection code
//it creates a new ClientSocket for each connect and that
//instance is now responsible for the communication with that client.

void SimpleServer::newConnection( int socket )
{
  ClientSocket * thesock = new ClientSocket( socket, this, portnumber );

  QString text; text = "Incoming Connection on socket :" + text.setNum(socket);
  emit copyText(text); debug(text);

  emit recipConnect(thesock->peerAddress());

}

void SimpleServer::socketText( QString text)
{
    emit copyText(text);
}

void SimpleServer::socketUpdateDesktop( assetData datafile)
{

  debug("passing on assetData from socket close");

  //datafile.printinfo();

  emit sendupdateDesktop(datafile);
}


int SimpleServer::lastPortNum( void)
{
    return portnumber;
}

