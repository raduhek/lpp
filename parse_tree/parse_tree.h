#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../stack/stack_node.h"

/*
 * Definition of parse_tree_struct
 */
typedef struct parse_tree_struct {
    void *val;
    struct parse_tree_struct *left;
    struct parse_tree_struct *right;
    short int is_operator;
} parse_tree_t;

parse_tree_t *new_parse_tree (void *, parse_tree_t *, parse_tree_t *, short int);
parse_tree_t *construct_tree (char *, stack_t *); 
#endif

