#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <playground.h>
#include <playground_backend_glfw.h>
#include <stdarg.h>
#include <stdio.h>
#include "../game.h"
void save(Game* game, int index)
{
    char buffer[19];
    sprintf(buffer, "save%d.sav", index);
    FILE* savefile = fopen(buffer, "w");
    fwrite(game, sizeof(Game), 1, savefile);

    fclose(savefile);
}

int load(Game* game, int index)
{
    char buffer[19];
    sprintf(buffer, "save%d.sav", index);
    FILE* savefile = fopen(buffer, "r");
    if (savefile == NULL)
    {
        return 1;
    }
    else
    {
        fread(game, sizeof(Game), 1, savefile);
        fclose(savefile);
    }
}



void save_button(Game* game, int index)
{
    if (im_button(1, 8, game->lang.save_slot_1) == true)
    {
        save(game, 1);
    }
    if (im_button(1, 9, game->lang.load_slot_1) == true)
    {
        load(game, 1);
    }
    if (im_button(1, 10, game->lang.save_slot_2) == true)
    {
        save(game, 2);
    }
    if (im_button(1, 11, game->lang.load_slot_2) == true)
    {
        load(game, 2);
    }
    if (im_button(1, 12, game->lang.save_slot_3) == true)
    {
        save(game, 3);
    }
    if (im_button(1, 13, game->lang.load_slot_3) == true)
    {
        load(game, 3);
    }
    if (im_button(1, 14, game->lang.save_slot_4) == true)
    {
        save(game, 4);
    }
    if (im_button(1, 15, game->lang.load_slot_4) == true)
    {
        load(game, 4);
    }
}

