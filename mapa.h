#ifndef __MAPA__
#define __MAPA__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define METAL     0
#define TERRA     1
#define MURO      2
#define PEDRA     3
#define DIAMANTE  4
#define VAZIO     5
#define PLAYER    6
#define EXPLOSAO  7
#define EXPLOSAO2 8
#define EXPLOSAO3 9
#define SAIDA     10

#define ROWS 22
#define COLS 40

typedef struct{
  int x, y,caindo,tipo;
  bool ativo; 
} rochedos;

typedef struct{
  rochedos *rochedos;
  rochedos *diamond;
  ALLEGRO_BITMAP* terra;
  ALLEGRO_BITMAP* muro;
  ALLEGRO_BITMAP* metal;
  ALLEGRO_BITMAP* pedra;
  ALLEGRO_BITMAP* vazio;
  ALLEGRO_BITMAP* saida;
  ALLEGRO_BITMAP* diamante[8];
  ALLEGRO_BITMAP* explosao[4];
  int qtPedras, qtDiamantes,total;
  int animacao;
}tObjetos;

typedef struct{
  int lin,col; 
  char code[8];
  // Flags de movimento
  bool moveKeys[4];
  int pontuacao;
  int vidas; 
  int diamantes;
} infoMapa;

// FUNCOES
tObjetos* iniciaObjetos(ALLEGRO_BITMAP* sheet); 
int** iniciaMapa(char* nome_mapa, tObjetos* obj);
void iniciaPedrasEDiamantes(int **mapa,tObjetos *objetos); 
#endif