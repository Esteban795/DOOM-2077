#include "../include/segment_handler.h"

bool BSP_TRAVERSE = true;

color get_color(char *texture, i16 light_level) {
  srand(light_level & texture[0]);
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
  scale = min(MAX_SCALE, max(MIN_SCALE, scale));
  return scale;
}

void draw_solid_walls_range(segment_handler *sh, int x1, int x2) {
  // will be used soon while applying textures on walls
  char *wall_texture = sh->seg->linedef->front_sidedef->middle_texture;
  // char* ceiling_texture = sh->seg->front_sector->ceiling_texture;
  // char* floor_texture = sh->seg->front_sector->floor_texture;
  i16 light_level = sh->seg->front_sector->light_level;

  int world_front_z1 = sh->seg->front_sector->ceiling_height - sh->player->height;
  int world_front_z2 = sh->seg->front_sector->floor_height - sh->player->height;
  // check which parts needs to be rendered
  bool draw_wall = strcmp(sh->seg->linedef->front_sidedef->middle_texture, "-");
  // bool draw_ceiling = world_front_z1 > 0; //can we actually see the ceiling ?
  // bool draw_floor = world_front_z2 < 0; // can we actually see the floor ?

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
      if (draw_wall) {
        color c = get_color(wall_texture, light_level);
        int wy1 = wall_y1;
        int wy2 = wall_y2;
        draw_vline(sh->engine->map_renderer, i, wy1, wy2, c);
      }
    wall_y1 += wall_y1_step;
    wall_y2 += wall_y2_step;
  }
}

void draw_portal_walls_range(segment_handler *sh, int x1, int x2) { return; }

int *calculate_ranges_to_draw(int *screen_range, int x1, int x2) {
  int len = x2 - x1;
  int *ranges_to_draw = malloc(sizeof(int) * len);
  for (int i = 0; i < len; i++) {
    ranges_to_draw[i] = screen_range[x1 + i] ^ 1;
  }
  return ranges_to_draw;
}

void clip_solid_walls(segment_handler *sh, int x1, int x2) {
  if (sh->screen_range_count <
      WIDTH) { // the screen is not fully occupied by walls
    int len = x2 - x1;
    int *ranges_to_draw = calculate_ranges_to_draw(sh->screen_range, x1, x2);
    int index_first_1 = -1;
    int index_last_1 = -1;
    for (int i = 0; i < len; i++) {
      if (ranges_to_draw[i] == 1) {
        if (index_first_1 == -1) {
          index_first_1 = i;
        }
        index_last_1 = i;
        sh->screen_range[x1 + i] = 1; // mark the pixel as drawn
      } else if (index_first_1 != -1) {
        sh->screen_range_count += index_last_1 - index_first_1 + 1;
        draw_solid_walls_range(sh, x1 + index_first_1, x1 + index_last_1 + 1);
        index_first_1 = -1;
        index_last_1 = -1;
      }
    }
    if (index_first_1 !=
        -1) { // we've reached the end of the loop but we started a chunk of 1s
      sh->screen_range_count += index_last_1 - index_first_1 + 1;
      draw_solid_walls_range(sh, x1 + index_first_1, x1 + index_last_1 + 1);
    }
    free(ranges_to_draw);
  } else { // we've already drawn every wall on our screen, stop there
    BSP_TRAVERSE = false;
  }
}

void clip_portal_walls(segment_handler *sh, int x1, int x2) {
  int len = x2 - x1;
  int *ranges_to_draw = calculate_ranges_to_draw(sh->screen_range, x1, x2);
  int index_first_1 = -1;
  int index_last_1 = -1;
  for (int i = 0; i < len; i++) {
    if (ranges_to_draw[i] == 1) {
      if (index_first_1 == -1) {
        index_first_1 = i;
      }
      index_last_1 = i;
    } else if (index_first_1 != -1) {
      draw_portal_walls_range(sh, x1 + index_first_1, x1 + index_last_1 + 1);
      index_first_1 = -1;
      index_last_1 = -1;
    }
  }
  if (index_first_1 != -1) { // the end of the loop but we started a chunk of 1s
    draw_portal_walls_range(sh, x1 + index_first_1, x1 + index_last_1 + 1);
  }
  free(ranges_to_draw);
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
    // clip_portal_walls(sh, x1, x2);
  } else if (!strcmp(back_sector->ceiling_texture,
                     front_sector->ceiling_texture) &&
             !strcmp(back_sector->floor_texture, front_sector->floor_texture) &&
             back_sector->light_level ==
                 front_sector->light_level) { // its a portal
    return;
  } else { // its a wall
    // clip_portal_walls(sh, x1, x2);
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
  memset(sh->screen_range, 0, WIDTH * sizeof(int));
  memset(sh->upper_clip, 0, WIDTH * sizeof(int));
  memset(sh->lower_clip, 0, WIDTH * sizeof(int));
  return sh;
}

void segment_handler_free(segment_handler *sh) { free(sh); }

void segment_handler_update(segment_handler *sh) {
  memset(sh->screen_range, 0, WIDTH * sizeof(int)); // reset screen range
  sh->screen_range_count = 0;
  memset(sh->upper_clip, -1, WIDTH * sizeof(int));     // reset upper clip
  memset(sh->lower_clip, HEIGHT, WIDTH * sizeof(int)); // reset lower clip
}