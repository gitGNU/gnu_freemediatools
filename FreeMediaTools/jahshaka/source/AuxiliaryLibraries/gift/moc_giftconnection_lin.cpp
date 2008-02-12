/****************************************************************************
** giFTConnection meta object code from reading C++ file 'giftconnection_lin.h'
**
** Created: Wed Jan 23 18:40:51 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "giftconnection_lin.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *giFTConnection::className() const
{
    return "giFTConnection";
}

QMetaObject *giFTConnection::metaObj = 0;
static QMetaObjectCleanUp cleanUp_giFTConnection( "giFTConnection", &giFTConnection::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString giFTConnection::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "giFTConnection", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString giFTConnection::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "giFTConnection", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* giFTConnection::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"sockReadReady", 0, 0 };
    static const QUMethod slot_1 = {"sockClosed", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"sockError", 1, param_slot_2 };
    static const QUMethod slot_3 = {"makeAttachRequest", 0, 0 };
    static const QUMethod slot_4 = {"makeStatsRequest", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "ID", &static_QUType_int, 0, QUParameter::In },
	{ "index", &static_QUType_int, 0, QUParameter::In },
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"makeSearchRequest", 3, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"makeCancelSearchRequest", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"makeCancelBrowseRequest", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "ID", &static_QUType_int, 0, QUParameter::In },
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"makeLocateRequest", 2, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "user", &static_QUType_QString, 0, QUParameter::In },
	{ "hash", &static_QUType_QString, 0, QUParameter::In },
	{ "size", &static_QUType_QString, 0, QUParameter::In },
	{ "url", &static_QUType_QString, 0, QUParameter::In },
	{ "file", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"makeAddRequest", 5, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "ID", &static_QUType_int, 0, QUParameter::In },
	{ "action", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"makeTransferRequest", 2, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "ID", &static_QUType_int, 0, QUParameter::In },
	{ "url", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"makeDelSourceRequest", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "action", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"makeShareRequest", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "ID", &static_QUType_int, 0, QUParameter::In },
	{ "user", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"makeBrowseRequest", 2, param_slot_13 };
    static const QUMethod slot_14 = {"makeQuitRequest", 0, 0 };
    static const QUParameter param_slot_15[] = {
	{ "req", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_15 = {"addRequest", 1, param_slot_15 };
    static const QUMethod slot_16 = {"sockWrite", 0, 0 };
    static const QUParameter param_slot_17[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::Out },
	{ "sendstr", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_17 = {"escapeStr", 2, param_slot_17 };
    static const QUMethod slot_18 = {"restartDaemon", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "sockReadReady()", &slot_0, QMetaData::Public },
	{ "sockClosed()", &slot_1, QMetaData::Public },
	{ "sockError(int)", &slot_2, QMetaData::Public },
	{ "makeAttachRequest()", &slot_3, QMetaData::Public },
	{ "makeStatsRequest()", &slot_4, QMetaData::Public },
	{ "makeSearchRequest(int,int,QString)", &slot_5, QMetaData::Public },
	{ "makeCancelSearchRequest(int)", &slot_6, QMetaData::Public },
	{ "makeCancelBrowseRequest(int)", &slot_7, QMetaData::Public },
	{ "makeLocateRequest(int,QString)", &slot_8, QMetaData::Public },
	{ "makeAddRequest(QString,QString,QString,QString,QString)", &slot_9, QMetaData::Public },
	{ "makeTransferRequest(int,QString)", &slot_10, QMetaData::Public },
	{ "makeDelSourceRequest(int,QString)", &slot_11, QMetaData::Public },
	{ "makeShareRequest(QString)", &slot_12, QMetaData::Public },
	{ "makeBrowseRequest(int,QString)", &slot_13, QMetaData::Public },
	{ "makeQuitRequest()", &slot_14, QMetaData::Public },
	{ "addRequest(QString&)", &slot_15, QMetaData::Public },
	{ "sockWrite()", &slot_16, QMetaData::Public },
	{ "escapeStr(QString&)", &slot_17, QMetaData::Public },
	{ "restartDaemon()", &slot_18, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"connected", 0, 0 };
    static const QUMethod signal_1 = {"disconnected", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ "error", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"socketError", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"giftStatusReply", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"giftItemReply", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"giftAddDownloadReply", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_6 = {"giftAddUploadReply", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"giftAddSourceReply", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_8 = {"giftDelSourceReply", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"giftChgUploadReply", 1, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_10 = {"giftChgDownloadReply", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_11 = {"giftDelDownloadReply", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_12 = {"giftDelUploadReply", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_13 = {"giftShareReply", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_ptr, "pid_t", QUParameter::In }
    };
    static const QUMethod signal_14 = {"sigGiFTPID", 1, param_signal_14 };
    static const QMetaData signal_tbl[] = {
	{ "connected()", &signal_0, QMetaData::Public },
	{ "disconnected()", &signal_1, QMetaData::Public },
	{ "socketError(int)", &signal_2, QMetaData::Public },
	{ "giftStatusReply(QString)", &signal_3, QMetaData::Public },
	{ "giftItemReply(QString)", &signal_4, QMetaData::Public },
	{ "giftAddDownloadReply(QString)", &signal_5, QMetaData::Public },
	{ "giftAddUploadReply(QString)", &signal_6, QMetaData::Public },
	{ "giftAddSourceReply(QString)", &signal_7, QMetaData::Public },
	{ "giftDelSourceReply(QString)", &signal_8, QMetaData::Public },
	{ "giftChgUploadReply(QString)", &signal_9, QMetaData::Public },
	{ "giftChgDownloadReply(QString)", &signal_10, QMetaData::Public },
	{ "giftDelDownloadReply(QString)", &signal_11, QMetaData::Public },
	{ "giftDelUploadReply(QString)", &signal_12, QMetaData::Public },
	{ "giftShareReply(QString)", &signal_13, QMetaData::Public },
	{ "sigGiFTPID(pid_t)", &signal_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"giFTConnection", parentObject,
	slot_tbl, 19,
	signal_tbl, 15,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_giFTConnection.setMetaObject( metaObj );
    return metaObj;
}

void* giFTConnection::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "giFTConnection" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL connected
void giFTConnection::connected()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL disconnected
void giFTConnection::disconnected()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL socketError
void giFTConnection::socketError( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL giftStatusReply
void giFTConnection::giftStatusReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL giftItemReply
void giFTConnection::giftItemReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL giftAddDownloadReply
void giFTConnection::giftAddDownloadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL giftAddUploadReply
void giFTConnection::giftAddUploadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 6, t0 );
}

// SIGNAL giftAddSourceReply
void giFTConnection::giftAddSourceReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL giftDelSourceReply
void giFTConnection::giftDelSourceReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 8, t0 );
}

// SIGNAL giftChgUploadReply
void giFTConnection::giftChgUploadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 9, t0 );
}

// SIGNAL giftChgDownloadReply
void giFTConnection::giftChgDownloadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 10, t0 );
}

// SIGNAL giftDelDownloadReply
void giFTConnection::giftDelDownloadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 11, t0 );
}

// SIGNAL giftDelUploadReply
void giFTConnection::giftDelUploadReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 12, t0 );
}

// SIGNAL giftShareReply
void giFTConnection::giftShareReply( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 13, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL sigGiFTPID
void giFTConnection::sigGiFTPID( pid_t t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 14 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool giFTConnection::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: sockReadReady(); break;
    case 1: sockClosed(); break;
    case 2: sockError((int)static_QUType_int.get(_o+1)); break;
    case 3: makeAttachRequest(); break;
    case 4: makeStatsRequest(); break;
    case 5: makeSearchRequest((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 6: makeCancelSearchRequest((int)static_QUType_int.get(_o+1)); break;
    case 7: makeCancelBrowseRequest((int)static_QUType_int.get(_o+1)); break;
    case 8: makeLocateRequest((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 9: makeAddRequest((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4),(QString)static_QUType_QString.get(_o+5)); break;
    case 10: makeTransferRequest((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 11: makeDelSourceRequest((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 12: makeShareRequest((QString)static_QUType_QString.get(_o+1)); break;
    case 13: makeBrowseRequest((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 14: makeQuitRequest(); break;
    case 15: addRequest((QString&)static_QUType_QString.get(_o+1)); break;
    case 16: sockWrite(); break;
    case 17: static_QUType_QString.set(_o,escapeStr((QString&)static_QUType_QString.get(_o+1))); break;
    case 18: restartDaemon(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool giFTConnection::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: connected(); break;
    case 1: disconnected(); break;
    case 2: socketError((int)static_QUType_int.get(_o+1)); break;
    case 3: giftStatusReply((QString)static_QUType_QString.get(_o+1)); break;
    case 4: giftItemReply((QString)static_QUType_QString.get(_o+1)); break;
    case 5: giftAddDownloadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 6: giftAddUploadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 7: giftAddSourceReply((QString)static_QUType_QString.get(_o+1)); break;
    case 8: giftDelSourceReply((QString)static_QUType_QString.get(_o+1)); break;
    case 9: giftChgUploadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 10: giftChgDownloadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 11: giftDelDownloadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 12: giftDelUploadReply((QString)static_QUType_QString.get(_o+1)); break;
    case 13: giftShareReply((QString)static_QUType_QString.get(_o+1)); break;
    case 14: sigGiFTPID((pid_t)(*((pid_t*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool giFTConnection::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool giFTConnection::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
