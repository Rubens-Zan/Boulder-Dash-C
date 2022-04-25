#include <stdio.h>
#include <stdlib.h>
#include "texturas.h"

ALLEGRO_BITMAP *carregaBitmapEmTamanhoFixo(const char *filename, int w, int h)
{
  ALLEGRO_BITMAP *bmpRedimensionado, *bmpCarregado, *versaoAnterior;

  // 1. create a temporary bitmap of size we want
  bmpRedimensionado = al_create_bitmap(w, h);
  if (!bmpRedimensionado)
    return NULL;

  // 2. load the bitmap at the original size
  bmpCarregado = al_load_bitmap(filename);
  if (!bmpCarregado)
  {
    al_destroy_bitmap(bmpRedimensionado);
    return NULL;
  }

  // 3. set the target bitmap to the resized bmp
  versaoAnterior = al_get_target_bitmap();
  al_set_target_bitmap(bmpRedimensionado);

  // 4. copy the loaded bitmap to the resized bmp
  al_draw_scaled_bitmap(bmpCarregado,
                        0, 0,                             // source origin
                        al_get_bitmap_width(bmpCarregado),  // source width
                        al_get_bitmap_height(bmpCarregado), // source height
                        0, 0,                             // target origin
                        w, h,                             // target dimensions
                        0                                 // flags
  );

  // 5. restore the previous target and clean up
  al_set_target_bitmap(versaoAnterior);
  al_destroy_bitmap(bmpCarregado);

  return bmpRedimensionado;
}

tTexturas *inicializaTexturas(ALLEGRO_BITMAP *sheet)
{
  tTexturas *texturas = malloc(sizeof(tTexturas));
  iniciaSpritesObjetos(sheet, texturas);
  iniciaSpritesJogador(sheet, texturas);
  return texturas;
}

// CRIA OBJETO CONTENDO AS SPRITES DOS OBJETOS
void iniciaSpritesObjetos(ALLEGRO_BITMAP *sheet, tTexturas *texturas)
{
  texturas->metal = al_create_sub_bitmap(sheet, 0, 48, 15, 16);
  texturas->saida = al_create_sub_bitmap(sheet, 16, 48, 15, 16);
  texturas->muro = al_create_sub_bitmap(sheet, 32, 48, 15, 16);
  texturas->terra = al_create_sub_bitmap(sheet, 48, 48, 15, 16);
  texturas->pedra = al_create_sub_bitmap(sheet, 80, 48, 15, 16);
  texturas->vazio = al_create_sub_bitmap(sheet, 96, 48, 15, 16);

  texturas->explosao[3] = al_create_sub_bitmap(sheet, 96, 48, 15, 16);
  texturas->explosao[2] = al_create_sub_bitmap(sheet, 112, 64, 15, 16);
  texturas->explosao[1] = al_create_sub_bitmap(sheet, 128, 64, 15, 16);
  texturas->explosao[0] = al_create_sub_bitmap(sheet, 128, 80, 15, 16);

  // array para efetuar animacao do diamante
  texturas->diamante[0] = al_create_sub_bitmap(sheet, 0, 64, 15, 16);
  texturas->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  texturas->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  texturas->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  texturas->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  texturas->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  texturas->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  texturas->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);

  texturas->amoeba[0] = al_create_sub_bitmap(sheet, 64, 64, 15, 16);
  texturas->amoeba[1] = al_create_sub_bitmap(sheet, 80, 64, 15, 16);
  texturas->amoeba[2] = al_create_sub_bitmap(sheet, 64, 80, 15, 16);
  texturas->amoeba[3] = al_create_sub_bitmap(sheet, 80, 80, 15, 16);
  texturas->amoeba[4] = al_create_sub_bitmap(sheet, 64, 96, 15, 16);
  texturas->amoeba[5] = al_create_sub_bitmap(sheet, 80, 96, 15, 16);
  texturas->amoeba[6] = al_create_sub_bitmap(sheet, 64, 112, 15, 16);
  texturas->amoeba[7] = al_create_sub_bitmap(sheet, 80, 112, 15, 16);

  texturas->borboleta[0] = al_create_sub_bitmap(sheet, 96, 64, 15, 16);
  texturas->borboleta[1] = al_create_sub_bitmap(sheet, 96, 80, 15, 16);
  texturas->borboleta[2] = al_create_sub_bitmap(sheet, 96, 96, 15, 16);
  texturas->borboleta[3] = al_create_sub_bitmap(sheet, 96, 112, 15, 16);

  texturas->vagalume[0] = al_create_sub_bitmap(sheet, 80, 64, 15, 16);
  texturas->vagalume[1] = al_create_sub_bitmap(sheet, 80, 80, 15, 16);
  texturas->vagalume[2] = al_create_sub_bitmap(sheet, 80, 96, 15, 16);
  texturas->vagalume[3] = al_create_sub_bitmap(sheet, 80, 112, 15, 16);

  texturas->creeper = carregaBitmapEmTamanhoFixo("resources/img/creeper.png", SIZE_OBJS / 2, SIZE_OBJS / 2);
  texturas->arrowDown = carregaBitmapEmTamanhoFixo("resources/img/arrow-down.png", SIZE_OBJS / 2, SIZE_OBJS / 2);
  texturas->arrowUp = carregaBitmapEmTamanhoFixo("resources/img/arrow-up.png", SIZE_OBJS / 2, SIZE_OBJS / 2);
}

// Inicia as sprites do jogador
void iniciaSpritesJogador(ALLEGRO_BITMAP *sheet, tTexturas *texturas)
{
  for (int i = 0; i <= 6; i++)
    texturas->jogadorParado[i] = al_create_sub_bitmap(sheet, (16 * i), 0, 15, 16);

  for (int i = 0; i <= 6; i++)
    texturas->jogadorEsq[i] = al_create_sub_bitmap(sheet, (16 * i), 16, 15, 16);

  for (int i = 0; i <= 6; i++)
    texturas->jogadorDir[i] = al_create_sub_bitmap(sheet, (16 * i), 32, 15, 16);
}

// Funcao para destroir as sprites carregadas dos objetos
void destroiSpritesObjetos(tTexturas *texturas)
{
  al_destroy_bitmap(texturas->metal);
  al_destroy_bitmap(texturas->saida);
  al_destroy_bitmap(texturas->muro);
  al_destroy_bitmap(texturas->terra);
  al_destroy_bitmap(texturas->pedra);
  al_destroy_bitmap(texturas->vazio);
  al_destroy_bitmap(texturas->arrowUp);
  al_destroy_bitmap(texturas->arrowDown);
  al_destroy_bitmap(texturas->creeper);

  
  for (int i = 0; i < 4; i++)
  {
    al_destroy_bitmap(texturas->explosao[i]);
    al_destroy_bitmap(texturas->vagalume[i]);
    al_destroy_bitmap(texturas->borboleta[i]);
  }

  for (int i = 0; i < 8; i++)
  {
    al_destroy_bitmap(texturas->amoeba[i]);
    al_destroy_bitmap(texturas->diamante[i]);
  }
}

// Funcao para destroir as sprites carregadas do player
void destroi_sprites_player(tTexturas *texturas)
{
  for (int i = 0; i < 7; i++)
    al_destroy_bitmap(texturas->jogadorParado[i]);

  for (int i = 0; i < 7; i++)
    al_destroy_bitmap(texturas->jogadorEsq[i]);

  for (int i = 0; i < 7; i++)
    al_destroy_bitmap(texturas->jogadorDir[i]);
}