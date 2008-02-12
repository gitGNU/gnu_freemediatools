/****************************************************************************
** GLEffect meta object code from reading C++ file 'effect.h'
**
** Created: Wed Jan 23 19:12:27 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "effect.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLEffect::className() const
{
    return "GLEffect";
}

QMetaObject *GLEffect::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLEffect( "GLEffect", &GLEffect::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLEffect::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLEffect", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLEffect::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLEffect", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLEffect::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUMethod slot_0 = {"addCpuEffect", 0, 0 };
    static const QUMethod slot_1 = {"addMeshEffect", 0, 0 };
    static const QUMethod slot_2 = {"addGpuEffect", 0, 0 };
    static const QUMethod slot_3 = {"addCpuEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_4 = {"addMeshEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_5 = {"addGpuEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_6 = {"grabDesktop", 0, 0 };
    static const QUMethod slot_7 = {"setOption1", 0, 0 };
    static const QUMethod slot_8 = {"setOption2", 0, 0 };
    static const QUMethod slot_9 = {"setOption3", 0, 0 };
    static const QUMethod slot_10 = {"setOption4", 0, 0 };
    static const QUMethod slot_11 = {"changeVideoRender", 0, 0 };
    static const QUMethod slot_12 = {"changeRenderOnlyToSelectedEffect", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "addCpuEffect()", &slot_0, QMetaData::Public },
	{ "addMeshEffect()", &slot_1, QMetaData::Public },
	{ "addGpuEffect()", &slot_2, QMetaData::Public },
	{ "addCpuEffectFromTopMenu()", &slot_3, QMetaData::Public },
	{ "addMeshEffectFromTopMenu()", &slot_4, QMetaData::Public },
	{ "addGpuEffectFromTopMenu()", &slot_5, QMetaData::Public },
	{ "grabDesktop()", &slot_6, QMetaData::Public },
	{ "setOption1()", &slot_7, QMetaData::Public },
	{ "setOption2()", &slot_8, QMetaData::Public },
	{ "setOption3()", &slot_9, QMetaData::Public },
	{ "setOption4()", &slot_10, QMetaData::Public },
	{ "changeVideoRender()", &slot_11, QMetaData::Public },
	{ "changeRenderOnlyToSelectedEffect()", &slot_12, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLEffect", parentObject,
	slot_tbl, 13,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLEffect.setMetaObject( metaObj );
    return metaObj;
}

void* GLEffect::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLEffect" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLEffect::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool GLEffect::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addCpuEffect(); break;
    case 1: addMeshEffect(); break;
    case 2: addGpuEffect(); break;
    case 3: addCpuEffectFromTopMenu(); break;
    case 4: addMeshEffectFromTopMenu(); break;
    case 5: addGpuEffectFromTopMenu(); break;
    case 6: grabDesktop(); break;
    case 7: setOption1(); break;
    case 8: setOption2(); break;
    case 9: setOption3(); break;
    case 10: setOption4(); break;
    case 11: changeVideoRender(); break;
    case 12: changeRenderOnlyToSelectedEffect(); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLEffect::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    default:
	return GLWorld::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLEffect::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLEffect::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
