#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mapa.h"
#include "texturas.h"

void iniciaSpritesObjetos(ALLEGRO_BITMAP* sheet, objetos* obj){
  obj->metal = al_create_sub_bitmap(sheet, 0, 48, 15, 16);
  
  obj->saida = al_create_sub_bitmap(sheet, 16, 48, 15, 16);
  obj->muro  = al_create_sub_bitmap(sheet, 32, 48, 15, 16);
  obj->terra = al_create_sub_bitmap(sheet, 48, 48, 15, 16);
  obj->pedra = al_create_sub_bitmap(sheet, 80, 48, 15, 16);
  obj->vazio = al_create_sub_bitmap(sheet, 96, 48, 15, 16);

  obj->explosao[3] = al_create_sub_bitmap(sheet, 96, 48, 15, 16);
  obj->explosao[2] = al_create_sub_bitmap(sheet, 112, 64, 15, 16);
  obj->explosao[1] = al_create_sub_bitmap(sheet, 128, 64, 15, 16);
  obj->explosao[0] = al_create_sub_bitmap(sheet, 128, 80, 15, 16);

  obj->diamante[0] = al_create_sub_bitmap(sheet, 0, 64, 15, 16);
  obj->diamante[2] = al_create_sub_bitmap(sheet, 0, 80, 15, 16);
  obj->diamante[4] = al_create_sub_bitmap(sheet, 0, 96, 15, 16);
  obj->diamante[6] = al_create_sub_bitmap(sheet, 0, 112, 15, 16);
  obj->diamante[1] = al_create_sub_bitmap(sheet, 16, 64, 15, 16);
  obj->diamante[3] = al_create_sub_bitmap(sheet, 16, 80, 15, 16);
  obj->diamante[5] = al_create_sub_bitmap(sheet, 16, 96, 15, 16);
  obj->diamante[7] = al_create_sub_bitmap(sheet, 16, 112, 15, 16);
}

#define METAL     0
#define TERRA     1
#define MURO      2
#define PEDRA     3
#define DIAMANTE  4
#define VAZIO     5
#define PLAYER    6
#define EXPLOSAO  7
#define EXPLOSAO2 8
#define EXPLOSAO3 9
#define SAIDA     10

ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h){
  ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;

  // 1. create a temporary bitmap of size we want
  resized_bmp = al_create_bitmap(w, h);
  if (!resized_bmp) return NULL;

  // 2. load the bitmap at the original size
  loaded_bmp = al_load_bitmap(filename);
  if (!loaded_bmp)
  {
     al_destroy_bitmap(resized_bmp);
     return NULL;
  }

  // 3. set the target bitmap to the resized bmp
  prev_target = al_get_target_bitmap();
  al_set_target_bitmap(resized_bmp);

  // 4. copy the loaded bitmap to the resized bmp
  al_draw_scaled_bitmap(loaded_bmp,
     0, 0,                                // source origin
     al_get_bitmap_width(loaded_bmp),     // source width
     al_get_bitmap_height(loaded_bmp),    // source height
     0, 0,                                // target origin
     w, h,                                // target dimensions
     0                                    // flags
  );

  // 5. restore the previous target and clean up
  al_set_target_bitmap(prev_target);
  al_destroy_bitmap(loaded_bmp);

  return resized_bmp;
}



ALLEGRO_BITMAP * inicializaTexturas(int level){
   /* graphics */
    ALLEGRO_BITMAP *texturas[10];
    texturas[VAZIO] = load_bitmap_at_size("resources/img/Empty.png",SIZE_OBJS, SIZE_OBJS);
    texturas[TERRA] = load_bitmap_at_size("resources/img/Earth.png", SIZE_OBJS, SIZE_OBJS);
    texturas[PLAYER] = load_bitmap_at_size("resources/img/Miner.png", SIZE_OBJS, SIZE_OBJS);
    texturas[PEDRA] = load_bitmap_at_size("resources/img/Rock.png", SIZE_OBJS, SIZE_OBJS);
    texturas[MURO] = load_bitmap_at_size("resources/img/Border.png", SIZE_OBJS, SIZE_OBJS);
    texturas[DIAMANTE] = load_bitmap_at_size("resources/img/Diamond.png", SIZE_OBJS,SIZE_OBJS);
    texturas[SAIDA] = load_bitmap_at_size("resources/img/Door.png", SIZE_OBJS, SIZE_OBJS);
    // return (*texturas); 
    // texturas_menu = load_bitmap_at_size("texturass/front.png", SCREEN_WIDTH, SCREEN_HEIGHT);
    // texturas_icon_diamond = load_bitmap_at_size("texturass/Diamond.png", 3*SIZE_OBJ/5, 3*SIZE_OBJ/5);
}

void destroiSpritesObjetos(objetos* obj){
  al_destroy_bitmap(obj->metal);
  al_destroy_bitmap(obj->saida);
  al_destroy_bitmap(obj->muro);
  al_destroy_bitmap(obj->terra);
  al_destroy_bitmap(obj->pedra);
  al_destroy_bitmap(obj->vazio);

  for(int i = 0;i < 4;i++)
    al_destroy_bitmap(obj->explosao[i]);

  for(int i = 0;i < 8;i++)
    al_destroy_bitmap(obj->diamante[i]);
}