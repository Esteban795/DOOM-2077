#include "../../include/core/lift.h"
#include <stdio.h>

// WORKS THE EXACT SAME WAY AS IN door.c

lift *lift_create(uint64_t id, sector *sector, enum LiftTransitionSpeed speed,
                  i16 low_height, i16 high_height, int delay, bool init_state,
                  bool once) {
  lift *l = malloc(sizeof(lift));
  l->id = id;
  l->sector = sector;
  l->speed = speed;
  l->low_height = low_height;
  l->high_height = high_height;
  l->delay = delay;
  l->is_switching = false;
  l->init_state = init_state;
  l->state = init_state;
  l->next_lift = NULL;
  l->time_elapsed = 0;
  l->next_lift = NULL;
  l->center_pos = (vec2){.x = 0, .y = 0};
  l->once = once;
  return l;
}

bool lift_trigger_switch(lift *l) {
  if (l != NULL) {
    bool switched = false;
    if (!l->is_switching && !(l->state != l->init_state)) {
      l->is_switching = true;
      add_sound_to_play(LIFT_START_SOUND, l->sector->center_pos.x,
                        l->sector->center_pos.y);
      switched = true;
    }
    if (l->next_lift != NULL) {
      switched = switched || lift_trigger_switch(l->next_lift);
    }
    return switched;
  }
  return false;
}

void lift_update(lift *l, int DT) {
  if (l->speed == L_NO_SPEED) {
    return;
  }

  if (l->state != l->init_state) {
    l->time_elapsed += DT;
    if (l->time_elapsed >= l->delay) {
      l->time_elapsed = 0;
      l->is_switching = true;
      add_sound_to_play(LIFT_STOP_SOUND, l->sector->center_pos.x,
                        l->sector->center_pos.y);
    }
  }

  if (l->is_switching) {
    double multiplier;
    if (l->state ==
        l->init_state) {   // we're switching from init state to other state
      if (l->init_state) { // lift initially at high height
        multiplier = -1;
      } else {
        multiplier = 1;
      }
    } else { // we're switching from other state to init state
      if (l->init_state) {
        multiplier = 1;
      } else {
        multiplier = -1;
      }
    }
    double height_gained =
        multiplier *
        ceil((double)l->speed * (l->high_height - l->low_height) * DT / 500);
    int new_height = l->sector->floor_height + height_gained;
    l->sector->floor_height =
        max(l->low_height, min(l->high_height, new_height));
    if (new_height < l->low_height || new_height > l->high_height) {
      l->is_switching = false;
      l->state = !l->state;
      l->sector->floor_height = l->state ? l->high_height : l->low_height;
      if (l->once) {
        l->speed = L_NO_SPEED;
      }
    }
  }
}

void lifts_free(lift **lifts, int len_lifts) {
  for (int i = 0; i < len_lifts; i++) {
    free(lifts[i]);
  }
  free(lifts);
}

lift *lift_add(lift *l, lift *new_lift) {
  if (l == NULL) {
    return new_lift;
  }
  new_lift->next_lift = l;
  return new_lift;
}

bool is_a_lift(i16 line_type) {
  return line_type == WR_3_S_LNF || line_type == W1_3_S_LNF ||
         line_type == SR_0_S_24U || line_type == S1_0_S_24U ||
         line_type == WR_0_S_24U || line_type == W1_0_S_24U ||
         line_type == SR_0_S_32U || line_type == S1_0_S_32U ||
         line_type == WR_0_S_32U || line_type == W1_0_S_32U ||
         line_type == SR_0_S_RNF || line_type == S1_0_S_RNF ||
         line_type == WR_0_S_RNF || line_type == W1_0_S_RNF ||
         line_type == G1_0_S_RNF || line_type == SR_3_S_LFHP ||
         line_type == S1_3_S_LFHP || line_type == WR_3_S_LFHP ||
         line_type == W1_3_S_LFHP || line_type == SR_3_S_LNF ||
         line_type == S1_3_S_LNF || line_type == SR_3_F_LNF ||
         line_type == S1_3_F_LNF || line_type == WR_3_F_LNF ||
         line_type == W1_3_F_LNF;
}