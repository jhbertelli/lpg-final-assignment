#include <stdio.h>
#include <stdlib.h>
#include "helpers/console.h"
#include "lib/games.h"
#include "lib/developers.h"

int setup_devs();
int setup_games();

int main()
{
    int option;

    if (setup_devs() == 1 || setup_games() == 1)
    {
        printf("Ocorreu um erro ao gerar os arquivos binarios necessarios.");
        return 1;
    }

    print_line();

    printf("RELACIONAMENTO ENTRE TABELAS\n");

    print_line();

    while (option != 3)
    {
        printf("1 - Lista de devs\n");
        printf("2 - Lista de jogos\n");
        printf("3 - Sair\n");

        scanf("%d", &option);

        clear_console();

        switch (option)
        {
            case 1:
                show_devs_menu();
                break;
            case 2:
                show_games_menu();
                break;
            case 3:
                break;

            default:
                printf("Voce inseriu um numero invalido.\n");
                break;
        }
    }

    return 0;
}

int setup_devs()
{
    FILE *devs_fp;
    
    devs_fp = fopen(DEVS_FILENAME, "rb");

    if (devs_fp == NULL)
    {
        Developer developers[MAX_DEVS];

        for (int i = 0; i < MAX_DEVS; i++)
            developers[i].developer_id = 0;

        devs_fp = fopen(DEVS_FILENAME, "wb");

        if (devs_fp == NULL)
        {
            printf("Erro ao criar o arquivo de desenvolvedores.");
            return 1;
        }

        fwrite(developers, sizeof(Developer), MAX_DEVS, devs_fp);
        
        fclose(devs_fp);
    }
}

int setup_games()
{
    FILE *games_fp;
    
    games_fp = fopen(GAMES_FILENAME, "rb");

    if (games_fp == NULL)
    {
        Game games[MAX_GAMES];

        for (int i = 0; i < MAX_GAMES; i++)
            games[i].game_id = 0;

        games_fp = fopen(GAMES_FILENAME, "wb");

        if (games_fp == NULL)
        {
            printf("Erro ao criar o arquivo de jogos.");
            return 1;
        }
        
        fwrite(games, sizeof(Game), MAX_GAMES, games_fp);
        
        fclose(games_fp);
    }
}