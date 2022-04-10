#include <stdio.h>
#include <stdlib.h>
#include "game.h"



int main(){
  state = INICIO;
  tNivel *infoNivel = malloc(sizeof(tNivel)); 

  while(1){
  	switch(state){
  	  case INICIO: state_init(infoNivel); break;
  	  case SERVINDO: state_serve(infoNivel); break;
  	  case JOGANDO: state_play(infoNivel); break;
  	//   case FIMPART: state_end(); break;
  	//   case FIMJOGO: state_close(); break;
  	}
  }
  return 0;
}