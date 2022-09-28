
#include <string.h>
#include <math.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <playground.h>
#include <playground_backend_glfw.h>

// h from 0.0 (0 deg) to 1.0 (360 deg) of the color wheel
unsigned int hsv_to_rgb(float h, float s, float v)
{
    float hp = 6 * fmodf((fmodf(h, 1.f) + 1.f), 1.f);
    float c = s * v;
    float x = c * (1 - fabsf(fmodf(hp, 2) - 1));
    float m = v - c;
    float r = 0, g = 0, b = 0;
    if (hp <= 1.f) {
        r = c;
        g = x;
    } else if (hp <= 2.f) {
        r = x;
        g = c;
    } else if (hp <= 3.f) {
        g = c;
        b = x;
    } else if (hp <= 4.f) {
        g = x;
        b = c;
    } else if (hp <= 5.f) {
        r = x;
        b = c;
    } else {
        r = c;
        b = x;
    }
    r += m;
    g += m;
    b += m;
    return (0xFF << 24) 
        | ((int)(r * 255.f) << 16)
        | ((int)(g * 255.f) << 8)
        | ((int)(b * 255.f) << 0);
}

void print_text(int x, int y, int width, const char* str, unsigned int color)
{
    for (int i = 0; str[i] != '\0'; ++i)
        pg_put_char_ex((x + i) % width, y, str[i], color, 0, PG_COL_NONE);
}

int main(int argc, char* argv[])
{
    int width = 40;
    int height = 20;

    GLFWwindow* window;
    pg_backend_glfw_helper_init(&window, "Hello world", pg_config_simple(width, height, "assets/VGA8x16.png"));

    int frameId = 0;

    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        pg_backend_glfw_poll_events();

        pg_clear_all();

        const char* str = "SCROLLING TEXT!";
        int len = strlen(str);
        int offsetX = frameId / 3;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; str[j] != '\0'; ++j)
            {
                unsigned int color = hsv_to_rgb((offsetX - j - i) / (float)len, 1.f, 1.f);
                pg_put_char_ex((offsetX + j + i) % width, i , str[j], color, 0, PG_COL_NONE);
            }
        }

        // Render to screen
        pg_render();
        glfwSwapBuffers(window);

        frameId += 1;
    }
    
    // Shutdown library
    pg_shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
