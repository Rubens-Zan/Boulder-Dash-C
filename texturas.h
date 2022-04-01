#ifndef __SPRITES__
#define __SPRITES__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mapa.h"
// #include "player.h"


void carregarTexturas(ALLEGRO_BITMAP *texture[10]); 

ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h);
ALLEGRO_BITMAP * inicializaTexturas(int level); 
void iniciaSpritesObjetos(ALLEGRO_BITMAP* sheet, objetos* obj);
// void iniciaSpritesJogador(ALLEGRO_BITMAP* sheet, player* jogador);
void destroiTexturasObjetos(objetos* obj);
// void destroiSpritesPlayer(player* jogador);
#endif