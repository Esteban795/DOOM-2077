#include "../include/drawseg.h"
#include <stdio.h>

drawseg DRAWSEGS[MAX_DRAWSEGS];
int DRAWSEGS_INDEX = 0;

// drawseg drawseg_new(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep, int high_clip, int low_clip,int *upper_clip, int *lower_clip) {
//     drawseg ds;
//     ds.seg = seg;
//     ds.x1 = x1;
//     ds.x2 = x2;
//     ds.scale1 = scale1;
//     ds.scale2 = scale2;
//     ds.scalestep = scalestep;
//     ds.high_clip = high_clip;
//     ds.low_clip = low_clip;
//     ds.upper_clip = upper_clip;
//     ds.lower_clip = lower_clip;
//     return ds;
// }

// void drawseg_add(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep, int high_clip, int low_clip,int* upper_clip, int* lower_clip) {
//     drawseg d = drawseg_new(seg, x1, x2, scale1, scale1, scalestep, high_clip, low_clip, upper_clip, lower_clip);
//     DRAWSEGS[DRAWSEGS_INDEX] = d;
//     DRAWSEGS_INDEX++;
// }

drawseg drawseg_new(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep) {
    drawseg ds;
    ds.seg = seg;
    ds.x1 = x1;
    ds.x2 = x2;
    ds.scale1 = scale1;
    ds.scale2 = scale2;
    ds.scalestep = scalestep;
    return ds;
}

void drawseg_add(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep) {
    drawseg d = drawseg_new(seg, x1, x2, scale1, scale2, scalestep);
    DRAWSEGS[DRAWSEGS_INDEX] = d;
    DRAWSEGS_INDEX++;
}

void drawseg_direct_add(drawseg d) {
    DRAWSEGS[DRAWSEGS_INDEX] = d;
    DRAWSEGS_INDEX++;
}

int find_clip_seg(int x1, int x2,double scale,int start_index){
    drawseg ds;
    for (int i = start_index; i >= 0; i--) {
        ds = DRAWSEGS[i];
        if (scale < ds.scale2 && scale < ds.scale1 && do_segs_intersect(x1, x2, ds.x1, ds.x2)) {
            // printf("found clip seg, from %d to %d with scale %f \n", ds.x1, ds.x2, ds.scale1);
            return i;
        }
    }
    return -1;
}