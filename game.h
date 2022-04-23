#ifndef __GAME__
#define __GAME__

#include <allegro5/allegro5.h>
#include "mapa.h"

#define SIZE_OBJS 30
#define MARGIN_TOP 30
#define WIDTH (40 * SIZE_OBJS)
#define HEIGHT ((22 * SIZE_OBJS) + MARGIN_TOP)
#define FRAMERATE 60.0
#define TEMPO_RESET 105
#define KEY_SEEN 1
#define KEY_RELEASED 2

#define PATH_MAP_1 "resources/mapas/mapa8.txt"
#define MIN_DIAMANTES 12
#define SIZE_ARQ_PONTOS 5
#define PATH_SPRITESHEET "resources/img/spritesheet.png"

// #define METAL     0 W
// #define TERRA     1 .
// #define MURO      2 w
// #define PEDRA     3 r
// #define DIAMANTE  4 d
// #define VAZIO     5 ' '
// #define PLAYER    6 P
// #define SAIDA     10 X
// #define FIREFLY   11 q
// #define BUTTERFLY 12 B
// #define AMOEBA    13 m


typedef enum tState
{
    INICIO,
    SERVINDO,
    JOGANDO,
    FIMPART,
    FIMJOGO
}tState;

typedef enum tDirecao
{
    STOP, 
    UP,
    LEFT,
    BOTTOM,
    RIGHT
}tDirecao;


typedef struct tAudio{
    ALLEGRO_SAMPLE *sound_walk_earth, *sound_walk_empty, *sound_boulder, *sound_diamond, *sound_melody, *sound_start;
    ALLEGRO_SAMPLE_INSTANCE *walk_empty, *walk_earth, *boulder, *collect_diamond, *music, *starting_music; 
}tAudio; 

// TODO ADICIONAR AS VARIAVEIS GLOBAIS TUDO AQ
typedef struct tNivel{
    int state;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *sheet;
    tPlayer *jogador;
    tObjetos *objetosMapa;
    unsigned char keys[ALLEGRO_KEY_MAX];
    tAudio *sonsJogo; 
    // pontos *pontos_totais;

    int **mapa, relogio;
    long frames;
}tNivel; 


void state_init(tNivel *infoNivel);
void state_serve(tNivel *infoNivel);
void state_play(tNivel *infoNivel);
void state_end(tNivel *infoNivel);
void state_close(tNivel *infoNivel);

int linhaEColunaValidas(int lin,int col);
void destroiRocha(tObjetos *objetos, int **mapa, int lin,int col, tAudio *sons);
void empurrarPedra(int **mapa,tObjetos *objetos,int direcao, int lin, int col, tAudio *sons); 
int testaObjetosCaminho(tPlayer *jogador,int **mapa,tObjetos *objetos,int y,int x,int vertical,int horizontal,tAudio *sons, long frames);
int testaMapa(int **mapa,tPlayer *jogador,tObjetos *objetos,long frames, tAudio *sons);
void criaSaida(int **mapa); 
void verificaRolamento(int **mapa,tObjetos *objetos, int col,int lin, int rochaAtual);
void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames);
void atualizaPlayer(tPlayer *jogador); 
void movimentaObjetos(int **mapa, tPlayer *jogador, int direcao, tObjetos *objetos, long frames, tAudio *sons);
void mataPlayer(tPlayer *jogador, int lin, int col, int **mapa, tObjetos *objetos, tAudio *sons);
void rolaRocha(int **mapa,rochedos *rocha,int lin,int col,int direcao); 

#endif