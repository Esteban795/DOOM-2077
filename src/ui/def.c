#include "../../include/ui/def.h"

UIModule** get_ui_ingame(int* nuimodules){
    *nuimodules = 1;
    UIModule** modules = malloc(*nuimodules * sizeof(UIModule*));

    TTF_Font* font = TTF_OpenFont("./fonts/lemon_milk/LEMONMILK-Regular.otf", 50);
    if (font == NULL){
        printf("%s\n", SDL_GetError());
        exit(1);
    }

    UILabel* test_label = uilabel_create(0, 0, 1, 1, UIAP_TOP_LEFT, UIAP_TOP_RIGHT, "bonjour la team ce texte est un peu long", 255, 0, 0, 255, font);

    UIModule* test_label_module = uimodule_create(69, 1);
    uimodule_set_element(test_label_module, 0, UIET_Label, test_label);

    *(modules) = test_label_module;

    return modules;
}

void free_ui_ingame(UIModule** yay) {
    TTF_CloseFont(((UILabel*)yay[0])->font);
    for (int i=0; i<1; i++){
        uimodule_free(yay[i]);
    }
    free(yay);
}