#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../game.h"

int npc_shopkeeper(Game* game)
{
    print_text(9, 9, " ___ ");
    print_text(9,10, "(~w~)");
    print_text(9,11, " \x5c /");
    print_text(9,12, "  |  ");
    print_text(9,13, "  |  ");
    print_text(9,14, " / \x5c ");

    if (im_button(15,10, "Talk to the shopkeeper."))
    {
        game->visual.last_action = 51;
    }
}