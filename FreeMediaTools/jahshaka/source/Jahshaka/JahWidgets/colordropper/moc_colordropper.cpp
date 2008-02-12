/****************************************************************************
** ColorDropper meta object code from reading C++ file 'colordropper.h'
**
** Created: Sat Jan 26 08:34:37 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "colordropper.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColorDropper::className() const
{
    return "ColorDropper";
}

QMetaObject *ColorDropper::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorDropper( "ColorDropper", &ColorDropper::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorDropper::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDropper", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorDropper::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorDropper", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorDropper::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_0 = {"colorPicked", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "colorPicked(const QColor&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorDropper", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorDropper.setMetaObject( metaObj );
    return metaObj;
}

void* ColorDropper::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorDropper" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL colorPicked
void ColorDropper::colorPicked( const QColor& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool ColorDropper::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool ColorDropper::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: colorPicked((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ColorDropper::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ColorDropper::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
