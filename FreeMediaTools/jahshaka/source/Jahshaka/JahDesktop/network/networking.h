/*******************************************************************************
**
** The header file for the Jahshaka network module header
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef NETWORKING_H
#define NETWORKING_H

#include <qwidget.h>
#include <assetdata.h>
#include <qhostaddress.h>

extern QString JahMediaPath;

class JahControl;

class QCheckBox;
class JahDesktop;

class giFTConnection;
class ApollonQueryView;
class ApollonTransferTab;
class FileMainFrame;
class QLineEdit;
class QGridLayout;
class QHBoxLayout;
class QWidgetStack;
class JahHttpd;
class JahProgressBar;
class QTabWidget;
class QListView;
class QComboBox;
class QSpacerItem;
class QHBox;
class QHostAddress;
class QTextEdit;
class SimpleServer;
class QSocket;
class QTextView;
class QFrame;
class QLabel;
class QVBox;
class QPushButton;
class QTextBrowser;

class JahNet :  public  QWidget {
  //  Q_OBJECT

private:
	JahDesktop *m_desktop;
	QWidget *m_parent;
	QHBox *m_controls;
	QHBox *m_controller;
	bool m_started;
		  
public:
	JahNet( JahDesktop *desktop, QWidget* parent, QHBox*, const char* name, 
        	QHBox* controls, QHBox* controller,
        	int* globalclipnumber , JahControl *jc);

  	~JahNet();

  	void activate( );

    JahControl *jahcontrol;

    QWidget* parentobject;
    int*  clipnumber;

    bool NetStatus;

    void buildInterface( QHBox* qtinterface);
    void buildController( QHBox* controller);
    void buildNetframe( QWidget* controller);
    void initializeObjects();
    void initializeUiOptions();

    void ServerInfo();
    
	QString    ModuleName;

public slots:

    void setConnectServer();

    void serverNewConnect();
    void imageserverNewConnect();
    void echoText(QString);
    void requestStats();
    void giftStatusReply(QString status);

    void giftsocketError(int error);

    void cominterface();
    void p2pinterface();
	void netinterface();
    void httpdinterface();

    void languageChange();

    void createNewClient();
    void createNewClient(QHostAddress);

    void  sendClipToServer();

    void recirprocateConnection(QHostAddress addy);

    void updatetheDesktop( assetData );

    //used for the p2p engine
    void startDaemon(  );
    void initializeGift();

    // tries to connect to a remote gift daemon
    void retryRemoteConnection();

    // we've just been connected with the giFT daemon
    void connected();

    void searchGiftNetwork(  );
    void getGiftNetworkStats(  );
    void startQuery(const int &index, const QString &text);

    void setupTransferTab();

    void sendClipToDesktop();

	void slotGetHash(QString hash);
	void slotDeleteHash(QString hash);
    void renameConnectButton(int);
    void bindHttpd();

    void updatebacklogLineEdit(const QString &);
    void updateportLineEdit(const QString &);


private slots:

    void closeConnection();
    void sendToServer();
    void sendMediaToServer(assetData,int);
    void socketReadyRead();
    void socketConnected();
    void socketConnectionClosed();
    void socketError(int);
    void datasocketError(int);
    void datasocketConnected();
    void launchServer();

    //ui switching
    void togglep2plocalview(QWidget*);

signals:

    void GrabClip();
    void updateDesktop( assetData );

    void updateDesktopString(QString);


	void giftItemReply(QString);

    void sigSendHash(QString);
	void sigDeleteHash(QString hash); // sends the hash which was removed from the list

    //used for main ui switching
    void switchToNetCom();
    void switchToNetP2P();
	void switchToNetNet();
    void switchToNetHttpd();

protected:

private:

	bool autoLaunchServer;
	bool serverLaunched;

    giFTConnection* jahGift;

    ApollonQueryView* kqv;

    ApollonTransferTab   *m_transTab;


    QValueList<QString>* m_hashList;  // a pointer to this list exists also in apollonqueryview!!

    int          m_numStatUpdates;   // tells how many times the stats have been updated
	int          m_tries;            // number of tries to start the giFT daemon
	QTextEdit   *m_infoBox;          // the one and only info box which shows the stats

    QTimer *m_timer;

    FileMainFrame* theSharedFolder;


    SimpleServer *server ;
    SimpleServer *imageserver ;

    QSocket *socket; QSocket *datasocket;
    QSocket *recipsocket;

    assetData currentclip;
    int currentclipnumber;

    QTextView *infoText;
    QTextView *infoText2;

    QHBox *hb;
    QLineEdit *inputText;
    QPushButton *send;
	QPushButton *close;
    QCheckBox* autoConnect;

    QFrame* JahP2PSearch;

 
    QFrame* mainsearchframe;
    QHBox* mainshareframe;
    QVBox* mainnetframe;

    QGridLayout* mainnetLayout;
    QGridLayout* jahshakanetLayout;

    QVBox   *netbox;
    QHBox   *ComBox;
 
    QFrame* clientBox2;
    QFrame* clientBox1;

    QFrame* clientframe;
    QFrame* NetboxLayersFrame;
    QLabel* ClientLabel;
    QLabel* serverlabel;
    QLabel* texteditLabel;

    QLineEdit* serveredit;

    QLineEdit* texteditbox;
    QPushButton* sendbutton, * sendimagebutton, * connectbutton,* closebutton;
    QPushButton *launchserverbutton;
    QTextView* clienttextfeedback;

    ////////////////////////////////////////////////////////////////////////////
    //interface variables
    QFrame*      LayersFrame;

    QPushButton*  AddClient;
    QPushButton*  AddServer;

    QPushButton*  AddP2P;
    QFrame*       P2PBox;
	QVBox *NetBox;

	QPushButton *AddNet;
    //QPushButton *AddHttpd;
    QVBox *HttpdBox;

	QWidgetStack*  NetInterface;



    QPushButton*   ResetButton;             QPushButton*   SaveButton;    QPushButton*   LoadButton;
	  QCheckBox*     SmoothSelect;            QCheckBox*     BlurSelect;    QCheckBox*     TextSelect;

    QFrame*        ControlFrame_2;
    QWidgetStack*  fxoptionals;
    QHBox*         fxoptionals_clipone;     QHBox*         fxoptionals_cliptwo;
    QHBox*         fxoptionals_object;      QHBox*         fxoptionals_world;
    QHBox*         fxoptionals_text;

    QFrame*        ControlFrame1;
    QLabel*        RotateText_1;
    QLabel*        TranslateText_1;
    QLabel*        ScaleText_1;


    //for jahgift interface

    QTabWidget* jahp2p;
    QWidget* tab;
    QLineEdit* usersonlinebox_2;
    QLabel* giftserveridtext;
    QPushButton* connectgiftbutton;
    QPushButton* disconnectgiftbutton;
    QLabel* giftportidtext;
    QLineEdit* usersonlinebox_2_2;
    QLineEdit* shareduserbox;
    QLabel* textLabel3_4;
    QLineEdit* localsharesbox;
    QLabel* textLabel3_3;
    QLineEdit* totalsharedbox;
    QLabel* textLabel3_2;
    QLabel* textLabel3;
    QLineEdit* usersonlinebox;
    QLabel* giftportidtext_2;
    QLabel* giftstatuslable;
    QLineEdit* gidtstatusbox;
    QWidget* tab_2;
    QListView* searchresultes;
    QFrame* searchframe;
    QLabel* searchforlabel;
    QLineEdit* searchforlineedit;
    QPushButton* searchbutton;
    QLabel* mediatypelabel;
    QComboBox* mediatypebox;
    QComboBox* mediatypebox_2;
    QLabel* mediacategorylabel;
    QWidget* tab_3;
    QLabel* textLabel5_2;
    QLabel* textLabel5;
    QListView* outgoingtraffic;
    //QListView* incomingtraffic;
    QHBox* incomingtraffic;
    QWidget* tab_4;
    QTextEdit* textEdit2;
    QWidget* tab_5;
    QLabel* publicsharelabel;
    QLineEdit* sharelocation;
    QPushButton* setshare;
    QPushButton* syncshares;

    QWidgetStack* theSharedNetLayout;

    //for gift app
     QDialog * MaDialog;
     QTabWidget * viewtab;

	// for net screen
	QPushButton *connectButton;
    JahProgressBar *jtbprogbar;
	QTextBrowser *textBrowser;
    // QHBoxLayout* frame3Layout;
    //QSpacerItem* netspacer1;
    //QSpacerItem* netspacer2;
    
   // for httpd server
    //QPushButton *bindButton;
    //QLineEdit *backlogLineEdit, *portLineEdit;
    QString backlogLineEdit;
    QString portLineEdit;

    JahHttpd *httpd; 
    
    QFrame* mainnetcontroller;
    QWidgetStack* MainnetController;
    QWidget* netpage;
    QFrame* controllernetframe;
    
    QPushButton* netcontrollerhomebutton;
    QPushButton* netcontrollerbackbutton;
    QPushButton* netcontrollerforebutton;
    QPushButton* netcontrollerreloadbutton;
    QPushButton* netcontrollerstopbutton;

    QHBoxLayout* Form3Layout;
    QHBoxLayout* mainnetcontrollerLayout;
    QHBoxLayout* netpageLayout;
    QSpacerItem* netspacer1;
    QSpacerItem* netspacer4;
    
};

#endif























