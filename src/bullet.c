
#include <stdio.h>
#include <stdlib.h>
#include "../include/bullet.h"

#define player_hitbox_size 150
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
                        printf("g touche le joueur %i\n longueur de trajet %i\n",i,j);
                        players[i]->life=players[i]->life-damage;
                has_hit=1;}
        }
        bullet_->posx=bullet_->posx+cos(bullet_->angle)*hitscan_precision;
        bullet_->posy=bullet_->posy+sin(bullet_->angle)*hitscan_precision;
        xb=from_coords_to_collision_map(bullet_->posx);
        yb=from_coords_to_collision_map(bullet_->posy);
        printf("%i  %i\n",xb,yb);
        if(abs(xb)>999||abs(yb)>999||xb<1||yb<1){
            has_hit=2;
        }
        j++;
        printf("%lf,%lf, \n", bullet_->posx,bullet_->posy);
    }
    if(has_hit==0){
        printf("Pas touché, longueur de trajet %i\n",j);
    }
    if(has_hit==2){
        printf("balle enfuie !longueur de trajet %i\n",j);
    }
    player_->cooldown=player_->cooldown+100;
    free(bullet_);
    }
    else{
        //printf("je ne peux pas tirer\n");
    }
}

void fire_bullet2(player** players,int num_players,player* player_,int damage){
    int touche=0;
    if(player_->cooldown<50){
        player_->cooldown=player_->cooldown+100;
        linedef* linedefs=player_->engine->wData->linedefs;
        double x1=player_->pos.x;
        double y1=player_->pos.y;
        double x2=x1+100*cos(deg_to_rad(player_->angle));
        double y2=y1+100*sin(deg_to_rad(player_->angle));
        double a =0;
        if(x1!=x2){
            a=(y2-y1)/(x2-x1);
        }
        double b=y1-a*x1;
        //printf("%f %f %f %f ,%f \n",x1,x2,y1,y2,pente);
        for(int i=0;i<475;i++){
            double x=0;
            double y=0;
            if(!(linedefs[i].has_back_sidedef)){
                double x1a=linedefs[i].start_vertex->x;
                double y1a=linedefs[i].start_vertex->y;
                double x2a=linedefs[i].end_vertex->x;
                double y2a=linedefs[i].end_vertex->y;
                double c =0;
                double d=0;
                if(x1a!=x2a){
                    c=(y2a-y1a)/(x1a-x2a);
                    d=y1a-c*x1a;
                    x=(d-b)/(a-c);
                    y=a*x+b;
                    if(((x1a<=x)&&(x<=x2a))||((x2a<=x)&&(x<=x1a))){
                        touche=2;
                        printf("2\n");
                    }

                }
                else{
                    x=x1a;
                    y=a*x+b;
                    if(((y1a<=y)&&(y<=y2a))||((y2a<=y)&&(y<=y1a))){
                        touche=1;
                        printf("1\n");
                    }
                }
            }
        }
    if(touche==0){
        printf("rate\n");
    }
    if(touche==1){
        printf("touche 1\n");
    }
    if(touche==2){
        printf("touche 2\n");
    }    
    }
}
