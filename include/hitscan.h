#include "player.h" //Utilisation de create_bullet qui demande le struct player plus loin, c'est moche mais j'ai pas trouvé mieux
//On considere la map vue du dessus comme un plan, un angle de zéro consiste du pov du dessus a regarder vers la droite
#include <math.h>
#include "geometry.h"
#include "remote.h"

typedef struct _bullet{
    double posx;
    double posy;
    double angle;
} bullet;



bullet *create_bullet(player* player_);

double distance(double posx_a,double posy_a,double posx_b,double posy_b);

void fire_bullet(entity_t** players, int num_players, player *player_,weapons_array* weapons_list);

int correct_height(linedef* wall, int height);

linedef *cast_ray(linedef** linedefs, int len_linedefs,vec2 player_pos, double player_angle,double height);

void aux_fire_bullet(engine* mr,weapons_array* wa);

