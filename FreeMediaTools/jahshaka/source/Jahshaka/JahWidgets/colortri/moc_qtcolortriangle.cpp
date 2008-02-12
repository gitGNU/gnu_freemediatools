/****************************************************************************
** QtColorTriangle meta object code from reading C++ file 'qtcolortriangle.h'
**
** Created: Sat Jan 26 08:33:47 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtcolortriangle.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtColorTriangle::className() const
{
    return "QtColorTriangle";
}

QMetaObject *QtColorTriangle::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtColorTriangle( "QtColorTriangle", &QtColorTriangle::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtColorTriangle::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtColorTriangle", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtColorTriangle::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtColorTriangle", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtColorTriangle::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setColor", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_1 = {"setColorSilently", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_2 = {"setColorSilentlyConstantHue", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "setColor(const QColor&)", &slot_0, QMetaData::Public },
	{ "setColorSilently(const QColor&)", &slot_1, QMetaData::Public },
	{ "setColorSilentlyConstantHue(const QColor&)", &slot_2, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod signal_0 = {"colorChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "colorChanged(const QColor&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtColorTriangle", parentObject,
	slot_tbl, 3,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtColorTriangle.setMetaObject( metaObj );
    return metaObj;
}

void* QtColorTriangle::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtColorTriangle" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL colorChanged
void QtColorTriangle::colorChanged( const QColor& t0 )
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

bool QtColorTriangle::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setColor((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 1: setColorSilently((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 2: setColorSilentlyConstantHue((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtColorTriangle::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: colorChanged((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtColorTriangle::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool QtColorTriangle::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
