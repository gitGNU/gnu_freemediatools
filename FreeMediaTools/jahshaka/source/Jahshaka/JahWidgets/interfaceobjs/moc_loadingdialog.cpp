/****************************************************************************
** loadingDialog meta object code from reading C++ file 'loadingdialog.h'
**
** Created: Sat Jan 26 08:34:02 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "loadingdialog.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *loadingDialog::className() const
{
    return "loadingDialog";
}

QMetaObject *loadingDialog::metaObj = 0;
static QMetaObjectCleanUp cleanUp_loadingDialog( "loadingDialog", &loadingDialog::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString loadingDialog::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "loadingDialog", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString loadingDialog::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "loadingDialog", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* loadingDialog::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"loadingDialog", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_loadingDialog.setMetaObject( metaObj );
    return metaObj;
}

void* loadingDialog::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "loadingDialog" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool loadingDialog::qt_invoke( int _id, QUObject* _o )
{
    return QDialog::qt_invoke(_id,_o);
}

bool loadingDialog::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool loadingDialog::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool loadingDialog::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
