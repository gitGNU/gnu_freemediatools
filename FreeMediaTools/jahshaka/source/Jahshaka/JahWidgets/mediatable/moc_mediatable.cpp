/****************************************************************************
** MediaTable meta object code from reading C++ file 'mediatable.h'
**
** Created: Sat Jan 26 08:34:34 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mediatable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MediaTable::className() const
{
    return "MediaTable";
}

QMetaObject *MediaTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MediaTable( "MediaTable", &MediaTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MediaTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MediaTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MediaTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MediaTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MediaTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    static const QUMethod signal_0 = {"gotFocus", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "gotFocus()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MediaTable", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MediaTable.setMetaObject( metaObj );
    return metaObj;
}

void* MediaTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MediaTable" ) )
	return this;
    return QTable::qt_cast( clname );
}

// SIGNAL gotFocus
void MediaTable::gotFocus()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool MediaTable::qt_invoke( int _id, QUObject* _o )
{
    return QTable::qt_invoke(_id,_o);
}

bool MediaTable::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: gotFocus(); break;
    default:
	return QTable::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MediaTable::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool MediaTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *JahDesktopSideTable::className() const
{
    return "JahDesktopSideTable";
}

QMetaObject *JahDesktopSideTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahDesktopSideTable( "JahDesktopSideTable", &JahDesktopSideTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahDesktopSideTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktopSideTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahDesktopSideTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktopSideTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahDesktopSideTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"showItemInFigure", 2, param_slot_0 };
    static const QUMethod slot_1 = {"stopAnimation", 0, 0 };
    static const QUMethod slot_2 = {"playControlClicked", 0, 0 };
    static const QUMethod slot_3 = {"stopControlClicked", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "show", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"showPlayerControls", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In },
	{ "newname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"renameAsset", 2, param_slot_5 };
    static const QUMethod slot_6 = {"receivedFocus", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_7 = {"onUpdateFrameOffset", 1, param_slot_7 };
    static const QMetaData slot_tbl[] = {
	{ "showItemInFigure(int,int)", &slot_0, QMetaData::Public },
	{ "stopAnimation()", &slot_1, QMetaData::Public },
	{ "playControlClicked()", &slot_2, QMetaData::Public },
	{ "stopControlClicked()", &slot_3, QMetaData::Public },
	{ "showPlayerControls(bool)", &slot_4, QMetaData::Public },
	{ "renameAsset(int,QString)", &slot_5, QMetaData::Public },
	{ "receivedFocus()", &slot_6, QMetaData::Public },
	{ "onUpdateFrameOffset(assetData)", &slot_7, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"currentChanged", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_1 = {"assetSelected", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "currentChanged(int,int)", &signal_0, QMetaData::Public },
	{ "assetSelected(assetData)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahDesktopSideTable", parentObject,
	slot_tbl, 8,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahDesktopSideTable.setMetaObject( metaObj );
    return metaObj;
}

void* JahDesktopSideTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahDesktopSideTable" ) )
	return this;
    if ( !qstrcmp( clname, "AssetTable" ) )
	return (AssetTable*)this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL currentChanged
void JahDesktopSideTable::currentChanged( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL assetSelected
void JahDesktopSideTable::assetSelected( assetData t0 )
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

bool JahDesktopSideTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: showItemInFigure((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: stopAnimation(); break;
    case 2: playControlClicked(); break;
    case 3: stopControlClicked(); break;
    case 4: showPlayerControls((bool)static_QUType_bool.get(_o+1)); break;
    case 5: renameAsset((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 6: receivedFocus(); break;
    case 7: onUpdateFrameOffset((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahDesktopSideTable::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: currentChanged((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: assetSelected((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahDesktopSideTable::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahDesktopSideTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
