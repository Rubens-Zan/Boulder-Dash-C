#ifndef __TEXTURAS__
#define __TEXTURAS__

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>

#define SIZE_OBJS 30

typedef struct tTexturas
{
    ALLEGRO_BITMAP *vazio, *saida, *pedra, *metal, *muro, *terra, *arrowUp, *arrowDown;
    ALLEGRO_BITMAP *jogadorParado[7];
    ALLEGRO_BITMAP *jogadorEsq[7];
    ALLEGRO_BITMAP *jogadorDir[7];
    ALLEGRO_BITMAP *amoeba[8];
    ALLEGRO_BITMAP *diamante[8];
    ALLEGRO_BITMAP *borboleta[4];
    ALLEGRO_BITMAP *vagalume[4];
    ALLEGRO_BITMAP *explosao[4];
    ALLEGRO_BITMAP *creeper;

} tTexturas;

tTexturas *inicializaTexturas(ALLEGRO_BITMAP *sheet);
void iniciaSpritesObjetos(ALLEGRO_BITMAP *sheet, tTexturas *texturas);
void iniciaSpritesJogador(ALLEGRO_BITMAP *sheet, tTexturas *texturas);
void destroiSpritesObjetos(tTexturas *texturas);
void destroi_sprites_player(tTexturas *texturas);
ALLEGRO_BITMAP *carregaBitmapEmTamanhoFixo(const char *filename, int w, int h);

#endif