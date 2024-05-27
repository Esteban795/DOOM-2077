#include "../../include/component/animation.h"

const int COMPONENT_TAG_ANIMATION = 9;

component_t* animation_create(enum AnimationType animation) {
    animation_ct* new_animation = malloc(sizeof(animation_ct));
    new_animation->tag = COMPONENT_TAG_ANIMATION;
    new_animation->animation = animation;
    return (component_t*)new_animation;
}

extern inline enum AnimationType animation_get(animation_ct* animation);

inline enum AnimationType animation_set(animation_ct* animation, enum AnimationType animation_value){
    if (animation_value > animation->animation) {
        animation->animation = animation_value;
    }
    return animation->animation;
}