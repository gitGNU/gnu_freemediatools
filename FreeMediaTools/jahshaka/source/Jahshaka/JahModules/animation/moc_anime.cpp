/****************************************************************************
** GLAnime meta object code from reading C++ file 'anime.h'
**
** Created: Wed Jan 23 19:12:06 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "anime.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLAnime::className() const
{
    return "GLAnime";
}

QMetaObject *GLAnime::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLAnime( "GLAnime", &GLAnime::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLAnime::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLAnime", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLAnime::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLAnime", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLAnime::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUMethod slot_0 = {"nameLayerFromTopMenu", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"addLayerFromTopMenu", 1, param_slot_1 };
    static const QUMethod slot_2 = {"addLayer", 0, 0 };
    static const QUMethod slot_3 = {"addCpuEffect", 0, 0 };
    static const QUMethod slot_4 = {"addMeshEffect", 0, 0 };
    static const QUMethod slot_5 = {"addGpuEffect", 0, 0 };
    static const QUMethod slot_6 = {"addCpuEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_7 = {"addMeshEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_8 = {"addGpuEffectFromTopMenu", 0, 0 };
    static const QUMethod slot_9 = {"setOption1", 0, 0 };
    static const QUMethod slot_10 = {"setOption2", 0, 0 };
    static const QUMethod slot_11 = {"setOption3", 0, 0 };
    static const QUMethod slot_12 = {"setOption4", 0, 0 };
    static const QUMethod slot_13 = {"setOption5", 0, 0 };
    static const QUMethod slot_14 = {"slotSetLayerSelectedVisible", 0, 0 };
    static const QUMethod slot_15 = {"changeParticle", 0, 0 };
    static const QUMethod slot_16 = {"changeParticleDraw", 0, 0 };
    static const QUMethod slot_17 = {"changeFontDraw", 0, 0 };
    static const QUMethod slot_18 = {"changeObjectDraw", 0, 0 };
    static const QUMethod slot_19 = {"keyCliplayer", 0, 0 };
    static const QUMethod slot_20 = {"keyClipkey", 0, 0 };
    static const QUMethod slot_21 = {"grabDesktop", 0, 0 };
    static const QUMethod slot_22 = {"grabDesktopKey", 0, 0 };
    static const QUParameter param_slot_23[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_23 = {"layerClicked", 1, param_slot_23 };
    static const QUMethod slot_24 = {"locateMissingMedia", 0, 0 };
    static const QUParameter param_slot_25[] = {
	{ 0, &static_QUType_ptr, "QCheckBox", QUParameter::Out }
    };
    static const QUMethod slot_25 = {"getKeySelect", 1, param_slot_25 };
    static const QUMethod slot_26 = {"slotChooseLightColorAmbient", 0, 0 };
    static const QUMethod slot_27 = {"slotChooseLightColorDiffuse", 0, 0 };
    static const QUMethod slot_28 = {"slotChooseLightColorSpecular", 0, 0 };
    static const QUParameter param_slot_29[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_29 = {"slotLightColorAmbient", 1, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_30 = {"slotLightColorDiffuse", 1, param_slot_30 };
    static const QUParameter param_slot_31[] = {
	{ "color", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_31 = {"slotLightColorSpecular", 1, param_slot_31 };
    static const QUMethod slot_32 = {"textEdChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "nameLayerFromTopMenu()", &slot_0, QMetaData::Public },
	{ "addLayerFromTopMenu(int)", &slot_1, QMetaData::Public },
	{ "addLayer()", &slot_2, QMetaData::Public },
	{ "addCpuEffect()", &slot_3, QMetaData::Public },
	{ "addMeshEffect()", &slot_4, QMetaData::Public },
	{ "addGpuEffect()", &slot_5, QMetaData::Public },
	{ "addCpuEffectFromTopMenu()", &slot_6, QMetaData::Public },
	{ "addMeshEffectFromTopMenu()", &slot_7, QMetaData::Public },
	{ "addGpuEffectFromTopMenu()", &slot_8, QMetaData::Public },
	{ "setOption1()", &slot_9, QMetaData::Public },
	{ "setOption2()", &slot_10, QMetaData::Public },
	{ "setOption3()", &slot_11, QMetaData::Public },
	{ "setOption4()", &slot_12, QMetaData::Public },
	{ "setOption5()", &slot_13, QMetaData::Public },
	{ "slotSetLayerSelectedVisible()", &slot_14, QMetaData::Public },
	{ "changeParticle()", &slot_15, QMetaData::Public },
	{ "changeParticleDraw()", &slot_16, QMetaData::Public },
	{ "changeFontDraw()", &slot_17, QMetaData::Public },
	{ "changeObjectDraw()", &slot_18, QMetaData::Public },
	{ "keyCliplayer()", &slot_19, QMetaData::Public },
	{ "keyClipkey()", &slot_20, QMetaData::Public },
	{ "grabDesktop()", &slot_21, QMetaData::Public },
	{ "grabDesktopKey()", &slot_22, QMetaData::Public },
	{ "layerClicked(QListViewItem*)", &slot_23, QMetaData::Public },
	{ "locateMissingMedia()", &slot_24, QMetaData::Public },
	{ "getKeySelect()", &slot_25, QMetaData::Public },
	{ "slotChooseLightColorAmbient()", &slot_26, QMetaData::Public },
	{ "slotChooseLightColorDiffuse()", &slot_27, QMetaData::Public },
	{ "slotChooseLightColorSpecular()", &slot_28, QMetaData::Public },
	{ "slotLightColorAmbient(QColor)", &slot_29, QMetaData::Public },
	{ "slotLightColorDiffuse(QColor)", &slot_30, QMetaData::Public },
	{ "slotLightColorSpecular(QColor)", &slot_31, QMetaData::Public },
	{ "textEdChanged()", &slot_32, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QUMethod signal_1 = {"GrabClipKey", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Public },
	{ "GrabClipKey()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLAnime", parentObject,
	slot_tbl, 33,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLAnime.setMetaObject( metaObj );
    return metaObj;
}

void* GLAnime::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLAnime" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLAnime::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL GrabClipKey
void GLAnime::GrabClipKey()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool GLAnime::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: nameLayerFromTopMenu(); break;
    case 1: addLayerFromTopMenu((int)static_QUType_int.get(_o+1)); break;
    case 2: addLayer(); break;
    case 3: addCpuEffect(); break;
    case 4: addMeshEffect(); break;
    case 5: addGpuEffect(); break;
    case 6: addCpuEffectFromTopMenu(); break;
    case 7: addMeshEffectFromTopMenu(); break;
    case 8: addGpuEffectFromTopMenu(); break;
    case 9: setOption1(); break;
    case 10: setOption2(); break;
    case 11: setOption3(); break;
    case 12: setOption4(); break;
    case 13: setOption5(); break;
    case 14: slotSetLayerSelectedVisible(); break;
    case 15: changeParticle(); break;
    case 16: changeParticleDraw(); break;
    case 17: changeFontDraw(); break;
    case 18: changeObjectDraw(); break;
    case 19: keyCliplayer(); break;
    case 20: keyClipkey(); break;
    case 21: grabDesktop(); break;
    case 22: grabDesktopKey(); break;
    case 23: layerClicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 24: locateMissingMedia(); break;
    case 25: static_QUType_ptr.set(_o,getKeySelect()); break;
    case 26: slotChooseLightColorAmbient(); break;
    case 27: slotChooseLightColorDiffuse(); break;
    case 28: slotChooseLightColorSpecular(); break;
    case 29: slotLightColorAmbient((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 30: slotLightColorDiffuse((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 31: slotLightColorSpecular((QColor)(*((QColor*)static_QUType_ptr.get(_o+1)))); break;
    case 32: textEdChanged(); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLAnime::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    case 1: GrabClipKey(); break;
    default:
	return GLWorld::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLAnime::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLAnime::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
