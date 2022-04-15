
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "texturas.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>
#include <stdio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/keyboard.h>
#include <allegro5/timer.h>
#include <allegro5/color.h>
#include <stdlib.h>
#include <string.h>

#define M_FPS 10.0
#define R_FPS 6.0
#define S_FPS 3.0
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define SIZE 40.0

int **iniciaMapa(char *pathMapa, tObjetos *obj){
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

  int qtPedras = 0, qtDiamantes = 0;
  for (i = 0; i < lin; i++)
    for (j = 0; j < col; j++){
      fscanf(arq, "%d", &mapa[i][j]);
      if (mapa[i][j] == PEDRA)
        qtPedras++;
      if (mapa[i][j] == DIAMANTE)
        qtDiamantes++;
    }
  obj->qtPedras = qtPedras;
  obj->qtDiamantes = qtDiamantes;
  obj->rock = malloc(qtPedras * sizeof(rock));
  obj->diamond = malloc(qtDiamantes * sizeof(rock));
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
  obj->animacao = 0;
}

void iniciaPedrasEDiamantes(int **mapa,tObjetos *objetos){
  int pedraAux = 0;
  int diamanteAux = 0;
  int i,j; 

 for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
  	  if(mapa[i][j] == PEDRA){
  	  	objetos->rock[pedraAux].x = i;
  	    objetos->rock[pedraAux].y = j;
  	    objetos->rock[pedraAux].caindo = 0;
  	    pedraAux++;
  	  }
  	  if(mapa[i][j] == DIAMANTE){
  	  	objetos->diamond[diamanteAux].x = i;
  	    objetos->diamond[diamanteAux].y = j;
  	    objetos->diamond[diamanteAux].caindo = 0;
  	    diamanteAux++;
  	  }
  	}
  }

}



void destroi_mapa(int **mapa){
  free(mapa[0]);
  free(mapa);
}

void destroi_objetos(tObjetos *obj){
  free(obj->pedra);
  free(obj->diamante);
  free(obj);
}