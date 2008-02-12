/*******************************************************************************
**
** The header file for the Jahshaka jahpreferences object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahtranslate.h"

////////////////////////////////////////////////////////
// we will store the translators in separate files

QString JahTranslate::translateGerman(QString text)
    throw()
{
	QString lang = text.lower();
	bool langchange = false;

	//our slogan :)
	//if ( lang == "jahshaka media intergrator")  
	//{ lang = "Intigrateur de Medias Jahshaka"; langchange = true; }

	// Note:
	// regarding the umlaut tokens and if you dont use UNICODE: I put possible
	// replacements behind the line as a comment.
	// Oh and check the line I added in the main translations section below, 
	// hope its correct that way!  
	// - Heiko

	//main translations
	if ( lang == "english" )               { lang = "Englisch";          langchange = true; }
	if ( lang == "french" )                { lang = "Französisch";       langchange = true; }  // Franzoesisch
	if ( lang == "portuguese" )            { lang = "Portugisisch";      langchange = true; }
	if ( lang == "spanish" )               { lang = "Spanisch";          langchange = true; }
	if ( lang == "german" )                { lang = "Deutsch";           langchange = true; }  // added 

	//language translations
	if ( lang == "&animation" )            { lang = "&Animation";        langchange = true; }
	if ( lang == "animation" )             { lang = "Animation";         langchange = true; }
	if ( lang == "&editing" )              { lang = "&Bearbeiten";       langchange = true; }
	if ( lang == "editing" )               { lang = "Bearbeiten";        langchange = true; }
	if ( lang == "e&ffects" )              { lang = "&Effekte";          langchange = true; }
	if ( lang == "effects" )               { lang = "Effekte";           langchange = true; }
	if ( lang == "&paint" )                { lang = "&Malen";            langchange = true; }
	if ( lang == "paint" )                 { lang = "Malen";             langchange = true; }
	if ( lang == "&text cg" )              { lang = "&Text";             langchange = true; }
	if ( lang == "text cg" )               { lang = "Text";              langchange = true; }
	if ( lang == "&desktop" )              { lang = "&Desktop";          langchange = true; }  
	if ( lang == "desktop" )               { lang = "Desktop";           langchange = true; }  
	if ( lang == "network" )               { lang = "Netzwerk";          langchange = true; }
	if ( lang == "colorize")               { lang = "Kolorieren";        langchange = true; }

	if ( lang == "load" )                  { lang = "Laden";             langchange = true; }
	if ( lang == "save" )                  { lang = "Speichern";         langchange = true; }
	if ( lang == "reset" )                 { lang = "Rücksetzen";        langchange = true; } // Ruecksetzen
	if ( lang == "clear" )                 { lang = "Löschen";           langchange = true; } // Loeschen
	if ( lang == "clear all")              { lang = "Alles löschen";     langchange = true; } // loeschen
	if ( lang == "import")                 { lang = "Importieren";       langchange = true; }
	if ( lang == "export")                 { lang = "Exportieren";       langchange = true; }
	if ( lang == "share")                  { lang = "Weitergeben";       langchange = true; }
	if ( lang == "encode")                 { lang = "Codieren";          langchange = true; }
	if ( lang == "broadcast")              { lang = "Senden";            langchange = true; }
	if ( lang == "load clip" )             { lang = "Clip laden";        langchange = true; }
	if ( lang == "play clip" )             { lang = "Clip abspielen";    langchange = true; }
	if ( lang == "quit")                   { lang = "Beenden";           langchange = true; }
	if ( lang == "&quit")                  { lang = "&Beenden";          langchange = true; }

	if ( lang == "asset")                  { lang = "Element";          langchange = true; }
	if ( lang == "utility")                { lang = "Utility";          langchange = true; }
	if ( lang == "video player")           { lang = "Video Player";     langchange = true; }

	if ( lang == "preferences")            { lang = "Einstellungen";       langchange = true; }
	if ( lang == "interface")              { lang = "Interface";           langchange = true; }
	if ( lang == "platinum")               { lang = "Platin";              langchange = true; }
	if ( lang == "grey")                   { lang = "Grau";                langchange = true; }
	if ( lang == "blue")                   { lang = "Blau";                langchange = true; }
	if ( lang == "green")                  { lang = "Grün";                langchange = true; }
	if ( lang == "white")                  { lang = "Weiß";                langchange = true; } // Weiss
	if ( lang == "metal")                  { lang = "Metall";              langchange = true; }
	if ( lang == "charcoal")               { lang = "Kohle";               langchange = true; }
	if ( lang == "media storage")          { lang = "Medienbibliothek";    langchange = true; }
	if ( lang == "set")                    { lang = "Set";                 langchange = true; }
	if ( lang == "render quality")         { lang = "Renderqualität";      langchange = true; } // Renderqualitaet
	if ( lang == "startup screen")         { lang = "Startbildschirm";     langchange = true; }
	if ( lang == "dual head")              { lang = "Dual Head";           langchange = true; }
	if ( lang == "video head")             { lang = "Video Head";          langchange = true; }
	if ( lang == "remember interface")     { lang = "Interface speichern"; langchange = true; }

	if ( lang == "&get desktop" )          { lang = "Desktop &ändern";   langchange = true; }  
	if ( lang == "get desktop" )           { lang = "Desktop ändern";    langchange = true; }  
	if ( lang == "grab desktop" )          { lang = "Desktopauswahl";   langchange = true; }  
	if ( lang == "empty layer" )           { lang = "Leerer Layer";     langchange = true; }
	if ( lang == "name" )                  { lang = "Name";             langchange = true; }
	if ( lang == "delete" )                { lang = "Löschen";          langchange = true; } // Loeschen
	if ( lang == "key" )                   { lang = "Key";              langchange = true; }
	if ( lang == "photo" )                 { lang = "Foto";             langchange = true; }
	if ( lang == "render" )                { lang = "Rendern";          langchange = true; }
	
	if ( lang == "object" )                { lang = "Objekt";         langchange = true; }
	if ( lang == "aliasing" )              { lang = "Aliasing";       langchange = true; }
	if ( lang == "grid" )                  { lang = "Gitter";         langchange = true; }
	if ( lang == "title safe" )            { lang = "Title Safe";     langchange = true; }
	if ( lang == "startframe" )            { lang = "Start-Frame";    langchange = true; }
	if ( lang == "endframe" )              { lang = "End-Frame";      langchange = true; }
	if ( lang == "axis" )                  { lang = "Achse";          langchange = true; }
	if ( lang == "rotate" )                { lang = "Drehen";         langchange = true; }
	if ( lang == "translate" )             { lang = "Bewegen";        langchange = true; }
	if ( lang == "clip" )                  { lang = "Clip";           langchange = true; }
	if ( lang == "media" )                 { lang = "Media";          langchange = true; }
	if ( lang == "invert" )                { lang = "Invertieren";    langchange = true; }
	if ( lang == "frame in" )              { lang = "Frame In";       langchange = true; }
	if ( lang == "frame out" )             { lang = "Frame Out";      langchange = true; }
	if ( lang == "foreground" )            { lang = "Vordergrund";    langchange = true; }
	if ( lang == "transparency" )          { lang = "Transparenz";    langchange = true; }
	if ( lang == "add" )                   { lang = "Hinzufügen";     langchange = true; } // Hinzufuegen
	if ( lang == "delete" )                { lang = "Löschen";        langchange = true; } // Loeschen
	if ( lang == "del" )                   { lang = "Lö.";            langchange = true; }  // Loe.
	if ( lang == "layers" )                { lang = "Layer";          langchange = true; }
	if ( lang == "settings")               { lang = "Einstellungen";  langchange = true; }
	if ( lang == "motion blur")            { lang = "Motion Blur";    langchange = true; }
	if ( lang == "world")                  { lang = "Welt";           langchange = true; }
	
	if ( lang == "particle")               { lang = "Partikel";       langchange = true; }
	if ( lang == "object")                 { lang = "Objekt";         langchange = true; }
	if ( lang == "maxobject")              { lang = "Objekt Max";     langchange = true; }
	if ( lang == "cylinder")               { lang = "Zylinder";       langchange = true; }
	if ( lang == "sphere")                 { lang = "Kugel";          langchange = true; }
	if ( lang == "bezier")                 { lang = "Bezier";         langchange = true; }
	if ( lang == "mesh")                   { lang = "Mesh";           langchange = true; }
	if ( lang == "layer")                  { lang = "Layer";          langchange = true; }
	if ( lang == "cube")                   { lang = "Würfel";         langchange = true; }  //Wuerfel
	if ( lang == "font")                   { lang = "Font";           langchange = true; }
	if ( lang == "color")                  { lang = "Farbe";          langchange = true; }

	if ( lang == "mirror")                 { lang = "Spiegeln";              langchange = true; }
	if ( lang == "swap rgb")               { lang = "RGB invert.";           langchange = true; }
	if ( lang == "negative")               { lang = "Negativ";               langchange = true; }
	if ( lang == "gaussian blur")          { lang = "Gaußsches Verwisch.";   langchange = true; }  // Gausssches
	if ( lang == "gaussian")               { lang = "Gauß";                  langchange = true; }  // Gauss
	if ( lang == "blur")                   { lang = "Verwischen";            langchange = true; }
	if ( lang == "return")                 { lang = "Zurück";                langchange = true; }  // Zurueck
	if ( lang == "&return")                { lang = "Zu&rück";               langchange = true; }  // Zurueck

	if ( lang == "chat")                   { lang = "Chatten";          langchange = true; }
	if ( lang == "server")                 { lang = "Server";           langchange = true; }
	if ( lang == "server ip")              { lang = "Server IP";        langchange = true; }
	if ( lang == "connect")                { lang = "Verbinden";        langchange = true; }
	if ( lang == "send")                   { lang = "Senden";           langchange = true; }
	if ( lang == "close")                  { lang = "Schließen";        langchange = true; }  // Schliessen
	if ( lang == "send clip")              { lang = "Clip senden";      langchange = true; }
	
	if ( lang == "scene")                  { lang = "Szene";                     langchange = true; }
	if ( lang == "keyframes")              { lang = "Keyframes";                 langchange = true; }
	if ( lang == "render using source res") { lang = "Mit Quellauflösung rend."; langchange = true; }  // Quellenaufloesung

	if ( lang == "toggle mesh")            { lang = "Mesh Toggle";      langchange = true; }
	if ( lang == "zoom")                   { lang = "Zoom";             langchange = true; }
	if ( lang == "timeline")               { lang = "Zeitleiste";       langchange = true; }
	if ( lang == "tools")                  { lang = "Tools";            langchange = true; }
	if ( lang == "draw objects")           { lang = "Objekte zeichn.";  langchange = true; }
	if ( lang == "brushes")                { lang = "Pinsel";           langchange = true; }
	if ( lang == "brush size")             { lang = "Pinselgröße";      langchange = true; }  // Pinselgroesse
	if ( lang == "invert")                 { lang = "Invert";           langchange = true; }
	if ( lang == "fade")                   { lang = "Fade";             langchange = true; }
	
	if ( lang == "contrast")               { lang = "Kontrast";         langchange = true; }
	if ( lang == "negative")               { lang = "Negativ";          langchange = true; }
	if ( lang == "background")             { lang = "Hintergrund";      langchange = true; }
	if ( lang == "graphic")                { lang = "Grafik";           langchange = true; }
	if ( lang == "pen")                    { lang = "Stift";            langchange = true; }
	if ( lang == "fill")                   { lang = "Füllen";           langchange = true; }  // Fuellen
	if ( lang == "gradient angle")         { lang = "Verlaufswinkel";   langchange = true; }
	if ( lang == "intensify")              { lang = "Intensivieren";    langchange = true; }
	if ( lang == "sharpen")                { lang = "Schärfen";         langchange = true; }  // Schaerfen
	
	//no translation found
	if (!langchange)                       { lang = text; } 
	
    return lang;
}

