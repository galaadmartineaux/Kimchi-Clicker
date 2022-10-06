
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>

#include "gui.h"

int im_print_text(int x, int y, const char* text)
{
    for (int i = 0; text[i] != '\0'; ++i)
    {
        pg_put_char(x + i, y, text[i], PG_COL_REPLACE);
    }
}

int im_print(int x, int y, const char* txt, ...)
{
    va_list list;
    va_start(list, txt);

    char buffer[1024];

    vsprintf(buffer, txt, list);
    int len = im_print_text(x, y, buffer);

    va_end(list);
}

int im_print_color(int x, int y, unsigned int fg, unsigned int bg, const char* txt, ...)
{
    va_list list;
    pg_set_default_bg_color(bg);
    pg_set_default_fg_color(fg);
    im_print(x, y, txt, list);
    pg_set_default_bg_color(0xFF000000);
    pg_set_default_fg_color(0xFFFFFFFF);
}

bool im_button(int x, int y, const char* text)
{
    int length = strlen(text);

    int mouseX, mouseY;
    pg_get_mouse_coords(&mouseX, &mouseY);
    
    bool clicked = false;
    bool hover = false;
    bool down = false;
    if (   mouseX >= x && mouseX < x + length && mouseY >= y && mouseY < y + 1)
    {
        hover = true;
        clicked = pg_io_mouse_button_released(GLFW_MOUSE_BUTTON_LEFT);
        down = pg_io_mouse_button_down(GLFW_MOUSE_BUTTON_LEFT);
    }
    
    if (down)
    {
        pg_set_default_bg_color(0xFFFFFFFF);
        pg_set_default_fg_color(0xFF000000);
    }
    else if (hover)
    {
        pg_set_default_bg_color(0xFFAAAAAA);
    }
    else
    {
        pg_set_default_bg_color(0xFF222222);
    }

    im_print_text(x, y, text);
    
    pg_set_default_bg_color(0xFF000000);
    pg_set_default_fg_color(0xFFFFFFFF);

    return clicked;
}