#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "texturas.h"

tPlayer* inicia_jogador(ALLEGRO_BITMAP* sheet){
  tPlayer *jogador;
  jogador = malloc(sizeof(tPlayer));
  if(jogador == NULL){
  	printf("Erro ao alocar memoria!\n");
  	exit(1);
  }
  jogador->posX = SPAWN_X;
  jogador->posY = SPAWN_Y;
  jogador->vel = SIZE_OBJS/4;
  jogador->animacaoAtual=0;
  jogador->tired = 0;
  jogador->vivo = 1;
  iniciaSpritesJogador(sheet, jogador);
  return jogador;
}

// CRIA OBJETO CONTENDO AS SPRITES DOS OBJETOS
void iniciaSpritesObjetos(ALLEGRO_BITMAP* sheet, tObjetos* obj){
  obj->metal = al_create_sub_bitmap(sheet, 0, 48, 15, 16);
  obj->saida = al_create_sub_bitmap(sheet, 16, 48, 15, 16);
  obj->muro  = al_create_sub_bitmap(sheet, 32, 48, 15, 16);
  obj->terra = al_create_sub_bitmap(sheet, 48, 48, 15, 16);
  obj->pedra = al_create_sub_bitmap(sheet, 80, 48, 15, 16);
  obj->vazio = al_create_sub_bitmap(sheet, 96, 48, 15, 16);

  obj->explosao[3] = al_create_sub_bitmap(sheet, 96, 48, 15, 16);
  obj->explosao[2] = al_create_sub_bitmap(sheet, 112, 64, 15, 16);
  obj->explosao[1] = al_create_sub_bitmap(sheet, 128, 64, 15, 16);
  obj->explosao[0] = al_create_sub_bitmap(sheet, 128, 80, 15, 16);

  // array para efetuar animacao do diamante
  obj->diamante[0] = al_create_sub_bitmap(sheet, 0, 64, 15, 16);
  obj->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  obj->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  obj->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  obj->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  obj->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  obj->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  obj->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);
}

// INICIA SPRITES DO JOGADOR
void iniciaSpritesJogador(ALLEGRO_BITMAP* sheet, tPlayer* jogador){
  jogador->animacaoParado[0] = al_create_sub_bitmap(sheet, 0, 0, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->animacaoParado[i] = al_create_sub_bitmap(sheet, (16 * i), 0, 15, 16);	

  jogador->animacaoEsq[0] = al_create_sub_bitmap(sheet, 0, 16, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->animacaoEsq[i] = al_create_sub_bitmap(sheet, (16 * i), 16, 15, 16);	

  jogador->animacaoDir[0] = al_create_sub_bitmap(sheet, 0, 32, 15, 16);
  for(int i = 1;i <= 6;i++)
    jogador->animacaoDir[i] = al_create_sub_bitmap(sheet, (16 * i), 32, 15, 16);
}

void destroiSpritesObjetos(tObjetos* obj){
  al_destroy_bitmap(obj->metal);
  al_destroy_bitmap(obj->saida);
  al_destroy_bitmap(obj->muro);
  al_destroy_bitmap(obj->terra);
  al_destroy_bitmap(obj->pedra);
  al_destroy_bitmap(obj->vazio);

  for(int i = 0;i < 4;i++)
    al_destroy_bitmap(obj->explosao[i]);

  for(int i = 0;i < 8;i++)
    al_destroy_bitmap(obj->diamante[i]);
}

void destroi_sprites_player(tPlayer* jogador){
  for(int i = 0;i < 7;i++)
    al_destroy_bitmap(jogador->animacaoParado[i]);

  for(int i = 0;i < 7;i++)
    al_destroy_bitmap(jogador->animacaoEsq[i]);

  for(int i = 0;i < 7;i++)
    al_destroy_bitmap(jogador->animacaoDir[i]);
}