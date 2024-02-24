#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

struct PlayerSettings {
    SDL_Keycode MOVE_FORWARD;
    SDL_Keycode MOVE_BACKWARD;
    SDL_Keycode MOVE_LEFT;
    SDL_Keycode MOVE_RIGHT;
    SDL_Keycode ATTACK;
    SDL_Keycode INTERACT;
    double MOUSE_SENS;
    bool INVERT_MOUSE;
    double FOV;
};

typedef struct PlayerSettings player_settings;
#endif