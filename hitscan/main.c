#include <stdio.h>
#include <stdlib.h>
#include "bullet.h"
#include <math.h>

int main(void){
    player** players=create_players(2);
    create_player(0.0,0.0,0,players);
    create_player(10.0,10.0,1,players);
    fire_bullet(players,2,0,100);
}


