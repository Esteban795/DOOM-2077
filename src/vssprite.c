#include "../include/vssprite.h"

vs_sprite VSSPRITES[MAX_SPRITES];
int VSSPRITES_INDEX = 0;

void vssprite_add(vec2 camera_pos, double angle, vec2 pos, patch *sprite,Uint32 color) {
  if (VSSPRITES_INDEX >= MAX_SPRITES) {
    return;
  }
  vs_sprite s;
  double sprite_angle = point_to_angle(camera_pos, pos);
  double x1 = angle_to_x_pos(angle + sprite_angle);
  double x_angle = norm(rad_to_deg(atan((HALF_WIDTH - x1) / SCREEN_DISTANCE)));
  double D = SCREEN_DISTANCE / cos(deg_to_rad(x_angle));
  double d = dist(camera_pos, pos);
  double scale1 = fmin(MAX_SCALE, fmax(MIN_SCALE, D / d));
  s.pos = pos;
  s.x1 = x1 - scale1 * sprite->header.width;
  s.x2 = x1 + scale1 * sprite->header.width;
  s.scale = scale1;
  s.sprite = sprite;
  s.color = color;
  VSSPRITES[VSSPRITES_INDEX] = s;
  VSSPRITES_INDEX++;
}

// we're drawing from back to front order, so we need to sort the sprites by scale and ascending
int sort_by_scale(const void *a, const void *b) {
  vs_sprite *sprite_a = (vs_sprite *)a;
  vs_sprite *sprite_b = (vs_sprite *)b;
  if (sprite_a->scale < sprite_b->scale) {
    return -1;
  } else if (sprite_a->scale > sprite_b->scale) {
    return 1;
  }
  return 0;
}

void vssprite_sort() {
  if (VSSPRITES_INDEX == 0) {
    return;
  }
  qsort(VSSPRITES, VSSPRITES_INDEX, sizeof(vs_sprite), sort_by_scale);
}