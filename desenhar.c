#include "desenhar.h"
#include <stdio.h>

// Função de desenho principal
void desenhaTela(bool redraw, long frames, tNivel *infoNivel, tGame *game)
{
  desenhaHeader(game->font, infoNivel->relogio, infoNivel->jogador->vidas, game->nivelAtual->pontos, infoNivel->jogador->diamantes, infoNivel->objetosMapa->qtDiamantes, game->texturas, game->level);
  desenhaMapa(infoNivel->mapa, infoNivel->objetosMapa, frames, 22, 40, game->texturas);
  desenhaJogador(game->texturas, infoNivel->jogador->lin, infoNivel->jogador->col, infoNivel->jogador->direction, infoNivel->jogador->animacaoAtual);
  al_flip_display();
  redraw = false;
}

void desenhaMapa(int **mapa, tObjetos *objetosMapa, long frames, int col, int lin, tTexturas *texturas)
{
  int i, j, scaledI, scaledJ;
  for (i = 0; i < col; i++)
  {
    for (j = 0; j < lin; j++)
    {
      scaledI = (i * SIZE_OBJS) + MARGIN_TOP;
      scaledJ = j * SIZE_OBJS;

      switch (mapa[i][j])
      {
      case METAL:
        al_draw_scaled_bitmap(texturas->metal, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case TERRA:
        al_draw_scaled_bitmap(texturas->terra, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case MURO:
        al_draw_scaled_bitmap(texturas->muro, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case PEDRA:
        al_draw_scaled_bitmap(texturas->pedra, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case VAZIO:
        al_draw_scaled_bitmap(texturas->vazio, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case SAIDA:
        if (objetosMapa->saidaAtiva)
        {
          al_draw_scaled_bitmap(texturas->saida, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        }
        else
        {
          al_draw_scaled_bitmap(texturas->metal, 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        }
        break;
      case DIAMANTE:
        al_draw_scaled_bitmap(texturas->diamante[objetosMapa->animacaoLonga], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case EXPLOSAO:
        al_draw_scaled_bitmap(texturas->explosao[0], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 10 == 0)
          mapa[i][j] = EXPLOSAO2;
        break;
      case EXPLOSAO2:
        al_draw_scaled_bitmap(texturas->explosao[1], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 10 == 0)
          mapa[i][j] = EXPLOSAO3;
        break;
      case EXPLOSAO3:
        al_draw_scaled_bitmap(texturas->explosao[2], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        if (frames % 10 == 0)
          mapa[i][j] = VAZIO;
        break;
      case FIREFLY:
        al_draw_scaled_bitmap(texturas->vagalume[objetosMapa->animacaoCurta], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case BUTTERFLY:
        al_draw_scaled_bitmap(texturas->borboleta[objetosMapa->animacaoCurta], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      case AMOEBA:
        al_draw_scaled_bitmap(texturas->amoeba[objetosMapa->animacaoLonga], 0, 0, 15, 16, scaledJ, scaledI, SIZE_OBJS, SIZE_OBJS, 0);
        break;
      }
    }
  }

  // Reseta frames do diamante se necessário e faz animação
  if (frames % 10 == 0)
  {
    objetosMapa->animacaoCurta++;
    objetosMapa->animacaoLonga++;
  }

  if (objetosMapa->animacaoLonga == 8)
    objetosMapa->animacaoLonga = 0;
  if (objetosMapa->animacaoCurta == 4)
    objetosMapa->animacaoCurta = 0;
}

void desenhaJogador(tTexturas *texturas, int lin, int col, int direcao, int animacaoAtual)
{

  int offsetI, offsetJ;
  offsetJ = SIZE_OBJS * col;
  offsetI = (SIZE_OBJS * lin) + MARGIN_TOP;

  switch (direcao)
  {
  case UP:
    al_draw_scaled_bitmap(texturas->jogadorParado[animacaoAtual], 0, 0, 15, 16, offsetJ, offsetI, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case BOTTOM:
    al_draw_scaled_bitmap(texturas->jogadorParado[animacaoAtual], 0, 0, 15, 16, offsetJ, offsetI, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case LEFT:
    al_draw_scaled_bitmap(texturas->jogadorEsq[animacaoAtual], 0, 0, 15, 16, offsetJ, offsetI, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case RIGHT:
    al_draw_scaled_bitmap(texturas->jogadorDir[animacaoAtual], 0, 0, 15, 16, offsetJ, offsetI, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  case STOP:
    al_draw_scaled_bitmap(texturas->jogadorParado[animacaoAtual], 0, 0, 15, 16, offsetJ, offsetI, SIZE_OBJS, SIZE_OBJS, 0);
    break;
  }
}

void desenhaFim(ALLEGRO_FONT *fonte, int totalPontos, int *pontuacoes, int nPontuacoes)
{

  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.6));
  al_draw_textf(fonte, al_map_rgb(255, 255, 161), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "FIM DO JOGO");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 80 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "SEU SCORE FOI: %d", totalPontos);

  al_draw_textf(fonte, al_map_rgb(255, 255, 161), WIDTH / 4 + 7 * SIZE_OBJS, 160 + 2 * SIZE_OBJS, 0, "HIGHSCORES");
  for (int i = 0; i < nPontuacoes+1 && i < 10; i++)
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 180 + 25 * i + 2 * SIZE_OBJS, 0, "%d: %d", i + 1, pontuacoes[i]);
  

  al_flip_display();
}

void desenhaInstrucoes(ALLEGRO_FONT *fonte)
{
  al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.6));
  al_draw_textf(fonte, al_map_rgb(120, 120, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "INSTRUCOES");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 80 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Colete todos os diamantes e chegue na saida que sera aberta,");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 110 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "antes que o tempo acabe. Cuidado com as pedras,");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 140 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "elas podem cair em cima voce, perdendo uma vida. ");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 170 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Utilize as setas do teclado para se movimentar");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 200 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "ou as teclas W,S,A,D. ");

  al_draw_textf(fonte, al_map_rgb(120, 120, 255), WIDTH / 4 + 7 * SIZE_OBJS, 240 + 2 * SIZE_OBJS, 0, "DICAS");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 300 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "O desabamento de pedras sobre monstros geram explosoes");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 330 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Vagalumes se movimentam no sentido horario.");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 360 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Borboletas se movimentam no sentido anti-horario e");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 390 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "geram diamantes em explosoes. ");

  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 570, ALLEGRO_ALIGN_LEFT, "Jogo desenvolvido por Rubens Laszlo ");
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 120, 600, ALLEGRO_ALIGN_LEFT, "Abril de 2022.");
  al_flip_display();
}

void desenhaHeader(ALLEGRO_FONT *fonte, int relogio, int vidas, int pontuacao, int diamantesJogador, int diamantesTotal, tTexturas *texturas, int nivel)
{
  al_clear_to_color(al_map_rgb(0, 0, 0));

  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, "NIVEL %d", nivel);

  al_draw_bitmap(texturas->arrowDown, 140, 10, 0);
  al_draw_bitmap(texturas->arrowUp, 180, 10, 0);

  al_draw_bitmap(texturas->diamante[0], 220, 10, 0);
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), 250, 10, 0, "%d/%d", diamantesJogador, diamantesTotal);

  al_draw_textf(fonte, al_map_rgb(245, 233, 66), WIDTH / 2, 10, 0, "%05d", pontuacao);
  al_draw_textf(fonte, al_map_rgb(245, 233, 66), WIDTH / 2 - 150, 10, 0, "%d", relogio);
  al_draw_textf(fonte, al_map_rgb(255, 0, 34), 700, 10, 0, "Vidas: %d", vidas);
  al_draw_textf(fonte, al_map_rgb(255, 255, 255), WIDTH - 250, 10, 0, "Help: H/F1");
  al_draw_bitmap(texturas->creeper, WIDTH - 50, 10, 0);
}
