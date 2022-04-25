#ifndef __MAPA__
#define __MAPA__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define METAL 0
#define TERRA 1
#define MURO 2
#define PEDRA 3
#define DIAMANTE 4
#define VAZIO 5
#define PLAYER 6
#define EXPLOSAO 7
#define EXPLOSAO2 8
#define EXPLOSAO3 9
#define SAIDA 10
#define FIREFLY 11
#define BUTTERFLY 12
#define AMOEBA 13
#define CREEPER 14

#define TOTAL_LINHAS 22
#define TOTAL_COLUNAS 40

typedef struct
{
  int lin, col, caindo, tipo;
  bool ativo;
} rochedos;

typedef struct
{
  int lin, col, tipo, direcaoAtual, direcaoAntiga;
  bool ativo;
} tMonstro;

typedef struct tPlayer
{
  int diamantes, direction, animacaoAtual;
  int vidas;
  int vel;
  int lin, col, linInicial, colInicial;
} tPlayer;

typedef struct tObjetos
{
  rochedos *rochedos;
  tMonstro *monstros;
  int qtPedras, qtMonstros, qtDiamantes, totalRochas;
  int animacaoCurta, animacaoLonga;
  int saidaAtiva;
} tObjetos;

typedef struct infoMapa
{
  int lin, col;
  char code[8];
  // Flags de movimento
  bool moveKeys[4];
  int pontuacao;
  int vidas;
  int diamantes;
} infoMapa;

// FUNCOES
int **iniciaMapa(char *nome_mapa, tObjetos *obj, tPlayer *jogador);
void iniciaPedrasEDiamantes(int **mapa, tObjetos *objetos);
void iniciaMonstros(int **mapa, tObjetos *objetos);
#endif