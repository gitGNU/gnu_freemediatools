/****************************************************************************
** Apollon::giFTSocket meta object code from reading C++ file 'giftsocket.h'
**
** Created: Wed Jan 23 18:40:50 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "giftsocket.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Apollon::giFTSocket::className() const
{
    return "Apollon::giFTSocket";
}

QMetaObject *Apollon::giFTSocket::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Apollon__giFTSocket( "Apollon::giFTSocket", &Apollon::giFTSocket::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Apollon::giFTSocket::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Apollon::giFTSocket", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Apollon::giFTSocket::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Apollon::giFTSocket", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Apollon::giFTSocket::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSocket::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"Apollon::giFTSocket", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Apollon__giFTSocket.setMetaObject( metaObj );
    return metaObj;
}

void* Apollon::giFTSocket::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Apollon::giFTSocket" ) )
	return this;
    return QSocket::qt_cast( clname );
}

bool Apollon::giFTSocket::qt_invoke( int _id, QUObject* _o )
{
    return QSocket::qt_invoke(_id,_o);
}

bool Apollon::giFTSocket::qt_emit( int _id, QUObject* _o )
{
    return QSocket::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Apollon::giFTSocket::qt_property( int id, int f, QVariant* v)
{
    return QSocket::qt_property( id, f, v);
}

bool Apollon::giFTSocket::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
