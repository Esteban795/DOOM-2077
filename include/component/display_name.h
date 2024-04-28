#ifndef _LIB_DOOM_COMPONENT_DISPLAY_NAME_H
#define _LIB_DOOM_COMPONENT_DISPLAY_NAME_H

#include "../ecs/component.h"

extern const int COMPONENT_TAG_DISPLAY_NAME;

/*
* display_name_ct is a component that represents the display name of an entity.
* It contains information about the name of the entity.
*
* Note: Name is at most 127 characters long.
*/
typedef struct {
    int tag;
    char name[128];
} display_name_ct;

// display_name_create creates a new display name component with the given name.
component_t* display_name_create(char* name);

char* display_name_get(display_name_ct* display_name);

#endif