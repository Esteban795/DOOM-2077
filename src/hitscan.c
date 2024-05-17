
#include "../include/hitscan.h"

#define PLAYER_HITBOX_SIZE 50
#define HITSCAN_PRECISION 10
#define MELEE_RADIUS 20
#define MAX_SPRAY  10

bullet *create_bullet(player *player_) {
  bullet *bullet_ = malloc(sizeof(bullet));
  bullet_->posx = player_->pos.x;
  bullet_->posy = player_->pos.y;
  bullet_->angle = player_->angle;
  return bullet_;
}

double distance(double posx_a, double posy_a, double posx_b, double posy_b) {
  return sqrt(pow((posx_b - posx_a), 2) + pow((posy_b - posy_a), 2));
}

void fire_bullet(player **players, int num_players, player *player_,weapons_array* weapons_list) { // toutes les valeurs de y sont négatives
  //initialisation
  double distance_finale = 10000;
  printf("%i\n",player_->cooldown);
  weapon* weapon_used=weapons_list->weapons[player_->active_weapon];
  int damage=weapon_used->max_damage;
  int is_ranged; //0 correspond a une arme de melée sinon une arme a distance
  if(weapon_used->type==-1){
    is_ranged=0;
  }
  else{
    is_ranged=1;
  }
  //gestion du spray
  if(is_ranged==1){
    if(player_->spray%2==0){
      player_->angle+=player_->spray;
    }
    else{
      player_->angle-=player_->spray; //le spray fait bouger la caméra
    }
  }
  //gestion du tir
  if ((player_->cooldown ==0)&&(!((is_ranged==1)&&(player_->ammo[player_->active_weapon]==0)))){ //On véfrifie d'un coté que le temps de cooldown est respecté et ensuite que si l'arme est a distance elle dispose d'assez de muntitions
    linedef *linedefs = player_->engine->wData->linedefs;
    double x1 = player_->pos.x;
    double y1 = -player_->pos.y;
    double x2 = x1 + 100 * cos(deg_to_rad((player_->angle)));
    double y2 = y1 + 100 * sin(deg_to_rad((player_->angle)));
    double a = 0;
    double x_final = 0;
    double y_final = 0;
    int direction =
        1; // 0 correspond a ni droite ni auche 1 a gauche 2 a droite
    double height = player_->height;
    if (x1 != x2) {
      a = (y2 - y1) / (x2 - x1);
      if (x1 > x2) {
        direction = 1; // vers la gauche
      } else {
        direction = 2; // vers la droite
      }
    }
    double b = y1 - a * x1;
    for (int i = 0; i < player_->engine->wData->len_linedefs; i++) {
      double x = 0;
      double y = 0;
      if ((!(linedefs[i].has_back_sidedef)) ||
          (correct_height(linedefs[i], height))) {
        double x1a = linedefs[i].start_vertex->x;
        double y1a = -linedefs[i].start_vertex->y;
        double x2a = linedefs[i].end_vertex->x;
        double y2a = -linedefs[i].end_vertex->y;
        double c = 0;
        double d = 0;
        if (x1a != x2a) {
          c = (y2a - y1a) / (x2a - x1a);
          d = y1a - c * x1a;
          x = (d - b) / (a - c);
          y = a * x + b;
          if (distance(x1, y1, x, y) < 100) {
          }
          if (((x1a <= x) && (x <= x2a)) || ((x2a <= x) && (x <= x1a))) {
            if (((direction == 1) && (x1 > x)) ||
                ((direction == 2) && (x1 < x))) {
              if (distance(x1, y1, x, y) < distance_finale) {
                distance_finale = distance(x1, y1, x, y);
                x_final = x;
                y_final = y;
              }
            }
          }

        } else {
          x = x1a;
          y = a * x + b;
          if (((y1a <= y) && (y <= y2a)) || ((y2a <= y) && (y <= y1a))) {
            if (((direction == 1) && (x1 >= x)) ||
                ((direction == 2) && (x1 <= x))) {
              if (distance(x1, y1, x, y) < distance_finale) {
                distance_finale = distance(x1, y1, x, y);
                x_final = x;
                y_final = y;
              }
            }
          }
        }
      }
    }
    for (int j = 0; j < num_players; j++) {
      double dist_to_hitscan =
          (fabs(a * (players[j]->pos.x) + (players[j]->pos.y) + b)) /
          (sqrt(pow(a, 2) + pow(-1, 2)));
      if (dist_to_hitscan < PLAYER_HITBOX_SIZE) {
        if ((min(x1, x_final) < players[j]->pos.x) &&
            (max(x1, x_final) > players[j]->pos.x) &&
            (min(y1, y_final) < -players[j]->pos.y) &&
            (min(y1, y_final) < -players[j]->pos.y)) {
          if((dist_to_hitscan< MELEE_RADIUS)){
            players[j]->life -= damage;
            player_->ammo[player_->active_weapon] -=1;
          }
        }
      }
    }
    player_->cooldown+=1000/(weapon_used->fire_rate);
    printf("%f\n",1000/(weapon_used->fire_rate)); //a update plus tard quand une unité de temps sera fixée
    if(player_->spray<MAX_SPRAY*weapon_used->spray){ 
    player_->spray+=weapon_used->spray;
    }
  }
}

int correct_height(linedef wall, int height) {
  if (!(wall.has_back_sidedef)) {
    return 0;
  } else {
    int ceil_height_1 = wall.back_sidedef->sector->ceiling_height;
    int floor_height_1 = wall.back_sidedef->sector->floor_height;
    int ceil_height_2 = wall.front_sidedef->sector->ceiling_height;
    int floor_height_2 = wall.front_sidedef->sector->floor_height;
    if ((min(ceil_height_1, ceil_height_2) < height) &&
        (max(ceil_height_1, ceil_height_2) > height)) {
      return 1;
    } else {
      if ((min(floor_height_1, floor_height_2) < height) &&
          (max(floor_height_1, floor_height_2) > height)) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}
