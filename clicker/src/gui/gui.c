
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>

#include "gui.h"

int im_print_text(int x, int y, const char* text)
{
    int i;
    for (i = 0; text[i] != '\0'; ++i)
        pg_put_char(x + i, y, text[i], PG_COL_REPLACE);
    return i;
}

int im_print(int x, int y, const char* format, ...)
{
    va_list list;
    va_start(list, format);

    char buffer[1024];

    vsprintf(buffer, format, list);
    int len = im_print_text(x, y, buffer);

    va_end(list);

    return len;
}

bool im_button(int x, int y, const char* text)
{
    int len = strlen(text);

    int mouseX, mouseY;
    pg_get_mouse_coords(&mouseX, &mouseY);
    
    bool clicked = false;
    bool hover = false;
    bool down = false;
    if (   mouseX >= x && mouseX < x + len
        && mouseY >= y && mouseY < y + 1)
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
        pg_set_default_bg_color(0xff666666);
    }
    else
    {
        pg_set_default_bg_color(0xff444444);
    }

    im_print_text(x, y, text);
    
    pg_set_default_bg_color(0xFF000000);
    pg_set_default_fg_color(0xFFFFFFFF);

    return clicked;
}