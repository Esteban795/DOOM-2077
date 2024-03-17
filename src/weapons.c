#include "../include/weapons.h"


void switch_weapon(player* p, int weapon_id){
    p->active_weapon = weapon_id;
}

weapon* init_one_weapon(int id, char* weapon_name, char* sprite, int magsize, int max_damage, int min_damage, double fire_rate, double spray, int ammo_bounce, int ammo_id, int type){
    weapon* w = malloc(sizeof(weapon));
    w->id = id;
    w->weapon_name = weapon_name;
    w->sprite = sprite;
    w->magsize = magsize;
    w->max_damage = max_damage;
    w->min_damage = min_damage;
    w->fire_rate = fire_rate;
    w->spray = spray;
    w->ammo_bounce = ammo_bounce;
    w->ammo_id = ammo_id;
    w->type = type;
    return w;
}

weapons_array* init_weapons_array(){
    weapons_array* wa = malloc(sizeof(weapons_array));

    FILE* f = fopen("weapons.csv","r");
    if(f==NULL){
        perror("The file couldn't be opened \n");
        free(wa);
        return NULL;
    }

    int weapons_number;
    fscanf(f,"%d\n",&weapons_number);
    wa->weapons_number = weapons_number;

    // Skip the header line
    char header[1024];
    fgets(header, sizeof(header), f);
    printf("header: %s\n",header);

    WeaponInventory winv = malloc(weapons_number*sizeof(weapon*));
    for(int i = 0; i<weapons_number;i++){
        char line[1024];
        int id;
        char weapon_name[1024];
        char sprite[1024];
        int magsize; 
        int max_damage; 
        int min_damage; 
        double fire_rate; 
        double spray; 
        int ammo_bounce; 
        int ammo_id; 
        int type; 

        fgets(line,sizeof(line),f);
        sscanf(line, "%d,%1023[^,],%1023[^,],%d,%d,%d,%lf,%lf,%d,%d,%d", &id, weapon_name, sprite, &magsize, &max_damage, &min_damage, &fire_rate, &spray, &ammo_bounce, &ammo_id, &type);
        printf("id: %d, weapon_name: %s, sprite: %s, magsize: %d, max_damage: %d, min_damage: %d, fire_rate: %lf, spray: %lf, ammo_bounce: %d, ammo_id: %d, type: %d\n",id,weapon_name,sprite,magsize,max_damage,min_damage,fire_rate,spray,ammo_bounce,ammo_id,type);
        winv[i] = NULL;
    }
    wa->weapons = winv;

    if(fclose(f) !=0){
        perror("Error while closing the file");
        free(wa);
        return NULL;
    }
    return wa;
}

void free_weapons_array(weapons_array* wa){
    for(int i = 0; i<wa->weapons_number;i++){
        if (wa->weapons[i] != NULL){
            free(wa->weapons[i]);
        }
    }
    free(wa->weapons);
    free(wa);
}