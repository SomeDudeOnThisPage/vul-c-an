//
// Created by robin on 03.09.2021.
//
#include <malloc.h>
#include <string.h>
#include "../game.h"
#include "window.h"

#ifndef GAME_VULKAN_H
#define GAME_VULKAN_H

typedef struct {
    VkInstance *instance;
    uint32_t x;
} VulkanInstance_t;

uint32_t vulkan_initialize(GameWindow_t *window, VulkanInstance_t *vulkan_instance);
void vulkan_destroy(VulkanInstance_t *instance);

#endif
