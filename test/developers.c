#include <stdio.h>
#define MAX_DEVS 30

typedef struct
{
    int developer_id;
    char developer_name[50];
    char country[25];
    int year_founded;
} Developer;

// para verificar se os desenvolvedores estão com os IDs certos
int main()
{
    FILE *fp;
    Developer developers[MAX_DEVS];

    fp = fopen("../developers.bin", "rb");
    
    if (fp == NULL)
    {
        printf("Erro ao abrir o arquivo.");
        return 1;
    }

    fread(developers, sizeof(Developer), MAX_DEVS, fp);

    for (int i = 0; i < MAX_DEVS; i++)
    {
        printf("indice %d: %d\n", i, developers[i].developer_id);
    }
}