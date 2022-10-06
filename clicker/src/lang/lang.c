#include "lang.h"
#include <stdio.h>
#include "../gui/gui.h"

void lang_init(Game* game, int langc)
{
    if (langc == 1)
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
        game->lang.debug_button_fps = "Afficher le compteur de FPS";
        game->lang.debug_button_kimchi_eated = "Kimchi Mang\x82s";
        game->lang.debug_button_kimchi_per_second = "K/s";
        game->lang.debug_button_kimchi_thrown = "Kimchi Jet\x82";
        game->lang.debug_button_lollipops = "Sucettes";
        game->lang.debug_button_visual_last_action = "Derni\x8Are action";
        game->lang.debug_menu_values = "Valeurs";
        game->lang.debug_menu_others = "Autre";
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
        game->lang.debug_button_fps = "Show FPS Counter";
        game->lang.debug_button_kimchi_eated = "Kimchi Eated";
        game->lang.debug_button_kimchi_per_second = "K/s";
        game->lang.debug_button_kimchi_thrown = "Kimchi Thrown";
        game->lang.debug_button_lollipops = "Lollipops";
        game->lang.debug_button_visual_last_action = "Last Action";
        game->lang.debug_menu_values = "Values";
        game->lang.debug_menu_others = "Others";
    }
}