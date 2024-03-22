#include "../../include/ui/label.h"

UILabel* uilabel_create(float x, float y, float w, float h,
                        UIAnchorPoint m_anchor, UIAnchorPoint text_anchor, char* string,
                        int r, int g, int b, int a) {
    UILabel* uilabel = malloc(sizeof(UILabel));

    uilabel->common.x = x;
    uilabel->common.y = y;
    uilabel->common.width = w;
    uilabel->common.height = h;
    uilabel->common.anchor = m_anchor;

    uilabel->text_anchor = text_anchor;
    uilabel->string = string;

    uilabel->color.r = r;
    uilabel->color.g = g;
    uilabel->color.b = b;
    uilabel->color.a = a;

    return uilabel;
}

void uilabel_free(UILabel* uilabel) {
    free(uilabel);
}

void uilabel_render(UILabel* uilabel) {
    printf("%s\n", uilabel->string);
    return;
}