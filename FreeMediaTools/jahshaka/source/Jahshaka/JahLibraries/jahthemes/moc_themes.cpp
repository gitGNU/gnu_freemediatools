/****************************************************************************
** JahThemes meta object code from reading C++ file 'themes.h'
**
** Created: Wed Jan 23 19:11:46 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "themes.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahThemes::className() const
{
    return "JahThemes";
}

QMetaObject *JahThemes::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahThemes( "JahThemes", &JahThemes::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahThemes::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahThemes", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahThemes::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahThemes", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahThemes::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"makeTheme", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "makeTheme(const QString&)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahThemes", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahThemes.setMetaObject( metaObj );
    return metaObj;
}

void* JahThemes::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahThemes" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool JahThemes::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeTheme((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahThemes::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahThemes::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool JahThemes::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
