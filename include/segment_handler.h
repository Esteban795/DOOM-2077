#ifndef SEGMENT_HANDLER_H
#define SEGMENT_HANDLER_H

#include "segment.h"
#include "settings.h"
#include "structs.h"
#include "map_renderer.h"
#include <stdlib.h>

#define MAX_SCALE 64.0
#define MIN_SCALE 0.00390625
extern bool BSP_TRAVERSE;
void classify_segment(segment_handler* sh,segment* seg,int x1, int x2,double raw_angle_1);

segment_handler* segment_handler_init(engine* e);

void segment_handler_update(segment_handler* sh);

void segment_handler_free(segment_handler* sh);
#endif