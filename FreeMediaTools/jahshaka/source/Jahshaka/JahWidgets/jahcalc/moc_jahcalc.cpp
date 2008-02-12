/****************************************************************************
** JahCalc meta object code from reading C++ file 'jahcalc.h'
**
** Created: Sat Jan 26 08:34:06 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahcalc.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahCalc::className() const
{
    return "JahCalc";
}

QMetaObject *JahCalc::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahCalc( "JahCalc", &JahCalc::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahCalc::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahCalc", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahCalc::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahCalc", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahCalc::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QUMethod slot_1 = {"returnAccept", 0, 0 };
    static const QUMethod slot_2 = {"returnCancel", 0, 0 };
    static const QUMethod slot_3 = {"clearAll", 0, 0 };
    static const QUMethod slot_4 = {"select0", 0, 0 };
    static const QUMethod slot_5 = {"select1", 0, 0 };
    static const QUMethod slot_6 = {"select2", 0, 0 };
    static const QUMethod slot_7 = {"select3", 0, 0 };
    static const QUMethod slot_8 = {"select4", 0, 0 };
    static const QUMethod slot_9 = {"select5", 0, 0 };
    static const QUMethod slot_10 = {"select6", 0, 0 };
    static const QUMethod slot_11 = {"select7", 0, 0 };
    static const QUMethod slot_12 = {"select8", 0, 0 };
    static const QUMethod slot_13 = {"select9", 0, 0 };
    static const QUMethod slot_14 = {"selectAdd", 0, 0 };
    static const QUMethod slot_15 = {"selectSubtract", 0, 0 };
    static const QUMethod slot_16 = {"selectMultiply", 0, 0 };
    static const QUMethod slot_17 = {"selectModulo", 0, 0 };
    static const QUMethod slot_18 = {"selectDivide", 0, 0 };
    static const QUMethod slot_19 = {"selectTotal", 0, 0 };
    static const QUMethod slot_20 = {"doMath", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected },
	{ "returnAccept()", &slot_1, QMetaData::Protected },
	{ "returnCancel()", &slot_2, QMetaData::Protected },
	{ "clearAll()", &slot_3, QMetaData::Protected },
	{ "select0()", &slot_4, QMetaData::Protected },
	{ "select1()", &slot_5, QMetaData::Protected },
	{ "select2()", &slot_6, QMetaData::Protected },
	{ "select3()", &slot_7, QMetaData::Protected },
	{ "select4()", &slot_8, QMetaData::Protected },
	{ "select5()", &slot_9, QMetaData::Protected },
	{ "select6()", &slot_10, QMetaData::Protected },
	{ "select7()", &slot_11, QMetaData::Protected },
	{ "select8()", &slot_12, QMetaData::Protected },
	{ "select9()", &slot_13, QMetaData::Protected },
	{ "selectAdd()", &slot_14, QMetaData::Protected },
	{ "selectSubtract()", &slot_15, QMetaData::Protected },
	{ "selectMultiply()", &slot_16, QMetaData::Protected },
	{ "selectModulo()", &slot_17, QMetaData::Protected },
	{ "selectDivide()", &slot_18, QMetaData::Protected },
	{ "selectTotal()", &slot_19, QMetaData::Protected },
	{ "doMath()", &slot_20, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahCalc", parentObject,
	slot_tbl, 21,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahCalc.setMetaObject( metaObj );
    return metaObj;
}

void* JahCalc::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahCalc" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool JahCalc::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    case 1: returnAccept(); break;
    case 2: returnCancel(); break;
    case 3: clearAll(); break;
    case 4: select0(); break;
    case 5: select1(); break;
    case 6: select2(); break;
    case 7: select3(); break;
    case 8: select4(); break;
    case 9: select5(); break;
    case 10: select6(); break;
    case 11: select7(); break;
    case 12: select8(); break;
    case 13: select9(); break;
    case 14: selectAdd(); break;
    case 15: selectSubtract(); break;
    case 16: selectMultiply(); break;
    case 17: selectModulo(); break;
    case 18: selectDivide(); break;
    case 19: selectTotal(); break;
    case 20: doMath(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahCalc::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahCalc::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool JahCalc::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
