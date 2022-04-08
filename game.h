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

typedef struct tPlayer
{
    // Sprites animadas
    ALLEGRO_BITMAP *animParado[7];
    ALLEGRO_BITMAP *animEsq[7];
    ALLEGRO_BITMAP *animDir[7];
    // Ciclos de animação (percorre vetor de sprites)
    int animarParado, animarEsq, animarDir;
    int tired;
    int vivo;
    int pos_x, pos_y;
    int vel_x, vel_y;
} tPlayer;

typedef struct{
    float speed;
    bool alive;
    unsigned int diamond, score, life;
}Miner;

typedef struct{
    bool is_falling, alive;
}FallingObject;


typedef struct{
    float speed;
    bool alive;
}MovingObject;

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


void draw(bool redraw, long frames);
void draw_player(tPlayer *jogador, int** mapa, tObjetos *obj, long frames); 
void draw_instructions(); 
void drawHeader();
void draw_instructions();
void draw_inicial_menu();
#endif