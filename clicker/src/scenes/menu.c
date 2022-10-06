#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../utils/constants.h"
#include "../game.h"
#include "minigames/search_lollipops.c"

//Scene
//Buttons
void btsc_kimchi(Game* game)
{
    if (im_button(3, 2, game->lang.menu_kimchi) == true || im_button(3, 3, game->lang.menu_fridge) == true)
    {
        game->scene = 0;
    }
}

void btsc_map(Game* game)
{
    if (im_button(((WINDOW_WIDTH / 5) * 1) - (strlen(game->lang.menu_map_the) / 2), 2, game->lang.menu_map_the) || im_button(((WINDOW_WIDTH / 5) * 1) - (strlen(game->lang.menu_map_the) / 2), 3, game->lang.menu_map_map))
    {
        game->scene = 3;
    }
}

void btsc_minigame(Game* game)
{
    if (im_button(((WINDOW_WIDTH / 5) * 2) - (strlen(game->lang.menu_mini) / 2), 2, game->lang.menu_mini) == true || im_button(((WINDOW_WIDTH / 5) * 2) - (strlen(game->lang.menu_game) / 2), 3, game->lang.menu_game) == true)
    {
        game->scene = 50;
    }
}

void btsc_save(Game* game)
{
    if (im_button(((WINDOW_WIDTH / 5) * 3) - (strlen(game->lang.save) / 2), 2, game->lang.save) == true || im_button(((WINDOW_WIDTH / 5) * 3) - (strlen(game->lang.menu) / 2), 3, game->lang.menu) == true)
    {
        game->scene = 2;
    }
}

void button_debug(Game* game)
{
    if (game->scene != 99)
    {
        if (im_button(((WINDOW_WIDTH / 5) * 4) - (strlen(game->lang.menu_debug) / 2), 2, game->lang.menu_debug) == true || im_button(((WINDOW_WIDTH / 5) * 4) - 3, 3, " Menu ") == true)
        {
            game->scene = 99;
        }
    }
    else 
    {
        if (im_button(((WINDOW_WIDTH / 5) * 4) - (strlen(game->lang.menu_debug) / 2), 2, game->lang.menu_debug) == true || im_button(((WINDOW_WIDTH / 5) * 4) - 3, 3, " Menu ") == true)
        {
            game->scene = 0;
        }
    }
}

void menu_display(Game* game)
{
    if (game->visual.menu >= 1)
        {
            draw_box(0,0,WINDOW_WIDTH - 1,5);
            btsc_kimchi(game);
            btsc_save(game);
            switch (game->visual.menu)
            {
            case 2:
                btsc_map(game);
                break;
            
            case 3:
                btsc_map(game);
                btsc_minigame(game);
                break;

            default:
                break;
            }
        }
    if (game->scene != 1)
    {
        button_debug(game);
    }
}