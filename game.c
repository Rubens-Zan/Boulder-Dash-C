#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_ttf.h"
#include "game.h"
#include "mapa.h"
#include "texturas.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// GLOBAIS
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;
ALLEGRO_FONT *font;
ALLEGRO_BITMAP *sheet;

tPlayer *jogador;
tObjetos *objetos_mapa;
unsigned char keys[ALLEGRO_KEY_MAX];

// audio *sons_jogo;
// pontos *pontos_totais;

int **mapa, relogio = 150;
long frames = 0;
unsigned char key[ALLEGRO_KEY_MAX];

void testaAllegro(bool ok, char *descricao){
  if (ok)
    return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void inicializarAllegro(){
  testaAllegro(al_init(), "allegro");
  testaAllegro(al_install_keyboard(), "keyboard");

  timer = al_create_timer(1.0 / FRAMERATE);
  testaAllegro(timer, "timer");

  queue = al_create_event_queue();
  testaAllegro(queue, "queue");

  // Pega spritesheet
  testaAllegro(al_init_image_addon(), "image addon");
  sheet = al_load_bitmap(PATH_SPRITESHEET);
  testaAllegro(sheet, "spritesheet");

  // Pega áudios
  testaAllegro(al_install_audio(), "audio");
  testaAllegro(al_init_acodec_addon(), "audio codecs");
  testaAllegro(al_reserve_samples(16), "reserve samples");

  // Inicia estruturas principais
  jogador = inicia_jogador(sheet);
  objetos_mapa = iniciaObjetos(sheet);
  mapa = iniciaMapa(PATH_MAP_1, objetos_mapa);

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  disp = al_create_display(WIDTH, HEIGHT);
  testaAllegro(disp, "display");

  // Inicia fontes
  testaAllegro(al_init_font_addon(), "fonte");
  testaAllegro(al_init_ttf_addon(), "fonte");
  font = al_create_builtin_font();
  font = al_load_ttf_font("resources/font/zig.ttf", 25, 0);
  testaAllegro(font, "fonteee");

  testaAllegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
}

void state_init(){
  inicializarAllegro();
  jogador = inicia_jogador(sheet);
  objetos_mapa = iniciaObjetos(sheet);
  mapa = iniciaMapa(PATH_MAP_1, objetos_mapa);

  state = JOGANDO;
}

void state_serve(){
  bool done = false;
  al_flush_event_queue(queue);
  while (1){
    drawInstructions();
    al_wait_for_event(queue, &event);
    if (al_is_event_queue_empty(queue))
      drawInstructions();
    switch (event.type){
    case ALLEGRO_EVENT_KEY_DOWN:
      keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keys[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    }
    // Caso H/F1 seja pressionado, volta ao jogo
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1]){
      keys[ALLEGRO_KEY_H] = 0;
      keys[ALLEGRO_KEY_F1] = 0;
      state = JOGANDO;
      done = true;
    }
    // Caso ESC seja pressionado, fim partida
    else if (keys[ALLEGRO_KEY_ESCAPE])
    {
      keys[ALLEGRO_KEY_ESCAPE] = 0;
      state = FIMPART;
      done = true;
    }
    if (done)
      break;
  }
}

void state_play(){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu = 0, ganhou = 0;
  al_flush_event_queue(queue);

  memset(key, 0, sizeof(key));
  al_start_timer(timer);
  
  while (1){
    al_wait_for_event(queue, &event);
    switch (event.type){
    case ALLEGRO_EVENT_KEY_DOWN:
      keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keys[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    }
    // Caso H/F1 seja pressionado, volta ao jogo
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1]){
      keys[ALLEGRO_KEY_H] = 0;
      keys[ALLEGRO_KEY_F1] = 0;
      state = SERVINDO;
      done = true;
    }else {
      draw(redraw, frames);
    }

    frames++;
  }
}

// Função de desenho principal
void draw(bool redraw, long frames){
  drawHeader();
  drawMap(mapa, objetos_mapa, frames, 22, 40);
  drawPlayer(jogador, mapa, objetos_mapa, frames);
  al_flip_display();
  redraw = false;
}

void drawPlayer(tPlayer *jogador, int **mapa, tObjetos *obj, long frames){
  al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animParadoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
  jogador->animParadoAtual = 0;
}

void drawEndGame(){
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "F I M D E J O G O");
  // al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 100 + 2 * SIZE_OBJS, 0, "PONTUACAO: %d",jogador->pontuacao);
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 160 + 2 * SIZE_OBJS, 0, "Placar de lideres");
  // for(int i = 0;i < 5;i++)
  // al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 220 + (i*40) + 2 * SIZE_OBJS, 0, "%d: %d pts", i + 1, pontos_totais->score[i]);
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 5 * SIZE_OBJS, 500 + 2 * SIZE_OBJS, 0, "Pressione ESC para sair...");
  al_flip_display();
}

void drawInstructions(){
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "INSTRUCOES");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 80 + 2 * SIZE_OBJS, 0, "Colete todos os diamantes e");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 100 + 2 * SIZE_OBJS, 0, "chegue na saida antes que o tempo acabe. Cuidado com as");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 120 + 2 * SIZE_OBJS, 0, "pedras, elas podem cair em cima voce!");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 160 + 2 * SIZE_OBJS, 0, "Utilize as setas do teclado para se movimentar");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 180 + 2 * SIZE_OBJS, 0, "para finalizar o jogo pressione ESC.");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 280 + 2 * SIZE_OBJS, 0, "Jogo desenvolvido por Rubens Laszlo ");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 4, 300 + 2 * SIZE_OBJS, 0, "Abril de 2021.");
  al_flip_display();
}

void drawHeader(){
  int vidas = 7; 

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", 20);
  al_draw_bitmap(objetos_mapa->diamante[0], 0, 8, 0);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", 4, MIN_DIAMANTES);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 200, 10, 0, "%d", relogio);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 10, 0, "Vidas: %d", vidas);
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH / 3 - 20 , 10, 0, "Help: H/F1");
}
