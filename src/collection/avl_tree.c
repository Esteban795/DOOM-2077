#include "../../include/collection/avl_tree.h"
#include <stdbool.h>
#include <stdio.h>

int max(int a,int b){
    return a > b ? a : b;
}

node* new_node(patch* p){
    node* n = malloc(sizeof(node));
    n->height = 0;
    n->patch = p;
    n->left = NULL;
    n->right = NULL;
    return n;
}

avl_tree* avl_new(void){
    avl_tree* t = malloc(sizeof(avl_tree));
    t->root = NULL;
    t->size = 0;
    return t;
}

bool node_member(node* n, char* key){
    if (n == NULL || key[0] == '\0') return false;
    if (strcmp(key,n->patch->patchname) < 0) node_member(n->left,key);
    if (strcmp(key, n->patch->patchname) > 0) node_member(n->right,key);
    return true;
}

// Check if a key is in the tree
bool avl_member(avl_tree* t,char* key){
    return node_member(t->root,key);
}

int avl_size(avl_tree* t){
    return t->size;
}

void node_elements(node* n,patch** tab_elts,int* i){
    if (n == NULL) return;
    if (n->left != NULL) node_elements(n->left,tab_elts,i);
    tab_elts[*i] = n->patch;
    (*i)++;
    if (n->right != NULL) node_elements(n->right,tab_elts,i);
}

// Returns the elements that are in the tree, in a sorted array.
patch** avl_elements(avl_tree* t){
    patch** elts = malloc(sizeof(patch*) * t->size);
    int i = 0;
    node_elements(t->root,elts,&i);
    return elts;
}

int node_height(node* n){
    if (n == NULL) return -1;
    return 1 + max(node_height(n->left),node_height(n->right)); 
}

int avl_height(avl_tree* t){
    return node_height(t->root);
}

/* 
Does the following operation :

          y                         x
         |  \                      /  |
        x    c     <------        a    y
      /   \                           /  \
      a    b                         b    c
*/  
node* left_rotate(node* x){
    node* y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = 1 + max(node_height(x->left),node_height(x->right));
    y->height = 1 + max(node_height(y->left),node_height(y->right));
    return y;
}

/* 
Does the following operation : 

          y                         x
         |  \                      /  |
        x    c     ------>        a    y
      /   \                           /  \
      a    b                         b    c
*/     
node* right_rotate(node* y){
    node* x = y->left;
    y->left = x->right;
    x->right = y;
    x->height = 1 + max(node_height(x->left),node_height(x->right));
    y->height = 1 + max(node_height(y->left),node_height(y->right));
    return x;
}

int get_node_balance(node* n){
    if (n == NULL) return 0;
    return node_height(n->left) - node_height(n->right);
}

node* insert_node(node* n,patch* p,bool* inserted){
    if (n == NULL){
        *inserted = true;
        return new_node(p);
    }
    int compare = strcmp(p->patchname,n->patch->patchname);
    if (compare < 0) n->left = insert_node(n->left,p,inserted); //traversing the tree
    else if (compare > 0) n->right = insert_node(n->right,p,inserted);
    else return n; //element already exists

    //Re-balancing the tree.
    //updating n's height
    n->height = 1 + max(node_height(n->left),node_height(n->right));
    int n_balance = get_node_balance(n);
    if (n_balance > 1 && strcmp(p->patchname, n->left->patch->patchname) < 0) return right_rotate(n);
    if (n_balance > 1 && strcmp(p->patchname, n->left->patch->patchname) > 0) {
        n->left = left_rotate(n->left);
        return right_rotate(n);
    }

    if (n_balance < -1 && strcmp(p->patchname, n->right->patch->patchname) < 0) return left_rotate(n);
    if (n_balance < -1 && strcmp(p->patchname, n->right->patch->patchname) > 0){
        n->right = right_rotate(n->right);
        return left_rotate(n);
    }
    return n;
}

bool avl_insert(avl_tree* t,patch* p){
    bool inserted = false;
    t->root = insert_node(t->root,p,&inserted);
    if (inserted) t->size++;
    return inserted;
}

node* find_min(node* n) {
    if (n == NULL) return NULL;
    while (n->left){
        n = n->left;
    }
    return n;
}

node* node_remove(node* n,char* patchname,bool* removed){
    if (n == NULL) return NULL;
    if (strcmp(patchname, n->patch->patchname) < 0) n->left = node_remove(n->left,patchname,removed); // traversing the tree
    else if (strcmp(patchname, n->patch->patchname) > 0) n->right = node_remove(n->right,patchname,removed);
    else  { // we found the node that is going to get deleted
        *removed = true;
        if (n->left == NULL || n->right == NULL){
            node* temp = n->left == NULL ? n->left : n->right;
            if (temp == NULL) { // leaf node
                temp = n;
                n = NULL;
            } else {
                *n = *temp; // overwrites the parent with child data, free child
            }
            free(temp);
        } else {
            node* temp = find_min(n->right);
            n->patch = temp->patch;
            n->right = node_remove(n->right,temp->patch->patchname,removed);
        }
    }
    if (n == NULL) return NULL; //means n was the root, so the tree is now empty

    // Rebalancing tree
    n->height = 1 + max(node_height(n->left),node_height(n->right));
    int n_balance = get_node_balance(n);
    if (n_balance > 1 && get_node_balance(n->left) >= 0) return right_rotate(n);
    if (n_balance > 1 && get_node_balance(n->left) < 0){
        n->left = left_rotate(n->left);
        return right_rotate(n);
    }

    if (n_balance < -1 && get_node_balance(n->right) <= 0) return left_rotate(n);
    if (n_balance < -1 && get_node_balance(n->right) > 0){
        n->right = left_rotate(n->right);
        return left_rotate(n);
    }
    return n;
}

bool avl_remove(avl_tree* t,char* patchname){
    bool removed = false;
    t->root = node_remove(t->root,patchname,&removed);
    if (removed) {
        t->size--;
    }
    return removed;
}

void print_sorted(node* n) {
  if (n != NULL) {
    print_sorted(n->left);
    printf("%s ", n->patch->patchname);
    print_sorted(n->right);
  }
}

void avl_print_sorted(avl_tree* t){
    print_sorted(t->root);
}

void node_delete(node* n){
    if (n == NULL) return;
    node_delete(n->left);
    node_delete(n->right);
    free(n);
}

void avl_delete(avl_tree* t){
    node_delete(t->root);
    free(t);
    t = NULL;
}


// int main() {
//     avl_tree* t = avl_new();
//     patch* p1 = malloc(sizeof(patch));
//     p1->patchname = "patch1";
//     patch* p2 = malloc(sizeof(patch));
//     p2->patchname = "patch2";
//     patch* p3 = malloc(sizeof(patch));
//     p3->patchname = "patch3";
//     patch* p4 = malloc(sizeof(patch));
//     p4->patchname = "patch4";
//     patch* p5 = malloc(sizeof(patch));
//     p5->patchname = "patch5";
//     patch* p6 = malloc(sizeof(patch));
//     p6->patchname = "patch6";
//     patch* p7 = malloc(sizeof(patch));
//     p7->patchname = "patch7";
//     patch* p8 = malloc(sizeof(patch));
//     p8->patchname = "patch8";
//     avl_insert(t, p2);
//     printf("Inserted %s\n",p2->patchname);
//     fflush(stdout);

//     avl_insert(t, p1);
//     printf("Inserted %s\n",p1->patchname);
//     fflush(stdout);
    
//     avl_insert(t, p7);
//     printf("Inserted %s\n",p7->patchname);
//     fflush(stdout);
//     printf("Height of tree : %d\n",avl_height(t));

//     avl_insert(t, p4);
//     printf("Inserted %s\n",p4->patchname);
//     printf("Height of tree : %d\n",avl_height(t));
//     fflush(stdout);

//     avl_insert(t, p5);
//     printf("Inserted %s\n",p5->patchname);
//     printf("Height of tree : %d\n",avl_height(t));
//     fflush(stdout);

//     avl_insert(t, p3);
//     printf("Inserted %s\n",p3->patchname);
//     printf("Height of tree : %d\n",avl_height(t));
//     fflush(stdout);

//     avl_insert(t, p8);
//     printf("Inserted %s\n",p8->patchname);
//     printf("Height of tree : %d\n",avl_height(t));
//     fflush(stdout);

//     avl_insert(t, p6);
//     printf("Inserted %s\n",p6->patchname);
//     printf("Height of tree : %d\n",avl_height(t));
//     printf("Nb d'éléments : %d\n",t->size);
//     fflush(stdout);
//     patch** elts = avl_elements(t);
//     for (int i = 0; i < t->size;i++){
//         printf("%s ",elts[i]->patchname);
//     }
//     printf("\n\n");
//     free(elts);
//     avl_remove(t,p3->patchname);
//     printf("After deletion of %s\n",p3->patchname);
//     fflush(stdout);
//     printf("\nAfter deletion: ");
//     avl_print_sorted(t);
//     printf("\n");
//     printf("Root is %s, tree height is : %d\n",t->root->patch->patchname,avl_height(t));
//     printf("\n");
//     printf("%s is member of tree : %d\n",p3->patchname,avl_member(t,p3->patchname));
//     printf("%s is member of tree : %d\n",p2->patchname,avl_member(t,p2->patchname));
//     printf("\n");
//     free(p1);
//     free(p2);
//     free(p3);
//     free(p4);
//     free(p5);
//     free(p6);
//     free(p7);
//     free(p8);
//     avl_delete(t);
//     return 0;
// }

