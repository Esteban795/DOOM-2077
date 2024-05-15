#include "../include/bsp.h"

bsp *bsp_init(engine *e, player *p) {
  bsp *b = malloc(sizeof(bsp));
  b->engine = e;
  b->player = p;
  b->nodes = e->wData->nodes;
  b->subsectors = e->wData->subsectors;
  b->segments = e->wData->segments;
  b->root_node_id = e->wData->len_nodes - 1;
  return b;
}

/*
b**********c
*          *
*          *
*          *
a**********d

Consider 8 cases where the box could be visivle from the player's perspective :
above, below, left, right, top-left, top-right, bottom-left, bottom-right

You can have at most two sides visible at the same time, so one face is
possibly_visible_bb_sides[0], possibly_visible_bb_sides[1] and the other face is
possibly_visible_bb_sides[2], possibly_visible_bb_sides[3]
*/
static bool check_if_bbox_visible(bbox bb, player *p) {
  vec2 a = {.x = bb.left, .y = bb.bottom};
  vec2 b = {.x = bb.left, .y = bb.top};
  vec2 c = {.x = bb.right, .y = bb.top};
  vec2 d = {.x = bb.right, .y = bb.bottom};
  vec2 possibly_visible_bb_sides[4];
  double x = p->pos.x;
  double y = p->pos.y;
  size_t counter = 0;
  if (x < bb.left) {
    if (y > bb.top) {
      possibly_visible_bb_sides[0] = b;
      possibly_visible_bb_sides[1] = a;
      possibly_visible_bb_sides[2] = c;
      possibly_visible_bb_sides[3] = b;
      counter = 2;
    } else if (y < bb.bottom) {
      possibly_visible_bb_sides[0] = b;
      possibly_visible_bb_sides[1] = a;
      possibly_visible_bb_sides[2] = a;
      possibly_visible_bb_sides[3] = d;
      counter = 2;
    } else {
      possibly_visible_bb_sides[0] = b;
      possibly_visible_bb_sides[1] = a;
      counter = 1;
    }
  } else if (x > bb.right) {
    if (y > bb.top) {
      possibly_visible_bb_sides[0] = c;
      possibly_visible_bb_sides[1] = b;
      possibly_visible_bb_sides[2] = d;
      possibly_visible_bb_sides[3] = c;
      counter = 2;
    } else if (y < bb.bottom) {
      possibly_visible_bb_sides[0] = a;
      possibly_visible_bb_sides[1] = d;
      possibly_visible_bb_sides[2] = d;
      possibly_visible_bb_sides[3] = c;
      counter = 2;
    } else {
      possibly_visible_bb_sides[0] = d;
      possibly_visible_bb_sides[1] = c;
      counter = 1;
    }
  } else {
    if (y > bb.top) {
      possibly_visible_bb_sides[0] = c;
      possibly_visible_bb_sides[1] = b;
      counter = 1;
    } else if (y < bb.bottom) {
      possibly_visible_bb_sides[0] = a;
      possibly_visible_bb_sides[1] = d;
      counter = 1;
    } else
      return true; // we are inside the box
  }
  vec2 player_pos = p->pos;
  for (size_t i = 0; i < 2 * counter; i += 2) {
    double angle1 = point_to_angle(player_pos, possibly_visible_bb_sides[i]); // angle between player and point 1 of face i
    double angle2 =
        point_to_angle(player_pos, possibly_visible_bb_sides[i + 1]); // angle between player and point 2 of face i
    double span = norm(angle1 - angle2);
    angle1 += p->angle;
    double span1 = norm(angle1 + HALF_FOV); // determines if point 1 is in FOV
    if (span1 > FOV) {
      if (span1 + 0.1 >= span + FOV)
        continue;
    }
    return true;
  }
  return false;
}

// returns true if the segment is in the player's field of view, and sets x1 and
// x2 to the x position of the segment for the screen
bool is_segment_in_fov(player *p, segment seg, int *x1, int *x2,
                       double *raw_angle_1) {
  vertex *v1 = seg.start_vertex;
  vertex *v2 = seg.end_vertex;
  vec2 v1v = {.x = v1->x, .y = v1->y};
  vec2 v2v = {.x = v2->x, .y = v2->y};
  double angle1 = point_to_angle(p->pos, v1v); // angle from player to v1
  double angle2 = point_to_angle(p->pos, v2v); // angle from player to v2
  double span = norm(angle1 - angle2);
  if (span >= 180.0) // segment is not facing us
    return false;
  *raw_angle_1 = angle1; // save the angle for later use when drawing
  angle1 += p->angle; // total angle from origin to v1, don't forget y-axis is reversed
  angle2 += p->angle; // total angle from origin to v2,don't forget y-axis is reversed
  double span1 = norm(HALF_FOV + angle1);
  if (span1 > FOV) {
    if (span1 + 0.1 >= span + FOV)
      return false;
    else
      angle1 = HALF_FOV; // clipping if part of the segment is outside the FOV
  }
  double span2 = norm(HALF_FOV - angle2);
  if (span2 > FOV) {
    if (span2 + 0.2 >= span + FOV)
      return false;
    else
      angle2 = -HALF_FOV;
  }
  *x1 = angle_to_x_pos(angle1); // get the screen x coordinates for v_start and v_end after projection
  *x2 = angle_to_x_pos(angle2);
  return true;
}

// determines if a player is on the backside of a node
static bool is_on_back_side(bsp *b, node n) { 
  i16 dx = b->player->pos.x - n.x_partition;
  i16 dy = b->player->pos.y - n.y_partition;
  return dx * n.dy_partition - dy * n.dx_partition <= 0;
}

// BSP traveral algorithm
void render_bsp_node(bsp *b, size_t node_id) {
  if (BSP_TRAVERSE) {
    if (node_id >= SUBSECTOR_IDENTIFIER) {
      i16 subsector_id = node_id - SUBSECTOR_IDENTIFIER;
      subsector ss = b->subsectors[subsector_id];
      int x1, x2;
      double raw_angle_1;
      for (i16 i = 0; i < ss.num_segs; i++) {
        segment seg = ss.segs[i];
        if (is_segment_in_fov(b->player, seg, &x1, &x2, &raw_angle_1)) {
          classify_segment(b->engine->seg_handler, &seg, x1, x2, raw_angle_1);
        }
      }
    } else {
      node n = b->nodes[node_id];
      bool is_back_side = is_on_back_side(b, n);
      if (is_back_side) {
        render_bsp_node(b, n.back_child_id);
        if (check_if_bbox_visible(n.front_bbox, b->player)) { // avoid exploring branches where BB is not visible
          render_bsp_node(b, n.front_child_id);
        }
      } else {
        render_bsp_node(b, n.front_child_id);
        if (check_if_bbox_visible(n.back_bbox, b->player)) {
          render_bsp_node(b, n.back_child_id);
        }
      }
    }
  }
}


void get_ssector_height(bsp* b){
  size_t node_id = b->root_node_id;
  while (node_id < SUBSECTOR_IDENTIFIER) {
    node n = b->nodes[node_id];
    bool is_back_side = is_on_back_side(b, n);
    if (is_back_side) {
      node_id = n.back_child_id;
    } else {
      node_id = n.front_child_id;
    }
  }
  i16 subsector_id = node_id - SUBSECTOR_IDENTIFIER;
  subsector player_ssector = b->subsectors[subsector_id];
  segment seg = player_ssector.segs[0];
  double floor_height = seg.front_sector->floor_height;
  update_height(b->engine->p, floor_height);
}

void update_bsp(bsp *b) {
  BSP_TRAVERSE = true;
  render_bsp_node(b, b->root_node_id);
}

void bsp_free(bsp *b) { free(b); }
