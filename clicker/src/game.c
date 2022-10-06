#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <stdio.h>
#include "game.h"
#include "utils/utils.c"
#include <string.h>
#include "gui/gui.h"
#include "debug/debug.h"
#include "debug/debug.c"
#include "scenes/save.c"
#include "scenes/save.h"
#include "scenes/map.c"
#include "scenes/shop/shop.c"
#include "scenes/shop/shop_npc.c"
#include "scenes/kimchi_box.c"
#include "scenes/menu.c"
#include "scenes/main_menu.c"
#include "lang/lang.h"



void game_init(Game* game)
{
    game->kimchi.value= 0;
    game->timepassing = 0;
    game->kimchi.per_second = 1;
    game->visual.last_action = 0;
    game->kimchi.thrown = 0;
    game->visual.menu = false;
    game->visual.offset = 0;
    game->scene = 1; 
    game->is_dialog = 0;
    game->lollipops.value = 0;
    game->debug.FPS_show = false;
    lang_init(game, game->language);
}


//Display

void kimchi_display(Game* game)
{
    im_print(1, 0 + game->visual.offset, game->lang.kimchi_value, game->kimchi.value, game->kimchi.value > 1 ? "s." : ".");
}

void lollipop_display(Game* game)
{
    if (game->visual.menu >= 3)
    {
        im_print(1, 1 + game->visual.offset, game->lang.lolis_value, game->lollipops.value, game->lollipops.value > 1 ? "s." : ".");
    }
}

void last_action(Game* game)
{
    switch (game->visual.last_action)
    {
    case 1:
        print_text(0, 49, game->lang.kimchi_eaten, game->kimchi.eated, game->kimchi.eated > 1 ? "s." : ".");
        break;
    
    case 2:
        print_text(0, 49, game->lang.kimchi_thrown, game->kimchi.thrown, game->kimchi.thrown > 1 ? "s." : ".");
        break;
    case 51:
        print_text(0, 49, game->lang.dialog_shopkeeper);
        break;

    case 52:
        print_text(0, 49, game->lang.dialog_shopkeeper2);
        break;

    case 53:
        print_text(0, 49, game->lang.dialog_shopkeeper3);
        break;
    case 54:
        print_text(0,49, game->lang.notenoughmoney, 150 - game->kimchi.value);
        break;
    case 101:
        print_text(0, 49, game->lang.error_load);
        break;
    default:
        break;
    }
}

void display_game(Game* game)
{
    draw_box(-1,48,WINDOW_WIDTH + 1,48);
    last_action(game);

    switch (game->scene)
    {
    case 0:
        kimchi_display(game);
        scene0_kimchibox(game);
        break;
    
    case 1:
        scene1_mainmenu(game);
        break;
    
    case 2:
        kimchi_display(game);
        save_button(game, 1);
        save_button(game, 2);
        save_button(game, 3);
        save_button(game, 4);
        break;
    
    case 3:
        kimchi_display(game);
        display_map(game);
        break;

    case 4:
        kimchi_display(game);
        npc_shopkeeper(game);
        break;
    
    case 50:
        kimchi_display(game);
        lollipop_display(game);
        scene50_display(game);
        break;

    case 99:
        kimchi_display(game);
        scene99_debug(game);
        break;

    default:
        break;
    }
    /*display_dialog(game, game->currentDialog);*/


    menu_display(game);
    if (game->debug.FPS_show == true)
        im_print(WINDOW_WIDTH - 9,0,"%.0f FPS",60 / pg_io_get_frame_time());
}


void kimchi_increment(Game* game)
{
    game->frameTime = pg_io_get_frame_time();
    if (game->timepassing >= 1 / game->kimchi.per_second)
    {
        im_print(1, 5, "%d", game->timepassing);
        game->timepassing = 0;
        game->kimchi.value++;
    }
    game->timepassing += game->frameTime;
}









void game_update(Game* game)
{
    if (game->scene != 1)
    {kimchi_increment(game);}
    pg_clear_all();
    display_game(game);
}


void game_shutdown(Game* game)
{

}
