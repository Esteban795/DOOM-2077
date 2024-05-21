#ifndef BSP_H
#define BSP_H

#include "map_renderer.h"
#include "structs.h"
#include "vec2.h"
#include "segment_handler.h"


extern bool BSP_TRAVERSE; // boolean used to determine if screen is filled and doesn't require anymore traversal

#define SUBSECTOR_IDENTIFIER (size_t)0x8000 // identifier used by BSP to determine if the node is a leaf or not

bsp *bsp_init(engine *e, player *p);

void render_bsp_node(bsp *b, size_t node_id);

void update_bsp(bsp *b);

void bsp_free(bsp *b);

double get_ssector_height(bsp* b);
#endif