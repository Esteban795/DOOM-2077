
#include "../include/hitscan.h"
#include "../include/component/position.h"
#include "../include/component/weapon.h"
#include "../include/player.h"
#include <stdio.h>

#define PLAYER_HITBOX_SIZE 50
#define HITSCAN_PRECISION 10
#define MELEE_RADIUS 20
#define MAX_SPRAY 50
#define SPRAY_COEFF 5 // Vitesse a laquelle le spray augmente

bullet *create_bullet(player *player_) {
  bullet *bullet_ = malloc(sizeof(bullet));
  position_ct *pos = player_get_position(player_);
  bullet_->posx = position_get_x(pos);
  bullet_->posy = position_get_y(pos);
  bullet_->angle = position_get_angle(pos);
  return bullet_;
}

void fire_bullet(
    entity_t **players, int num_players, player *player_,
    weapons_array *weapons_list) { // toutes les valeurs de y sont négatives
  position_ct *pos = player_get_position(player_);
  weapon_ct *weapon_ecs = player_get_weapon(player_);
  double distance_finale = 10000;
  if (weapon_get_active_cooldown(weapon_ecs) < 80) {
    *weapon_get_mut_active_cooldown(weapon_ecs) += 100;
    weapon *weapon_used = weapons_list->weapons[weapon_ecs->active_weapon];
    int damage = weapon_used->max_damage;
    int weapon_number = weapon_used->id;
    double cs =
        player_->cooldowns_sprays
            ->cs[weapon_number]; // temps depuis le dernier tir de cette arme
    double time = SDL_GetTicks();
    int is_ranged; // 0 correspond a une arme de melée sinon une arme a distance
    if (weapon_used->type == -1) {
      is_ranged = 0;
    } else {
      is_ranged = 1;
    }
    // gestion du spray
    if (is_ranged == 1) {
      int spray = 0;
      if (spray % 2 == 0) {
        pos->angle += spray;
      } else {
        pos->angle -= spray; // le spray fait bouger la caméra
      }
    }
    // gestion du tir jusqu'au mur
    if ((cs < time) &&
        (!((is_ranged == 1) &&
           (player_->ammo[weapon_ecs->active_weapon] ==
            0)))) { // On véfrifie d'un coté que le temps de cooldown est
                    // respecté et ensuite que si l'arme est a distance elle
                    // dispose d'assez de muntitions
      linedef **linedefs = player_->engine->wData->linedefs;
      double x1 = position_get_x(pos);
      double y1 = -position_get_y(pos);
      double x2 = x1 + 100 * cos(deg_to_rad(position_get_angle(pos)));
      double y2 = y1 + 100 * sin(deg_to_rad(position_get_angle(pos)));
      double a = 0;
      double x_final = 0;
      double y_final = 0;
      int direction = 1; // 0 = ni gauche ni droite, 1 = gauche, 2 = droite
      double height = position_get_z(pos);
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
        if ((!(linedefs[i]->has_back_sidedef)) ||
            (correct_height(linedefs[i], height))) {
          double x1a = linedefs[i]->start_vertex->x;
          double y1a = -linedefs[i]->start_vertex->y;
          double x2a = linedefs[i]->end_vertex->x;
          double y2a = -linedefs[i]->end_vertex->y;
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

      // TODO: Déplacer coté serveur le calcul des dégats sur un joueur.
      // Gestion de la collison du tir avec les joueurs
      for (int j = 0; j < num_players; j++) {
        if (players[j] == NULL) continue;
        position_ct *pos_pj = (position_ct *)world_get_component(
            player_->engine->world, players[j], COMPONENT_TAG_POSITION);
    
        health_ct *health_pj = (health_ct *)world_get_component(
            player_->engine->world, players[j], COMPONENT_TAG_HEALTH);

        double dist_to_hitscan = (fabs(a * (position_get_x(pos_pj)) +
                                       (position_get_y(pos_pj)) + b)) /
                                 (sqrt(pow(a, 2) + pow(-1, 2)));
        if (dist_to_hitscan < PLAYER_HITBOX_SIZE) {
          if ((min(x1, x_final) < position_get_x(pos_pj)) &&
              (max(x1, x_final) > position_get_x(pos_pj)) &&
              (min(y1, y_final) < -position_get_y(pos_pj)) &&
              (min(y1, y_final) < -position_get_y(pos_pj))) {
            if ((dist_to_hitscan < MELEE_RADIUS)) {
              health_sub(health_pj, damage);
              player_->ammo[weapon_ecs->ammunitions[weapon_ecs->active_weapon]] -= 1;
            }
          }
        }
      }
      // Update des etats du joueur
      player_->cooldowns_sprays->cs[weapon_number] = time;
      //  printf("final x: %f, final y: %f\n", x_final, y_final);
    }
    
  }
 
}

void aux_fire_bullet(engine *e, weapons_array *wa) {
  player *p = e->p;
  player_keybind *kb = p->keybinds;
  bool attack = keys[get_key_from_action(kb, "ATTACK")];
  if (attack) {
    fire_bullet(e->players, NUM_PLAYERS, p, wa);
  }
}