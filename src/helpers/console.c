#include <stdio.h>
#include <stdlib.h>
#include "console.h"

void clear_console()
{
    system("cls");
}

void not_implemented()
{
    printf("Esta funcao ainda nao foi implementada.\n");
    
    show_return_message();
}

void show_return_message()
{
    char temp[100];

    printf("Envie qualquer tecla para voltar...\n");
    
    scanf("%s", temp);

    clear_console();
}

void show_not_found_message(char *entity, int id)
{
    clear_console();
    printf("Nao foi possivel encontrar o %s para o ID %d.\n", entity, id);
    show_return_message();
    return;
}

void print_line()
{
    for (int i = 0; i < 50; i++)
        printf("-");
    
    printf("\n");
}
