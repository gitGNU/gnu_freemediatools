/****************************************************************************
** KeyerColorWidget meta object code from reading C++ file 'keyercolorwidget.h'
**
** Created: Sat Jan 26 08:33:46 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "keyercolorwidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#include <qvariant.h>
const char *KeyerColorWidget::className() const
{
    return "KeyerColorWidget";
}

QMetaObject *KeyerColorWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KeyerColorWidget( "KeyerColorWidget", &KeyerColorWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KeyerColorWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KeyerColorWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KeyerColorWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KeyerColorWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KeyerColorWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"colorTriangleChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"buttonPressRGB", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setHValueSpin", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setSValueSpin", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setLValueSpin", 1, param_slot_4 };
    static const QUMethod slot_5 = {"buttonPressHSL", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_6 = {"setColorBarMin", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_7 = {"setColorBarMax", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"sliderValueChanged", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"softenSliderValueChanged", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_QVariant, "\x0a", QUParameter::Out }
    };
    static const QUMethod slot_10 = {"minColor", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_QVariant, "\x0a", QUParameter::Out }
    };
    static const QUMethod slot_11 = {"maxColor", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"spinBoxChanged", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::InOut }
    };
    static const QUMethod slot_13 = {"setColor", 1, param_slot_13 };
    static const QUMethod slot_14 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "colorTriangleChanged(const QColor&)", &slot_0, QMetaData::Public },
	{ "buttonPressRGB()", &slot_1, QMetaData::Public },
	{ "setHValueSpin(int)", &slot_2, QMetaData::Public },
	{ "setSValueSpin(int)", &slot_3, QMetaData::Public },
	{ "setLValueSpin(int)", &slot_4, QMetaData::Public },
	{ "buttonPressHSL()", &slot_5, QMetaData::Public },
	{ "setColorBarMin(const QColor&)", &slot_6, QMetaData::Public },
	{ "setColorBarMax(const QColor&)", &slot_7, QMetaData::Public },
	{ "sliderValueChanged(int)", &slot_8, QMetaData::Public },
	{ "softenSliderValueChanged(int)", &slot_9, QMetaData::Public },
	{ "minColor()", &slot_10, QMetaData::Public },
	{ "maxColor()", &slot_11, QMetaData::Public },
	{ "spinBoxChanged(int)", &slot_12, QMetaData::Public },
	{ "setColor(QColor&)", &slot_13, QMetaData::Public },
	{ "languageChange()", &slot_14, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"KeyerColorWidget", parentObject,
	slot_tbl, 15,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KeyerColorWidget.setMetaObject( metaObj );
    return metaObj;
}

void* KeyerColorWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KeyerColorWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool KeyerColorWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: colorTriangleChanged((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 1: buttonPressRGB(); break;
    case 2: setHValueSpin((int)static_QUType_int.get(_o+1)); break;
    case 3: setSValueSpin((int)static_QUType_int.get(_o+1)); break;
    case 4: setLValueSpin((int)static_QUType_int.get(_o+1)); break;
    case 5: buttonPressHSL(); break;
    case 6: setColorBarMin((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 7: setColorBarMax((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 8: sliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 9: softenSliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 10: static_QUType_QVariant.set(_o,QVariant(minColor())); break;
    case 11: static_QUType_QVariant.set(_o,QVariant(maxColor())); break;
    case 12: spinBoxChanged((int)static_QUType_int.get(_o+1)); break;
    case 13: setColor((QColor&)*((QColor*)static_QUType_ptr.get(_o+1))); break;
    case 14: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KeyerColorWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KeyerColorWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool KeyerColorWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
