#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main()
{
	tNivel *infoNivel = malloc(sizeof(tNivel));
	infoNivel->state = INICIO;

	while (1)
	{
		switch (infoNivel->state)
		{
		case INICIO:
			state_init(infoNivel);
			break;
		case SERVINDO:
			state_serve(infoNivel);
			break;
		case JOGANDO:
			state_play(infoNivel);
			break;
		// case FIMPART: state_end(); break;
		// case FIMJOGO: state_close(); break;
		}
	}
	return 0;
}