/****************************************************************************
** SupergRangeControl meta object code from reading C++ file 'supergrangecontrol.h'
**
** Created: Sat Jan 26 08:34:03 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "supergrangecontrol.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *SupergRangeControl::className() const
{
    return "SupergRangeControl";
}

QMetaObject *SupergRangeControl::metaObj = 0;
static QMetaObjectCleanUp cleanUp_SupergRangeControl( "SupergRangeControl", &SupergRangeControl::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString SupergRangeControl::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SupergRangeControl", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString SupergRangeControl::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "SupergRangeControl", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* SupergRangeControl::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 1, param_slot_0 };
    static const QUMethod slot_1 = {"mouseHoldHelperSlot", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int)", &slot_0, QMetaData::Public },
	{ "mouseHoldHelperSlot()", &slot_1, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"valueChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_1 = {"inputPressEvent", 1, param_signal_1 };
    static const QUMethod signal_2 = {"inputPressEvent", 0, 0 };
    static const QUMethod signal_3 = {"inputReleaseEvent", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "valueChanged(int)", &signal_0, QMetaData::Public },
	{ "inputPressEvent(QMouseEvent*)", &signal_1, QMetaData::Public },
	{ "inputPressEvent()", &signal_2, QMetaData::Public },
	{ "inputReleaseEvent()", &signal_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"SupergRangeControl", parentObject,
	slot_tbl, 2,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_SupergRangeControl.setMetaObject( metaObj );
    return metaObj;
}

void* SupergRangeControl::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "SupergRangeControl" ) )
	return this;
    if ( !qstrcmp( clname, "QRangeControl" ) )
	return (QRangeControl*)this;
    return QWidget::qt_cast( clname );
}

// SIGNAL valueChanged
void SupergRangeControl::valueChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL inputPressEvent
void SupergRangeControl::inputPressEvent( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL inputPressEvent
void SupergRangeControl::inputPressEvent()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL inputReleaseEvent
void SupergRangeControl::inputReleaseEvent()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

bool SupergRangeControl::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1)); break;
    case 1: mouseHoldHelperSlot(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool SupergRangeControl::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: valueChanged((int)static_QUType_int.get(_o+1)); break;
    case 1: inputPressEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 2: inputPressEvent(); break;
    case 3: inputReleaseEvent(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool SupergRangeControl::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool SupergRangeControl::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
