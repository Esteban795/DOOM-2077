#ifndef BSP_H
#define BSP_H

#include "map_renderer.h"
#include "structs.h"
#include "vec2.h"

#define SUBSECTOR_IDENTIFIER (size_t)0x8000

bsp *bsp_init(engine *e, player *p);

void render_bsp_node(bsp *b, size_t node_id);

void update_bsp(bsp *b);

void bsp_free(bsp *b);
#endif