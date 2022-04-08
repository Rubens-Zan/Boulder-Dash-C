// #ifndef __SPRITES__
// #define __SPRITES__

// #include <allegro5/allegro5.h>
// #include <allegro5/allegro_font.h>
// #include <allegro5/allegro_primitives.h>
// #include <allegro5/allegro_audio.h>
// #include <allegro5/allegro_acodec.h>
// #include <allegro5/allegro_image.h>
// #include "game.h"
// #include "mapa.h"
// // #include "player.h"


// void carregarTexturas(ALLEGRO_BITMAP *texture[10]); 

// ALLEGRO_BITMAP *load_bitmap_at_size(char *filename, int w, int h);
// ALLEGRO_BITMAP * inicializaTexturas(int level); 
// void iniciaSpritesObjetos(ALLEGRO_BITMAP* sheet, tObjetos* obj);
// // void iniciaSpritesJogador(ALLEGRO_BITMAP* sheet, player* jogador);
// void destroiTexturasObjetos(tObjetos* obj);
// // void destroiSpritesPlayer(player* jogador);
// #endif


#ifndef __TEXTURAS__
#define __TEXTURAS__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "mapa.h"

tPlayer* inicia_jogador(ALLEGRO_BITMAP* sheet); 
void inicia_sprites_objetos(ALLEGRO_BITMAP* sheet, tObjetos* obj);
void inicia_sprites_jogador(ALLEGRO_BITMAP* sheet, tPlayer* jogador);
void destroi_sprites_objetos(tObjetos* obj);
void destroi_sprites_player(tPlayer* jogador);
#endif