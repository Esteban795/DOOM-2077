#include "../../include/ui/def.h"

UIModule** get_ui_ingame(int* nmodules){
    *nmodules = 1;
    UIModule** modules = malloc(*nmodules * sizeof(UIModule*));

    UILabel* test_label = uilabel_create(0, 0, 0.5, 0.5, UIAP_TOP_LEFT, UIAP_TOP_LEFT, "derp:3", 255, 0, 0, 255);

    UIModule* test_label_module = uimodule_create(69, 1);
    uimodule_set_element(test_label_module, 0, UIET_Label, test_label);

    *(modules) = test_label_module;

    return modules;
}

void free_ui_ingame(UIModule** yay, int nmodules) {
    for (int i=0; i<nmodules; i++){
        uimodule_free(yay[i]);
    }
    free(yay);
}