/****************************************************************************
** JahDesktopTable meta object code from reading C++ file 'desktoptable.h'
**
** Created: Wed Jan 23 19:10:54 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "desktoptable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahDesktopTable::className() const
{
    return "JahDesktopTable";
}

QMetaObject *JahDesktopTable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahDesktopTable( "JahDesktopTable", &JahDesktopTable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahDesktopTable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktopTable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahDesktopTable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktopTable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahDesktopTable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTable::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "id", &static_QUType_int, 0, QUParameter::In },
	{ "newname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"renameAsset", 2, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "renameAsset(int,QString)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahDesktopTable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahDesktopTable.setMetaObject( metaObj );
    return metaObj;
}

void* JahDesktopTable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahDesktopTable" ) )
	return this;
    return QTable::qt_cast( clname );
}

bool JahDesktopTable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: renameAsset((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    default:
	return QTable::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahDesktopTable::qt_emit( int _id, QUObject* _o )
{
    return QTable::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahDesktopTable::qt_property( int id, int f, QVariant* v)
{
    return QTable::qt_property( id, f, v);
}

bool JahDesktopTable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
