#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../game.h"
#include <stdlib.h>

int randBetween(int upper, int lower)
{
    return rand() % (upper - lower + lower) + lower;
}




void scene50_display(Game* game)
{
    
    if (game->lollipops.rng == 0)
    {
        game->lollipops.x = randBetween(49,1);
        game->lollipops.y = randBetween(40,10);
        game->lollipops.rng = 1;
    }


    if (im_button(game->lollipops.x, game->lollipops.y, "a") == true)
    {
        game->lollipops.value ++;
        game->lollipops.rng = 0;
    }
}