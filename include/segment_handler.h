#ifndef SEGMENT_HANDLER_H
#define SEGMENT_HANDLER_H

#include "segment.h"
#include "settings.h"
#include "structs.h"
#include "bsp.h"

#include <stdlib.h>

void classify_segment(segment_handler* sh,segment* seg,int x1, int x2);

segment_handler* segment_handler_init(engine* e);

void segment_handler_update(segment_handler* sh);

void segment_handler_free(segment_handler* sh);
#endif