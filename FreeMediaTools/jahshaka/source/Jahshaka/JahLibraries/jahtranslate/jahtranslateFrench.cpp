/*******************************************************************************
**
** The source file for the Jahshaka jahtranslator object 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahtranslate.h"

/////////////////////////////////////////////////////////////////
//routines to set data start here


QString JahTranslate::translateFrench(QString text)
    throw()
{
	QString lang = text.lower();
	bool langchange = false;

	//our slogan :)
	if ( lang == "jahshaka media intergrator")  
	{ lang = "Intigrateur de Medias Jahshaka"; langchange = true; }

	//main translations
	if ( lang == "english" )               { lang = "English";    langchange = true; }
	if ( lang == "french" )                { lang = "French";     langchange = true; }
	if ( lang == "portuguese" )            { lang = "Portuguese"; langchange = true; }
	if ( lang == "spanish" )               { lang = "Espanol";    langchange = true; }

	//language translations
	if ( lang == "&animation" )            { lang = "&Animation";        langchange = true; }
	if ( lang == "animation" )             { lang = "Animation";         langchange = true; }
	if ( lang == "&editing" )              { lang = "&Montage";          langchange = true; }
	if ( lang == "editing" )               { lang = "Montage";           langchange = true; }
	if ( lang == "e&ffects" )              { lang = "E&ffets";           langchange = true; }
	if ( lang == "effects" )               { lang = "Effets";            langchange = true; }
	if ( lang == "&paint" )                { lang = "&Peinture";         langchange = true; }
	if ( lang == "paint" )                 { lang = "Peinture";          langchange = true; }
	if ( lang == "&text cg" )              { lang = "&Typo";             langchange = true; }
	if ( lang == "text cg" )               { lang = "Typo";              langchange = true; }
	if ( lang == "&desktop" )              { lang = "&Bureau";           langchange = true; }
	if ( lang == "desktop" )               { lang = "Bureau";            langchange = true; }
	if ( lang == "network" )               { lang = "Riseau";            langchange = true; }
	if ( lang == "colorize")               { lang = "Italonnage";        langchange = true; }

	if ( lang == "load" )                  { lang = "Importer";          langchange = true; }
	if ( lang == "save" )                  { lang = "Enregistre";        langchange = true; }
	if ( lang == "reset" )                 { lang = "Riinitialiser";     langchange = true; }
	if ( lang == "clear" )                 { lang = "Supprimer";         langchange = true; }
	if ( lang == "clear all")              { lang = "Tout Effacer";      langchange = true; }
	if ( lang == "import")                 { lang = "Importation";       langchange = true; }
	if ( lang == "export")                 { lang = "Exportation";       langchange = true; }
	if ( lang == "share")                  { lang = "Partages";          langchange = true; }
	if ( lang == "encode")                 { lang = "Encodage";          langchange = true; }
	if ( lang == "broadcast")              { lang = "Imettre";           langchange = true; }
	if ( lang == "load clip" )             { lang = "Ouvrir Clip";       langchange = true; }
	if ( lang == "play clip" )             { lang = "Joue Clip";         langchange = true; }
	if ( lang == "quit")                   { lang = "Quitter";           langchange = true; }
	if ( lang == "&quit")                  { lang = "&Quitter";          langchange = true; }

	if ( lang == "asset")                  { lang = "Propriitis";        langchange = true; }
	if ( lang == "utility")                { lang = "utils";             langchange = true; }
	if ( lang == "video player")           { lang = "Platine Vidio";     langchange = true; }

	if ( lang == "preferences")            { lang = "Preferences";       langchange = true; }
	if ( lang == "interface")              { lang = "Interface";         langchange = true; }
	if ( lang == "platinum")               { lang = "Platine";           langchange = true; }
	if ( lang == "grey")                   { lang = "Grise";             langchange = true; }
	if ( lang == "blue")                   { lang = "Bleue";             langchange = true; }
	if ( lang == "green")                  { lang = "Verte";             langchange = true; }
	if ( lang == "white")                  { lang = "Blanche";           langchange = true; }
	if ( lang == "metal")                  { lang = "Metallique";        langchange = true; }
	if ( lang == "charcoal")               { lang = "Fusain";            langchange = true; }
	if ( lang == "media storage")          { lang = "Chemin des clips";  langchange = true; }
	if ( lang == "set")                    { lang = "Enregistrer";       langchange = true; }
	if ( lang == "render quality")         { lang = "Qualiti de rendu";  langchange = true; }
	if ( lang == "startup screen")         { lang = "Icran de dimarrag"; langchange = true; }
	if ( lang == "dual head")              { lang = "Double icran";      langchange = true; }
	if ( lang == "video head")             { lang = "Sortie TV";         langchange = true; }
	//if ( lang == "remember interface")     { lang = "Sortie TV";         langchange = true; }

	if ( lang == "get desktop" )           { lang = "Get Bureau";        langchange = true; }  //importer du bureau (or importer du chutier) doesnt fit on button
	if ( lang == "&get desktop" )          { lang = "&Get Bureau";       langchange = true; }  //importer du bureau (or importer du chutier) doesnt fit on button
	if ( lang == "grab desktop" )          { lang = "Grab Bureau";       langchange = true; }  //importer du bureau (or importer du chutier) doesnt fit on button
	if ( lang == "empty layer" )           { lang = "Layer Vide";        langchange = true; }
	if ( lang == "name" )                  { lang = "Nom";               langchange = true; }
	if ( lang == "delete" )                { lang = "Effacer";           langchange = true; }
	if ( lang == "key" )                   { lang = "Cli";               langchange = true; }
	if ( lang == "photo" )                 { lang = "Photo";             langchange = true; }
	if ( lang == "render" )                { lang = "Rendu";             langchange = true; }
	
	if ( lang == "object" )                { lang = "Objet";             langchange = true; }
	if ( lang == "aliasing" )              { lang = "Aliasing";          langchange = true; }
	if ( lang == "grid" )                  { lang = "Grille";            langchange = true; }
	if ( lang == "title safe" )            { lang = "Zone titrage";      langchange = true; }
	if ( lang == "startframe" )            { lang = "Image dibut";       langchange = true; }
	if ( lang == "endframe" )              { lang = "Image fin";         langchange = true; }
	if ( lang == "axis" )                  { lang = "Axe";               langchange = true; }
	if ( lang == "rotate" )                { lang = "Rotation";          langchange = true; }
	if ( lang == "translate" )             { lang = "Translation";       langchange = true; }
	if ( lang == "clip" )                  { lang = "Clip";              langchange = true; }
	if ( lang == "media" )                 { lang = "Media";             langchange = true; }
	if ( lang == "invert" )                { lang = "Inverser";          langchange = true; }
	if ( lang == "frame in" )              { lang = "Point d'entrie";    langchange = true; }
	if ( lang == "frame out" )             { lang = "Point d'sortie";    langchange = true; }
	if ( lang == "foreground" )            { lang = "Premiere plan";     langchange = true; }
	if ( lang == "transparency" )          { lang = "Transparence";      langchange = true; }
	if ( lang == "add" )                   { lang = "Ajouter";           langchange = true; }
	if ( lang == "delete" )                { lang = "Suppr";             langchange = true; }
	if ( lang == "del" )                   { lang = "Del";               langchange = true; }
	if ( lang == "layers" )                { lang = "Couche";            langchange = true; }
	if ( lang == "settings")               { lang = "Ajustages";         langchange = true; }
	if ( lang == "motion blur")            { lang = "Flou de mouvement"; langchange = true; }
	if ( lang == "world")                  { lang = "Origine";           langchange = true; }
	
	if ( lang == "particle")               { lang = "Particules";        langchange = true; }
	if ( lang == "object")                 { lang = "Objet";             langchange = true; }
	if ( lang == "maxobject")              { lang = "Objet Max";         langchange = true; }
	if ( lang == "cylinder")               { lang = "Cylindre";          langchange = true; }
	if ( lang == "sphere")                 { lang = "Sphere";            langchange = true; }
	if ( lang == "bezier")                 { lang = "Bezier";            langchange = true; }
	if ( lang == "mesh")                   { lang = "Objet";             langchange = true; }
	if ( lang == "layer")                  { lang = "Couche";            langchange = true; }
	if ( lang == "cube")                   { lang = "Cube";              langchange = true; }
	if ( lang == "font")                   { lang = "Police";            langchange = true; }
	if ( lang == "color")                  { lang = "Couleur";           langchange = true; }

	if ( lang == "mirror")                 { lang = "Miroir";            langchange = true; }
	if ( lang == "swap rgb")               { lang = "Inversion RVB";     langchange = true; }
	if ( lang == "negative")               { lang = "Nigatif";           langchange = true; }
	if ( lang == "gaussian blur")          { lang = "Flou gaussien";     langchange = true; }
	if ( lang == "gaussian")               { lang = "Gaussien";          langchange = true; }
	if ( lang == "blur")                   { lang = "Flou";              langchange = true; }
	if ( lang == "&return")                { lang = "&Retourner";        langchange = true; }
	if ( lang == "return")                 { lang = "Retourner";         langchange = true; }

	if ( lang == "chat")                   { lang = "Causer";            langchange = true; }
	if ( lang == "server")                 { lang = "Serveur";           langchange = true; }
	if ( lang == "server ip")              { lang = "Ip du serveur";     langchange = true; }
	if ( lang == "connect")                { lang = "Connecter";         langchange = true; }
	if ( lang == "send")                   { lang = "Envoyer";           langchange = true; }
	if ( lang == "close")                  { lang = "Fermer";            langchange = true; }
	if ( lang == "send clip")              { lang = "Envoyer clip";      langchange = true; }
	
	if ( lang == "scene")                  { lang = "Scene";             langchange = true; }
	if ( lang == "keyframes")              { lang = "Clis d'animation";  langchange = true; }
	if ( lang == "render using source res") { lang = "Calcul de la source"; langchange = true; }

	if ( lang == "toggle mesh")            { lang = "A/R maillage";      langchange = true; }
	if ( lang == "zoom")                   { lang = "Zoom";              langchange = true; }
	if ( lang == "timeline")               { lang = "Montage";           langchange = true; }
	if ( lang == "tools")                  { lang = "Outils";            langchange = true; }
	if ( lang == "draw objects")           { lang = "Outils Dessi";      langchange = true; }
	if ( lang == "brushes")                { lang = "Brosses";           langchange = true; }
	if ( lang == "brush size")             { lang = "Taille Brosse";     langchange = true; }
	if ( lang == "invert")                 { lang = "Inversion";         langchange = true; }
	if ( lang == "fade")                   { lang = "Fondu";             langchange = true; }
	
	if ( lang == "contrast")               { lang = "contraste";         langchange = true; }
	if ( lang == "negative")               { lang = "nigatif";           langchange = true; }
	if ( lang == "background")             { lang = "arrihre plan";      langchange = true; }
	if ( lang == "graphic")                { lang = "Image";             langchange = true; }
	if ( lang == "pen")                    { lang = "crayon";            langchange = true; }
	if ( lang == "fill")                   { lang = "remplissage";       langchange = true; }
	if ( lang == "gradient angle")         { lang = "angle du digradi";  langchange = true; }
	if ( lang == "intensify")              { lang = "intensifier";       langchange = true; }
	if ( lang == "sharpen")                { lang = "affiner";           langchange = true; }
	
	//no translation found
	if (!langchange)                       { lang = text; } 
	
    return lang;
}

