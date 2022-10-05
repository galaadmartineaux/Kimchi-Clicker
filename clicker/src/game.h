#pragma once
#include "lang/lang.h"


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

// Game data
typedef struct Game
{
    float timepassing;
    float frameTime;

    int scene;
    int is_dialog;
    int currentDialog;

    const char* language;

    lollipops lollipops;
    kimchi kimchi;
    visual visual;
    Lang lang;
} Game;




void game_init(Game* game);
void game_update(Game* game);
void game_shutdown(Game* game);
