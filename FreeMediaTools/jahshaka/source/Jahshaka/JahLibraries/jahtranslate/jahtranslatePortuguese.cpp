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

QString JahTranslate::translatePortuguese(QString text)
    throw()
{
	QString lang = text.lower();
	bool langchange = false;

	//our slogan :)
	if ( lang == "jahshaka media intergrator")  
	{ lang = "Integrador de mídias Jahshaka"; langchange = true; }

	//main translations
	if ( lang == "english" )               { lang = "English";    langchange = true; }
	if ( lang == "french" )                { lang = "French";     langchange = true; }
	if ( lang == "portuguese" )            { lang = "Portuguese"; langchange = true; }
	if ( lang == "spanish" )               { lang = "Espanol";    langchange = true; }

	//language translations
	if ( lang == "&animation" )            { lang = "&Animação";        langchange = true; }
	if ( lang == "animation" )             { lang = "Animação";         langchange = true; }
	if ( lang == "&editing" )              { lang = "&Edição";          langchange = true; }
	if ( lang == "editing" )               { lang = "Edição";           langchange = true; }
	if ( lang == "e&ffects" )              { lang = "E&feitos";         langchange = true; }
	if ( lang == "effects" )               { lang = "Efeitos";          langchange = true; }
	if ( lang == "&paint" )                { lang = "&Pintura";         langchange = true; }
	if ( lang == "paint" )                 { lang = "Pintura";          langchange = true; }
	if ( lang == "&text cg" )              { lang = "&Texto";           langchange = true; }
	if ( lang == "text cg" )               { lang = "Texto";            langchange = true; }
	if ( lang == "&desktop" )              { lang = "&Desktop";         langchange = true; }
	if ( lang == "network" )               { lang = "Rede";             langchange = true; }
	if ( lang == "colorize")               { lang = "Colorir";          langchange = true; }

	if ( lang == "load" )                  { lang = "Carregar";         langchange = true; }
	if ( lang == "save" )                  { lang = "Salvar";           langchange = true; }
	if ( lang == "reset" )                 { lang = "Reajustar";        langchange = true; }
	if ( lang == "clear" )                 { lang = "Limpar";           langchange = true; }
	if ( lang == "clear all")              { lang = "Limpar tudo";      langchange = true; }
	if ( lang == "import")                 { lang = "Importar";         langchange = true; }
	if ( lang == "export")                 { lang = "Exportar";         langchange = true; }
	if ( lang == "share")                  { lang = "Compartilhar";     langchange = true; }
	if ( lang == "encode")                 { lang = "Codificar";        langchange = true; }
	if ( lang == "broadcast")              { lang = "Transmitir";       langchange = true; }
	if ( lang == "load clip" )             { lang = "Carregar Clipe";   langchange = true; }
	if ( lang == "play clip" )             { lang = "Rodar Clipe";      langchange = true; }
	if ( lang == "quit")                   { lang = "Sair";             langchange = true; }
	if ( lang == "&quit")                  { lang = "&Sair";            langchange = true; }
	if ( lang == "&return")                { lang = "&Retornar";        langchange = true; }
	if ( lang == "return")                 { lang = "Retornar";         langchange = true; }

	if ( lang == "asset")                  { lang = "Mídia";            langchange = true; }
	if ( lang == "utility")                { lang = "Utilidade";        langchange = true; }
	if ( lang == "video player")           { lang = "Leitor de Vídeo";  langchange = true; }

	if ( lang == "preferences")            { lang = "Preferência";      langchange = true; }
	//if ( lang == "interface")              { lang = "Interface";         langchange = true; }
	if ( lang == "platinum")               { lang = "Platina";          langchange = true; }
	if ( lang == "grey")                   { lang = "Cinza";            langchange = true; }
	if ( lang == "blue")                   { lang = "Azul";             langchange = true; }
	if ( lang == "green")                  { lang = "Verde";            langchange = true; }
	if ( lang == "white")                  { lang = "Branco";           langchange = true; }
	//if ( lang == "metal")                  { lang = "Metallique";        langchange = true; }
	if ( lang == "charcoal")               { lang = "Carvão";           langchange = true; }
	if ( lang == "media storage")          { lang = "Directório Media";  langchange = true; }
	if ( lang == "set")                    { lang = "Ajustar";          langchange = true; }
	if ( lang == "render quality")         { lang = "Qualidade de renderização";  langchange = true; }
	if ( lang == "startup screen")         { lang = "Tela de início";   langchange = true; }
	if ( lang == "dual head")              { lang = "Tela Dupla";       langchange = true; }
	if ( lang == "video head")             { lang = "Tela de vídeo";    langchange = true; }
	if ( lang == "remember interface")     { lang = "Lembrar interface"; langchange = true; }

	//if ( lang == "get desktop" )           { lang = "Get Desktop";        langchange = true; }  //importar do desktop 
	//if ( lang == "grab desktop" )          { lang = "Grab Bureau";       langchange = true; }  
	//if ( lang == "empty layer" )           { lang = "Layer Vide";        langchange = true; }
	if ( lang == "name" )                  { lang = "Nome";             langchange = true; }
	if ( lang == "delete" )                { lang = "Apagar";           langchange = true; }
	if ( lang == "key" )                   { lang = "Chave";            langchange = true; }
	//if ( lang == "photo" )                 { lang = "Photo";             langchange = true; }  //salvar quadro too long
	if ( lang == "render" )                { lang = "Renderizar";       langchange = true; }
	
	if ( lang == "object" )                { lang = "Objecto";           langchange = true; }
	//if ( lang == "aliasing" )              { lang = "Aliasing";          langchange = true; }
	if ( lang == "grid" )                  { lang = "Grade";            langchange = true; }
	if ( lang == "title safe" )            { lang = "área de segurança"; langchange = true; }
	if ( lang == "startframe" )            { lang = "Quadro inicial";   langchange = true; }
	if ( lang == "endframe" )              { lang = "Quadro final";     langchange = true; }
	if ( lang == "axis" )                  { lang = "Eixo";             langchange = true; }
	if ( lang == "rotate" )                { lang = "Rotacionar";       langchange = true; }
	if ( lang == "translate" )             { lang = "Traduzir";         langchange = true; }
	if ( lang == "clip" )                  { lang = "Clipe ";           langchange = true; }
	if ( lang == "media" )                 { lang = "Mídia";            langchange = true; }
	if ( lang == "invert" )                { lang = "Inverter";         langchange = true; }
	if ( lang == "frame in" )              { lang = "Ponto entrada";    langchange = true; }
	if ( lang == "frame out" )             { lang = "Ponto saída";      langchange = true; }
	if ( lang == "foreground" )            { lang = "Primeiro plano";   langchange = true; }
	if ( lang == "transparency" )          { lang = "Transparência";    langchange = true; }
	if ( lang == "add" )                   { lang = "Adicionar";        langchange = true; }
	if ( lang == "delete" )                { lang = "Retirar";          langchange = true; }
	//if ( lang == "del" )                   { lang = "Del";               langchange = true; }
	if ( lang == "layers" )                { lang = "Camadas";          langchange = true; }
	if ( lang == "settings")               { lang = "Ajustes";          langchange = true; }
	if ( lang == "motion blur")            { lang = "Desfocagem de movimento"; langchange = true; }
	if ( lang == "world")                  { lang = "Mundo";            langchange = true; }
	
	if ( lang == "particle")               { lang = "Partículas";       langchange = true; }
	//if ( lang == "object")                 { lang = "Objet";             langchange = true; }
	//if ( lang == "maxobject")              { lang = "Objet Max";         langchange = true; }
	if ( lang == "cylinder")               { lang = "Cilindro";         langchange = true; }
	if ( lang == "sphere")                 { lang = "Esfera";           langchange = true; }
	//if ( lang == "bezier")                 { lang = "Bezier";            langchange = true; }
	//if ( lang == "mesh")                   { lang = "Mesh";             langchange = true; }
	if ( lang == "layer")                  { lang = "Camada";           langchange = true; }
	if ( lang == "cube")                   { lang = "Cubo";             langchange = true; }
	if ( lang == "font")                   { lang = "Fonte";            langchange = true; }
	if ( lang == "color")                  { lang = "Cor";              langchange = true; }

	if ( lang == "mirror")                 { lang = "Espelhar";         langchange = true; }
	if ( lang == "swap rgb")               { lang = "Inverter RGB";     langchange = true; }
	if ( lang == "negative")               { lang = "Negativo";         langchange = true; }
	if ( lang == "gaussian blur")          { lang = "Desfoque Gaussiano"; langchange = true; }
	if ( lang == "gaussian")               { lang = "Gaussiano";        langchange = true; }
	if ( lang == "blur")                   { lang = "Desfoque";         langchange = true; }
	//if ( lang == "return")                 { lang = "Retourner";         langchange = true; }

	if ( lang == "chat")                   { lang = "Chat";             langchange = true; }
	if ( lang == "server")                 { lang = "Servidor";         langchange = true; }
	if ( lang == "server ip")              { lang = "IP do servidor";   langchange = true; }
	if ( lang == "connect")                { lang = "Conectar";         langchange = true; }
	if ( lang == "send")                   { lang = "Enviar";           langchange = true; }
	if ( lang == "close")                  { lang = "Fechar";           langchange = true; }
	if ( lang == "send clip")              { lang = "Enviar clipe";     langchange = true; }
	
	if ( lang == "scene")                  { lang = "Cena";             langchange = true; }
	if ( lang == "keyframes")              { lang = "Quadros-chave";    langchange = true; }
	if ( lang == "render using source res") { lang = "Fonte de render"; langchange = true; }

	//if ( lang == "toggle mesh")            { lang = "A/R maillage";      langchange = true; }
	//if ( lang == "zoom")                   { lang = "Zoom";              langchange = true; }
	if ( lang == "timeline")               { lang = "Linha de tempo";   langchange = true; }
	if ( lang == "tools")                  { lang = "Ferramentas";      langchange = true; }
	//if ( lang == "draw objects")           { lang = "Outils Dessi";      langchange = true; }
	if ( lang == "brushes")                { lang = "Pincéis";          langchange = true; }
	if ( lang == "brush size")             { lang = "Tamanho do pincel";     langchange = true; }
	if ( lang == "invert")                 { lang = "Inverter";         langchange = true; }
	//if ( lang == "fade")                   { lang = "Fondu";             langchange = true; }
	
	if ( lang == "contrast")               { lang = "Contraste";        langchange = true; }
	if ( lang == "negative")               { lang = "Negativo";         langchange = true; }
	if ( lang == "background")             { lang = "Plano de fundo";   langchange = true; }
	if ( lang == "graphic")                { lang = "Gráfico";          langchange = true; }
	if ( lang == "pen")                    { lang = "Contorno";         langchange = true; }
	if ( lang == "fill")                   { lang = "Preencher";        langchange = true; }
	if ( lang == "gradient angle")         { lang = "Ângulo de gradiente";  langchange = true; }
	if ( lang == "intensify")              { lang = "Intensificar";     langchange = true; }
	if ( lang == "sharpen")                { lang = "Realçar";          langchange = true; }
		
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
