/****************************************************************************
** ApollonQueryView meta object code from reading C++ file 'apollonqueryview.h'
**
** Created: Wed Jan 23 18:40:47 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "apollonqueryview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ApollonQueryView::className() const
{
    return "ApollonQueryView";
}

QMetaObject *ApollonQueryView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApollonQueryView( "ApollonQueryView", &ApollonQueryView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ApollonQueryView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonQueryView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApollonQueryView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonQueryView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApollonQueryView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"moreSearch", 0, 0 };
    static const QUMethod slot_1 = {"stopSearch", 0, 0 };
    static const QUMethod slot_2 = {"deleteThis", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "point", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"queryPopup", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_4 = {"itemDoubleClicked", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "searchReply", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"addItem", 1, param_slot_5 };
    static const QUMethod slot_6 = {"browse", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "index", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"browse", 1, param_slot_7 };
    static const QUMethod slot_8 = {"download", 0, 0 };
    static const QUMethod slot_9 = {"closeAllTabs", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "s", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"findItems", 1, param_slot_10 };
    static const QUMethod slot_11 = {"clearFindbox", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "list", &static_QUType_ptr, "QValueList<QString>", QUParameter::In }
    };
    static const QUMethod slot_12 = {"setHashList", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "hash", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"slotDeleteHash", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "index", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"updateFilterColumn", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "index", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"updateFilterMode", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_16 = {"itemVisible", 2, param_slot_16 };
    static const QMetaData slot_tbl[] = {
	{ "moreSearch()", &slot_0, QMetaData::Public },
	{ "stopSearch()", &slot_1, QMetaData::Public },
	{ "deleteThis()", &slot_2, QMetaData::Public },
	{ "queryPopup(QListViewItem*,const QPoint&,int)", &slot_3, QMetaData::Public },
	{ "itemDoubleClicked(QListViewItem*)", &slot_4, QMetaData::Public },
	{ "addItem(QString)", &slot_5, QMetaData::Public },
	{ "browse()", &slot_6, QMetaData::Public },
	{ "browse(int)", &slot_7, QMetaData::Public },
	{ "download()", &slot_8, QMetaData::Public },
	{ "closeAllTabs()", &slot_9, QMetaData::Public },
	{ "findItems(const QString&)", &slot_10, QMetaData::Public },
	{ "clearFindbox()", &slot_11, QMetaData::Public },
	{ "setHashList(QValueList<QString>*)", &slot_12, QMetaData::Public },
	{ "slotDeleteHash(QString)", &slot_13, QMetaData::Public },
	{ "updateFilterColumn(int)", &slot_14, QMetaData::Private },
	{ "updateFilterMode(int)", &slot_15, QMetaData::Private },
	{ "itemVisible(QListViewItem*)", &slot_16, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In },
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"giftAddRequest", 5, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"giftCancelSearchRequest", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"giftBrowseRequest", 1, param_signal_2 };
    static const QUMethod signal_3 = {"closetabs", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "giftAddRequest(QString,QString,QString,QString,QString)", &signal_0, QMetaData::Private },
	{ "giftCancelSearchRequest(int)", &signal_1, QMetaData::Private },
	{ "giftBrowseRequest(QString)", &signal_2, QMetaData::Private },
	{ "closetabs()", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApollonQueryView", parentObject,
	slot_tbl, 17,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApollonQueryView.setMetaObject( metaObj );
    return metaObj;
}

void* ApollonQueryView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApollonQueryView" ) )
	return this;
    return QVBox::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL giftAddRequest
void ApollonQueryView::giftAddRequest( QString t0, QString t1, QString t2, QString t3, QString t4 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[6];
    static_QUType_QString.set(o+1,t0);
    static_QUType_QString.set(o+2,t1);
    static_QUType_QString.set(o+3,t2);
    static_QUType_QString.set(o+4,t3);
    static_QUType_QString.set(o+5,t4);
    activate_signal( clist, o );
}

// SIGNAL giftCancelSearchRequest
void ApollonQueryView::giftCancelSearchRequest( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL giftBrowseRequest
void ApollonQueryView::giftBrowseRequest( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL closetabs
void ApollonQueryView::closetabs()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

bool ApollonQueryView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: moreSearch(); break;
    case 1: stopSearch(); break;
    case 2: deleteThis(); break;
    case 3: queryPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 4: itemDoubleClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 5: addItem((QString)static_QUType_QString.get(_o+1)); break;
    case 6: browse(); break;
    case 7: browse((int)static_QUType_int.get(_o+1)); break;
    case 8: download(); break;
    case 9: closeAllTabs(); break;
    case 10: findItems((const QString&)static_QUType_QString.get(_o+1)); break;
    case 11: clearFindbox(); break;
    case 12: setHashList((QValueList<QString>*)static_QUType_ptr.get(_o+1)); break;
    case 13: slotDeleteHash((QString)static_QUType_QString.get(_o+1)); break;
    case 14: updateFilterColumn((int)static_QUType_int.get(_o+1)); break;
    case 15: updateFilterMode((int)static_QUType_int.get(_o+1)); break;
    case 16: static_QUType_bool.set(_o,itemVisible((QListViewItem*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ApollonQueryView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: giftAddRequest((QString)static_QUType_QString.get(_o+1),(QString)static_QUType_QString.get(_o+2),(QString)static_QUType_QString.get(_o+3),(QString)static_QUType_QString.get(_o+4),(QString)static_QUType_QString.get(_o+5)); break;
    case 1: giftCancelSearchRequest((int)static_QUType_int.get(_o+1)); break;
    case 2: giftBrowseRequest((QString)static_QUType_QString.get(_o+1)); break;
    case 3: closetabs(); break;
    default:
	return QVBox::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ApollonQueryView::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool ApollonQueryView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
