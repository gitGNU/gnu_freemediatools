/****************************************************************************
** ApollonTransferTab meta object code from reading C++ file 'apollontransfertab.h'
**
** Created: Wed Jan 23 18:40:45 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "apollontransfertab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ApollonTransferTab::className() const
{
    return "ApollonTransferTab";
}

QMetaObject *ApollonTransferTab::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApollonTransferTab( "ApollonTransferTab", &ApollonTransferTab::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ApollonTransferTab::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonTransferTab", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApollonTransferTab::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonTransferTab", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApollonTransferTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ApollonTab::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"addDownloadViewItem", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"delDownloadViewItem", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"addUploadViewItem", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"delUploadViewItem", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"updateDownloadView", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_5 = {"updateDownloadSources", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"updateUploadView", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_7 = {"updateUploadSources", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"addSourceItem", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"delSourceItem", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "line", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"itemReply", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"setShowPopup", 1, param_slot_11 };
    static const QUMethod slot_12 = {"disconnected", 0, 0 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"autoSearchInterval", 1, param_slot_13 };
    static const QUMethod slot_14 = {"checkToolbarActions", 0, 0 };
    static const QUMethod slot_15 = {"previewFile", 0, 0 };
    static const QUMethod slot_16 = {"autoSourceSearch", 0, 0 };
    static const QUMethod slot_17 = {"sourceSearch", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ "autoSearch", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"sourceSearch", 1, param_slot_18 };
    static const QUMethod slot_19 = {"browseUser", 0, 0 };
    static const QUParameter param_slot_20[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_20 = {"getUserIP", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ 0, &static_QUType_ptr, "ApollonListView", QUParameter::In }
    };
    static const QUMethod slot_21 = {"cancelFiles", 1, param_slot_21 };
    static const QUMethod slot_22 = {"removeSourceitem", 0, 0 };
    static const QUMethod slot_23 = {"pauseDownload", 0, 0 };
    static const QUMethod slot_24 = {"unpauseDownload", 0, 0 };
    static const QUParameter param_slot_25[] = {
	{ 0, &static_QUType_ptr, "ApollonListView", QUParameter::In }
    };
    static const QUMethod slot_25 = {"clearFinishedFiles", 1, param_slot_25 };
    static const QUMethod slot_26 = {"clearAll", 0, 0 };
    static const QUParameter param_slot_27[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_27 = {"downloadPopup", 3, param_slot_27 };
    static const QUParameter param_slot_28[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_28 = {"uploadPopup", 3, param_slot_28 };
    static const QUMethod slot_29 = {"slotClickedDownListView", 0, 0 };
    static const QUMethod slot_30 = {"slotClickedUpListView", 0, 0 };
    static const QUMethod slot_31 = {"cancel", 0, 0 };
    static const QUMethod slot_32 = {"clearFinished", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "addDownloadViewItem(QString)", &slot_0, QMetaData::Public },
	{ "delDownloadViewItem(QString)", &slot_1, QMetaData::Public },
	{ "addUploadViewItem(QString)", &slot_2, QMetaData::Public },
	{ "delUploadViewItem(QString)", &slot_3, QMetaData::Public },
	{ "updateDownloadView(QString)", &slot_4, QMetaData::Public },
	{ "updateDownloadSources(QListViewItem*)", &slot_5, QMetaData::Public },
	{ "updateUploadView(QString)", &slot_6, QMetaData::Public },
	{ "updateUploadSources(QListViewItem*)", &slot_7, QMetaData::Public },
	{ "addSourceItem(QString)", &slot_8, QMetaData::Public },
	{ "delSourceItem(QString)", &slot_9, QMetaData::Public },
	{ "itemReply(QString)", &slot_10, QMetaData::Public },
	{ "setShowPopup(int)", &slot_11, QMetaData::Public },
	{ "disconnected()", &slot_12, QMetaData::Public },
	{ "autoSearchInterval(int)", &slot_13, QMetaData::Public },
	{ "checkToolbarActions()", &slot_14, QMetaData::Private },
	{ "previewFile()", &slot_15, QMetaData::Private },
	{ "autoSourceSearch()", &slot_16, QMetaData::Private },
	{ "sourceSearch()", &slot_17, QMetaData::Private },
	{ "sourceSearch(bool)", &slot_18, QMetaData::Private },
	{ "browseUser()", &slot_19, QMetaData::Private },
	{ "getUserIP(int)", &slot_20, QMetaData::Private },
	{ "cancelFiles(ApollonListView*)", &slot_21, QMetaData::Private },
	{ "removeSourceitem()", &slot_22, QMetaData::Private },
	{ "pauseDownload()", &slot_23, QMetaData::Private },
	{ "unpauseDownload()", &slot_24, QMetaData::Private },
	{ "clearFinishedFiles(ApollonListView*)", &slot_25, QMetaData::Private },
	{ "clearAll()", &slot_26, QMetaData::Private },
	{ "downloadPopup(QListViewItem*,const QPoint&,int)", &slot_27, QMetaData::Private },
	{ "uploadPopup(QListViewItem*,const QPoint&,int)", &slot_28, QMetaData::Private },
	{ "slotClickedDownListView()", &slot_29, QMetaData::Private },
	{ "slotClickedUpListView()", &slot_30, QMetaData::Private },
	{ "cancel()", &slot_31, QMetaData::Private },
	{ "clearFinished()", &slot_32, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "user", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"browseUser", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "activeDownloads", &static_QUType_int, 0, QUParameter::In },
	{ "rate", &static_QUType_ptr, "float", QUParameter::In }
    };
    static const QUMethod signal_1 = {"statsUpdate", 2, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sigSendHash", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"sigDeleteHash", 1, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "browseUser(QString)", &signal_0, QMetaData::Private },
	{ "statsUpdate(int,float)", &signal_1, QMetaData::Private },
	{ "sigSendHash(QString)", &signal_2, QMetaData::Private },
	{ "sigDeleteHash(QString)", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApollonTransferTab", parentObject,
	slot_tbl, 33,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApollonTransferTab.setMetaObject( metaObj );
    return metaObj;
}

void* ApollonTransferTab::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApollonTransferTab" ) )
	return this;
    return ApollonTab::qt_cast( clname );
}

// SIGNAL browseUser
void ApollonTransferTab::browseUser( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL statsUpdate
void ApollonTransferTab::statsUpdate( int t0, float t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL sigSendHash
void ApollonTransferTab::sigSendHash( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL sigDeleteHash
void ApollonTransferTab::sigDeleteHash( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

bool ApollonTransferTab::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addDownloadViewItem((QString)static_QUType_QString.get(_o+1)); break;
    case 1: delDownloadViewItem((QString)static_QUType_QString.get(_o+1)); break;
    case 2: addUploadViewItem((QString)static_QUType_QString.get(_o+1)); break;
    case 3: delUploadViewItem((QString)static_QUType_QString.get(_o+1)); break;
    case 4: updateDownloadView((QString)static_QUType_QString.get(_o+1)); break;
    case 5: updateDownloadSources((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 6: updateUploadView((QString)static_QUType_QString.get(_o+1)); break;
    case 7: updateUploadSources((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 8: addSourceItem((QString)static_QUType_QString.get(_o+1)); break;
    case 9: delSourceItem((QString)static_QUType_QString.get(_o+1)); break;
    case 10: itemReply((QString)static_QUType_QString.get(_o+1)); break;
    case 11: setShowPopup((int)static_QUType_int.get(_o+1)); break;
    case 12: disconnected(); break;
    case 13: autoSearchInterval((int)static_QUType_int.get(_o+1)); break;
    case 14: checkToolbarActions(); break;
    case 15: previewFile(); break;
    case 16: autoSourceSearch(); break;
    case 17: sourceSearch(); break;
    case 18: sourceSearch((bool)static_QUType_bool.get(_o+1)); break;
    case 19: browseUser(); break;
    case 20: getUserIP((int)static_QUType_int.get(_o+1)); break;
    case 21: cancelFiles((ApollonListView*)static_QUType_ptr.get(_o+1)); break;
    case 22: removeSourceitem(); break;
    case 23: pauseDownload(); break;
    case 24: unpauseDownload(); break;
    case 25: clearFinishedFiles((ApollonListView*)static_QUType_ptr.get(_o+1)); break;
    case 26: clearAll(); break;
    case 27: downloadPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 28: uploadPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 29: slotClickedDownListView(); break;
    case 30: slotClickedUpListView(); break;
    case 31: cancel(); break;
    case 32: clearFinished(); break;
    default:
	return ApollonTab::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ApollonTransferTab::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: browseUser((QString)static_QUType_QString.get(_o+1)); break;
    case 1: statsUpdate((int)static_QUType_int.get(_o+1),(float)(*((float*)static_QUType_ptr.get(_o+2)))); break;
    case 2: sigSendHash((QString)static_QUType_QString.get(_o+1)); break;
    case 3: sigDeleteHash((QString)static_QUType_QString.get(_o+1)); break;
    default:
	return ApollonTab::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ApollonTransferTab::qt_property( int id, int f, QVariant* v)
{
    return ApollonTab::qt_property( id, f, v);
}

bool ApollonTransferTab::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
