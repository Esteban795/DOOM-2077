#include "../include/keybindings.h"
#include <SDL2/SDL_log.h>
#include <stdio.h>

void read_action(FILE* f){
    size_t line_len;
    char* line;
    size_t char_read = getline(&line, &line_len, f);
    if (char_read == -1){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error reading line from config file");
        exit(1);
    }
}
player_settings get_player_settings(){
    player_settings settings;
    FILE* f = fopen("settings.cfg", "r");
    return settings;
}