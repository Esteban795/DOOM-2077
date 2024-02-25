#include "../include/bsp.h"

#define M_PI 3.14159265358979323846
#define FOV 90.0
#define HALF_FOV (FOV / 2.0)

double rad_to_deg(double rad) { return rad * (180 / M_PI); }

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

float norm(double angle) { return fmod((angle + 360.0), 360.0); }

double point_to_angle(vec2 p1, vec2 p2) {
  vec2 delta = {p2.x - p1.x, p2.y - p1.y};
  return rad_to_deg(atan2(delta.y, delta.x));
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
  double x = p->x;
  double y = p->y;
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
  vec2 player_pos = {.x = p->x, .y = p->y};
  for (size_t i = 0; i < 2 * counter; i += 2) {
    double angle1 = point_to_angle(player_pos, possibly_visible_bb_sides[i]);
    double angle2 =
        point_to_angle(player_pos, possibly_visible_bb_sides[i + 1]);
    double span = norm(angle1 - angle2);
    angle1 += p->angle;
    double span1 = norm(angle1 + HALF_FOV);
    if (span1 > FOV) {
      if (span1 >= span + FOV)
        continue;
    }
    return true;
  }
  return false;
}

vertex get_vertex_from_segment(engine* e,i16 vertex_id){
  return e->wData->vertexes[vertex_id];
}

bool is_segment_in_fov(player* p,segment seg){
  vec2 player = {.x = p->x, .y = p->y};
  vertex v1 = get_vertex_from_segment(p->engine, seg.start_vertex_id);
  vertex v2 = get_vertex_from_segment(p->engine, seg.end_vertex_id);
  vec2 v1v = {.x = v1.x, .y = v1.y};
  vec2 v2v = {.x = v2.x, .y = v2.y};
  double angle1 = point_to_angle(player, v1v);
  double angle2 = point_to_angle(player, v2v);
  double span = norm(angle1 - angle2);
  bool res = span >= 180.0 ? false : true;
  if (res) {
    angle1 += p->angle;
    double span1 = norm(angle1 + HALF_FOV);
    if (span1 > FOV) {
      if (span1 >= span + FOV)
        return false;
    }
  }
  return res;
}

static bool is_on_back_side(bsp *b, node n) {
  i16 dx = b->player->x - n.x_partition;
  i16 dy = b->player->y - n.y_partition;
  return dx * n.dy_partition - dy * n.dx_partition <= 0;
}

void render_bsp_node(bsp *b, size_t node_id) {
  if (node_id > SUBSECTOR_IDENTIFIER) {
    i16 subsector_id = node_id - SUBSECTOR_IDENTIFIER;
    subsector ss = b->engine->wData->subsectors[subsector_id];
    SDL_SetRenderDrawColor(b->engine->map_renderer->renderer, 0, 255, 0, 255);
    for (i16 i = 0; i < ss.num_segs; i++) {
      segment seg = b->engine->wData->segments[ss.first_seg_id + i];
      if (is_segment_in_fov(b->player, seg)) {
        draw_segment(b->engine->map_renderer, seg);
      }
    }
  } else {
    node n = b->nodes[node_id];
    bool is_back_side = is_on_back_side(b, n);
    if (is_back_side) {
      render_bsp_node(b, n.back_child_id);
      if (check_if_bbox_visible(n.front_bbox, b->player)) {
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

void update_bsp(bsp *b) { render_bsp_node(b, b->root_node_id); }

void bsp_free(bsp *b) { free(b); }