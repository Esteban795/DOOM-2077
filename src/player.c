#include "../include/player.h"

#define SIGN(x) (int) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
#define DOT(a,b) (a.x * b.x) + (a.y * b.y)

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  p->engine = e;
  p->thing = e->wData->things[0];
  p->pos.x = (double)p->thing.x;
  p->pos.y = (double)p->thing.y;
  p->angle = (double)-p->thing.angle;
  p->keybinds = get_player_keybinds(KEYBINDS_FILE);
  p->settings = get_player_settings(SETTINGS_FILE);
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

// linedef* get_linedefs_in_active_blocks(player* p, double* velocity, int* nlinedefs){
//   //param nlinedefs is a pointer to return the amount of linedefs in the concerned blocks
//   blockmap* bmap = p->engine->wData->blockmap;
//   //we assume velocity is a 2d vector

//   //getting the amount of linedefs to process in the active blocks
//   int block_index_current = blockmap_get_block_index(bmap, p->pos.x, p->pos.y);
//   size_t two_sided_linedefs_count = count_two_sided_linedefs(bmap->blocks[block_index_current].linedefs, bmap->blocks[block_index_current].nlinedefs);
//   int block_index_aftermove = blockmap_get_block_index(bmap, p->pos.x + velocity[0], p->pos.y + velocity[1]);
//   if (block_index_aftermove != block_index_current){
//     two_sided_linedefs_count += count_two_sided_linedefs(bmap->blocks[block_index_aftermove].linedefs, bmap->blocks[block_index_aftermove].nlinedefs);
//   }
//   *nlinedefs = two_sided_linedefs_count;
//   //gather up all the linedefs
//   linedef* linedefs = malloc(sizeof(linedef) * (two_sided_linedefs_count));
//   size_t ind = 0;
//   for (size_t i = 0; i < bmap->blocks[block_index_current].nlinedefs; i++){
//     if (!(bmap->blocks[block_index_current].linedefs[i].flag & TWO_SIDED)){ //its a solid wall, because it has no two sided flag
//       linedefs[ind] = bmap->blocks[block_index_current].linedefs[i];
//       ind++;
//     }
//   }
//   if (block_index_aftermove != block_index_current){
//     for (size_t i = 0; i < bmap->blocks[block_index_aftermove].nlinedefs; i++){
//       if (!(bmap->blocks[block_index_aftermove].linedefs[i].flag & TWO_SIDED)){
//         linedefs[ind] = bmap->blocks[block_index_aftermove].linedefs[i];
//         ind++;
//       }
//     }
//   }
//   return linedefs;
// }

// linedef* get_linedefs_in_active_blocks(player* p, double* velocity, int* nlinedefs){
//   //param nlinedefs is a pointer to return the amount of linedefs in the concerned blocks
//   blockmap* bmap = p->engine->wData->blockmap;
//   //we assume velocity is a 2d vector

//   //getting the amount of linedefs to process in the active blocks
//   int block_index_current = blockmap_get_block_index(bmap, p->pos.x, p->pos.y);
//   (*nlinedefs) += bmap->blocks[block_index_current].nlinedefs;
//   int block_index_aftermove = blockmap_get_block_index(bmap, p->pos.x + velocity[0], p->pos.y + velocity[1]);
//   if (block_index_aftermove != block_index_current){
//     (*nlinedefs) += bmap->blocks[block_index_aftermove].nlinedefs;
//   }

//   //gather up all the linedefs
//   linedef* linedefs = malloc(sizeof(linedef) * (*nlinedefs));
//   for (int i = 0; i < (int)bmap->blocks[block_index_current].nlinedefs; i++){
//     linedefs[i] = bmap->blocks[block_index_current].linedefs[i];
//   }
//   if (block_index_aftermove != block_index_current){
//     int offset = bmap->blocks[block_index_current].nlinedefs;
//     for (int i = 0; i < (int)bmap->blocks[block_index_aftermove].nlinedefs; i++){
//       linedefs[offset + i] = bmap->blocks[block_index_aftermove].linedefs[i];
//     }
//   }
//   return linedefs;
//}

linedef* get_linedefs_in_active_blocks(player* p, int* nlinedefs){
  blockmap* bmap = p->engine->wData->blockmap;

  for (int x = -1; x <= 1; x++){
    for (int y = -1; y <= 1; y++){
      int block_index = blockmap_get_block_index(bmap, p->pos.x + x * 128, p->pos.y + y * 128);
      (*nlinedefs) += bmap->blocks[block_index].nlinedefs;
    }
  }

  linedef* linedefs = malloc(sizeof(linedef) * (*nlinedefs));
  int offset = 0;

  for (int x = -1; x <= 1; x++){
    for (int y = -1; y <= 1; y++){
      int block_index = blockmap_get_block_index(bmap, p->pos.x + x * 128, p->pos.y + y * 128);
      for (int i = 0; i < (int)bmap->blocks[block_index].nlinedefs; i++){
        linedefs[offset] = bmap->blocks[block_index].linedefs[i];
        offset++;
      }
    }
  }

  return linedefs;
}


double cross_pos_linedef(linedef* line, vec2 pos, vertex* vertexes){
  vertex v1 = vertexes[line->start_vertex_id];
  vertex v2 = vertexes[line->end_vertex_id];
  vec2 a = {.x = v2.x - v1.x, .y = v2.y - v1.y};
  vec2 b = {.x = pos.x - v1.x, .y = pos.y - v1.y};
  return (a.y * b.x) - (a.x * b.y);
}

double dot_pos_linedef(linedef* line, vec2 pos, vertex* vertexes){
  vertex v1 = vertexes[line->start_vertex_id];
  vertex v2 = vertexes[line->end_vertex_id];
  vec2 a = {.x = v2.x - v1.x, .y = v2.y - v1.y};
  vec2 b = {.x = pos.x - v1.x, .y = pos.y - v1.y};
  return DOT(a,b);
}

double get_wall_length(linedef* line, vertex* vertexes){
  vertex v1 = vertexes[line->start_vertex_id];
  vertex v2 = vertexes[line->end_vertex_id];
  vec2 a = {.x = v2.x - v1.x, .y = v2.y - v1.y};
  return sqrt(pow(a.x,2)+pow(a.y,2));
}

void get_projections(linedef* line, vec2 pos, vertex* vertexes, vec2* projected, vec2* projected_hitbox){
  vertex v1 = vertexes[line->start_vertex_id];
  vertex v2 = vertexes[line->end_vertex_id];

  vec2 l = {.x = (double)v2.x - (double)v1.x, .y = (double)v2.y - (double)v1.y};
  vec2 pmv1 = {.x = pos.x - (double)v1.x, .y = pos.y - (double)v1.y};

  double projection_factor = (DOT(pmv1, l))/(DOT(l,l));
  projected->x = (double)v1.x + projection_factor * l.x;
  projected->y = (double)v1.y + projection_factor * l.y;

  if (!projected_hitbox){return;}
  double norm_projection = sqrt(pow((projected->x - pos.x),2) + pow((projected->y - pos.y),2));

  printf("%f\n", pos.x + ((projected->x - pos.x)/norm_projection) * PLAYER_RADIUS);

  projected_hitbox->x = pos.x + ((projected->x - pos.x)/norm_projection) * PLAYER_RADIUS * 2;
  projected_hitbox->y = pos.y + ((projected->y - pos.y)/norm_projection) * PLAYER_RADIUS * 2;

  // printf("projecting...\n");
  // printf("po: x=%f,y=%f\n", pos.x, pos.y);
  // printf("v1: x=%f,y=%f\n", (double)v1.x, (double)v1.y);
  // printf("v2: x=%f,y=%f\n", (double)v2.x, (double)v2.y);
  // printf("pr: x=%f,y=%f\n", projected->x, projected->y);
  // printf("no: %f\n", norm_projection);
  // printf("ph: x=%f,y=%f\n", projected_hitbox->x, projected_hitbox->y);
}

void slide_against_wall(vec2* pos_inside_wall, vec2 projected){
  double norm = sqrt(pow((projected.x - pos_inside_wall->x),2) + pow((projected.y - pos_inside_wall->y),2));
  pos_inside_wall->x = projected.x - ((projected.x - pos_inside_wall->x)/norm) * (PLAYER_RADIUS * 2 + 2);
  pos_inside_wall->y = projected.y - ((projected.y - pos_inside_wall->y)/norm) * (PLAYER_RADIUS * 2 + 2);
}

void move_and_slide(player* p, double* velocity){
  int nlinedefs = 0;
  linedef* linedefs = get_linedefs_in_active_blocks(p, &nlinedefs);
  vec2 next_pos = {.x = p->pos.x + velocity[0], .y = p->pos.y + velocity[1]};

  for (int ii = 0; ii < 2*nlinedefs; ii++){

    int i = ii%nlinedefs;

    vec2 p_b; //projection before moving
    vec2 ph_b; //projection hitbox before moving
    vec2 p_a; //projection after moving

    get_projections(linedefs+i, p->pos, p->engine->wData->vertexes, &p_b, &ph_b);

    // check if the player can actually collide with the wall in directions parallel to the wall
    double d = dot_pos_linedef(linedefs+i, p->pos, p->engine->wData->vertexes);
    if (d < -pow(PLAYER_RADIUS+1,2) || d > pow(get_wall_length(linedefs+i, p->engine->wData->vertexes) + PLAYER_RADIUS+1,2)){
      //? why -1?
      // i got no fucking clue mate.
      continue;
    }

    get_projections(linedefs+i, next_pos, p->engine->wData->vertexes, &p_a, 0);

    vec2 ph_bpm = {.x = ph_b.x + (next_pos.x - p->pos.x), .y = ph_b.y + (next_pos.y - p->pos.y)}; //projection before moving + velocity

    double cp_before = cross_pos_linedef(linedefs+i, ph_b, p->engine->wData->vertexes);
    double cp_after = cross_pos_linedef(linedefs+i, ph_bpm, p->engine->wData->vertexes);

    if ((SIGN(cp_before)) != (SIGN(cp_after))){
      //collision happened
      //if cp_after < 0: use the second sidedef
      //else: use the first linedef (first linedef faces "clockwise")
      
      if (linedefs[i].back_sidedef_id != 0xFFFF && linedefs[i].front_sidedef_id != 0xFFFF){
        //we are handling a two-sided linedef, so most probably a portal, or an epic awesome sauce fail
        sector from;
        sector to;
        if (cp_after > 0){
          from = p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].back_sidedef_id].sector];
          to = p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].front_sidedef_id].sector];
        } else {
          from = p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].front_sidedef_id].sector];
          to = p->engine->wData->sectors[p->engine->wData->sidedefs[linedefs[i].back_sidedef_id].sector];
        }

        if (to.floor_height - from.floor_height > PLAYER_STEP) {
          slide_against_wall(&next_pos, p_a);
          continue;
        }
        if (to.ceiling_height - to.floor_height < PLAYER_HEIGHT) {
          slide_against_wall(&next_pos, p_a);
          continue;
        }


      } else {
        uint16_t sidedef_id; //this is the sidedef that faces the player upon bonking against the wall
        if (cp_after > 0){
          sidedef_id = linedefs[i].back_sidedef_id;
        } else {
          sidedef_id = linedefs[i].front_sidedef_id;
        }
        if (sidedef_id != 0xFFFF){
          slide_against_wall(&next_pos, p_a);
        }
      }
    }
  }

  p->pos.x = next_pos.x;
  p->pos.y = next_pos.y;

  free(linedefs);
}

void update_player(player *p, int mouse_x, const uint8_t *keyboard_state) {
  bool forward =
      keyboard_state[get_key_from_action(p->keybinds, "MOVE_FORWARD")];
  bool left = keyboard_state[get_key_from_action(p->keybinds, "MOVE_LEFT")];
  bool backward =
      keyboard_state[get_key_from_action(p->keybinds, "MOVE_BACKWARD")];
  bool right_d = keyboard_state[get_key_from_action(p->keybinds, "MOVE_RIGHT")];
  double speed = DT * PLAYER_SPEED;
  double rot_speed = PLAYER_ROTATION_SPEED * DT;
  double vec[2] = {0.0, 0.0};
  int count_dir = 0;
  int count_strafe = 0;
  if (forward) {
    vec[0] += speed * cos(deg_to_rad(p->angle));
    vec[1] -= speed * sin(deg_to_rad(p->angle));
    count_dir++;
  }
  if (backward) {
    vec[0] -= speed * cos(deg_to_rad(p->angle));
    vec[1] += speed * sin(deg_to_rad(p->angle));
    count_dir++;
  }
  if (left) {
    vec[0] += speed * sin(deg_to_rad(p->angle));
    vec[1] += speed * cos(deg_to_rad(p->angle));
    count_strafe++;
  }
  if (right_d) {
    vec[0] -= speed * sin(deg_to_rad(p->angle));
    vec[1] -= speed * cos(deg_to_rad(p->angle));
    count_strafe++;
  }
  if (count_dir && count_strafe) {
    vec[0] *= DIAGONAL_CORRECTION;
    vec[1] *= DIAGONAL_CORRECTION;
  }
  //p->pos.x += vec[0];
  //p->pos.y += vec[1];
  move_and_slide(p, vec);
  p->angle += rot_speed * mouse_x;
  p->angle = fmod(p->angle, 360);
  p->angle = p->angle < 0 ? 360 + p->angle : p->angle;
}

void player_free(player *p) {
  free_keybinds(p->keybinds);
  free_settings(p->settings);
  free(p);
}