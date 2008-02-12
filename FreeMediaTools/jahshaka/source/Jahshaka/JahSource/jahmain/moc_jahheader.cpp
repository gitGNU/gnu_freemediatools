/****************************************************************************
** JahHeader meta object code from reading C++ file 'jahheader.h'
**
** Created: Wed Jan 23 19:13:31 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahheader.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahHeader::className() const
{
    return "JahHeader";
}

QMetaObject *JahHeader::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahHeader( "JahHeader", &JahHeader::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahHeader::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahHeader", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahHeader::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahHeader", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahHeader::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"onMenuContentsChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "onMenuContentsChanged()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahHeader", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahHeader.setMetaObject( metaObj );
    return metaObj;
}

void* JahHeader::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahHeader" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool JahHeader::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: onMenuContentsChanged(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahHeader::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahHeader::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahHeader::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
