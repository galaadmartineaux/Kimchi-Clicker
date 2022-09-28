
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <playground_backend_glfw.h>

int main(int argc, char* argv[])
{
    // Create a 40x20 console using "assets/VGA8x16.png" tileset
    // The underlying window is set with "Hello world" title and returned for further use
    GLFWwindow* window;
    pg_backend_glfw_helper_init(&window, "Hello world", pg_config_simple(40, 20, "assets/VGA8x16.png"));

    // Main loop run until user close window
    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        // Poll events must be called to process the OS inputs (key/mouse/window buttons)
        pg_backend_glfw_poll_events();

        // Clear console
        pg_clear_all();

        // Print hello world string
        {
            const char* str = "Hello World!";
            for (int i = 0; str[i] != '\0'; ++i)
                pg_put_char(i, 0, str[i], PG_COL_REPLACE);
        }

        // Print help string
        {
            const char* str = "(F1, F2, F3 should zoom in/out)";
            for (int i = 0; str[i] != '\0'; ++i)
                pg_put_char(i, 1, str[i], PG_COL_REPLACE);
        }

        // Render to screen
        pg_render();
        glfwSwapBuffers(window);
    }
    
    // Shutdown library
    pg_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
