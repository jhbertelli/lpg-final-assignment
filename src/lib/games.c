#include <stdio.h>
#include <string.h>
#include "../helpers/console.h"
#include "../helpers/strhelpers.h"
#include "games.h"
#include "developers.h"

int check_has_developers(Developer *developers, FILE *developers_fp);
int has_developers(Developer *developers);
void handle_games_file_close(FILE *fp, Game *games);

void show_games_menu()
{
    int option = 0;
    
    while (option != 7)
    {
        printf("1 - Listar jogos\n");
        printf("2 - Listar jogos por uma desenvolvedora\n");
        printf("3 - Procurar jogos\n");
        printf("4 - Inserir\n");
        printf("5 - Editar\n");
        printf("6 - Excluir\n");
        printf("7 - Voltar\n");

        scanf("%d", &option);

        clear_console();

        switch (option)
        {
            case 1:
                list_games();
                break;
            case 2:
                list_games_per_dev();
                break;
            case 3:
                search_games();
                break;
            case 4:
                insert_games();
                break;
            case 5:
                edit_games();
                break;
            case 6:
                delete_games();
                break;
            case 7:
                break;

            default:
                printf("Voce inseriu um numero invalido.\n");
                break;
        }
    }
    
    clear_console();
}

void list_games()
{
    FILE *fp;
    Game games[MAX_GAMES];

    fp = fopen(GAMES_FILENAME, "rb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(games, sizeof(Game), MAX_GAMES, fp);

    print_line();

    for (int i = 0; i < MAX_GAMES; i++)
    {
        Game game = games[i];
        
        if (game.game_id == 0)
        {
            if (i == 0) 
            {
                printf("Nenhum registro encontrado.\n");
                print_line();
            }
            
            break;
        }

        printf("ID do jogo: %d\n", game.game_id);
        printf("ID do desenvolvedor: %d\n", game.developer_id);
        printf("Nome: %s\n", game.game_name);
        printf("Ano lancado: %d\n", game.year_released);
        printf("Genero: %s\n", game.genre);
        
        print_line();
    }

    fclose(fp);
}

void insert_games()
{
    FILE *games_fp, *developers_fp;
    Game games[MAX_GAMES];
    Developer developers[MAX_DEVS];

    developers_fp = fopen(DEVS_FILENAME, "rb");

    if (developers_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de desenvolvedores.");
        fclose(games_fp);
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, developers_fp);
    
    if (!check_has_developers(developers, developers_fp)) return;

    games_fp = fopen(GAMES_FILENAME, "rb");

    if (games_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        fclose(developers_fp);
        return;
    }

    fread(games, sizeof(Game), MAX_GAMES, games_fp);

    // verifica qual será o novo índice do item no vetor
    int new_entry_index = 0;

    for (int i = 0; i < MAX_GAMES; i++)
    {
        Game game = games[i];

        if (game.game_id == 0)
            break;

        new_entry_index++;
    }

    games_fp = fopen(GAMES_FILENAME, "wb");

    if (games_fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    printf("Insira o nome do jogo:\n");
    scanf(" %[^\n]", games[new_entry_index].game_name);

    clear_console();
    
    printf("Insira o genero do jogo:\n");
    scanf(" %[^\n]", games[new_entry_index].genre);
    
    clear_console();

    printf("Insira o ano do lancamento do jogo:\n");
    scanf("%d", &games[new_entry_index].year_released);

    clear_console();
    
    int success;
    int developer_id;

    while (success == 0)
    {
        printf("Insira o ID do desenvolvedor do jogo:\n");
        scanf("%d", &developer_id);
        
        clear_console();

        for (int i = 0; i < MAX_DEVS; i++)
        {
            if (developers[i].developer_id == developer_id)
            {
                success = 1;
            }
        }

        if (success == 0)
            printf("Voce inseriu um ID invalido. Tente novamente.\n");
    }

    games[new_entry_index].developer_id = developer_id;

    if (new_entry_index == 0)
    {
        games[new_entry_index].game_id = 1;
    }
    else
    {
        // se o item não for o primeiro do vetor, pega o ID do último item e incrementa em um
        games[new_entry_index].game_id = games[new_entry_index - 1].game_id + 1;
    }

    fwrite(games, sizeof(Game), MAX_GAMES, games_fp);

    fclose(games_fp);
    fclose(developers_fp);
}

void search_games()
{
    FILE *fp = fopen(GAMES_FILENAME, "rb");
    Game games[MAX_GAMES];

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(games, sizeof(Game), MAX_GAMES, fp);

    fclose(fp);

    Game games_found[MAX_GAMES];
    int count_found = 0;

    char search_text[50];

    printf("Digite o nome do jogo que voce deseja encontrar:\n");
    scanf(" %[^\n]", search_text);

    to_lowercase(search_text);

    for (int i = 0; i < MAX_GAMES; i++)
    {   
        Game game = games[i];

        if (game.game_id == 0)
            break;

        char aux[50];
        strcpy(aux, game.game_name);

        to_lowercase(aux);

        if (strstr(aux, search_text) != NULL)
        {
            games_found[count_found] = game;
            count_found++;
        }
    }

    print_line();

    printf("Exibindo %d registros...\n", count_found);
    
    print_line();

    for (int i = 0; i < count_found; i++)
    {
        printf("Registro %d\n", i + 1);
        printf("ID do jogo: %d\n", games_found[i].game_id);
        printf("ID do desenvolvedor: %d\n", games_found[i].developer_id);
        printf("Nome: %s\n", games_found[i].game_name);
        printf("Ano lancado: %d\n", games_found[i].year_released);
        printf("Genero: %s\n", games_found[i].genre);

        print_line();
    }

    printf("%d registro(s) mostrados.\n", count_found);

    print_line();
}

void list_games_per_dev()
{
    FILE *games_fp, *developers_fp;
    Game games[MAX_GAMES];
    Developer developers[MAX_DEVS];

    developers_fp = fopen(DEVS_FILENAME, "rb");

    if (developers_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de desenvolvedores.");
        fclose(games_fp);
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, developers_fp);
    
    if (!check_has_developers(developers, developers_fp)) return;

    fclose(developers_fp);

    int id;

    printf("Insira o id da desenvoldora a qual voce deseja ver os jogos:\n");
    scanf("%d", &id);
    
    if (!find_developer(developers, id))
    {
        show_not_found_message("desenvolvedor", id);
        return;
    }

    clear_console();

    int developer_index = id - 1;

    Developer developer_found = developers[developer_index];

    games_fp = fopen(GAMES_FILENAME, "rb");

    if (games_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        return;
    }

    fread(games, sizeof(Game), MAX_GAMES, games_fp);

    fclose(games_fp);

    int count_games_by_dev = 0;

    print_line();

    for (int i = 0; i < MAX_GAMES; i++)
    {
        Game game = games[i];
        
        if (game.game_id == 0)
            break;

        if (game.developer_id == developer_found.developer_id)
        {
            count_games_by_dev++;

            printf("ID do Jogo: %d\n", game.game_id);
            printf("ID do desenvolvedor: %d\n", developer_found.developer_id);
            printf("Nome do jogo: %s\n", game.game_name);
            printf("Ano lancado: %d\n", game.year_released);
            printf("Genero: %s\n", game.genre);
            printf("Nome do desenvolvedor: %s\n", developer_found.developer_name);
            printf("Pais do desenvolvedor: %s\n", developer_found.country);
            printf("Ano de fundacao do desenvolvedor: %d\n", developer_found.year_founded);
            
            print_line();
        }
    }

    printf("%d registro(s) encontrados.\n", count_games_by_dev);
    print_line();
}

void edit_games()
{
    FILE *games_fp = fopen(GAMES_FILENAME, "rb"), *developers_fp;
    Game games[MAX_GAMES];
    Developer developers[MAX_DEVS];
    
    if (games_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        return;
    }
    
    fread(games, sizeof(Game), MAX_GAMES, games_fp);
   
    developers_fp = fopen(DEVS_FILENAME, "rb");

    if (developers_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de desenvolvedores.");
        fclose(games_fp);
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, developers_fp);
    
    if (!has_developers(developers))
    {
        printf("Por favor insira pelo menos um desenvolvedor antes de inserir um jogo.\n");
        show_return_message();
        
        fclose(games_fp);
        fclose(developers_fp);
        return;
    }

    int id, found = 0;

    printf("Insira o id do jogo que deseja editar: \n\n");
    scanf("%d", &id);
    
    for (int i = 0; i < MAX_GAMES && !found; i++)
    {
        if(games[i].game_id == id)
            found = 1;
    }

    if (!found)
    {
        fclose(games_fp);
        fclose(developers_fp);
        show_not_found_message("jogo", id);
        return;
    }
    
    games_fp = fopen(GAMES_FILENAME, "wb");

    if (games_fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        return;
    }

    int index = 0;
    
    for (int i = 0; i < MAX_GAMES; i++)
    {
        Game game = games[i];

        if (game.game_id == id)
            break;

        index++;
    }

    printf("Insira o nome do jogo:\n");
    scanf(" %[^\n]", games[index].game_name);

    clear_console();

    printf("Insiro o genero do jogo:\n");
    scanf(" %[^\n]", games[index].genre);

    clear_console();

    printf("Insira o ano de lancamento do jogo:\n");
    scanf("%d", &games[index].year_released);

    clear_console();

    int success = 0;
    int developer_id;

    while (success == 0)
    {
        printf("Insira o ID do desenvolvedor do jogo:\n");
        scanf("%d", &developer_id);
        
        clear_console();

        for (int i = 0; i < MAX_DEVS; i++)
        {
            if (developers[i].developer_id == developer_id)
            {
                success = 1;
            }
        }

        if (success == 0)
            printf("Voce inseriu um ID invalido. Tente novamente.\n");
    }

    games[index].developer_id = developer_id;
    

    fwrite(games, sizeof(Game), MAX_GAMES, games_fp);

    fclose(games_fp);
    fclose(developers_fp);
}

void delete_games()
{
    FILE *fp;
    Game games[MAX_GAMES];
    Developer developers[MAX_DEVS];

    fp = fopen(GAMES_FILENAME, "rb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        return;
    }

    fread(games, sizeof(Game), MAX_GAMES, fp);

    fclose(fp);

    fp = fopen(GAMES_FILENAME, "wb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo de jogos.");
        return;
    }

    int found = 0, id;

    // seria bom mover isso pra outra função
    // mas por conta do prazo fica por aqui por enquanto
    while (!found)
    {
        printf("Insira o ID do jogo que deseja excluir, ou digite 0 para voltar:\n");
        scanf("%d", &id);

        if (id == 0)
        {
            handle_games_file_close(fp, games);
            clear_console();
            return;
        }

        for (int i = 0; i < MAX_GAMES; i++)
        {
            if (games[i].game_id == 0 && found == 0)
                break;
            
            if (games[i].game_id == id)
                found = 1;

            if (found)
            {
                if (i == MAX_GAMES - 1)
                {
                    // modifica o ID do último valor do array para 0
                    // por conta do jogo excluido
                    games[i].game_id = 0;
                    break;
                }
                
                games[i] = games[i + 1];
            }
        }
        
        if (!found)
        {
            clear_console();
            printf("Nao foi possivel encontrar o jogo com o ID %d.\n", id);
        }
    }

    fwrite(games, sizeof(Game), MAX_GAMES, fp);

    fclose(fp);
}

int check_has_developers(Developer *developers, FILE *developers_fp)
{
    if (!has_developers(developers))
    {
        printf("Por favor insira pelo menos um desenvolvedor antes de inserir um jogo.\n");
        show_return_message();
        
        fclose(developers_fp);
        return 0;
    }

    return 1;
}

int has_developers(Developer *developers)
{
    return developers[0].developer_id != 0;
}

void handle_games_file_close(FILE *fp, Game *games)
{
    // por algum motivo que não entendi, antes de fechar um arquivo aberto como write
    // é necessário dar write no array que existia antes para não enviar lixo depois
    fwrite(games, sizeof(Game), MAX_GAMES, fp);
    fclose(fp);
}
