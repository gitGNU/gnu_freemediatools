/*******************************************************************************
**
** The header file for the Jahshaka network server module header
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qsocket.h>
#include <qserversocket.h>
#include <qapplication.h>
#include <qvbox.h>
#include <qtextview.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtextstream.h>
#include <qimage.h>
#include <qfile.h>
#include <qhostaddress.h>
#include <qbuffer.h>

#include <stdlib.h>

#include "opencore.h"
#include "assetdata.h"

class ClientSocket : public QSocket
{
    Q_OBJECT
public:
  ClientSocket( int sock, QObject *parent=0, int theport=4242, const char *name=0 ) :
	      QSocket( parent, name )
  {
  
    line = 0;
    servport = theport;

    connect( this, SIGNAL(readyRead()), SLOT(readClient() ) );
    connect( this, SIGNAL(connectionClosed()), SLOT(connectionClosed()) );
    connect( this, SIGNAL(sendtext(QString)), parent, SLOT( socketText(QString)) );

    buf.open(IO_WriteOnly);

    connect( this, SIGNAL(updateDesktop(assetData)), parent, SLOT( socketUpdateDesktop(assetData)) );

    setSocket( sock );
    thesocket = sock;

  }

  ~ClientSocket() {  }



  int thesocket,servport;
    QBuffer buf;

  void readClientData();
  void readClientMedia();

private slots:
  void readClient();
  void connectionClosed();

signals:
  void sendtext(QString);
  void updateDesktop( assetData );

private:
  int line;
  QString JahMediaPath;

};

// The SimpleServer class handles new connections to the server. For every
// client that connects, it creates a new ClientSocket -- that instance is now
// responsible for the communication with that client.

class SimpleServer : public QServerSocket
{
    Q_OBJECT
public:
  SimpleServer( int port=4242, QObject* parent=0 ) : QServerSocket( port, 1, parent )
  {
    //set up tracer
    //JahTrace& jtrace = JahTrace::getInstance();
	jtrace = OL_Trace::getInstance();

    QString theport; 
    theport.setNum(port);
    
    if ( !ok() ) 
    { 
        jtrace->error("Failed to bind to port ",theport);   
    }
    else       
    { 
        jtrace->debug("Jahserver bound to port ",theport);  
    }
    portnumber = port;

    connect( this, SIGNAL(sendupdateDesktop(assetData)), parent, SLOT( updatetheDesktop(assetData)) );

  }

	~SimpleServer()  { }

	void newConnection( int socket );

	int portnumber;
	QHostAddress  * address;

	int lastPortNum( void);

	//for tracer
	OL_Trace *jtrace;

public slots:
  void socketText(QString);
  void socketUpdateDesktop(assetData);

signals:
  void newConnect();
  void copyText(QString);
  void sendupdateDesktop(assetData);
  void recipConnect(QHostAddress);

};




