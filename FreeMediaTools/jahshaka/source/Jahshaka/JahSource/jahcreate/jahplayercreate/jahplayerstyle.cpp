/*******************************************************************************
**
** The source file for the Jahshaka Jahstyle module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayercreate.h"
#include "anime.h"
#include <qaction.h>
#include <qsignalmapper.h>

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
    //StyleColor theStyleColor = (StyleColor)color;

    switch(jahStyleColor)   {

            case JGREY      : { styleGrey();        break;      }
            case JWHITE     : { styleWhite();       break;      }
            case JBLUE      : { styleBlue();        break;      }
            case JBLACK     : { styleBlack();       break;      }

            default   : {}
    }

}

/////////////////////////////////////////////////////////
//Color styles are sent up to desktop object so as
//to color all widgets

//setstyle is called by the pushbutton
void JahControl::setstyleGrey()
{
    jahthemes->makeTheme("SGI");
    jahStyleColor = JGREY;
	restoreColor();
}
//style is called by the routine above...
void JahControl::styleGrey()
{
    QPalette p( QColor( 80, 80, 80 ) );
    emit emitStyleColor(p);

    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setStyleColor(jahStyleColor);
}

//setstyle is called by the push button
void JahControl::setstyleBlack()
{
    jahthemes->makeTheme("SGI");
    jahStyleColor = JBLACK;
	restoreColor();
}

//green is thee new black
void JahControl::styleBlack()
{
    QPalette p( QColor( 25, 25, 25 ) );
    emit emitStyleColor(p);

    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setStyleColor(jahStyleColor);
}

void JahControl::setstyleWhite()
{
    jahthemes->makeTheme("SGI");
    jahStyleColor = JWHITE;
	restoreColor();
}

void JahControl::styleWhite()
{
    QPalette p( QColor( 200, 200, 200 ) );
    emit emitStyleColor(p);

    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setStyleColor(jahStyleColor);
}

void JahControl::setstyleBlue()
{
    jahthemes->makeTheme("SGI");
    jahStyleColor = JBLUE;
	restoreColor();
}

void JahControl::styleBlue()
{
    QPalette p( QColor( 100, 100, 200 ) );
    emit emitStyleColor(p);

    JahPrefs& jprefs  = JahPrefs::getInstance();
    jprefs.setStyleColor(jahStyleColor);
}



