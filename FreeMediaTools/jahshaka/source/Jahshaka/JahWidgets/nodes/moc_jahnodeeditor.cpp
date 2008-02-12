/****************************************************************************
** NodeEditor meta object code from reading C++ file 'jahnodeeditor.h'
**
** Created: Sat Jan 26 08:34:29 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahnodeeditor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *NodeEditor::className() const
{
    return "NodeEditor";
}

QMetaObject *NodeEditor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_NodeEditor( "NodeEditor", &NodeEditor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString NodeEditor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NodeEditor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString NodeEditor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "NodeEditor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* NodeEditor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCanvasView::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"status", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "layernumber", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"nodeClicked", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "status(const QString&)", &signal_0, QMetaData::Protected },
	{ "nodeClicked(int)", &signal_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"NodeEditor", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_NodeEditor.setMetaObject( metaObj );
    return metaObj;
}

void* NodeEditor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "NodeEditor" ) )
	return this;
    return QCanvasView::qt_cast( clname );
}

// SIGNAL status
void NodeEditor::status( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL nodeClicked
void NodeEditor::nodeClicked( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool NodeEditor::qt_invoke( int _id, QUObject* _o )
{
    return QCanvasView::qt_invoke(_id,_o);
}

bool NodeEditor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: status((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: nodeClicked((int)static_QUType_int.get(_o+1)); break;
    default:
	return QCanvasView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool NodeEditor::qt_property( int id, int f, QVariant* v)
{
    return QCanvasView::qt_property( id, f, v);
}

bool NodeEditor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
