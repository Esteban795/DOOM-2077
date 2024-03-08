#include "../include/game_states.h"



void handle_menu_state(GameStateArgs *args){
    //faire les trucs du menu
}

void handle_ingame_state(GameStateArgs *args) {
    if(args->isFirstTime[STATE_INGAME]){
        args->e = init_engine("maps/DOOM1.WAD", args->renderer, args->numkeys, args->keys);
        args->isFirstTime[STATE_INGAME] = false;
    }
    int res = update_engine(args->e);
    if (res == 1){
        args->isRunning = false;
    }
}

void handle_paused_state(GameStateArgs *args){
    // éventuellement faire les trucs de la pause, je sais plus si on gère ça en état ou pas
}

void handle_gameover_state(GameStateArgs *args){
    // faire les trucs pour quand c'est finito pipo
}

GameStateFunction game_states[] = {
    handle_menu_state,
    handle_ingame_state,
    handle_paused_state,
    handle_gameover_state
};