/****************************************************************************
** GLChar meta object code from reading C++ file 'character.h'
**
** Created: Wed Jan 23 19:13:04 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "character.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLChar::className() const
{
    return "GLChar";
}

QMetaObject *GLChar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLChar( "GLChar", &GLChar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLChar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLChar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLChar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLChar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLChar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLAnime::staticMetaObject();
    static const QUMethod slot_0 = {"addLayer", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"addLayerFromTopMenu", 1, param_slot_1 };
    static const QUMethod slot_2 = {"addLightFromTopMenu", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "addLayer()", &slot_0, QMetaData::Public },
	{ "addLayerFromTopMenu(int)", &slot_1, QMetaData::Public },
	{ "addLightFromTopMenu()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLChar", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLChar.setMetaObject( metaObj );
    return metaObj;
}

void* GLChar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLChar" ) )
	return this;
    return GLAnime::qt_cast( clname );
}

bool GLChar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addLayer(); break;
    case 1: addLayerFromTopMenu((int)static_QUType_int.get(_o+1)); break;
    case 2: addLightFromTopMenu(); break;
    default:
	return GLAnime::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLChar::qt_emit( int _id, QUObject* _o )
{
    return GLAnime::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GLChar::qt_property( int id, int f, QVariant* v)
{
    return GLAnime::qt_property( id, f, v);
}

bool GLChar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
