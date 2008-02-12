/****************************************************************************
** ColorDisplay meta object code from reading C++ file 'qtcolordisplay.h'
**
** Created: Sat Jan 26 08:33:48 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtcolordisplay.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColorDisplay::className() const
{
    return "ColorDisplay";
}

QMetaObject *ColorDisplay::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorDisplay( "ColorDisplay", &ColorDisplay::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorDisplay::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDisplay", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorDisplay::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDisplay", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorDisplay::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "c", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setColor", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setColor(const QColor&)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorDisplay", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorDisplay.setMetaObject( metaObj );
    return metaObj;
}

void* ColorDisplay::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorDisplay" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool ColorDisplay::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setColor((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColorDisplay::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ColorDisplay::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool ColorDisplay::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
