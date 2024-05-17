
#include "../include/hitscan.h"
#include "../include/player.h"
#include "../include/component/position.h"
#include "../include/component/weapon.h"

#define PLAYER_HITBOX_SIZE 50
#define HITSCAN_PRECISION 10


bullet *create_bullet(player *player_) {
  bullet *bullet_ = malloc(sizeof(bullet));
  position_ct *pos = player_get_position(player_);
  bullet_->posx = position_get_x(pos);
  bullet_->posy = position_get_y(pos);
  bullet_->angle = position_get_angle(pos);
  return bullet_;
}

void fire_bullet(entity_t** players, int num_players, player *player_,
                 int damage) { // toutes les valeurs de y sont négatives

  position_ct *pos = player_get_position(player_);
  weapon_ct *weapon = player_get_weapon(player_);
  
  double distance_finale = 10000;
  if (weapon_get_active_cooldown(weapon) < 80) {
    *weapon_get_mut_active_cooldown(weapon) += 100;
    linedef **linedefs = player_->engine->wData->linedefs;
    double x1 = position_get_x(pos);
    double y1 = -position_get_y(pos);
    double x2 = x1 + 100 * cos(deg_to_rad(position_get_angle(pos)));
    double y2 = y1 + 100 * sin(deg_to_rad(position_get_angle(pos)));
    double a = 0;
    double x_final = 0;
    double y_final = 0;
    int direction =
        1; // 0 correspond a ni droite ni auche 1 a gauche 2 a droite
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
    for (int j = 0; j < num_players; j++) {
      position_ct *pos_pj = (position_ct*)world_get_component(player_->engine->world, players[j], COMPONENT_TAG_POSITION);
      health_ct *health_pj = (health_ct*)world_get_component(player_->engine->world, players[j], COMPONENT_TAG_HEALTH);

      double dist_to_hitscan =
          (fabs(a * (position_get_x(pos_pj)) + (position_get_y(pos_pj)) + b)) /
          (sqrt(pow(a, 2) + pow(-1, 2)));
      if (dist_to_hitscan < PLAYER_HITBOX_SIZE) {
        if ((min(x1, x_final) < position_get_x(pos_pj)) &&
            (max(x1, x_final) > position_get_x(pos_pj)) &&
            (min(y1, y_final) < -position_get_y(pos_pj)) &&
            (min(y1, y_final) < -position_get_y(pos_pj))) {
          health_sub(health_pj, damage);
        }
      }
    }
  }
}

// int correct_height(linedef *wall, int height) {
//   if (!(wall->has_back_sidedef)) {
//     return 0;
//   } else {
//     int ceil_height_1 = wall->back_sidedef->sector->ceiling_height;
//     int floor_height_1 = wall->back_sidedef->sector->floor_height;
//     int ceil_height_2 = wall->front_sidedef->sector->ceiling_height;
//     int floor_height_2 = wall->front_sidedef->sector->floor_height;
//     if ((min(ceil_height_1, ceil_height_2) < height) &&
//         (max(ceil_height_1, ceil_height_2) > height)) {
//       return 1;
//     } else {
//       if ((min(floor_height_1, floor_height_2) < height) &&
//           (max(floor_height_1, floor_height_2) > height)) {
//         return 1;
//       } else {
//         return 0;
//       }
//     }
//   }
// }

// linedef *cast_ray(linedef** linedefs, int len_linedefs,vec2 player_pos, double player_angle,double height) {
//   double distance_finale = 100000;
//   linedef *target_linedef = NULL;
//   double x1 = player_pos.x;
//   double y1 = -player_pos.y;
//   double x2 = x1 + 100 * cos(deg_to_rad((player_angle)));
//   double y2 = y1 + 100 * sin(deg_to_rad((player_angle)));
//   double a = 0;
//   double x_final = 0;
//   double y_final = 0;
//   int direction = 1; // 0 correspond a ni droite ni auche 1 a gauche 2 a droite
//   if (x1 != x2) {
//     a = (y2 - y1) / (x2 - x1);
//     if (x1 > x2) {
//       direction = 1; // vers la gauche
//     } else {
//       direction = 2; // vers la droite
//     }
//   }
//   double b = y1 - a * x1;
//   for (int i = 0; i < len_linedefs; i++) {
//     double x = 0;
//     double y = 0;
//     if ((!(linedefs[i]->has_back_sidedef)) ||
//         (correct_height(linedefs[i], height))) {
//       double x1a = linedefs[i]->start_vertex->x;
//       double y1a = -linedefs[i]->start_vertex->y;
//       double x2a = linedefs[i]->end_vertex->x;
//       double y2a = -linedefs[i]->end_vertex->y;
//       double c = 0;
//       double d = 0;
//       if (x1a != x2a) {
//         c = (y2a - y1a) / (x2a - x1a);
//         d = y1a - c * x1a;
//         x = (d - b) / (a - c);
//         y = a * x + b;
//         if (distance(x1, y1, x, y) < 100) {
//         }
//         if (((x1a <= x) && (x <= x2a)) || ((x2a <= x) && (x <= x1a))) {
//           if (((direction == 1) && (x1 > x)) ||
//               ((direction == 2) && (x1 < x))) {
//             if (distance(x1, y1, x, y) < distance_finale) {
//               distance_finale = distance(x1, y1, x, y);
//               if (distance_finale < TRIGGER_ACTIVATION_RANGE) {
//                 target_linedef = linedefs[i];
//                 x_final = x;
//                 y_final = y;
//               }
//             }
//           }
//         }
//       } else {
//         x = x1a;
//         y = a * x + b;
//         if (((y1a <= y) && (y <= y2a)) || ((y2a <= y) && (y <= y1a))) {
//           if (((direction == 1) && (x1 >= x)) ||
//               ((direction == 2) && (x1 <= x))) {
//             if (distance(x1, y1, x, y) < distance_finale) {
//               distance_finale = distance(x1, y1, x, y);
//               if (distance_finale < TRIGGER_ACTIVATION_RANGE) {
//                 target_linedef = linedefs[i];
//                 x_final = x;
//                 y_final = y;
//               }
//             }
//           }
//         }
//       }
//     }
//   }
//   return target_linedef;
// }