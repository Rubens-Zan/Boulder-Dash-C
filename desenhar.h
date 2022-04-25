#ifndef __DESENHAR__
#define __DESENHAR__


#include "mapa.h"
#include "game.h"
#include "texturas.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


// FUNCOES DE DESENHO 
void desenhaTela(bool redraw, long frames, tNivel *infoNivel, tGame *game);
void desenhaJogador(tTexturas *texturas, int lin,int col,int direcao,int animacaoAtual);
void desenhaInstrucoes(ALLEGRO_FONT *fonte); 
void desenhaMapa(int **mapa, tObjetos *objetosMapa, long frames, int col, int lin, tTexturas *texturas);
void desenhaHeader(ALLEGRO_FONT *fonte, int relogio, int vidas, int pontuacao, int diamantesJogador, int diamantesTotal,tTexturas *texturas, int nivel);
void drawInicialMenu();
#endif