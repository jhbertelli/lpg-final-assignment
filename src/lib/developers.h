#ifndef DEVELOPERS_H
#define DEVELOPERS_H

#define DEVS_FILENAME "developers.bin"
#define MAX_DEVS 30

typedef struct
{
    int developer_id;
    char developer_name[50];
    char country[25];
    int year_founded;
} Developer;

extern void show_devs_menu();
extern void insert_devs();
extern void search_devs();
extern void list_devs();
extern void edit_devs();
extern void delete_devs();
extern int find_developer(Developer *developers, int id);

#endif