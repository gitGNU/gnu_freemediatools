/****************************************************************************
** JahNet meta object code from reading C++ file 'networking.h'
**
** Created: Wed Jan 23 19:11:20 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "networking.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahNet::className() const
{
    return "JahNet";
}

QMetaObject *JahNet::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahNet( "JahNet", &JahNet::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahNet::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahNet", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahNet::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahNet", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahNet::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"setConnectServer", 0, 0 };
    static const QUMethod slot_1 = {"serverNewConnect", 0, 0 };
    static const QUMethod slot_2 = {"imageserverNewConnect", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"echoText", 1, param_slot_3 };
    static const QUMethod slot_4 = {"requestStats", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "status", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"giftStatusReply", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "error", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"giftsocketError", 1, param_slot_6 };
    static const QUMethod slot_7 = {"cominterface", 0, 0 };
    static const QUMethod slot_8 = {"p2pinterface", 0, 0 };
    static const QUMethod slot_9 = {"netinterface", 0, 0 };
    static const QUMethod slot_10 = {"httpdinterface", 0, 0 };
    static const QUMethod slot_11 = {"languageChange", 0, 0 };
    static const QUMethod slot_12 = {"createNewClient", 0, 0 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_ptr, "QHostAddress", QUParameter::In }
    };
    static const QUMethod slot_13 = {"createNewClient", 1, param_slot_13 };
    static const QUMethod slot_14 = {"sendClipToServer", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ "addy", &static_QUType_ptr, "QHostAddress", QUParameter::In }
    };
    static const QUMethod slot_15 = {"recirprocateConnection", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_16 = {"updatetheDesktop", 1, param_slot_16 };
    static const QUMethod slot_17 = {"startDaemon", 0, 0 };
    static const QUMethod slot_18 = {"initializeGift", 0, 0 };
    static const QUMethod slot_19 = {"retryRemoteConnection", 0, 0 };
    static const QUMethod slot_20 = {"connected", 0, 0 };
    static const QUMethod slot_21 = {"searchGiftNetwork", 0, 0 };
    static const QUMethod slot_22 = {"getGiftNetworkStats", 0, 0 };
    static const QUParameter param_slot_23[] = {
	{ "index", &static_QUType_int, 0, QUParameter::In },
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_23 = {"startQuery", 2, param_slot_23 };
    static const QUMethod slot_24 = {"setupTransferTab", 0, 0 };
    static const QUMethod slot_25 = {"sendClipToDesktop", 0, 0 };
    static const QUParameter param_slot_26[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_26 = {"slotGetHash", 1, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_27 = {"slotDeleteHash", 1, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"renameConnectButton", 1, param_slot_28 };
    static const QUMethod slot_29 = {"bindHttpd", 0, 0 };
    static const QUParameter param_slot_30[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_30 = {"updatebacklogLineEdit", 1, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_31 = {"updateportLineEdit", 1, param_slot_31 };
    static const QUMethod slot_32 = {"closeConnection", 0, 0 };
    static const QUMethod slot_33 = {"sendToServer", 0, 0 };
    static const QUParameter param_slot_34[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_34 = {"sendMediaToServer", 2, param_slot_34 };
    static const QUMethod slot_35 = {"socketReadyRead", 0, 0 };
    static const QUMethod slot_36 = {"socketConnected", 0, 0 };
    static const QUMethod slot_37 = {"socketConnectionClosed", 0, 0 };
    static const QUParameter param_slot_38[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_38 = {"socketError", 1, param_slot_38 };
    static const QUParameter param_slot_39[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"datasocketError", 1, param_slot_39 };
    static const QUMethod slot_40 = {"datasocketConnected", 0, 0 };
    static const QUMethod slot_41 = {"launchServer", 0, 0 };
    static const QUParameter param_slot_42[] = {
	{ 0, &static_QUType_ptr, "QWidget", QUParameter::In }
    };
    static const QUMethod slot_42 = {"togglep2plocalview", 1, param_slot_42 };
    static const QMetaData slot_tbl[] = {
	{ "setConnectServer()", &slot_0, QMetaData::Public },
	{ "serverNewConnect()", &slot_1, QMetaData::Public },
	{ "imageserverNewConnect()", &slot_2, QMetaData::Public },
	{ "echoText(QString)", &slot_3, QMetaData::Public },
	{ "requestStats()", &slot_4, QMetaData::Public },
	{ "giftStatusReply(QString)", &slot_5, QMetaData::Public },
	{ "giftsocketError(int)", &slot_6, QMetaData::Public },
	{ "cominterface()", &slot_7, QMetaData::Public },
	{ "p2pinterface()", &slot_8, QMetaData::Public },
	{ "netinterface()", &slot_9, QMetaData::Public },
	{ "httpdinterface()", &slot_10, QMetaData::Public },
	{ "languageChange()", &slot_11, QMetaData::Public },
	{ "createNewClient()", &slot_12, QMetaData::Public },
	{ "createNewClient(QHostAddress)", &slot_13, QMetaData::Public },
	{ "sendClipToServer()", &slot_14, QMetaData::Public },
	{ "recirprocateConnection(QHostAddress)", &slot_15, QMetaData::Public },
	{ "updatetheDesktop(assetData)", &slot_16, QMetaData::Public },
	{ "startDaemon()", &slot_17, QMetaData::Public },
	{ "initializeGift()", &slot_18, QMetaData::Public },
	{ "retryRemoteConnection()", &slot_19, QMetaData::Public },
	{ "connected()", &slot_20, QMetaData::Public },
	{ "searchGiftNetwork()", &slot_21, QMetaData::Public },
	{ "getGiftNetworkStats()", &slot_22, QMetaData::Public },
	{ "startQuery(const int&,const QString&)", &slot_23, QMetaData::Public },
	{ "setupTransferTab()", &slot_24, QMetaData::Public },
	{ "sendClipToDesktop()", &slot_25, QMetaData::Public },
	{ "slotGetHash(QString)", &slot_26, QMetaData::Public },
	{ "slotDeleteHash(QString)", &slot_27, QMetaData::Public },
	{ "renameConnectButton(int)", &slot_28, QMetaData::Public },
	{ "bindHttpd()", &slot_29, QMetaData::Public },
	{ "updatebacklogLineEdit(const QString&)", &slot_30, QMetaData::Public },
	{ "updateportLineEdit(const QString&)", &slot_31, QMetaData::Public },
	{ "closeConnection()", &slot_32, QMetaData::Private },
	{ "sendToServer()", &slot_33, QMetaData::Private },
	{ "sendMediaToServer(assetData,int)", &slot_34, QMetaData::Private },
	{ "socketReadyRead()", &slot_35, QMetaData::Private },
	{ "socketConnected()", &slot_36, QMetaData::Private },
	{ "socketConnectionClosed()", &slot_37, QMetaData::Private },
	{ "socketError(int)", &slot_38, QMetaData::Private },
	{ "datasocketError(int)", &slot_39, QMetaData::Private },
	{ "datasocketConnected()", &slot_40, QMetaData::Private },
	{ "launchServer()", &slot_41, QMetaData::Private },
	{ "togglep2plocalview(QWidget*)", &slot_42, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_1 = {"updateDesktop", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"updateDesktopString", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"giftItemReply", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"sigSendHash", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"sigDeleteHash", 1, param_signal_5 };
    static const QUMethod signal_6 = {"switchToNetCom", 0, 0 };
    static const QUMethod signal_7 = {"switchToNetP2P", 0, 0 };
    static const QUMethod signal_8 = {"switchToNetNet", 0, 0 };
    static const QUMethod signal_9 = {"switchToNetHttpd", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Private },
	{ "updateDesktop(assetData)", &signal_1, QMetaData::Private },
	{ "updateDesktopString(QString)", &signal_2, QMetaData::Private },
	{ "giftItemReply(QString)", &signal_3, QMetaData::Private },
	{ "sigSendHash(QString)", &signal_4, QMetaData::Private },
	{ "sigDeleteHash(QString)", &signal_5, QMetaData::Private },
	{ "switchToNetCom()", &signal_6, QMetaData::Private },
	{ "switchToNetP2P()", &signal_7, QMetaData::Private },
	{ "switchToNetNet()", &signal_8, QMetaData::Private },
	{ "switchToNetHttpd()", &signal_9, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahNet", parentObject,
	slot_tbl, 43,
	signal_tbl, 10,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahNet.setMetaObject( metaObj );
    return metaObj;
}

void* JahNet::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahNet" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL GrabClip
void JahNet::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL updateDesktop
void JahNet::updateDesktop( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL updateDesktopString
void JahNet::updateDesktopString( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL giftItemReply
void JahNet::giftItemReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL sigSendHash
void JahNet::sigSendHash( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL sigDeleteHash
void JahNet::sigDeleteHash( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL switchToNetCom
void JahNet::switchToNetCom()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

// SIGNAL switchToNetP2P
void JahNet::switchToNetP2P()
{
    activate_signal( staticMetaObject()->signalOffset() + 7 );
}

// SIGNAL switchToNetNet
void JahNet::switchToNetNet()
{
    activate_signal( staticMetaObject()->signalOffset() + 8 );
}

// SIGNAL switchToNetHttpd
void JahNet::switchToNetHttpd()
{
    activate_signal( staticMetaObject()->signalOffset() + 9 );
}

bool JahNet::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setConnectServer(); break;
    case 1: serverNewConnect(); break;
    case 2: imageserverNewConnect(); break;
    case 3: echoText((QString)static_QUType_QString.get(_o+1)); break;
    case 4: requestStats(); break;
    case 5: giftStatusReply((QString)static_QUType_QString.get(_o+1)); break;
    case 6: giftsocketError((int)static_QUType_int.get(_o+1)); break;
    case 7: cominterface(); break;
    case 8: p2pinterface(); break;
    case 9: netinterface(); break;
    case 10: httpdinterface(); break;
    case 11: languageChange(); break;
    case 12: createNewClient(); break;
    case 13: createNewClient((QHostAddress)(*((QHostAddress*)static_QUType_ptr.get(_o+1)))); break;
    case 14: sendClipToServer(); break;
    case 15: recirprocateConnection((QHostAddress)(*((QHostAddress*)static_QUType_ptr.get(_o+1)))); break;
    case 16: updatetheDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 17: startDaemon(); break;
    case 18: initializeGift(); break;
    case 19: retryRemoteConnection(); break;
    case 20: connected(); break;
    case 21: searchGiftNetwork(); break;
    case 22: getGiftNetworkStats(); break;
    case 23: startQuery((const int&)static_QUType_int.get(_o+1),(const QString&)static_QUType_QString.get(_o+2)); break;
    case 24: setupTransferTab(); break;
    case 25: sendClipToDesktop(); break;
    case 26: slotGetHash((QString)static_QUType_QString.get(_o+1)); break;
    case 27: slotDeleteHash((QString)static_QUType_QString.get(_o+1)); break;
    case 28: renameConnectButton((int)static_QUType_int.get(_o+1)); break;
    case 29: bindHttpd(); break;
    case 30: updatebacklogLineEdit((const QString&)static_QUType_QString.get(_o+1)); break;
    case 31: updateportLineEdit((const QString&)static_QUType_QString.get(_o+1)); break;
    case 32: closeConnection(); break;
    case 33: sendToServer(); break;
    case 34: sendMediaToServer((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2)); break;
    case 35: socketReadyRead(); break;
    case 36: socketConnected(); break;
    case 37: socketConnectionClosed(); break;
    case 38: socketError((int)static_QUType_int.get(_o+1)); break;
    case 39: datasocketError((int)static_QUType_int.get(_o+1)); break;
    case 40: datasocketConnected(); break;
    case 41: launchServer(); break;
    case 42: togglep2plocalview((QWidget*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahNet::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    case 1: updateDesktop((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 2: updateDesktopString((QString)static_QUType_QString.get(_o+1)); break;
    case 3: giftItemReply((QString)static_QUType_QString.get(_o+1)); break;
    case 4: sigSendHash((QString)static_QUType_QString.get(_o+1)); break;
    case 5: sigDeleteHash((QString)static_QUType_QString.get(_o+1)); break;
    case 6: switchToNetCom(); break;
    case 7: switchToNetP2P(); break;
    case 8: switchToNetNet(); break;
    case 9: switchToNetHttpd(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahNet::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahNet::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
