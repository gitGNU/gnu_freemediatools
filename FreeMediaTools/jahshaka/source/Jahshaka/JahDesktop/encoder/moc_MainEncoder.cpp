/****************************************************************************
** MainEncoder meta object code from reading C++ file 'MainEncoder.h'
**
** Created: Mon Jan 28 21:49:30 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MainEncoder.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainEncoder::className() const
{
    return "MainEncoder";
}

QMetaObject *MainEncoder::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainEncoder( "MainEncoder", &MainEncoder::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainEncoder::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainEncoder", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainEncoder::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainEncoder", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainEncoder::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "position", &static_QUType_int, 0, QUParameter::In },
	{ "image", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_0 = {"showImage", 2, param_slot_0 };
    static const QUMethod slot_1 = {"slotGenerateRenderFormatList", 0, 0 };
    static const QUMethod slot_2 = {"slotGenerateRenderProfileList", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"slotSelectRenderFormat", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"slotSelectRenderProfile", 1, param_slot_4 };
    static const QUMethod slot_5 = {"slotStartRender", 0, 0 };
    static const QUMethod slot_6 = {"sliderPressed", 0, 0 };
    static const QUMethod slot_7 = {"sliderReleased", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "sf", &static_QUType_int, 0, QUParameter::In },
	{ "aborted", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"LoadSequenceCompleted", 2, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "fr", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"sliderValueChanged", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"SetFrameNumber", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_ptr, "FPS", QUParameter::In }
    };
    static const QUMethod slot_11 = {"SetFPS", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "fpsval", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"switchFps", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"SetStep", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"SetZoom", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_ptr, "mediaSize", QUParameter::In }
    };
    static const QUMethod slot_15 = {"ResizeDisplay", 1, param_slot_15 };
    static const QUMethod slot_16 = {"loadPreferences", 0, 0 };
    static const QUParameter param_slot_17[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"SetMinFrame", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"SetMaxFrame", 1, param_slot_18 };
    static const QUMethod slot_19 = {"draggedAImage", 0, 0 };
    static const QUMethod slot_20 = {"Clear", 0, 0 };
    static const QUMethod slot_21 = {"LoopForward", 0, 0 };
    static const QUMethod slot_22 = {"setLoopForward", 0, 0 };
    static const QUMethod slot_23 = {"PingPong", 0, 0 };
    static const QUMethod slot_24 = {"Stop", 0, 0 };
    static const QUMethod slot_25 = {"FastForward", 0, 0 };
    static const QUMethod slot_26 = {"FrameForward", 0, 0 };
    static const QUMethod slot_27 = {"FrameBackward", 0, 0 };
    static const QUMethod slot_28 = {"Rewind", 0, 0 };
    static const QUMethod slot_29 = {"Action", 0, 0 };
    static const QUMethod slot_30 = {"ActionLoopForward", 0, 0 };
    static const QUMethod slot_31 = {"ActionPingPong", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "showImage(int,QImage*)", &slot_0, QMetaData::Public },
	{ "slotGenerateRenderFormatList()", &slot_1, QMetaData::Public },
	{ "slotGenerateRenderProfileList()", &slot_2, QMetaData::Public },
	{ "slotSelectRenderFormat(int)", &slot_3, QMetaData::Public },
	{ "slotSelectRenderProfile(int)", &slot_4, QMetaData::Public },
	{ "slotStartRender()", &slot_5, QMetaData::Public },
	{ "sliderPressed()", &slot_6, QMetaData::Public },
	{ "sliderReleased()", &slot_7, QMetaData::Public },
	{ "LoadSequenceCompleted(int,bool)", &slot_8, QMetaData::Protected },
	{ "sliderValueChanged(int)", &slot_9, QMetaData::Protected },
	{ "SetFrameNumber(int)", &slot_10, QMetaData::Protected },
	{ "SetFPS(FPS)", &slot_11, QMetaData::Protected },
	{ "switchFps(int)", &slot_12, QMetaData::Protected },
	{ "SetStep(int)", &slot_13, QMetaData::Protected },
	{ "SetZoom(int)", &slot_14, QMetaData::Protected },
	{ "ResizeDisplay(mediaSize)", &slot_15, QMetaData::Protected },
	{ "loadPreferences()", &slot_16, QMetaData::Protected },
	{ "SetMinFrame(int)", &slot_17, QMetaData::Protected },
	{ "SetMaxFrame(int)", &slot_18, QMetaData::Protected },
	{ "draggedAImage()", &slot_19, QMetaData::Protected },
	{ "Clear()", &slot_20, QMetaData::Protected },
	{ "LoopForward()", &slot_21, QMetaData::Protected },
	{ "setLoopForward()", &slot_22, QMetaData::Protected },
	{ "PingPong()", &slot_23, QMetaData::Protected },
	{ "Stop()", &slot_24, QMetaData::Protected },
	{ "FastForward()", &slot_25, QMetaData::Protected },
	{ "FrameForward()", &slot_26, QMetaData::Protected },
	{ "FrameBackward()", &slot_27, QMetaData::Protected },
	{ "Rewind()", &slot_28, QMetaData::Protected },
	{ "Action()", &slot_29, QMetaData::Protected },
	{ "ActionLoopForward()", &slot_30, QMetaData::Protected },
	{ "ActionPingPong()", &slot_31, QMetaData::Protected }
    };
    static const QUMethod signal_0 = {"encoderDraggedAImage", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "encoderDraggedAImage()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainEncoder", parentObject,
	slot_tbl, 32,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainEncoder.setMetaObject( metaObj );
    return metaObj;
}

void* MainEncoder::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainEncoder" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL encoderDraggedAImage
void MainEncoder::encoderDraggedAImage()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool MainEncoder::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: showImage((int)static_QUType_int.get(_o+1),(QImage*)static_QUType_varptr.get(_o+2)); break;
    case 1: slotGenerateRenderFormatList(); break;
    case 2: slotGenerateRenderProfileList(); break;
    case 3: slotSelectRenderFormat((int)static_QUType_int.get(_o+1)); break;
    case 4: slotSelectRenderProfile((int)static_QUType_int.get(_o+1)); break;
    case 5: slotStartRender(); break;
    case 6: sliderPressed(); break;
    case 7: sliderReleased(); break;
    case 8: LoadSequenceCompleted((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 9: sliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 10: SetFrameNumber((int)static_QUType_int.get(_o+1)); break;
    case 11: SetFPS((FPS)(*((FPS*)static_QUType_ptr.get(_o+1)))); break;
    case 12: switchFps((int)static_QUType_int.get(_o+1)); break;
    case 13: SetStep((int)static_QUType_int.get(_o+1)); break;
    case 14: SetZoom((int)static_QUType_int.get(_o+1)); break;
    case 15: ResizeDisplay((mediaSize)(*((mediaSize*)static_QUType_ptr.get(_o+1)))); break;
    case 16: loadPreferences(); break;
    case 17: SetMinFrame((int)static_QUType_int.get(_o+1)); break;
    case 18: SetMaxFrame((int)static_QUType_int.get(_o+1)); break;
    case 19: draggedAImage(); break;
    case 20: Clear(); break;
    case 21: LoopForward(); break;
    case 22: setLoopForward(); break;
    case 23: PingPong(); break;
    case 24: Stop(); break;
    case 25: FastForward(); break;
    case 26: FrameForward(); break;
    case 27: FrameBackward(); break;
    case 28: Rewind(); break;
    case 29: Action(); break;
    case 30: ActionLoopForward(); break;
    case 31: ActionPingPong(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainEncoder::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: encoderDraggedAImage(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MainEncoder::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MainEncoder::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
