#include <stdio.h>

#include "../include/segment_handler.h"
#include "../include/player.h"
#include "../include/component/position.h"

bool BSP_TRAVERSE = true;

// Given the x position of a pixel, the normal angle of the segment and the distance from the player to the wall
// Returns the scale of the texture column
double scale_from_global_angle(segment_handler *sh, int x, double normal_angle,
                               double dist) {
  position_ct *player_pos = player_get_position(sh->player);

  double x_angle = rad_to_deg(atan((HALF_WIDTH - x) / SCREEN_DISTANCE));
  double num =
      fabs(SCREEN_DISTANCE *
           cos(deg_to_rad(-normal_angle + x_angle - position_get_angle(player_pos))));
  double den = dist * cos(deg_to_rad(x_angle));
  double scale = num / den;
  scale = fmin(MAX_SCALE, fmax(MIN_SCALE, scale));
  return scale;
}

void draw_solid_walls_range(segment_handler *sh, int x1, int x2) {
  position_ct *player_pos = player_get_position(sh->player);
  segment *seg = sh->seg;
  sector *front_sector = seg->front_sector;
  linedef *ld = seg->linedef;
  sidedef *front_sidedef = seg->direction ? ld->back_sidedef : ld->front_sidedef;
  texture_map *wall_texture = front_sidedef->middle_texture;
  flat *ceiling_texture = front_sector->ceiling_texture;
  flat *floor_texture = front_sector->floor_texture;
  i16 light_level = front_sector->light_level;
  int world_front_z1 = (int)(front_sector->ceiling_height - position_get_z(player_pos));
  int world_front_z2 = (int)(front_sector->floor_height -position_get_z(player_pos));

  // check which parts need to be rendered
  bool draw_wall = front_sidedef->hash_middle != NO_TEXTURE_HASH; // in theory, it will NEVER be false, but lets make sure
  bool draw_ceiling = world_front_z1 > 0; // can we actually see the ceiling ?
  bool draw_floor = world_front_z2 < 0;   // can we actually see the floor ?

  // GET SCALING FACTORS FOR WALLS
  double normal_angle = sh->seg->angle + 90.0; // get normal angle of segment
  double offset_angle = normal_angle - sh->raw_angle_1; // get the offset angle between angle of start_vertex and player's angle

  vec2 start_vertex_pos = {.x = sh->seg->start_vertex->x,
                           .y = sh->seg->start_vertex->y};
  double hyp = dist(position_get_pos(player_pos),
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

  // manage texture vertical alignment
  double middle_texture_alt = world_front_z1;

  if (ld->flag & LOWER_UNPEGGED) {
    int v_top = front_sector->floor_height + wall_texture->height;
    middle_texture_alt = v_top - position_get_z(player_pos);
  }

  middle_texture_alt += front_sidedef->y_offset;

  // manage texture horizontal alignment
  double rw_offset = hyp * sin(deg_to_rad(offset_angle));
  rw_offset += seg->offset + front_sidedef->x_offset;

  double center_angle = norm(normal_angle + position_get_angle(player_pos));
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
      int cy1 = (int)(sh->upper_clip[i] + 1); // clipping with previously drawn walls
      int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
      draw_flat(sh->engine, ceiling_texture, light_level, i, cy1,
                cy2, world_front_z1);
    }

    if (draw_wall) {
      int wy1 =
          (int)fmax(draw_wall_y1, sh->upper_clip[i] + 1); // max(0, upper_clip)
      int wy2 = (int)fmin(draw_wall_y2,
                          sh->lower_clip[i] - 1); // min(HEIGHT, lower_clip)
      if (wy1 < wy2) {
        double angle =
            center_angle - rad_to_deg(atan((HALF_WIDTH - i) / SCREEN_DISTANCE));
        double texture_column = raw_dist * tan(deg_to_rad(angle)) - rw_offset; // which column of the texture are we drawing ? depends on the distance
        double inverted_scale = 1.0 / scale1;
        draw_wall_column(sh->engine, wall_texture, texture_column,
                         i, wy1, wy2, middle_texture_alt, inverted_scale,
                         light_level);
      }
    }

    if (draw_floor) {
      int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
      int fy2 = (int)(sh->lower_clip[i] - 1);
      draw_flat(sh->engine, floor_texture, light_level, i, fy1,
                fy2, world_front_z2);
    }
    wall_y1 += wall_y1_step;
    wall_y2 += wall_y2_step;
    scale1 += rw_scale_step;
  }
}

void draw_portal_walls_range(segment_handler *sh, int x1, int x2) {
  segment *seg = sh->seg;
  linedef *ld = seg->linedef;
  sector *front_sector = seg->front_sector;
  sector *back_sector = seg->back_sector;
  sidedef *front_sidedef = seg->direction ? seg->linedef->back_sidedef : seg->linedef->front_sidedef;

  texture_map *upper_wall_texture = front_sidedef->upper_texture;
  texture_map *lower_wall_texture = front_sidedef->lower_texture;
  flat *ceiling_texture = front_sector->ceiling_texture;
  flat *floor_texture = front_sector->floor_texture;
  i16 light_level = front_sector->light_level;

  position_ct *player_pos = player_get_position(sh->player);

  int world_front_z1 = (int)(front_sector->ceiling_height - player_pos->z);
  int world_front_z2 = (int)(front_sector->floor_height - player_pos->z);
  int world_back_z1 = (int)(back_sector->ceiling_height - player_pos->z);
  int world_back_z2 = (int)(back_sector->floor_height - player_pos->z);
  bool draw_upper_wall = false;
  bool draw_ceiling = false;
  if (world_front_z1 != world_back_z1 ||
      front_sector->light_level != back_sector->light_level ||
      front_sector->hash_ceiling != back_sector->hash_ceiling) {
    draw_upper_wall = front_sidedef->hash_upper != NO_TEXTURE_HASH &&
                      world_back_z1 < world_front_z1;
    draw_ceiling = world_front_z1 >= 0 || front_sector->hash_ceiling == SKY_TEXTURE_HASH;
  }

  bool draw_lower_wall = false;
  bool draw_floor = false;

  if (world_front_z2 != world_back_z2 ||
      front_sector->light_level != back_sector->light_level ||
      front_sector->hash_floor != back_sector->hash_floor) {
    draw_lower_wall = front_sidedef->hash_lower != NO_TEXTURE_HASH &&
                      world_back_z2 > world_front_z2;
    draw_floor = world_front_z2 <= 0;
  }

  if (!draw_upper_wall && !draw_lower_wall && !draw_ceiling &&
      !draw_floor) { // nothing to draw
    return;
  }

  // back to calculating scaling factors, the exact same way than before
  double normal_angle = sh->seg->angle + 90.0; // get normal angle of segment
  double offset_angle = normal_angle - sh->raw_angle_1;

  vec2 start_vertex_pos = {.x = sh->seg->start_vertex->x,
                           .y = sh->seg->start_vertex->y};
  double hyp = dist(position_get_pos(player_pos),
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

  // manage texture vertical alignment
  double upper_texture_alt = 0;
  if (draw_upper_wall) {
    if (ld->flag & UPPER_UNPEGGED) {
      upper_texture_alt = world_front_z1;
    } else {
      int v_top = back_sector->ceiling_height + upper_wall_texture->height;
      upper_texture_alt = v_top - player_pos->z;
    }
    upper_texture_alt += front_sidedef->y_offset;
  }

  double lower_texture_alt = 0;
  if (draw_lower_wall) {
    if (ld->flag & LOWER_UNPEGGED) {
      lower_texture_alt = world_front_z1;
    } else {
      lower_texture_alt = world_back_z2;
    }
    lower_texture_alt += front_sidedef->y_offset;
  }

  double rw_offset = 0;
  double center_angle = 0;

  // manage texture horizontal alignment
  bool seg_textured = draw_upper_wall || draw_lower_wall;
  if (seg_textured) {
    rw_offset = hyp * sin(deg_to_rad(offset_angle));
    rw_offset += seg->offset + front_sidedef->x_offset;
    center_angle = norm(normal_angle + player_pos->angle);
  }

  // Determine the y coordinates of the wall
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

  // change the y positions of the top / bottoms edges of the wall if needed
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

  // lets start actually rendering
  double angle = 0;
  double texture_column = 0;
  double inverted_scale = 1.0 / scale1;
  for (int i = x1; i < x2; i++) {
    double draw_wall_y1 = wall_y1 - 1;
    double draw_wall_y2 = wall_y2;

    if (seg_textured) { // update the angle for the texture column
      angle =
          center_angle - rad_to_deg(atan((HALF_WIDTH - i) / SCREEN_DISTANCE));
      texture_column = raw_dist * tan(deg_to_rad(angle)) - rw_offset;
      inverted_scale = 1 / scale1;
    }

    if (draw_upper_wall) {
      double draw_upper_wall_y1 = wall_y1 - 1;
      double draw_upper_wall_y2 = portal_y1;

      if (draw_ceiling) {
        int cy1 = (int)(sh->upper_clip[i] + 1);
        int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
        draw_flat(sh->engine, ceiling_texture, light_level, i,
                  cy1, cy2, world_front_z1);
      }

      int wy1 = (int)fmax(draw_upper_wall_y1, sh->upper_clip[i] + 1);
      int wy2 = (int)fmin(draw_upper_wall_y2, sh->lower_clip[i] - 1);
      draw_wall_column(sh->engine, upper_wall_texture,
                       texture_column, i, wy1, wy2, upper_texture_alt,
                       inverted_scale, light_level);
      if (sh->upper_clip[i] < wy2) {
        sh->upper_clip[i] = wy2;
      }
      portal_y1 += portal_y1_step;
    }

    if (draw_ceiling) {
      int cy1 = (int)(sh->upper_clip[i] + 1);
      int cy2 = (int)fmin(draw_wall_y1 - 1, sh->lower_clip[i] - 1);
      draw_flat(sh->engine, ceiling_texture, light_level, i, cy1,
                cy2, world_front_z1);
      if (sh->upper_clip[i] < cy2) {
        sh->upper_clip[i] = cy2;
      }
    }

    if (draw_lower_wall) {
      if (draw_floor) {
        int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
        int fy2 = (int)(sh->lower_clip[i] - 1);
        draw_flat(sh->engine, floor_texture, light_level, i, fy1,
                  fy2, world_front_z2);
      }
      int draw_lower_wall_y1 = portal_y2 - 1;
      int draw_lower_wall_y2 = wall_y2;
      int wy1 = (int)fmax(draw_lower_wall_y1, sh->upper_clip[i] + 1);
      int wy2 = (int)fmin(draw_lower_wall_y2, sh->lower_clip[i] - 1);
      draw_wall_column(sh->engine, lower_wall_texture,
                       texture_column, i, wy1, wy2, lower_texture_alt,
                       inverted_scale, light_level);
      if (sh->lower_clip[i] > wy1) {
        sh->lower_clip[i] = wy1;
      }
      portal_y2 += portal_y2_step;
    }

    if (draw_floor) {
      int fy1 = (int)fmax(draw_wall_y2 + 1, sh->upper_clip[i] + 1);
      int fy2 = (int)(sh->lower_clip[i] - 1);
      draw_flat(sh->engine, floor_texture, light_level, i, fy1,
                fy2, world_front_z2);

      if (sh->lower_clip[i] > draw_wall_y2)
        sh->lower_clip[i] = fy1;
    }
    wall_y1 += wall_y1_step; // we're working with linear interpolation, don't forget to update the y_1 and y_2 positions
    wall_y2 += wall_y2_step;
    scale1 += rw_scale_step;
  }
}

// Given x1 and x2, clip the solid wall with what is already existing on the screen
// Updates the screen range to avoid overdraws
void clip_solid_walls(segment_handler *sh, int x1, int x2) {
  if (sh->screen_range_count <
      WIDTH) { // the screen is not fully occupied by walls
    int index_first_0 = x1;
    int index_last_0 = x2;
    int i = x1;
    while (i < x2) {
      if (sh->screen_range[i] == 0) {
        index_first_0 = i; // we found a range that is not drawn yet
        while (i < x2 && sh->screen_range[i] == 0) {
          i++;
        }
        index_last_0 = i; // we can draw from index_first_0 to index_last_0
        if (index_last_0 - index_first_0 > 0) { // if there is something to draw
          draw_solid_walls_range(sh, index_first_0, index_last_0);
          sh->screen_range_count += index_last_0 - index_first_0;
        }
      } else {
        i++; // screen is already occupied here
      }
    }
    for (int i = x1; i < x2; i++) {
      sh->screen_range[i] = 1;
    }
  } else { // we've already drawn every solid wall on our screen, stop there
    BSP_TRAVERSE = false;
  }
}

// Given x1 and x2, clip the portal wall with what is already existing on the screen
// DOES NOT UPDATE THE SCREEN RANGE
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

// Classifies segments in three differents categories : solid walls, walls with windows and actual portal windows
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
  } else if (front_sector->hash_ceiling == back_sector->hash_ceiling &&
             front_sector->hash_floor == back_sector->hash_floor &&
             back_sector->light_level == front_sector->light_level &&
             seg->linedef->front_sidedef->hash_middle ==
                 NO_TEXTURE_HASH) { // its a portal
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