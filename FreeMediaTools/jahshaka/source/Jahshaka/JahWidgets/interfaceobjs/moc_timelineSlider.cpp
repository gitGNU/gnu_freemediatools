/****************************************************************************
** TimelineSlider meta object code from reading C++ file 'timelineSlider.h'
**
** Created: Sat Jan 26 08:34:04 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "timelineSlider.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TimelineSlider::className() const
{
    return "TimelineSlider";
}

QMetaObject *TimelineSlider::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TimelineSlider( "TimelineSlider", &TimelineSlider::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString TimelineSlider::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TimelineSlider", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TimelineSlider::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TimelineSlider", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TimelineSlider::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setValue", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setValue(int)", &slot_0, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"sliderPressed", 0, 0 };
    static const QUMethod signal_1 = {"sliderReleased", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"sliderMoved", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"valueChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"minChanged", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"maxChanged", 1, param_signal_5 };
    static const QMetaData signal_tbl[] = {
	{ "sliderPressed()", &signal_0, QMetaData::Public },
	{ "sliderReleased()", &signal_1, QMetaData::Public },
	{ "sliderMoved(int)", &signal_2, QMetaData::Public },
	{ "valueChanged(int)", &signal_3, QMetaData::Public },
	{ "minChanged(int)", &signal_4, QMetaData::Public },
	{ "maxChanged(int)", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TimelineSlider", parentObject,
	slot_tbl, 1,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TimelineSlider.setMetaObject( metaObj );
    return metaObj;
}

void* TimelineSlider::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TimelineSlider" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL sliderPressed
void TimelineSlider::sliderPressed()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL sliderReleased
void TimelineSlider::sliderReleased()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL sliderMoved
void TimelineSlider::sliderMoved( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL valueChanged
void TimelineSlider::valueChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL minChanged
void TimelineSlider::minChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL maxChanged
void TimelineSlider::maxChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

bool TimelineSlider::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setValue((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TimelineSlider::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sliderPressed(); break;
    case 1: sliderReleased(); break;
    case 2: sliderMoved((int)static_QUType_int.get(_o+1)); break;
    case 3: valueChanged((int)static_QUType_int.get(_o+1)); break;
    case 4: minChanged((int)static_QUType_int.get(_o+1)); break;
    case 5: maxChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool TimelineSlider::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool TimelineSlider::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
