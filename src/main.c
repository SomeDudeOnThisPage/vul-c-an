#include "game.h"

uint8_t main() {
    // initialize the GLFW window
    GameWindow_t *window = window_create(API_VULKAN, 1600, 900);
    glfwMakeContextCurrent(window->m_window);

    // initialize vulkan instance and required extensions for GLFW
    VulkanInstance_t vk_instance;
    if (!vulkan_initialize(window, &vk_instance)) {
        return -1;
    }

    Game_t game;
    game.window = window;
    game.vulkan_instance = &vk_instance;

    while (!glfwWindowShouldClose(window->m_window)) {
        glfwSwapBuffers(window->m_window);
        glfwPollEvents();
    }

    game_destroy(&game);

    return 0;
}
