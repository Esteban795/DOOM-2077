#ifndef DRAWSEG_H
#define DRAWSEG_H

#include <stdlib.h>
#include "segment.h"
#include "geometry.h"

#define MAX_DRAWSEGS 2048
struct Drawseg {
    segment* seg;
    int x1;
    int x2;
    double scale1;
    double scale2;
    double scalestep;
    // int high_clip;
    // int low_clip;
    // int* upper_clip;
    // int* lower_clip;
};

typedef struct Drawseg drawseg;

extern drawseg DRAWSEGS[2048]; // stores drawsegs for clippin information
extern int DRAWSEGS_INDEX;

void drawseg_add(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep);

void drawseg_direct_add(drawseg d);

int find_clip_seg(int x1, int x2,double scale,int start_index);
#endif