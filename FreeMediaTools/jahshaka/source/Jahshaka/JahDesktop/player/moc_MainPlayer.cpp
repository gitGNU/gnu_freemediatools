/****************************************************************************
** MainPlayer meta object code from reading C++ file 'MainPlayer.h'
**
** Created: Tue Jan 29 23:20:01 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MainPlayer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainPlayer::className() const
{
    return "MainPlayer";
}

QMetaObject *MainPlayer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainPlayer( "MainPlayer", &MainPlayer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainPlayer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainPlayer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainPlayer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainPlayer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainPlayer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"toggleDesktopImages", 0, 0 };
    static const QUMethod slot_1 = {"toggleDesktopTable", 0, 0 };
    static const QUMethod slot_2 = {"LoadSequenceCompleted", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"SetFrameNumber", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"switchModeFps", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"switchModeStep", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"SetZoom", 1, param_slot_6 };
    static const QUMethod slot_7 = {"toggleZoomLock", 0, 0 };
    static const QUMethod slot_8 = {"toggleZoomReset", 0, 0 };
    static const QUMethod slot_9 = {"updateZoomIn", 0, 0 };
    static const QUMethod slot_10 = {"updateZoomOut", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"SetMinFrame", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"SetMaxFrame", 1, param_slot_12 };
    static const QUMethod slot_13 = {"toggleViewAll", 0, 0 };
    static const QUMethod slot_14 = {"toggleViewRed", 0, 0 };
    static const QUMethod slot_15 = {"toggleViewGreen", 0, 0 };
    static const QUMethod slot_16 = {"toggleViewBlue", 0, 0 };
    static const QUMethod slot_17 = {"toggleViewAlpha", 0, 0 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"setPlayMode", 1, param_slot_18 };
    static const QUMethod slot_19 = {"draggedAImage", 0, 0 };
    static const QUMethod slot_20 = {"requestNextItem", 0, 0 };
    static const QUParameter param_slot_21[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_21 = {"sliderValueChanged", 1, param_slot_21 };
    static const QUMethod slot_22 = {"sliderPressed", 0, 0 };
    static const QUMethod slot_23 = {"sliderReleased", 0, 0 };
    static const QUParameter param_slot_24[] = {
	{ "position", &static_QUType_int, 0, QUParameter::In },
	{ "image", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_24 = {"showImage", 2, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_25 = {"realTimeChanged", 1, param_slot_25 };
    static const QUMethod slot_26 = {"LoopForward", 0, 0 };
    static const QUMethod slot_27 = {"Stop", 0, 0 };
    static const QUMethod slot_28 = {"FastForward", 0, 0 };
    static const QUMethod slot_29 = {"FrameForward", 0, 0 };
    static const QUMethod slot_30 = {"FrameBackward", 0, 0 };
    static const QUMethod slot_31 = {"Rewind", 0, 0 };
    static const QUMethod slot_32 = {"toggleRamPlayer", 0, 0 };
    static const QUParameter param_slot_33[] = {
	{ 0, &static_QUType_ptr, "JahHeadable", QUParameter::In }
    };
    static const QUMethod slot_33 = {"showHead", 1, param_slot_33 };
    static const QMetaData slot_tbl[] = {
	{ "toggleDesktopImages()", &slot_0, QMetaData::Protected },
	{ "toggleDesktopTable()", &slot_1, QMetaData::Protected },
	{ "LoadSequenceCompleted()", &slot_2, QMetaData::Protected },
	{ "SetFrameNumber(int)", &slot_3, QMetaData::Protected },
	{ "switchModeFps(int)", &slot_4, QMetaData::Protected },
	{ "switchModeStep(int)", &slot_5, QMetaData::Protected },
	{ "SetZoom(int)", &slot_6, QMetaData::Protected },
	{ "toggleZoomLock()", &slot_7, QMetaData::Protected },
	{ "toggleZoomReset()", &slot_8, QMetaData::Protected },
	{ "updateZoomIn()", &slot_9, QMetaData::Protected },
	{ "updateZoomOut()", &slot_10, QMetaData::Protected },
	{ "SetMinFrame(int)", &slot_11, QMetaData::Protected },
	{ "SetMaxFrame(int)", &slot_12, QMetaData::Protected },
	{ "toggleViewAll()", &slot_13, QMetaData::Protected },
	{ "toggleViewRed()", &slot_14, QMetaData::Protected },
	{ "toggleViewGreen()", &slot_15, QMetaData::Protected },
	{ "toggleViewBlue()", &slot_16, QMetaData::Protected },
	{ "toggleViewAlpha()", &slot_17, QMetaData::Protected },
	{ "setPlayMode(int)", &slot_18, QMetaData::Protected },
	{ "draggedAImage()", &slot_19, QMetaData::Protected },
	{ "requestNextItem()", &slot_20, QMetaData::Protected },
	{ "sliderValueChanged(int)", &slot_21, QMetaData::Protected },
	{ "sliderPressed()", &slot_22, QMetaData::Protected },
	{ "sliderReleased()", &slot_23, QMetaData::Protected },
	{ "showImage(int,QImage*)", &slot_24, QMetaData::Public },
	{ "realTimeChanged(bool)", &slot_25, QMetaData::Public },
	{ "LoopForward()", &slot_26, QMetaData::Public },
	{ "Stop()", &slot_27, QMetaData::Public },
	{ "FastForward()", &slot_28, QMetaData::Public },
	{ "FrameForward()", &slot_29, QMetaData::Public },
	{ "FrameBackward()", &slot_30, QMetaData::Public },
	{ "Rewind()", &slot_31, QMetaData::Public },
	{ "toggleRamPlayer()", &slot_32, QMetaData::Public },
	{ "showHead(JahHeadable*)", &slot_33, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"toggleTheDesktopImages", 0, 0 };
    static const QUMethod signal_1 = {"toggleTheDesktopTable", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_2 = {"syncMediaTable", 1, param_signal_2 };
    static const QUMethod signal_3 = {"playerDraggedAImage", 0, 0 };
    static const QUMethod signal_4 = {"playerRequestsNextItem", 0, 0 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_ptr, "JahHeadable", QUParameter::In }
    };
    static const QUMethod signal_5 = {"showInHead", 1, param_signal_5 };
    static const QMetaData signal_tbl[] = {
	{ "toggleTheDesktopImages()", &signal_0, QMetaData::Public },
	{ "toggleTheDesktopTable()", &signal_1, QMetaData::Public },
	{ "syncMediaTable(assetData)", &signal_2, QMetaData::Public },
	{ "playerDraggedAImage()", &signal_3, QMetaData::Public },
	{ "playerRequestsNextItem()", &signal_4, QMetaData::Public },
	{ "showInHead(JahHeadable*)", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainPlayer", parentObject,
	slot_tbl, 34,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainPlayer.setMetaObject( metaObj );
    return metaObj;
}

void* MainPlayer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainPlayer" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL toggleTheDesktopImages
void MainPlayer::toggleTheDesktopImages()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL toggleTheDesktopTable
void MainPlayer::toggleTheDesktopTable()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL syncMediaTable
void MainPlayer::syncMediaTable( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL playerDraggedAImage
void MainPlayer::playerDraggedAImage()
{
    activate_signal( staticMetaObject()->signalOffset() + 3 );
}

// SIGNAL playerRequestsNextItem
void MainPlayer::playerRequestsNextItem()
{
    activate_signal( staticMetaObject()->signalOffset() + 4 );
}

// SIGNAL showInHead
void MainPlayer::showInHead( JahHeadable* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool MainPlayer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: toggleDesktopImages(); break;
    case 1: toggleDesktopTable(); break;
    case 2: LoadSequenceCompleted(); break;
    case 3: SetFrameNumber((int)static_QUType_int.get(_o+1)); break;
    case 4: switchModeFps((int)static_QUType_int.get(_o+1)); break;
    case 5: switchModeStep((int)static_QUType_int.get(_o+1)); break;
    case 6: SetZoom((int)static_QUType_int.get(_o+1)); break;
    case 7: toggleZoomLock(); break;
    case 8: toggleZoomReset(); break;
    case 9: updateZoomIn(); break;
    case 10: updateZoomOut(); break;
    case 11: SetMinFrame((int)static_QUType_int.get(_o+1)); break;
    case 12: SetMaxFrame((int)static_QUType_int.get(_o+1)); break;
    case 13: toggleViewAll(); break;
    case 14: toggleViewRed(); break;
    case 15: toggleViewGreen(); break;
    case 16: toggleViewBlue(); break;
    case 17: toggleViewAlpha(); break;
    case 18: setPlayMode((int)static_QUType_int.get(_o+1)); break;
    case 19: draggedAImage(); break;
    case 20: requestNextItem(); break;
    case 21: sliderValueChanged((int)static_QUType_int.get(_o+1)); break;
    case 22: sliderPressed(); break;
    case 23: sliderReleased(); break;
    case 24: showImage((int)static_QUType_int.get(_o+1),(QImage*)static_QUType_varptr.get(_o+2)); break;
    case 25: realTimeChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 26: LoopForward(); break;
    case 27: Stop(); break;
    case 28: FastForward(); break;
    case 29: FrameForward(); break;
    case 30: FrameBackward(); break;
    case 31: Rewind(); break;
    case 32: toggleRamPlayer(); break;
    case 33: showHead((JahHeadable*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainPlayer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: toggleTheDesktopImages(); break;
    case 1: toggleTheDesktopTable(); break;
    case 2: syncMediaTable((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 3: playerDraggedAImage(); break;
    case 4: playerRequestsNextItem(); break;
    case 5: showInHead((JahHeadable*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MainPlayer::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MainPlayer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
