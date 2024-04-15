#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef _LIB_DOOM_COLLECTION_AVL_TREE_H
#include "../../include/collection/avl_tree.h"
#endif

#ifndef ASSERT
#define ASSERT(x, msg) if (!(x)) { printf("[%s:%d]assertion failed: %s\n", __FILE__, __LINE__, msg); return 1; } 
#endif

int test_avl_new() {
    printf("collection::avl::avl_new...\n");
    avl_tree* tree = avl_new();
    ASSERT(tree->root == NULL, "avl_init: root is not NULL");
    ASSERT(tree->size == 0, "avl_init: nb_elts is not 0");
    return 0;
}

int test_avl_delete() {
    printf("collection::avl::avl_delete...\n");
    avl_tree* tree = avl_new();
    avl_delete(tree);
    ASSERT(tree == NULL, "avl_destroy: tree is not NULL");
    return 0;
}

int test_avl_insert_one() {
    printf("collection::avl::avl_insert...\n");
    avl_tree* tree = avl_new();
    patch* p = malloc(sizeof(patch));
    p->patchname = "patch1";
    bool res = avl_insert(tree,p);
    ASSERT(res == true, "avl_push: res is not true");
    ASSERT(tree->size == 1, "avl_push: nb_elts is not 1");
    ASSERT(tree->root->patch == p, "avl_push: root->patch is not p");
    free(p);
    avl_delete(tree);
    return 0;
}
 
int test_avl_insert_many() {
    printf("collection::avl::avl_insert_many...\n");
    avl_tree* tree = avl_new();
    patch* p1 = malloc(sizeof(patch));
    p1->patchname = "patch1";
    patch* p2 = malloc(sizeof(patch));
    p2->patchname = "patch2";
    patch* p3 = malloc(sizeof(patch));
    p3->patchname = "patch3";
    bool res1 = avl_insert(tree,p2);
    bool res2 = avl_insert(tree,p1);
    bool res3 = avl_insert(tree,p3);
    ASSERT(res2 == true, "avl_push: res2 is not true");
    ASSERT(res1 == true, "avl_push: res1 is not true");
    ASSERT(res3 == true, "avl_push: res3 is not true");
    ASSERT(tree->size == 3, "avl_push: nb_elts is not 3");
    free(p1);
    free(p2);
    free(p3);
    avl_delete(tree);
    return 0;
}

int test_avl_member() {
    printf("collection::avl::avl_member...\n");
    avl_tree* tree = avl_new();
    patch* p1 = malloc(sizeof(patch));
    p1->patchname = "patch1";
    patch* p2 = malloc(sizeof(patch));
    p2->patchname = "patch2";
    patch* p3 = malloc(sizeof(patch));
    p3->patchname = "patch3";
    avl_insert(tree,p1);
    avl_insert(tree,p2);
    avl_insert(tree,p3);
    bool res1 = avl_member(tree,p1->patchname);
    bool res2 = avl_member(tree,p2->patchname);
    bool res3 = avl_member(tree,p3->patchname);
    bool res4 = avl_member(tree,"patch4");
    ASSERT(res1 == true, "avl_member: res1 is not true");
    ASSERT(res2 == true, "avl_member: res2 is not true");
    ASSERT(res3 == true, "avl_member: res3 is not true");
    ASSERT(res4 == false, "avl_member: res4 is not false");
    free(p1);
    free(p2);
    free(p3);
    avl_delete(tree);
    return 0;
}

int test_avl_height(){
    printf("collection::avl::avl_height...\n");
    avl_tree* tree = avl_new();
    patch* p1 = malloc(sizeof(patch));
    p1->patchname = "patch1";
    patch* p2 = malloc(sizeof(patch));
    p2->patchname = "patch2";
    patch* p3 = malloc(sizeof(patch));
    p3->patchname = "patch3";
    avl_insert(tree,p2);
    ASSERT(avl_height(tree) == 0, "avl_height: height is not 0");
    avl_insert(tree,p1);
    avl_insert(tree,p3);
    ASSERT(avl_height(tree) == 1, "avl_height: height is not 1");
    free(p1);
    free(p2);
    free(p3);
    avl_delete(tree);
    return 0;
}

int test_avl_elements() {
    printf("collection::avl::avl_elements...\n");
    avl_tree* tree = avl_new();
    patch* p1 = malloc(sizeof(patch));
    p1->patchname = "patch1";
    patch* p2 = malloc(sizeof(patch));
    p2->patchname = "patch2";
    patch* p3 = malloc(sizeof(patch));
    p3->patchname = "patch3";
    avl_insert(tree,p2);
    patch** elements = avl_elements(tree);
    ASSERT(elements[0] == p2, "avl_elements: elements[0] is not p2");
    free(elements);
    avl_insert(tree,p1);
    patch** elements2 = avl_elements(tree);
    ASSERT(elements2[0] == p1, "avl_elements: elements2[0] is not p1");
    ASSERT(elements2[1] == p2, "avl_elements: elements2[1] is not p2");
    free(elements2);
    avl_insert(tree,p3);
    patch** elements3 = avl_elements(tree);
    ASSERT(elements3[0] == p1, "avl_elements: elements3[0] is not p1");
    ASSERT(elements3[1] == p2, "avl_elements: elements3[1] is not p2");
    ASSERT(elements3[2] == p3, "avl_elements: elements3[2] is not p3");
    ASSERT(tree->size == 3, "avl_elements: nb_elts is not 3");
    ASSERT(avl_height(tree) == 1, "avl_elements: height is not 1");
    free(elements3);
    free(p1);
    free(p2);
    free(p3);
    avl_delete(tree);
    return 0;
}

int test_avl_remove(){
    avl_tree* t = avl_new();
    patch* p1 = malloc(sizeof(patch));
    p1->patchname = "patch1";
    patch* p2 = malloc(sizeof(patch));
    p2->patchname = "patch2";
    patch* p3 = malloc(sizeof(patch));
    p3->patchname = "patch3";
    patch* p4 = malloc(sizeof(patch));
    p4->patchname = "patch4";
    avl_insert(t,p2);
    avl_insert(t,p1);
    avl_insert(t,p3);
    avl_insert(t,p4);
    bool was_deleted = avl_remove(t,"patch4");
    ASSERT(was_deleted == true, "avl_remove: patch4 was not deleted");
    ASSERT(avl_member(t,"patch4") == false, "avl_remove: patch4 is still in the tree");
    ASSERT(t->size == 3, "avl_remove: size is not 3");
    patch** elements = avl_elements(t);
    ASSERT(elements[0] == p1, "avl_remove: elements[0] is not p1");
    ASSERT(elements[1] == p2, "avl_remove: elements[1] is not p2");
    ASSERT(elements[2] == p3, "avl_remove: elements[2] is not p3");
    free(elements);
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    avl_delete(t);
    return 0;
}

int test_avl() {
    int failed = 0;
    printf("Testing libcollection/vec...\n");
    failed += test_avl_new();
    failed += test_avl_delete();
    failed += test_avl_insert_one();
    failed += test_avl_insert_many();
    failed += test_avl_member();
    failed += test_avl_height();
    failed += test_avl_elements();
    failed += test_avl_remove();
    printf("libcollection/avl_tree tests finished with %d failures.\n", failed);
    return failed;
}