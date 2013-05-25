#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../stack/stack_node.h"

/*
 * Forward declarations
 */
typedef struct parantheses_match_struct {
    int start;
    int end;
} pair_t;

/*
 * Definition of parse_tree_struct
 */
typedef struct parse_tree_struct {
    void *val;
    struct parse_tree_struct *children[2];
} parse_tree_t;

parse_tree_t *new_parse_tree (void *, parse_tree_t *, parse_tree_t *);
parse_tree_t *construct_parse_tree(char *, stack_t *); 
#endif

