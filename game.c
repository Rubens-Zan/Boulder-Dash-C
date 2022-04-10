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
// ALLEGRO_FONT *font;
// ALLEGRO_BITMAP *sheet;

unsigned char keys[ALLEGRO_KEY_MAX];

// audio *sons_jogo;
// pontos *pontos_totais;

// int **mapa, relogio = 150;
long frames = 0;

void testaAllegro(bool ok, char *descricao){
  if (ok)
    return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void inicializarAllegro(tNivel *infoNivel){

  testaAllegro(al_init(), "allegro");
  testaAllegro(al_install_keyboard(), "keyboard");

  // infoNivel->timer = al_create_timer(1.0 / FRAMERATE);

  timer = al_create_timer(1.0 / FRAMERATE);
  testaAllegro(timer, "timer");

  // infoNivel->queue = al_create_event_queue();

  queue = al_create_event_queue();
  testaAllegro(queue, "queueeeee");

  // Pega spritesheet
  testaAllegro(al_init_image_addon(), "image addon");
  infoNivel->sheet = al_load_bitmap(PATH_SPRITESHEET);

  // Pega áudios
  testaAllegro(al_install_audio(), "audio");
  testaAllegro(al_init_acodec_addon(), "audio codecs");
  testaAllegro(al_reserve_samples(16), "reserve samples");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  disp = al_create_display(WIDTH, HEIGHT);

  // Inicia fontes
  testaAllegro(al_init_font_addon(), "fonte");
  testaAllegro(al_init_ttf_addon(), "fonte");
  infoNivel->font = al_create_builtin_font();
  infoNivel->font = al_load_ttf_font("resources/font/zig.ttf", 20, 0);

  testaAllegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
}

void state_init(tNivel *infoNivel)
{

  inicializarAllegro(infoNivel);
  infoNivel->jogador = inicia_jogador(infoNivel->sheet);
  infoNivel->objetos_mapa = iniciaObjetos(infoNivel->sheet);
  infoNivel->mapa = iniciaMapa(PATH_MAP_1, infoNivel->objetos_mapa);

  state = JOGANDO;
  // state = SERVINDO;
}

void state_serve(tNivel *infoNivel)
{
  bool done = false;
  al_flush_event_queue(queue);
  while (1)
  {
    drawInstructions(infoNivel);
    al_wait_for_event(queue, &event);
    if (al_is_event_queue_empty(queue))
      drawInstructions(infoNivel);
    switch (event.type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
      keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keys[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    }
    // Caso H/F1 seja pressionado, volta ao jogo
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1])
    {
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

void state_play(tNivel *infoNivel){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu = 0, ganhou = 0;
  al_flush_event_queue(queue);
  memset(keys, 0, sizeof(keys));
  al_start_timer(timer);

  while (1){
    al_wait_for_event(queue, &event);

    switch (event.type){
    case ALLEGRO_EVENT_TIMER:
      verificaEntrada(keys, &done, redraw, infoNivel->jogador, frames);
      if (frames % 60 == 0 && infoNivel->jogador->vivo && infoNivel->relogio > 0)
        infoNivel->relogio--;
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keys[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = true;
      break;
    }
    // Vai para menu de ajuda
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1]){
      keys[ALLEGRO_KEY_H] = 0;
      keys[ALLEGRO_KEY_F1] = 0;
      state = SERVINDO;
      done = true;
    }
    if (redraw && al_is_event_queue_empty(queue))
      draw(redraw, frames, infoNivel);

    if (done){
      break; 
    }
    frames++;
  }
}

void atualiza_player(tPlayer *jogador, int andou){
  if(jogador->direction == UP){
    jogador->posY -= jogador->vel;
    jogador->tired = 0;
  }
  if(jogador->direction == BOTTOM){
    jogador->posY += jogador->vel;
    jogador->tired = 0;
  }
  if(jogador->direction == LEFT){
    jogador->posX -= jogador->vel;
    jogador->tired = 0;
  }
  if(jogador->direction == RIGHT){
    jogador->posX += jogador->vel;
    jogador->tired = 0;
  }

  jogador->direction = STOP; 
}

void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames){
  int oldDirection = jogador->direction; 

  if (keys[ALLEGRO_KEY_UP] || keys[ALLEGRO_KEY_W]){
    if ((jogador->posY - SIZE_OBJS - MARGIN_TOP) > 0){
      jogador->direction = UP;

      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_DOWN] || keys[ALLEGRO_KEY_S]){
    if (jogador->posY < HEIGHT - SIZE_OBJS - MARGIN_TOP){
      jogador->direction = BOTTOM;
      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_LEFT] || keys[ALLEGRO_KEY_A]){
    if (jogador->posX > SIZE_OBJS){
      jogador->direction = LEFT;
      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_RIGHT] || keys[ALLEGRO_KEY_D]){
    if (jogador->posX < WIDTH - 2 * SIZE_OBJS){
      jogador->direction = RIGHT;
      jogador->tired++;
    }
  }

  if (keys[ALLEGRO_KEY_ESCAPE])
    *done = true;

  for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
    keys[i] &= KEY_SEEN;

  // Verifica se o jogador mudou de direcao
  if (oldDirection != jogador->direction)
    jogador->animacaoAtual=0; 
  else{
    if (frames % 10 == 0)
      jogador->animacaoAtual++;
    if (jogador->animacaoAtual == 7)
      jogador->animacaoAtual = 0;
  }

  redraw = true;
}

// Função de desenho principal
void draw(bool redraw, long frames, tNivel *infoNivel){
  drawHeader(infoNivel);
  drawMap(infoNivel->mapa, infoNivel->objetos_mapa, frames, 22, 40);
  drawPlayer(infoNivel->jogador, infoNivel->mapa, infoNivel->objetos_mapa, frames);
  al_flip_display();
  redraw = false;
}

void drawPlayer(tPlayer *jogador, int **mapa, tObjetos *obj, long frames){
  switch (jogador->direction){
  case UP:
    al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);

    break;
  case BOTTOM:
    al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case LEFT:
    al_draw_scaled_bitmap(jogador->animacaoEsq[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);

    break;
  case RIGHT:
    al_draw_scaled_bitmap(jogador->animacaoDir[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case STOP:
    al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
    break; 
  }
  atualiza_player(jogador,1);
  
}

void drawEndGame(tNivel *infoNivel)
{
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "F I M D E J O G O");
  // al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 100 + 2 * SIZE_OBJS, 0, "PONTUACAO: %d",jogador->pontuacao);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 160 + 2 * SIZE_OBJS, 0, "Placar de lideres");
  // for(int i = 0;i < 5;i++)
  // al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 220 + (i*40) + 2 * SIZE_OBJS, 0, "%d: %d pts", i + 1, pontos_totais->score[i]);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 5 * SIZE_OBJS, 500 + 2 * SIZE_OBJS, 0, "Pressione ESC para sair...");
  al_flip_display();
}

// ARRUMAR TAMANHO DISSO
void drawInstructions(tNivel *infoNivel)
{
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
  al_draw_textf(infoNivel->font, al_map_rgb(120, 120, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "INSTRUCOES");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 80 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Colete todos os diamantes e chegue na saida");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 100 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "antes que o tempo acabe. Cuidado com as");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 120 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "pedras, elas podem cair em cima voce! ");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 160 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Utilize as setas do teclado para se movimentar");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 190 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "ou as teclas W,S,A,D");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 210 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "para finalizar o jogo pressione ESC.");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 550, ALLEGRO_ALIGN_LEFT, "Jogo desenvolvido por Rubens Laszlo ");
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 600, ALLEGRO_ALIGN_LEFT, "Abril de 2022.");
  al_flip_display();
}

// TODO AJUSTAR ALINHAMENTO
void drawHeader(tNivel *infoNivel)
{
  int vidas = 7;

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", 20);
  al_draw_bitmap(infoNivel->objetos_mapa->diamante[0], 0, 8, 0);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", 4, MIN_DIAMANTES);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH - 800, 10, 0, "%d", infoNivel->relogio);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH - 600, 10, 0, "Vidas: %d", vidas);
  al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH - 250, 10, 0, "Help: H/F1");
}
