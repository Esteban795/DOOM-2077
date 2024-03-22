
#include <stdio.h>
#include <stdlib.h>
#include "../include/bullet.h"

#define player_hitbox_size 6
#define hitscan_precision 0.1

bullet* create_bullet(player **players,int num_player){
    bullet *bullet_ = malloc(sizeof(bullet));
    bullet_->posx = players[num_player]->pos.x;
    bullet_->posy = players[num_player]->pos.y;
    bullet_->angle = players[num_player]->angle;
    return bullet_;
}

double distance(double posx_a,double posy_a,double posx_b,double posy_b){
    return sqrt(pow((posx_b-posx_a),2)+pow((posy_b-posy_a),2));
}





void fire_bullet(player** players,int num_players,int num_player,int damage){ //pour l'instant ne detecte que les joueurs, pas les murs
    int j=0;
    int has_hit=0;
    bullet* bullet_=create_bullet(players,num_player);
    while (j<1000&&has_hit==0){ //tant qu'une collison avec un mur n'est pas detectée(a implémenter)
        int i=0;
        for(i=0;i<num_players;i++){    
            if(i!=num_player){
                if (distance(bullet_->posx,bullet_->posy,players[i]->pos.x,players[i]->pos.y)<player_hitbox_size){//radius de distance nécéssaire pour toucher
                        printf("g touche le joueur %i\n",i);
                        players[i]->life=players[i]->life-damage;
                has_hit=1;}
            }
        }
        bullet_->posx=bullet_->posx+cos(bullet_->angle)*hitscan_precision;
        bullet_->posy=bullet_->posy+sin(bullet_->angle)*hitscan_precision;
        j++;
        //printf("%lf,%lf, \n", bullet->posx,bullet->posy,bullet->angle);
    }
    if(has_hit==0){
        printf("pas touché cheh");
    }

}
