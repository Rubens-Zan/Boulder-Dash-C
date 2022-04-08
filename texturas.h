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
void iniciaSpritesObjetos(ALLEGRO_BITMAP* sheet, tObjetos* obj);
void iniciaSpritesJogador(ALLEGRO_BITMAP* sheet, tPlayer* jogador);
void destroiSpritesObjetos(tObjetos* obj);
void destroi_sprites_player(tPlayer* jogador);

#endif