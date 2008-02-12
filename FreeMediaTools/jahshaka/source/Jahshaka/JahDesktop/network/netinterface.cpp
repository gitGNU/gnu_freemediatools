/*******************************************************************************
**
** The source file for the Jahshaka editing interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "networking.h"
#include <qframe.h>
#include <qwidgetstack.h>
#include <qhbox.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qlistview.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtextview.h>
#include <qvbox.h>

void JahNet::buildInterface( QHBox* qtinterface ) {

///////////////////////////This is the interface////////////////////////////////

    LayersFrame = new QFrame( qtinterface, "EffectsFrame" );
    LayersFrame->setGeometry( QRect( 0, 0, 80, 140 ) );
    LayersFrame->setMinimumSize( QSize( 80, 140 ) );  //was 960
    LayersFrame->setMaximumSize( QSize( 80, 140 ) );

    //////////////////// layer options /////////////////////////
    AddClient = new QPushButton( LayersFrame, "client" );
    AddClient->setGeometry( QRect( 0, 5, 75, 25 ) );
    AddClient->setText( tr( "JahCom"  ) );
    connect( AddClient,  SIGNAL(clicked()), this, SLOT( cominterface())  );

    AddP2P = new QPushButton( LayersFrame, "jahp2p" );
    AddP2P->setGeometry( QRect( 0, 65, 75, 25 ) );
    AddP2P->setText( tr( "JahP2P"  ) );
    connect( AddP2P,  SIGNAL(clicked()), this, SLOT( p2pinterface())  );

	AddNet = new QPushButton( LayersFrame, "jahnet" );
    AddNet->setGeometry( QRect( 0, 35, 75, 25 ) );
    AddNet->setText( tr( "JahNet"  ) );
    connect( AddNet,  SIGNAL(clicked()), this, SLOT( netinterface())  );

    //AddHttpd = new QPushButton( LayersFrame, "jahhttpd");
    //AddHttpd->setGeometry( QRect( 0, 95, 75, 25 ) );
    //AddHttpd->setText( tr( "JahHttpd"  ) );
    //connect( AddHttpd,  SIGNAL(clicked()), this, SLOT( httpdinterface())  );

    //////////////////////////////////////////////////////////////

    //this stack controls the different network modules controlling ui boxes
    NetInterface = new QWidgetStack(qtinterface);
    NetInterface->setGeometry( QRect( 0, 0, 855, 140) );

    ComBox   = new QHBox(NetInterface); NetInterface->addWidget(ComBox,0);
    
    P2PBox = new QFrame(NetInterface); NetInterface->addWidget(P2PBox,1);

    NetBox = new QVBox(NetInterface); NetInterface->addWidget(NetBox,2);

    NetInterface->raiseWidget(0);

    HttpdBox = new QVBox(NetInterface); NetInterface->addWidget(HttpdBox,3);

	/////////////////////////////////////////////
	// jahnet interface

    clientframe = new QFrame( ComBox, "clientframe" );
    clientframe->setGeometry( QRect( 0, 0, 620, 140 ) );
    clientframe->setFrameShape( QFrame::StyledPanel );
    clientframe->setFrameShadow( QFrame::Raised );

	////////////////////////////////////////////////

    ClientLabel = new QLabel( clientframe, "ClientLabel" );
    ClientLabel->setGeometry( QRect( 70, 1, 170, 20 ) );
    ClientLabel->setText( tr( "JahNet Client Feedback" ) );
    
	clienttextfeedback = new QTextView( clientframe, "clienttextfeedback" );
	clienttextfeedback->setGeometry( QRect( 10, 24, 270, 150 ) );

	//////////////////////////////////////////////

    texteditLabel = new QLabel( clientframe, "ClientLabel" );
    texteditLabel->setGeometry( QRect( 330, 1, 170, 20 ) );
    texteditLabel->setText( tr( "Communication" ) );

    texteditbox = new QLineEdit( clientframe, "texteditbox" );
    texteditbox->setGeometry( QRect( 300, 24, 200, 110 ) );
    connect(  texteditbox,     SIGNAL(returnPressed()), SLOT(sendToServer())    );

    sendbutton = new QPushButton( clientframe, "sendbutton" );
    sendbutton->setGeometry( QRect( 300, 140, 70, 30 ) );
    sendbutton->setText( tr( "Send" ) );
    connect(  sendbutton,      SIGNAL(clicked()), SLOT(sendToServer())    );

    sendimagebutton = new QPushButton( clientframe, "sendclipbutton" );
    sendimagebutton->setGeometry( QRect( 410, 140, 90, 30 ) );
    sendimagebutton->setText( tr( "Send Clip" ) );
    connect(  sendimagebutton, SIGNAL(clicked()), SLOT(sendClipToServer()) );

    ///////////////////////////////////////////////
	
	serverlabel = new QLabel( clientframe, "serverlabel" );
    serverlabel->setGeometry( QRect( 540, 1, 120, 20 ) );
    serverlabel->setText( tr( "Connect to Client IP" ) );

    serveredit = new QLineEdit( clientframe, "serveredit" );
    serveredit->setGeometry( QRect( 540, 20, 200, 26 ) );
    serveredit->setText( "localhost" );

    connectbutton = new QPushButton( clientframe, "connectbutton" );
    connectbutton->setGeometry( QRect( 540, 55, 70, 30 ) );
    connectbutton->setText( tr( "Connect" ) );
    connect(  connectbutton,   SIGNAL(clicked()), SLOT(createNewClient()) );

    launchserverbutton = new QPushButton( clientframe, "launchserverbutton" );
    launchserverbutton->setGeometry( QRect( 700, 160, 70, 30 ) );
    launchserverbutton->setText( tr( "Launch" ) );
    connect(launchserverbutton, SIGNAL(clicked()), SLOT(launchServer()) );

    closebutton = new QPushButton( clientframe, "closebutton" );
    closebutton->setGeometry( QRect( 670, 55, 70, 30 ) );
    closebutton->setText( tr( "Close" ) );
    //closebutton->setPalette( QPalette( QColor(255, 25, 25) ) );
    connect(  closebutton,     SIGNAL(clicked()), SLOT(closeConnection()) );

    autoConnect = new QCheckBox( clientframe, "autoConnect" );
    autoConnect->setGeometry( QRect( 540, 160, 150, 21 ) );
    autoConnect->setText("Auto launch Server");
    autoConnect->setChecked(autoLaunchServer);
    connect( autoConnect,  SIGNAL(clicked()), this, SLOT(setConnectServer())  );


    ////////////////// p2 interface and options

    jahp2p = new QTabWidget( P2PBox, "jahp2p" );
    jahp2p->setGeometry( QRect( 0, 0, 750, 200 ) );
    connect( jahp2p,  SIGNAL(currentChanged(QWidget*)), this, SLOT( togglep2plocalview(QWidget*))  );

    tab = new QWidget( jahp2p, "tab" );

    usersonlinebox_2 = new QLineEdit( tab, "usersonlinebox_2" );
    usersonlinebox_2->setGeometry( QRect( 100, 20, 181, 21 ) );

    giftserveridtext = new QLabel( tab, "giftserveridtext" );
    giftserveridtext->setGeometry( QRect( 10, 20, 90, 21 ) );

    connectgiftbutton = new QPushButton( tab, "connectgiftbutton" );
    connectgiftbutton->setGeometry( QRect( 100, 130, 70, 21 ) );
    connect( connectgiftbutton,  SIGNAL(clicked()), this, SLOT( startDaemon())  );

    disconnectgiftbutton = new QPushButton( tab, "disconnectgiftbutton" );
    disconnectgiftbutton->setGeometry( QRect( 201, 130, 80, 21 ) );
    //connect( disconnectgiftbutton,  SIGNAL(clicked()), this, SLOT( disconnectFromGift())  );

    giftportidtext = new QLabel( tab, "giftportidtext" );
    giftportidtext->setGeometry( QRect( 8, 48, 90, 21 ) );

    usersonlinebox_2_2 = new QLineEdit( tab, "usersonlinebox_2_2" );
    usersonlinebox_2_2->setGeometry( QRect( 100, 50, 181, 21 ) );

    shareduserbox = new QLineEdit( tab, "shareduserbox" );
    shareduserbox->setGeometry( QRect( 530, 130, 181, 21 ) );

    textLabel3_4 = new QLabel( tab, "textLabel3_4" );
    textLabel3_4->setGeometry( QRect( 420, 130, 90, 21 ) );

    localsharesbox = new QLineEdit( tab, "localsharesbox" );
    localsharesbox->setGeometry( QRect( 530, 100, 181, 21 ) );

    textLabel3_3 = new QLabel( tab, "textLabel3_3" );
    textLabel3_3->setGeometry( QRect( 420, 100, 90, 21 ) );

    totalsharedbox = new QLineEdit( tab, "totalsharedbox" );
    totalsharedbox->setGeometry( QRect( 530, 50, 181, 21 ) );

    textLabel3_2 = new QLabel( tab, "textLabel3_2" );
    textLabel3_2->setGeometry( QRect( 420, 50, 90, 21 ) );

    textLabel3 = new QLabel( tab, "textLabel3" );
    textLabel3->setGeometry( QRect( 420, 20, 90, 21 ) );

    usersonlinebox = new QLineEdit( tab, "usersonlinebox" );
    usersonlinebox->setGeometry( QRect( 530, 20, 181, 21 ) );

    giftportidtext_2 = new QLabel( tab, "giftportidtext_2" );
    giftportidtext_2->setGeometry( QRect( 8, 98, 90, 21 ) );

    giftstatuslable = new QLabel( tab, "giftstatuslable" );
    giftstatuslable->setGeometry( QRect( 108, 98, 60, 21 ) );

    gidtstatusbox = new QLineEdit( tab, "gidtstatusbox" );
    gidtstatusbox->setGeometry( QRect( 188, 98, 90, 21 ) );
    jahp2p->insertTab( tab, QString("") );

    tab_2 = new QWidget( jahp2p, "tab_2" );
/*
    searchresultes = new QListView( tab_2, "searchresultes" );
    searchresultes->addColumn( tr( "Column 1" ) );
    searchresultes->setGeometry( QRect( 10, 40, 720, 130 ) );
*/
    searchframe = new QFrame( tab_2, "searchframe" );
    searchframe->setGeometry( QRect( 8, 0, 721, 40 ) );
    searchframe->setFrameShape( QFrame::StyledPanel );
    searchframe->setFrameShadow( QFrame::Raised );

    searchforlabel = new QLabel( searchframe, "searchforlabel" );
    searchforlabel->setGeometry( QRect( 11, 10, 70, 21 ) );

    searchforlineedit = new QLineEdit( searchframe, "searchforlineedit" );
    searchforlineedit->setGeometry( QRect( 90, 10, 200, 21 ) );
    connect( searchforlineedit,  SIGNAL(returnPressed()), this, SLOT( searchGiftNetwork())  );

    searchbutton = new QPushButton( searchframe, "searchbutton" );
    searchbutton->setGeometry( QRect( 300, 10, 40, 21 ) );
    connect( searchbutton,  SIGNAL(clicked()), this, SLOT( searchGiftNetwork())  );

    mediatypelabel = new QLabel( searchframe, "mediatypelabel" );
    mediatypelabel->setGeometry( QRect( 360, 10, 60, 21 ) );

    mediatypebox = new QComboBox( FALSE, searchframe, "mediatypebox" );
    mediatypebox->setGeometry( QRect( 420, 10, 110, 20 ) );

    mediatypebox_2 = new QComboBox( FALSE, searchframe, "mediatypebox_2" );
    mediatypebox_2->setGeometry( QRect( 600, 10, 110, 20 ) );

    mediacategorylabel = new QLabel( searchframe, "mediacategorylabel" );
    mediacategorylabel->setGeometry( QRect( 540, 10, 60, 21 ) );
    jahp2p->insertTab( tab_2, QString("") );

	// incomingtraffic = new QListView( tab_3, "incomingtraffic" );
    incomingtraffic = new QHBox( tab_2, "incomingtraffic" );
    //incomingtraffic->addColumn( tr( "Column 1" ) );
    incomingtraffic->setGeometry( QRect( 80, 45, 660, 120 ) );
    incomingtraffic->setFrameShape( QFrame::StyledPanel );
    incomingtraffic->setFrameShadow( QFrame::Raised );

    textLabel5_2 = new QLabel( tab_2, "textLabel5_2" );
    textLabel5_2->setGeometry( QRect( 10, 120, 61, 31 ) );

    textLabel5 = new QLabel( tab_2, "textLabel5" );
    textLabel5->setGeometry( QRect( 10, 45, 61, 31 ) );

    //tab_3 = new QWidget( jahp2p, "tab_3" );
    //outgoingtraffic = new QListView( tab_3, "outgoingtraffic" );
    //outgoingtraffic->addColumn( tr( "Column 1" ) );
    //outgoingtraffic->setGeometry( QRect( 81, 80, 660, 90 ) );
    //jahp2p->insertTab( tab_3, QString("") );

    tab_4 = new QWidget( jahp2p, "tab_4" );

    textEdit2 = new QTextEdit( tab_4, "textEdit2" );
    textEdit2->setGeometry( QRect( 10, 10, 730, 160 ) );
    jahp2p->insertTab( tab_4, QString("") );

    tab_5 = new QWidget( jahp2p, "tab_5" );

    publicsharelabel = new QLabel( tab_5, "publicsharelabel" );
    publicsharelabel->setGeometry( QRect( 20, 30, 90, 21 ) );

    sharelocation = new QLineEdit( tab_5, "sharelocation" );
    sharelocation->setGeometry( QRect( 130, 30, 181, 21 ) );

    setshare = new QPushButton( tab_5, "setshare" );
    setshare->setGeometry( QRect( 330, 30, 60, 21 ) );

    syncshares = new QPushButton( tab_5, "syncshares" );
    syncshares->setGeometry( QRect( 130, 100, 141, 20 ) );
    jahp2p->insertTab( tab_5, QString("") );
    languageChange();
    resize( QSize(939, 480).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    initializeUiOptions();

}

void JahNet::languageChange()
{
    //setCaption( tr( "Form1" ) );
    giftserveridtext->setText( tr( "Gift Server Id" ) );
    connectgiftbutton->setText( tr( "Connect" ) );
    disconnectgiftbutton->setText( tr( "Disconnect" ) );
    giftportidtext->setText( tr( "Gift Port Id" ) );
    //textLabel3_4->setText( tr( "Shared/user" ) );
    textLabel3_3->setText( tr( "Total Files" ) );

    textLabel3_2->setText( tr( "Total Shared" ) );
    textLabel3->setText( tr( "Users Online" ) );

    giftportidtext_2->setText( tr( "Network Server" ) );
    giftstatuslable->setText( tr( "Status:" ) );
    jahp2p->changeTab( tab, tr( "Network" ) );
    //searchresultes->header()->setLabel( 0, tr( "Column 1" ) );
    //searchresultes->clear();
    //QListViewItem * item = new QListViewItem( searchresultes, 0 );
    //item->setText( 0, tr( "New Item" ) );

    searchforlabel->setText( tr( "Search For:" ) );
    searchbutton->setText( tr( "GO" ) );
    mediatypelabel->setText( tr( "Media Type:" ) );
    mediacategorylabel->setText( tr( "Category:" ) );
    jahp2p->changeTab( tab_2, tr( "Searches" ) );

    textLabel5_2->setText( tr( "Outgoing" ) );

    textLabel5->setText( tr( "Incoming" ) );

    //outgoingtraffic->header()->setLabel( 0, tr( "Column 1" ) );
    //outgoingtraffic->clear();
    //item = new QListViewItem( outgoingtraffic, 0 );
    //item->setText( 0, tr( "New Item" ) );

    //incomingtraffic->header()->setLabel( 0, tr( "Column 1" ) );
    //incomingtraffic->clear();
    //item = new QListViewItem( incomingtraffic, 0 );
    //item->setText( 0, tr( "New Item" ) );

    jahp2p->changeTab( tab_3, tr( "Transfers" ) );
    jahp2p->changeTab( tab_4, tr( "Console" ) );
    publicsharelabel->setText( tr( "Public Share" ) );
    setshare->setText( tr( "set" ) );
    syncshares->setText( tr( "Sync Shares" ) );
    jahp2p->changeTab( tab_5, tr( "Settings" ) );


}

void JahNet::initializeUiOptions(void)
{
    mediatypebox->insertItem("Everything",-1);
    mediatypebox->insertItem("audio",-1);
    mediatypebox->insertItem("video",-1);
    mediatypebox->insertItem("images",-1);
    mediatypebox->insertItem("textures",-1);
    mediatypebox->insertItem("models",-1);
    mediatypebox->setCurrentItem(0); //4:2:0
}

//need to split parent in 2
//so we can use it for search and display share

void JahNet::buildNetframe( QWidget* parent)
{
    mainnetLayout = new QGridLayout( parent, 1, 1, 11, 6, "Form2Layout");

    theSharedNetLayout = new QWidgetStack(parent);

    mainnetframe = new QVBox( theSharedNetLayout, "mainnetframe" );
    mainnetframe->setFrameShape( QFrame::StyledPanel );
    mainnetframe->setFrameShadow( QFrame::Raised );
    theSharedNetLayout->addWidget(mainnetframe,0);

    mainshareframe = new QHBox( theSharedNetLayout, "mainshareframe" );
    mainshareframe->setFrameShape( QFrame::StyledPanel );
    mainshareframe->setFrameShadow( QFrame::Raised );
    theSharedNetLayout->addWidget(mainshareframe,1);
    
    mainsearchframe = new QFrame( theSharedNetLayout, "mainsearchframe" );
    mainsearchframe->setFrameShape( QFrame::StyledPanel );
    mainsearchframe->setFrameShadow( QFrame::Raised );
    theSharedNetLayout->addWidget(mainsearchframe,2);

 
    jahshakanetLayout = new QGridLayout( mainsearchframe, 1, 1, 11, 6, "frame34Layout");
    viewtab = new QTabWidget( mainsearchframe, "tabWidget6" );
    jahshakanetLayout->addWidget( viewtab, 0, 0 );

    mainnetLayout->addWidget( theSharedNetLayout, 0, 0 );

    theSharedNetLayout->raiseWidget(1);
}


void JahNet::buildController( QHBox* controller)
{ 
       /*
    frame3Layout = new QHBoxLayout( controller, 11, 6, "frame3Layout"); 

    netspacer1 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    frame3Layout->addItem( netspacer1 );

    homeButton = new QPushButton( controller, "pushButton1" );
    homeButton->setText( tr( "home" ) );
    frame3Layout->addWidget( homeButton );

    backButton = new QPushButton( controller, "pushButton2" );
    backButton->setText( tr( "back" ) );
    frame3Layout->addWidget( backButton );

    netspacer2 = new QSpacerItem( 200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    frame3Layout->addItem( netspacer2 );
    */
    
    
    Form3Layout = new QHBoxLayout( controller, 11, 6, "Form1Layout");

    mainnetcontroller = new QFrame( controller, "maincontroller" );
    mainnetcontroller->setMinimumSize( QSize( 0, 35 ) );
    mainnetcontroller->setMaximumSize( QSize( 32767, 35 ) );
    mainnetcontroller->setPaletteBackgroundColor( QColor( 100, 100, 100 ) );
    mainnetcontroller->setFrameShape( QFrame::Box );
    mainnetcontroller->setFrameShadow( QFrame::Plain );
    mainnetcontrollerLayout = new QHBoxLayout( mainnetcontroller, 0, 0, "maincontrollerLayout"); 

    MainnetController = new QWidgetStack( mainnetcontroller, "MainController" );

    netpage = new QWidget( MainnetController, "page" );
    netpageLayout = new QHBoxLayout( netpage, 0, 0, "pageLayout"); 
    netspacer1 = new QSpacerItem( 30, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );
    netpageLayout->addItem( netspacer1 );

    controllernetframe = new QFrame( netpage, "controllerframe" );
    controllernetframe->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, controllernetframe->sizePolicy().hasHeightForWidth() ) );
    controllernetframe->setMinimumSize( QSize( 150, 30 ) );
    controllernetframe->setMaximumSize( QSize( 150, 30 ) );
    controllernetframe->setFrameShape( QFrame::NoFrame );
    controllernetframe->setFrameShadow( QFrame::Plain );

    // XXX Home, Stop, Back, Fore, Reload
    netcontrollerbackbutton = new QPushButton( controllernetframe, "controllerbackbutton" );
    netcontrollerbackbutton->setGeometry( QRect( 0, 2, 30, 28 ) );
    netcontrollerbackbutton->setMinimumSize( QSize( 30, 0 ) );
    netcontrollerbackbutton->setMaximumSize( QSize( 30, 32767 ) );

    netcontrollerforebutton = new QPushButton( controllernetframe, "controllerforebutton" );
    netcontrollerforebutton->setGeometry( QRect( 30, 2, 30, 28 ) );
    netcontrollerforebutton->setMinimumSize( QSize( 30, 0 ) );
    netcontrollerforebutton->setMaximumSize( QSize( 30, 32767 ) );

    netcontrollerreloadbutton = new QPushButton( controllernetframe, "controllerreloadbutton" );
    netcontrollerreloadbutton->setGeometry( QRect( 60, 2, 30, 28 ) );
    netcontrollerreloadbutton->setMinimumSize( QSize( 30, 0 ) );
    netcontrollerreloadbutton->setMaximumSize( QSize( 30, 32767 ) );
    
    netcontrollerstopbutton = new QPushButton( controllernetframe, "controllerstopbutton" );
    netcontrollerstopbutton->setGeometry( QRect( 90, 2, 30, 28 ) );
    netcontrollerstopbutton->setMinimumSize( QSize( 30, 0 ) );
    netcontrollerstopbutton->setMaximumSize( QSize( 30, 32767 ) );

    netcontrollerhomebutton = new QPushButton( controllernetframe, "controllerhomebutton" );
    netcontrollerhomebutton->setGeometry( QRect( 120, 2, 30, 28 ) );
    netcontrollerhomebutton->setMinimumSize( QSize( 30, 0 ) );
    netcontrollerhomebutton->setMaximumSize( QSize( 30, 32767 ) );

    netpageLayout->addWidget( controllernetframe );
    netspacer4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    netpageLayout->addItem( netspacer4 );
    MainnetController->addWidget( netpage, 0 );
    mainnetcontrollerLayout->addWidget( MainnetController );
    Form3Layout->addWidget( mainnetcontroller );

    netcontrollerbackbutton->setText( tr( "<<" ) );
    netcontrollerforebutton->setText( tr( ">>" ) );
    netcontrollerreloadbutton->setText( tr( "<>" ) );
    netcontrollerstopbutton->setText( tr( "X" ) );
    netcontrollerhomebutton->setText( tr( "H" ) );

//    clearWState( WState_Polished );


}













