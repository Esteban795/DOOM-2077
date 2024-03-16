#ifndef WEAPONS_H
#define WEAPONS_H

typedef struct Weapon{
    /*Identification de l'arme*/
    int id; /*Identifie précisement l'arme*/
    char* weapon_name; /*Nom de l'arme du coup*/
    char* sprite; /*Fichier contenant le/les sprite de l'arme*/

    /*Spécification de l'arme*/
    int magsize; /*Taille du chargeur*/
    int max_damage; /*Dégats max possible par balle*/
    int min_damage; /*Dégats min possible*/
    double fire_rate; /*Nombre de balle tirées/s*/
    double spray; /*Potentiel rayon de dispersion*/
    int ammo_bounce; /*Nombre de rebond sur les murs*/
    int ammo_id; /*ID des (types de) munitions utilisées */
    int type; /*Eventuellement si on veut rajouter/classifier les armes (melee vs range, hitscan vs projectile....)*/
} weapon;

#endif