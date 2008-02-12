/*******************************************************************************
**
** The source file for the Jahshaka Jahstyle module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifdef JAHPLAYER
#include "jahplayercreate.h"
#else
#include "jahcreate.h"
#endif

#include <qpopupmenu.h>
#include <qaction.h>
#include <qsignalmapper.h>
#include <qstylefactory.h>
#include <qdict.h>
#include <qpushbutton.h>

#include "jahtracer.h"
#include "jahtranslate.h"
#include "jahpreferences.h"
#include "themes.h"

///////////////////////////////////////////////
//we should initialize the jahtheme object here
//if !jahthemes 
//    jahthemes = new JahThemes ();
// instead of in JahControl::CreateDesktopPreferences

//need to save changes here to the prefs object

void JahControl::setJahTheme()
{
    //debug("setting theme");
    //appFont = QApplication::font();

    QString thestyle;
    
    QPopupMenu *style = new QPopupMenu( 0 );
    
    style->setCheckable( TRUE );

    QActionGroup *ag = new QActionGroup( this, 0 );
    ag->setExclusive( TRUE );
    
    QSignalMapper *styleMapper = new QSignalMapper( this );
    connect( styleMapper, SIGNAL( mapped( const QString& ) ), jahthemes, SLOT( makeTheme( const QString& ) ) );
    connect( styleMapper, SIGNAL( mapped( const QString& ) ), this, SLOT( recordTheme( const QString& ) ) );
    
    QStringList list = QStyleFactory::keys();
    list.sort();
	
    ////////////////////////////////////////////
    // our custom styles are here
    
#ifndef QT_NO_STYLE_WINDOWS
    list.insert(list.begin(), "High Tech");
    list.insert(list.begin(), "Marble");
    list.insert(list.begin(), "Brush Metal");
    list.insert(list.begin(), "Charcoal");
#endif
    list.insert(list.begin(), "SuperG");
    
    QDict<int> stylesDict( 17, FALSE );
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) 
    {
        QString styleStr = *it;
        QString styleAccel = styleStr;
        
        if ( stylesDict[styleAccel.left(1)] ) 
        {
            for ( uint i = 0; i < styleAccel.length(); i++ ) 
            {
                if ( !stylesDict[styleAccel.mid( i, 1 )] ) 
                {
                    stylesDict.insert(styleAccel.mid( i, 1 ), (const int *)1);
                    styleAccel = styleAccel.insert( i, '&' );
                    break;
                }
            }
        } 
        else 
        {
            stylesDict.insert(styleAccel.left(1), (const int *)1);
            styleAccel = "&"+styleAccel;
        }
        
        QAction *a = new QAction( styleStr, QIconSet(), styleAccel, 0, ag, 0, ag->isExclusive() );
        connect( a, SIGNAL( activated() ), styleMapper, SLOT(map()) );
        styleMapper->setMapping( a, a->text() );
        
        thestyle = styleStr;
        //debug("listed style"+thestyle);
        
    }
    

    ag->addTo(style);
    
    style->exec( stylebutton->mapToGlobal( QPoint( 0, stylebutton->height() + 1 ) ) );
    
    //if they use a theme color doesnt work mon
    jahStyleColor = NOCOLOR;

}

void JahControl::recordTheme(const QString &thestyle)
{

    //debug("style is:"+thestyle);
    
    //this needs to now set this in the preferences!
    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setThemestyle(thestyle.data());
}

////////////////////////////////////////////////////////
//used to set the global font and font size

void JahControl::styleFont(int size, QString font)
{
    QFont appFont;

    //dont know which ones work so we do them all!
    appFont = QApplication::font();
    appFont.setPointSize(size);
    appFont.setFamily(font);

    QFont::setDefaultFont(QFont(font, size, QFont::Normal, false));
    qApp->setFont( QFont( font, size ), TRUE );
}

//run at startuptime to restore the style and color
void JahControl::restoreStyle()
{
    /////////////////////////////////////////////////////////////////
    //restore the style
    JahTrace::getInstance()->debug("in restorestyle");

    JahPrefs& jprefs  = JahPrefs::getInstance();

    if ( !jprefs.getStyle() || jprefs.getStyleTheme() == "NULL" )
    {
        JahThemes th;
        th.makeTheme( "SuperG" );
        return;
    }

    if ( jprefs.getStyle() )
    {

        //restore the theme here
        std::string theTheme = jprefs.getStyleTheme().data();
        if (theTheme!="NULL")
        {
            JahThemes th;
            th.makeTheme(theTheme.data());
        }
    }
}


void JahControl::restoreColor(void)
{
    jtrace->debug("in restorecolor");

    JahPrefs& jprefs  = JahPrefs::getInstance();

    if ( jprefs.getStyle() )
    {
        JahPrefs& jprefs  = JahPrefs::getInstance();
        int theColorValue = jprefs.getStyleColor();
        jahStyleColor = (StyleColor)theColorValue;

        if ( jahStyleColor == NOCOLOR)
        {
            return;
        }
    
        saveAndUpdateColor();
    }

}

QColor JahControl::findColorForStyleColor( StyleColor s )
{
    switch( s )
    {
    case JGREY:
        return  QColor( 80, 80, 80 );
        
    case JWHITE:
        return QColor( 200, 200, 200 );

    case JBLUE:
        return QColor( 100, 100, 200 );

    case JBLACK:
    default:
        return QColor( 25, 25, 25 );
    }
}

/////////////////////////////////////////////////////////
//Color styles are sent up to desktop object so as
//to color all widgets

void JahControl::saveAndUpdateColor()
{
    QPalette p( findColorForStyleColor( jahStyleColor ) );
    emit emitStyleColor(p);

    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setStyleColor(jahStyleColor);
}

//setstyle is called by the pushbutton
void JahControl::setstyleGrey()
{
    jahStyleColor = JGREY;
    saveAndUpdateColor();
}

//setstyle is called by the push button
void JahControl::setstyleBlack()
{
    jahStyleColor = JBLACK;
    saveAndUpdateColor();
}

void JahControl::setstyleWhite()
{
    jahStyleColor = JWHITE;
    saveAndUpdateColor();
}

void JahControl::setstyleBlue()
{
    jahStyleColor = JBLUE;
    saveAndUpdateColor();
}
