#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include <math.h>
#define rotation_speed 1
#define moving_speed 1


void create_player(double posx, double posy, int num_player, player** Players){
    Players[num_player]=malloc(sizeof(player));
    Players[num_player] -> life = 100;
    Players[num_player] -> posx = posx;
    Players[num_player] -> posy = posy;
    Players[num_player] -> angle = 0.7;
}

player** create_players(int num_players){
    player** Players = malloc(sizeof(player*)*num_players);
    return(Players);
}

void rotate_player(player** players,int num_player){
    players[num_player]->angle=players[num_player]->angle+1*rotation_speed;
}

void move_player_forward(player** players,int num_player){
    players[num_player]->posx=players[num_player]->posx+cos(players[num_player]->angle)*moving_speed;
    players[num_player]->posy=players[num_player]->posy+sin(players[num_player]->angle)*moving_speed;
}

