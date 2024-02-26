#include "../include/player.h"

#define M_PI 3.14159265358979323846
#define HALF_M_PI 1.57079632679489661923

#define SIGN(x) (int) (x > 0) ? 1 : ((x < 0) ? -1 : 0)
#define DOT(a,b) (a.x * b.x) + (a.y * b.y)

double deg_to_rad(double deg) { return deg * (M_PI / 180); }

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  p->engine = e;
  p->thing = e->wData->things[0];
  p->pos.x = (double)p->thing.x;
  p->pos.y = (double)p->thing.y;
  p->angle = (double)p->thing.angle;
  return p;
}

linedef* get_linedefs_in_active_blocks(player* p, double* velocity, int* nlinedefs){
  //param nlinedefs is a pointer to return the amount of linedefs in the concerned blocks
  blockmap* bmap = p->engine->wData->blockmap;
  //we assume velocity is a 2d vector

  //getting the amount of linedefs to process in the active blocks
  int block_index_current = blockmap_get_block_index(bmap, p->pos.x, p->pos.y);
  (*nlinedefs) += bmap->blocks[block_index_current].nlinedefs;
  int block_index_aftermove = blockmap_get_block_index(bmap, p->pos.x + velocity[0], p->pos.y + velocity[1]);
  if (block_index_aftermove != block_index_current){
    (*nlinedefs) += bmap->blocks[block_index_aftermove].nlinedefs;
  }

  //gather up all the linedefs
  linedef* linedefs = malloc(sizeof(linedef) * (*nlinedefs));
  for (int i = 0; i < (int)bmap->blocks[block_index_current].nlinedefs; i++){
    linedefs[i] = bmap->blocks[block_index_current].linedefs[i];
  }
  if (block_index_aftermove != block_index_current){
    int offset = bmap->blocks[block_index_current].nlinedefs;
    for (int i = 0; i < (int)bmap->blocks[block_index_aftermove].nlinedefs; i++){
      linedefs[offset + i] = bmap->blocks[block_index_aftermove].linedefs[i];
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

double get_wall_length_squared(linedef* line, vertex* vertexes){
  vertex v1 = vertexes[line->start_vertex_id];
  vertex v2 = vertexes[line->end_vertex_id];
  vec2 a = {.x = v2.x - v1.x, .y = v2.y - v1.y};
  return DOT(a,a);
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

  projected_hitbox->x = pos.x + ((projected->x - pos.x)/norm_projection) * PLAYER_RADIUS;
  projected_hitbox->y = pos.y + ((projected->y - pos.y)/norm_projection) * PLAYER_RADIUS;

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
  pos_inside_wall->x = projected.x - ((projected.x - pos_inside_wall->x)/norm) * (PLAYER_RADIUS + 1);
  pos_inside_wall->y = projected.y - ((projected.y - pos_inside_wall->y)/norm) * (PLAYER_RADIUS + 1);
}

void move_and_slide(player* p, double* velocity){
  int nlinedefs = 0;
  linedef* linedefs = get_linedefs_in_active_blocks(p, velocity, &nlinedefs);
  vec2 next_pos = {.x = p->pos.x + velocity[0], .y = p->pos.y + velocity[1]};

  for (int i = 0; i < nlinedefs; i++){
    vec2 p_b; //projection before moving
    vec2 ph_b; //projection hitbox before moving
    vec2 p_a; //projection after moving

    get_projections(linedefs+i, p->pos, p->engine->wData->vertexes, &p_b, &ph_b);

    // check if the player can actually collide with the wall in directions parallel to the wall
    double d = dot_pos_linedef(linedefs+i, p->pos, p->engine->wData->vertexes);
    if (d < 0 || d > get_wall_length_squared(linedefs+i, p->engine->wData->vertexes)){
      continue;
    }

    get_projections(linedefs+i, next_pos, p->engine->wData->vertexes, &p_a, 0);

    vec2 ph_bpm = {.x = ph_b.x + (next_pos.x - p->pos.x), .y = ph_b.y + (next_pos.y - p->pos.y)}; //projection before moving + velocity

    double cp_before = cross_pos_linedef(linedefs+i, ph_b, p->engine->wData->vertexes);
    double cp_after = cross_pos_linedef(linedefs+i, ph_bpm, p->engine->wData->vertexes);

    if ((SIGN(cp_before)) != (SIGN(cp_after))){
      //collision happened
      //TODO: implement floor and height logic
      //TODO: add a step height
      //TODO: make sure the sector is tall enough for you to fit in
      slide_against_wall(&next_pos, p_a);
    }
  }

  p->pos.x = next_pos.x;
  p->pos.y = next_pos.y;

  free(linedefs);
}

void update_player(player *p, int mouse_x, const uint8_t *keyboard_state) {
  bool keydown_z = keyboard_state[SDL_SCANCODE_W];
  bool keydown_q = keyboard_state[SDL_SCANCODE_A];
  bool keydown_s = keyboard_state[SDL_SCANCODE_S];
  bool keydown_d = keyboard_state[SDL_SCANCODE_D];
  double speed = DT * PLAYER_SPEED;
  double rot_speed = PLAYER_ROTATION_SPEED * DT;
  double vec[2] = {0.0, 0.0};
  int count_dir = 0;
  int count_strafe = 0;
  if (keydown_z) {
    vec[0] += speed * cos(deg_to_rad(p->angle));
    vec[1] -= speed * sin(deg_to_rad(p->angle));
    count_dir++;
  }
  if (keydown_s) {
    vec[0] -= speed * cos(deg_to_rad(p->angle));
    vec[1] += speed * sin(deg_to_rad(p->angle));
    count_dir++;
  }
  if (keydown_q) {
    vec[0] += speed * sin(deg_to_rad(p->angle));
    vec[1] += speed * cos(deg_to_rad(p->angle));
    count_strafe++;
  }
  if (keydown_d) {
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

void player_free(player *p) { free(p); }