/****************************************************************************
** JHClient meta object code from reading C++ file 'jhclient.h'
**
** Created: Wed Jan 23 19:09:26 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jhclient.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JHClient::className() const
{
    return "JHClient";
}

QMetaObject *JHClient::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JHClient( "JHClient", &JHClient::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JHClient::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JHClient", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JHClient::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JHClient", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JHClient::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QSocket::staticMetaObject();
    static const QUMethod slot_0 = {"readClient", 0, 0 };
    static const QUMethod slot_1 = {"closeConnection", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"bytesTransferred", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "readClient()", &slot_0, QMetaData::Private },
	{ "closeConnection()", &slot_1, QMetaData::Private },
	{ "bytesTransferred(int)", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JHClient", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JHClient.setMetaObject( metaObj );
    return metaObj;
}

void* JHClient::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JHClient" ) )
	return this;
    return QSocket::qt_cast( clname );
}

bool JHClient::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: readClient(); break;
    case 1: closeConnection(); break;
    case 2: bytesTransferred((int)static_QUType_int.get(_o+1)); break;
    default:
	return QSocket::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JHClient::qt_emit( int _id, QUObject* _o )
{
    return QSocket::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JHClient::qt_property( int id, int f, QVariant* v)
{
    return QSocket::qt_property( id, f, v);
}

bool JHClient::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
