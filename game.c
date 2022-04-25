#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allegro5/allegro_ttf.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "game.h"
#include "mapa.h"
#include "texturas.h"
#include "desenhar.h"
#include "mecanicas.h"

// GLOBAIS
ALLEGRO_TIMER *timer;
ALLEGRO_EVENT event;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;

unsigned char keys[ALLEGRO_KEY_MAX];

// Funcoes para inicializacao do jogo

void testaAllegro(bool ok, char *descricao)
{
  if (ok)
    return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void inicializarAllegro(tGame *game)
{

  testaAllegro(al_init(), "allegro");
  testaAllegro(al_install_keyboard(), "keyboard");

  timer = al_create_timer(1.0 / FRAMERATE);
  testaAllegro(timer, "timer");

  queue = al_create_event_queue();
  testaAllegro(queue, "queue");

  // Pega spritesheet
  testaAllegro(al_init_image_addon(), "image addon");
  game->sheet = al_load_bitmap("resources/img/spritesheet.png");

  // Pega áudios
  testaAllegro(al_install_audio(), "audio");
  testaAllegro(al_init_acodec_addon(), "audio codecs");
  testaAllegro(al_reserve_samples(20), "reserve samples");

  al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
  al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
  al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

  disp = al_create_display(WIDTH, HEIGHT);

  // Configura o título da janela
  al_set_window_title(disp, "Boulder Dash Game by Rubens Zan");

  // Torna apto o uso de mouse na aplicação
  if (!al_install_mouse())
  {
    fprintf(stderr, "Falha ao inicializar o mouse.\n");
    al_destroy_display(disp);
  }

  // Atribui o cursor padrão do sistema para ser usado
  if (!al_set_system_mouse_cursor(disp, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
  {
    fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
    al_destroy_display(disp);
  }

  // Inicia fontes
  testaAllegro(al_init_font_addon(), "fonte");
  testaAllegro(al_init_ttf_addon(), "fonte");
  game->font = al_create_builtin_font();
  game->font = al_load_ttf_font("resources/font/zig.ttf", 20, 0);

  testaAllegro(al_init_primitives_addon(), "primitives");

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_mouse_event_source());
}

tAudio *iniciaAudio()
{
  tAudio *audio = malloc(sizeof(tAudio));

  // inicia as samples
  audio->soundWalkEarth = al_load_sample("resources/audios/walk_earth.ogg");
  audio->soundWalkEmpty = al_load_sample("resources/audios/walk_empty.ogg");
  audio->soundDiamond = al_load_sample("resources/audios/collect_diamond.ogg");
  audio->soundMelody = al_load_sample("resources/audios/melody.ogg");
  audio->soundStart = al_load_sample("resources/audios/bg_music.ogg");
  audio->soundExplosion = al_load_sample("resources/audios/explosion.ogg");
  audio->soundDoor = al_load_sample("resources/audios/door.wav");

  // // inicia as samples instances com as samples
  audio->door = al_create_sample_instance(audio->soundDoor);
  audio->explosion = al_create_sample_instance(audio->soundExplosion);
  audio->walkEmpty = al_create_sample_instance(audio->soundWalkEmpty);
  audio->walkEarth = al_create_sample_instance(audio->soundWalkEarth);
  audio->collectDiamond = al_create_sample_instance(audio->soundDiamond);
  audio->music = al_create_sample_instance(audio->soundMelody);
  audio->startingMusic = al_create_sample_instance(audio->soundStart);

  // // seta as samples instances para serem tocadas nos eventos
  al_attach_sample_instance_to_mixer(audio->door, al_get_default_mixer());
  al_set_sample_instance_playmode(audio->music, ALLEGRO_PLAYMODE_LOOP);
  al_set_sample_instance_playmode(audio->startingMusic, ALLEGRO_PLAYMODE_LOOP);
  al_attach_sample_instance_to_mixer(audio->walkEmpty, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(audio->walkEarth, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(audio->explosion, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(audio->collectDiamond, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(audio->music, al_get_default_mixer());
  al_attach_sample_instance_to_mixer(audio->startingMusic, al_get_default_mixer());

  return audio;
}

char *pegaPath(int level)
{
  int levelStrSize = level / 10;
  char levelStr[levelStrSize + 1];
  sprintf(levelStr, "%d", level);
  char str1[21] = "resources/mapas/mapa", str2[5] = ".txt";
  char *path = (char *)malloc(strlen(str1) + strlen(str2) + strlen(levelStr) + 1);
  strcpy(path, str1);
  strcat(path, levelStr);
  strcat(path, str2);

  return path;
}
// Funcao para inicializacao do jogador
tPlayer *iniciaJogador()
{
  tPlayer *jogador;
  jogador = malloc(sizeof(tPlayer));

  if (jogador == NULL)
  {
    printf("Erro ao alocar memoria!\n");
    exit(1);
  }

  jogador->vel = 1;
  jogador->animacaoAtual = jogador->diamantes = 0;
  jogador->vidas = 7;
  return jogador;
}

tObjetos *iniciaObjetos()
{
  tObjetos *obj;
  obj = malloc(sizeof(tObjetos));
  if (obj == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória!\n");
    exit(1);
  }

  obj->saidaAtiva = 0;
  obj->animacaoCurta = 0;
  obj->animacaoLonga = 0;
}

// O easter egg vai explodir todas as pedras
void executaEasterEgg(int **mapa, rochedos *rochas, int totalRochas, tAudio *sons){
  for (int i=0;i< 22;i++){
    for (int j=0;j<40;j++){
      if (mapa[i][j] == PEDRA){
        mapa[i][j] = EXPLOSAO;
        al_play_sample_instance(sons->explosion);
      }
    }
  }

  for (int k=0;k < totalRochas;k++){
    if (rochas[k].tipo == PEDRA && rochas[k].ativo)
      rochas[k].ativo = false; 
  }
}

// Funcao para inicializacao do nivel
tNivel *iniciaNivel(int level)
{
  tNivel *nivel = malloc(sizeof(tNivel));

  nivel->jogador = iniciaJogador();
  nivel->objetosMapa = iniciaObjetos();
  nivel->pontos = 0;

  char *path = pegaPath(level);
  nivel->mapa = iniciaMapa(path, nivel->objetosMapa, nivel->jogador);
  nivel->relogio = 150;
}

void destroiNivel(tNivel *nivel)
{
  free(nivel->jogador);
  free(nivel->mapa[0]);
  free(nivel->mapa);
  free(nivel->objetosMapa);
  free(nivel);
}

void alteraNivel(tGame *game, int novo, int venceu)
{

  if (novo > 10)
  {
    fprintf(stderr, "Não existem niveis apos esse\n");
  }
  else if (novo < 1)
  {
    fprintf(stderr, "Nao existem niveis anteriores \n");
  }
  else
  {
    if (!venceu)
    {
      game->pontuacao[game->level - 1] = 0;
    }
    else
    {
      // jogador ganha ponto extras conforme a velocidade para completar o nivel
      game->pontuacao[game->level - 1] += game->nivelAtual->relogio * 2;
    }

    destroiNivel(game->nivelAtual);

    game->nivelAtual = iniciaNivel(novo);
    game->level = novo;
  }
}

// ESTADOS DO JOGO
void state_init(tGame *game)
{

  inicializarAllegro(game);
  game->texturas = inicializaTexturas(game->sheet);
  game->sonsJogo = iniciaAudio();

  game->level = 1;
  for (int i = 0; i < 10; i++)
  {
    game->pontuacao[i] = 0;
  }

  game->nivelAtual = iniciaNivel(game->level);
  game->state = JOGANDO;
}

void state_serve(tGame *game)
{
  bool done = false;
  al_flush_event_queue(queue);
  while (1)
  {
    drawInstructions(game->font);
    al_wait_for_event(queue, &event);
    if (al_is_event_queue_empty(queue))
      drawInstructions(game->font);
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
      game->state = JOGANDO;
      done = true;
    }
    // Caso ESC seja pressionado, fim partida
    else if (keys[ALLEGRO_KEY_ESCAPE])
    {
      keys[ALLEGRO_KEY_ESCAPE] = 0;
      game->state = FIMPART;
      done = true;
    }
    if (done)
      break;
  }
}

void state_play(tGame *game)
{
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu, ganhou = 0;
  al_flush_event_queue(queue);
  memset(keys, 0, sizeof(keys));
  al_start_timer(timer);

  al_play_sample_instance(game->sonsJogo->music);

  while (1)
  {
    al_wait_for_event(queue, &event);

    switch (event.type)
    {
    // Indica que um botao do mouse foi pressionado.
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      // checa se o clique eh no botao down
      if (event.mouse.x > 130 && event.mouse.x <= 160 && event.mouse.y <= 30)
      {
        alteraNivel(game, game->level - 1, 0);
      }
      // checa se o clique eh no botao up
      else if (event.mouse.x >= 170 && event.mouse.x <= 200 && event.mouse.y <= 30)
      {
        alteraNivel(game, game->level + 1, 0);
      }
      else if (event.mouse.x >= WIDTH- 100 && event.mouse.x <= WIDTH && event.mouse.y <= 30){
        executaEasterEgg(game->nivelAtual->mapa,game->nivelAtual->objetosMapa->rochedos,game->nivelAtual->objetosMapa->totalRochas, game->sonsJogo); 
      }

      break;
    case ALLEGRO_EVENT_TIMER:
      verificaEntrada(keys, &done, redraw, game->nivelAtual->jogador, frames);
      movimentaObjetos(game->nivelAtual->mapa, game->nivelAtual->jogador, game->nivelAtual->jogador->direction, game->nivelAtual->objetosMapa, frames, game->sonsJogo);
      if (testaMapa(game->nivelAtual->mapa, game->nivelAtual->jogador, game->nivelAtual->objetosMapa, frames, game->sonsJogo, game))
        atualizaPlayer(game->nivelAtual->jogador);
      if (frames % 60 == 0 && game->nivelAtual->relogio > 0)
      {
        game->nivelAtual->relogio--;
      }
      break;
    case ALLEGRO_EVENT_KEY_DOWN:
      keys[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      keys[event.keyboard.keycode] &= KEY_RELEASED;
      break;
    // Indica que o botao de fechar da janela foi pressionado.
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      done = true;
      break;
    }
    // Vai para menu de ajuda
    if (keys[ALLEGRO_KEY_H] || keys[ALLEGRO_KEY_F1])
    {
      keys[ALLEGRO_KEY_H] = 0;
      keys[ALLEGRO_KEY_F1] = 0;
      game->state = SERVINDO;
      done = true;
    }
    if (redraw && al_is_event_queue_empty(queue))
      draw(redraw, frames, game->nivelAtual, game);

    if (game->nivelAtual->relogio == 0 || game->nivelAtual->jogador->vidas == 0)
    {
      alteraNivel(game, game->level, 0);
    }
    if (done)
      break;

    frames++;
  }
}

void state_end(tGame *game)
{
  destroiNivel(game->nivelAtual);
}

int testaMapa(int **mapa, tPlayer *jogador, tObjetos *objetos, long frames, tAudio *sons, tGame *game)
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
    ok = testaObjetosCaminho(jogador, mapa, objetos, linAtual, colAtual, verticalOffset, horizontalOffset, sons, frames, game);
  }

  return ok;
}

int testaObjetosCaminho(tPlayer *jogador, int **mapa, tObjetos *objetos, int linAtual, int colAtual, int vertical, int horizontal, tAudio *sons, long frames, tGame *game)
{

  int col = colAtual + horizontal;
  int lin = linAtual + vertical;
  int *pos = &mapa[lin][col];

  if (*pos == VAZIO)
  {
    al_play_sample_instance(sons->walkEmpty);
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;
    return 1;
  }

  if (*pos == TERRA)
  {
    al_play_sample_instance(sons->walkEarth);
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;
    return 1;
  }

  if (*pos == DIAMANTE)
  {
    al_play_sample_instance(sons->collectDiamond);

    destroiRocha(objetos, mapa, lin, col, sons);
    game->nivelAtual->pontos += 10;
    game->pontuacao[game->level - 1] += 10;

    jogador->diamantes += 1;
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;

    if (objetos->qtDiamantes == jogador->diamantes)
      criaSaida(objetos, sons);
    return 1;
  }

  if (*pos == PEDRA)
  {
    if (mapa[linAtual][colAtual + 2 * horizontal] == VAZIO && frames % 20 == 0)
    {
      empurrarPedra(mapa, objetos, horizontal, lin, col, sons);
      *pos = PLAYER;
      mapa[linAtual][colAtual] = VAZIO;
      mapa[linAtual][colAtual + 2 * horizontal] = PEDRA;

      return 1;
    }
  }

  if (*pos == SAIDA && objetos->saidaAtiva)
  {
    alteraNivel(game, game->level + 1, 1);
  }

  return 0;
}

void verificaEntrada(unsigned char *keys, bool *done, bool redraw, tPlayer *jogador, long frames)
{
  int oldDirection = jogador->direction;

  // VERIFICA A DIRECAO E
  // VERIFICA SE NAO ESTA NAS BORDAS
  if (keys[ALLEGRO_KEY_UP] || keys[ALLEGRO_KEY_W])
  {
    if (jogador->lin > 0)
    {
      jogador->direction = UP;
    }
  }
  else if (keys[ALLEGRO_KEY_DOWN] || keys[ALLEGRO_KEY_S])
  {
    if (jogador->lin < TOTAL_LINHAS)
    {
      jogador->direction = BOTTOM;
    }
  }
  else if (keys[ALLEGRO_KEY_LEFT] || keys[ALLEGRO_KEY_A])
  {
    if (jogador->col > 0)
    {
      jogador->direction = LEFT;
    }
  }
  else if (keys[ALLEGRO_KEY_RIGHT] || keys[ALLEGRO_KEY_D])
  {
    if (jogador->col < TOTAL_COLUNAS)
    {
      jogador->direction = RIGHT;
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

void criaSaida(tObjetos *obj, tAudio *sons)
{

  al_play_sample_instance(sons->door);
  obj->saidaAtiva = 1;
}

int linhaEColunaValidas(int lin, int col)
{
  if (lin > 0 && lin <= TOTAL_LINHAS && col > 0 && col <= TOTAL_COLUNAS)
    return 1;
  return 0;
}