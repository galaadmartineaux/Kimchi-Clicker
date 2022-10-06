#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../utils/constants.h"
#include "../game.h"

void debug_button_kimchi_per_second(Game* game)
{
    if (im_button(1, 10, game->lang.debug_button_kimchi_per_second) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi.per_second += 1.0;
    }
    else if (im_button(1, 10, game->lang.debug_button_kimchi_per_second) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.per_second -= 1.0;
    }
    print_text(strlen(game->lang.debug_button_kimchi_per_second)+1, 10," = %1.f", game->kimchi.per_second);
    print_text(1, 9,"%1.d", pg_io_key_down(GLFW_KEY_RIGHT));
}
void debug_button_visual_last_action(Game* game)
{
    if (im_button(1, 12, game->lang.debug_button_visual_last_action) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->visual.last_action ++;
    }
    else if (im_button(1, 12, game->lang.debug_button_visual_last_action) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->visual.last_action --;
    }
    print_text(strlen(game->lang.debug_button_visual_last_action) +1, 12," = %d", game->visual.last_action);
}
void debug_button_kimchi_eated(Game* game)
{
    if (im_button(1, 14, game->lang.debug_button_kimchi_eated) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi.eated ++;
    }
    else if (im_button(1, 14, game->lang.debug_button_kimchi_eated) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.eated --;
    }
    print_text(strlen(game->lang.debug_button_kimchi_eated)+1, 14," = %d", game->kimchi.eated);
}
void debug_button_kimchi_thrown(Game* game)
{
    if (im_button(1, 16, game->lang.debug_button_kimchi_thrown) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->kimchi.thrown ++;
    }
    else if (im_button(1, 16, game->lang.debug_button_kimchi_thrown) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->kimchi.thrown --;
    }
    print_text(strlen(game->lang.debug_button_kimchi_thrown)+1, 16," = %d", game->kimchi.thrown);
}

void debug_button_lollipops(Game* game)
{
    if (im_button(1, 18, game->lang.debug_button_lollipops) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->lollipops.value ++;
    }
    else if (im_button(1, 18, game->lang.debug_button_lollipops) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->lollipops.value --;
    }
    print_text(strlen(game->lang.debug_button_lollipops)+1, 18," = %d", game->lollipops.value);
}

void debug_button_menu(Game* game)
{
    if (im_button(1, 20, game->lang.menu) == true && pg_io_key_down(GLFW_KEY_RIGHT))
    {
        game->visual.menu ++;
    }
    else if (im_button(1, 20, game->lang.menu) == true && pg_io_key_down(GLFW_KEY_LEFT))
    {
        game->visual.menu --;
    }
    print_text(strlen(game->lang.menu)+1, 20," = %d", game->visual.menu);
    if (game->visual.menu >= 1)
    {
        game->visual.offset = 6;
    }
    else
    {
        game->visual.offset = 0;
    }
}

void debug_button_fps(Game* game)
{
    if (im_button(24,10, game->lang.debug_button_fps) == true)
    {
        if (game->debug.FPS_show == true)
        {
            game->debug.FPS_show = false;
        }
        else if (game->debug.FPS_show == false)
        {
            game->debug.FPS_show = true;
        }
    }
}

void debug_visualadapt(Game* game)
{
    if (game->language == 1)
    {
        draw_box(0,8,strlen(game->lang.debug_button_visual_last_action) + strlen(" = x") + 1,22);
        im_print_color(7,8, 0xFF000000, 0xFFFFFFFF, game->lang.debug_menu_values);
        draw_box(strlen(game->lang.debug_button_visual_last_action) + strlen(" = x") + 4, 8,
        strlen(game->lang.debug_button_visual_last_action) + strlen(" = x") + strlen(game->lang.debug_button_fps) + 7, 22);
        im_print_color(35,8, 0xFF000000, 0xFFFFFFFF, game->lang.debug_menu_others);
    }
    else
    {
        draw_box(0,8,strlen(game->lang.debug_button_kimchi_thrown) + strlen(" = x") + 2,22);
        im_print_color(7,8, 0xFF000000, 0xFFFFFFFF, game->lang.debug_menu_values);
        draw_box(strlen(game->lang.debug_button_visual_last_action) + strlen(" = x") + 8, 8,
        strlen(game->lang.debug_button_visual_last_action) + strlen(" = x") + strlen(game->lang.debug_button_fps) + 11, 22);
        im_print_color(30,8, 0xFF000000, 0xFFFFFFFF, game->lang.debug_menu_others);
    }
}

void scene99_debug(Game* game)
{
    debug_visualadapt(game);
    debug_button_kimchi_per_second(game);
    debug_button_kimchi_thrown(game);
    debug_button_kimchi_eated(game);
    debug_button_visual_last_action(game);
    debug_button_lollipops(game);
    debug_button_menu(game);
    debug_button_fps(game);
    if (game->debug.FPS_show == true)
        im_print_text(25 + strlen(game->lang.debug_button_fps), 10, "\xFB");
}