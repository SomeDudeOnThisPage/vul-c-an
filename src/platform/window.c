#include "window.h"

GameWindow_t* window_create(uint32_t client_api, uint16_t size_x, uint16_t size_y) {
    if (!glfwInit() || !glfwVulkanSupported()) {
        printf("Failed to initialize GLFW");
        return NULL;
    }

    glfwWindowHint(GLFW_CLIENT_API, (int) client_api);

    GameWindow_t *window = malloc(sizeof(GameWindow_t));
    window->m_window = glfwCreateWindow(size_x, size_y, "Vulkan babee", NULL, NULL);
    if (!window->m_window) {
        printf("Failed to initialize GLFW Window");
        glfwTerminate();
        return NULL;
    }

    return window;
}

/**
 * Destroys the game window, and the GLFW window therein.
 * @param window
 */
void window_destroy(GameWindow_t *window) {
    glfwDestroyWindow(window->m_window);
    glfwTerminate();
    free(window);
}


