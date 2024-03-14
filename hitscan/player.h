typedef struct _player {
    int life;
    double posx;
    double posy;
    double angle;
}player;


void create_player(double posx ,double posy,int num_player,player** players);

player** create_players(int num_players);

void rotate_player(player** players,int num_player);

void move_player_forward(player** players,int num_player);


