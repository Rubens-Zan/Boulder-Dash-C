#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int main()
{
	tGame *game = malloc(sizeof(tGame)); 
	game->state = INICIO; 
	
	
	while (1)
	{
		switch (game->state)
		{
		case INICIO:
			state_init(game);
			break;
		case SERVINDO:
			state_serve(game);
			break;
		case JOGANDO:
			state_play(game);
			break;
		case FIMPART: state_end(game); break;
		case FIMJOGO: state_close(game); break;
		default: break; 
		}
	}
	return 0;
}