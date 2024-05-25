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
#define UILINK_SET_HEALTH(uimodules,health) sprintf(((UILabel*)uimodules[1]->elements[0]->element)->text, "%i", health)
#define UILINK_SET_AMMO(uimodules,ammo) sprintf(((UILabel*)uimodules[1]->elements[2]->element)->text, "%i", ammo)
#define UILINK_SET_AMMO_MAX(uimodules,ammo) sprintf(((UILabel*)uimodules[1]->elements[1]->element)->text, "/%i", ammo)

#define UILINK_FEED_KILL(uimodules,msg) uifeed_append(((UIFeed*)uimodules[2]->elements[0]->element), msg)
#define UILINK_FEED_CHAT(uimodules,msg) uifeed_append(((UIFeed*)uimodules[3]->elements[5]->element), msg)

#define UILINK_SB_N(uimodules,nth,player_name,score) sprintf(((UILabel*)uimodules[4]->elements[nth]->element)->text, "%d - %s [%i]", nth+1, player_name, score)
#define UILINK_SB_1(uimodules,player_name,score) sprintf(((UILabel*)uimodules[4]->elements[0]->element)->text, "1 - %s [%i]", player_name, score)
#define UILINK_SB_2(uimodules,player_name,score) sprintf(((UILabel*)uimodules[4]->elements[1]->element)->text, "2 - %s [%i]", player_name, score)
#define UILINK_SB_3(uimodules,player_name,score) sprintf(((UILabel*)uimodules[4]->elements[2]->element)->text, "3 - %s [%i]", player_name, score)
#define UILINK_SB_4(uimodules,player_name,score) sprintf(((UILabel*)uimodules[4]->elements[3]->element)->text, "4 - %s [%i]", player_name, score)
// #endregion

#endif