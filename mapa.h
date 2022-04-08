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

struct rock{
  int x, y;
  int caindo;
};
typedef struct rock rock;

struct tObjetos{
  rock *rock;
  rock *diamond;
  ALLEGRO_BITMAP* terra;
  ALLEGRO_BITMAP* muro;
  ALLEGRO_BITMAP* metal;
  ALLEGRO_BITMAP* pedra;
  ALLEGRO_BITMAP* vazio;
  ALLEGRO_BITMAP* saida;
  ALLEGRO_BITMAP* diamante[8];
  ALLEGRO_BITMAP* explosao[4];
  int qtPedras, qtDiamantes;
  int animacao;
};
typedef struct tObjetos tObjetos;

struct infoMapa
{
    char code[8];
    // Flags de movimento
    bool keys[4];
    int pontuacao;
    int vidas; 
    int diamantes;
};
typedef struct infoMapa infoMapa;

tObjetos* iniciaObjetos(ALLEGRO_BITMAP* sheet); 
void draw_map(int** mapa, tObjetos* objetosMapa, long frames, int col, int lin);
int** iniciaMapa(char* nome_mapa, tObjetos* obj);

#endif