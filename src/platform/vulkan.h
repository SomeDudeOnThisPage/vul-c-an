#pragma once

#include <vulkan/vulkan.h>
#include <malloc.h>
#include <string.h>
#include "window.h"
#include "../debug.h"

/**
 * The main struct containing all references to the stuff we need to instance the vulkan API.
 * Mostly, this includes the actual VkInstance, the physical and logical devices, as well as
 * any queues we are using.
 */
typedef struct {
    VkInstance          *instance;
    VkPhysicalDevice    **physical_devices;
    uint32_t            physical_device;
    VkDevice            *logical_device;

    uint32_t            validation_layer_count;
    const char          **validation_layers;

    uint32_t graphics_queue_family;
} VulkanInstance_t;

uint8_t vulkan_initialize(GameWindow_t *window, VulkanInstance_t *vulkan_instance);
uint8_t vulkan_initialize_physical_device(VulkanInstance_t *vulkan_instance);
uint8_t vulkan_initialize_queue_families(VulkanInstance_t *vulkan_instance);
uint8_t vulkan_initialize_logical_device(VulkanInstance_t *vulkan_instance);
void vulkan_destroy(VulkanInstance_t *instance);