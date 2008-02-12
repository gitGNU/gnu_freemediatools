/****************************************************************************
** ApollonTab meta object code from reading C++ file 'apollontab.h'
**
** Created: Wed Jan 23 18:40:46 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "apollontab.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ApollonTab::className() const
{
    return "ApollonTab";
}

QMetaObject *ApollonTab::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ApollonTab( "ApollonTab", &ApollonTab::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ApollonTab::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonTab", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ApollonTab::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ApollonTab", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ApollonTab::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"statusMessage", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "user", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"browseUser", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "statusMessage(QString)", &signal_0, QMetaData::Protected },
	{ "browseUser(const QString&)", &signal_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ApollonTab", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ApollonTab.setMetaObject( metaObj );
    return metaObj;
}

void* ApollonTab::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ApollonTab" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL statusMessage
void ApollonTab::statusMessage( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL browseUser
void ApollonTab::browseUser( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool ApollonTab::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool ApollonTab::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: statusMessage((QString)static_QUType_QString.get(_o+1)); break;
    case 1: browseUser((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ApollonTab::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ApollonTab::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
