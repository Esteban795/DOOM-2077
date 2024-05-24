#include "../include/player_animation.h"
#include <math.h>
#include <stdio.h>

char ANIMATION_NAME[9] = {'P','L','A','Y','A','1','A','1','\0'};
int CURRENT_ANIMATION_PROGRESS[NUM_PLAYERS] = {0};
enum AnimationType CURRENT_ANIMATION[NUM_PLAYERS] = {IDLE, IDLE, IDLE};

const int A_OFFSET = (int)'A';

// RETURNS A NUMBER BETWEEN 1-8 depending on the orientation of the sprite relative to the player
int get_sprite_orientation(vec2 origin_pos,double origin_angle, vec2 target_pos, double target_angle) {
  int angle_diff = (int)fabs(origin_angle - target_angle);
  int res;
  if ((angle_diff < 22.5) || (angle_diff >= 337.5)) {
    res = 0; // Affichage du dos du sprite
  } else if (angle_diff < 45 + 22.5) {
    res = 1; //
  } else if (angle_diff < 90 + 22.5) {
    res = 2; // le joueur 2 regarde vers la droite du pov du joueur 1
  } else if (angle_diff < 135 + 22.5) {
    res = 3;
  } else if (angle_diff < 180 + 22.5) {
    res = 4; // les deux joueurs se font face
  } else if (angle_diff < 225 + 22.5) {
    res = 5;
  } else if (angle_diff < 270 + 22.5) {
    res = 6; // le joueur 2 regarde vers la gauche
  } else {
    res = 7;
  }
  return (res + 4) % NUMBER_ORIENTATION + 1;
}

void set_orientation(int sprite_orientation) {
    if (sprite_orientation == 1 || sprite_orientation == 5) {
        ANIMATION_NAME[5] = '0' + sprite_orientation;
        ANIMATION_NAME[6] = '\0';
        printf("first branch\n");
        printf("Animation name: %s\n", ANIMATION_NAME);
    } else if (1 < sprite_orientation && sprite_orientation < 5){
        ANIMATION_NAME[5] = '0' + sprite_orientation;
        ANIMATION_NAME[5 + 2] = '0' + NUMBER_ORIENTATION + 2 - sprite_orientation;
        printf("second branch\n");
        printf("Animation name: %s\n", ANIMATION_NAME);
    } else if (5 < sprite_orientation && sprite_orientation < NUMBER_ORIENTATION + 1) {
        ANIMATION_NAME[5 + 2] =  '0' + sprite_orientation;
        ANIMATION_NAME[5] = '0' + NUMBER_ORIENTATION + 2 - sprite_orientation;
        printf("Animation name: %s\n", ANIMATION_NAME);
    }
    printf("%s\n", ANIMATION_NAME);
}

bool set_correct_animation_name(int i,vec2 origin_pos,double origin_angle, vec2 pos,double angle,enum AnimationType type) {
    int sprite_orientation = get_sprite_orientation(origin_pos,origin_angle, pos, angle);
    fflush(stdout);
    if (type == IDLE){
        set_orientation(sprite_orientation);
        ANIMATION_NAME[4] = ANIMATION_IDLE_INIT;
        if (sprite_orientation != 1 && sprite_orientation != 5) {
            ANIMATION_NAME[6] = ANIMATION_IDLE_INIT;
        }
        CURRENT_ANIMATION[i] = IDLE;
        CURRENT_ANIMATION_PROGRESS[i] = 0;
    } else if (type == DYING){
        if (CURRENT_ANIMATION[i] != DYING){
            ANIMATION_NAME[4] = ANIMATION_DEATH_INIT;
            ANIMATION_NAME[5] = 0;
            CURRENT_ANIMATION[i] = DYING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
        } else {
            CURRENT_ANIMATION_PROGRESS[i] += (CURRENT_ANIMATION_PROGRESS[i] % ANIMATION_DEATH_FRAME_COUNT);
            ANIMATION_NAME[5] = ANIMATION_DEATH_INIT + CURRENT_ANIMATION_PROGRESS[i];
        }
    } else if (type == EXTREME_DYING) {
        if (CURRENT_ANIMATION[i] != EXTREME_DYING){
            ANIMATION_NAME[4] = ANIMATION_EXTREME_DEATH_INIT;
            ANIMATION_NAME[5] = 0;
            CURRENT_ANIMATION[i] = EXTREME_DYING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
        } else {
            CURRENT_ANIMATION_PROGRESS[i] = (CURRENT_ANIMATION_PROGRESS[i] % ANIMATION_EXTREME_DEATH_FRAME_COUNT);
            ANIMATION_NAME[5] = ANIMATION_EXTREME_DEATH_INIT + CURRENT_ANIMATION_PROGRESS[i];
        }
    } else if (type == MOVING) {
        if (CURRENT_ANIMATION[i] != MOVING) {
            set_orientation(sprite_orientation);
            ANIMATION_NAME[4] = ANIMATION_WALK_INIT;
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_WALK_INIT;
            }
            CURRENT_ANIMATION[i] = MOVING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
        } else {
            set_orientation(sprite_orientation);
            CURRENT_ANIMATION_PROGRESS[i] += (CURRENT_ANIMATION_PROGRESS[i] % ANIMATION_WALK_FRAME_COUNT);
            ANIMATION_NAME[4] = ANIMATION_WALK_INIT + CURRENT_ANIMATION_PROGRESS[i];
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_WALK_INIT + CURRENT_ANIMATION_PROGRESS[i];
            }
        }
    } else if (type == SHOOTING) {
        if (CURRENT_ANIMATION[i] != SHOOTING) {
            set_orientation(sprite_orientation);
            ANIMATION_NAME[4] = ANIMATION_SHOOT_INIT;
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_SHOOT_INIT;
            }
            CURRENT_ANIMATION[i] = SHOOTING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
        } else {
            set_orientation(sprite_orientation);
            CURRENT_ANIMATION_PROGRESS[i] += (CURRENT_ANIMATION_PROGRESS[i] % ANIMATION_SHOOT_FRAME_COUNT);
            ANIMATION_NAME[4] = ANIMATION_SHOOT_INIT + CURRENT_ANIMATION_PROGRESS[i];
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_SHOOT_INIT + CURRENT_ANIMATION_PROGRESS[i];
            }
        }
    }
    return !(5 < sprite_orientation && sprite_orientation < NUMBER_ORIENTATION + 1); // should we use the mirror 
}