#ifndef DRAWSEG_H
#define DRAWSEG_H

#include <stdlib.h>
#include "segment.h"
#include "geometry.h"

#define MAX_DRAWSEGS 2048

enum DrawsegType {
    DRAWSEG_SOLID_WALL,
    DRAWSEG_PORTAL_WALL
};

struct Drawseg {
    segment* seg;
    int x1;
    int x2;
    double scale1;
    double scale2;
    double scalestep;
    enum DrawsegType type;
    double* top_clips;
    double* bottom_clips;
};

typedef struct Drawseg drawseg;

extern drawseg DRAWSEGS[2048]; // stores drawsegs for clippin information
extern int DRAWSEGS_INDEX;

void drawseg_add(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep,double* top_clips, double* bottom_clips,enum DrawsegType type);

void drawseg_direct_add(drawseg d);

int find_clip_seg(int x1, int x2,double scale,int start_index);
#endif