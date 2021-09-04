//
// Created by robin on 03.09.2021.
//
#include "vulkan.h"

uint32_t get_glfw_extensions(const char** extensions) {
    uint32_t glfw_extension_count = 0;
    extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    return glfw_extension_count;
}

void vulkan_destroy(VulkanInstance_t *instance) {
    printf("destroying instance '%p'\n", instance->instance);

    VkInstance iss = *instance->instance;
    vkDestroyInstance(iss, NULL);

    free(instance->instance);
    free(instance->a_info);
    free(instance->c_info);
    free(instance);
}

uint32_t vulkan_initialize(GameWindow_t *window, VulkanInstance_t *vulkan_instance) {

    const char** glfw_extensions;
    // uint32_t glfw_extension_count = get_glfw_extensions(glfw_extensions);
    uint32_t glfw_extension_count = 0;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    VkInstance instance;

    VkApplicationInfo info;
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Vulkan Babee";
    info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    info.pEngineName = "Vulkan Engine Babee";
    info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo c_info;
    c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    c_info.pApplicationInfo = &info;

    c_info.enabledExtensionCount = glfw_extension_count;
    c_info.ppEnabledExtensionNames = glfw_extensions;
    c_info.enabledLayerCount = 0;

    if (vkCreateInstance(&c_info, NULL, &instance) != VK_SUCCESS) {
         printf("failed to create vulkan instance");
         return -1;
    }

    vulkan_instance->instance = &instance;
    vulkan_instance->c_info = &c_info;
    vulkan_instance->a_info = &info;

    printf("created instance '%p'\n", &instance);

    return 1;
}

