#define METAL     0
#define TERRA     1
#define MURO      2
#define PEDRA     3
#define DIAMANTE  4
#define VAZIO     5
#define PLAYER    6
#define EXPLOSAO  7
#define EXPLOSAO2 8
#define EXPLOSAO3 9
#define SAIDA     10


typedef struct{
    int x, y;
}Coordenadas;

typedef struct{
    int id;
    ALLEGRO_BITMAP *image;

}Objeto;