#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../game.h"

//Buttons
void button_eatkimchis(Game* game)
{
    if (im_button(1, 2 + game->visual.offset, game->lang.kimchi_eat) == true)
    {
        game->kimchi.eated += game->kimchi.value;
        game->kimchi.value= 0;
        game->visual.last_action = 1;
    }
}


void button_throwkimchis(Game* game)
{
    if (im_button(1, 4 + game->visual.offset, game->lang.kimchi_throw) == true)
    {
        game->kimchi.thrown += 10;
        game->kimchi.value-= 10;
        game->visual.last_action = 2;
    }
}

void button_enablemenu(Game* game)
{
    if (im_button(1, 6 + game->visual.offset, game->lang.bribe) == true && game->visual.menu < 3)
    {
        game->kimchi.value-= 30;
        game->visual.last_action = 3;
        game->visual.menu += 1;
        game->visual.offset = 6;
    }
}


void scene0_kimchibox(Game* game)
{
    switch (game->kimchi.value)
        {
        case 1 ... 9:
            button_eatkimchis(game);
            break;
        
        case 10 ... 29:
            button_eatkimchis(game);
            button_throwkimchis(game);
            break;

        case 30 ... 100000000:
            button_eatkimchis(game);
            button_throwkimchis(game);
            button_enablemenu(game);
            break;

        default:
            break;
        }
}
