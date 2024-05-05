#include <stdlib.h>
#include <string.h>

#include "../../include/ecs/component.h"
#include "../../include/component/display_name.h"

const int COMPONENT_TAG_DISPLAY_NAME = 4;

component_t* display_name_create(char* name) {
    display_name_ct* display_name_component = malloc(sizeof(display_name_ct));
    display_name_component->tag = COMPONENT_TAG_DISPLAY_NAME;
    strncpy(display_name_component->name, name, 127);
    display_name_component->name[127] = '\0';
    return (component_t*) display_name_component;
}

extern inline char* display_name_get(display_name_ct* display_name);