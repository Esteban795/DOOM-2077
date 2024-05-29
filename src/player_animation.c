#include "../include/player_animation.h"
#include <stdio.h>

char ANIMATION_NAME[9] = {'P','L','A','Y','A','1','A','1','\0'};
int CURRENT_ANIMATION_PROGRESS[NUM_PLAYERS] = {0};
enum AnimationType CURRENT_ANIMATION[NUM_PLAYERS] = {PLAYER_IDLE, PLAYER_IDLE, PLAYER_IDLE};
int ANIMATION_COOLDOWNS[NUM_PLAYERS] = {0};

// RETURNS A NUMBER BETWEEN 1-8 depending on the orientation of the sprite relative to the player
// 1 IS SOUTH, 2 IS SOUTHWEST, 3 IS WEST, 4 IS NORTHWEST, 5 IS NORTH, 6 IS NORTHEAST, 7 IS EAST, 8 IS SOUTHEAST
int get_sprite_orientation(vec2 origin_pos,double origin_angle, vec2 pos,double angle) {
    double angle_diff = angle - origin_angle;
    double angle_to_camera = atan2(pos.y - origin_pos.y, pos.x - origin_pos.x);
    if (angle_to_camera > angle) {
        if (angle_diff > 180) {
            angle_diff -= 360;
        }
    } else {
        if (angle_diff < -180) {
            angle_diff += 360;
        }
    }
    angle_diff = (angle_diff / 360) + 0.5;
    double direction = 8 * angle_diff + 0.5;
    int orientation = rem((int)direction,8);
    return orientation + 1;
}

// SETS THE ANIMATION NAME BASED ON THE ORIENTATION OF THE SPRITE
// KEEP IN MIND THAT IF SPRITE ORIENTATION IS 1 OR 5, THE ANIMATION WILL ONLY HAVE
// ONE PART
void set_orientation(int sprite_orientation) {
    if (sprite_orientation == 1 || sprite_orientation == 5) {
        ANIMATION_NAME[5] = '0' + sprite_orientation;
        ANIMATION_NAME[6] = '\0';
    } else if (1 < sprite_orientation && sprite_orientation < 5){
        ANIMATION_NAME[5] = '0' + sprite_orientation;
        ANIMATION_NAME[5 + 2] = '0' + NUMBER_ORIENTATION + 2 - sprite_orientation;
    } else if (5 < sprite_orientation && sprite_orientation < NUMBER_ORIENTATION + 1) {
        ANIMATION_NAME[5 + 2] =  '0' + sprite_orientation;
        ANIMATION_NAME[5] = '0' + NUMBER_ORIENTATION + 2 - sprite_orientation;
    }
}

// SETS THE ANIMATION NAME BASED ON THE ORIENTATION OF THE SPRITE   s
bool set_correct_animation_name(int i,vec2 origin_pos,double origin_angle, vec2 pos,double angle,enum AnimationType* type,bool has_moved) {
    int sprite_orientation = get_sprite_orientation(origin_pos,origin_angle, pos, angle);
    if (*type == PLAYER_IDLE){
        idle: // useful for other functions
        set_orientation(sprite_orientation);
        ANIMATION_NAME[4] = ANIMATION_IDLE_INIT;
        if (sprite_orientation != 1 && sprite_orientation != 5) { // sprite orientation 1 or 5 have no 'second part'
            ANIMATION_NAME[6] = ANIMATION_IDLE_INIT;
        }
        CURRENT_ANIMATION[i] = PLAYER_IDLE;
        CURRENT_ANIMATION_PROGRESS[i] = 0; // doesn't really matter since it only has 1 frame, but lets keep constistency
        *type = PLAYER_IDLE; // sets the ACTUAL PLAYED ANIMATION in the ECS too
    } else if (*type == DYING){ // DEATH ANIMATION ARE NOT CAMERA RELATIVE
        if (CURRENT_ANIMATION[i] != DYING){ // switch to dying animation, doesn't matter if previous isn't done
            ANIMATION_NAME[4] = ANIMATION_DEATH_INIT;
            ANIMATION_NAME[5] = '0';
            ANIMATION_NAME[6] = '\0'; // tell the ECS that there is no second part
            CURRENT_ANIMATION[i] = DYING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
            *type = DYING;
        } else {
            if (ANIMATION_COOLDOWNS[i] < 0) { // shall we continue the animation ?
                ANIMATION_COOLDOWNS[i] = ANIMATION_COOLDOWN;
                CURRENT_ANIMATION_PROGRESS[i] = (CURRENT_ANIMATION_PROGRESS[i] + 1) % ANIMATION_DEATH_FRAME_COUNT;
                if (CURRENT_ANIMATION_PROGRESS[i] == 0) { // animation is done, go to IDLE
                    goto idle;
                }
            }
            ANIMATION_NAME[4] = ANIMATION_DEATH_INIT + CURRENT_ANIMATION_PROGRESS[i];
            ANIMATION_NAME[5] = '0';
            ANIMATION_NAME[6] = '\0';
            *type = DYING;
        }
    } else if (*type == EXTREME_DYING) {
        if (CURRENT_ANIMATION[i] != EXTREME_DYING){
            ANIMATION_NAME[4] = ANIMATION_EXTREME_DEATH_INIT;
            ANIMATION_NAME[5] = '0';
            ANIMATION_NAME[6] = '\0'; 
            CURRENT_ANIMATION[i] = EXTREME_DYING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
            *type = EXTREME_DYING;
        } else {
            if (ANIMATION_COOLDOWNS[i] < 0) {
                ANIMATION_COOLDOWNS[i] = 50;
                CURRENT_ANIMATION_PROGRESS[i] = (CURRENT_ANIMATION_PROGRESS[i] + 1) % ANIMATION_EXTREME_DEATH_FRAME_COUNT;
                if (CURRENT_ANIMATION_PROGRESS[i] == 0) {
                    goto idle;
                }
            }
            ANIMATION_NAME[4] = ANIMATION_EXTREME_DEATH_INIT + CURRENT_ANIMATION_PROGRESS[i];
            ANIMATION_NAME[5] = '0';
            ANIMATION_NAME[6] = '\0';
            *type = EXTREME_DYING;
        }
    } else if (*type == MOVING && has_moved) {
        if (CURRENT_ANIMATION[i] != MOVING) { // switch to moving
            set_orientation(sprite_orientation); // set the orientation of the sprite in ANIMATION_NAME
            ANIMATION_NAME[4] = ANIMATION_WALK_INIT;
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_WALK_INIT;
            }
            CURRENT_ANIMATION[i] = MOVING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
            *type = MOVING;
        } else {
            set_orientation(sprite_orientation);
            if (ANIMATION_COOLDOWNS[i] < 0) {
                ANIMATION_COOLDOWNS[i] = ANIMATION_COOLDOWN;
                CURRENT_ANIMATION_PROGRESS[i] = (CURRENT_ANIMATION_PROGRESS[i] + 1) % ANIMATION_WALK_FRAME_COUNT;
                if (CURRENT_ANIMATION_PROGRESS[i] == 0) {
                    goto idle; // animation is done, go idle
                }
            }
            ANIMATION_NAME[4] = ANIMATION_WALK_INIT + CURRENT_ANIMATION_PROGRESS[i];
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_WALK_INIT + CURRENT_ANIMATION_PROGRESS[i];
            }
            *type = MOVING;
        }
    } else if (*type == SHOOTING) {
        if (CURRENT_ANIMATION[i] != SHOOTING) {
            set_orientation(sprite_orientation);
            ANIMATION_NAME[4] = ANIMATION_SHOOT_INIT;
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_SHOOT_INIT;
            }
            CURRENT_ANIMATION[i] = SHOOTING;
            CURRENT_ANIMATION_PROGRESS[i] = 0;
            *type = SHOOTING;
        } else {
            set_orientation(sprite_orientation);
            if (ANIMATION_COOLDOWNS[i] < 0) {
                ANIMATION_COOLDOWNS[i] = ANIMATION_COOLDOWN;
                CURRENT_ANIMATION_PROGRESS[i] = (CURRENT_ANIMATION_PROGRESS[i] + 1) % ANIMATION_SHOOT_FRAME_COUNT;
                if (CURRENT_ANIMATION_PROGRESS[i] == 0) {
                    goto idle;
                }
            }
            ANIMATION_NAME[4] = ANIMATION_SHOOT_INIT + CURRENT_ANIMATION_PROGRESS[i];
            if (sprite_orientation != 1 && sprite_orientation != 5) {
                ANIMATION_NAME[6] = ANIMATION_SHOOT_INIT + CURRENT_ANIMATION_PROGRESS[i];
            }
            *type = SHOOTING;
        }
    } else { // nothing was found, just keep idling
        goto idle;
    }
    return (5 < sprite_orientation && sprite_orientation < NUMBER_ORIENTATION + 1); // should we use the mirror 
}