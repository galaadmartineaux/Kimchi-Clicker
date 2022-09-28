
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "playground_internal.h"
#include "playground_backend_glfw.h"

static const char* CFG_KEY_WINDOW_POSX = "WindowPosX";
static const char* CFG_KEY_WINDOW_POSY = "WindowPosY";

enum keys_sticky_state
{
    PG_STICKY_UNSET = 0,                // 
    PG_STICKY_PRESSED = 1,              // Key was pressed this frame
    PG_STICKY_PRESSED_AND_RELEASED = 2, // Key was pressed and release this frame so keysDowns[x] will stay true until the next frame
    PG_STICKY_HANDLED = 3               // Event was handled, sticky state should return to 0 and keysDown[x] will be false
};

struct backend_context_t
{
    GLFWwindow* window;
    GLFWerrorfun error_callback;
    GLFWkeyfun key_callback;
    GLFWmousebuttonfun mouse_button_callback;
    GLFWscrollfun scroll_callback;
    GLFWwindowposfun window_pos_callback;

    enum keys_sticky_state keys_sticky[512];
    enum keys_sticky_state mouse_buttons_sticky[7];

    struct pg_io* io;
};

static struct backend_context_t bk_ctx = {0};

static void pg_glfw_error_callback(int errorCode, const char* description)
{
    fprintf(stderr, "GLFW error (0x%x): %s\n", errorCode, description);

    if (bk_ctx.error_callback)
        bk_ctx.error_callback(errorCode, description);
}

static void pg_bk_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    struct pg_io* io = (struct pg_io*)glfwGetWindowUserPointer(window);

    if (key >= 0 && key < PG_ARRAYSIZE(io->keys_down))
    {
        if (action == GLFW_PRESS)
        {
            bk_ctx.keys_sticky[key] = PG_STICKY_PRESSED;
            io->keys_down[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            if (bk_ctx.keys_sticky[key] == PG_STICKY_PRESSED)
                bk_ctx.keys_sticky[key] = PG_STICKY_PRESSED_AND_RELEASED;
            else
                io->keys_down[key] = false;
        }
    }

    if (bk_ctx.key_callback)
        bk_ctx.key_callback(window, key, scancode, action, mods);
}

static void pg_bk_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    struct pg_io* io = (struct pg_io*)glfwGetWindowUserPointer(window);

    if (button >= 0 && button < PG_ARRAYSIZE(io->mouse_buttons_down))
    {
        if (action == GLFW_PRESS)
        {
            bk_ctx.mouse_buttons_sticky[button] = PG_STICKY_PRESSED;
            io->mouse_buttons_down[button] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            if (bk_ctx.mouse_buttons_sticky[button] == PG_STICKY_PRESSED)
                bk_ctx.mouse_buttons_sticky[button] = PG_STICKY_PRESSED_AND_RELEASED;
            else
                io->mouse_buttons_down[button] = false;
        }
    }

    if (bk_ctx.mouse_button_callback)
        bk_ctx.mouse_button_callback(window, button, action, mods);
}

static void pg_bk_scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    struct pg_io* io = (struct pg_io*)glfwGetWindowUserPointer(window);

    io->mouse_wheel_offset += (float)yOffset;

    if (bk_ctx.scroll_callback)
        bk_ctx.scroll_callback(window, xOffset, yOffset);
}

static void pg_bk_glfw_window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    cfg_set_int(pg_get_config(), CFG_KEY_WINDOW_POSX, xpos);
    cfg_set_int(pg_get_config(), CFG_KEY_WINDOW_POSY, ypos);

    if (bk_ctx.window_pos_callback)
        bk_ctx.window_pos_callback(window, xpos, ypos);
}

int pg_backend_glfw_init(GLFWwindow* window)
{
    bk_ctx.window = window;
    bk_ctx.error_callback        = glfwSetErrorCallback(pg_glfw_error_callback);
    bk_ctx.window_pos_callback   = glfwSetWindowPosCallback(window, pg_bk_glfw_window_pos_callback);
    bk_ctx.key_callback          = glfwSetKeyCallback(window, pg_bk_key_callback);
    bk_ctx.mouse_button_callback = glfwSetMouseButtonCallback(window, pg_bk_mouse_button_callback);
    bk_ctx.scroll_callback       = glfwSetScrollCallback(window, pg_bk_scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    printf("GLFW_VERSION: %s\n", glfwGetVersionString());

    struct pg_io* io = pg_get_io();
    glfwSetWindowUserPointer(window, io);
    glfwGetWindowSize(window, &io->display_width, &io->display_height);

    return 0;
}

void pg_backend_glfw_poll_events(void)
{
    struct pg_io* io = (struct pg_io*)glfwGetWindowUserPointer(bk_ctx.window);

    memcpy(io->keys_down_prev, io->keys_down, sizeof(io->keys_down));
    memcpy(io->mouse_buttons_down_prev, io->mouse_buttons_down, sizeof(io->mouse_buttons_down));
    io->mouse_wheel_offset = 0.f;

    // TODO(Paul): Move poll event outside backend
    glfwPollEvents();

    // Update screen size
    {
        int window_width, window_height;
        glfwGetWindowSize(bk_ctx.window, &window_width, &window_height);
        if (window_height != bk_ctx.io->display_width || window_width != bk_ctx.io->display_height)
            glfwSetWindowSize(bk_ctx.window, bk_ctx.io->display_width, bk_ctx.io->display_height);
    }

    // Update keyboard
    for (int i = 0; i < PG_ARRAYSIZE(bk_ctx.keys_sticky); ++i)
    {
        switch (bk_ctx.keys_sticky[i])
        {
        case PG_STICKY_PRESSED:
            bk_ctx.keys_sticky[i] = PG_STICKY_UNSET;
            break;
        case PG_STICKY_PRESSED_AND_RELEASED:
            bk_ctx.keys_sticky[i] = PG_STICKY_HANDLED;
            break;
        case PG_STICKY_HANDLED:
            bk_ctx.keys_sticky[i] = 0;
            io->keys_down[i] = false;
            break;
        default:;
        }
    }

    // Update mouse position
    {
        double mouseX, mouseY;
        glfwGetCursorPos(bk_ctx.window, &mouseX, &mouseY);
        io->mouse_position[0] = (float)mouseX;
        io->mouse_position[1] = (float)mouseY;
    }

    // Update mouse buttons
    for (int i = 0; i < PG_ARRAYSIZE(bk_ctx.mouse_buttons_sticky); ++i)
    {
        switch (bk_ctx.mouse_buttons_sticky[i])
        {
        case PG_STICKY_PRESSED:
            bk_ctx.mouse_buttons_sticky[i] = PG_STICKY_UNSET;
            break;
        case PG_STICKY_PRESSED_AND_RELEASED:
            bk_ctx.mouse_buttons_sticky[i] = PG_STICKY_HANDLED;
            break;
        case PG_STICKY_HANDLED:
            bk_ctx.mouse_buttons_sticky[i] = 0;
            io->mouse_buttons_down[i] = false;
            break;
        default:;
        }
    }

    // Update time related variables
    // TODO(Paul): See where else to put it
    {
        double time = glfwGetTime();
        if (io->time > 0.0)
            io->frame_time = (float)(time - io->time);
        io->time = time;
    }
}

int pg_backend_glfw_helper_init(GLFWwindow** window, const char* title, struct pg_config_t config)
{
    if (!glfwInit())
    {
        const char* errorString; glfwGetError(&errorString);
        fprintf(stderr, "Cannot init glfw: %s\n", errorString);
        return 1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Minimal version to support core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    *window = glfwCreateWindow(16, 16, title, NULL, NULL);
    if (*window == NULL)
    {
        fprintf(stderr, "glfwCreateWindow() failed\n");
        return 1;
    }

    if (pg_backend_glfw_init(*window) != 0)
    {
        fprintf(stderr, "pg_backend_glfw_init() failed\n");
        return 1;
    }

    if (pg_init(glfwGetProcAddress, config) != 0)
    {
        fprintf(stderr, "pg_init() failed\n");
        return 1;
    }

    bk_ctx.io = pg_get_io();

    int window_pos_x = cfg_get_int(pg_get_config(), CFG_KEY_WINDOW_POSX, 0);
    int window_pos_y = cfg_get_int(pg_get_config(), CFG_KEY_WINDOW_POSY, 0);
    if (window_pos_x > 0 && window_pos_y > 0)
    {
        glfwSetWindowPos(bk_ctx.window, window_pos_x, window_pos_y);
    }

    glfwSetWindowSizeLimits(*window, bk_ctx.io->display_width, bk_ctx.io->display_height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    return 0;
}
