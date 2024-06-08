#include "../../include/render/drawseg.h"

drawseg DRAWSEGS[MAX_DRAWSEGS];
int DRAWSEGS_INDEX = 0;

drawseg drawseg_new(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep,double* top_clips,double* bottom_clips,enum DrawsegType type) {
    drawseg ds;
    ds.seg = seg;
    ds.x1 = x1;
    ds.x2 = x2;
    ds.scale1 = scale1;
    ds.scale2 = scale2;
    ds.scalestep = scalestep;
    ds.type = type;
    ds.top_clips = top_clips;
    ds.bottom_clips = bottom_clips;
    return ds;
}

void drawseg_add(segment* seg, int x1, int x2, double scale1, double scale2, double scalestep,double* top_clips, double* bottom_clips,enum DrawsegType type) {
    drawseg d = drawseg_new(seg, x1, x2, scale1, scale2, scalestep,top_clips,bottom_clips,type);
    DRAWSEGS[DRAWSEGS_INDEX] = d;
    DRAWSEGS_INDEX++;
}

void drawseg_direct_add(drawseg d) {
    DRAWSEGS[DRAWSEGS_INDEX] = d;
    DRAWSEGS_INDEX++;
}


// Find the segment that both crosses our [x1,x2] and has a bigger scale than give scale
int find_clip_seg(int x1, int x2,double scale,int start_index){
    drawseg ds;
    for (int i = start_index; i >= 0; i--) {
        ds = DRAWSEGS[i];
        if (scale < ds.scale2 && scale < ds.scale1 && do_segs_intersect(x1, x2, ds.x1, ds.x2)) {
            return i;
        }
    }
    return -1;
}