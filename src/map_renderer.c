#include "../include/map_renderer.h"
#include <limits.h>

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
      pixel =
          texture->pixels[texture_y_int * texture_width + texture_column_int];
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
    position_ct* pos = player_get_position(e->p);

    double player_dir_x = cos(deg_to_rad(pos->angle));
    double player_dir_y = -sin(deg_to_rad(
        pos->angle)); // - because the fucking y axis is reversed
    double z,px,py,left_x,left_y,right_x,right_y,dx,dy;
    int texture_x,texture_y;
    Uint32 pixel,light_adjusted_pixel;
    u8 r,g,b,a;
    for (int iy = y1; iy < y2; iy++) {
      z = HALF_WIDTH * world_z / (HALF_HEIGHT - iy);
      px = player_dir_x * z + pos->x;
      py = player_dir_y * z + pos->y;
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

void draw_crosshair(engine* e, color c, int size) {
  int middle_x = WIDTH / 2;
  int middle_y = HEIGHT / 2;

  SDL_SetRenderDrawColor(e->renderer, c.r, c.g, c.b, 255);
  SDL_RenderDrawLine(e->renderer, middle_x, middle_y + size, middle_x,
                     middle_y - size);
  SDL_RenderDrawLine(e->renderer, middle_x + size, middle_y, middle_x - size,
                     middle_y);
}

void draw_sprite_column(engine* engine, patch *sprite,
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
      Uint32 pixel =
          sprite->pixels[sprite_y_int * sprite->header.width + sprite_column];
      if (pixel == 0) {
        sprite_y += inverted_scale;
        continue;
      }
      engine->pixels[top * WIDTH + screen_x] = pixel;
      sprite_y += inverted_scale;
    }
  }
}

double get_height_difference(bsp *b, vec2 pos1, vec2 pos2) {
  return get_xy_floor_height(b, pos1) - get_xy_floor_height(b, pos2);
}

void get_drawing_rect(vs_sprite vssprite, int *top, int *height,
                      double z_diff) {
  *top = HALF_HEIGHT - vssprite.scale * vssprite.sprite->header.height -
         z_diff * vssprite.scale;
  *height = 2 * vssprite.scale * vssprite.sprite->header.height;
}

void render_vssprite(engine* e, vs_sprite vssprite) {
  position_ct* pos = player_get_position(e->p);
  vec2 player_pos = (vec2){pos->x, pos->y};
  patch *sprite = vssprite.sprite;
  double z_diff = get_height_difference(
      e->bsp, player_pos,
      vssprite.pos); // height difference between the player and the sprite
  int top, height;   // top and height of the sprite
  get_drawing_rect(vssprite, &top, &height, z_diff); // get left,top corner and height width  
  int left = vssprite.x1;
  int width = vssprite.x2 - vssprite.x1;
  double inverted_scale =
      1 / (2 * vssprite.scale); // used later as a function to scale the sprite
  int ds_ind = find_clip_seg(
      vssprite.x1, vssprite.x2, vssprite.scale,
      DRAWSEGS_INDEX); // this seg is partially obscuring the sprite
  if (ds_ind == -1) {  // nothing is obscuring the sprite that is before in the
                       // FOV, draw the whole sprite
    double sprite_column = vssprite.x1 < 0 ? -vssprite.x1 * inverted_scale : 0; // account for clipping the sprite
    int sprite_column_int = 0;
    for (int screen_x = max(vssprite.x1, 0); screen_x < min(vssprite.x2, WIDTH);
         screen_x++) {
      sprite_column_int = (int)sprite_column;
      draw_sprite_column(e, sprite, sprite_column_int, screen_x, top,
                         top + height, inverted_scale, vssprite.color);
      sprite_column += inverted_scale;
    }
  } else { // something is obscuring the sprite
    int left_clip = INT_MAX;
    int right_clip = INT_MIN;
    do {
      left_clip = min(left_clip, DRAWSEGS[ds_ind].x1);
      right_clip = max(right_clip, DRAWSEGS[ds_ind].x2);
      ds_ind =
          find_clip_seg(vssprite.x1, vssprite.x2, vssprite.scale, ds_ind - 1);
    } while (ds_ind != -1);
    if (vssprite.x1 <= left_clip && right_clip <= vssprite.x2) { // seg is visible on both sides (e.g : there is a pillar in between but the sprite is wider than pillar)
      double sprite_column = vssprite.x1 < 0 ? -vssprite.x1 * inverted_scale : 0; // account for clipping the sprite
      int sprite_column_int = 0;
      for (int screen_x = max(0, vssprite.x1); screen_x < min(WIDTH, vssprite.x2);
          screen_x++) {
        sprite_column_int = (int)(sprite_column);
        if (left_clip < screen_x && screen_x < right_clip) {
          sprite_column += inverted_scale;
          continue;
        }
        draw_sprite_column(e, sprite, sprite_column_int, screen_x, top,
                          top + height, inverted_scale, vssprite.color);
        sprite_column += inverted_scale;
      }
    } else if (vssprite.x1 <= left_clip) { // only the left part of the sprite is visible
      double sprite_column = vssprite.x1 < 0 ? -vssprite.x1 * inverted_scale : 0;
      int sprite_column_int = 0;
      for (int screen_x = max(0, vssprite.x1); screen_x < min(WIDTH, left_clip);
          screen_x++) {
        sprite_column_int = (int)(sprite_column);
        draw_sprite_column(e, sprite, sprite_column_int, screen_x, top,
                          top + height, inverted_scale, vssprite.color);
        sprite_column += inverted_scale;
      }
    } else if (right_clip <= vssprite.x2) { // only the right part of the sprite is visible
      double sprite_column = (double)(right_clip - vssprite.x1) * inverted_scale;
      int sprite_column_int = 0;
      for (int screen_x = max(0, right_clip); screen_x < min(WIDTH, vssprite.x2);
          screen_x++) {
        sprite_column_int = (int)(sprite_column);
        draw_sprite_column(e, sprite, sprite_column_int, screen_x, top,
                          top + height, inverted_scale, vssprite.color);
        sprite_column += inverted_scale;
      }
    }
  }
}

void render_vssprites(engine* e) {
  for (int i = 0; i < VSSPRITES_INDEX; i++) {
    render_vssprite(e, VSSPRITES[i]);
  }
  VSSPRITES_INDEX = 0;
  DRAWSEGS_INDEX = 0;
}

// Pour l'instant un int est renvoyé en fonction de l'angle relatif des deux
// joueurs, a changer plus tard (?)
int sprite_orientation(player *player_1,
                       player *player_2) { // le joueur controlé est le joueur
  position_ct* pos1 = player_get_position(player_1);
  position_ct* pos2 = player_get_position(player_2);
  int angle_diff = (int)fabs(pos1->angle - pos2->angle);
  if ((angle_diff < 22.5) || (angle_diff >= 337.5)) {
    return 0; // Affichage du dos du sprite
  } else if (angle_diff < 45 + 22.5) {
    return 1; //
  } else if (angle_diff < 90 + 22.5) {
    return 2; // le joueur 2 regarde vers la droite du pov du joueur 1
  } else if (angle_diff < 135 + 22.5) {
    return 3;
  } else if (angle_diff < 180 + 22.5) {
    return 4; // les deux joueurs se font face
  } else if (angle_diff < 225 + 22.5) {
    return 5;
  } else if (angle_diff < 270 + 22.5) {
    return 6; // le joueur 2 regarde vers la gauche
  } else {
    return 7;
  }
}
