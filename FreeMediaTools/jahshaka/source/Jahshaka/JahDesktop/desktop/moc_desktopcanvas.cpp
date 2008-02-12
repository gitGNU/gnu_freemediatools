/****************************************************************************
** FigureEditor meta object code from reading C++ file 'desktopcanvas.h'
**
** Created: Fri Feb 1 08:06:33 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "desktopcanvas.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FigureEditor::className() const
{
    return "FigureEditor";
}

QMetaObject *FigureEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FigureEditor( "FigureEditor", &FigureEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FigureEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FigureEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FigureEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FigureEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FigureEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCanvasView::staticMetaObject();
    static const QUMethod slot_0 = {"playControlClicked", 0, 0 };
    static const QUMethod slot_1 = {"playControlStop", 0, 0 };
    static const QUMethod slot_2 = {"playControlPlay", 0, 0 };
    static const QUMethod slot_3 = {"forwardControlClicked", 0, 0 };
    static const QUMethod slot_4 = {"fForwardControlClicked", 0, 0 };
    static const QUMethod slot_5 = {"backControlClicked", 0, 0 };
    static const QUMethod slot_6 = {"rewindControlClicked", 0, 0 };
    static const QUMethod slot_7 = {"advanceClipFrame", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "dframe", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"advanceClipFrame", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "dframe", &static_QUType_int, 0, QUParameter::In },
	{ "keepplaying", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"advanceClipFrame", 2, param_slot_9 };
    static const QUMethod slot_10 = {"revealPlayerControls", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"playerControlsMoveBy", 2, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "x", &static_QUType_int, 0, QUParameter::In },
	{ "y", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"playerControlsMoveTo", 2, param_slot_12 };
    static const QUMethod slot_13 = {"showHidePlayerControls", 0, 0 };
    static const QUParameter param_slot_14[] = {
	{ "show", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"showHidePlayerControls", 1, param_slot_14 };
    static const QUMethod slot_15 = {"mouseStoppedSlot", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "playControlClicked()", &slot_0, QMetaData::Public },
	{ "playControlStop()", &slot_1, QMetaData::Public },
	{ "playControlPlay()", &slot_2, QMetaData::Public },
	{ "forwardControlClicked()", &slot_3, QMetaData::Public },
	{ "fForwardControlClicked()", &slot_4, QMetaData::Public },
	{ "backControlClicked()", &slot_5, QMetaData::Public },
	{ "rewindControlClicked()", &slot_6, QMetaData::Public },
	{ "advanceClipFrame()", &slot_7, QMetaData::Public },
	{ "advanceClipFrame(int)", &slot_8, QMetaData::Public },
	{ "advanceClipFrame(int,bool)", &slot_9, QMetaData::Public },
	{ "revealPlayerControls()", &slot_10, QMetaData::Public },
	{ "playerControlsMoveBy(int,int)", &slot_11, QMetaData::Public },
	{ "playerControlsMoveTo(int,int)", &slot_12, QMetaData::Public },
	{ "showHidePlayerControls()", &slot_13, QMetaData::Public },
	{ "showHidePlayerControls(bool)", &slot_14, QMetaData::Public },
	{ "mouseStoppedSlot()", &slot_15, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"status", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_1 = {"updateClipPosition", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_2 = {"updateClipSize", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_3 = {"updateLockStatus", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_4 = {"updateFilmstripStatus", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod signal_5 = {"signalUpdateFrameOffset", 1, param_signal_5 };
    static const QUMethod signal_6 = {"activeFlagDesktop", 0, 0 };
    static const QUParameter param_signal_7[] = {
	{ "clipnumber", &static_QUType_int, 0, QUParameter::In },
	{ "newname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"itemRenamed", 2, param_signal_7 };
    static const QMetaData signal_tbl[] = {
	{ "status(const QString&)", &signal_0, QMetaData::Protected },
	{ "updateClipPosition(assetData)", &signal_1, QMetaData::Protected },
	{ "updateClipSize(assetData)", &signal_2, QMetaData::Protected },
	{ "updateLockStatus(assetData)", &signal_3, QMetaData::Protected },
	{ "updateFilmstripStatus(assetData)", &signal_4, QMetaData::Protected },
	{ "signalUpdateFrameOffset(assetData)", &signal_5, QMetaData::Protected },
	{ "activeFlagDesktop()", &signal_6, QMetaData::Protected },
	{ "itemRenamed(int,QString)", &signal_7, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FigureEditor", parentObject,
	slot_tbl, 16,
	signal_tbl, 8,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FigureEditor.setMetaObject( metaObj );
    return metaObj;
}

void* FigureEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FigureEditor" ) )
	return this;
    return QCanvasView::qt_cast( clname );
}

// SIGNAL status
void FigureEditor::status( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL updateClipPosition
void FigureEditor::updateClipPosition( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL updateClipSize
void FigureEditor::updateClipSize( assetData t0 )
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

// SIGNAL updateLockStatus
void FigureEditor::updateLockStatus( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL updateFilmstripStatus
void FigureEditor::updateFilmstripStatus( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL signalUpdateFrameOffset
void FigureEditor::signalUpdateFrameOffset( assetData t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL activeFlagDesktop
void FigureEditor::activeFlagDesktop()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

// SIGNAL itemRenamed
void FigureEditor::itemRenamed( int t0, QString t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 7 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_QString.set(o+2,t1);
    activate_signal( clist, o );
}

bool FigureEditor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: playControlClicked(); break;
    case 1: playControlStop(); break;
    case 2: playControlPlay(); break;
    case 3: forwardControlClicked(); break;
    case 4: fForwardControlClicked(); break;
    case 5: backControlClicked(); break;
    case 6: rewindControlClicked(); break;
    case 7: advanceClipFrame(); break;
    case 8: advanceClipFrame((int)static_QUType_int.get(_o+1)); break;
    case 9: advanceClipFrame((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 10: revealPlayerControls(); break;
    case 11: playerControlsMoveBy((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 12: playerControlsMoveTo((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 13: showHidePlayerControls(); break;
    case 14: showHidePlayerControls((bool)static_QUType_bool.get(_o+1)); break;
    case 15: mouseStoppedSlot(); break;
    default:
	return QCanvasView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FigureEditor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: status((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: updateClipPosition((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 2: updateClipSize((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 3: updateLockStatus((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 4: updateFilmstripStatus((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 5: signalUpdateFrameOffset((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 6: activeFlagDesktop(); break;
    case 7: itemRenamed((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    default:
	return QCanvasView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool FigureEditor::qt_property( int id, int f, QVariant* v)
{
    return QCanvasView::qt_property( id, f, v);
}

bool FigureEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
