#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../game.h"
void visual_shopkeeper(Game* game)
{
    print_text(9, 9, " ___ ");
    print_text(9,11, " \x5c /");
    print_text(9,12, "  |  ");
    print_text(8,13, "___|_________");
    print_text(8,14, "|           |");
    print_text(8,15, "|           |");



    if (game->visual.last_action == 51)
    {
        print_text(9,10, "('w')");
    }
    else if (game->visual.last_action == 52)
    {
        print_text(9,10, "(UwU)");
    }
    else if (game->visual.last_action == 53)
    {
        print_text(9,10, "(^w^)");
    }
    else if (game->visual.last_action == 54)
    {
        print_text(9,10, "('-')");
    }
    else
    {
        print_text(9,10, "(~w~)");
    }
}


int npc_shopkeeper(Game* game)
{
    visual_shopkeeper(game);
    if (game->visual.last_action != 51 && game->visual.last_action != 52 && game->visual.last_action != 53 && game->visual.last_action != 54 && game->visual.menu < 3)
    {
        if (im_button(15,10, "Talk to the shopkeeper."))
        {
            game->visual.last_action = 51;
        }
    }
    else if (game->visual.last_action == 51)
    {
        if (im_button(15,10, "I'm good, I want to buy something from you."))
        {
            game->visual.last_action = 52;
        }
    }
    else if (game->visual.last_action == 52 || game->visual.last_action == 54)
    {
        if (im_button(15,10, "[Buy the lollipop forest for 150 kimchis.]"))
        {
            if (game->kimchi.value >= 150)
            {
                game->visual.last_action = 53;
                game->kimchi.value -= 150;
                game->visual.menu += 1;
            }
            else
            {
                game->visual.last_action = 54;
            }
        }
        
    }
    
    

}