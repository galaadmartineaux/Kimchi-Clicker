#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../utils/dialog.c"
#include "../game.h"

void printflag(int x, int y, const char* country)
{
    if (country == "france")
    {
        for(int i = 0; i<5; i++)
        {
            pg_set_default_fg_color(0xFF0000FF);
            pg_set_default_bg_color(0xFF000000);
            im_print_text(x, y+i, "\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFFFFFF);
            pg_set_default_bg_color(0xFF000000);
            im_print_text(x+3, y+i, "\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFF0000);
            pg_set_default_bg_color(0xFF000000);
            im_print_text(x+6, y+i, "\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFFFFFF);
            pg_set_default_bg_color(0xFF000000);
        }
    }
    else
    {
        for(int i = 0; i<2; i++)
        {
            pg_set_default_fg_color(0xFFFFFFFF);
            pg_set_default_bg_color(0xFF000000);
            im_print_text(x, y+i, "\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFF0000);
            im_print_text(x+3, y+i, "\xDB");
            pg_set_default_fg_color(0xFFFFFFFF);
            im_print_text(x+4, y+i, "\xDB\xDB\xDB");
        }
            pg_set_default_fg_color(0xFFFF0000);
            pg_set_default_bg_color(0xFFFFFFFF);
            im_print_text(x, y+2, "\xDB\xDB\xDB\xDB\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFF0000);

        for(int i = 3; i<5; i++)
        {
            pg_set_default_fg_color(0xFFFFFFFF);
            pg_set_default_bg_color(0xFF000000);
            im_print_text(x, y+i, "\xDB\xDB\xDB");
            pg_set_default_fg_color(0xFFFF0000);
            im_print_text(x+3, y+i, "\xDB");
            pg_set_default_fg_color(0xFFFFFFFF);
            im_print_text(x+4, y+i, "\xDB\xDB\xDB");
        }
    }
    
    
        
}