#include "../include/lift.h"
#include <stdio.h>

lift *lift_create(entity_t *id, sector *sector, enum LiftTransitionSpeed speed,
                  i16 low_height, i16 high_height, int delay, bool init_state,bool once) {
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
  l->center_pos =  (vec2){.x = 0, .y = 0};
  l->once = once;
  return l;
}

void lift_trigger_switch(vec2 cam_pos,lift *l) {
  if (l != NULL) {
    if (!l->is_switching && !(l->state != l->init_state)) {
      l->is_switching = true;
      struct SoundEntry se;
      se.sound = DOOR_OPEN_SOUND;
      se.angle = point_to_angle(cam_pos, l->sector->center_pos);
      se.volume = get_audio_gain(dist(cam_pos, l->sector->center_pos));
      SOUNDS_TO_PLAY[SOUNDS_INDEX] = se;
      SOUNDS_INDEX++;
    }
    if (l->next_lift != NULL) {
      lift_trigger_switch(cam_pos,l->next_lift);
    }
  }
}

void lift_update(lift *l, vec2 player_pos,int DT) {
  if (l->speed == L_NO_SPEED) {
    return;
  }
  
  if (l->state != l->init_state) {
    l->time_elapsed += DT;
    if (l->time_elapsed >= l->delay) {
      l->time_elapsed = 0;
      l->is_switching = true;
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
    double height_gained = multiplier * ceil((double)l->speed * (l->high_height - l->low_height) * DT / 500);
    int new_height = l->sector->floor_height + height_gained;
    l->sector->floor_height =
        max(l->low_height, min(l->high_height, new_height));
    if (new_height < l->low_height || new_height > l->high_height) {
      l->is_switching = false;
      l->state = !l->state;
      l->sector->floor_height = l->state ? l->high_height : l->low_height;
      struct SoundEntry se;
      se.sound = DOOR_OPEN_SOUND;
      se.angle = point_to_angle(player_pos, l->sector->center_pos);
      se.volume = get_audio_gain(dist(player_pos, l->sector->center_pos));
      SOUNDS_TO_PLAY[SOUNDS_INDEX] = se;
      SOUNDS_INDEX++;
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