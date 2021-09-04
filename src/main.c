#include "game.h"

#include "platform/window.h"
#include "platform/vulkan.h"

void game_cleanup(VulkanInstance_t *vk, GameWindow_t *window) {
    vulkan_destroy(vk);
    window_destroy(window);
}

uint8_t main() {
    // initialize the GLFW window
    GameWindow_t *window = window_create(API_VULKAN, 1600, 900);
    glfwMakeContextCurrent(window->m_window);

    // initialize vulkan instance and required extensions for GLFW
    VulkanInstance_t vk_instance;
    if (!vulkan_initialize(window, &vk_instance)) {
        return -1;
    }

    while (!glfwWindowShouldClose(window->m_window)) {
        glfwSwapBuffers(window->m_window);
        glfwPollEvents();
    }

    // cleanup vulkan context and GLFW window
    game_cleanup(&vk_instance, window);

    return 0;
}
