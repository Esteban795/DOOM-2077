#ifndef BSP_H
#define BSP_H

#include "map_renderer.h"
#include "structs.h"
#include "vec2.h"
#include "segment_handler.h"


extern bool BSP_TRAVERSE;

#define SUBSECTOR_IDENTIFIER (size_t)0x8000

bsp *bsp_init(engine *e, player *p);

void render_bsp_node(bsp *b, size_t node_id);

void update_bsp(bsp *b);

void bsp_free(bsp *b);

void get_ssector_height(bsp* b);
#endif