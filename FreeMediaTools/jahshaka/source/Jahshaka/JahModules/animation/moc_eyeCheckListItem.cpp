/****************************************************************************
** EyeCheckListItem meta object code from reading C++ file 'eyeCheckListItem.h'
**
** Created: Wed Jan 23 19:12:07 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "eyeCheckListItem.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EyeCheckListItem::className() const
{
    return "EyeCheckListItem";
}

QMetaObject *EyeCheckListItem::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EyeCheckListItem( "EyeCheckListItem", &EyeCheckListItem::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EyeCheckListItem::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EyeCheckListItem", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EyeCheckListItem::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EyeCheckListItem", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EyeCheckListItem::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "c", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"onPressed", 3, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "onPressed(QListViewItem*,const QPoint&,int)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"EyeCheckListItem", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EyeCheckListItem.setMetaObject( metaObj );
    return metaObj;
}

void* EyeCheckListItem::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EyeCheckListItem" ) )
	return this;
    if ( !qstrcmp( clname, "QCheckListItem" ) )
	return (QCheckListItem*)this;
    return QObject::qt_cast( clname );
}

bool EyeCheckListItem::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: onPressed((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool EyeCheckListItem::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EyeCheckListItem::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool EyeCheckListItem::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
