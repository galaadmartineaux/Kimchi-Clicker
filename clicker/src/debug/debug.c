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
        game->kimchi.per_second += 1.0;
    }
    else if (im_button(1, 8, "K/s = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.per_second -= 1.0;
    }
    print_text(8, 8,"%1.f", game->kimchi.per_second);
    print_text(8, 9,"%1.d", pg_io_key_down(GLFW_KEY_RIGHT));
}
void debug_button_visual_last_action(Game* game)
{
    if (im_button(1, 10, "Last Action = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->visual.last_action ++;
    }
    else if (im_button(1, 10, "Last Action = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->visual.last_action --;
    }
    print_text(15, 10,"%d", game->visual.last_action);
}
void debug_button_kimchi_eated(Game* game)
{
    if (im_button(1, 12, "Kimchi Eated = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi.eated ++;
    }
    else if (im_button(1, 12, "Kimchi Eated = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.eated --;
    }
    print_text(15, 12,"%d", game->kimchi.eated);
}
void debug_button_kimchi_thrown(Game* game)
{
    if (im_button(1, 14, "Kimchi Thrown = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi.thrown ++;
    }
    else if (im_button(1, 14, "Kimchi Thrown = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.thrown --;
    }
    print_text(18, 14,"%d", game->kimchi.thrown);
}

void debug_button_lollipops(Game* game)
{
    if (im_button(1, 16, "Lollipops = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->lollipops.value ++;
    }
    else if (im_button(1, 16, "Lollipops = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->lollipops.value --;
    }
    print_text(13, 16,"%d", game->lollipops.value);
}

void debug_button_menu(Game* game)
{
    if (im_button(1, 16, "Menu = ") == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->visual.menu ++;
    }
    else if (im_button(1, 16, "Menu = ") == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->visual.menu --;
    }
    print_text(13, 16,"%d", game->visual.menu);
}

void scene99_debug(Game* game)
{
    debug_button_kimchi_per_second(game);
    debug_button_kimchi_thrown(game);
    debug_button_kimchi_eated(game);
    debug_button_visual_last_action(game);
}