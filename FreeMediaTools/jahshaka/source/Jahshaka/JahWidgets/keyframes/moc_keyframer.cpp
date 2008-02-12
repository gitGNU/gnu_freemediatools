/****************************************************************************
** JahKeyframer meta object code from reading C++ file 'keyframer.h'
**
** Created: Sat Jan 26 08:34:22 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "keyframer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahKeyframer::className() const
{
    return "JahKeyframer";
}

QMetaObject *JahKeyframer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahKeyframer( "JahKeyframer", &JahKeyframer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahKeyframer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahKeyframer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahKeyframer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahKeyframer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahKeyframer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"clear", 0, 0 };
    static const QUMethod slot_1 = {"buildInterface", 0, 0 };
    static const QUMethod slot_2 = {"buildKeyframes", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "max_frame", &static_QUType_int, 0, QUParameter::In },
	{ "current_frame", &static_QUType_int, 0, QUParameter::In },
	{ "nodelist", &static_QUType_ptr, "QPtrList<motionNode>", QUParameter::In }
    };
    static const QUMethod slot_3 = {"buildKeyframes", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "keyframe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"movePlayLine", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "x1", &static_QUType_int, 0, QUParameter::In },
	{ "y1", &static_QUType_int, 0, QUParameter::In },
	{ "x2", &static_QUType_int, 0, QUParameter::In },
	{ "y2", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"drawLine", 4, param_slot_5 };
    static const QUMethod slot_6 = {"enlarge", 0, 0 };
    static const QUMethod slot_7 = {"shrink", 0, 0 };
    static const QUMethod slot_8 = {"zoomIn", 0, 0 };
    static const QUMethod slot_9 = {"zoomOut", 0, 0 };
    static const QUMethod slot_10 = {"addPlayLine", 0, 0 };
    static const QUMethod slot_11 = {"emitAddKeyframe", 0, 0 };
    static const QUMethod slot_12 = {"emitRemoveKeyframe", 0, 0 };
    static const QUMethod slot_13 = {"emitPrevKeyframe", 0, 0 };
    static const QUMethod slot_14 = {"emitNextKeyframe", 0, 0 };
    static const QUMethod slot_15 = {"canvasResized", 0, 0 };
    static const QUMethod slot_16 = {"slotZoomInY", 0, 0 };
    static const QUMethod slot_17 = {"slotZoomOutY", 0, 0 };
    static const QUMethod slot_18 = {"slotShiftYUp", 0, 0 };
    static const QUMethod slot_19 = {"slotShiftYDown", 0, 0 };
    static const QUMethod slot_20 = {"languageChange", 0, 0 };
    static const QUMethod slot_21 = {"slotKeyframeChanged", 0, 0 };
    static const QUMethod slot_22 = {"traceSelectionChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "clear()", &slot_0, QMetaData::Public },
	{ "buildInterface()", &slot_1, QMetaData::Public },
	{ "buildKeyframes()", &slot_2, QMetaData::Public },
	{ "buildKeyframes(int,int,QPtrList<motionNode>*)", &slot_3, QMetaData::Public },
	{ "movePlayLine(int)", &slot_4, QMetaData::Public },
	{ "drawLine(int,int,int,int)", &slot_5, QMetaData::Private },
	{ "enlarge()", &slot_6, QMetaData::Private },
	{ "shrink()", &slot_7, QMetaData::Private },
	{ "zoomIn()", &slot_8, QMetaData::Private },
	{ "zoomOut()", &slot_9, QMetaData::Private },
	{ "addPlayLine()", &slot_10, QMetaData::Private },
	{ "emitAddKeyframe()", &slot_11, QMetaData::Private },
	{ "emitRemoveKeyframe()", &slot_12, QMetaData::Private },
	{ "emitPrevKeyframe()", &slot_13, QMetaData::Private },
	{ "emitNextKeyframe()", &slot_14, QMetaData::Private },
	{ "canvasResized()", &slot_15, QMetaData::Private },
	{ "slotZoomInY()", &slot_16, QMetaData::Private },
	{ "slotZoomOutY()", &slot_17, QMetaData::Private },
	{ "slotShiftYUp()", &slot_18, QMetaData::Private },
	{ "slotShiftYDown()", &slot_19, QMetaData::Private },
	{ "languageChange()", &slot_20, QMetaData::Protected },
	{ "slotKeyframeChanged()", &slot_21, QMetaData::Protected },
	{ "traceSelectionChanged()", &slot_22, QMetaData::Protected }
    };
    static const QUMethod signal_0 = {"AddKeyframe", 0, 0 };
    static const QUMethod signal_1 = {"RemoveKeyframe", 0, 0 };
    static const QUMethod signal_2 = {"PrevKeyframe", 0, 0 };
    static const QUMethod signal_3 = {"NextKeyframe", 0, 0 };
    static const QUMethod signal_4 = {"signalKeyframeChanged", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "AddKeyframe()", &signal_0, QMetaData::Private },
	{ "RemoveKeyframe()", &signal_1, QMetaData::Private },
	{ "PrevKeyframe()", &signal_2, QMetaData::Private },
	{ "NextKeyframe()", &signal_3, QMetaData::Private },
	{ "signalKeyframeChanged()", &signal_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahKeyframer", parentObject,
	slot_tbl, 23,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahKeyframer.setMetaObject( metaObj );
    return metaObj;
}

void* JahKeyframer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahKeyframer" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL AddKeyframe
void JahKeyframer::AddKeyframe()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL RemoveKeyframe
void JahKeyframer::RemoveKeyframe()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL PrevKeyframe
void JahKeyframer::PrevKeyframe()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL NextKeyframe
void JahKeyframer::NextKeyframe()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL signalKeyframeChanged
void JahKeyframer::signalKeyframeChanged()
{
    activate_signal( staticMetaObject()->signalOffset() + 4 );
}

bool JahKeyframer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: clear(); break;
    case 1: buildInterface(); break;
    case 2: buildKeyframes(); break;
    case 3: buildKeyframes((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QPtrList<motionNode>*)static_QUType_ptr.get(_o+3)); break;
    case 4: movePlayLine((int)static_QUType_int.get(_o+1)); break;
    case 5: drawLine((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(int)static_QUType_int.get(_o+3),(int)static_QUType_int.get(_o+4)); break;
    case 6: enlarge(); break;
    case 7: shrink(); break;
    case 8: zoomIn(); break;
    case 9: zoomOut(); break;
    case 10: addPlayLine(); break;
    case 11: emitAddKeyframe(); break;
    case 12: emitRemoveKeyframe(); break;
    case 13: emitPrevKeyframe(); break;
    case 14: emitNextKeyframe(); break;
    case 15: canvasResized(); break;
    case 16: slotZoomInY(); break;
    case 17: slotZoomOutY(); break;
    case 18: slotShiftYUp(); break;
    case 19: slotShiftYDown(); break;
    case 20: languageChange(); break;
    case 21: slotKeyframeChanged(); break;
    case 22: traceSelectionChanged(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahKeyframer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: AddKeyframe(); break;
    case 1: RemoveKeyframe(); break;
    case 2: PrevKeyframe(); break;
    case 3: NextKeyframe(); break;
    case 4: signalKeyframeChanged(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahKeyframer::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahKeyframer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
