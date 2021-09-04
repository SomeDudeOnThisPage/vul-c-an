#include "game.h"

#include "platform/window.h"
#include "platform/vulkan.h"

int main() {
    GameWindow_t *window = window_create(API_VULKAN, 1600, 900);

    glfwMakeContextCurrent(window->m_window);

    VulkanInstance_t vk_instance;
    vulkan_initialize(window, &vk_instance);

    while (!glfwWindowShouldClose(window->m_window)) {
        glfwSwapBuffers(window->m_window);
        glfwPollEvents();
    }

    vulkan_destroy(&vk_instance);
    window_destroy(window);

    return 0;
}
