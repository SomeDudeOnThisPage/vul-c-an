//
// Created by robin on 03.09.2021.
//

#ifndef GAME_GAME_H
#define GAME_GAME_H

#ifdef DEBUG
    #define DEBUG_PRINT printf
#else
    #define DEBUG_PRINT(...)
#endif

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "platform/vulkan.h"
#include "platform/window.h"

typedef struct {
    VulkanInstance_t *vulkan_instance;
    GameWindow_t *window;
} Game_t;

void game_destroy(Game_t *game);

#endif //GAME_GAME_H
