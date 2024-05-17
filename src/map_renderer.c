#include "../include/map_renderer.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <limits.h>
#include <stdio.h>

#define FOV 90.0
#define H_FOV (FOV / 2.0)

SDL_PixelFormat *fmt = NULL;

static bool is_on_back_side(node n, vec2 pos) {
  i16 dx = pos.x - n.x_partition;
  i16 dy = pos.y - n.y_partition;
  return dx * n.dy_partition - dy * n.dx_partition <= 0;
}

double get_xy_floor_height(bsp *b, vec2 pos) {
  size_t node_id = b->root_node_id;
  while (node_id < SUBSECTOR_IDENTIFIER) {
    node n = b->nodes[node_id];
    bool is_back_side = is_on_back_side(n, pos);
    if (is_back_side) {
      node_id = n.back_child_id;
    } else {
      node_id = n.front_child_id;
    }
  }
  i16 subsector_id = node_id - SUBSECTOR_IDENTIFIER;
  subsector ssector = b->subsectors[subsector_id];
  segment seg = ssector.segs[0];
  return seg.front_sector->floor_height;
}

color get_random_color2() {
  color c;
  c.r = rand() % 256;
  c.g = rand() % 256;
  c.b = rand() % 256;
  return c;
}

color get_random_color(i16 seed) {
  srand(seed);
  return (color){.r = rand() % 255, .g = rand() % 255, .b = rand() % 255};
}

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
  return (fmax(x_min, fmax(current_x, x_max)) - x_min) * (OUT_MAX_W - OUT_MIN) /
             (x_max - x_min) +
         OUT_MIN;
}

int remap_y(int current_y, int y_min, int y_max) {
  return HEIGHT -
         (fmax(y_min, fmin(current_y, y_max)) - y_min) * (OUT_MAX_H - OUT_MIN) /
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

void draw_linedefs(SDL_Renderer *renderer, linedef *linedefs, int len) {
  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < len; i++) {
    vertex *p1 = linedefs[i].start_vertex;
    vertex *p2 = linedefs[i].end_vertex;
    SDL_RenderDrawLine(renderer, p1->x, p1->y, p2->x, p2->y);
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
  draw_bbox(mr, bbox_front, (color){0, 0, 255, 255});
  draw_bbox(mr, bbox_back, (color){0, 255, 0, 255});
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
  i16 x =
      remap_x(mr->engine->p->pos.x, mr->map_bounds.left, mr->map_bounds.right);
  i16 y =
      remap_y(mr->engine->p->pos.y, mr->map_bounds.top, mr->map_bounds.bottom);
  DrawCircle(mr->renderer, x, y, PLAYER_RADIUS);
}

void draw_segment(map_renderer *mr, segment seg) {
  vertex *v_start = seg.start_vertex;
  vertex *v_end = seg.end_vertex;
  SDL_RenderDrawLine(mr->renderer, v_start->x, v_start->y, v_end->x, v_end->y);
}

void draw_subsector(map_renderer *mr, i16 subsector_id) {
  subsector ss = mr->wData->subsectors[subsector_id];
  SDL_SetRenderDrawColor(mr->renderer, 0, 255, 0, 255);
  for (i16 i = 0; i < ss.num_segs; i++) {
    segment seg = ss.segs[i];
    draw_segment(mr, seg);
  }
}

void draw_fov(map_renderer *mr) {
  const int RAY_LENGTH = 200;
  int x =
      remap_x(mr->engine->p->pos.x, mr->map_bounds.left, mr->map_bounds.right);
  int y =
      remap_y(mr->engine->p->pos.y, mr->map_bounds.top, mr->map_bounds.bottom);
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

void draw_block(map_renderer *mr, int block_index) {
  // routine that just draws the box of the block
  SDL_SetRenderDrawColor(mr->renderer, 255, 255, 255, 255);
  int block_bound_tl_x =
      mr->engine->wData->blockmap->header->x +
      (block_index % mr->engine->wData->blockmap->header->ncols) * 128;
  int block_bound_tl_y =
      mr->engine->wData->blockmap->header->y +
      (block_index / mr->engine->wData->blockmap->header->ncols) * 128;

  int x1 = remap_x(block_bound_tl_x, mr->map_bounds.left, mr->map_bounds.right);
  int y1 = remap_y(block_bound_tl_y, mr->map_bounds.top, mr->map_bounds.bottom);
  int x2 = remap_x(block_bound_tl_x + 128, mr->map_bounds.left,
                   mr->map_bounds.right);
  int y2 = remap_y(block_bound_tl_y + 128, mr->map_bounds.top,
                   mr->map_bounds.bottom);
  SDL_Rect rect = {.x = x1, .y = y1, .w = x2 - x1, .h = y2 - y1};
  SDL_RenderDrawRect(mr->renderer, &rect);

  // and now drawing the linedefs, let's go baby
  SDL_SetRenderDrawColor(mr->renderer, 255, 0, 255, 255);
  draw_linedefs(mr->renderer,
                mr->engine->wData->blockmap->blocks[block_index].linedefs,
                mr->engine->wData->blockmap->blocks[block_index].nlinedefs);
}

void draw_active_blocks(map_renderer *mr) {
  SDL_SetRenderDrawColor(mr->renderer, 255, 255, 255, 255);

  for (int y = -1; y <= 1; y++) {
    for (int x = -1; x <= 1; x++) {
      int block_index = blockmap_get_block_index(
          mr->engine->wData->blockmap, mr->engine->p->pos.x + x * 128,
          mr->engine->p->pos.y + y * 128);
      draw_block(mr, block_index);
    }
  }
}

void draw(map_renderer *mr) {
  // draw_vertexes(mr->renderer, mr->vertexes, mr->wData->len_vertexes);
  // draw_linedefs(mr->renderer, mr->wData->linedefs, mr->wData->len_linedefs,
  // mr->vertexes);
  draw_active_blocks(mr);
  draw_player(mr);
  draw_fov(mr);
}

map_renderer *map_renderer_init(engine *e, SDL_Renderer *renderer) {
  fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
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

void draw_vline(map_renderer *mr, int x, int y1, int y2, color c) {
  if (y1 < y2) {
    Uint32 pixel = SDL_MapRGBA(fmt, c.r, c.g, c.b, 255);
    for (int y = y1; y < y2; y++) {
      mr->engine->pixels[y * WIDTH + x] = pixel;
    }
  }
}

void draw_column(map_renderer *mr, int x, int y1, int y2, color c) {
  SDL_SetRenderDrawColor(mr->renderer, c.r, c.g, c.b, 255);
  SDL_RenderDrawLine(mr->renderer, x, y1, x, y2);
}

Uint32 get_this_fucking_pixel(Uint32 *pixels, int width, int x, int y) {
  return pixels[y * width + x];
}

void draw_wall_column(map_renderer *mr, texture_map *texture,
                      double texture_column, int x, int y1, int y2,
                      double texture_alt, double inverted_scale,
                      i16 light_level) {
  if (y1 < y2) {
    int texture_width = texture->width;
    int texture_height = texture->height;
    int texture_column_int = (int)mod(texture_column, (double)texture_width);
    double texture_y =
        texture_alt + ((double)y1 - HALF_HEIGHT) * inverted_scale;
    for (int y = y1; y < y2; y++) {
      int texture_y_int = (int)mod(texture_y, (double)texture_height);
      Uint32 pixel =
          texture->pixels[texture_y_int * texture_width + texture_column_int];
      u8 r, g, b, a;
      SDL_GetRGBA(pixel, texture->format, &r, &g, &b, &a);
      r = r * light_level / 255;
      g = g * light_level / 255;
      b = b * light_level / 255;
      Uint32 light_adjusted_pixel = SDL_MapRGBA(fmt, r, g, b, a);
      mr->engine->pixels[y * WIDTH + x] = light_adjusted_pixel;
      texture_y += inverted_scale;
    }
  }
}

void draw_flat(map_renderer *mr, flat *texture, i16 light_level, int x, int y1,
               int y2, int world_z) {
  if (y1 < y2) {
    double player_dir_x = cos(deg_to_rad(mr->engine->p->angle));
    double player_dir_y = -sin(deg_to_rad(
        mr->engine->p->angle)); // - because the fucking y axis is reversed
    for (int iy = y1; iy < y2; iy++) {
      double z = HALF_WIDTH * world_z / (HALF_HEIGHT - iy);
      double px = player_dir_x * z + mr->engine->p->pos.x;
      double py = player_dir_y * z + mr->engine->p->pos.y;
      double left_x = -player_dir_y * z + px;
      double left_y = player_dir_x * z + py;
      double right_x = player_dir_y * z + px;
      double right_y = -player_dir_x * z + py;
      double dx = (right_x - left_x) / WIDTH;
      double dy = (right_y - left_y) / WIDTH;
      int texture_x = (int)(left_x + dx * x) & 63;
      int texture_y = (int)(left_y + dy * x) & 63;
      Uint32 pixel = texture->pixels[texture_y * 64 + texture_x];
      u8 r, g, b, a;
      SDL_GetRGBA(pixel, texture->format, &r, &g, &b, &a);
      r = r * light_level / 255;
      g = g * light_level / 255;
      b = b * light_level / 255;
      Uint32 light_adjusted_pixel = SDL_MapRGBA(fmt, r, g, b, a);
      mr->engine->pixels[iy * WIDTH + x] = light_adjusted_pixel;
    }
  }
}

void draw_crosshair(map_renderer *mr, color c, int size) {
  int middle_x = WIDTH / 2;
  int middle_y = HEIGHT / 2;

  SDL_SetRenderDrawColor(mr->renderer, c.r, c.g, c.b, 255);
  SDL_RenderDrawLine(mr->renderer, middle_x, middle_y + size, middle_x,
                     middle_y - size);
  SDL_RenderDrawLine(mr->renderer, middle_x + size, middle_y, middle_x - size,
                     middle_y);
}

void draw_sprite_column(map_renderer *mr, patch *sprite,
                        int sprite_column, // does NOT WORK YET
                        int screen_x, int y1, int y2, double inverted_scale,
                        Uint32 c) { 
    if (y1 < y2) {
    double sprite_y = y1 < 0 ? -y1 * inverted_scale : 0;
    y1 = max(y1, 0); // clipping
    y2 = min(y2, HEIGHT - 1);
    int sprite_y_int;
    for (int top = y1; top < y2 - 1; top++) {
      sprite_y_int = (int)sprite_y;
      Uint32 pixel = sprite->pixels[sprite_y_int * sprite->header.width + sprite_column];
      if (pixel == 0) {
        sprite_y += inverted_scale;
        continue;
      }
      mr->engine->pixels[top * WIDTH + screen_x] = pixel;
      sprite_y += inverted_scale;
    }
  }
}

double get_height_difference(bsp *b, vec2 pos1, vec2 pos2) {
  return get_xy_floor_height(b, pos1) - get_xy_floor_height(b, pos2);
}

void get_drawing_rect(vs_sprite vssprite,int* top,int* height,double z_diff) {
  *top = HALF_HEIGHT - vssprite.scale * vssprite.sprite->header.height - z_diff * vssprite.scale;
  *height = 2 * vssprite.scale * vssprite.sprite->header.height;
}

void render_vssprite(map_renderer *mr, vs_sprite vssprite) {
  patch *sprite = vssprite.sprite;
  double z_diff = get_height_difference(mr->engine->bsp, mr->engine->p->pos, vssprite.pos); // height difference between the player and the sprite
  int top,height; // top and height of the sprite
  get_drawing_rect(vssprite,&top,&height,z_diff);
  int left = vssprite.x1;
  int width = vssprite.x2 - vssprite.x1;
  double inverted_scale = 1 / (2 * vssprite.scale); // used later as a function to scale the sprite
  int ds_ind = find_clip_seg(
      vssprite.x1, vssprite.x2, vssprite.scale,
      DRAWSEGS_INDEX); // this seg is partially obscuring the sprite
  if (ds_ind == -1) {  // nothing is obscuring the sprite that is before in the FOV, draw the whole sprite
    double sprite_column = vssprite.x1 < 0 ? -vssprite.x1 * inverted_scale : 0;
    int sprite_column_int = 0;
    for (int screen_x = max(vssprite.x1, 0); screen_x < min(vssprite.x2, WIDTH); 
         screen_x++) {
      sprite_column_int = (int)sprite_column;
      draw_sprite_column(mr, sprite, sprite_column_int, screen_x, top,
                         top + height, inverted_scale,
                         vssprite.color);
      sprite_column += inverted_scale;
    }
  } else { // something is obscuring the sprite
    printf("SOMETHING IS OBSCURING\n");
    // int left_clip = INT_MAX;
    // int right_clip = INT_MIN;
    // do {
    //   left_clip = min(left_clip, DRAWSEGS[ds_ind].x1);
    //   right_clip = max(right_clip, DRAWSEGS[ds_ind].x2);
    //   ds_ind =
    //       find_clip_seg(vssprite.x1, vssprite.x2, vssprite.scale, ds_ind - 1);
    // } while (ds_ind != -1);
    // double sprite_column = 0;
    // int sprite_column_int = 0;
    // for (int screen_x = max(0, left); screen_x < min(WIDTH, left_clip);
    //      screen_x++) {
    //   sprite_column_int = (int)(sprite_column) ;
    //   draw_sprite_column(mr, sprite, sprite_column_int, screen_x, max(top, 0),
    //                      min(HEIGHT, top + height), inverted_scale,
    //                      vssprite.color);
    //   sprite_column += 1 / vssprite.scale;
    // }

    // sprite_column = ((double)right_clip) / vssprite.scale;
    // for (int screen_x = max(right_clip, 0); screen_x < min(WIDTH, left + width);
    //      screen_x++) {
    //    sprite_column_int = (int)(sprite_column);
    //   draw_sprite_column(mr, sprite, sprite_column, screen_x, max(top, 0),
    //                      min(HEIGHT, top + height), inverted_scale,
    //                      vssprite.color);
    //   sprite_column += 1 / vssprite.scale;
    // }
  }
}

void render_vssprites(map_renderer *mr) {
  for (int i = 0; i < VSSPRITES_INDEX; i++) {
    render_vssprite(mr, VSSPRITES[i]);
  }
  VSSPRITES_INDEX = 0;
  DRAWSEGS_INDEX = 0;
}