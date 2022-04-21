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


#define PATH_MAP_1 "resources/mapas/mapa1.txt"
#define MIN_DIAMANTES 12
#define SIZE_ARQ_PONTOS 5
#define PATH_SPRITESHEET "resources/img/spritesheet.png"


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



typedef struct tPlayer{
    // Sprites animadas
    ALLEGRO_BITMAP *animacaoParado[7];
    ALLEGRO_BITMAP *animacaoEsq[7];
    ALLEGRO_BITMAP *animacaoDir[7];
    int pontuacao;
    int diamantes; 
    int direction; 
    int animacaoAtual;
    int tired;
    int vivo,vidas;
    int lin, col;
    int vel;
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

    // audio *sons_jogo;
    // pontos *pontos_totais;

    int **mapa, relogio;
    long frames;
}tNivel; 





void state_init(tNivel *infoNivel);
void state_serve(tNivel *infoNivel);
void state_play(tNivel *infoNivel);
void state_end(tNivel *infoNivel);
void state_close(tNivel *infoNivel);
void coletaDiamante(tPlayer *jogador, tObjetos *objetos, int **mapa, int lin,int col);

int testaObjetosCaminho(tPlayer *jogador,int **mapa,tObjetos *objetos,int y,int x,int vertical,int horizontal);
int testaMapa(int **mapa,tPlayer *jogador,tObjetos *objetos,long frames);
void criaSaida(int **mapa); 
void verificaRolamento(int **mapa,tObjetos *objetos, int col,int lin, int rochaAtual);
void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames);
void atualizaPlayer(tPlayer *jogador); 
void verificaPedras(int **mapa, tPlayer *jogador, int direcao,tObjetos *objetos, long frames);
void mataPlayer(tPlayer *jogador,int x,int y, int **mapa);
void rolaRocha(int **mapa,rochedos *rocha,int lin,int col,int direcao); 

#endif