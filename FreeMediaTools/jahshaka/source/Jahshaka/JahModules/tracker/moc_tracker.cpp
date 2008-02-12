/****************************************************************************
** GLTracker meta object code from reading C++ file 'tracker.h'
**
** Created: Wed Jan 23 19:13:13 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "tracker.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLTracker::className() const
{
    return "GLTracker";
}

QMetaObject *GLTracker::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLTracker( "GLTracker", &GLTracker::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLTracker::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLTracker", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLTracker::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLTracker", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLTracker::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "clip", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_0 = {"addClip", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_1 = {"layerClicked", 1, param_slot_1 };
    static const QUMethod slot_2 = {"setStabilize", 0, 0 };
    static const QUMethod slot_3 = {"setGpuSelect", 0, 0 };
    static const QUMethod slot_4 = {"setOption1", 0, 0 };
    static const QUMethod slot_5 = {"setOption2", 0, 0 };
    static const QUMethod slot_6 = {"setOption3", 0, 0 };
    static const QUMethod slot_7 = {"setOption4", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"captureTrackerPoint", 1, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "addClip(assetData)", &slot_0, QMetaData::Public },
	{ "layerClicked(QListViewItem*)", &slot_1, QMetaData::Public },
	{ "setStabilize()", &slot_2, QMetaData::Public },
	{ "setGpuSelect()", &slot_3, QMetaData::Public },
	{ "setOption1()", &slot_4, QMetaData::Public },
	{ "setOption2()", &slot_5, QMetaData::Public },
	{ "setOption3()", &slot_6, QMetaData::Public },
	{ "setOption4()", &slot_7, QMetaData::Public },
	{ "captureTrackerPoint(int)", &slot_8, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLTracker", parentObject,
	slot_tbl, 9,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLTracker.setMetaObject( metaObj );
    return metaObj;
}

void* GLTracker::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLTracker" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLTracker::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool GLTracker::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addClip((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 1: layerClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 2: setStabilize(); break;
    case 3: setGpuSelect(); break;
    case 4: setOption1(); break;
    case 5: setOption2(); break;
    case 6: setOption3(); break;
    case 7: setOption4(); break;
    case 8: captureTrackerPoint((int)static_QUType_int.get(_o+1)); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLTracker::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    default:
	return GLWorld::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLTracker::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLTracker::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
