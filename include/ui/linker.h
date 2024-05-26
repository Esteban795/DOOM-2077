//
// Helper macros for UI updating!
//

#ifndef UI_LINKER_H
#define UI_LINKER_H

#include "module.h"

// #¶egion Menu
#define UILINK_GET_CONNECTION_NAME(uimodules) (((UITextBox*)uimodules[3]->elements[6]->element)->text)
#define UILINK_GET_CONNECTION_IP(uimodules) (((UITextBox*)uimodules[3]->elements[8]->element)->text)
// #endregion

// #region In-game
#define UILINK_SET_HEALTH(uimodules,hp) uilabel_set_content(((UILabel*)uimodules[1]->elements[0]->element), hp)
#define UILINK_SET_AMMO(uimodules,ammo) uilabel_set_content(((UILabel*)uimodules[1]->elements[2]->element), ammo)
#define UILINK_SET_AMMO_MAX(uimodules,ammo) uilabel_set_content(((UILabel*)uimodules[1]->elements[1]->element), ammo)

#define UILINK_FEED_KILL(uimodules,msg) uifeed_append(((UIFeed*)uimodules[2]->elements[0]->element), msg)
#define UILINK_FEED_CHAT(uimodules,msg) uifeed_append(((UIFeed*)uimodules[3]->elements[5]->element), msg)

#define UILINK_SET_SB_N(uimodules,nth,content) uilabel_set_content(((UILabel*)uimodules[4]->elements[nth]->element), content)
// #endregion

#endif