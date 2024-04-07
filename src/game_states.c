#include "../include/game_states.h"

bool firstTimeLaunching = true;

void handle_menu_state(engine* e){
    //faire les trucs du menu
}

void handle_ingame_state(engine* e) {
    update_player(e->p);
    get_ssector_height(e->bsp);
    segment_handler_update(e->seg_handler);
    update_bsp(e->bsp);
    return;
}

void handle_paused_state(engine* e){
    // éventuellement faire les trucs de la pause, je sais plus si on gère ça en état ou pas
}

void handle_gameover_state(engine* e){
    // faire les trucs pour quand c'est finito pipo
}

void handle_settings_state(engine* e){
    // faire les trucs pour les settings
}

GameStateFunction game_states[] = {
    handle_menu_state,
    handle_ingame_state,
    handle_paused_state,
    handle_gameover_state,
    handle_settings_state
};