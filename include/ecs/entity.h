#ifndef _LIB_DOOM_ECS_ENTITY_H
#define _LIB_DOOM_ECS_ENTITY_H

#include <stdint.h>

/**
 * Entity representation
 *
 * An entity is basically just an identifier. It is used to retrieve its components easily.
 */
typedef struct {
    uint64_t id;
} entity_t;

#define ENTITY_BY_ID(id_) (entity_t) { .id = id_ }

#endif