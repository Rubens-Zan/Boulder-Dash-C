
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



int **iniciaMapa(char *pathMapa, objetos *obj)
{
  FILE *arq;
  int **mapa, i, j, lin, col;
  arq = fopen(pathMapa, "r");
  if (!arq)
  {
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
    for (j = 0; j < col; j++)
    {
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

// Funcao principal para desenhar o mapa
// carrega as texturas inicial para nao gastar processamento
void desenhaMapa(int **map, int lin, int col, ALLEGRO_BITMAP *texture[10])
{
  for (int i = 0; i < lin; i++)
    for (int j = 0; j < col; j++)
      al_draw_bitmap(texture[map[i][j]], j * SIZE_OBJS, i * SIZE_OBJS, 0);
}

void destroi_mapa(int **mapa)
{
  free(mapa[0]);
  free(mapa);
}

void destroi_objetos(objetos *obj)
{
  free(obj->pedra);
  free(obj->diamante);
  free(obj);
}