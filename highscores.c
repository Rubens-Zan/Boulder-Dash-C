#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregarScores(char *nome_arquivo, int *scores, int nScores){
    char * line = NULL;
    size_t len = 0;
    size_t read;
    
    FILE *fp = fopen(nome_arquivo, "r");

    if (fp == NULL){
         printf("Nao foi possivel ler o arquivo de senhas\n");
         exit(EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, fp)) != -1  && (nScores < 10)) {
        line[strlen(line)-1] = '\0';
        scores[nScores] = atoi(line); 
        nScores++;
    }
    // printf("leitura dos scores completadas, %d senhas lidas\n",nScores);

    fclose(fp);
}