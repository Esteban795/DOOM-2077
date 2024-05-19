#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/ecs/entity.h"
#include "../../include/ecs/archetype.h"
#include "../../include/collection/vec.h"

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_compare_entity(void) {
    printf("Testing ecs::archetype::compare_entity...\n");
    entity_t* e1 = malloc(sizeof(entity_t));
    entity_t* e2 = malloc(sizeof(entity_t));
    e1->id = 1;
    e2->id = 2;
    ASSERT(compare_entity(&e1, &e2) == -1, "compare_entity(&e1, &e2) == -1");
    ASSERT(compare_entity(&e2, &e1) == 1, "compare_entity(&e2, &e1) == 1");
    ASSERT(compare_entity(&e1, &e1) == 0, "compare_entity(&e1, &e1) == 0");
    free(e1);
    free(e2);
    return 0;
}

int test_archetype_init(void) {
    printf("Testing ecs::archetype::archetype_init...\n");
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    ASSERT(archetype.entities.length == 0, "archetype.entities.length == 0");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");
    ASSERT(*(int*)archetype.tags.data[0] == 1, "archetype.tags.data[0] == 1");
    ASSERT(*(int*)archetype.tags.data[1] == 2, "archetype.tags.data[1] == 2");
    ASSERT(*(int*)archetype.tags.data[2] == 3, "archetype.tags.data[2] == 3");

    vec_destroy(&archetype.entities, true);
    for (int i = 0; i < (int) archetype.components.length; i++) {
        vec_destroy(archetype.components.data[i], true);
    }
    vec_destroy(&archetype.components, true);
    vec_destroy(&archetype.tags, true);
    return 0;
}

int test_archetype_destroy(void) {
    printf("Testing ecs::archetype::archetype_destroy...\n");
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    archetype_destroy(&archetype);
    ASSERT(archetype.entities.length == 0, "archetype.entities.length == 0");
    ASSERT(archetype.components.length == 0, "archetype.components.length == 0");
    ASSERT(archetype.tags.length == 0, "archetype.tags.length == 0");
    return 0;
}

int test_archetype_add_entity(void) {
    printf("Testing ecs::archetype::archetype_add_entity...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entities to archetype
    entity_t e1;
    e1.id = 0;
    entity_t e2;
    e2.id = 1;

    // Create the components of the entity
    component_t** components = malloc(6 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    components[3] = malloc(sizeof(component_t));
    components[3]->tag = 1;
    components[4] = malloc(sizeof(component_t));
    components[4]->tag = 2;
    components[5] = malloc(sizeof(component_t));
    components[5]->tag = 3;
    archetype_add_entity(&archetype, &e1, &components[3]);
    archetype_add_entity(&archetype, &e2, components);
    //archetype_add_entity(&archetype, &e1, &components[3]);

    // Check if the entities are added to the archetype
    ASSERT(archetype.entities.length == 2, "archetype.entities.length == 2");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Check the components of the entities (and the order)
    ASSERT(((entity_t*) archetype.entities.data[0])->id == 0, "archetype.entities.data[0]->id == 0");
    ASSERT(((entity_t*) archetype.entities.data[1])->id == 1, "archetype.entities.data[1]->id == 1");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[0]) == components[3], "archetype.components.data[0]->data[0] == components[3]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[0]) == components[4], "archetype.components.data[1]->data[0] == components[4]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[0]) == components[5], "archetype.components.data[2]->data[0] == components[5]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[1]) == components[0], "archetype.components.data[0]->data[1] == components[0]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[1]) == components[1], "archetype.components.data[1]->data[1] == components[1]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[1]) == components[2], "archetype.components.data[2]->data[1] == components[2]");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    return 0;
}

int test_archetype_add_entity_unordered(void) {
    printf("Testing ecs::archetype::archetype_add_entity_unordered...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entity to archetype
    entity_t entity;
    entity.id = 1;

    // Create the components of the entity
    component_t** components = malloc(3 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    archetype_add_entity_unordered(&archetype, &entity, components);

    // Check if the entity was added to the archetype
    ASSERT(archetype.entities.length == 1, "archetype.entities.length == 1");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    return 0;
}

int test_sort_components() {
    printf("Testing ecs::archetype::sort_components...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entities to archetype
    entity_t e1;
    e1.id = 1;
    entity_t e2;
    e2.id = 2;

    // Create the components of the entity
    component_t** components = malloc(6 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    components[3] = malloc(sizeof(component_t));
    components[3]->tag = 1;
    components[4] = malloc(sizeof(component_t));
    components[4]->tag = 2;
    components[5] = malloc(sizeof(component_t));
    components[5]->tag = 3;
    archetype_add_entity_unordered(&archetype, &e2, components);
    archetype_add_entity_unordered(&archetype, &e1, &components[3]);

    archetype_sort_components(&archetype);

    // Check if the entities are added to the archetype
    ASSERT(archetype.entities.length == 2, "archetype.entities.length == 2");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Check the components of the entities (and the order)
    ASSERT(((entity_t*) archetype.entities.data[0])->id == 1, "archetype.entities.data[0]->id == 1");
    ASSERT(((entity_t*) archetype.entities.data[1])->id == 2, "archetype.entities.data[1]->id == 2");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[0]) == components[3], "archetype.components.data[0]->data[0] == components[3]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[0]) == components[4], "archetype.components.data[1]->data[0] == components[4]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[0]) == components[5], "archetype.components.data[2]->data[0] == components[5]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[1]) == components[0], "archetype.components.data[0]->data[1] == components[0]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[1]) == components[1], "archetype.components.data[1]->data[1] == components[1]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[1]) == components[2], "archetype.components.data[2]->data[1] == components[2]");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    return 0;
}

int test_archetype_remove_entity(void) {
    printf("Testing ecs::archetype::archetype_remove_entity...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entities to archetype
    entity_t* e1 = malloc(sizeof(entity_t));
    entity_t* e2 = malloc(sizeof(entity_t));
    entity_t* e3 = malloc(sizeof(entity_t)); 
    e1->id = 1;
    e2->id = 2;
    e3->id = 3;

    // Create the components of the entity
    component_t** components = malloc(9 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    components[3] = malloc(sizeof(component_t));
    components[3]->tag = 1;
    components[4] = malloc(sizeof(component_t));
    components[4]->tag = 2;
    components[5] = malloc(sizeof(component_t));
    components[5]->tag = 3;
    components[6] = malloc(sizeof(component_t));
    components[6]->tag = 1;
    components[7] = malloc(sizeof(component_t));
    components[7]->tag = 2;
    components[8] = malloc(sizeof(component_t));
    components[8]->tag = 3;
    archetype_add_entity(&archetype, e2, components);
    archetype_add_entity(&archetype, e1, &components[3]);
    archetype_add_entity(&archetype, e3, &components[6]);

    ASSERT(archetype.entities.length == 3, "after insertion: archetype.entities.length == 3");

    // Remove entity from archetype
    archetype_remove_entity(&archetype, e1, true);

    // Check if the entity was removed from the archetype
    ASSERT(archetype.entities.length == 2, "archetype.entities.length == 2");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Check the components of the entities (and the order)
    ASSERT(((entity_t*) archetype.entities.data[0])->id == 2, "archetype.entities.data[0]->id == 2");
    ASSERT(((entity_t*) archetype.entities.data[1])->id == 3, "archetype.entities.data[1]->id == 3");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[0]) == components[0], "archetype.components.data[0]->data[0] == components[0]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[0]) == components[1], "archetype.components.data[1]->data[0] == components[1]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[0]) == components[2], "archetype.components.data[2]->data[0] == components[2]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[0])->data[1]) == components[6], "archetype.components.data[0]->data[1] == components[6]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[1])->data[1]) == components[7], "archetype.components.data[1]->data[1] == components[7]");
    ASSERT(((component_t*) ((vec_t*) archetype.components.data[2])->data[1]) == components[8], "archetype.components.data[2]->data[1] == components[8]");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    free(e1);
    free(e2);
    free(e3);
    return 0;
}

int test_archetype_remove_entity_unordered(void) {
    printf("Testing ecs::archetype::archetype_remove_entity_unordered...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entity to archetype
    entity_t* entity = malloc(sizeof(entity_t));
    entity->id = 0;

    // Create the components of the entity
    component_t** components = malloc(3 * sizeof(component_t*));
    components[0] = malloc(sizeof(component_t));
    components[0]->tag = 1;
    components[1] = malloc(sizeof(component_t));
    components[1]->tag = 2;
    components[2] = malloc(sizeof(component_t));
    components[2]->tag = 3;
    archetype_add_entity_unordered(&archetype, entity, components);
    ASSERT(archetype.entities.length == 1, "archetype.entities.length == 1");

    // Remove entity from archetype
    archetype_remove_entity_unordered(&archetype, entity, true);

    // Check if the entity was removed from the archetype
    ASSERT(archetype.entities.length == 0, "archetype.entities.length == 0");
    ASSERT(archetype.components.length == 3, "archetype.components.length == 3");
    ASSERT(archetype.tags.length == 3, "archetype.tags.length == 3");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    free(entity);
    return 0;
}

int test_archetype_get_component() {
    printf("Testing ecs::archetype::archetype_get_component...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
    
    // Add entity to archetype
    entity_t* entity = malloc(sizeof(entity_t));
    entity->id = 0;

    // Create the components of the entity
    component_t** components = malloc(3 * (sizeof(component_t*)));
    components[0] = malloc(sizeof(component_t) + sizeof(uint8_t));
    components[0]->tag = 1;
    *components[0]->data = (uint8_t) 1;
    components[1] = malloc(sizeof(component_t) + sizeof(uint8_t));
    components[1]->tag = 2;
    *components[1]->data = (uint8_t) 2;
    components[2] = malloc(sizeof(component_t) + sizeof(uint8_t));
    components[2]->tag = 3;
    *components[2]->data = (uint8_t) 3;
    archetype_add_entity_unordered(&archetype, entity, components);

    // Get the components of the entity
    component_t* c1 = archetype_get_component(&archetype, entity, 1);
    component_t* c2 = archetype_get_component(&archetype, entity, 2);
    component_t* c3 = archetype_get_component(&archetype, entity, 3);

    // Check if the components are correct
    ASSERT(c1->tag == 1, "c1->tag == 1");
    ASSERT(c2->tag == 2, "c2->tag == 2");
    ASSERT(c3->tag == 3, "c3->tag == 3");
    ASSERT(*(uint8_t*) c1->data == 1, "*(int*) c1->data == 1");
    ASSERT(*(uint8_t*) c2->data == 2, "*(int*) c2->data == 2");
    ASSERT(*(uint8_t*) c3->data == 3, "*(int*) c3->data == 3");

    // Free archetype
    archetype_destroy(&archetype);
    free(components);
    free(entity);
    return 0;
}

int test_archetype_match() {
    printf("Testing ecs::archetype::archetype_match...\n");
    // Initialize archetype
    archetype_t archetype;
    int component_tags[] = {1, 2, 3};
    archetype_init(&archetype, 3, component_tags);
  
    // Create an archetype_tag
    archetype_tag_t archetype_tag;
    archetype_tag.component_tags = malloc(3 * sizeof(int));
    archetype_tag.component_tags[0] = 3;
    archetype_tag.component_tags[1] = 1;
    archetype_tag.component_tags[2] = 2;
    archetype_tag.component_count = 3;

    archetype_t* arch = &archetype;
    archetype_tag_t* tag = &archetype_tag;

    // Check if the archetypes match
    ASSERT(archetype_match(&tag, &arch) == 0, "archetype_match(&tag, &arch) == 0");

    archetype_tag.component_count = 2;
    ASSERT(archetype_match(&tag, &arch) < 0, "archetype_match(&tag, &arch) < 0");

    archetype_tag.component_count = 3;
    archetype_tag.component_tags[0] = 0;
    ASSERT(archetype_match(&tag, &arch) < 0, "archetype_match(&tag, &arch) < 0");

    archetype_tag.component_tags[0] = 2;
    archetype_tag.component_tags[1] = 3;
    archetype_tag.component_tags[2] = 4;
    ASSERT(archetype_match(&tag, &arch) > 0, "archetype_match(&tag, &arch) > 0");

    // Free archetype
    archetype_destroy(&archetype);
    free(archetype_tag.component_tags);
    return 0;
}

int test_archetype(void) {
    int failed = 0;
    printf("Testing libecs/archetype...\n");
    failed += test_compare_entity();
    failed += test_archetype_init();
    failed += test_archetype_destroy();
    failed += test_archetype_add_entity();
    failed += test_archetype_add_entity_unordered();
    failed += test_sort_components();
    failed += test_archetype_remove_entity();
    failed += test_archetype_remove_entity_unordered();
    failed += test_archetype_get_component();
    failed += test_archetype_match();
    printf("libecs/archetype tests finished with %d failures.\n", failed);
    return failed;
}