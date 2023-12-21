#include <stdio.h>
#define MAX_GAMES 60

typedef struct
{
    int game_id;
    int developer_id;
    char game_name[50];
    int year_released;
    char genre[20];
} Game;

// para verificar se os jogos estão com os IDs certos
int main()
{
    FILE *fp;
    Game games[MAX_GAMES];

    fp = fopen("../games.bin", "rb");
    
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    fread(games, sizeof(Game), MAX_GAMES, fp);

    for (int i = 0; i < MAX_GAMES; i++)
    {
        printf("indice %d: %d\n", i, games[i].game_id);
    }
}