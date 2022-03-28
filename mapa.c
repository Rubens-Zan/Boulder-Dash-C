
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

int** iniciaMapa(char* pathMapa, objetos* obj){
  FILE *arq;
  int **mapa, i, j, lin, col;
  arq = fopen(pathMapa, "r");
  if(!arq){
  	fprintf(stderr, "Erro ao abrir mapa!\n");
  	exit(1);
  }
  fscanf(arq, "%d %d", &lin, &col);

  // alocar matriz
  mapa = malloc(lin * sizeof(int*));
  mapa[0]=malloc(lin*col*sizeof(int));
  for(i = 0;i < lin;i++)
  	mapa[i] = mapa[0]+i*col;
  
  int qtPedras = 0, qtDiamantes = 0;
  for(i = 0;i < lin;i++)
  	for(j = 0;j < col;j++){
  	  fscanf(arq, "%d", &mapa[i][j]);
  	  if(mapa[i][j] == PEDRA)
  	  	qtPedras++;
  	  if(mapa[i][j] == DIAMANTE)
  	  	qtDiamantes++;
  	}
  obj->qtPedras = qtPedras;
  obj->qtDiamantes = qtDiamantes;
  obj->rock = malloc(qtPedras * sizeof(rock));
  obj->diamond = malloc(qtDiamantes * sizeof(rock));
  iniciaPedrasEDiamantes(mapa, obj);
  fclose(arq);
  return mapa;
}

void iniciaPedrasEDiamantes(int** mapa, objetos* obj){
  int qtPedras = 0, qtDiamantes = 0, i, j;

  for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
  	  if(mapa[i][j] == PEDRA){
  	  	obj->rock[qtPedras].x = i;
  	    obj->rock[qtPedras].y = j;
  	    obj->rock[qtPedras].caindo = 0;
  	    qtPedras++;
  	  }
  	  if(mapa[i][j] == DIAMANTE){
  	  	obj->diamond[qtDiamantes].x = i;
  	    obj->diamond[qtDiamantes].y = j;
  	    obj->diamond[qtDiamantes].caindo = 0;
  	    qtDiamantes++;
  	  }
  	}
  }
}

objetos* iniciaObjetos(ALLEGRO_BITMAP* sheet){
  objetos *obj;
  obj = malloc(sizeof(objetos));
  if(obj == NULL){
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }
  // iniciaSpritesObjetos(sheet, obj);
  obj->ciclos_diamante = 0;
}

void desenhaMapa(int** mapa, objetos* objetos_mapa, long frames){
  int i, j, i_aux, j_aux;
  int level=1;
  ALLEGRO_BITMAP *texturas[10];
  
  texturas[VAZIO] = load_bitmap_at_size("resources/img/Empty.png",SIZE_OBJS, SIZE_OBJS);
  texturas[TERRA] = load_bitmap_at_size("resources/img/Earth.png", SIZE_OBJS, SIZE_OBJS);
  texturas[PLAYER] = load_bitmap_at_size("resources/img/Miner.png", SIZE_OBJS, SIZE_OBJS);
  texturas[PEDRA] = load_bitmap_at_size("resources/img/Rock.png", SIZE_OBJS, SIZE_OBJS);
  texturas[MURO] = load_bitmap_at_size("resources/img/Border.png", SIZE_OBJS, SIZE_OBJS);
  texturas[DIAMANTE] = load_bitmap_at_size("resources/img/Diamond.png", SIZE_OBJS,SIZE_OBJS);
  texturas[SAIDA] = load_bitmap_at_size("resources/img/Door.png", SIZE_OBJS, SIZE_OBJS);

  for(i = 0;i < 22;i++){
  	for(j = 0;j < 40;j++){
  	  i_aux = i * SIZE_OBJS;
  	  j_aux = j * SIZE_OBJS;
  	  switch(mapa[i][j]){
  	  	case METAL:
          al_draw_bitmap(texturas[METAL], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
  	  	  // al_draw_scaled_bitmap(objetos_mapa->metal, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case TERRA:
          al_draw_bitmap(texturas[TERRA], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
  	  	  // al_draw_scaled_bitmap(objetos_mapa->terra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
  	  	case MURO:
          al_draw_bitmap(texturas[MURO], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
  	  	  // al_draw_scaled_bitmap(objetos_mapa->muro, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
  	  	  break;
        case PEDRA:
          al_draw_bitmap(texturas[PEDRA], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
          // al_draw_scaled_bitmap(objetos_mapa->pedra, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case VAZIO:
          al_draw_bitmap(texturas[VAZIO], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
          // al_draw_scaled_bitmap(objetos_mapa->vazio, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case SAIDA:
          al_draw_bitmap(texturas[SAIDA], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
          // al_draw_scaled_bitmap(objetos_mapa->saida, 0, 0, 15, 16, j_aux, i_aux + MARGIN_TOP, SIZE_OBJS, SIZE_OBJS, 0);
          break;
        case DIAMANTE:
          al_draw_bitmap(texturas[DIAMANTE], i_aux*SIZE_OBJS, j_aux*SIZE_OBJS,0);
          break;
  	  }
  	}
  } 
}

// void draw_map(int** mapa, int row, int col){
//     for(int y=0; y<col; y++){
//         for(int x=0; x<row; x++){
//           al_draw_bitmap(mapa[x][y].image, y*SIZE_OBJS, x*SIZE_OBJS,0);
//         }
//     }
// }

void destroi_mapa(int** mapa){
  int i;
  free(mapa[0]);
  free(mapa);
}

void destroi_objetos(objetos* obj){
  free(obj->pedra);
  free(obj->diamante);
  free(obj);
}