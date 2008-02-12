/****************************************************************************
** FilmStrip meta object code from reading C++ file 'filmstrip.h'
**
** Created: Wed Jan 23 19:10:55 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "filmstrip.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FilmStrip::className() const
{
    return "FilmStrip";
}

QMetaObject *FilmStrip::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FilmStrip( "FilmStrip", &FilmStrip::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FilmStrip::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FilmStrip", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FilmStrip::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FilmStrip", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FilmStrip::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"velocityMove", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "velocityMove()", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"FilmStrip", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FilmStrip.setMetaObject( metaObj );
    return metaObj;
}

void* FilmStrip::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FilmStrip" ) )
	return this;
    if ( !qstrcmp( clname, "QCanvasRectangle" ) )
	return (QCanvasRectangle*)this;
    return QObject::qt_cast( clname );
}

bool FilmStrip::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: velocityMove(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FilmStrip::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FilmStrip::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool FilmStrip::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
