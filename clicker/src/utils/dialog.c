#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../game.h"


//à refaire
//J'en ai marre.
/*int dialog_msg(Game* game, char message1, char message2)
{
    im_print(0, 42, "\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB");
    im_print(0, 43, "\xBA                                                \xBA");
    im_print(0, 44, "\xBA                                                \xBA");
    im_print(0, 45, "\xBA                                                \xBA");
    im_print(0, 46, "\xBA                                                \xBA");
    im_print(0, 47, "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC");

    im_print(2, 44, message1);
    im_print(2, 44, message2);

    if (im_button(47, 46, "\x1A") == true)
    {
        game->is_dialog = 0;
    }
}

void display_dialog(Game* game, int index)
{
    if (game->is_dialog == 1)
    {
        switch (index)
        {
        case 1:
            dialog_msg(game, "char message1", "test2");
            break;
        
        default:
            break;
        }
    }
}*/