#include "../include/map_renderer.h"
#include <SDL2/SDL_render.h>

#define FOV 90.0
#define H_FOV (FOV / 2.0)

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

// code stolen from the internet, I just needed to draw a circle for the
// vertexes.
static void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY,
                       int32_t radius) {
  const int32_t diameter = (radius * 2);
  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);
  while (x >= y) {
    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }
    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

/*
map_bounds[0] = min_x
map_bounds[1] = max_x
map_bounds[2] = min_y
map_bounds[3] = max_y
*/
int *get_map_bounds(vertex *vertexes, int len) {
  int *map_bounds = malloc(sizeof(int) * 4);
  map_bounds[0] = vertexes[0].x;
  map_bounds[1] = vertexes[0].x;
  map_bounds[2] = vertexes[0].y;
  map_bounds[3] = vertexes[0].y;
  for (int i = 1; i < len; i++) {
    if (vertexes[i].x < map_bounds[0]) {
      map_bounds[0] = vertexes[i].x;
    } else if (vertexes[i].x > map_bounds[1]) {
      map_bounds[1] = vertexes[i].x;
    }
    if (vertexes[i].y < map_bounds[2]) {
      map_bounds[2] = vertexes[i].y;
    }
    if (vertexes[i].y > map_bounds[3]) {
      map_bounds[3] = vertexes[i].y;
    }
  }
  return map_bounds;
}

// remap to keep proportions
int remap_x(int current_x, int x_min, int x_max) {
  return (max(x_min, min(current_x, x_max)) - x_min) * (OUT_MAX_W - OUT_MIN) /
             (x_max - x_min) +
         OUT_MIN;
}

int remap_y(int current_y, int y_min, int y_max) {
  return HEIGHT -
         (max(y_min, min(current_y, y_max)) - y_min) * (OUT_MAX_H - OUT_MIN) /
             (y_max - y_min) -
         OUT_MIN;
}

vertex *remap_vertexes(vertex *vertexes, int len, int *map_bounds) {
  vertex *remapped_vertexes = malloc(sizeof(vertex) * len);
  for (int i = 0; i < len; i++) {
    remapped_vertexes[i].x =
        remap_x(vertexes[i].x, map_bounds[0], map_bounds[1]);
    remapped_vertexes[i].y =
        remap_y(vertexes[i].y, map_bounds[2], map_bounds[3]);
  }
  return remapped_vertexes;
}

void draw_linedefs(SDL_Renderer *renderer, linedef *linedefs, int len,
                   vertex *vertexes) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < len; i++) {
    vertex p1 = vertexes[linedefs[i].start_vertex_id];
    vertex p2 = vertexes[linedefs[i].end_vertex_id];
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
  }
}

// static void draw_vertexes(SDL_Renderer *renderer, vertex *vertexes, int len)
// {
//   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//   for (int i = 0; i < len; i++) {
//     DrawCircle(renderer, vertexes[i].x, vertexes[i].y, 5);
//   }
// }

static void draw_bbox(map_renderer *mr, bbox b, color c) {
  SDL_SetRenderDrawColor(mr->renderer, c.r, c.g, c.b, 255);
  i16 x_min = mr->map_bounds.left;
  i16 x_max = mr->map_bounds.right;
  i16 y_min = mr->map_bounds.top;
  i16 y_max = mr->map_bounds.bottom;
  int x = remap_x(b.left, x_min, x_max);
  int y = remap_y(b.top, y_min, y_max);
  int w = remap_x(b.right, x_min, x_max) - x;
  int h = remap_y(b.bottom, y_min, y_max) - y;
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};
  SDL_RenderDrawRect(mr->renderer, &rect);
}

void draw_node(map_renderer *mr, int node_id) {
  node n = mr->engine->wData->nodes[node_id];
  bbox bbox_front = n.front_bbox;
  bbox bbox_back = n.back_bbox;
  draw_bbox(mr, bbox_front, (color){0, 0, 255});
  draw_bbox(mr, bbox_back, (color){0, 255, 0});
  i16 x1 = remap_x(n.x_partition, mr->map_bounds.left, mr->map_bounds.right);
  i16 y1 = remap_y(n.y_partition, mr->map_bounds.top, mr->map_bounds.bottom);
  i16 x2 = remap_x(n.x_partition + n.dx_partition, mr->map_bounds.left,
                   mr->map_bounds.right);
  i16 y2 = remap_y(n.y_partition + n.dy_partition, mr->map_bounds.top,
                   mr->map_bounds.bottom);
  SDL_RenderDrawLine(mr->renderer, x1, y1, x2, y2);
}

static void draw_player(map_renderer *mr) {
  SDL_SetRenderDrawColor(mr->renderer, 0, 0, 255, 255);
  i16 x = remap_x(mr->engine->p->x, mr->map_bounds.left, mr->map_bounds.right);
  i16 y = remap_y(mr->engine->p->y, mr->map_bounds.top, mr->map_bounds.bottom);
  DrawCircle(mr->renderer, x, y, 5);
}

void draw_segment(map_renderer *mr, segment seg) {
  vertex v_start = mr->vertexes[seg.start_vertex_id];
  vertex v_end = mr->vertexes[seg.end_vertex_id];
  SDL_RenderDrawLine(mr->renderer, v_start.x, v_start.y, v_end.x, v_end.y);
}

void draw_subsector(map_renderer *mr, i16 subsector_id) {
  subsector ss = mr->wData->subsectors[subsector_id];
  SDL_SetRenderDrawColor(mr->renderer, 0, 255, 0, 255);
  for (i16 i = 0; i < ss.num_segs; i++) {
    segment seg = mr->wData->segments[ss.first_seg_id + i];
    draw_segment(mr, seg);
  }
}

void draw_fov(map_renderer *mr) {
  const int RAY_LENGTH = 200;
  int x = remap_x(mr->engine->p->x, mr->map_bounds.left, mr->map_bounds.right);
  int y = remap_y(mr->engine->p->y, mr->map_bounds.top, mr->map_bounds.bottom);
  int x1 = x + RAY_LENGTH * cos(deg_to_rad(mr->engine->p->angle + H_FOV));
  int y1 = y + RAY_LENGTH * sin(deg_to_rad(mr->engine->p->angle + H_FOV));
  int x2 = x + RAY_LENGTH * cos(deg_to_rad(mr->engine->p->angle - H_FOV));
  int y2 = y + RAY_LENGTH * sin(deg_to_rad(mr->engine->p->angle - H_FOV));
  int x3 = x + RAY_LENGTH * cos(deg_to_rad(mr->engine->p->angle));
  int y3 = y + RAY_LENGTH * sin(deg_to_rad(mr->engine->p->angle));
  SDL_SetRenderDrawColor(mr->renderer, 0, 120, 255, 255);
  SDL_RenderDrawLine(mr->renderer, x, y, x1, y1);
  SDL_RenderDrawLine(mr->renderer, x, y, x2, y2);
  SDL_SetRenderDrawColor(mr->renderer, 255, 0, 0, 255);
  SDL_RenderDrawLine(mr->renderer, x, y, x3, y3);
}

void draw(map_renderer *mr) {
  // draw_vertexes(mr->renderer, mr->vertexes, mr->wData->len_vertexes);
  // draw_linedefs(mr->renderer, mr->wData->linedefs, mr->wData->len_linedefs,
  // mr->vertexes);
  draw_player(mr);
  draw_fov(mr);
}

map_renderer *map_renderer_init(engine *e, SDL_Renderer *renderer) {
  map_renderer *mr = malloc(sizeof(map_renderer));
  mr->engine = e;
  mr->wData = e->wData;
  int *bounds = get_map_bounds(mr->wData->vertexes, mr->wData->len_vertexes);
  mr->vertexes =
      remap_vertexes(mr->wData->vertexes, mr->wData->len_vertexes, bounds);
  mr->linedefs = mr->wData->linedefs;
  mr->map_bounds = (bbox){.top = bounds[2],
                          .bottom = bounds[3],
                          .left = bounds[0],
                          .right = bounds[1]};
  mr->renderer = renderer;
  free(bounds);
  return mr;
}

void map_renderer_free(map_renderer *mr) {
  free(mr->vertexes);
  SDL_DestroyRenderer(mr->renderer);
  free(mr);
}