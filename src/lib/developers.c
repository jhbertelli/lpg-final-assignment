#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../helpers/console.h"
#include "../helpers/strhelpers.h"
#include "developers.h"

void handle_devs_file_close(FILE *fp, Developer *developers);

void show_devs_menu()
{
    int option = 0;
    
    while (option != 6)
    {
        printf("1 - Listar desenvolvedores\n");
        printf("2 - Procurar desenvolvedores\n");
        printf("3 - Inserir\n");
        printf("4 - Editar\n");
        printf("5 - Excluir\n");
        printf("6 - Voltar\n");

        scanf("%d", &option);

        clear_console();

        switch (option)
        {
            case 1:
                list_devs();
                break;
            case 2:
                search_devs();
                break;
            case 3:
                insert_devs();
                break;
            case 4:
                edit_devs();
                break;
            case 5:
                delete_devs();
                break;
            case 6:
                break;

            default:
                printf("Voce inseriu um numero invalido.\n");
                break;
        }
    }
    
    clear_console();
}

void list_devs()
{
    FILE *fp = fopen(DEVS_FILENAME, "rb");
    Developer developers[MAX_DEVS];

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    print_line();

    for (int i = 0; i < MAX_DEVS; i++)
    {
        Developer dev = developers[i];
        
        if (dev.developer_id == 0)
        {
            if (i == 0) 
            {
                printf("Nenhum registro encontrado.\n");
                print_line();
            }
            
            break;
        }

        printf("ID do desenvolvedor: %d\n", dev.developer_id);
        printf("Nome: %s\n", developers[i].developer_name);
        printf("Ano fundado: %d\n", developers[i].year_founded);
        printf("Pais: %s\n", developers[i].country);
        
        print_line();
    }

    fclose(fp);
}

void search_devs()
{
    FILE *fp = fopen(DEVS_FILENAME, "rb");
    Developer developers[MAX_DEVS];

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);

    Developer developers_found[MAX_DEVS];
    int count_found = 0;

    char search_text[50];

    printf("Digite o nome do desenvolvedor que voce deseja encontrar:\n");
    scanf(" %[^\n]", search_text);

    to_lowercase(search_text);

    for (int i = 0; i < MAX_DEVS; i++)
    {   
        Developer developer = developers[i];

        if (developer.developer_id == 0)
            break;

        char aux[50];
        strcpy(aux, developer.developer_name);

        to_lowercase(aux);

        if (strstr(aux, search_text) != NULL)
        {
            developers_found[count_found] = developer;
            count_found++;
        }
    }

    print_line();

    printf("Exibindo %d registros...\n", count_found);
    
    print_line();

    for (int i = 0; i < count_found; i++)
    {
        printf("Registro %d\n", i + 1);
        printf("ID do desenvolvedor: %d\n", developers_found[i].developer_id);
        printf("Nome: %s\n", developers_found[i].developer_name);
        printf("Ano fundado: %d\n", developers_found[i].year_founded);
        printf("Pais: %s\n", developers_found[i].country);

        print_line();
    }

    printf("%d registro(s) mostrados.\n", count_found);

    print_line();
}

void insert_devs()
{
    FILE *fp = fopen(DEVS_FILENAME, "rb");
    Developer developers[MAX_DEVS];

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    // verifica qual será o novo índice do item no vetor
    int new_entry_index = 0;

    for (int i = 0; i < MAX_DEVS; i++)
    {
        Developer developer = developers[i];

        if (developer.developer_id == 0)
            break;

        new_entry_index++;
    }

    fp = fopen(DEVS_FILENAME, "wb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    printf("Insira o nome da desenvolvedora:\n");
    scanf(" %[^\n]", developers[new_entry_index].developer_name);

    clear_console();
    
    printf("Insira o pais da desenvolvedora:\n");
    scanf(" %[^\n]", developers[new_entry_index].country);
    
    clear_console();

    printf("Insira o ano de fundacao da desenvolvedora:\n");
    scanf("%d", &developers[new_entry_index].year_founded);

    clear_console();
    
    if (new_entry_index == 0)
    {
        developers[new_entry_index].developer_id = 1;
    }
    else
    {
        // se o item não for o primeiro do vetor, pega o ID do último item e incrementa em um
        developers[new_entry_index].developer_id = developers[new_entry_index - 1].developer_id + 1;
    }

    fwrite(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);
}

void edit_devs()
{
    FILE *fp = fopen(DEVS_FILENAME, "rb");
    Developer developers[MAX_DEVS];

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);

    fp = fopen(DEVS_FILENAME, "wb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    int id;

    printf("Insira o id da desenvoldora que deseja editar:\n");
    scanf("%d", &id);
    
    if (!find_developer(developers, id))
    {
        handle_devs_file_close(fp, developers);
        show_not_found_message("desenvolvedor", id);
        return;
    }

    int index = 0;

    for (int i = 0; i < MAX_DEVS; i++)
    {
        Developer developer = developers[i];

        if (developer.developer_id == id)
            break;

        index++;
    }

    printf("Insira o nome da desenvolvedora:\n");
    scanf(" %49[^\n]", developers[index].developer_name);

    clear_console();

    printf("Insira o pais da desenvolvedora:\n");
    scanf(" %24[^\n]", developers[index].country);

    clear_console();

    printf("Insira o ano de fundacao da desenvolvedora:\n");
    scanf("%d", &developers[index].year_founded);

    clear_console();
    
    fwrite(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);  
}  

void delete_devs()
{
    FILE *fp;
    Developer developers[MAX_DEVS];

    fp = fopen(DEVS_FILENAME, "rb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);

    fp = fopen(DEVS_FILENAME, "wb");

    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    int found = 0, id;

    // seria bom mover isso pra outra função
    // mas por conta do prazo fica por aqui por enquanto
    while (!found)
    {
        printf("Insira o ID do desenvolvedor que deseja excluir, ou digite 0 para voltar:\n");
        scanf("%d", &id);

        if (id == 0)
        {
            handle_devs_file_close(fp, developers);
            clear_console();
            return;
        }

        for (int i = 0; i < MAX_DEVS; i++)
        {
            if (developers[i].developer_id == 0 && found == 0)
                break;
            
            if (developers[i].developer_id == id)
                found = 1;

            if (found)
            {
                if (i == MAX_DEVS - 1)
                {
                    // modifica o ID do último valor do array para 0
                    // por conta do desenvolvedor excluido
                    developers[i].developer_id = 0;
                    break;
                }
                
                developers[i] = developers[i + 1];
            }
        }
        
        if (!found)
        {
            clear_console();
            printf("Nao foi possivel encontrar o desenvolvedor com o ID %d.\n", id);
        }
    }

    fwrite(developers, sizeof(Developer), MAX_DEVS, fp);

    fclose(fp);
}

void handle_devs_file_close(FILE *fp, Developer *developers)
{
    // por algum motivo que não entendi, antes de fechar um arquivo aberto como write
    // é necessário dar write no array que existia antes para não enviar lixo depois
    fwrite(developers, sizeof(Developer), MAX_DEVS, fp);
    fclose(fp);
}

int find_developer(Developer *developers, int id)
{
    int found = 0;

    for (int i = 0; i < MAX_DEVS && id != 0; i++)
    {
        if (developers[i].developer_id == id)
        {
            found = 1;
            break;
        }
    }

    return found;
}