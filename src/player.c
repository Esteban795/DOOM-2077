#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/component/display_name.h"
#include "../include/component/health.h"
#include "../include/component/position.h"
#include "../include/component/weapon.h"
#include "../include/ecs/archetype.h"
#include "../include/ecs/component.h"
#include "../include/ecs/entity.h"
#include "../include/ecs/world.h"
#include "../include/player.h"
#include "../include/settings.h"

#define SIGN(x) (int)(x > 0) ? 1 : ((x < 0) ? -1 : 0)
#define DOT(a, b) (a.x * b.x) + (a.y * b.y)
#define DISTANCE_VEC_VER(a, b) (sqrt(pow(b->x - a.x, 2) + pow(b->y - a.y, 2)))
#define TRIGGER_ACTIVATION_RANGE 100

#define M_PI 3.14159265358979323846
#define MAX_INTERACT_COOLDOWN 100
bool SHOULD_COLLIDE = true;
int INTERACT_CD = 0;

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  int ammo[WEAPONS_NUMBER];
  ammo[0] = -2;
  for (int i = 1; i < WEAPONS_NUMBER; i++) {
    ammo[i] = -1;
  }
  p->engine = e;
  p->thing = e->wData->things[0];
  p->keybinds = get_player_keybinds(KEYBINDS_FILE);
  p->settings = get_player_settings(SETTINGS_FILE);

  // Add player to the ECS world
  double coords[3] = {p->thing.x, p->thing.y, PLAYER_HEIGHT};
  component_t **comps = malloc(sizeof(component_t *) * 4);
  comps[0] = position_create(coords, p->thing.angle + 180.0);
  comps[1] = health_create(100.0, 100.0);
  comps[2] = weapon_create(ammo);
  comps[3] = display_name_create(PLAYER_USERNAME);
  p->entity = world_insert_entity(e->world, e->remote->player_id, comps, 4);
  free(comps);

  return p;
}

linedef **get_linedefs_in_active_blocks(player *p, int *nlinedefs) {
  blockmap *bmap = p->engine->wData->blockmap;
  position_ct *pos = player_get_position(p);

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int block_index = blockmap_get_block_index(
          bmap, position_get_x(pos) + x * 128, position_get_y(pos) + y * 128);
      if (block_index < 0 || block_index >= (int)bmap->nblocks)
        continue;
      (*nlinedefs) += bmap->blocks[block_index].nlinedefs;
    }
  }

  linedef **linedefs = malloc(sizeof(linedef *) * (*nlinedefs));
  int offset = 0;

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int block_index = blockmap_get_block_index(
          bmap, position_get_x(pos) + x * 128, position_get_y(pos) + y * 128);
      if (block_index < 0 || block_index >= (int)bmap->nblocks)
        continue;
      for (int i = 0; i < (int)bmap->blocks[block_index].nlinedefs; i++) {
        linedefs[offset] = bmap->blocks[block_index].linedefs[i];
        offset++;
      }
    }
  }

  return linedefs;
}

double cross_pos_linedef(linedef *line, vec2 pos) {
  vertex *v1 = line->start_vertex;
  vertex *v2 = line->end_vertex;
  vec2 a = {.x = v2->x - v1->x, .y = v2->y - v1->y};
  vec2 b = {.x = pos.x - v1->x, .y = pos.y - v1->y};
  return (a.y * b.x) - (a.x * b.y);
}

double dot_pos_linedef(linedef *line, vec2 pos) {
  vertex *v1 = line->start_vertex;
  vertex *v2 = line->end_vertex;
  vec2 a = {.x = v2->x - v1->x, .y = v2->y - v1->y};
  vec2 b = {.x = pos.x - v1->x, .y = pos.y - v1->y};
  return DOT(a, b);
}

double get_wall_length(linedef *line) {
  vertex *v1 = line->start_vertex;
  vertex *v2 = line->end_vertex;
  vec2 a = {.x = v2->x - v1->x, .y = v2->y - v1->y};
  return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

void get_projections(linedef *line, vec2 pos, vec2 *projected,
                     vec2 *projected_hitbox, vec2 *projected_hitbox_back) {
  vertex *v1 = line->start_vertex;
  vertex *v2 = line->end_vertex;

  vec2 l = {.x = (double)v2->x - (double)v1->x,
            .y = (double)v2->y - (double)v1->y};
  vec2 pmv1 = {.x = pos.x - (double)v1->x, .y = pos.y - (double)v1->y};

  double projection_factor = (DOT(pmv1, l)) / (DOT(l, l));
  projected->x = (double)v1->x + projection_factor * l.x;
  projected->y = (double)v1->y + projection_factor * l.y;

  // INFO: this is an artifact of old code. debating whether i need to remove
  // this or not
  if (!projected_hitbox) {
    return;
  }
  double norm_projection =
      sqrt(pow((projected->x - pos.x), 2) + pow((projected->y - pos.y), 2));

  projected_hitbox->x =
      pos.x + ((projected->x - pos.x) / norm_projection) * PLAYER_RADIUS * 2;
  projected_hitbox->y =
      pos.y + ((projected->y - pos.y) / norm_projection) * PLAYER_RADIUS * 2;

  projected_hitbox_back->x =
      pos.x - ((projected->x - pos.x) / norm_projection) * PLAYER_RADIUS * 2;
  projected_hitbox_back->y =
      pos.y - ((projected->y - pos.y) / norm_projection) * PLAYER_RADIUS * 2;
}

void slide_against_wall(vec2 *pos_inside_wall, vec2 projected) {
  double norm = sqrt(pow((projected.x - pos_inside_wall->x), 2) +
                     pow((projected.y - pos_inside_wall->y), 2));
  pos_inside_wall->x =
      projected.x -
      ((projected.x - pos_inside_wall->x) / norm) * (PLAYER_RADIUS * 2 + 0.1);
  pos_inside_wall->y =
      projected.y -
      ((projected.y - pos_inside_wall->y) / norm) * (PLAYER_RADIUS * 2 + 0.1);
}

void slide_against_point(vertex point, vec2 *post_move) {
  vec2 direction = {.x = post_move->x - point.x, .y = post_move->y - point.y};
  float l = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
  direction.x *= PLAYER_RADIUS / l;
  direction.y *= PLAYER_RADIUS / l;

  post_move->x += direction.x;
  post_move->y += direction.y;
}

bool can_collide_with_wall(double cp_after, linedef *linedef) {
  if (linedef->has_back_sidedef) {
    // we are handling a two-sided linedef, so most probably a portal, or an
    // epic awesome sauce fail
    sector *from;
    sector *to;
    if (cp_after > 0) {
      from = linedef->back_sidedef->sector;
      to = linedef->front_sidedef->sector;
    } else {
      from = linedef->front_sidedef->sector;
      to = linedef->back_sidedef->sector;
    }
    if (to->floor_height - from->floor_height > PLAYER_STEP) {
      return true;
    }
    if (to->ceiling_height - to->floor_height < PLAYER_HEIGHT) {
      return true;
    }
  } else {
    return !(cp_after > 0);
    // HACK: this assumes that all walls are facing inwards!
    // if the wad has a wall facing outwards it will break!
  }
  return false;
}

void move_and_slide(player *p, double *velocity) {
  position_ct *pos = player_get_position(p);

  int nlinedefs = 0;
  linedef **linedefs = get_linedefs_in_active_blocks(p, &nlinedefs);
  vec2 next_pos = {.x = position_get_x(pos) + velocity[0],
                   .y = position_get_y(pos) + velocity[1]};
  for (int ii = 0; ii < 2 * nlinedefs; ii++) {

    int i = ii % nlinedefs;
    vec2 p_a;   // projection after moving
    vec2 phf_a; // projection of the hitbox after moving, front
    vec2 phb_a; // projection of the hitbox after moving, back

    // check if the player can actually collide with the wall in directions
    // parallel to the wall
    double d = dot_pos_linedef(linedefs[i], position_get_pos(pos));
    if (d < 0 || d > pow(get_wall_length(linedefs[i]), 2)) {
      vertex *linedef_a = linedefs[i]->start_vertex;
      vertex *linedef_b = linedefs[i]->end_vertex;

      if (DISTANCE_VEC_VER(next_pos, linedef_a) < PLAYER_RADIUS) {
        slide_against_point(*linedef_a, &next_pos);
      } else if (DISTANCE_VEC_VER(next_pos, linedef_b) < PLAYER_RADIUS) {
        slide_against_point(*linedef_b, &next_pos);
      }

      continue;
    }
    get_projections(linedefs[i], next_pos, &p_a, &phf_a, &phb_a);

    double cp_hb = cross_pos_linedef(linedefs[i], phb_a);
    double cp_hf = cross_pos_linedef(linedefs[i], phf_a);

    if ((SIGN(cp_hb)) != (SIGN(cp_hf))) {

      // collision happened
      // if cp_after < 0: use the second sidedef
      // else: use the first linedef (first linedef faces "clockwise")
      if (linedefs[i]->has_doors && linedefs[i]->is_collidable) {
        if (!linedefs[i]->used) {
          client_door_trigger(p->engine, linedefs[i]->door->id);
          if (!linedefs[i]->is_repeatable) {
            linedefs[i]->used = true;
          }
        }
      }
      if (linedefs[i]->has_lifts && linedefs[i]->is_collidable) {
        if (!linedefs[i]->used) {
          client_lift_trigger(p->engine, linedefs[i]->lifts->id);
          if (!linedefs[i]->is_repeatable) {
            linedefs[i]->used = true;
          }
        }
      }
      if (can_collide_with_wall(cp_hf, linedefs[i])) {
        slide_against_wall(&next_pos, p_a);
        continue;
      }
    }
  }

  position_set_pos(pos, next_pos);

  free(linedefs);
}

// END COLLISION HANDLING /////////////////////////////////////////////////

////////// SHOOTING AND INTERACTING LOGIC /////////////////////////////////

// void update_height(player* p,double z){
//   position_ct* pos = player_get_position(p);

//   double target_height = z + PLAYER_HEIGHT;
//   double grav_height = position_get_z(pos) - G * 10e-2 / 2.0 * p->engine->DT
//   * p->engine->DT / 2; position_set_z(pos, fmax(grav_height, target_height));
// }

int correct_height(linedef *wall, int height) {
  if (!(wall->has_back_sidedef)) {
    return 0;
  } else {
    int ceil_height_1 = wall->back_sidedef->sector->ceiling_height;
    int floor_height_1 = wall->back_sidedef->sector->floor_height;
    int ceil_height_2 = wall->front_sidedef->sector->ceiling_height;
    int floor_height_2 = wall->front_sidedef->sector->floor_height;
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

// void fire_bullet(player **players, int num_players, player *player_,
//                  int damage) { // toutes les valeurs de y sont négatives
//   double distance_finale = 100000;
//   if (player_->cooldown < 80) {
//     player_->cooldown = player_->cooldown + 100;
//     linedef **linedefs = player_->engine->wData->linedefs;
//     double x1 = player_->pos.x;
//     double y1 = -player_->pos.y;
//     double x2 = x1 + 100 * cos(deg_to_rad((player_->angle)));
//     double y2 = y1 +
//     SR_3_F_LNF = 123,
//     S1_3_F_LNF = 122,
//     WR_3_F_LNF = 120,
//     W1_3_F_LNF = 121
//     double x_final = 0;
//     double y_final = 0;
//     int direction =
//         1; // 0 correspond a ni droite ni auche 1 a gauche 2 a droite
//     double height = player_->height;
//     if (x1 != x2) {
//       a = (y2 - y1) / (x2 - x1);
//       if (x1 > x2) {
//         direction = 1; // vers la gauche
//       } else {
//         direction = 2; // vers la droite
//       }
//     }
//     double b = y1 - a * x1;
//     for (int i = 0; i < player_->engine->wData->len_linedefs; i++) {
//       double x = 0;
//       double y = 0;
//       if ((!(linedefs[i]->has_back_sidedef)) ||
//           (correct_height(linedefs[i], height))) {
//         double x1a = linedefs[i]->start_vertex->x;
//         double y1a = -linedefs[i]->start_vertex->y;
//         double x2a = linedefs[i]->end_vertex->x;
//         double y2a = -linedefs[i]->end_vertex->y;
//         double c = 0;
//         double d = 0;
//         if (x1a != x2a) {
//           c = (y2a - y1a) / (x2a - x1a);
//           d = y1a - c * x1a;
//           x = (d - b) / (a - c);
//           y = a * x + b;
//           if (distance(x1, y1, x, y) < 100) {
//           }
//           if (((x1a <= x) && (x <= x2a)) || ((x2a <= x) && (x <= x1a))) {
//             if (((direction == 1) && (x1 > x)) ||
//                 ((direction == 2) && (x1 < x))) {
//               if (distance(x1, y1, x, y) < distance_finale) {
//                 distance_finale = distance(x1, y1, x, y);
//                 x_final = x;
//                 y_final = y;
//               }
//             }
//           }

//         } else {
//           x = x1a;
//           y = a * x + b;
//           if (((y1a <= y) && (y <= y2a)) || ((y2a <= y) && (y <= y1a))) {
//             if (((direction == 1) && (x1 >= x)) ||
//                 ((direction == 2) && (x1 <= x))) {
//               if (distance(x1, y1, x, y) < distance_finale) {
//                 distance_finale = distance(x1, y1, x, y);
//                 x_final = x;
//                 y_final = y;
//               }
//             }
//           }
//         }
//       }
//     }
//     for (int j = 0; j < num_players; j++) {
//       double dist_to_hitscan =
//           (fabs(a * (players[j]->pos.x) + (players[j]->pos.y) + b)) /
//           (sqrt(pow(a, 2) + pow(-1, 2)));
//       if (dist_to_hitscan < PLAYER_HITBOX_SIZE) {
//         if ((min(x1, x_final) < players[j]->pos.x) &&
//             (max(x1, x_final) > players[j]->pos.x) &&
//             (min(y1, y_final) < -players[j]->pos.y) &&
//             (min(y1, y_final) < -players[j]->pos.y)) {
//           players[j]->life -= damage;
//           break;
//         }
//       }
//     }
//   }
// }

linedef *cast_ray(linedef **linedefs, int len_linedefs, vec2 player_pos,
                  double player_angle, double height) {
  double distance_finale = 100000;
  linedef *target_linedef = NULL;
  double x1 = player_pos.x;
  double y1 = -player_pos.y;
  double x2 = x1 + 100 * cos(deg_to_rad((player_angle)));
  double y2 = y1 + 100 * sin(deg_to_rad((player_angle)));
  double a = 0;
  int direction = 1; // 0 correspond a ni droite ni auche 1 a gauche 2 a droite
  if (x1 != x2) {
    a = (y2 - y1) / (x2 - x1);
    if (x1 > x2) {
      direction = 1; // vers la gauche
    } else {
      direction = 2; // vers la droite
    }
  }
  double b = y1 - a * x1;
  for (int i = 0; i < len_linedefs; i++) {
    double x = 0;
    double y = 0;
    if (!linedefs[i]->has_back_sidedef || correct_height(linedefs[i], height) ||
        linedefs[i]->sector_tag != 0) {
      if (linedefs[i]->used || !linedefs[i]->is_pushable) {
        continue;
      }
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
              if (distance_finale < TRIGGER_ACTIVATION_RANGE) {
                target_linedef = linedefs[i];
              }
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
              if (distance_finale < TRIGGER_ACTIVATION_RANGE) {
                target_linedef = linedefs[i];
              }
            }
          }
        }
      }
    }
  }
  return target_linedef;
}

////////// END SHOOTING AND INTERACTING LOGIC /////////////////////////////////

void process_keys(player *p) {
  position_ct *pos = player_get_position(p);
  bool is_interacting = keys[get_key_from_action(p->keybinds, "INTERACT")];
  // to avoid spamming the interact key and crashing the audio lmao
  INTERACT_CD -= p->engine->DT;
  // printf("interact cd: %d\n", INTERACT_CD);
  if (is_interacting && INTERACT_CD <= 0) {
    // printf("interacting\n");
    linedef *trigger_linedef = cast_ray(
        p->engine->wData->linedefs, p->engine->wData->len_linedefs,
        position_get_pos(pos), position_get_angle(pos), position_get_z(pos));
    if (trigger_linedef != NULL) {
      // printf("triggering linedef\n");
      if (trigger_linedef->door != NULL) {
        client_door_trigger(p->engine, trigger_linedef->door->id);
      }
      if (trigger_linedef->lifts != NULL) {
        client_lift_trigger(p->engine, trigger_linedef->lifts->id);
      }
      // add_sound_to_play(SWITCH_USE_SOUND, p->pos.x, p->pos.y, p->angle,
      //                   p->pos.x, p->pos.y);
      INTERACT_CD = MAX_INTERACT_COOLDOWN;
    }
  }
  bool is_attacking = keys[get_key_from_action(p->keybinds, "ATTACK")];
  if (is_attacking) {
    printf("attacking\n");
    add_sound_to_play(PISTOL_SOUND, pos->x, pos->y, pos->angle,
                      pos->x + 10 * cos(deg_to_rad(pos->angle)),
                      pos->y - 10 * sin(deg_to_rad(pos->angle)));
    // fire_bullet(p->engine->players, 1, p, 10);
  }

  if (keys[SDL_GetScancodeFromKey(SDL_GetKeyFromName("M"))]) {
    SHOULD_COLLIDE = !SHOULD_COLLIDE;
  }
}

void update_player(player *p) {
  position_ct *pos = player_get_position(p);
  int DT = p->engine->DT;
  bool forward = keys[get_key_from_action(p->keybinds, "MOVE_FORWARD")];
  bool left = keys[get_key_from_action(p->keybinds, "MOVE_LEFT")];
  bool backward = keys[get_key_from_action(p->keybinds, "MOVE_BACKWARD")];
  bool right_d = keys[get_key_from_action(p->keybinds, "MOVE_RIGHT")];
  double speed = DT * PLAYER_SPEED;
  double rot_speed = DT * PLAYER_ROTATION_SPEED;
  position_set_angle(pos, norm(position_get_angle(pos) +
                               rot_speed * ((double)mouse[NUM_MOUSE_BUTTONS])));
  double vec[2] = {0.0, 0.0};
  int count_dir = 0;
  int count_strafe = 0;
  if (forward) {
    vec[0] += speed * cos(deg_to_rad(position_get_angle(pos)));
    vec[1] -= speed * sin(deg_to_rad(position_get_angle(pos)));
    count_dir++;
  }
  if (backward) {
    vec[0] -= speed * cos(deg_to_rad(position_get_angle(pos)));
    vec[1] += speed * sin(deg_to_rad(position_get_angle(pos)));
    count_dir++;
  }
  if (left) {
    vec[0] += speed * sin(deg_to_rad(position_get_angle(pos)));
    vec[1] += speed * cos(deg_to_rad(position_get_angle(pos)));
    count_strafe++;
  }
  if (right_d) {
    vec[0] -= speed * sin(deg_to_rad(position_get_angle(pos)));
    vec[1] -= speed * cos(deg_to_rad(position_get_angle(pos)));
    count_strafe++;
  }
  if (count_dir && count_strafe) {
    vec[0] *= DIAGONAL_CORRECTION;
    vec[1] *= DIAGONAL_CORRECTION;
  }
  if (SHOULD_COLLIDE) { // debug options to go through walls (prevent from
                        // triggering doors, lifts etc)
    move_and_slide(p, vec);
  } else {
    pos->x += vec[0];
    pos->y += vec[1];
  }
}

void player_free(player *p) {
  keybinds_free(p->keybinds);
  settings_free(p->settings);
  world_remove_entity(p->engine->world, p->entity);
  free(p);
}

int player_find(entity_t **list, entity_t *p) {
  return player_find_by_id(list, p->id);
}

int player_find_by_id(entity_t **list, uint64_t id) {
  for (int i = 0; i < PLAYER_MAXIMUM; i++) {
    if (list[i] != NULL && list[i]->id == id) {
      return i;
    }
  }
  return -1;
}

position_ct *player_get_position(player *p) {
  engine *e = p->engine;
  return (position_ct *)world_get_component(e->world, p->entity,
                                            COMPONENT_TAG_POSITION);
}

health_ct *player_get_health(player *p) {
  engine *e = p->engine;
  return (health_ct *)world_get_component(e->world, p->entity,
                                          COMPONENT_TAG_HEALTH);
}

weapon_ct *player_get_weapon(player *p) {
  engine *e = p->engine;
  return (weapon_ct *)world_get_component(e->world, p->entity,
                                          COMPONENT_TAG_WEAPON);
}
i16 get_ssector_floor_height(bsp *b) {
  position_ct *pos = player_get_position(b->player);
  vec2 player_pos = position_get_pos(pos);
  size_t node_id = b->root_node_id;
  while (node_id < SUBSECTOR_IDENTIFIER) {
    node n = b->nodes[node_id];
    bool is_back_side = is_on_back_side(n, player_pos);
    if (is_back_side) {
      node_id = n.back_child_id;
    } else {
      node_id = n.front_child_id;
    }
  }
  i16 subsector_id = node_id - SUBSECTOR_IDENTIFIER;
  subsector subsector = b->subsectors[subsector_id];
  segment seg = subsector.segs[0];
  return seg.front_sector->floor_height;
}

void update_height(player *p) {
  position_ct *pos = player_get_position(p);
  double floor_height = (double)get_ssector_floor_height(p->engine->bsp);
  double target_height = floor_height + PLAYER_HEIGHT;
  pos->z = target_height;

  // double grav_height =
  //     p->height - G * 10e-2 / 2.0 * p->engine->DT * p->engine->DT / 2;
  // p->height = fmax(grav_height, target_height);
}

void get_position_angle(player* p,vec2* res,double* angle){
  position_ct* pos = player_get_position(p);
  res->x = pos->x;
  res->y = pos->y;
  *angle = pos->angle;
}