#pragma once
#include "../game.h"

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
    
} Lang;
