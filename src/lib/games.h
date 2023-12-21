#ifndef GAMES_H
#define GAMES_H

#define GAMES_FILENAME "games.bin"
#define MAX_GAMES 60

typedef struct
{
    int game_id;
    int developer_id;
    char game_name[50];
    int year_released;
    char genre[20];
} Game;

extern void show_games_menu();
extern void insert_games();
extern void search_games();
extern void list_games();
extern void list_games_per_dev();
extern void delete_games();
extern void edit_games();

#endif