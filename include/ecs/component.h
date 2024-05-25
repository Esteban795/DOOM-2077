#ifndef _LIB_DOOM_ECS_COMPONENT_H
#define _LIB_DOOM_ECS_COMPONENT_H

#include <stdint.h>

/**
* Component representation
*
* A component is a piece of data that is attached to an entity. It is used to store data that is relevant to the entity.
* The goal of using components is to make the code more modular and easier to maintain, therefore a component is able to be 
* attached to multiple entities of different types.
*
* The tag field is used to identify the type of the component. To use a component, you must cast it to the correct type.
*/
typedef struct {
    int tag;
    uint8_t data[];
} component_t;

#endif