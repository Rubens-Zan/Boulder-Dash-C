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

void testaAllegro(bool ok, char *descricao){
  if (ok)
    return;
  fprintf(stderr, "Não foi possivel inicializar %s\n", descricao);
  exit(1);
}

void inicializarAllegro(tNivel *infoNivel){

  testaAllegro(al_init(), "allegro");
  testaAllegro(al_install_keyboard(), "keyboard");

  timer = al_create_timer(1.0 / FRAMERATE);
  testaAllegro(timer, "timer");


  queue = al_create_event_queue();
  testaAllegro(queue, "queue");

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

tAudio * iniciaAudio(){
  tAudio *sons;
  sons = malloc(sizeof(tAudio)); 
  if (sons == NULL){
    printf("Erro ao alocar memoria!\n");
  	exit(1);
  }

  /* Sound samples, instances; basicly all necessary loading stuff */
  sons->sound_walk_earth  = al_load_sample("resources/audios/walk_earth.ogg");
  sons->sound_walk_empty = al_load_sample("resources/audios/walk_empty.ogg");
  sons->sound_boulder = al_load_sample("resources/audios/bg_music.ogg");
  sons->sound_diamond = al_load_sample("resources/audios/collect_diamond.ogg");
  sons->sound_melody = al_load_sample("resources/audios/melody.ogg");

  sons->walk_empty = al_create_sample_instance(sons->sound_walk_empty);
  sons->walk_earth = al_create_sample_instance(sons->sound_walk_earth);
  sons->boulder = al_create_sample_instance(sons->sound_boulder);
  sons->collect_diamond = al_create_sample_instance(sons->sound_diamond);
  sons->music = al_create_sample_instance(sons->sound_melody);

  al_set_sample_instance_playmode(sons->music, ALLEGRO_PLAYMODE_LOOP);
  al_attach_sample_instance_to_mixer(sons->walk_empty,al_get_default_mixer());
  al_attach_sample_instance_to_mixer(sons->walk_earth,al_get_default_mixer());
  al_attach_sample_instance_to_mixer(sons->boulder,al_get_default_mixer());
  al_attach_sample_instance_to_mixer(sons->collect_diamond,al_get_default_mixer());
  al_attach_sample_instance_to_mixer(sons->music,al_get_default_mixer());

  return sons; 
}

void state_init(tNivel *infoNivel){
  inicializarAllegro(infoNivel);
  infoNivel->jogador = inicia_jogador(infoNivel->sheet);
  infoNivel->objetosMapa = iniciaObjetos(infoNivel->sheet);
  
  infoNivel->mapa = iniciaMapa(PATH_MAP_1, infoNivel->objetosMapa,infoNivel->jogador);
  infoNivel->relogio=150; 
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

void state_play(tNivel *infoNivel){
  bool done = false;
  bool redraw = true;
  long frames = 0;
  int morreu,ganhou = 0;
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
      movimentaObjetos(infoNivel->mapa, infoNivel->jogador, infoNivel->jogador->direction, infoNivel->objetosMapa, frames, infoNivel->sonsJogo);
      if (testaMapa(infoNivel->mapa, infoNivel->jogador, infoNivel->objetosMapa, frames, infoNivel->sonsJogo))
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

int testaMapa(int **mapa, tPlayer *jogador, tObjetos *objetos, long frames, tAudio *sons)
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
    ok = testaObjetosCaminho(jogador, mapa, objetos, linAtual, colAtual, verticalOffset, horizontalOffset,sons,frames);
  }

  return ok;
}

int testaObjetosCaminho(tPlayer *jogador, int **mapa, tObjetos *objetos, int linAtual, int colAtual, int vertical, int horizontal, tAudio *sons,long frames){

  int col = colAtual + horizontal;
  int lin = linAtual + vertical;
  int *pos = &mapa[lin][col];
  
  if (*pos == VAZIO){
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;
    // al_play_sample_instance(sons->walk_empty);
    return 1;
  }
  if (*pos == TERRA ){
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;

    // al_play_sample_instance(sons->walk_earth);
    return 1;
  }

  if (*pos == DIAMANTE){
    destroiRocha(objetos, mapa, lin, col,sons);
    jogador->pontuacao += 10;
    jogador->diamantes += 1;
    *pos = PLAYER;
    mapa[linAtual][colAtual] = VAZIO;

    if (objetos->qtDiamantes == jogador->diamantes)
      criaSaida(mapa);
    return 1;
  }
  if (*pos == PEDRA){
    if (mapa[linAtual][colAtual+2*horizontal] == VAZIO && frames % 20 == 0){
      empurrarPedra(mapa, objetos, horizontal,lin,col, sons);
      *pos = PLAYER;
      mapa[linAtual][colAtual] = VAZIO;
      mapa[linAtual][colAtual+2*horizontal] = PEDRA; 

      return 1; 
    }
    
  }

  if (*pos == SAIDA){
    printf("ok ganhou "); 
  }

  return 0;
}

// void proxNivel(){

// }

void explodeEmVolta(int **mapa,tObjetos *objetos,int lin, int col, tAudio *sons){

  for (int i=-1;i<2;i++){
    for (int j=-1;j<2;j++){
      if (linhaEColunaValidas(lin+i,col+j) && mapa[lin+i][col+j] != METAL && mapa[lin+i][col+j] != DIAMANTE){
        if (mapa[lin+i][col+j] == PEDRA){
          destroiRocha(objetos,mapa,lin+i,col+j,sons); 
        }
        mapa[lin+i][col+j] = EXPLOSAO;


      }
    }
  }

}

void mataPlayer(tPlayer *jogador, int lin, int col, int **mapa, tObjetos *objetos, tAudio *sons){
  jogador->vidas--;

  explodeEmVolta(mapa, objetos,lin,col,sons); 

  jogador->col = jogador->colInicial;
  jogador->lin = jogador->linInicial; 

  mapa[jogador->linInicial][jogador->colInicial] = PLAYER;
}

void destroiRocha(tObjetos *objetos, int **mapa, int lin, int col, tAudio *sons){
  // al_play_sample_instance(sons->collect_diamond);

  for (int i = 0; i < objetos->totalRochas; i++){
    rochedos *rochedoAtual = &objetos->rochedos[i];

    if (rochedoAtual->ativo && rochedoAtual->col == col && rochedoAtual->lin == lin)
      rochedoAtual->ativo = false;
  }

}

void empurrarPedra(int **mapa,tObjetos *objetos,int direcao, int lin, int col, tAudio *sons){
  for (int i = 0; i < objetos->totalRochas; i++){
    rochedos *rochedoAtual = &objetos->rochedos[i];

    if (rochedoAtual->ativo && rochedoAtual->col == col && rochedoAtual->lin == lin){
      rochedoAtual->col += direcao;
    }
    
  }
}

void criaSaida(int **mapa){
  mapa[16][38] = SAIDA;
}

void movimentaObjetos(int **mapa, tPlayer *jogador, int direcao, tObjetos *objetos, long frames, tAudio *sons){
  int lin,col; 

  if (frames % 10 == 0)
  {
    for (int i = 0; i < objetos->totalRochas; i++)
    {
      rochedos *rochedoAtual = &objetos->rochedos[i];

      // checa apenas para pedras que ainda nao foram destruidas
      if (rochedoAtual->ativo)
      {

        lin = rochedoAtual->lin;
        col=rochedoAtual->col;
        verificaRolamento(mapa, objetos, col, lin, i);

        // Testa se deve continuar caindo
        if (rochedoAtual->caindo == 1){
          // Se a pedra esta caindo e o player esta em baixo mata ele
          if (mapa[lin + 1][col] == PLAYER){
            mataPlayer(jogador, lin+1, col, mapa,objetos,sons);
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
  if (lin > 0 && lin <= TOTAL_LINHAS && col > 0 && col <= TOTAL_COLUNAS)
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
