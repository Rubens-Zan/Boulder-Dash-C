# Autor: Rubens Laszlo
# Data: 04/2022
# GRR 20206147 


CC = gcc
EXEC = Boulder_Dash
ALLEGRO = $$(pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
CFLAG = -Wall -std=c99
MODULOS = game \
	mapa \
	texturas \
	desenhar \
	mecanicas \
	highscores
 
OBJETOS = main.o $(addsuffix .o,$(MODULOS))

.PHONY: all clean

all: jogo

jogo: $(OBJETOS)
	$(CC) -o $(EXEC) $(OBJETOS) $(ALLEGRO) $(CFLAG)

clean:
	$(RM) $(OBJETOS)

purge:
	$(RM) $(OBJETOS) $(EXEC)
