#include "../include/map_renderer.h"

void draw_wall_column(engine* e, texture_map *texture,
                      double texture_column, int x, int y1, int y2,
                      double texture_alt, double inverted_scale,
                      i16 light_level) {
  if (y1 < y2) {
    int texture_width = texture->width;
    int texture_height = texture->height;
    int texture_column_int = (int)mod(texture_column, (double)texture_width);
    double texture_y =
        texture_alt + ((double)y1 - HALF_HEIGHT) * inverted_scale;
    u8 r,g,b,a;
    Uint32 pixel, light_adjusted_pixel;
    double db_texture_height = texture_height;
    int texture_y_int;
    for (int y = y1; y < y2; y++) {
      texture_y_int = (int)mod(texture_y, db_texture_height);
      pixel = texture->pixels[texture_y_int * texture_width + texture_column_int];
      SDL_GetRGBA(pixel, texture->format, &r, &g, &b, &a);
      r = r * light_level / 255;
      g = g * light_level / 255;
      b = b * light_level / 255;
      light_adjusted_pixel = SDL_MapRGBA(texture->format, r, g, b, a);
      e->pixels[y * WIDTH + x] = light_adjusted_pixel;
      texture_y += inverted_scale;
    }
  }
}

void draw_flat(engine* e, flat *texture, i16 light_level, int x, int y1,
               int y2, int world_z) {
  if (y1 < y2) {
    double player_dir_x = cos(deg_to_rad(e->p->angle));
    double player_dir_y = -sin(deg_to_rad(
        e->p->angle)); // - because the fucking y axis is reversed
    double z,px,py,left_x,left_y,right_x,right_y,dx,dy;
    int texture_x,texture_y;
    Uint32 pixel,light_adjusted_pixel;
    u8 r,g,b,a;
    for (int iy = y1; iy < y2; iy++) {
      z = HALF_WIDTH * world_z / (HALF_HEIGHT - iy);
      px = player_dir_x * z + e->p->pos.x;
      py = player_dir_y * z + e->p->pos.y;
      left_x = -player_dir_y * z + px;
      left_y = player_dir_x * z + py;
      right_x = player_dir_y * z + px;
      right_y = -player_dir_x * z + py;
      dx = (right_x - left_x) / WIDTH;
      dy = (right_y - left_y) / WIDTH;
      texture_x = (int)(left_x + dx * x) & 63;
      texture_y = (int)(left_y + dy * x) & 63;
      pixel = texture->pixels[texture_y * 64 + texture_x];
      SDL_GetRGBA(pixel, texture->format, &r, &g, &b, &a);
      r = r * light_level / 255;
      g = g * light_level / 255;
      b = b * light_level / 255;
      light_adjusted_pixel = SDL_MapRGBA(texture->format, r, g, b, a);
      e->pixels[iy * WIDTH + x] = light_adjusted_pixel;
    }
  }
}

void draw_crosshair(engine* e,color c,int size){
  int middle_x = WIDTH/2;
  int middle_y = HEIGHT/2;
  
  SDL_SetRenderDrawColor(e->renderer, c.r, c.g, c.b, 255);
  SDL_RenderDrawLine(e->renderer, middle_x, middle_y+size, middle_x, middle_y-size);
  SDL_RenderDrawLine(e->renderer, middle_x + size, middle_y, middle_x -size, middle_y);
}