/****************************************************************************
** EditDesktop meta object code from reading C++ file 'jahtimeline.h'
**
** Created: Sat Jan 26 19:17:51 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahtimeline.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EditDesktop::className() const
{
    return "EditDesktop";
}

QMetaObject *EditDesktop::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EditDesktop( "EditDesktop", &EditDesktop::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EditDesktop::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EditDesktop", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EditDesktop::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EditDesktop", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EditDesktop::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "numline", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"addRectangle", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "numline", &static_QUType_int, 0, QUParameter::In },
	{ "status", &static_QUType_bool, 0, QUParameter::In },
	{ "startframe", &static_QUType_int, 0, QUParameter::In },
	{ "endframe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"addEditLayer", 4, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "numline", &static_QUType_int, 0, QUParameter::In },
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_2 = {"addEditLayer", 2, param_slot_2 };
    static const QUMethod slot_3 = {"clear", 0, 0 };
    static const QUMethod slot_4 = {"init", 0, 0 };
    static const QUMethod slot_5 = {"addRectangle", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "width", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"drawScale", 1, param_slot_6 };
    static const QUMethod slot_7 = {"addLine", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "width", &static_QUType_int, 0, QUParameter::In },
	{ "numline", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"addLines", 2, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "addRectangle(int)", &slot_0, QMetaData::Public },
	{ "addEditLayer(int,bool,int,int)", &slot_1, QMetaData::Public },
	{ "addEditLayer(int,assetData)", &slot_2, QMetaData::Public },
	{ "clear()", &slot_3, QMetaData::Private },
	{ "init()", &slot_4, QMetaData::Private },
	{ "addRectangle()", &slot_5, QMetaData::Private },
	{ "drawScale(int)", &slot_6, QMetaData::Private },
	{ "addLine()", &slot_7, QMetaData::Private },
	{ "addLines(int,int)", &slot_8, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"EditDesktop", parentObject,
	slot_tbl, 9,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EditDesktop.setMetaObject( metaObj );
    return metaObj;
}

void* EditDesktop::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EditDesktop" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool EditDesktop::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addRectangle((int)static_QUType_int.get(_o+1)); break;
    case 1: addEditLayer((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4)); break;
    case 2: addEditLayer((int)static_QUType_int.get(_o+1),(assetData)(*((assetData*)static_QUType_ptr.get(_o+2)))); break;
    case 3: clear(); break;
    case 4: init(); break;
    case 5: addRectangle(); break;
    case 6: drawScale((int)static_QUType_int.get(_o+1)); break;
    case 7: addLine(); break;
    case 8: addLines((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool EditDesktop::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool EditDesktop::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool EditDesktop::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
