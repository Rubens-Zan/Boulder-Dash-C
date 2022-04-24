#include "mecanicas.h"
#include <stdio.h>
#include <stdlib.h>
#include "mecanicas.h"


// FUNCOES MOVIMENTAR JOGO
void movimentaObjetos(int **mapa, tPlayer *jogador, int direcao, tObjetos *objetos, long frames, tAudio *sons)
{
  int lin, col;

  if (frames % 10 == 0)
  {
    for (int i = 0; i < objetos->totalRochas; i++)
    {
      rochedos *rochedoAtual = &objetos->rochedos[i];

      // checa apenas para pedras que ainda nao foram destruidas
      if (rochedoAtual->ativo)
      {

        lin = rochedoAtual->lin;
        col = rochedoAtual->col;
        verificaRolamento(mapa, objetos, col, lin, i);

        // Testa se deve continuar caindo
        if (rochedoAtual->caindo == 1)
        {
          // Se a pedra esta caindo e o player esta em baixo mata ele
          if (mapa[lin + 1][col] == PLAYER)
          {
            mataPlayer(jogador, lin + 1, col, mapa, objetos, sons);
          }

          if (mapa[lin + 1][col] == BUTTERFLY)
          {
            mataMonstro(mapa, objetos, lin + 1, col, sons);
          }

          if (mapa[lin + 1][col] != VAZIO && mapa[lin + 1][col] != PLAYER && mapa[lin + 1][col])
          {
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

    for (int k = 0; k < objetos->qtMonstros; k++)
    {
      tMonstro *monstroAtual = &objetos->monstros[k];

      // checa apenas para pedras que ainda nao foram destruidas
      if (monstroAtual->ativo)
      {
        if (monstroAtual->tipo == BUTTERFLY || monstroAtual->tipo == FIREFLY)
        {
          movimentaMonstro(mapa, monstroAtual, jogador, objetos, sons);
        }
      }
    }
  }
}

void movimentaMonstro(int **mapa, tMonstro *monstro, tPlayer *jogador, tObjetos *objetos, tAudio *sons)
{
  int horizontal = 0, vertical = 0;
  if (monstro->direcaoAtual == RIGHT)
  {
    horizontal = 1;
  }
  else if (monstro->direcaoAtual == LEFT)
  {
    horizontal = -1;
  }
  else if (monstro->direcaoAtual == BOTTOM)
  {
    vertical = 1;
  }
  else if (monstro->direcaoAtual == UP)
  {
    vertical = -1;
  }

  if (linhaEColunaValidas(monstro->lin + vertical, monstro->col + horizontal) && (mapa[monstro->lin + vertical][monstro->col + horizontal] == VAZIO || mapa[monstro->lin + vertical][monstro->col + horizontal] == PLAYER))
  {
    if (mapa[monstro->lin + vertical][monstro->col + horizontal] == PLAYER)
    {
      mataPlayer(jogador, monstro->lin + vertical, monstro->col + horizontal, mapa, objetos, sons);
    }
    mapa[monstro->lin][monstro->col] = VAZIO;
    mapa[monstro->lin + vertical][monstro->col + horizontal] = monstro->tipo;
    monstro->lin += vertical;
    monstro->col += horizontal;
  }
  else
  {
    mudaDirecaoMonstro(monstro);
  }
}

void mudaDirecaoMonstro(tMonstro *monstro)
{
  if (monstro->tipo == FIREFLY)
  {
    if (monstro->direcaoAtual == RIGHT)
    {
      monstro->direcaoAtual = BOTTOM;
    }
    else if (monstro->direcaoAtual == BOTTOM)
    {
      monstro->direcaoAtual = LEFT;
    }
    else if (monstro->direcaoAtual == LEFT)
    {
      monstro->direcaoAtual = UP;
    }
    else if (monstro->direcaoAtual == UP)
    {
      monstro->direcaoAtual = RIGHT;
    }
  }
  else if (monstro->tipo == BUTTERFLY)
  {
    if (monstro->direcaoAtual == LEFT)
    {
      monstro->direcaoAtual = BOTTOM;
    }
    else if (monstro->direcaoAtual == BOTTOM)
    {
      monstro->direcaoAtual = RIGHT;
    }
    else if (monstro->direcaoAtual == RIGHT)
    {
      monstro->direcaoAtual = UP;
    }
    else if (monstro->direcaoAtual == UP)
    {
      monstro->direcaoAtual = LEFT;
    }
  }
}

void verificaRolamento(int **mapa, tObjetos *objetos, int col, int lin, int rochaAtual)
{
  rochedos *rocha = &objetos->rochedos[rochaAtual];

  int pedraX = rocha->lin;
  int pedraY = rocha->col;
  int tipo = rocha->tipo;
  int direcao = 0;

  if (lin > 0 && lin < TOTAL_LINHAS && col > 0 && col < TOTAL_COLUNAS)
  {
    // verifica se esta sobre uma rocha
    if (mapa[lin + 1][col] == PEDRA || mapa[lin + 1][col] == DIAMANTE || mapa[lin + 1][col] == MURO)
    {
      // verifica rolamento para a direita
      if (mapa[lin + 1][col + 1] == VAZIO && mapa[lin][col + 1] == VAZIO)
      {
        direcao = 1;
        rolaRocha(mapa, rocha, lin, col, direcao);
      }
      // verifica o rolamento para esquerda
      if (mapa[lin + 1][col - 1] == VAZIO && mapa[lin][col - 1] == VAZIO)
      {
        direcao = -1;
        rolaRocha(mapa, rocha, lin, col, direcao);
      }
    }
  }
}

void rolaRocha(int **mapa, rochedos *rocha, int lin, int col, int direcao)
{
  mapa[lin][col] = VAZIO;
  mapa[lin + 1][col + direcao] = rocha->tipo;

  rocha->caindo = 1;
  rocha->lin = lin + 1;
  rocha->col = col + direcao;
}

// Funcoes para destruir coisas
void explodeEmVolta(int **mapa, tObjetos *objetos, int lin, int col, tAudio *sons)
{
  for (int i = -1; i < 2; i++)
  {
    for (int j = -1; j < 2; j++)
    {
      if (linhaEColunaValidas(lin + i, col + j) && mapa[lin + i][col + j] != METAL && mapa[lin + i][col + j] != DIAMANTE)
      {
        if (mapa[lin + i][col + j] == PEDRA)
        {
          destroiRocha(objetos, mapa, lin + i, col + j, sons);
        }
        mapa[lin + i][col + j] = EXPLOSAO;

        // al_play_sample_instance(sons->explosion);
      }
    }
  }
}

void mataPlayer(tPlayer *jogador, int lin, int col, int **mapa, tObjetos *objetos, tAudio *sons)
{
  jogador->vidas--;

  explodeEmVolta(mapa, objetos, lin, col, sons);

  jogador->col = jogador->colInicial;
  jogador->lin = jogador->linInicial;

  mapa[jogador->linInicial][jogador->colInicial] = PLAYER;
}

void destroiRocha(tObjetos *objetos, int **mapa, int lin, int col, tAudio *sons)
{
  // al_play_sample_instance(sons->collect_diamond);

  for (int i = 0; i < objetos->totalRochas; i++)
  {
    rochedos *rochedoAtual = &objetos->rochedos[i];

    if (rochedoAtual->ativo && rochedoAtual->col == col && rochedoAtual->lin == lin)
      rochedoAtual->ativo = false;
  }
}

void mataMonstro(int **mapa, tObjetos *objetos, int lin, int col, tAudio *sons)
{
  for (int i = 0; i < objetos->qtMonstros; i++)
  {
    tMonstro *monstroAux = &objetos->monstros[0];

    if (monstroAux->lin == lin && monstroAux->col == col)
    {
      monstroAux->ativo = 0;
      explodeEmVolta(mapa, objetos, lin, col, sons);

      if (monstroAux->tipo == BUTTERFLY)
      {
        mapa[monstroAux->lin][monstroAux->col] = DIAMANTE;
      }
    }
  }
}

// ACOES PLAYER
void empurrarPedra(int **mapa, tObjetos *objetos, int direcao, int lin, int col, tAudio *sons)
{
  for (int i = 0; i < objetos->totalRochas; i++)
  {
    rochedos *rochedoAtual = &objetos->rochedos[i];

    if (rochedoAtual->ativo && rochedoAtual->col == col && rochedoAtual->lin == lin)
    {
      rochedoAtual->col += direcao;
    }
  }
}

void atualizaPlayer(tPlayer *jogador)
{

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
