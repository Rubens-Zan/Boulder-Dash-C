
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

void drawMap(int** mapa, tObjetos* objetosMapa, long frames, int col, int lin){
  int i, j, i_aux, j_aux;
  for(i = 0;i < col;i++){
  	for(j = 0;j < lin;j++){
  	  i_aux = i * SIZE_OBJS;
  	  j_aux = j * SIZE_OBJS;
  	  switch(mapa[i][j]){
  	  	case METAL:
  	  	  al_draw_scaled_bitmap(objetosMapa->metal, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case TERRA:
  	  	  al_draw_scaled_bitmap(objetosMapa->terra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case MURO:
  	  	  al_draw_scaled_bitmap(objetosMapa->muro, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
        case PEDRA:
          al_draw_scaled_bitmap(objetosMapa->pedra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case VAZIO:
          al_draw_scaled_bitmap(objetosMapa->vazio, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case SAIDA:
          al_draw_scaled_bitmap(objetosMapa->saida, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case DIAMANTE:
          //Reseta frames do diamante se necessário e faz animação
          if(objetosMapa->animacao == 7)
          	objetosMapa->animacao = 0;
          if(frames % 30 == 0)
            objetosMapa->animacao++;
          al_draw_scaled_bitmap(objetosMapa->diamante[objetosMapa->animacao], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case EXPLOSAO:
          al_draw_scaled_bitmap(objetosMapa->explosao[1], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = EXPLOSAO2;
          break;
        case EXPLOSAO2:
          al_draw_scaled_bitmap(objetosMapa->explosao[2], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = EXPLOSAO3;
          break;
        case EXPLOSAO3:
          al_draw_scaled_bitmap(objetosMapa->explosao[3], 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          if(frames % 15 == 0)
            mapa[i][j] = VAZIO;
          break;
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