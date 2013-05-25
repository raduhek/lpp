#include "stdlib.h"
#include "parse_tree.h"

parse_tree_t *new_parse_tree (void *val, parse_tree_t *left, parse_tree_t *right) {
    parse_tree_t *t = (parse_tree_t*) malloc (sizeof(parse_tree_t));

    t->val = val;
    t->children[0] = left;
    t->children[1] = right;

    return t;
}

