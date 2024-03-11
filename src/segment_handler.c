#include "../include/segment_handler.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <string.h>

bool BSP_TRAVERSE = true;

color get_color(char *texture, i16 light_level) {
  srand(texture[1] + light_level);
  return (color){.r = rand() % 255, .g = rand() % 255, .b = rand() % 255};
}

double scale_from_global_angle(segment_handler *sh, int x, double normal_angle,
                               double dist) {
  double x_angle = rad_to_deg(atan((HALF_WIDTH - x) / SCREEN_DISTANCE));
  double num =
      fabs(SCREEN_DISTANCE *
           cos(deg_to_rad(-normal_angle + x_angle - sh->player->angle)));
  double den = dist * cos(deg_to_rad(x_angle));
  double scale = num / den;
  scale = fmin(MAX_SCALE, fmax(MIN_SCALE, scale));
  return scale;
}

void draw_solid_walls_range(segment_handler *sh, int x1, int x2) {
  // will be used soon while applying textures on walls
  char *wall_texture = sh->seg->linedef->front_sidedef->middle_texture;
  char *ceiling_texture = sh->seg->front_sector->ceiling_texture;
  char *floor_texture = sh->seg->front_sector->floor_texture;
  i16 light_level = sh->seg->front_sector->light_level;

  int world_front_z1 =
      sh->seg->front_sector->ceiling_height - sh->player->height;
  int world_front_z2 = sh->seg->front_sector->floor_height - sh->player->height;

  // check which parts need to be rendered
  bool draw_wall = strcmp(sh->seg->linedef->front_sidedef->middle_texture, "-");
  bool draw_ceiling = world_front_z1 > 0; // can we actually see the ceiling ?
  bool draw_floor = world_front_z2 < 0;   // can we actually see the floor ?

  double normal_angle = sh->seg->angle + 90.0; // get normal angle of segment
  double offset_angle = normal_angle - sh->raw_angle_1;

  player *p = sh->engine->p;
  vec2 player_pos = {.x = p->x, .y = p->y};
  vec2 start_vertex_pos = {.x = sh->seg->start_vertex->x,
                           .y = sh->seg->start_vertex->y};
  double hyp = dist(player_pos,
                    start_vertex_pos); // distance from player to start vertex
  double raw_dist =
      hyp * cos(deg_to_rad(offset_angle)); // distance from player to wall

  double rw_scale_step = 0.0;
  double scale1 = scale_from_global_angle(sh, x1, normal_angle, raw_dist);
  if (x1 < x2) {
    double scale2 = scale_from_global_angle(sh, x2, normal_angle, raw_dist);
    rw_scale_step =
        (scale2 - scale1) /
        (x2 - x1); // interpolation to find the scale for second vertex
  }
  double wall_y1 =
      HALF_HEIGHT -
      world_front_z1 * scale1; // initial y position of top of the wall
  double wall_y1_step =
      -rw_scale_step *
      world_front_z1; // step to find the next y position of top of the wall

  double wall_y2 =
      HALF_HEIGHT -
      world_front_z2 * scale1; // initial y position of bottom of the wall
  double wall_y2_step =
      -rw_scale_step *
      world_front_z2; // step to find the next y position of bottom of the wall
  for (int i = x1; i < x2; i++) {
    int draw_wall_y1 = (int)(wall_y1)-1;
    int draw_wall_y2 = (int)wall_y2;

    if (draw_ceiling) {
      int cy1 = (int)(sh->upper_clip[i] + 1);
      int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
      color c = get_color(ceiling_texture, light_level);
      draw_vline(sh->engine->map_renderer, i, cy1, cy2, c);
    }

    if (draw_wall) {
      color c = get_color(wall_texture, light_level);
      int wy1 =
          (int)fmax(draw_wall_y1, sh->upper_clip[i] + 1); // max(0, upper_clip)
      int wy2 = (int)fmin(draw_wall_y2,
                          sh->lower_clip[i] - 1); // min(HEIGHT, lower_clip)
      draw_vline(sh->engine->map_renderer, i, wy1, wy2, c);
    }

    if (draw_floor) {
      int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
      int fy2 = (int)(sh->lower_clip[i] - 1);
      color c = get_color(floor_texture, light_level);
      draw_vline(sh->engine->map_renderer, i, fy1, fy2, c);
    }
    wall_y1 += wall_y1_step;
    wall_y2 += wall_y2_step;
  }
}

void draw_portal_walls_range(segment_handler *sh, int x1, int x2) {
  segment *seg = sh->seg;
  sector *front_sector = seg->front_sector;
  sector *back_sector = seg->back_sector;
  sidedef *front_sidedef = seg->linedef->front_sidedef;
  char *upper_wall_texture = front_sidedef->upper_texture;
  char *lower_wall_texture = front_sidedef->lower_texture;
  char *texture_ceiling_id = front_sector->ceiling_texture;
  char *texture_floor_id = front_sector->floor_texture;
  i16 light_level = front_sector->light_level;

  int world_front_z1 =
      (int)front_sector->ceiling_height - (int)sh->player->height;
  int world_front_z2 =
      (int)front_sector->floor_height - (int)sh->player->height;
  int world_back_z1 =
      (int)back_sector->ceiling_height - (int)sh->player->height;
  int world_back_z2 = (int)back_sector->floor_height - (int)sh->player->height;

  bool draw_upper_wall = false;
  bool draw_ceiling = false;
  if (world_front_z1 != world_back_z1 ||
      front_sector->light_level != back_sector->light_level ||
      strcmp(front_sector->ceiling_texture, back_sector->ceiling_texture)) {
    draw_upper_wall =
        strcmp(upper_wall_texture, "-") && world_back_z1 < world_front_z1;
    draw_ceiling = world_front_z1 >= 0;
  }

  bool draw_lower_wall = false;
  bool draw_floor = false;

  if (world_front_z2 != world_back_z2 ||
      front_sector->light_level != back_sector->light_level ||
      strcmp(front_sector->floor_texture, back_sector->floor_texture)) {
    draw_lower_wall =
        strcmp(lower_wall_texture, "-") && world_back_z2 > world_front_z2;
    draw_floor = world_front_z2 <= 0;
  }

  if (!draw_upper_wall && !draw_lower_wall && !draw_ceiling &&
      !draw_floor) { // nothing to draw
    return;
  }

  double normal_angle = sh->seg->angle + 90.0; // get normal angle of segment
  double offset_angle = normal_angle - sh->raw_angle_1;

  player *p = sh->engine->p;
  vec2 player_pos = {.x = p->x, .y = p->y};
  vec2 start_vertex_pos = {.x = sh->seg->start_vertex->x,
                           .y = sh->seg->start_vertex->y};
  double hyp = dist(player_pos,
                    start_vertex_pos); // distance from player to start vertex
  double raw_dist =
      hyp * cos(deg_to_rad(offset_angle)); // distance from player to wall

  double rw_scale_step = 0.0;
  double scale1 = scale_from_global_angle(sh, x1, normal_angle, raw_dist);
  if (x1 < x2) {
    double scale2 = scale_from_global_angle(sh, x2, normal_angle, raw_dist);
    rw_scale_step =
        (scale2 - scale1) /
        (x2 - x1); // interpolation to find the scale for second vertex
  }
  double wall_y1 =
      HALF_HEIGHT -
      world_front_z1 * scale1; // initial y position of top of the wall
  double wall_y1_step =
      -rw_scale_step *
      world_front_z1; // step to find the next y position of top of the wall

  double wall_y2 =
      HALF_HEIGHT -
      world_front_z2 * scale1; // initial y position of bottom of the wall
  double wall_y2_step =
      -rw_scale_step *
      world_front_z2; // step to find the next y position of bottom of the wall

  double portal_y1 = 0;
  double portal_y2 = 0;
  double portal_y1_step = 0;
  double portal_y2_step = 0;

  if (draw_upper_wall) {
    if (world_back_z1 > world_front_z2) {
      portal_y1 = HALF_HEIGHT - world_back_z1 * scale1;
      portal_y1_step = -rw_scale_step * world_back_z1;
    } else {
      portal_y1 = wall_y2;
      portal_y1_step = wall_y2_step;
    }
  }

  if (draw_lower_wall) {
    if (world_back_z2 < world_front_z1) {
      portal_y2 = HALF_HEIGHT - world_back_z2 * scale1;
      portal_y2_step = -rw_scale_step * world_back_z2;
    } else {
      portal_y2 = wall_y1;
      portal_y2_step = wall_y1_step;
    }
  }

  for (int i = x1; i < x2 + 1; i++) {
    double draw_wall_y1 = wall_y1 - 1;
    double draw_wall_y2 = wall_y2;

    if (draw_upper_wall) {
      double draw_upper_wall_y1 = wall_y1 - 1;
      double draw_upper_wall_y2 = portal_y1;

      if (draw_ceiling) {
        color c = get_color(texture_ceiling_id, light_level);
        int cy1 = (int)(sh->upper_clip[i] + 1);
        int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
        draw_vline(sh->engine->map_renderer, i, cy1, cy2, c);
      }

      int wy1 = (int)fmax(draw_upper_wall_y1, sh->upper_clip[i] + 1);
      int wy2 = (int)fmin(draw_upper_wall_y2, sh->lower_clip[i] - 1);
      color c = get_color(upper_wall_texture, light_level);
      draw_vline(sh->engine->map_renderer, i, wy1, wy2, c);
      if (sh->upper_clip[i] < wy2) {
        sh->upper_clip[i] = wy2;
      }
      portal_y1 += portal_y1_step;
    }

    if (draw_ceiling) {
      int cy1 = (int)(sh->upper_clip[i] + 1);
      int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
      color c = get_color(texture_ceiling_id, light_level);
      draw_vline(sh->engine->map_renderer, i, cy1, cy2, c);
      if (sh->upper_clip[i] < cy2) {
        sh->upper_clip[i] = cy2;
      }
    }

    if (draw_lower_wall) {
      if (draw_floor) {
        int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
        int fy2 = (int)(sh->lower_clip[i] - 1);
        color c = get_color(texture_floor_id, light_level);
        draw_vline(sh->engine->map_renderer, i, fy1, fy2, c);
      }
      int draw_lower_wall_y1 = portal_y2 - 1;
      int draw_lower_wall_y2 = wall_y2;
      int wy1 = (int)fmax(draw_lower_wall_y1, sh->upper_clip[i] + 1);
      int wy2 = (int)fmin(draw_lower_wall_y2, sh->lower_clip[i] - 1);
      color c = get_color(lower_wall_texture, light_level);
      draw_vline(sh->engine->map_renderer, i, wy1, wy2, c);
      if (sh->lower_clip[i] > wy1) {
        sh->lower_clip[i] = wy1;
      }
      portal_y2 += portal_y2_step;
    }

    if (draw_floor) {
      int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
      int fy2 = (int)(sh->lower_clip[i] - 1);
      color c = get_color(texture_floor_id, light_level);
      draw_vline(sh->engine->map_renderer, i, fy1, fy2, c);

      if (sh->lower_clip[i] > draw_wall_y2)
        sh->lower_clip[i] = fy1;
    }
    wall_y1 += wall_y1_step;
    wall_y2 += wall_y2_step;
  }
}

void clip_solid_walls(segment_handler *sh, int x1, int x2) {
  if (sh->screen_range_count <
      WIDTH) { // the screen is not fully occupied by walls
    int index_first_0 = x1;
    int index_last_0 = x2;
    int i = x1;
    while (i < x2) {
      if (sh->screen_range[i] == 0) {
        index_first_0 = i;
        while (i < x2 && sh->screen_range[i] == 0) {
          i++;
        }
        index_last_0 = i;
        if (index_last_0 - index_first_0 > 0) {
          draw_solid_walls_range(sh, index_first_0, index_last_0);
          sh->screen_range_count += index_last_0 - index_first_0;
        }
      } else {
        i++;
      }
    }
    for (int i = x1; i < x2; i++) {
      sh->screen_range[i] = 1;
    }
  } else { // we've already drawn every solid wall on our screen, stop there
    BSP_TRAVERSE = false;
  }
}

void clip_portal_walls(segment_handler *sh, int x1, int x2) {
  int index_first_0 = x1;
  int index_last_0 = x2;
  int i = x1;
  while (i < x2) {
    if (sh->screen_range[i] == 0) {
      index_first_0 = i;
      while (i < x2 && sh->screen_range[i] == 0) {
        i++;
      }
      index_last_0 = i;
      if (index_last_0 - index_first_0 > 0) {
        draw_portal_walls_range(sh, index_first_0, index_last_0);
      }
    } else {
      i++;
    }
  }
}

void classify_segment(segment_handler *sh, segment *seg, int x1, int x2,
                      double raw_angle_1) {
  if (x1 == x2)
    return; // no pixel needed to be drawn
  sector *front_sector = seg->front_sector;
  sector *back_sector = seg->back_sector;
  sh->seg = seg;
  sh->raw_angle_1 = raw_angle_1;
  if (back_sector == NULL) { // its a solid wall
    clip_solid_walls(sh, x1, x2);
  } else if (front_sector->ceiling_height != back_sector->ceiling_height ||
             front_sector->floor_height !=
                 back_sector->floor_height) { // its a window
    clip_portal_walls(sh, x1, x2);
  } else if (!strcmp(back_sector->ceiling_texture,
                     front_sector->ceiling_texture) &&
             !strcmp(back_sector->floor_texture, front_sector->floor_texture) &&
             back_sector->light_level ==
                 front_sector->light_level) { // its a portal
    return;
  } else { // its a wall
    clip_portal_walls(sh, x1, x2);
  }
}

segment_handler *segment_handler_init(engine *e) {
  segment_handler *sh = malloc(sizeof(segment_handler));
  sh->engine = e;
  sh->seg =
      NULL; // init to NULL, will be changed each time by 'classify_segment'
  sh->screen_range_count = 0;
  sh->player = e->p;
  sh->raw_angle_1 = 0.0;
  for (int i = 0; i < WIDTH; i++) {
    sh->screen_range[i] = 0;
    sh->upper_clip[i] = -1;
    sh->lower_clip[i] = HEIGHT;
  }
  return sh;
}

void segment_handler_free(segment_handler *sh) { free(sh); }

void segment_handler_update(segment_handler *sh) {
  sh->screen_range_count = 0;
  for (int i = 0; i < WIDTH; i++) {
    sh->screen_range[i] = 0;
    sh->upper_clip[i] = -1;
    sh->lower_clip[i] = HEIGHT;
  }
}