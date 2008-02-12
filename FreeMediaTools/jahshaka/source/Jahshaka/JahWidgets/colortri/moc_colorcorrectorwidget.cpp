/****************************************************************************
** ColorCorrectorWidget meta object code from reading C++ file 'colorcorrectorwidget.h'
**
** Created: Sat Jan 26 08:33:49 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "colorcorrectorwidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ColorCorrectorWidget::className() const
{
    return "ColorCorrectorWidget";
}

QMetaObject *ColorCorrectorWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ColorCorrectorWidget( "ColorCorrectorWidget", &ColorCorrectorWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ColorCorrectorWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorCorrectorWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ColorCorrectorWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ColorCorrectorWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ColorCorrectorWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_0 = {"colorTriangleChanged", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"setColor", 1, param_slot_1 };
    static const QUMethod slot_2 = {"resetSliders", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_3 = {"setRGB", 1, param_slot_3 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "colorTriangleChanged(const QColor&)", &slot_0, QMetaData::Public },
	{ "setColor(QColor&)", &slot_1, QMetaData::Public },
	{ "resetSliders()", &slot_2, QMetaData::Public },
	{ "setRGB(const QColor&)", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ColorCorrectorWidget", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ColorCorrectorWidget.setMetaObject( metaObj );
    return metaObj;
}

void* ColorCorrectorWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ColorCorrectorWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool ColorCorrectorWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: colorTriangleChanged((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 1: setColor((QColor&)*((QColor*)static_QUType_ptr.get(_o+1))); break;
    case 2: resetSliders(); break;
    case 3: setRGB((const QColor&)*((const QColor*)static_QUType_ptr.get(_o+1))); break;
    case 4: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ColorCorrectorWidget::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ColorCorrectorWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ColorCorrectorWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
