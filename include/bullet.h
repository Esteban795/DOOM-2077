#include "player.h" //Utilisation de create_bullet qui demande le struct player plus loin, c'est moche mais j'ai pas trouvé mieux
//On considere la map vue du dessus comme un plan, un angle de zéro consiste du pov du dessus a regarder vers la droite
#include <math.h>
#include "collision_map.h"
#include "geometry.h"

typedef struct _bullet{
    double posx;
    double posy;
    double angle;
} bullet;



bullet *create_bullet(player* player_);

double distance(double posx_a,double posy_a,double posx_b,double posy_b);

void fire_bullet(player** players,int num_players,player*  player_,int damage,int** collision_map);

void fire_bullet2(player** players,int num_players,player* player_,int damage);
