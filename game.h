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
#define SPAWN_X (3 * SIZE_OBJS)
#define SPAWN_Y ((2 * SIZE_OBJS) + MARGIN_TOP)
#define PATH_MAP_1 "resources/mapas/mapa1.txt"
#define MIN_DIAMANTES 12
#define SIZE_ARQ_PONTOS 5
#define PATH_SPRITESHEET "resources/img/spritesheet.png"



typedef struct tPlayer{
    // Sprites animadas
    ALLEGRO_BITMAP *animacaoParado[7];
    ALLEGRO_BITMAP *animacaoEsq[7];
    ALLEGRO_BITMAP *animacaoDir[7];
    // Ciclos de animação (percorre vetor de sprites)
    int animParadoAtual, animEsqAtual, animDirAtual;
    int tired;
    int vivo;
    int posX, posY;
    int velX, velY;
} tPlayer;

typedef struct objetoEstatico{
    bool is_falling, alive;
}objetoEstatico;


typedef struct objetoMovel{
    float speed;
    bool alive;
}objetoMovel;

// TODO ADICIONAR AS VARIAVEIS GLOBAIS TUDO AQ
typedef struct tNivel{
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT event;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_DISPLAY *disp;
    ALLEGRO_FONT *font;
    ALLEGRO_BITMAP *sheet;

    tPlayer *jogador;
    tObjetos *objetos_mapa;
    unsigned char keys[ALLEGRO_KEY_MAX];

    // audio *sons_jogo;
    // pontos *pontos_totais;

    int **mapa, relogio;
    long frames;
}tNivel; 

enum
{
    INICIO,
    SERVINDO,
    JOGANDO,
    FIMPART,
    FIMJOGO
} state;

void state_init();
void state_serve();
void state_play();
void state_end();
void state_close();

void verificaEntrada(unsigned char *keys, bool *done, bool redraw,tPlayer *jogador); 

// FUNCOES DE DESENHO 
void draw(bool redraw, long frames);
void drawPlayer(tPlayer *jogador, int** mapa, tObjetos *obj, long frames); 
void drawInstructions(); 
void drawHeader();
void drawInstructions();
void drawInicialMenu();
#endif