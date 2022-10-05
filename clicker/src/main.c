
#include <stdarg.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <playground_backend_glfw.h>

#include "constants.h"
#include "game.h"
#include "utils/utils.h"
//#include "../tests/test_button.h"

int main(int argc, char* argv[])
{
    GLFWwindow* window;
    pg_backend_glfw_helper_init(&window, "Kimchi Clicker", pg_config_simple(WINDOW_WIDTH, 50, "assets/DB_curses_12x12.png"));

    //int testIndex = 0;

    Game game = {0};
    game_init(&game);


    while (glfwWindowShouldClose(window) == GLFW_FALSE && pg_io_key_down(GLFW_KEY_ESCAPE) != 1)
    {
        pg_backend_glfw_poll_events();


        game_update(&game);

        pg_render();
        glfwSwapBuffers(window);
    }
    











    game_shutdown(&game);
    pg_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
