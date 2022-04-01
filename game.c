#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_ttf.h"
#include "game.h"
#include "mapa.h"
#include "texturas.h"
#include "objetos.h"

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* sheet;
objetos *objetos_mapa;


#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 680


ALLEGRO_BITMAP *texture[20];

int **mapa, relogio = 150;
long frames = 0;
unsigned char key[ALLEGRO_KEY_MAX];
char *cheat_code = "time";
ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *miner_timer, *rock_timer, *spider_timer, *water_timer, *level_timer;
ALLEGRO_FONT *score_text, *pause_text;

void state_init(){
   


    /* font loading */
  score_text = al_load_font("resources/font/BoulderDash6128.ttf", 27, 0);
  pause_text = al_load_font("resources/font/BoulderDash6128.ttf", 50, 0);

    /* event queue for event handling and timers init. */
    event_queue = al_create_event_queue();

    /* created the big picture */
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* adding inputs, timers and outputs to event queue to handle them later */
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(miner_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(rock_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(spider_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(water_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(level_timer));

    /* cleared the screen before we start */
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

  state = JOGANDO;
}

void state_serve(){
  bool done = false;
  al_flush_event_queue(queue);
  while(1){
  	al_wait_for_event(queue, &event);
  	if(al_is_event_queue_empty(queue))
  	  draw_instructions();
  	switch(event.type){
  	  case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
      case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
  	}
  	//Caso H/F1 seja pressionado, volta ao jogo
  	if(key[ALLEGRO_KEY_H] || key[ALLEGRO_KEY_F1]){
  	  key[ALLEGRO_KEY_H] = 0;
      key[ALLEGRO_KEY_F1] = 0;
      state = JOGANDO;
      done = true;
  	}
  	//Caso ESC seja pressionado, fim partida
  	else if(key[ALLEGRO_KEY_ESCAPE]){
  	  key[ALLEGRO_KEY_ESCAPE] = 0;
  	  state = FIMPART;
  	  done = true;
  	}
  	if(done)
  	  break;
  }
}

void state_play(){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu = 0, ganhou = 0, glitch = 1;
  memset(key, 0, sizeof(key));
  al_start_timer(timer);
  while (1){
    draw(redraw,frames); 
  }
}

//Função de desenho principal
void draw(bool redraw, long frames){
  drawHeader();
  desenhaMapa(mapa, ROWS,COLS,texture);
  al_flip_display();
  redraw = false;
}

void draw_instructions(){
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "INSTRUCOES");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 80 + 2 * SIZE_OBJS, 0, "Colete todos os diamantes e");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 100 + 2 * SIZE_OBJS, 0, "e chegue na saida antes que o tempo acabe. Cuidado com as");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 120 + 2 * SIZE_OBJS, 0, "pedras, elas podem cair em cima voce!");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 160 + 2 * SIZE_OBJS, 0, "Utilize as setas do teclado para se movimentar");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 180 + 2 * SIZE_OBJS, 0, "para finalizar o jogo pressione ESC.");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 280 + 2 * SIZE_OBJS, 0, "Jogo desenvolvido por Rubens Laszlo ");
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/4, 300 + 2 * SIZE_OBJS, 0, "Abril de 2021.");
  al_flip_display();
}

void drawHeader(){
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(font, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", 20);
  al_draw_bitmap(objetos_mapa->diamante[0], 0, 8, 0);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", 4, MIN_DIAMANTES);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 200, 10, 0, "%d", relogio);
  al_draw_textf(font, al_map_rgb(255, 255, 255), 100, 10, 0, "Vidas: %d", 5);
  al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/2 - 10, 10, 0, "Help: H/F1");
}
