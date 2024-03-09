#include "../include/player.h"

#define M_PI 3.14159265358979323846

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  p->engine = e;
  p->thing = e->wData->things[0];
  p->x = (double)p->thing.x;
  p->y = (double)p->thing.y;
  p->angle = (double)p->thing.angle + 180.0;
  p->height = PLAYER_HEIGHT;
  return p;
}


void update_height(player* p,double z){
  double target_height = z + PLAYER_HEIGHT;
  double grav_height = p->height - G * 10e-2 / 2.0 * p->engine->DT * p->engine->DT / 2;
  p->height = fmax(grav_height, target_height);
}

void update_player(player *p, int mouse_x, const uint8_t *keyboard_state) {
  int DT = p->engine->DT;
  bool keydown_z = keyboard_state[SDL_SCANCODE_W];
  bool keydown_q = keyboard_state[SDL_SCANCODE_A];
  bool keydown_s = keyboard_state[SDL_SCANCODE_S];
  bool keydown_d = keyboard_state[SDL_SCANCODE_D];
  double speed = DT * PLAYER_SPEED;
  double rot_speed = DT * PLAYER_ROTATION_SPEED;
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
  p->x += vec[0];
  p->y += vec[1];
  if (SDL_WINDOW_INPUT_FOCUS) {
    p->angle += rot_speed * mouse_x;
    p->angle = norm(p->angle);
  }
}

void player_free(player *p) { free(p); }