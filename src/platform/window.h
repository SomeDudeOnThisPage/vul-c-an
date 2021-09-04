#include "../game.h"
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#define API_VULKAN GLFW_NO_API

typedef struct {
    GLFWwindow *m_window;
    uint32_t m_extension_count;
} GameWindow_t;

GameWindow_t* window_create(uint32_t client_api, uint16_t size_x, uint16_t size_y);
void window_destroy(GameWindow_t *window);

#endif //GAME_WINDOW_H
