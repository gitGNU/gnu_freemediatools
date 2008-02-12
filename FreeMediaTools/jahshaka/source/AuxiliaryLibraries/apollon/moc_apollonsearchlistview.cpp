/****************************************************************************
** ApollonSearchListView meta object code from reading C++ file 'apollonsearchlistview.h'
**
** Created: Wed Jan 23 18:40:44 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "apollonsearchlistview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ApollonSearchListView::className() const
{
    return "ApollonSearchListView";
}

QMetaObject *ApollonSearchListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApollonSearchListView( "ApollonSearchListView", &ApollonSearchListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ApollonSearchListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonSearchListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApollonSearchListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonSearchListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApollonSearchListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ApollonListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "column", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"toggleColumnVisible", 1, param_slot_0 };
    static const QUMethod slot_1 = {"slotShowTip", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "toggleColumnVisible(int)", &slot_0, QMetaData::Private },
	{ "slotShowTip()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApollonSearchListView", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApollonSearchListView.setMetaObject( metaObj );
    return metaObj;
}

void* ApollonSearchListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApollonSearchListView" ) )
	return this;
    if ( !qstrcmp( clname, "QToolTip" ) )
	return (QToolTip*)this;
    return ApollonListView::qt_cast( clname );
}

bool ApollonSearchListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: toggleColumnVisible((int)static_QUType_int.get(_o+1)); break;
    case 1: slotShowTip(); break;
    default:
	return ApollonListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ApollonSearchListView::qt_emit( int _id, QUObject* _o )
{
    return ApollonListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ApollonSearchListView::qt_property( int id, int f, QVariant* v)
{
    return ApollonListView::qt_property( id, f, v);
}

bool ApollonSearchListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
