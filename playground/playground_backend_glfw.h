#pragma once

#include "playground.h"

typedef struct GLFWwindow GLFWwindow;

int pg_backend_glfw_init(GLFWwindow* window);
int pg_backend_glfw_helper_init(GLFWwindow** window, const char* title, struct pg_config_t config);
void pg_backend_glfw_poll_events(void);
