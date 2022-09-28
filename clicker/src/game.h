#pragma once

// Game data
typedef struct Game
{
    int kimchi;
    int timepassing;
    int kimchi_per_second;
    int last_action;
    int kimchi_eated;
    int kimchi_thrown;
    int kimchi_menu;



    int kimchi_offset;


    int scene;

} Game;

void game_init(Game* game);
void game_update(Game* game);
void game_shutdown(Game* game);
