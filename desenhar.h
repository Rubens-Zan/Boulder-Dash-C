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
void drawHeader(ALLEGRO_FONT *fonte, int relogio, int vidas, ALLEGRO_BITMAP *diamanteTextura);
void drawInicialMenu();

#endif