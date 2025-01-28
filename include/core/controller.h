#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>

#define NUM_CONTROLS_CONTROLLER 12
#define AXIS_OFFSET 6
/*
Using this controller Microsoft X-Box One S pad

0 - button A
1 - button B
2 - button X
3 - button Y
4 - left bumper
5 - right bumper
6 - left joystick horizontal movement
7 - left joystick vertical movement
8 - left trigger
9 - right joystick horizontal movement
10 - right joystick vertical movement
11 - right trigger
*/
uint16_t controls[NUM_CONTROLS_CONTROLLER];


void handle_joyaxismotion(SDL_JoyAxisEvent *event);

void handle_joybuttondown(SDL_JoyButtonEvent *event);

void handle_joybuttonup(SDL_JoyButtonEvent *event);


#endif