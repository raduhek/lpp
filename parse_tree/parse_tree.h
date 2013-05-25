#ifndef PARSE_TREE_H
#define PARSE_TREE_H

typedef struct parantheses_match_struct {
    int start;
    int end;
} pair_t;

typedef struct parse_tree_struct {
    void *val;
    struct parse_tree_struct *children[2];
} parse_tree_t;

parse_tree_t *new_parse_tree (void *, parse_tree_t *, parse_tree_t *);
#endif

