#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../patches.h"
/*
AVL C implementation
*/

struct Node {
    struct Node* left;
    struct Node* right;
    patch* patch;
    int height;
};

typedef struct Node node;

struct AVL {
    node* root;
    int nb_elts;
};

typedef struct AVL avl_tree;

/*
Returns a new AVL.
*/
avl_tree* avl_new(void);

/*
Checks whether the key already is in the AVL or not
*/
bool avl_member(avl_tree* t,char* key);

/*
Returns elements that are in the AVL tree, in an array. 
Access its length through avl_nb_elements
*/
patch** avl_elements(avl_tree* t);

/*
Returns the current number of element in the AVL
*/
int avl_nb_elements(avl_tree* t);

/*
Returns the height of the AVL passed as an argument
*/
int avl_height(avl_tree* t);

/*
Inserts given element in AVL, if it doesn't already exists.
*/
void avl_insert(avl_tree* t,patch* p);

/*
Removes key from the AVL if it exists in AVL. If not, doesn't do anything.
*/
void avl_remove(avl_tree* t,char* patchname);

/*
Prints out elements of the AVL, sorted.
*/
void avl_print_sorted(avl_tree* t);

/*
Deletes the AVL
*/
void avl_delete(avl_tree* t);
#endif