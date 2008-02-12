/****************************************************************************
** JahNodes meta object code from reading C++ file 'jahnodes.h'
**
** Created: Sat Jan 26 08:34:28 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahnodes.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahNodes::className() const
{
    return "JahNodes";
}

QMetaObject *JahNodes::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahNodes( "JahNodes", &JahNodes::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahNodes::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahNodes", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahNodes::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahNodes", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahNodes::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "name", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"addNodes", 2, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"delNodes", 1, param_slot_1 };
    static const QUMethod slot_2 = {"clear", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"selectNode", 1, param_slot_3 };
    static const QUMethod slot_4 = {"rebuildNodes", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "name", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"buildNodes", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "x1", &static_QUType_int, 0, QUParameter::In },
	{ "y1", &static_QUType_int, 0, QUParameter::In },
	{ "x2", &static_QUType_int, 0, QUParameter::In },
	{ "y2", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"drawLine", 4, param_slot_6 };
    static const QUMethod slot_7 = {"buildInterface", 0, 0 };
    static const QUMethod slot_8 = {"enlarge", 0, 0 };
    static const QUMethod slot_9 = {"shrink", 0, 0 };
    static const QUMethod slot_10 = {"zoomIn", 0, 0 };
    static const QUMethod slot_11 = {"zoomOut", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "addNodes(int,QString)", &slot_0, QMetaData::Public },
	{ "delNodes(int)", &slot_1, QMetaData::Public },
	{ "clear()", &slot_2, QMetaData::Public },
	{ "selectNode(int)", &slot_3, QMetaData::Public },
	{ "rebuildNodes()", &slot_4, QMetaData::Public },
	{ "buildNodes(QString)", &slot_5, QMetaData::Private },
	{ "drawLine(int,int,int,int)", &slot_6, QMetaData::Private },
	{ "buildInterface()", &slot_7, QMetaData::Private },
	{ "enlarge()", &slot_8, QMetaData::Private },
	{ "shrink()", &slot_9, QMetaData::Private },
	{ "zoomIn()", &slot_10, QMetaData::Private },
	{ "zoomOut()", &slot_11, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahNodes", parentObject,
	slot_tbl, 12,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahNodes.setMetaObject( metaObj );
    return metaObj;
}

void* JahNodes::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahNodes" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool JahNodes::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addNodes((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 1: delNodes((int)static_QUType_int.get(_o+1)); break;
    case 2: clear(); break;
    case 3: selectNode((int)static_QUType_int.get(_o+1)); break;
    case 4: rebuildNodes(); break;
    case 5: buildNodes((QString)static_QUType_QString.get(_o+1)); break;
    case 6: drawLine((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4)); break;
    case 7: buildInterface(); break;
    case 8: enlarge(); break;
    case 9: shrink(); break;
    case 10: zoomIn(); break;
    case 11: zoomOut(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahNodes::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahNodes::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahNodes::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
