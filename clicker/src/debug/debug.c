#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../game.h"

void debug_button_kimchi_per_second(Game* game)
{
    if (im_button(1, 8, "K/s = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi_per_second ++;
    }
    else if (im_button(1, 8, "K/s = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi_per_second --;
    }
    print_text(8, 8,"%d", game->kimchi_per_second);
    print_text(8, 9,"%d", pg_io_key_down(GLFW_KEY_RIGHT));
}
void debug_button_last_action(Game* game)
{
    if (im_button(1, 10, "Last Action = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->last_action ++;
    }
    else if (im_button(1, 10, "Last Action = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->last_action --;
    }
    print_text(15, 10,"%d", game->last_action);
}
void debug_button_kimchi_eated(Game* game)
{
    if (im_button(1, 12, "Kimchi Eated = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi_eated ++;
    }
    else if (im_button(1, 12, "Kimchi Eated = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi_eated --;
    }
    print_text(15, 12,"%d", game->kimchi_eated);
}
void debug_button_kimchi_thrown(Game* game)
{
    if (im_button(1, 14, "Kimchi Thrown = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi_thrown ++;
    }
    else if (im_button(1, 14, "Kimchi Thrown = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi_thrown --;
    }
    print_text(18, 14,"%d", game->kimchi_thrown);
}