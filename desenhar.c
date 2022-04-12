#include "desenhar.h"
// Função de desenho principal
void draw(bool redraw, long frames, tNivel *infoNivel)
{
    drawHeader(infoNivel->font, infoNivel->relogio, 8, infoNivel->objetos_mapa->diamante[0]);
    drawMap(infoNivel->mapa, infoNivel->objetos_mapa, frames, 22, 40);
    drawPlayer(infoNivel->jogador, infoNivel->mapa, infoNivel->objetos_mapa, frames);
    al_flip_display();
    redraw = false;
}

void drawPlayer(tPlayer *jogador, int **mapa, tObjetos *obj, long frames)
{
    switch (jogador->direction){
    case UP:
        al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
        break;
    case BOTTOM:
        al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
        break;
    case LEFT:
        al_draw_scaled_bitmap(jogador->animacaoEsq[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);

        break;
    case RIGHT:
        al_draw_scaled_bitmap(jogador->animacaoDir[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
        break;
    case STOP:
        al_draw_scaled_bitmap(jogador->animacaoParado[jogador->animacaoAtual], 0, 0, 15, 16, jogador->posX, jogador->posY, SIZE_OBJS, SIZE_OBJS, 0);
        break;
    }
}

void drawEndGame(tNivel *infoNivel)
{
    al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.9));
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "F I M D E J O G O");
    // al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 100 + 2 * SIZE_OBJS, 0, "PONTUACAO: %d",jogador->pontuacao);
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 7 * SIZE_OBJS, 160 + 2 * SIZE_OBJS, 0, "Placar de lideres");
    // for(int i = 0;i < 5;i++)
    // al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH/4 + 7 * SIZE_OBJS, 220 + (i*40) + 2 * SIZE_OBJS, 0, "%d: %d pts", i + 1, pontos_totais->score[i]);
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), WIDTH / 4 + 5 * SIZE_OBJS, 500 + 2 * SIZE_OBJS, 0, "Pressione ESC para sair...");
    al_flip_display();
}

void drawInstructions(tNivel *infoNivel)
{
    al_draw_filled_rectangle(3 * SIZE_OBJS, 2 * SIZE_OBJS, WIDTH - 3 * SIZE_OBJS, HEIGHT - 1 * SIZE_OBJS, al_map_rgba_f(0, 0, 0, 0.6));
    al_draw_textf(infoNivel->font, al_map_rgb(120, 120, 255), WIDTH / 4 + 7 * SIZE_OBJS, 20 + 2 * SIZE_OBJS, 0, "INSTRUCOES");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 80 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Colete todos os diamantes e chegue na saida");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 100 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "antes que o tempo acabe. Cuidado com as");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 120 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "pedras, elas podem cair em cima voce! ");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 160 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "Utilize as setas do teclado para se movimentar");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 190 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "ou as teclas W,S,A,D");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 210 + 2 * SIZE_OBJS, ALLEGRO_ALIGN_LEFT, "para finalizar o jogo pressione ESC.");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 550, ALLEGRO_ALIGN_LEFT, "Jogo desenvolvido por Rubens Laszlo ");
    al_draw_textf(infoNivel->font, al_map_rgb(255, 255, 255), 120, 600, ALLEGRO_ALIGN_LEFT, "Abril de 2022.");
    al_flip_display();
}

void drawHeader(ALLEGRO_FONT *fonte, int relogio, int vidas, ALLEGRO_BITMAP *diamanteTextura)
{

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 250, 10, 0, "%05d", 20);
    al_draw_bitmap(diamanteTextura, 0, 8, 0);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), 20, 10, 0, "%d/%d", 4, MIN_DIAMANTES);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), WIDTH - 800, 10, 0, "%d", relogio);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), WIDTH - 600, 10, 0, "Vidas: %d", vidas);
    al_draw_textf(fonte, al_map_rgb(255, 255, 255), WIDTH - 250, 10, 0, "Help: H/F1");
}
