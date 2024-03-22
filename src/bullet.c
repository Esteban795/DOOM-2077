
#include <stdio.h>
#include <stdlib.h>
#include "../include/bullet.h"

#define player_hitbox_size 6
#define hitscan_precision 10

bullet* create_bullet(player *player_){
    bullet *bullet_ = malloc(sizeof(bullet));
    bullet_->posx = player_->pos.x;
    bullet_->posy = player_->pos.y;
    bullet_->angle = player_->angle;
    return bullet_;
}

double distance(double posx_a,double posy_a,double posx_b,double posy_b){
    return sqrt(pow((posx_b-posx_a),2)+pow((posy_b-posy_a),2));
}



void fire_bullet(player** players,int num_players,player* player_,int damage,int** collision_map){ //pour l'instant ne detecte que les joueurs, pas les murs
    if(player_->cooldown<50){
    int j=0;
    int has_hit=0;
    bullet* bullet_=create_bullet(player_);
    int xb=from_coords_to_collision_map(bullet_->posx);
    int yb=from_coords_to_collision_map(bullet_->posy);
    while (collision_map[xb][yb]==0&&has_hit==0){ //tant qu'une collison avec un mur n'est pas detectée(a implémenter)
        int i=0;
        for(i=0;i<num_players;i++){    
                if (distance(bullet_->posx,bullet_->posy,players[i]->pos.x,players[i]->pos.y)<player_hitbox_size){//radius de distance nécéssaire pour toucher
                        printf("g touche le joueur %i\n",i);
                        players[i]->life=players[i]->life-damage;
                has_hit=1;}
        }
        bullet_->posx=bullet_->posx+cos(bullet_->angle)*hitscan_precision;
        bullet_->posy=bullet_->posy+sin(bullet_->angle)*hitscan_precision;
        xb=from_coords_to_collision_map(bullet_->posx);
        yb=from_coords_to_collision_map(bullet_->posy);
        j++;
        printf("%lf,%lf, \n", bullet_->posx,bullet_->posy);
    }
    if(has_hit==0){
        printf("pas touché cheh");
    }
    player_->cooldown=player_->cooldown+100;
    free(bullet_);
    }
    else{
        printf("je ne peux pas tirer");
    }
}
