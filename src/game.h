#pragma once

#include "platform/vulkan.h"
#include "platform/window.h"

#define GLFW_INCLUDE_VULKAN

typedef struct {
    VulkanInstance_t *vulkan_instance;
    GameWindow_t *window;
} Game_t;

void game_destroy(Game_t *game);
