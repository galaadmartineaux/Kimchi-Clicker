#pragma once
#include "debug/debug.h"
typedef struct kimchi
{
    int value;
    float per_second;
    
    int eated;
    int thrown;
} kimchi;

typedef struct lollipops
{
    int value;
    int rng;
    int x;
    int y;
} lollipops;

typedef struct visual
{
    int offset;
    int menu;
    int last_action;
} visual;


typedef struct Lang
{
    const char* newgame;
    const char* language;
    const char* load;
    const char* save;
    const char* save_slot_1;
    const char* save_slot_2;
    const char* save_slot_3;
    const char* save_slot_4;
    const char* load_slot_1;
    const char* load_slot_2;
    const char* load_slot_3;
    const char* load_slot_4;


    const char* kimchi_value;
    const char* lolis_value;

    const char* kimchi_eat;
    const char* kimchi_eaten;
    const char* kimchi_throw;
    const char* kimchi_thrown;
    const char* bribe;
    
    const char* menu;
    const char* menu_map_the;
    const char* menu_map_map;
    const char* menu_kimchi;
    const char* menu_fridge;
    const char* menu_debug;
    const char* menu_mini;
    const char* menu_game;

    const char* dialog_shopkeeper;
    const char* dialog_shopkeeper2;
    const char* dialog_shopkeeper3;
    const char* notenoughmoney;

    const char* error_load;

    const char* debug_button_fps;
    const char* debug_button_kimchi_eated;
    const char* debug_button_kimchi_per_second;
    const char* debug_button_kimchi_thrown;
    const char* debug_button_lollipops;
    const char* debug_button_visual_last_action;
    const char* debug_menu_values;
    const char* debug_menu_others;
    
} Lang;


// Game data
typedef struct Game
{
    float timepassing;
    float frameTime;

    int scene;
    int is_dialog;
    int currentDialog;

    int language;

    lollipops lollipops;
    kimchi kimchi;
    visual visual;
    Lang lang;
    debug debug;
} Game;




void game_init(Game* game);
void game_update(Game* game);
void game_shutdown(Game* game);
