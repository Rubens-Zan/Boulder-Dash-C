
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

  int qtPedras = 0, qtDiamantes = 0, totalObjs = 0, qtBorboletas = 0;
  for (i = 0; i < lin; i++)
    for (j = 0; j < col; j++){
      fscanf(arq, "%d", &mapa[i][j]);
      if (mapa[i][j] == PEDRA){
        totalObjs++;

        qtPedras++;
      }
      if (mapa[i][j] == DIAMANTE){
        totalObjs++;

        qtDiamantes++;
      }
      if (mapa[i][j] == BUTTERFLY){
        qtDiamantes++;
      }
      if (mapa[i][j] == PLAYER){
        jogador->linInicial=i;
        jogador->colInicial=j;
        jogador->lin=i;
        jogador->col=j;
      }
    }
  obj->qtPedras = qtPedras;
  obj->qtDiamantes = qtDiamantes;
  obj->totalRochas = totalObjs; 
  
  obj->rochedos = malloc((qtPedras + qtDiamantes) * sizeof(rochedos));
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
  int diamanteAux = 0;
  int i,j; 

 for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
      int tipo = mapa[i][j];
      
  	  if(tipo == PEDRA || tipo == DIAMANTE){
  	  	objetos->rochedos[pedraAux].lin = i;
  	    objetos->rochedos[pedraAux].col = j;
  	    objetos->rochedos[pedraAux].caindo = 0;
  	    objetos->rochedos[pedraAux].tipo =tipo;
        objetos->rochedos[pedraAux].ativo=true;

  	    pedraAux++;
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