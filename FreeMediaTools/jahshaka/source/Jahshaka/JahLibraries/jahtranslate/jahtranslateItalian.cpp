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

QString JahTranslate::translateItalian(QString text)
    throw()
{
	QString lang = text.lower();
	bool langchange = false;

	//our slogan :)
	if ( lang == "jahshaka media intergrator")  
	{ lang = "Jashaka, l'integratore multimediale"; langchange = true; }

	//main translations
	if ( lang == "english" )               { lang = "English";    langchange = true; }
	if ( lang == "french" )                { lang = "French";     langchange = true; }
	if ( lang == "portuguese" )            { lang = "Portuguese"; langchange = true; }
	if ( lang == "spanish" )               { lang = "Espanol";    langchange = true; }
	if ( lang == "italian" )               { lang = "Italian";    langchange = true; }

	//language translations
	if ( lang == "&animation" )            { lang = "&Animazione";        langchange = true; }
	if ( lang == "animation" )             { lang = "Animazione";         langchange = true; }
	if ( lang == "&editing" )              { lang = "&Modifica";          langchange = true; }
	if ( lang == "editing" )               { lang = "Modifica";           langchange = true; }
	if ( lang == "e&ffects" )              { lang = "E&ffetti";         langchange = true; }
	if ( lang == "effects" )               { lang = "Effetti";          langchange = true; }
	if ( lang == "&paint" )                { lang = "&Dipingi";         langchange = true; }
	if ( lang == "paint" )                 { lang = "Dipingi";          langchange = true; }
	if ( lang == "&text cg" )              { lang = "&Testo";           langchange = true; }
	if ( lang == "text cg" )               { lang = "Testo";            langchange = true; }
	if ( lang == "&desktop" )              { lang = "&Scrivania";         langchange = true; }
	if ( lang == "network" )               { lang = "Rete";             langchange = true; }
	if ( lang == "colorize")               { lang = "Colorazione";          langchange = true; }

	if ( lang == "load" )                  { lang = "Carica";         langchange = true; }
	if ( lang == "save" )                  { lang = "Salva";           langchange = true; }
	if ( lang == "reset" )                 { lang = "Azzera";        langchange = true; }
	if ( lang == "clear" )                 { lang = "Rimuovi";           langchange = true; }
	if ( lang == "clear all")              { lang = "Rimuovi tutti";      langchange = true; }
	if ( lang == "import")                 { lang = "Importa";         langchange = true; }
	if ( lang == "export")                 { lang = "Esporta";         langchange = true; }
	if ( lang == "share")                  { lang = "Condividi";     langchange = true; }
	if ( lang == "encode")                 { lang = "Codifica";        langchange = true; }
	if ( lang == "broadcast")              { lang = "Annuncia";       langchange = true; }
	if ( lang == "load clip" )             { lang = "Carica clip";   langchange = true; }
	if ( lang == "play clip" )             { lang = "Avvia clip";      langchange = true; }
	if ( lang == "quit")                   { lang = "Es&ci";             langchange = true; }
	if ( lang == "&return")                { lang = "&Ritorna";         langchange = true; }
	if ( lang == "return")                 { lang = "Ritorna";         langchange = true; }

	if ( lang == "asset")                  { lang = "Media";            langchange = true; }
	if ( lang == "utility")                { lang = "Utilita'";        langchange = true; }
	if ( lang == "video player")           { lang = "Lettore video";  langchange = true; }

	if ( lang == "preferences")            { lang = "Preference";      langchange = true; }
	//if ( lang == "interface")              { lang = "Interfaccia";         langchange = true; }
	if ( lang == "platinum")               { lang = "Platino";          langchange = true; }
	if ( lang == "grey")                   { lang = "Grigio";            langchange = true; }
	if ( lang == "blue")                   { lang = "Blu";             langchange = true; }
	if ( lang == "green")                  { lang = "Verde";            langchange = true; }
	if ( lang == "white")                  { lang = "Bianco";           langchange = true; }
	//if ( lang == "metal")                  { lang = "Metallico";        langchange = true; }
	if ( lang == "charcoal")               { lang = "Carbone";           langchange = true; }
	if ( lang == "media storage")          { lang = "Reposito di media";  langchange = true; }
	if ( lang == "set")                    { lang = "Imposta";          langchange = true; }
	if ( lang == "render quality")         { lang = "Qualita' di rendering";  langchange = true; }
	if ( lang == "startup screen")         { lang = "Schermo d'avvio";   langchange = true; }
	if ( lang == "dual head")              { lang = "Doppio schermo";       langchange = true; }
	if ( lang == "video head")             { lang = "Schermo video";    langchange = true; }
	if ( lang == "remember interface")     { lang = "Ricorda interfaccia"; langchange = true; }

	//if ( lang == "get desktop" )           { lang = "Prendi scrivania";        langchange = true; }  //importar do desktop 
	//if ( lang == "grab desktop" )          { lang = "Prendi scrivania";       langchange = true; }  
	//if ( lang == "empty layer" )           { lang = "Svuota strato";        langchange = true; }
	if ( lang == "name" )                  { lang = "Nome";             langchange = true; }
	if ( lang == "delete" )                { lang = "Elimina";           langchange = true; }
	if ( lang == "key" )                   { lang = "Chiave";            langchange = true; }
	//if ( lang == "photo" )                 { lang = "Foto";             langchange = true; }  //salvar quadro too long
	if ( lang == "render" )                { lang = "Render";       langchange = true; }
	
	if ( lang == "object" )                { lang = "Oggetto";           langchange = true; }
	//if ( lang == "aliasing" )              { lang = "Aliasing";          langchange = true; }
	if ( lang == "grid" )                  { lang = "Griglia";            langchange = true; }
	if ( lang == "title safe" )            { lang = "Titolo sicuro"; langchange = true; }
	if ( lang == "startframe" )            { lang = "Primo quadro";   langchange = true; }
	if ( lang == "endframe" )              { lang = "Ultimo quadro";     langchange = true; }
	if ( lang == "axis" )                  { lang = "Asse";             langchange = true; }
	if ( lang == "rotate" )                { lang = "Rotazione";       langchange = true; }
	if ( lang == "translate" )             { lang = "Transduzione";         langchange = true; }
	if ( lang == "clip" )                  { lang = "Clip";           langchange = true; }
	if ( lang == "media" )                 { lang = "Media";            langchange = true; }
	if ( lang == "invert" )                { lang = "Inverti";         langchange = true; }
	if ( lang == "frame in" )              { lang = "Quadro ingresso";    langchange = true; }
	if ( lang == "frame out" )             { lang = "Quadro uscita";      langchange = true; }
	if ( lang == "foreground" )            { lang = "Primo piano";   langchange = true; }
	if ( lang == "transparency" )          { lang = "Trasparenza";    langchange = true; }
	if ( lang == "add" )                   { lang = "Aggiungi";        langchange = true; }
	if ( lang == "delete" )                { lang = "Elimina";          langchange = true; }
	//if ( lang == "del" )                   { lang = "Del";               langchange = true; }
	if ( lang == "layers" )                { lang = "Strati";          langchange = true; }
	if ( lang == "settings")               { lang = "Impostazioni";          langchange = true; }
	if ( lang == "motion blur")            { lang = "Sfocamento di movimento"; langchange = true; }
	if ( lang == "world")                  { lang = "Mondo";            langchange = true; }
	
	if ( lang == "particle")               { lang = "Particelle";       langchange = true; }
	//if ( lang == "object")                 { lang = "Oggetto";             langchange = true; }
	//if ( lang == "maxobject")              { lang = "MaxObject";         langchange = true; }
	if ( lang == "cylinder")               { lang = "Cilindro";         langchange = true; }
	if ( lang == "sphere")                 { lang = "Sfera";           langchange = true; }
	//if ( lang == "bezier")                 { lang = "Bezier";            langchange = true; }
	//if ( lang == "mesh")                   { lang = "Mesh";             langchange = true; }
	if ( lang == "layer")                  { lang = "Strato";           langchange = true; }
	if ( lang == "cube")                   { lang = "Cubo";             langchange = true; }
	if ( lang == "font")                   { lang = "Carattere";            langchange = true; }
	if ( lang == "color")                  { lang = "Colore";              langchange = true; }

	if ( lang == "mirror")                 { lang = "Specchio";         langchange = true; }
	if ( lang == "swap rgb")               { lang = "Inverti RGB";     langchange = true; }
	if ( lang == "negative")               { lang = "Negativo";         langchange = true; }
	if ( lang == "gaussian blur")          { lang = "Sfocamento Gaussiano"; langchange = true; }
	if ( lang == "gaussian")               { lang = "Gaussiano";        langchange = true; }
	if ( lang == "blur")                   { lang = "Sfoca";         langchange = true; }
	//if ( lang == "return")                 { lang = "Ritorno";         langchange = true; }

	if ( lang == "chat")                   { lang = "Dialoga";             langchange = true; }
	if ( lang == "server")                 { lang = "Server";         langchange = true; }
	if ( lang == "server ip")              { lang = "Indirizzo Server";   langchange = true; }
	if ( lang == "connect")                { lang = "Connetti";         langchange = true; }
	if ( lang == "send")                   { lang = "Invia";           langchange = true; }
	if ( lang == "close")                  { lang = "Chiudi";           langchange = true; }
	if ( lang == "send clip")              { lang = "Invia clip";       langchange = true; }
	if ( lang == "scene")                  { lang = "Scena";             langchange = true; }
	if ( lang == "keyframes")              { lang = "Quadro chiave";    langchange = true; }
	if ( lang == "render using source res") { lang = "Rendi usando la risoluzione sorgente"; langchange = true; }

	//if ( lang == "toggle mesh")            { lang = "A/R maillage";      langchange = true; }
	//if ( lang == "zoom")                   { lang = "Zoom";              langchange = true; }
	if ( lang == "timeline")               { lang = "Linea temporale";   langchange = true; }
	if ( lang == "tools")                  { lang = "Arnesi";      langchange = true; }
	//if ( lang == "draw objects")           { lang = "Outils Dessi";      langchange = true; }
	if ( lang == "brushes")                { lang = "Pennelli";          langchange = true; }
	if ( lang == "brush size")             { lang = "Dimensione pennelli";     langchange = true; }
	if ( lang == "invert")                 { lang = "Inverti";         langchange = true; }
	//if ( lang == "fade")                   { lang = "Fondu";             langchange = true; }
	
	if ( lang == "contrast")               { lang = "Contrasto";        langchange = true; }
	if ( lang == "negative")               { lang = "Negativo";         langchange = true; }
	if ( lang == "background")             { lang = "Sfondo";   langchange = true; }
	if ( lang == "graphic")                { lang = "Grafica";          langchange = true; }
	if ( lang == "pen")                    { lang = "Penna";         langchange = true; }
	if ( lang == "fill")                   { lang = "Riempi";        langchange = true; }
	if ( lang == "gradient angle")         { lang = "Gradiente d'angolo";  langchange = true; }
	if ( lang == "intensify")              { lang = "Intensifica";     langchange = true; }
	if ( lang == "sharpen")                { lang = "Appuntisci";          langchange = true; }
		
	//no translation found
	if (!langchange)                       { lang = text; } 
	
    return lang;

}
/*
Portuguese translation

Empty layer		> Camada vazia

animation Module	> módulo de animação
Effects Module		> Módulo de efeitos
Editing Module		> Módulo de Edição
Paint Module		> Módulo de pintura
Text Module		> Módulo de texto

Frame in		> ponto de entrada
Video in/out		> Entrada/saída de video
*/
