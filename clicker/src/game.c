

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <stdio.h>
#include "game.h"
#include "utils.c"
#include <string.h>
#include "gui.h"
#include "debug/debug.h"
#include "debug/debug.c"

void game_init(Game* game)
{
    game->kimchi = 0;
    game->timepassing = 0;
    game->kimchi_per_second = 1;
    game->last_action = 0;
    game->kimchi_thrown = 0;
    game->kimchi_menu = false;
    game->kimchi_offset = 0;
    game->scene = 0;
}


//Buttons
void button_eatkimchis(Game* game)
{
    if (im_button(1, 2 + game->kimchi_offset, "Eat all the kimchis") == true)
    {
        game->kimchi_eated += game->kimchi;
        game->kimchi = 0;
        game->last_action = 1;
    }
}


void button_throwkimchis(Game* game)
{
    if (im_button(1, 4 + game->kimchi_offset, "Throw 10 kimchis.") == true)
    {
        game->kimchi_thrown += 10;
        game->kimchi -= 10;
        game->last_action = 2;
    }
}

void button_enablemenu(Game* game)
{
    if (im_button(1, 6 + game->kimchi_offset, "Bribe the dev' with 30 kimchis for a new feature.") == true)
    {
        game->kimchi -= 30;
        game->last_action = 3;
        game->kimchi_menu = 1;
        game->kimchi_offset = 6;
    }
}

//Scene
//Buttons
void btsc_kimchi(Game* game)
{
    if (im_button(3, 2, "Kimchi") == true)
    {
        
    }
    if (im_button(3, 3, "Fridge") == true)
    {
        
    }
}



void button_debug(Game* game)
{
    if (game->scene != 99)
    {
        if (im_button(40, 2, "Debug'") == true)
        {
            game->scene = 99;
        }
        if (im_button(40, 3, " Menu ") == true)
        {
            game->scene = 99;
        }
    }
    else 
    {
        if (im_button(40, 2, "Debug'") == true)
        {
            game->scene = 0;
        }
        if (im_button(40, 3, " Menu ") == true)
        {
            game->scene = 0;
        }
    }
}









//Display
void display_game(Game* game)
{
    
    
    im_print(1, 0 + game->kimchi_offset, "You have %d kimchis", game->kimchi);
    print_text(0, 48, "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD", 0xC4);



    switch (game->last_action)
    {
    case 1:
        print_text(0, 49,"You have eaten %d kimchis.", game->kimchi_eated);
        break;
    
    case 2:
        print_text(0, 49,"You have thrown %d kimchis. You wasteful.", game->kimchi_thrown);
        break;
    
    default:
        break;
    }


    if (game->scene == 0)
    {
        switch (game->kimchi)
        {
        case 1 ... 9:
            button_eatkimchis(game);
            break;
        
        case 10 ... 29:
            button_eatkimchis(game);
            button_throwkimchis(game);
            break;

        case 30 ... 100:
            button_eatkimchis(game);
            button_throwkimchis(game);
            button_enablemenu(game);
            break;

        default:
            break;
        }
    }
    else if (game->scene == 99)
    {
        debug_button_kimchi_per_second(game);
        debug_button_kimchi_thrown(game);
        debug_button_kimchi_eated(game);
        debug_button_last_action(game);
    }

    if (game->kimchi_menu == 1)
    {
        im_print(0, 0, "\xC9\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBB");
        im_print(0, 1, "\xBA                                                \xBA");
        im_print(0, 2, "\xBA                                                \xBA");
        im_print(0, 3, "\xBA                                                \xBA");
        im_print(0, 4, "\xBA                                                \xBA");
        im_print(0, 5, "\xC8\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xBC");
        btsc_kimchi(game);
    }

    button_debug(game);

}












void game_update(Game* game)
{
    //pg_io_get_frame_time();
    if (game->timepassing == 50)
    {
        game->timepassing = 0;
        game->kimchi += game->kimchi_per_second;
    }


    pg_clear_all();
    display_game(game);
    game->timepassing++;
}


void game_shutdown(Game* game)
{

}
