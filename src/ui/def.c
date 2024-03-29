#include "../../include/ui/def.h"

UIModule** get_ui_ingame(int* nuimodules){
    *nuimodules = 0;
    UIModule** modules = malloc(*nuimodules * sizeof(UIModule*));

    return modules;
}

void free_ui_ingame(UIModule** yay) {
    for (int i=0; i<0; i++){
        uimodule_free(yay[i]);
    }
    free(yay);
}