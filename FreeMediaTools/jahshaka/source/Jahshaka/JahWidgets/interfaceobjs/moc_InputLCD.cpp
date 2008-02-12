/****************************************************************************
** InputLCD meta object code from reading C++ file 'InputLCD.h'
**
** Created: Sat Jan 26 08:34:00 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "InputLCD.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *InputLCD::className() const
{
    return "InputLCD";
}

QMetaObject *InputLCD::metaObj = 0;
static QMetaObjectCleanUp cleanUp_InputLCD( "InputLCD", &InputLCD::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString InputLCD::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "InputLCD", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString InputLCD::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "InputLCD", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* InputLCD::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setMinInt", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setMaxInt", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "min", &static_QUType_int, 0, QUParameter::In },
	{ "max", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setMinMaxInt", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "status", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setActive", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int)", &slot_0, QMetaData::Public },
	{ "setMinInt(int)", &slot_1, QMetaData::Public },
	{ "setMaxInt(int)", &slot_2, QMetaData::Public },
	{ "setMinMaxInt(int,int)", &slot_3, QMetaData::Public },
	{ "setActive(bool)", &slot_4, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
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
	"InputLCD", parentObject,
	slot_tbl, 5,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_InputLCD.setMetaObject( metaObj );
    return metaObj;
}

void* InputLCD::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "InputLCD" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL valueChanged
void InputLCD::valueChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL inputPressEvent
void InputLCD::inputPressEvent( QMouseEvent* t0 )
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
void InputLCD::inputPressEvent()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL inputReleaseEvent
void InputLCD::inputReleaseEvent()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

bool InputLCD::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1)); break;
    case 1: setMinInt((int)static_QUType_int.get(_o+1)); break;
    case 2: setMaxInt((int)static_QUType_int.get(_o+1)); break;
    case 3: setMinMaxInt((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 4: setActive((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool InputLCD::qt_emit( int _id, QUObject* _o )
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

bool InputLCD::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool InputLCD::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
