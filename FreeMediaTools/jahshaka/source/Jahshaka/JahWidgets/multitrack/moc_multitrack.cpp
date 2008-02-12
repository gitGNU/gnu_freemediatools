/****************************************************************************
** MultiTrack meta object code from reading C++ file 'multitrack.h'
**
** Created: Sat Jan 26 08:34:32 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "multitrack.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MultiTrack::className() const
{
    return "MultiTrack";
}

QMetaObject *MultiTrack::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MultiTrack( "MultiTrack", &MultiTrack::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MultiTrack::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MultiTrack", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MultiTrack::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MultiTrack", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MultiTrack::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"deselectAll", 0, 0 };
    static const QUMethod slot_1 = {"clear", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "pos", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setPosition", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "pos", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"seek", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "sc", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setScaling", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "t", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"ensureTimeVisible", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "t", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"centerOnTime", 1, param_slot_6 };
    static const QUMethod slot_7 = {"scrolled", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "deselectAll()", &slot_0, QMetaData::Public },
	{ "clear()", &slot_1, QMetaData::Public },
	{ "setPosition(int)", &slot_2, QMetaData::Public },
	{ "seek(int)", &slot_3, QMetaData::Public },
	{ "setScaling(double)", &slot_4, QMetaData::Public },
	{ "ensureTimeVisible(int)", &slot_5, QMetaData::Public },
	{ "centerOnTime(int)", &slot_6, QMetaData::Public },
	{ "scrolled()", &slot_7, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "item", &static_QUType_ptr, "MultiTrackItem", QUParameter::In },
	{ "to", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"itemMoved", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "state", &static_QUType_ptr, "Qt::ButtonState", QUParameter::In },
	{ "item", &static_QUType_ptr, "MultiTrackItem", QUParameter::In },
	{ "to", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"itemMoved", 3, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "QValueList<MultiTrackItem*>", QUParameter::In },
	{ "delta", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"itemsMoved", 2, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "state", &static_QUType_ptr, "Qt::ButtonState", QUParameter::In },
	{ 0, &static_QUType_ptr, "QValueList<MultiTrackItem*>", QUParameter::In },
	{ "delta", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"itemsMoved", 3, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "delta", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"itemsMoved", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "state", &static_QUType_ptr, "Qt::ButtonState", QUParameter::In },
	{ "delta", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"itemsMoved", 2, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ "item", &static_QUType_ptr, "MultiTrackItem", QUParameter::In },
	{ "pt", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod signal_6 = {"rightClicked", 2, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ "p", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_7 = {"positionChanged", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ "state", &static_QUType_ptr, "Qt::ButtonState", QUParameter::In },
	{ "track", &static_QUType_ptr, "MultiTrackTrack", QUParameter::In }
    };
    static const QUMethod signal_8 = {"trackSelected", 2, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ "item", &static_QUType_ptr, "MultiTrackItem", QUParameter::In },
	{ "time", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_9 = {"itemClicked", 2, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_10 = {"trackSelectEvent", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_11 = {"itemSelectEvent", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ 0, &static_QUType_ptr, "MultiTrackTrack", QUParameter::In }
    };
    static const QUMethod signal_12 = {"silenceClicked", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ 0, &static_QUType_ptr, "MultiTrackTrack", QUParameter::In }
    };
    static const QUMethod signal_13 = {"enableClicked", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ 0, &static_QUType_ptr, "MultiTrackTrack", QUParameter::In }
    };
    static const QUMethod signal_14 = {"lockedClicked", 1, param_signal_14 };
    static const QUParameter param_signal_15[] = {
	{ "event", &static_QUType_ptr, "QDragEnterEvent", QUParameter::In }
    };
    static const QUMethod signal_15 = {"multitrackDragEnterEvent", 1, param_signal_15 };
    static const QUParameter param_signal_16[] = {
	{ "event", &static_QUType_ptr, "QDropEvent", QUParameter::In }
    };
    static const QUMethod signal_16 = {"multitrackDropEvent", 1, param_signal_16 };
    static const QMetaData signal_tbl[] = {
	{ "itemMoved(MultiTrackItem*,int)", &signal_0, QMetaData::Public },
	{ "itemMoved(Qt::ButtonState,MultiTrackItem*,int)", &signal_1, QMetaData::Public },
	{ "itemsMoved(const QValueList<MultiTrackItem*>&,int)", &signal_2, QMetaData::Public },
	{ "itemsMoved(Qt::ButtonState,const QValueList<MultiTrackItem*>&,int)", &signal_3, QMetaData::Public },
	{ "itemsMoved(int)", &signal_4, QMetaData::Public },
	{ "itemsMoved(Qt::ButtonState,int)", &signal_5, QMetaData::Public },
	{ "rightClicked(MultiTrackItem*,const QPoint&)", &signal_6, QMetaData::Public },
	{ "positionChanged(int)", &signal_7, QMetaData::Public },
	{ "trackSelected(Qt::ButtonState,MultiTrackTrack*)", &signal_8, QMetaData::Public },
	{ "itemClicked(MultiTrackItem*,int)", &signal_9, QMetaData::Public },
	{ "trackSelectEvent(QMouseEvent*)", &signal_10, QMetaData::Public },
	{ "itemSelectEvent(QMouseEvent*)", &signal_11, QMetaData::Public },
	{ "silenceClicked(MultiTrackTrack*)", &signal_12, QMetaData::Public },
	{ "enableClicked(MultiTrackTrack*)", &signal_13, QMetaData::Public },
	{ "lockedClicked(MultiTrackTrack*)", &signal_14, QMetaData::Public },
	{ "multitrackDragEnterEvent(QDragEnterEvent*)", &signal_15, QMetaData::Public },
	{ "multitrackDropEvent(QDropEvent*)", &signal_16, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MultiTrack", parentObject,
	slot_tbl, 8,
	signal_tbl, 17,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MultiTrack.setMetaObject( metaObj );
    return metaObj;
}

void* MultiTrack::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MultiTrack" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL itemMoved
void MultiTrack::itemMoved( MultiTrackItem* t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL itemMoved
void MultiTrack::itemMoved( Qt::ButtonState t0, MultiTrackItem* t1, int t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,t1);
    static_QUType_int.set(o+3,t2);
    activate_signal( clist, o );
}

// SIGNAL itemsMoved
void MultiTrack::itemsMoved( const QValueList<MultiTrackItem*>& t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL itemsMoved
void MultiTrack::itemsMoved( Qt::ButtonState t0, const QValueList<MultiTrackItem*>& t1, int t2 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[4];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    static_QUType_int.set(o+3,t2);
    activate_signal( clist, o );
}

// SIGNAL itemsMoved
void MultiTrack::itemsMoved( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL itemsMoved
void MultiTrack::itemsMoved( Qt::ButtonState t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL rightClicked
void MultiTrack::rightClicked( MultiTrackItem* t0, const QPoint& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 6 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_varptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL positionChanged
void MultiTrack::positionChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 7, t0 );
}

// SIGNAL trackSelected
void MultiTrack::trackSelected( Qt::ButtonState t0, MultiTrackTrack* t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 8 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL itemClicked
void MultiTrack::itemClicked( MultiTrackItem* t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 9 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL trackSelectEvent
void MultiTrack::trackSelectEvent( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 10 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL itemSelectEvent
void MultiTrack::itemSelectEvent( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 11 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL silenceClicked
void MultiTrack::silenceClicked( MultiTrackTrack* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 12 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL enableClicked
void MultiTrack::enableClicked( MultiTrackTrack* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 13 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL lockedClicked
void MultiTrack::lockedClicked( MultiTrackTrack* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 14 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL multitrackDragEnterEvent
void MultiTrack::multitrackDragEnterEvent( QDragEnterEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 15 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL multitrackDropEvent
void MultiTrack::multitrackDropEvent( QDropEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 16 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool MultiTrack::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: deselectAll(); break;
    case 1: clear(); break;
    case 2: setPosition((int)static_QUType_int.get(_o+1)); break;
    case 3: seek((int)static_QUType_int.get(_o+1)); break;
    case 4: setScaling((double)static_QUType_double.get(_o+1)); break;
    case 5: ensureTimeVisible((int)static_QUType_int.get(_o+1)); break;
    case 6: centerOnTime((int)static_QUType_int.get(_o+1)); break;
    case 7: scrolled(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MultiTrack::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: itemMoved((MultiTrackItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: itemMoved((Qt::ButtonState)(*((Qt::ButtonState*)static_QUType_ptr.get(_o+1))),(MultiTrackItem*)static_QUType_ptr.get(_o+2),(int)static_QUType_int.get(_o+3)); break;
    case 2: itemsMoved((const QValueList<MultiTrackItem*>&)*((const QValueList<MultiTrackItem*>*)static_QUType_ptr.get(_o+1)),(int)static_QUType_int.get(_o+2)); break;
    case 3: itemsMoved((Qt::ButtonState)(*((Qt::ButtonState*)static_QUType_ptr.get(_o+1))),(const QValueList<MultiTrackItem*>&)*((const QValueList<MultiTrackItem*>*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 4: itemsMoved((int)static_QUType_int.get(_o+1)); break;
    case 5: itemsMoved((Qt::ButtonState)(*((Qt::ButtonState*)static_QUType_ptr.get(_o+1))),(int)static_QUType_int.get(_o+2)); break;
    case 6: rightClicked((MultiTrackItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 7: positionChanged((int)static_QUType_int.get(_o+1)); break;
    case 8: trackSelected((Qt::ButtonState)(*((Qt::ButtonState*)static_QUType_ptr.get(_o+1))),(MultiTrackTrack*)static_QUType_ptr.get(_o+2)); break;
    case 9: itemClicked((MultiTrackItem*)static_QUType_ptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 10: trackSelectEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 11: itemSelectEvent((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 12: silenceClicked((MultiTrackTrack*)static_QUType_ptr.get(_o+1)); break;
    case 13: enableClicked((MultiTrackTrack*)static_QUType_ptr.get(_o+1)); break;
    case 14: lockedClicked((MultiTrackTrack*)static_QUType_ptr.get(_o+1)); break;
    case 15: multitrackDragEnterEvent((QDragEnterEvent*)static_QUType_ptr.get(_o+1)); break;
    case 16: multitrackDropEvent((QDropEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MultiTrack::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MultiTrack::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
