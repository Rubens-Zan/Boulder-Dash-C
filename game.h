#ifndef __GAME__
#define __GAME__

#include <allegro5/allegro5.h>
#include "mapa.h"
#include "texturas.h"

#define MARGIN_TOP 30
#define WIDTH (40 * SIZE_OBJS)
#define HEIGHT ((22 * SIZE_OBJS) + MARGIN_TOP)
#define FRAMERATE 60.0
#define TEMPO_RESET 105
#define KEY_SEEN 1
#define KEY_RELEASED 2

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
} tState;

typedef enum tDirecao
{
    STOP,
    UP,
    LEFT,
    BOTTOM,
    RIGHT
} tDirecao;

typedef struct tAudio
{
    ALLEGRO_SAMPLE *soundDoor,*soundWalkEarth, *soundWalkEmpty, *soundBoulder, *soundDiamond, *soundMelody, *soundStart, *soundExplosion;
    ALLEGRO_SAMPLE_INSTANCE *door,*walkEmpty, *walkEarth, *boulder, *collectDiamond, *music, *startingMusic, *explosion;
} tAudio;

typedef struct tNivel
{
    tPlayer *jogador;
    tObjetos *objetosMapa;
    unsigned char keys[ALLEGRO_KEY_MAX];

    int **mapa, relogio, pontos;
    long frames;
} tNivel;

typedef struct tGame
{
    tNivel *nivelAtual;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_DISPLAY *disp;
    int level, state;
    int pontuacao[10];
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *sheet;
    tAudio *sonsJogo;
    tPlayer *jogador;
    tTexturas *texturas;
} tGame;

void alteraNivel(tGame *game, int novo, int venceu);
void executaEasterEgg(int **mapa, rochedos *rochas, int totalRochas, tAudio *sons);
void state_init(tGame *game);
void state_serve(tGame *game);
void state_play(tGame *game);
void destroiNivel(tNivel *nivel); 
void state_end(tGame *game);
void state_close(tGame *game);
char *pegaPathMapa(int level);
void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames);
void criaSaida(tObjetos *obj, tAudio *sons); 
tNivel *iniciaNivel(int level);
void alteraNivel(tGame *game, int novo, int venceu);
tAudio *iniciaAudio();
tPlayer *iniciaJogador();
tObjetos *iniciaObjetos();
int testaObjetosCaminho(tPlayer *jogador, int **mapa, tObjetos *objetos, int linAtual, int colAtual, int vertical, int horizontal, tAudio *sons, long frames, tGame *game);
int testaMapa(int **mapa, tPlayer *jogador, tObjetos *objetos, long frames, tAudio *sons, tGame *game);
int linhaEColunaValidas(int lin, int col);
#endif