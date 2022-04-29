#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregaScores(int *pontos, int *nPontuacoes)
{
  FILE *arq;

  arq = fopen("resources/scores.txt", "r");
  if (!arq)
  {
    fprintf(stderr, "Não foi possivel inicializar o arquivo de highscore\n");
    exit(1);
  }
  // Carrega somente os top 10
  for (int i = 0; i < 10 && !feof(arq); i++)
  {
    fscanf(arq, "%d", &pontos[i]);
    *nPontuacoes += 1;
  }

  fclose(arq);
}

int cmpfunc(const void *a, const void *b)
{
  return -(*(int *)a - *(int *)b);
}

void salvaScoreAtual(int pontosJogador, int *highscores, int nPontuacoes)
{
  FILE *arq;

  arq = fopen("resources/scores.txt", "w");
  if (!arq)
  {
    fprintf(stderr, "Não foi possivel inicializar o arquivo de highscore\n");
    exit(1);
  }
  highscores[nPontuacoes] = pontosJogador;
  qsort(highscores, nPontuacoes + 1, sizeof(int), cmpfunc);

  // so entra no ranking se for top 10
  fprintf(arq, "%d", highscores[0]);
  for (int i = 1; i <= nPontuacoes && i < 10; i++)
  {
    fprintf(arq, "\n%d", highscores[i]);
  }

  fclose(arq);
}