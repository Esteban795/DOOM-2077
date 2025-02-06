#include "../../include/core/controller.h"

 int16_t controls[NUM_CONTROLS_CONTROLLER] = {0};

void handle_joyaxismotion(SDL_JoyAxisEvent *event) {
    printf("Axis: %d, Value: %d\n", event->axis, event->value);
    controls[AXIS_OFFSET + event->axis] = event->value;
}

void handle_joybuttondown(SDL_JoyButtonEvent *event) {
printf("Button: %d\n", event->button);
  controls[event->button] = 1;
}

void handle_joybuttonup(SDL_JoyButtonEvent *event) {
    controls[event->button] = 0;
}

void print_controls() {
    printf("Controls: ");
    for (int i = 0; i < NUM_CONTROLS_CONTROLLER; i++) {
        printf("%d ", controls[i]);
    }
    printf("\n");
}