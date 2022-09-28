
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <playground_backend_glfw.h>

#include "game.h"
#include "../tests/test_button.h"

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    pg_backend_glfw_helper_init(&window, "Kimchi Clicker", pg_config_simple(50, 50, "assets/DB_curses_12x12.png"));

    int testIndex = 0;

    Game game = {0};
    game_init(&game);
    //load_save();






    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        pg_backend_glfw_poll_events();


        switch (testIndex)
        {
            case 1: test_button_update(); break;
            // Add other tests here

            default: game_update(&game);
        };

        pg_render();
        glfwSwapBuffers(window);
    }
    











    game_shutdown(&game);
    pg_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
