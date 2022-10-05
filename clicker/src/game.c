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

void lang_init(Game* game, const char* langc)
{
    if (langc == "fr_FR")
    {
        game->lang.newgame = "Nouvelle Partie";
        game->lang.language = "Langue";
        game->lang.load = "Charger";
        game->lang.save = "Sauvegarder";
        game->lang.save_slot_1 = "Sauvegarder sur le bloc 1";
        game->lang.save_slot_2 = "Sauvegarder sur le bloc 2";
        game->lang.save_slot_3 = "Sauvegarder sur le bloc 3";
        game->lang.save_slot_4 = "Sauvegarder sur le bloc 4";
        game->lang.load_slot_1 = "Charger sur le bloc 1";
        game->lang.load_slot_2 = "Charger sur le bloc  2";
        game->lang.load_slot_3 = "Charger sur le bloc 3";
        game->lang.load_slot_4 = "Charger sur le bloc 4";

        game->lang.kimchi_value = "Vous avez %d kimchi%s \x04";
        game->lang.lolis_value = "Vous avez %d sucette%s \x07";
        game->lang.kimchi_eat = "Manger tout les kimchis";
        game->lang.kimchi_eaten = "Vous avez mang\x82 %d kimchi%s";
        game->lang.kimchi_throw = "Jeter 10 kimchis.";
        game->lang.kimchi_thrown = "Vous avez jet\x82 %d kimchi%s. Sale gaspilleur.";
        game->lang.bribe = "Soudoyer le d\x82veloppeur avec 30 kimchis pour une nouvelle feature.";

        game->lang.menu = "Menu";
        game->lang.menu_map_the = "  La  ";
        game->lang.menu_map_map = "Carte.";
        game->lang.menu_kimchi = "Frigo de";
        game->lang.menu_fridge = " Kimchi ";
        game->lang.menu_debug = "Debug'";
        game->lang.menu_mini = "Mini";
        game->lang.menu_game = "Jeux";


        game->lang.dialog_shopkeeper = "Vendeur: Heya, camarade!";

        game->lang.error_load = "Impossible de charger la sauvegarde! Ce bloc est peut-\x88tre vide.";
    }
    else
    {
        game->lang.newgame = "New Game";
        game->lang.language = "Language";
        game->lang.load = "Load";
        game->lang.save = "Save";
        game->lang.save_slot_1 = "Save on slot 1";
        game->lang.save_slot_2 = "Save on slot 2";
        game->lang.save_slot_3 = "Save on slot 3";
        game->lang.save_slot_4 = "Save on slot 4";
        game->lang.load_slot_1 = "Load slot 1";
        game->lang.load_slot_2 = "Load slot 2";
        game->lang.load_slot_3 = "Load slot 3";
        game->lang.load_slot_4 = "Load slot 4";

        game->lang.kimchi_value = "You have %d kimchi%s \x04";
        game->lang.lolis_value = "You have %d lollipop%s \x07";
        game->lang.kimchi_eat = "Eat all the kimchis";
        game->lang.kimchi_eaten = "You have eaten %d kimchi%s Yummy";
        game->lang.kimchi_throw = "Throw 10 kimchis.";
        game->lang.kimchi_thrown = "You have thrown %d kimchi%s. You wasteful.";
        game->lang.bribe = "Bribe the dev' with 30 kimchis for a new feature.";

        game->lang.menu = "Menu";
        game->lang.menu_map_the = "The";
        game->lang.menu_map_map = "Map";
        game->lang.menu_kimchi = "Kimchi";
        game->lang.menu_fridge = "Fridge";
        game->lang.menu_debug = "Debug'";
        game->lang.menu_mini = "Mini";
        game->lang.menu_game = "Game";


        game->lang.dialog_shopkeeper = "Shopkeeper: Heya, my comrade!";
        game->lang.dialog_shopkeeper2 = "Shopkeeper: I can sell you my lollipop plains. Pick one and another grows! (150 kimchis)";
        game->lang.dialog_shopkeeper3 = "You acquired the lollipop plains!";
        game->lang.notenoughmoney = "You do not have enough kimchis. You need %d more to buy this.";

        game->lang.error_load = "Failed to load save! This slot may be unused.";
    }
}

void game_init(Game* game)
{
    game->kimchi.value= 0;
    game->timepassing = 0;
    game->kimchi.per_second = 20;
    game->visual.last_action = 0;
    game->kimchi.thrown = 0;
    game->visual.menu = false;
    game->visual.offset = 0;
    game->scene = 1; 
    game->is_dialog = 0;
    game->lollipops.value = 0;
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
