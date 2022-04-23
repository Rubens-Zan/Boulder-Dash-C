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
  
  jogador->vel = 1;
  jogador->animacaoAtual = jogador->pontuacao = jogador->diamantes = 0;
  jogador->tired = 0;
  jogador->vidas = 7; 
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
  obj->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  obj->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  obj->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  obj->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  obj->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  obj->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  obj->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);

  obj->amoeba[0] = al_create_sub_bitmap(sheet, 64, 64, 15, 16);
  obj->amoeba[1] = al_create_sub_bitmap(sheet, 80, 64, 15, 16);
  obj->amoeba[2] = al_create_sub_bitmap(sheet, 64, 80, 15, 16);
  obj->amoeba[3] = al_create_sub_bitmap(sheet, 80, 80, 15, 16);
  obj->amoeba[4] = al_create_sub_bitmap(sheet, 64, 96, 15, 16);
  obj->amoeba[5] = al_create_sub_bitmap(sheet, 80, 96, 15, 16);
  obj->amoeba[6] = al_create_sub_bitmap(sheet, 64, 112, 15, 16);
  obj->amoeba[7] = al_create_sub_bitmap(sheet, 80, 112, 15, 16);

  obj->borboleta[0] = al_create_sub_bitmap(sheet, 96, 64, 15, 16);
  obj->borboleta[1] = al_create_sub_bitmap(sheet, 96, 80, 15, 16);
  obj->borboleta[2] = al_create_sub_bitmap(sheet, 96, 96, 15, 16);
  obj->borboleta[3] = al_create_sub_bitmap(sheet, 96, 112, 15, 16);

  obj->vagalume[0] = al_create_sub_bitmap(sheet, 80, 64, 15, 16);
  obj->vagalume[1] = al_create_sub_bitmap(sheet, 80, 80, 15, 16);
  obj->vagalume[2] = al_create_sub_bitmap(sheet, 80, 96, 15, 16);
  obj->vagalume[3] = al_create_sub_bitmap(sheet, 80, 112, 15, 16);
  
}

// INICIA SPRITES DO JOGADOR
void iniciaSpritesJogador(ALLEGRO_BITMAP* sheet, tPlayer* jogador){
  for(int i = 0;i <= 6;i++)
    jogador->animacaoParado[i] = al_create_sub_bitmap(sheet, (16 * i), 0, 15, 16);	

  for(int i = 0;i <= 6;i++)
    jogador->animacaoEsq[i] = al_create_sub_bitmap(sheet, (16 * i), 16, 15, 16);	

  for(int i = 0;i <= 6;i++)
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