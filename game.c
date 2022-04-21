#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro_ttf.h"
#include "game.h"
#include "mapa.h"
#include "texturas.h"
#include "desenhar.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// GLOBAIS
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;

unsigned char keys[ALLEGRO_KEY_MAX];

// audio *sons_jogo;
// pontos *pontos_totais;

long frames = 0;

void testaAllegro(bool ok, char *descricao)
{
  if (ok)
    return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void inicializarAllegro(tNivel *infoNivel)
{

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

void state_init(tNivel *infoNivel){

  inicializarAllegro(infoNivel);
  infoNivel->jogador = inicia_jogador(infoNivel->sheet);
  infoNivel->objetosMapa = iniciaObjetos(infoNivel->sheet);
  infoNivel->mapa = iniciaMapa(PATH_MAP_1, infoNivel->objetosMapa);
  infoNivel->relogio= 150; 
  infoNivel->state = JOGANDO;
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
      infoNivel->state = JOGANDO;
      done = true;
    }
    // Caso ESC seja pressionado, fim partida
    else if (keys[ALLEGRO_KEY_ESCAPE])
    {
      keys[ALLEGRO_KEY_ESCAPE] = 0;
      infoNivel->state = FIMPART;
      done = true;
    }
    if (done)
      break;
  }
}

void state_play(tNivel *infoNivel)
{
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu = 0, ganhou = 0;
  al_flush_event_queue(queue);
  memset(keys, 0, sizeof(keys));
  al_start_timer(timer);

  while (1)
  {
    al_wait_for_event(queue, &event);

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
      verificaEntrada(keys, &done, redraw, infoNivel->jogador, frames);
      verificaPedras(infoNivel->mapa, infoNivel->jogador, infoNivel->jogador->direction, infoNivel->objetosMapa, frames);
      if (testaMapa(infoNivel->mapa, infoNivel->jogador, infoNivel->objetosMapa, frames))
        atualizaPlayer(infoNivel->jogador);
      if (frames % 60 == 0 && infoNivel->jogador->vivo && infoNivel->relogio > 0){
        infoNivel->relogio--;
      }
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
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1])
    {
      keys[ALLEGRO_KEY_H] = 0;
      keys[ALLEGRO_KEY_F1] = 0;
      infoNivel->state = SERVINDO;
      done = true;
    }
    if (redraw && al_is_event_queue_empty(queue))
      draw(redraw, frames, infoNivel);

    if (done)
    {
      break;
    }
    frames++;
  }
}

int testaMapa(int **mapa, tPlayer *jogador, tObjetos *objetos, long frames)
{
  int colAtual, linAtual, horizontalOffset, verticalOffset, ok;
  // Coordenadas do personagem dentro do mapa
  colAtual = jogador->col;
  linAtual = jogador->lin;
  horizontalOffset = verticalOffset = ok = 0;
  bool andou = false;

  switch (jogador->direction)
  {
  case UP:
    verticalOffset = -1;
    andou = true;
    break;
  case BOTTOM:
    verticalOffset = 1;
    andou = true;
    break;
  case LEFT:
    andou = true;
    horizontalOffset = -1;
    break;
  case RIGHT:
    horizontalOffset = 1;
    andou = true;
    break;
  }

  // verifica se andou
  if (andou && frames % 10 == 0)
  {
    ok = testaObjetosCaminho(jogador, mapa, objetos, linAtual, colAtual, verticalOffset, horizontalOffset);
  }

  return ok;
}

int testaObjetosCaminho(tPlayer *jogador, int **mapa, tObjetos *objetos, int linAtual, int colAtual, int vertical, int horizontal){

  int col = colAtual + horizontal;
  int lin = linAtual + vertical;
  int *pos = &mapa[lin][col];

  if (*pos == TERRA || *pos == VAZIO){
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;
    return 1;
  }
  if (*pos == DIAMANTE){
    coletaDiamante(jogador, objetos, mapa, lin, col);
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;
    return 1;
  }

  return 0;
}

void mataPlayer(tPlayer *jogador, int lin, int col, int **mapa)
{
  jogador->vidas--;
  mapa[lin][col] = EXPLOSAO;

  jogador->col = 3;
  jogador->lin = 2;

  mapa[2][3] = PLAYER;
}

void coletaDiamante(tPlayer *jogador, tObjetos *objetos, int **mapa, int lin, int col){
  jogador->pontuacao += 10;
  jogador->diamantes += 1;

  for (int i = 0; i < objetos->total; i++){
    rochedos *rochedoAtual = &objetos->rochedos[i];

    if (objetos->rochedos[i].ativo && rochedoAtual->col == col && rochedoAtual->lin == lin)
      rochedoAtual->ativo = false;
  }

  // TODO AJUSTAS OBJETO COM ROCHAS PARA QUE SEJA DESTRUIDO DIAMANTE COLETADO
  if (objetos->qtDiamantes == jogador->diamantes)
    criaSaida(mapa);
  
}

void criaSaida(int **mapa)
{
  mapa[16][38] = SAIDA;
}

void verificaPedras(int **mapa, tPlayer *jogador, int direcao, tObjetos *objetos, long frames)
{
  int lin,col; 

  if (frames % 10 == 0)
  {
    for (int i = 0; i < objetos->total; i++)
    {
      rochedos *rochedoAtual = &objetos->rochedos[i];

      // checa apenas para pedras que ainda nao foram destruidas
      if (rochedoAtual->ativo)
      {

        lin = rochedoAtual->lin;
        col=rochedoAtual->col;
        verificaRolamento(mapa, objetos, col, lin, i);

        // Testa se deve continuar caindo
        if (rochedoAtual->caindo == 1)
        {
          // Se a pedra esta caindo e o player esta em baixo mata ele
          if (mapa[lin + 1][col] == PLAYER)
          {
            mataPlayer(jogador, lin+1, col, mapa);
          }

          if (mapa[lin + 1][col] != VAZIO && mapa[lin + 1][col] != PLAYER && mapa[lin + 1][col]){
            rochedoAtual->caindo = 0;
          }
        }

        // Desabamento normal
        if (mapa[lin + 1][col] == VAZIO && (lin + 1 < 21))
        {
          rochedoAtual->caindo = 1;
          rochedoAtual->lin++;
          mapa[lin + 1][col] = rochedoAtual->tipo;
          mapa[lin][col] = VAZIO;
        }
      }
    }
  }
}

int linhaEColunaValidas(int lin,int col){
  if (lin > 0 && lin < TOTAL_LINHAS && col > 0 && col < TOTAL_COLUNAS)
    return 1;
  return 0; 
}

void verificaRolamento(int **mapa, tObjetos *objetos, int col, int lin, int rochaAtual){
  rochedos *rocha = &objetos->rochedos[rochaAtual]; 

  int pedraX = rocha->lin;
  int pedraY = rocha->col;
  int tipo = rocha->tipo;
  int direcao = 0; 
  
  if (lin > 0 && lin < TOTAL_LINHAS && col >0 && col < TOTAL_COLUNAS){
    // verifica se esta sobre uma rocha
    if (mapa[lin+1][col] == PEDRA || mapa[lin+1][col] == DIAMANTE){
      // verifica rolamento para a direita
      if (mapa[lin+1][col+1] == VAZIO && mapa[lin][col+1] == VAZIO){
        direcao=1; 
        rolaRocha(mapa,rocha,lin,col,direcao); 
      }
      // verifica o rolamento para esquerda
      if (mapa[lin+1][col-1] == VAZIO && mapa[lin][col-1] == VAZIO){
        direcao=-1; 
        rolaRocha(mapa, rocha,lin,col,direcao);       
      }
    }
  }
}

void rolaRocha(int **mapa,rochedos *rocha,int lin,int col,int direcao){
  mapa[lin][col] = VAZIO;
  mapa[lin+1][col + direcao] = rocha->tipo;
  
  rocha->caindo = 1;
  rocha->lin = lin + 1;
  rocha->col = col + direcao; 
}

void atualizaPlayer(tPlayer *jogador){
  
  // DECREMENTO/INCREMENTO EM RELACAO A VELOCIDADE
  if (jogador->direction == UP)
  {
    jogador->lin -= jogador->vel;
    jogador->tired = 0;
  }
  if (jogador->direction == BOTTOM)
  {
    jogador->lin += jogador->vel;
    jogador->tired = 0;
  }
  if (jogador->direction == LEFT)
  {
    jogador->col -= jogador->vel;
    jogador->tired = 0;
  }
  if (jogador->direction == RIGHT)
  {
    jogador->col += jogador->vel;
    jogador->tired = 0;
  }

  jogador->direction = STOP;
}

void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames)
{
  int oldDirection = jogador->direction;

  // VERIFICA A DIRECAO E
  // VERIFICA SE NAO ESTA NAS BORDAS
  if (keys[ALLEGRO_KEY_UP] || keys[ALLEGRO_KEY_W])
  {
    if (jogador->lin > 0){
      jogador->direction = UP;
      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_DOWN] || keys[ALLEGRO_KEY_S])
  {
    if (jogador->lin < TOTAL_LINHAS)
    {
      jogador->direction = BOTTOM;
      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_LEFT] || keys[ALLEGRO_KEY_A])
  {
    if (jogador->col > 0)
    {
      jogador->direction = LEFT;
      jogador->tired++;
    }
  }
  else if (keys[ALLEGRO_KEY_RIGHT] || keys[ALLEGRO_KEY_D])
  {
    if (jogador->col < TOTAL_COLUNAS)
    {
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
    jogador->animacaoAtual = 0;
  else
  {
    if (frames % 10 == 0)
      jogador->animacaoAtual++;
    if (jogador->animacaoAtual == 7)
      jogador->animacaoAtual = 0;
  }

  redraw = true;
}
