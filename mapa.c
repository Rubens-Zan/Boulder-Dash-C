
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "texturas.h"

#include <allegro5/allegro.h>
// #include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_FPS 10.0
#define R_FPS 6.0
#define S_FPS 3.0
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define SIZE 40.0


int **iniciaMapa(char *pathMapa, tObjetos *obj, tPlayer *jogador){
  FILE *arq;
  int **mapa, i, j, lin, col;
  arq = fopen(pathMapa, "r");
  if (!arq){
    fprintf(stderr, "Erro ao abrir mapa!\n");
    exit(1);
  }
  fscanf(arq, "%d %d", &lin, &col);
  
  // alocar matriz
  mapa = malloc(lin * sizeof(int *));
  mapa[0] = malloc(lin * col * sizeof(int));
  for (i = 0; i < lin; i++)
    mapa[i] = mapa[0] + i * col;

  int qtPedras = 0, qtDiamantes = 0, totalObjs = 0, qtAmoebas=0,qtVagalumes =0, qtBorboletas = 0;

  for (i = 0; i < lin; i++)
    for (j = 0; j < col; j++){
      fscanf(arq, "%d", &mapa[i][j]);
      int tipo = mapa[i][j];
      if (tipo == PEDRA){
        totalObjs++;
        qtPedras++;
      }
      if (tipo == DIAMANTE){
        totalObjs++;

        qtDiamantes++;
      }
      if (tipo == BUTTERFLY){
        qtDiamantes++;
        qtBorboletas++;
      }
      if (tipo == AMOEBA || tipo == FIREFLY){
        qtAmoebas++;
      }
      if (tipo == PLAYER){
        jogador->linInicial=i;
        jogador->colInicial=j;
        jogador->lin=i;
        jogador->col=j;
      }
    }
  obj->qtPedras = qtPedras;
  obj->qtDiamantes = qtDiamantes;
  obj->qtMonstros =   qtBorboletas+qtAmoebas+qtVagalumes;
  obj->totalRochas = totalObjs; 
  obj->rochedos = malloc((qtPedras + qtDiamantes) * sizeof(rochedos));
  obj->monstros = malloc((qtBorboletas+qtAmoebas+qtVagalumes) * sizeof(tMonstro));


  iniciaMonstros(mapa,obj);
  iniciaPedrasEDiamantes(mapa,obj); 
  fclose(arq);
  return mapa;
}

tObjetos* iniciaObjetos(ALLEGRO_BITMAP* sheet){
  tObjetos *obj;
  obj = malloc(sizeof(tObjetos));
  if(obj == NULL){
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }
  iniciaSpritesObjetos(sheet, obj);
  obj->animacaoCurta = 0;
  obj->animacaoLonga = 0; 
}

void iniciaPedrasEDiamantes(int **mapa,tObjetos *objetos){
  int pedraAux = 0;
  int i,j; 

 for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
      int tipo = mapa[i][j];
      
  	  if(tipo == PEDRA || tipo == DIAMANTE){
        rochedos * rochedoAtual = &objetos->rochedos[pedraAux];

  	  	rochedoAtual->lin = i;
  	    rochedoAtual->col = j;
  	    rochedoAtual->caindo = 0;
  	    rochedoAtual->tipo =tipo;
        rochedoAtual->ativo=true;
  	    pedraAux++;
  	  }
  	}
  }
}

void iniciaMonstros(int **mapa,tObjetos *objetos){
  int monstroAux =0;
  int i,j;

  for (i=0;i < 22;i++){
    for (j=0;j < 40;j++){
      int tipo = mapa[i][j];

      if (tipo == BUTTERFLY || tipo == AMOEBA || tipo == FIREFLY){
        tMonstro *monstro = &objetos->monstros[monstroAux]; 
        monstro->lin=i;
        monstro->col=j;
        monstro->tipo=tipo;
        monstro->ativo=1; 
        monstro->direcaoAntiga=RIGHT;
        monstro->direcaoAtual=RIGHT;

        monstroAux++;
      }

    }
  }
}

void destroi_mapa(int **mapa){
  free(mapa[0]);
  free(mapa);
}

// void destroi_objetos(tObjetos *obj){
//   free(obj->pedra);
//   free(obj->diamante - 8);
//   free(obj->borboleta -8);
//   free(obj->explosao - 5); 
//   free(obj->vagalume - 8); 

//   free(obj);
// }