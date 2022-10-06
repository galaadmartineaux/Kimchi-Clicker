#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../utils/dialog.c"
#include "../game.h"
#include "../utils/flags.c"

void scene1_mainmenu(Game* game)
{
    im_print((WINDOW_WIDTH / 2) - (strlen("Kimchi Clicker") / 2),0, "Kimchi Clicker");
    if (im_button((WINDOW_WIDTH / 2) - (strlen(game->lang.newgame) / 2),3, game->lang.newgame))
    {
        game->scene = 0;
    }
    draw_box(-1,5,WINDOW_WIDTH + 1,5);


    im_print((WINDOW_WIDTH / 2) - (strlen(game->lang.load) / 2) ,7, game->lang.load);
    if (im_button((WINDOW_WIDTH / 4) - (strlen(game->lang.load_slot_1) / 2),9, game->lang.load_slot_1))
    {
        int temp = load(game, 1);
        if (temp == 1)
        {
            game->visual.last_action = 101;
        }
        else
        {
            game->scene = 0;
        }
    }

    if (im_button(((WINDOW_WIDTH / 4) * 3) - (strlen(game->lang.load_slot_2) / 2), 9, game->lang.load_slot_2))
    {
        game->scene = 0;
        load(game, 2);
    }

    if (im_button(((WINDOW_WIDTH / 4)) - (strlen(game->lang.load_slot_3) / 2), 11, game->lang.load_slot_3))
    {
        game->scene = 0;
        load(game, 3);
    }

    if (im_button(((WINDOW_WIDTH / 4) * 3) - (strlen(game->lang.load_slot_4) / 2), 11, game->lang.load_slot_4))
    {
        game->scene = 0;
        load(game, 4);
    }
    
    draw_box(-1,14,WINDOW_WIDTH + 1,14);

    im_print((WINDOW_WIDTH / 2) - (strlen(game->lang.language) / 2) ,16, game->lang.language);
    if (im_button((WINDOW_WIDTH / 4) - (strlen("English") / 2),18, "English"))
    {
        game->language = 0;
        game_init(game);
    }


    if (im_button(((WINDOW_WIDTH / 4) * 3) - (strlen("Français") / 2), 18, "Fran" "\x87" "ais" "\x03"))
    {
        game->language = 1;
        game_init(game);
    }
    printflag( ((WINDOW_WIDTH / 4) * 3) - (strlen("Français") / 2), 20, "france");
    printflag( (WINDOW_WIDTH / 4) - (strlen("English") / 2) , 20, "");


    im_print(0,47, "GALAAD MARTINEAUX");
    im_print(WINDOW_WIDTH - strlen("ISART DIGITAL"),47, "ISART DIGITAL");
}