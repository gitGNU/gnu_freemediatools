/****************************************************************************
** Collapsable meta object code from reading C++ file 'collapsable.h'
**
** Created: Sat Jan 26 08:34:03 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "collapsable.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Collapsable::className() const
{
    return "Collapsable";
}

QMetaObject *Collapsable::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Collapsable( "Collapsable", &Collapsable::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Collapsable::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Collapsable", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Collapsable::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Collapsable", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Collapsable::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"toggleState", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "toggleState()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Collapsable", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Collapsable.setMetaObject( metaObj );
    return metaObj;
}

void* Collapsable::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Collapsable" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool Collapsable::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: toggleState(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Collapsable::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Collapsable::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool Collapsable::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
