#include "../include/player.h"

player *player_init(engine *e) {
  player *p = malloc(sizeof(player));
  int* ammo = malloc(WEAPONS_NUMBER*sizeof(int));
  ammo[0] = -2;
  for (int i = 1; i < WEAPONS_NUMBER; i++) {
    ammo[i] = -1;
  }
  p->engine = e;
  p->thing = e->wData->things[0];
  p->x = (double)p->thing.x;
  p->y = (double)p->thing.y;
  p->angle = (double)-p->thing.angle;
  p->keybinds = get_player_keybinds(KEYBINDS_FILE);
  p->settings = get_player_settings(SETTINGS_FILE);
  p->ammo = ammo;
  p->active_weapon=0;
  return p;
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
  p->x += vec[0];
  p->y += vec[1];
  if (SDL_WINDOW_INPUT_FOCUS) {
    p->angle += rot_speed * mouse_x;
    p->angle = fmod(p->angle, 360);
    p->angle = p->angle < 0 ? 360 + p->angle : p->angle;
  }
}

void player_free(player *p) {
  free_keybinds(p->keybinds);
  free_settings(p->settings);
  free(p->ammo);
  free(p);
}
