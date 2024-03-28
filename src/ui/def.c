#include "../../include/ui/def.h"

void none(void) {
    return;
}

UIModule** get_ui_ingame(int* nuimodules){
    *nuimodules = 1;
    UIModule** modules = malloc(*nuimodules * sizeof(UIModule*));

    SDL_Color bg = {.r=0xFF, .g=0x00, .b=0x00, .a=0xFF};
    SDL_Color br = {.r=0x00, .g=0xFF, .b=0x00, .a=0xFF};

    UIButton* test = uibutton_create(0,0,0.5,0.5,UIAP_TOP_LEFT, bg, br, NULL, NULL, *none);

    UIModule* test_module = uimodule_create(69, 1);
    uimodule_set_element(test_module, 0, UIET_Button, test);

    *(modules) = test_module;

    return modules;
}

void free_ui_ingame(UIModule** yay) {
    for (int i=0; i<1; i++){
        uimodule_free(yay[i]);
    }
    free(yay);
}