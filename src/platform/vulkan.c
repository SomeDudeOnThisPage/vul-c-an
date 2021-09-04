//
// Created by robin on 03.09.2021.
//
#include <string.h>
#include "vulkan.h"

void vulkan_destroy(VulkanInstance_t *instance) {
    printf("destroying instance '%p'\n", instance->instance);

    VkInstance vk_instance = *instance->instance;
    vkDestroyInstance(vk_instance, NULL);
}

uint32_t query_extension_count() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);
    return extension_count;
}

void enumerate_extensions(uint32_t extension_count, VkExtensionProperties *extensions) {
    // query the info of available extensions
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);
    printf("Queried %d extensions:\n", extension_count);
    for (int i = 0; i < extension_count; i++) {
        printf("%d:\t%s\n", i, extensions[i].extensionName);
    }
}

uint32_t check_glfw_extensions(const uint32_t *glfw_extension_count, const char **glfw_extensions, uint32_t extension_count, VkExtensionProperties *extensions) {
    printf("GLFW requires the following %d extensions:\n", *glfw_extension_count);

    for (int i = 0; i < *glfw_extension_count; i++) {
        // check if we support all extensions required by glfw
        int supported = 0;
        // check if our extensions support the required glfw extension
        for (int j = 0; j < extension_count; j++) {
            if (strcmp(glfw_extensions[i], extensions[j].extensionName) == 0) {
                supported = 1;
                break;
            }
        }

        printf("%d:\t%s\t%s", i, glfw_extensions[i], supported == 1 ? "SUPPORTED\n" : "NOT SUPPORTED\n");

        if (!supported) {
            return 0;
        }
    }

    return 1;
}

uint32_t vulkan_initialize(GameWindow_t *window, VulkanInstance_t *vulkan_instance) {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    // check for all available extensions
    uint32_t extension_count = query_extension_count();
    VkExtensionProperties extensions[extension_count];
    enumerate_extensions(extension_count, extensions);

    // make sure we support all required glfw extensions
    if (!check_glfw_extensions(&glfw_extension_count, glfw_extensions, extension_count, extensions)) {
        printf("Failed to query all required extensions for GLFW");
        return 0;
    }

    // create application info containing version and names
    VkApplicationInfo a_info;
    a_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    a_info.pApplicationName = "Vulkan Babee";
    a_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    a_info.pEngineName = "Vulkan Engine Babee";
    a_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    a_info.apiVersion = VK_API_VERSION_1_0;

    // create instance creation info containing the application info and the extensions
    VkInstanceCreateInfo c_info;
    c_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    c_info.pApplicationInfo = &a_info;
    c_info.enabledExtensionCount = glfw_extension_count;
    c_info.ppEnabledExtensionNames = glfw_extensions;
    c_info.enabledLayerCount = 0;

    // create the actual instance using the instance creation info, check for creation failure
    VkInstance *instance = malloc(sizeof(VkInstance));
    if (vkCreateInstance(&c_info, NULL, instance) != VK_SUCCESS) {
         printf("failed to create vulkan instance");
         return 0;
    }

    // store a reference to the instance in the vulkan-stuff struct
    vulkan_instance->instance = instance;

    // print :)
    printf("created instance '%p'\n", instance);

    return 1;
}

