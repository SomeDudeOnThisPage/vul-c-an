//
// Created by robin on 05.09.2021.
//
#include "game.h"

void game_destroy(Game_t *game) {
    vulkan_destroy(game->vulkan_instance);
    window_destroy(game->window);
}