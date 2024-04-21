#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../include/player.h"
#include "../include/ecs/world.h"
#include "../include/ecs/entity.h"
#include "../include/ecs/archetype.h"
#include "../include/ecs/component.h"
#include "../include/component/position.h"
#include "../include/component/health.h"
#include "../include/component/weapon.h"

#define SIGN(x) (int)(x > 0) ? 1 : ((x < 0) ? -1 : 0)
#define DOT(a, b) (a.x * b.x) + (a.y * b.y)
#define DISTANCE_VEC_VER(a, b) (sqrt(pow(b->x - a.x, 2) + pow(b->y - a.y, 2)))

#define M_PI 3.14159265358979323846

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  unsigned int* ammo = malloc(WEAPONS_NUMBER*sizeof(unsigned int));
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
  component_t** comps = malloc(sizeof(component_t*) * 3);
  comps[0] = position_create(coords, p->thing.angle + 180.0);
  comps[1] = health_create(100.0, 100.0);
  comps[2] = weapon_create(ammo);
  p->entity = world_insert_entity(e->world, e->remote->player_id, comps, 3);
  free(comps);
  
  return p;
}

// size_t count_two_sided_linedefs(linedef* linedefs, size_t nlinedefs){
//   size_t count = 0;
//   for (size_t i = 0; i < nlinedefs; i++){
//     if (!(linedefs[i].flag & TWO_SIDED)){
//       count++;
//     }
//   }
//   return count;
// }

// linedef* get_linedefs_in_active_blocks(player* p, double* velocity, int*
// nlinedefs){
//   //param nlinedefs is a pointer to return the amount of linedefs in the
//   concerned blocks blockmap* bmap = p->engine->wData->blockmap;
//   //we assume velocity is a 2d vector

//   //getting the amount of linedefs to process in the active blocks
//   int block_index_current = blockmap_get_block_index(bmap, p->pos.x,
//   p->pos.y); size_t two_sided_linedefs_count =
//   count_two_sided_linedefs(bmap->blocks[block_index_current].linedefs,
//   bmap->blocks[block_index_current].nlinedefs); int block_index_aftermove =
//   blockmap_get_block_index(bmap, p->pos.x + velocity[0], p->pos.y +
//   velocity[1]); if (block_index_aftermove != block_index_current){
//     two_sided_linedefs_count +=
//     count_two_sided_linedefs(bmap->blocks[block_index_aftermove].linedefs,
//     bmap->blocks[block_index_aftermove].nlinedefs);
//   }
//   *nlinedefs = two_sided_linedefs_count;
//   //gather up all the linedefs
//   linedef* linedefs = malloc(sizeof(linedef) * (two_sided_linedefs_count));
//   size_t ind = 0;
//   for (size_t i = 0; i < bmap->blocks[block_index_current].nlinedefs; i++){
//     if (!(bmap->blocks[block_index_current].linedefs[i].flag & TWO_SIDED)){
//     //its a solid wall, because it has no two sided flag
//       linedefs[ind] = bmap->blocks[block_index_current].linedefs[i];
//       ind++;
//     }
//   }
//   if (block_index_aftermove != block_index_current){
//     for (size_t i = 0; i < bmap->blocks[block_index_aftermove].nlinedefs;
//     i++){
//       if (!(bmap->blocks[block_index_aftermove].linedefs[i].flag &
//       TWO_SIDED)){
//         linedefs[ind] = bmap->blocks[block_index_aftermove].linedefs[i];
//         ind++;
//       }
//     }
//   }
//   return linedefs;
// }

// linedef* get_linedefs_in_active_blocks(player* p, double* velocity, int*
// nlinedefs){
//   //param nlinedefs is a pointer to return the amount of linedefs in the
//   concerned blocks blockmap* bmap = p->engine->wData->blockmap;
//   //we assume velocity is a 2d vector

//   //getting the amount of linedefs to process in the active blocks
//   int block_index_current = blockmap_get_block_index(bmap, p->pos.x,
//   p->pos.y);
//   (*nlinedefs) += bmap->blocks[block_index_current].nlinedefs;
//   int block_index_aftermove = blockmap_get_block_index(bmap, p->pos.x +
//   velocity[0], p->pos.y + velocity[1]); if (block_index_aftermove !=
//   block_index_current){
//     (*nlinedefs) += bmap->blocks[block_index_aftermove].nlinedefs;
//   }

//   //gather up all the linedefs
//   linedef* linedefs = malloc(sizeof(linedef) * (*nlinedefs));
//   for (int i = 0; i < (int)bmap->blocks[block_index_current].nlinedefs; i++){
//     linedefs[i] = bmap->blocks[block_index_current].linedefs[i];
//   }
//   if (block_index_aftermove != block_index_current){
//     int offset = bmap->blocks[block_index_current].nlinedefs;
//     for (int i = 0; i < (int)bmap->blocks[block_index_aftermove].nlinedefs;
//     i++){
//       linedefs[offset + i] = bmap->blocks[block_index_aftermove].linedefs[i];
//     }
//   }
//   return linedefs;
//}

linedef *get_linedefs_in_active_blocks(player *p, int *nlinedefs) {
  blockmap *bmap = p->engine->wData->blockmap;
  position_ct* pos = player_get_position(p);

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int block_index = blockmap_get_block_index(bmap, position_get_x(pos) + x * 128,
                                                 position_get_y(pos) + y * 128);
      (*nlinedefs) += bmap->blocks[block_index].nlinedefs;
    }
  }

  linedef *linedefs = malloc(sizeof(linedef) * (*nlinedefs));
  int offset = 0;

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      int block_index = blockmap_get_block_index(bmap, position_get_x(pos) + x * 128,
                                                 position_get_y(pos) + y * 128);
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

  // printf("%f\n", pos.x + ((projected->x - pos.x)/norm_projection) *
  // PLAYER_RADIUS);

  projected_hitbox->x =
      pos.x + ((projected->x - pos.x) / norm_projection) * PLAYER_RADIUS * 2;
  projected_hitbox->y =
      pos.y + ((projected->y - pos.y) / norm_projection) * PLAYER_RADIUS * 2;

  projected_hitbox_back->x =
      pos.x - ((projected->x - pos.x) / norm_projection) * PLAYER_RADIUS * 2;
  projected_hitbox_back->y =
      pos.y - ((projected->y - pos.y) / norm_projection) * PLAYER_RADIUS * 2;

  // printf("projecting...\n");
  // printf("po: x=%f,y=%f\n", pos.x, pos.y);
  // printf("v1: x=%f,y=%f\n", (double)v1.x, (double)v1.y);
  // printf("v2: x=%f,y=%f\n", (double)v2.x, (double)v2.y);
  // printf("pr: x=%f,y=%f\n", projected->x, projected->y);
  // printf("no: %f\n", norm_projection);
  // printf("ph: x=%f,y=%f\n", projected_hitbox->x, projected_hitbox->y);
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

bool can_collide_with_wall(double cp_after, linedef linedef) {
  if (linedef.has_back_sidedef) {
    // we are handling a two-sided linedef, so most probably a portal, or an
    // epic awesome sauce fail
    sector *from;
    sector *to;
    if (cp_after > 0) {
      from = linedef.back_sidedef->sector;
      to = linedef.front_sidedef->sector;
      // from =
      // p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].back_sidedef_id].sector];
      // to =
      // p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].front_sidedef_id].sector];
    } else {
      from = linedef.front_sidedef->sector;
      to = linedef.back_sidedef->sector;
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
  position_ct* pos = player_get_position(p);

  int nlinedefs = 0;
  linedef *linedefs = get_linedefs_in_active_blocks(p, &nlinedefs);
  vec2 next_pos = {.x = position_get_x(pos) + velocity[0], .y = position_get_y(pos) + velocity[1]};
  for (int ii = 0; ii < 2 * nlinedefs; ii++) {

    int i = ii % nlinedefs;

    vec2 p_a;   // projection after moving
    vec2 phf_a; // projection of the hitbox after moving, front
    vec2 phb_a; // projection of the hitbox after moving, back

    // check if the player can actually collide with the wall in directions
    // parallel to the wall
    double d = dot_pos_linedef(linedefs + i, position_get_pos(pos));
    if (d < 0 || d > pow(get_wall_length(linedefs + i), 2)) {
      vertex *linedef_a = linedefs[i].start_vertex;
      vertex *linedef_b = linedefs[i].end_vertex;

      if (DISTANCE_VEC_VER(next_pos, linedef_a) < PLAYER_RADIUS) {
        slide_against_point(*linedef_a, &next_pos);
      } else if (DISTANCE_VEC_VER(next_pos, linedef_b) < PLAYER_RADIUS) {
        slide_against_point(*linedef_b, &next_pos);
      }

      continue;
    }
    get_projections(linedefs + i, next_pos, &p_a, &phf_a, &phb_a);

    double cp_hb = cross_pos_linedef(linedefs + i, phb_a);
    double cp_hf = cross_pos_linedef(linedefs + i, phf_a);

    if ((SIGN(cp_hb)) != (SIGN(cp_hf))) {
      // collision happened
      // if cp_after < 0: use the second sidedef
      // else: use the first linedef (first linedef faces "clockwise")
      if (can_collide_with_wall(cp_hf, linedefs[i])) {
        slide_against_wall(&next_pos, p_a);
        continue;
      }
    }
  }

  position_set_pos(pos, next_pos);

  free(linedefs);
}


void update_height(player* p,double z){
  position_ct* pos = player_get_position(p);

  double target_height = z + PLAYER_HEIGHT;
  double grav_height =
      position_get_z(pos) - G * 10e-2 / 2.0 * p->engine->DT * p->engine->DT / 2;
  position_set_z(pos, fmax(grav_height, target_height));
}

void update_player(player *p) {
  position_ct* pos = player_get_position(p);

  int DT = p->engine->DT;
  bool forward = keys[get_key_from_action(p->keybinds, "MOVE_FORWARD")];
  bool left = keys[get_key_from_action(p->keybinds, "MOVE_LEFT")];
  bool backward = keys[get_key_from_action(p->keybinds, "MOVE_BACKWARD")];
  bool right_d = keys[get_key_from_action(p->keybinds, "MOVE_RIGHT")];
  double speed = DT * PLAYER_SPEED;
  double rot_speed = DT * PLAYER_ROTATION_SPEED;
  position_set_angle(pos, norm(position_get_angle(pos) + rot_speed * ((double)mouse[NUM_MOUSE_BUTTONS])));
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
  // p->pos.x += vec[0];
  // p->pos.y += vec[1];
  move_and_slide(p, vec);
}

void player_free(player *p) {
  free_keybinds(p->keybinds);
  free_settings(p->settings);
  free(player_get_weapon(p)->ammunitions);
  world_remove_entity(p->engine->world, p->entity);
  free(p);
}

position_ct* player_get_position(player* p) {
  engine* e = p->engine;
  return (position_ct*)world_get_component(e->world, p->entity, COMPONENT_TAG_POSITION);
}

health_ct* player_get_health(player* p) {
  engine* e = p->engine;
  return (health_ct*)world_get_component(e->world, p->entity, COMPONENT_TAG_HEALTH);
}

weapon_ct* player_get_weapon(player* p) {
  engine* e = p->engine;
  return (weapon_ct*)world_get_component(e->world, p->entity, COMPONENT_TAG_WEAPON);
}