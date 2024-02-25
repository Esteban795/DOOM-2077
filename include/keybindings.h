#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>


struct PlayerKeybind {
    char* name;
    SDL_Keycode key;
    struct PlayerKeybind* next;
};

typedef struct PlayerKeybind player_keybind;

struct PlayerSetting {
    char* name;
    double value;
    struct PlayerSetting* next;
};

typedef struct PlayerSetting player_setting;

enum param_type {
    PlayerKeybind,
    PlayerSetting
};

#endif