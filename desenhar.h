#ifndef __DESENHAR__
#define __DESENHAR__


#include "mapa.h"
#include "texturas.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


// FUNCOES DE DESENHO 
void draw(bool redraw, long frames,tNivel *infoNivel);
void drawPlayer(tPlayer *jogador, int** mapa, tObjetos *obj, long frames); 
void drawInstructions(); 
void drawHeader(ALLEGRO_FONT *fonte, int relogio, int vidas,int pontuacao,int diamantesJogador,int diamantesTotal, ALLEGRO_BITMAP *diamanteTextura);
void drawInicialMenu();
void drawMap(int** mapa, tObjetos* objetosMapa, long frames, int col, int lin);
#endif