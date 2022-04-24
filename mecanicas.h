#ifndef __MECANICAS__
#define __MECANICAS__

#include "game.h"
#include "mapa.h"
#include "texturas.h"
#include "desenhar.h"

void movimentaObjetos(int **mapa, tPlayer *jogador, int direcao, tObjetos *objetos, long frames, tAudio *sons); 
void movimentaMonstro(int **mapa, tMonstro *monstro, tPlayer *jogador, tObjetos *objetos, tAudio *sons); 
void mudaDirecaoMonstro(tMonstro *monstro); 
void verificaRolamento(int **mapa, tObjetos *objetos, int col, int lin, int rochaAtual); 
void rolaRocha(int **mapa, rochedos *rocha, int lin, int col, int direcao); 
void explodeEmVolta(int **mapa, tObjetos *objetos, int lin, int col, tAudio *sons); 
void mataPlayer(tPlayer *jogador, int lin, int col, int **mapa, tObjetos *objetos, tAudio *sons); 
void destroiRocha(tObjetos *objetos, int **mapa, int lin, int col, tAudio *sons); 
void mataMonstro(int **mapa, tObjetos *objetos, int lin, int col, tAudio *sons); 
void empurrarPedra(int **mapa, tObjetos *objetos, int direcao, int lin, int col, tAudio *sons); 
void atualizaPlayer(tPlayer *jogador); 

#endif 