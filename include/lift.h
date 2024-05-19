#ifndef LIFT_H
#define LIFT_H

#include "ecs/entity.h"
#include "sector.h"
#include "geometry.h"

// https://doomwiki.org/wiki/Linedef_type#Platform_targets

// S = slow
// F = fast
// SR = switchable, retriggerable
// S1 = switchable, non-retriggerable
// WR = walkover, retriggerable
// W1 = walkover, non-retriggerable
// LNF = Lowest Neighbor Floor
// RNF = Raise Next Floor
// LFHP = Lowest and Highest Floor perpetual
//  xU where x is a number = raise of x units
// Order : TRIGGER_DELAY_SPEED_BEHAVIOR
enum LiftType {
    WR_3_S_LNF = 88,
    W1_3_S_LNF = 10,
    SR_0_S_24U = 66,
    S1_0_S_24U = 15,
    WR_0_S_24U = 148,
    W1_0_S_24U = 143,
    SR_0_S_32U = 67,
    S1_0_S_32U = 14,
    WR_0_S_32U = 149,
    W1_0_S_32U = 144,
    SR_0_S_RNF = 68,
    S1_0_S_RNF = 20,
    WR_0_S_RNF = 95,
    W1_0_S_RNF = 22,
    G1_0_S_RNF = 47,
    SR_3_S_LFHP = 181,
    S1_3_S_LFHP = 162,
    WR_3_S_LFHP = 87,
    W1_3_S_LFHP = 53,
    SR_3_S_LNF = 62,
    S1_3_S_LNF = 21,
    SR_3_F_LNF = 123,
    S1_3_F_LNF = 122,
    WR_3_F_LNF = 120,
    W1_3_F_LNF = 121
};

enum LiftTransitionSpeed {
    L_SLOW = 1,
    L_FAST = 2,
    L_TURBO = 3,
    L_NO_SPEED = 0
};

struct Lift {
    uint64_t id;
    int time_elapsed;
    sector* sector;
    enum LiftTransitionSpeed speed;
    i16 low_height;
    i16 high_height;
    bool once;
    int delay;
    bool is_switching;
    bool init_state; // 0 = low, 1 = high
    bool state; // 0 = low, 1 = high
    struct Lift* next_lift;
};

typedef struct Lift lift;

lift *lift_create(uint64_t id, sector *sector, enum LiftTransitionSpeed speed,
                  i16 low_height, i16 high_height, int delay, bool init_state,bool once);

bool lift_trigger_switch(lift* l);

void lifts_free(lift** lifts,int len_lifts);

void lift_update(lift* l,int DT);

lift* lift_add(lift* l, lift* new_lift);

bool is_a_lift(i16 line_type);
#endif