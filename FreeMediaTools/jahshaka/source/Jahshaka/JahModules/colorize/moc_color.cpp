/****************************************************************************
** GLColorize meta object code from reading C++ file 'color.h'
**
** Created: Wed Jan 23 19:12:14 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "color.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLColorize::className() const
{
    return "GLColorize";
}

QMetaObject *GLColorize::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLColorize( "GLColorize", &GLColorize::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLColorize::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLColorize", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLColorize::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLColorize", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLColorize::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUMethod slot_0 = {"grabDesktop", 0, 0 };
    static const QUMethod slot_1 = {"grabDesktopBaselayer", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"hueShiftHelperSlot", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"hueShiftSliderHelperSlot", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"hueStrengthHelperSlot", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"saturationHelperSlot", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"hueTintHelperSlot", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"hueWheelOffsetHelperSlot", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"hueWheelStrengthHelperSlot", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "offsetvalue", &static_QUType_int, 0, QUParameter::In },
	{ "strengthvalue", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"hueWheelChangeHelperSlot", 2, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_10 = {"setBaseRGB", 1, param_slot_10 };
    static const QUMethod slot_11 = {"gpuSelect", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"setRed", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"setGreen", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"setBlue", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"setBrightness", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"setHue", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"setSaturation", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"setValue", 1, param_slot_18 };
    static const QUMethod slot_19 = {"changeVideoRender", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "grabDesktop()", &slot_0, QMetaData::Public },
	{ "grabDesktopBaselayer()", &slot_1, QMetaData::Public },
	{ "hueShiftHelperSlot(int)", &slot_2, QMetaData::Public },
	{ "hueShiftSliderHelperSlot(int)", &slot_3, QMetaData::Public },
	{ "hueStrengthHelperSlot(int)", &slot_4, QMetaData::Public },
	{ "saturationHelperSlot(int)", &slot_5, QMetaData::Public },
	{ "hueTintHelperSlot(int)", &slot_6, QMetaData::Public },
	{ "hueWheelOffsetHelperSlot(int)", &slot_7, QMetaData::Public },
	{ "hueWheelStrengthHelperSlot(int)", &slot_8, QMetaData::Public },
	{ "hueWheelChangeHelperSlot(int,int)", &slot_9, QMetaData::Public },
	{ "setBaseRGB(const QColor&)", &slot_10, QMetaData::Public },
	{ "gpuSelect()", &slot_11, QMetaData::Public },
	{ "setRed(int)", &slot_12, QMetaData::Public },
	{ "setGreen(int)", &slot_13, QMetaData::Public },
	{ "setBlue(int)", &slot_14, QMetaData::Public },
	{ "setBrightness(int)", &slot_15, QMetaData::Public },
	{ "setHue(int)", &slot_16, QMetaData::Public },
	{ "setSaturation(int)", &slot_17, QMetaData::Public },
	{ "setValue(int)", &slot_18, QMetaData::Public },
	{ "changeVideoRender()", &slot_19, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QUMethod signal_1 = {"GrabBaseClip", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Public },
	{ "GrabBaseClip()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLColorize", parentObject,
	slot_tbl, 20,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLColorize.setMetaObject( metaObj );
    return metaObj;
}

void* GLColorize::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLColorize" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLColorize::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL GrabBaseClip
void GLColorize::GrabBaseClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool GLColorize::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: grabDesktop(); break;
    case 1: grabDesktopBaselayer(); break;
    case 2: hueShiftHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 3: hueShiftSliderHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 4: hueStrengthHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 5: saturationHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 6: hueTintHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 7: hueWheelOffsetHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 8: hueWheelStrengthHelperSlot((int)static_QUType_int.get(_o+1)); break;
    case 9: hueWheelChangeHelperSlot((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 10: setBaseRGB((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 11: gpuSelect(); break;
    case 12: setRed((int)static_QUType_int.get(_o+1)); break;
    case 13: setGreen((int)static_QUType_int.get(_o+1)); break;
    case 14: setBlue((int)static_QUType_int.get(_o+1)); break;
    case 15: setBrightness((int)static_QUType_int.get(_o+1)); break;
    case 16: setHue((int)static_QUType_int.get(_o+1)); break;
    case 17: setSaturation((int)static_QUType_int.get(_o+1)); break;
    case 18: setValue((int)static_QUType_int.get(_o+1)); break;
    case 19: changeVideoRender(); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLColorize::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    case 1: GrabBaseClip(); break;
    default:
	return GLWorld::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLColorize::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLColorize::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
