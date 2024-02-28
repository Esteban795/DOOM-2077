#include "../include/segment_handler.h"

void draw_solid_walls_range(segment_handler* sh, int x1, int x2){
    return;

}

void draw_portal_walls_range(segment_handler* sh, int x1, int x2){
    return;
}

int* calculate_ranges_to_draw(int* screen_range, int x1, int x2){
    int len = x2 - x1;
    int* ranges_to_draw = malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++){
        ranges_to_draw[i] = screen_range[x1 + i] ^ 1;
        screen_range[x1 + i] = 1; //so the screen range occupied is actually updated in a single pass.
    }
    return ranges_to_draw;
}

void clip_solid_walls(segment_handler* sh,int x1, int x2){
    if (sh->screen_range_count < WIDTH){
        int len = x2 - x1;
        int* ranges_to_draw = calculate_ranges_to_draw(sh->screen_range, x1, x2);
        int index_first_1 = x1;
        int index_last_1 = x1 + 1;
        for (int i = 0; i < len; i++){
            if (ranges_to_draw[i] == 1){
                index_last_1 += 1;
            } else {
                if (index_first_1 != index_last_1){
                    draw_solid_walls_range(sh, index_first_1, index_last_1);
                }
                index_first_1 = x1 + i + 1;
            }
        }
        free(ranges_to_draw);
    } else { // we've already drawn every wall on our screen, stop there
        BSP_TRAVERSE = false;
    }
}

void clip_portal_walls(segment_handler* sh,int x1, int x2){
    return;
}

void classify_segment(segment_handler* sh,segment* seg,int x1, int x2){
    if (x1 == x2) return; //no pixel needed to be drawn
    sector* front_sector = seg->front_sector;
    sector* back_sector = seg->back_sector;
    if (back_sector == NULL){ // its a solid wall
        clip_solid_walls(sh,x1, x2); 
    }
    else if (front_sector->ceiling_height != back_sector->ceiling_height || front_sector->floor_height != back_sector->floor_height) { //its a window
        clip_portal_walls(sh,x1, x2);
    } else if (!strcmp(back_sector->ceiling_texture, front_sector->ceiling_texture) && !strcmp(back_sector->floor_texture,front_sector->floor_texture) && back_sector->light_level == front_sector->light_level) { //its a portal
        return;
    } else { //its a wall
        clip_portal_walls(sh,x1, x2);
    }
}

segment_handler* segment_handler_init(engine* e){
    segment_handler* sh = malloc(sizeof(segment_handler));
    sh->seg = NULL; //init to NULL, will be changed each time by 'classify_segment'
    sh->player = e->p;
    sh->angle_to_p1 = 0.0;
    memset(sh->screen_range, 0, WIDTH * sizeof(int));
    sh->screen_range_count = 0;
    memset(sh->upper_clip, 0, WIDTH * sizeof(int));
    memset(sh->lower_clip, 0, WIDTH * sizeof(int));
    return sh;
}

void segment_handler_free(segment_handler* sh){
    free(sh);
}

void segment_handler_update(segment_handler* sh){
    memset(sh->screen_range, 0, WIDTH * sizeof(int)); //reset screen range
    sh->screen_range_count = 0;
    memset(sh->upper_clip, -1, WIDTH * sizeof(int)); //reset upper clip
    memset(sh->lower_clip, -1, WIDTH * sizeof(int)); //reset lower clip
}