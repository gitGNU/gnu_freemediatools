/****************************************************************************
** GLPaint meta object code from reading C++ file 'paint.h'
**
** Created: Tue Jan 29 20:10:42 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "paint.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLPaint::className() const
{
    return "GLPaint";
}

QMetaObject *GLPaint::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLPaint( "GLPaint", &GLPaint::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLPaint::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLPaint", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLPaint::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLPaint", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLPaint::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUMethod slot_0 = {"slotHistoryPrevious", 0, 0 };
    static const QUMethod slot_1 = {"slotHistoryNext", 0, 0 };
    static const QUMethod slot_2 = {"slotDelete", 0, 0 };
    static const QUMethod slot_3 = {"slotCopyOne", 0, 0 };
    static const QUMethod slot_4 = {"slotPaste", 0, 0 };
    static const QUMethod slot_5 = {"slotInvert", 0, 0 };
    static const QUMethod slot_6 = {"slotFade", 0, 0 };
    static const QUMethod slot_7 = {"slotIntensify", 0, 0 };
    static const QUMethod slot_8 = {"slotEmboss", 0, 0 };
    static const QUMethod slot_9 = {"slotBlur", 0, 0 };
    static const QUMethod slot_10 = {"slotSharpen", 0, 0 };
    static const QUMethod slot_11 = {"slotEdgeDetectX", 0, 0 };
    static const QUMethod slot_12 = {"slotEdgeDetectY", 0, 0 };
    static const QUMethod slot_13 = {"slotSetCompositeType", 0, 0 };
    static const QUMethod slot_14 = {"slotComposite", 0, 0 };
    static const QUMethod slot_15 = {"slotSobel", 0, 0 };
    static const QUMethod slot_16 = {"slotLaplacian", 0, 0 };
    static const QUMethod slot_17 = {"slotLaplacian2", 0, 0 };
    static const QUMethod slot_18 = {"slotLapOfGauss", 0, 0 };
    static const QUMethod slot_19 = {"slotClear", 0, 0 };
    static const QUMethod slot_20 = {"slotRefresh", 0, 0 };
    static const QUMethod slot_21 = {"slotTurnOffAdvanceFrame", 0, 0 };
    static const QUMethod slot_22 = {"slotTurnOffApplyEffectsToAllFrames", 0, 0 };
    static const QUMethod slot_23 = {"slotPen", 0, 0 };
    static const QUMethod slot_24 = {"slotLine", 0, 0 };
    static const QUMethod slot_25 = {"slotRectangle", 0, 0 };
    static const QUMethod slot_26 = {"slotRectangleFilled", 0, 0 };
    static const QUMethod slot_27 = {"slotCircle", 0, 0 };
    static const QUMethod slot_28 = {"slotCircleFilled", 0, 0 };
    static const QUMethod slot_29 = {"slotTriangle", 0, 0 };
    static const QUMethod slot_30 = {"slotTriangleFilled", 0, 0 };
    static const QUMethod slot_31 = {"slotPaintBucket", 0, 0 };
    static const QUMethod slot_32 = {"slotPaintBucketFillAll", 0, 0 };
    static const QUMethod slot_33 = {"slotForegroundColorPicker", 0, 0 };
    static const QUMethod slot_34 = {"slotBackgroundColorPicker", 0, 0 };
    static const QUMethod slot_35 = {"slotEraser", 0, 0 };
    static const QUMethod slot_36 = {"slotPenColor", 0, 0 };
    static const QUMethod slot_37 = {"slotFillColor", 0, 0 };
    static const QUMethod slot_38 = {"slotBackgroundColor", 0, 0 };
    static const QUParameter param_slot_39[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_39 = {"slotBrushSize", 1, param_slot_39 };
    static const QUParameter param_slot_40[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_40 = {"slotGradientDegree", 1, param_slot_40 };
    static const QUParameter param_slot_41[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_41 = {"slotFillBucketRange", 1, param_slot_41 };
    static const QUParameter param_slot_42[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_42 = {"slotSetPenAlpha", 1, param_slot_42 };
    static const QUParameter param_slot_43[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_43 = {"slotSetFillAlpha", 1, param_slot_43 };
    static const QUParameter param_slot_44[] = {
	{ "value", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_44 = {"slotSetBackgroundAlpha", 1, param_slot_44 };
    static const QUMethod slot_45 = {"startanimation", 0, 0 };
    static const QUMethod slot_46 = {"addLayer", 0, 0 };
    static const QUParameter param_slot_47[] = {
	{ "clip", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_47 = {"addClip", 1, param_slot_47 };
    static const QUParameter param_slot_48[] = {
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_48 = {"clicked", 1, param_slot_48 };
    static const QUMethod slot_49 = {"changeParticle", 0, 0 };
    static const QUMethod slot_50 = {"changeFontDraw", 0, 0 };
    static const QUMethod slot_51 = {"changeObjectDraw", 0, 0 };
    static const QUMethod slot_52 = {"keyCliplayer", 0, 0 };
    static const QUMethod slot_53 = {"keyClipkey", 0, 0 };
    static const QUMethod slot_54 = {"updateSliders", 0, 0 };
    static const QUMethod slot_55 = {"resizePaintGL", 0, 0 };
    static const QUMethod slot_56 = {"resetAll", 0, 0 };
    static const QUMethod slot_57 = {"SceneLoad", 0, 0 };
    static const QUMethod slot_58 = {"sceneSave", 0, 0 };
    static const QUMethod slot_59 = {"clear", 0, 0 };
    static const QUParameter param_slot_60[] = {
	{ "increment_history", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_60 = {"clear", 1, param_slot_60 };
    static const QMetaData slot_tbl[] = {
	{ "slotHistoryPrevious()", &slot_0, QMetaData::Protected },
	{ "slotHistoryNext()", &slot_1, QMetaData::Protected },
	{ "slotDelete()", &slot_2, QMetaData::Protected },
	{ "slotCopyOne()", &slot_3, QMetaData::Protected },
	{ "slotPaste()", &slot_4, QMetaData::Protected },
	{ "slotInvert()", &slot_5, QMetaData::Protected },
	{ "slotFade()", &slot_6, QMetaData::Protected },
	{ "slotIntensify()", &slot_7, QMetaData::Protected },
	{ "slotEmboss()", &slot_8, QMetaData::Protected },
	{ "slotBlur()", &slot_9, QMetaData::Protected },
	{ "slotSharpen()", &slot_10, QMetaData::Protected },
	{ "slotEdgeDetectX()", &slot_11, QMetaData::Protected },
	{ "slotEdgeDetectY()", &slot_12, QMetaData::Protected },
	{ "slotSetCompositeType()", &slot_13, QMetaData::Protected },
	{ "slotComposite()", &slot_14, QMetaData::Protected },
	{ "slotSobel()", &slot_15, QMetaData::Protected },
	{ "slotLaplacian()", &slot_16, QMetaData::Protected },
	{ "slotLaplacian2()", &slot_17, QMetaData::Protected },
	{ "slotLapOfGauss()", &slot_18, QMetaData::Protected },
	{ "slotClear()", &slot_19, QMetaData::Protected },
	{ "slotRefresh()", &slot_20, QMetaData::Protected },
	{ "slotTurnOffAdvanceFrame()", &slot_21, QMetaData::Protected },
	{ "slotTurnOffApplyEffectsToAllFrames()", &slot_22, QMetaData::Protected },
	{ "slotPen()", &slot_23, QMetaData::Protected },
	{ "slotLine()", &slot_24, QMetaData::Protected },
	{ "slotRectangle()", &slot_25, QMetaData::Protected },
	{ "slotRectangleFilled()", &slot_26, QMetaData::Protected },
	{ "slotCircle()", &slot_27, QMetaData::Protected },
	{ "slotCircleFilled()", &slot_28, QMetaData::Protected },
	{ "slotTriangle()", &slot_29, QMetaData::Protected },
	{ "slotTriangleFilled()", &slot_30, QMetaData::Protected },
	{ "slotPaintBucket()", &slot_31, QMetaData::Protected },
	{ "slotPaintBucketFillAll()", &slot_32, QMetaData::Protected },
	{ "slotForegroundColorPicker()", &slot_33, QMetaData::Protected },
	{ "slotBackgroundColorPicker()", &slot_34, QMetaData::Protected },
	{ "slotEraser()", &slot_35, QMetaData::Protected },
	{ "slotPenColor()", &slot_36, QMetaData::Protected },
	{ "slotFillColor()", &slot_37, QMetaData::Protected },
	{ "slotBackgroundColor()", &slot_38, QMetaData::Protected },
	{ "slotBrushSize(int)", &slot_39, QMetaData::Protected },
	{ "slotGradientDegree(int)", &slot_40, QMetaData::Protected },
	{ "slotFillBucketRange(int)", &slot_41, QMetaData::Protected },
	{ "slotSetPenAlpha(int)", &slot_42, QMetaData::Protected },
	{ "slotSetFillAlpha(int)", &slot_43, QMetaData::Protected },
	{ "slotSetBackgroundAlpha(int)", &slot_44, QMetaData::Protected },
	{ "startanimation()", &slot_45, QMetaData::Public },
	{ "addLayer()", &slot_46, QMetaData::Public },
	{ "addClip(assetData)", &slot_47, QMetaData::Public },
	{ "clicked(QListViewItem*)", &slot_48, QMetaData::Public },
	{ "changeParticle()", &slot_49, QMetaData::Public },
	{ "changeFontDraw()", &slot_50, QMetaData::Public },
	{ "changeObjectDraw()", &slot_51, QMetaData::Public },
	{ "keyCliplayer()", &slot_52, QMetaData::Public },
	{ "keyClipkey()", &slot_53, QMetaData::Public },
	{ "updateSliders()", &slot_54, QMetaData::Public },
	{ "resizePaintGL()", &slot_55, QMetaData::Public },
	{ "resetAll()", &slot_56, QMetaData::Public },
	{ "SceneLoad()", &slot_57, QMetaData::Public },
	{ "sceneSave()", &slot_58, QMetaData::Public },
	{ "clear()", &slot_59, QMetaData::Public },
	{ "clear(bool)", &slot_60, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QUMethod signal_1 = {"GrabClipKey", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Protected },
	{ "GrabClipKey()", &signal_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLPaint", parentObject,
	slot_tbl, 61,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLPaint.setMetaObject( metaObj );
    return metaObj;
}

void* GLPaint::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLPaint" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLPaint::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL GrabClipKey
void GLPaint::GrabClipKey()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool GLPaint::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: slotHistoryPrevious(); break;
    case 1: slotHistoryNext(); break;
    case 2: slotDelete(); break;
    case 3: slotCopyOne(); break;
    case 4: slotPaste(); break;
    case 5: slotInvert(); break;
    case 6: slotFade(); break;
    case 7: slotIntensify(); break;
    case 8: slotEmboss(); break;
    case 9: slotBlur(); break;
    case 10: slotSharpen(); break;
    case 11: slotEdgeDetectX(); break;
    case 12: slotEdgeDetectY(); break;
    case 13: slotSetCompositeType(); break;
    case 14: slotComposite(); break;
    case 15: slotSobel(); break;
    case 16: slotLaplacian(); break;
    case 17: slotLaplacian2(); break;
    case 18: slotLapOfGauss(); break;
    case 19: slotClear(); break;
    case 20: slotRefresh(); break;
    case 21: slotTurnOffAdvanceFrame(); break;
    case 22: slotTurnOffApplyEffectsToAllFrames(); break;
    case 23: slotPen(); break;
    case 24: slotLine(); break;
    case 25: slotRectangle(); break;
    case 26: slotRectangleFilled(); break;
    case 27: slotCircle(); break;
    case 28: slotCircleFilled(); break;
    case 29: slotTriangle(); break;
    case 30: slotTriangleFilled(); break;
    case 31: slotPaintBucket(); break;
    case 32: slotPaintBucketFillAll(); break;
    case 33: slotForegroundColorPicker(); break;
    case 34: slotBackgroundColorPicker(); break;
    case 35: slotEraser(); break;
    case 36: slotPenColor(); break;
    case 37: slotFillColor(); break;
    case 38: slotBackgroundColor(); break;
    case 39: slotBrushSize((int)static_QUType_int.get(_o+1)); break;
    case 40: slotGradientDegree((int)static_QUType_int.get(_o+1)); break;
    case 41: slotFillBucketRange((int)static_QUType_int.get(_o+1)); break;
    case 42: slotSetPenAlpha((int)static_QUType_int.get(_o+1)); break;
    case 43: slotSetFillAlpha((int)static_QUType_int.get(_o+1)); break;
    case 44: slotSetBackgroundAlpha((int)static_QUType_int.get(_o+1)); break;
    case 45: startanimation(); break;
    case 46: addLayer(); break;
    case 47: addClip((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 48: clicked((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 49: changeParticle(); break;
    case 50: changeFontDraw(); break;
    case 51: changeObjectDraw(); break;
    case 52: keyCliplayer(); break;
    case 53: keyClipkey(); break;
    case 54: updateSliders(); break;
    case 55: resizePaintGL(); break;
    case 56: resetAll(); break;
    case 57: SceneLoad(); break;
    case 58: sceneSave(); break;
    case 59: clear(); break;
    case 60: clear((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLPaint::qt_emit( int _id, QUObject* _o )
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

bool GLPaint::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLPaint::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
