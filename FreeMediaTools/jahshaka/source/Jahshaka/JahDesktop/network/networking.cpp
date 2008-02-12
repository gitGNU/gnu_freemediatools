/*******************************************************************************
**
** The header file for the Jahshaka network module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "networking.h"

#include <jahtextbrowser.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qtextview.h>
#include <qsocket.h>
#include <httpd.h>
#include <jahdatabase.h>
#include <jahhttpd.h>
#include "jahfileframe.h"
#include "jahprogressbar.h"

JahNet::JahNet( JahDesktop *desktop, QWidget* parent, QHBox*, const char* name, 
				QHBox* controls, QHBox* controller,
				int* globalclipnumber , JahControl *jc)
	: m_desktop( desktop )
	, m_parent( parent )
	, m_controls( controls )
	, m_controller( controller )
	, m_started( false )
{
    jahcontrol = jc;
    ModuleName = name;
    
	socket = NULL;
    datasocket = NULL;
    recipsocket = NULL;
    JahPrefs& jprefs  = JahPrefs::getInstance();
    autoLaunchServer = jprefs.getAutoLaunchServer();

    //initialize variables
    clipnumber = globalclipnumber;
	
	serverLaunched = FALSE;
	httpd = 0;
}

JahNet::~JahNet( )
{
	delete httpd;
}

void JahNet::activate( )
{
	if ( m_started ) return;

	m_started = true;

    buildInterface(m_controls);           //  builds the network interfaces
    buildController(m_controller);        //  draw slider interface

    //builds ui in parent - need a switch here
    buildNetframe(m_parent);              //  draw p2p interface

    initializeObjects();
}

void JahNet::initializeObjects()
{
	socket = new QSocket( this );
    datasocket = new QSocket( this );

	connect(socket, SIGNAL(connected()), SLOT(socketConnected()) );
	connect(socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()));
	connect(socket, SIGNAL(readyRead()), SLOT(socketReadyRead()) );
	connect(socket, SIGNAL(error(int)), SLOT(socketError(int)) );

    connect(datasocket, SIGNAL(connected()), SLOT(datasocketConnected()));
    connect(datasocket, SIGNAL(error(int)), SLOT(datasocketError(int)));


	if (autoLaunchServer)
		launchServer();

    httpd = NULL;

    //for p2p module
    jahGift = 0;

    NetStatus = true;

    //hook up Gift shared folder here
    theSharedFolder = new FileMainFrame ( mainshareframe,"shareview");

	//this is where we set the share folder
    theSharedFolder->fileView()->setDirectory( "/root/.giFT/completed/" );
    
	connect( theSharedFolder, SIGNAL(loadButton( )), this, SLOT(sendClipToDesktop( )) );

    //initialize gift engine
    #ifdef JAHGIFT
    initializeGift();
    #endif

    //////////////////////////////////////////////////////////////
    // JahGuide display for jahnet
    JahPrefs& jprefs = JahPrefs::getInstance();

    //can we just hard code these values instead of storing them?
    QString offlinepath("Pixmaps/network");
    QString offlinepage("offline.html");
    QString disconnectedpage("error.html");

    //and keep this one in the prefs
    QString onlineurl(jprefs.getJtbonlineurl().c_str());

    //////////////////////////////////////////////////////////////
    //create a ui in the main frame...
    NetboxLayersFrame = new QFrame( NetBox, "EffectsFrame" );
    NetboxLayersFrame->setGeometry( QRect( 0, 0, 800, 140 ) );
    NetboxLayersFrame->setMinimumSize( QSize( 800, 140 ) );  //was 960
    NetboxLayersFrame->setMaximumSize( QSize( 800, 140 ) );

    connectButton = new QPushButton("connect", NetboxLayersFrame);
        connectButton->setGeometry( QRect( 30, 40, 70, 25 ) );

    QLabel * layerlabel = new QLabel(NetboxLayersFrame);
    layerlabel->setGeometry( QRect( 200, 10, 120, 25 ) );
    layerlabel->setText( "Loading Elements" );
    QFont label_font(  layerlabel->font() );
    label_font.setPointSize( 10 );
    layerlabel->setFont( label_font );

    jtbprogbar = new JahProgressBar(10,NetboxLayersFrame);
        jtbprogbar->setGeometry( QRect( 110, 40, 400, 25 ) );

    textBrowser = new JahTextBrowser(jtbprogbar, onlineurl, offlinepath, offlinepage, disconnectedpage, mainnetframe);
    
    connect(connectButton, SIGNAL(clicked()), textBrowser, SLOT(switchLine()));
    connect(textBrowser, SIGNAL(modeChanged(int)),
        this, SLOT(renameConnectButton(int)));

    // setup connections with JahTextBrowser

    connect(netcontrollerhomebutton, SIGNAL(clicked()),
        textBrowser, SLOT(home()));
    connect(netcontrollerbackbutton, SIGNAL(clicked()),
        textBrowser, SLOT(backward()));

    connect(netcontrollerforebutton, SIGNAL(clicked()),
        textBrowser, SLOT(forward()));
    connect(netcontrollerreloadbutton, SIGNAL(clicked()),
        textBrowser, SLOT(reload()));
    connect(netcontrollerstopbutton, SIGNAL(clicked()),
        textBrowser, SLOT(stop()));

    // setup interface for Jah Httpd
    //this code hsa been relocated to the desktop preferences panel
    /*
    bindButton = new QPushButton("bind", HttpdBox);
    // create text boxes
    backlogLineEdit = new QLineEdit("1", HttpdBox);
    portLineEdit = new QLineEdit("8080", HttpdBox);
    connect(bindButton, SIGNAL(clicked()),
        this, SLOT(bindHttpd()));
        */ 

    //set the defaults
    //these should be saved in the preferences
    backlogLineEdit = "1";
    portLineEdit = "8080";
}

//these slots are connected to the desktop
void JahNet::updatebacklogLineEdit(const QString & val)
{
    backlogLineEdit = val;
}

void JahNet::updateportLineEdit(const QString & val)
{
    portLineEdit = val;
}

void JahNet::bindHttpd()
{
	activate( );

    if (httpd)
        delete httpd;

    Q_UINT16 port = portLineEdit.toUShort();
    int backlog = backlogLineEdit.toInt();
        
    httpd = new JahHttpd(m_desktop, port, backlog, this);
}

void JahNet::renameConnectButton(int mode)
{
    switch (mode)
        {
        case JTB_ONLINE:
            // rename button here
            connectButton->setText("disconnect");
            break;
        case JTB_OFFLINE:
            // rename button here
            connectButton->setText("connect");
            break;
        }
}

void JahNet::ServerInfo()
{

}

void JahNet::launchServer()
{
    if (serverLaunched)
        {
        clienttextfeedback->append("Servers are already running.\n");
        return;
        }

    server      = new SimpleServer(4242,this); // the text server
    imageserver = new SimpleServer(4243, this); // the image server

    connect( server, SIGNAL(newConnect()), SLOT(serverNewConnect()) );
    connect( imageserver, SIGNAL(newConnect()), SLOT(imageserverNewConnect()) );
    connect( server, SIGNAL(copyText(QString )), SLOT(echoText(QString  )) );

    connect( server, SIGNAL(recipConnect(QHostAddress )), SLOT(recirprocateConnection(QHostAddress  )) );

	serverLaunched = TRUE;
    clienttextfeedback->append("Text and Image servers launched.\n");

}

void JahNet::setConnectServer()
{
    JahPrefs& jprefs  = JahPrefs::getInstance();

	autoLaunchServer = autoConnect->isChecked();

    jprefs.setAutoLaunchServer(autoLaunchServer);
}


void JahNet::sendClipToDesktop()
{
	QValueList < QString > list = theSharedFolder->m_selected_items;
	QValueList < QString >::iterator it;
	for ( it = list.begin( ); it != list.end( ); it ++ )
       	emit updateDesktopString( *it );
}

void JahNet::serverNewConnect()
{
    clienttextfeedback->append( "New connection on text server.\n" );
}

void JahNet::imageserverNewConnect()
{
    clienttextfeedback->append( "New connection on image server.\n" );
}

void JahNet::echoText(QString text)
{
    clienttextfeedback->append( text+"\n" );
}

void JahNet::recirprocateConnection(QHostAddress addy)
{

    //debug("server has received connection, reciprocating");

    //but which server? 4242 or 4243?
    //QString theaddyip = addy.toString();
    //debug("client connected to server from:"+theaddyip);

    int theportnumber;
    theportnumber =    server->lastPortNum();
    //debug("last connection to server was from:");

    //QString tellme;
    //debug(tellme.setNum(theportnumber));

    createNewClient(addy);

}



void JahNet::updatetheDesktop(  assetData datafile )
{    
    //now we need to get the global clip number and increment it before adding...
    int clipnum = *clipnumber;
    clipnum++;

    datafile.clipnumber = clipnum;

    *clipnumber = clipnum;

    //now add datafile to desktop
    //maybe desktop should update clipnumber on its own everytime
    //for other routines as well?
    emit updateDesktop(datafile);
}








