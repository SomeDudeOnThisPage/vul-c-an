#pragma once

#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include <GLFW/glfw3.h>
#include "../debug.h"

#define API_VULKAN GLFW_NO_API

typedef struct {
    GLFWwindow *m_window;
    uint32_t m_extension_count;
} GameWindow_t;

GameWindow_t* window_create(uint32_t client_api, uint16_t size_x, uint16_t size_y);
void window_destroy(GameWindow_t *window);
