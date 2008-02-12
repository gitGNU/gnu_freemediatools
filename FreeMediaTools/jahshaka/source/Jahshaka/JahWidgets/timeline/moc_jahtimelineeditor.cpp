/****************************************************************************
** EditEditor meta object code from reading C++ file 'jahtimelineeditor.h'
**
** Created: Sat Jan 26 19:17:52 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahtimelineeditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *EditEditor::className() const
{
    return "EditEditor";
}

QMetaObject *EditEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_EditEditor( "EditEditor", &EditEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString EditEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EditEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString EditEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "EditEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* EditEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCanvasView::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"status", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"updateEditanimframe", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"updateSetinFrames", 2, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "status(const QString&)", &signal_0, QMetaData::Protected },
	{ "updateEditanimframe(int)", &signal_1, QMetaData::Protected },
	{ "updateSetinFrames(int,int)", &signal_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"EditEditor", parentObject,
	0, 0,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_EditEditor.setMetaObject( metaObj );
    return metaObj;
}

void* EditEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "EditEditor" ) )
	return this;
    return QCanvasView::qt_cast( clname );
}

// SIGNAL status
void EditEditor::status( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL updateEditanimframe
void EditEditor::updateEditanimframe( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL updateSetinFrames
void EditEditor::updateSetinFrames( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

bool EditEditor::qt_invoke( int _id, QUObject* _o )
{
    return QCanvasView::qt_invoke(_id,_o);
}

bool EditEditor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: status((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: updateEditanimframe((int)static_QUType_int.get(_o+1)); break;
    case 2: updateSetinFrames((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return QCanvasView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool EditEditor::qt_property( int id, int f, QVariant* v)
{
    return QCanvasView::qt_property( id, f, v);
}

bool EditEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
