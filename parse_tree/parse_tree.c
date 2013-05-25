#include "stdlib.h"
#include "stdio.h"

#include "../stack/stack.h"
#include "parse_tree.h"
#include "utils.h"

parse_tree_t *new_parse_tree (void *val, parse_tree_t *left, parse_tree_t *right) {
    parse_tree_t *t = (parse_tree_t*) malloc (sizeof(parse_tree_t));

    t->val = val;
    t->children[0] = left;
    t->children[1] = right;

    return t;
}

parse_tree_t *construct_parse_tree(char *str, stack_t *brackets) {
    int start;
    int end;
    int idx;
    int ok;
    char *last_token;
    char *operator ;
    int operator_position;
    stack_t *children_stack = new_stack();
    
    parse_tree_t *root, *left, *right;

    stack_t *brackets_batch = new_stack();
    pair_t *pair;
    pair_t *outer_pair;
    pair_t *inner_pair;

    // Get the first pair from the stack since it contains the entire phrase
    outer_pair = (pair_t*) stack_pop(brackets);
    start = outer_pair->start;
    end = outer_pair->end;

    while ((pair = (pair_t*) stack_peak(brackets)) != NULL) {
        while ((inner_pair = (pair_t*) stack_peak(brackets)) && 
                    (inner_pair->start > start) && (inner_pair->end < end)) {

            inner_pair = (pair_t*) stack_pop(brackets);
            stack_push(brackets_batch, (void*) inner_pair);
        }

        while ((inner_pair = (pair_t*) stack_pop(brackets_batch)) != NULL) {
            ok = 0;
            operator_position = inner_pair->end;
            for (idx = inner_pair->start; idx <= inner_pair->end; ++idx) {
                if (str[idx] == '>' || str[idx] == '&' || str[idx] == '|') {
                    operator_position = idx;
                    ok ++;
                    operator = (char*) malloc(2 * sizeof(char));
                    *operator = str[idx];
                    *(operator + 1) = '\0';
                    str[idx] = ' ';
                }
            }

            if (ok > 1) {
                empty_stack(brackets_batch);
                // free_parse_tree(&root);
                printf("Error inserting: %s\nMust have at most one binary operator. found : %d\n", str, ok);
                exit(11);
            }

            start = inner_pair->start;
            end = operator_position;
            do {
                last_token = string_copy(str, start, end, ' ');
                if (last_token != NULL) {
                    root = new_parse_tree((void*)last_token, NULL, NULL);

                    stack_push(children_stack, (void*) root);
                }
                start = end;
                end = inner_pair->end;
            } while (start != inner_pair->end);

            if (operator_position != inner_pair->end) {
                right = (parse_tree_t*) stack_pop(children_stack);
                left = (parse_tree_t*) stack_pop(children_stack);

                if (left == NULL) {
                    printf("Error: left token not found\n");
                    exit(14);
                }

                if (right == NULL) {
                    printf("Error: right token not found\n");
                    exit(14);
                }

                root = new_parse_tree((void*)operator, left, right);
                stack_push (children_stack, (void*) root);
            }
        }

    }

    /*

        Handling outer most parantheses pair

    */

            ok = 0;
            operator_position = outer_pair->end;
            for (idx = outer_pair->start; idx <= outer_pair->end; ++idx) {
                if (str[idx] == '>' || str[idx] == '&' || str[idx] == '|') {
                    operator_position = idx;
                    ok ++;
                    operator = (char*) malloc(2 * sizeof(char));
                    *operator = str[idx];
                    *(operator + 1) = '\0';
                    str[idx] = ' ';
                }
            }

            if (ok > 1) {
                empty_stack(brackets_batch);
                // free_parse_tree(&root);
                printf("Error inserting: %s\nMust have at most one binary operator. found : %d\n", str, ok);
                exit(11);
            }

            start = outer_pair->start;
            end = operator_position;
            do {
                last_token = string_copy(str, start, end, ' ');
                if (last_token != NULL) {
                    root = new_parse_tree((void*)last_token, NULL, NULL);

                    stack_push(children_stack, (void*) root);
                }
                start = end;
                end = outer_pair->end;
            } while (start != outer_pair->end);

            if (operator_position != outer_pair->end) {
                
                right = (parse_tree_t*) stack_pop(children_stack);
                left = (parse_tree_t*) stack_pop(children_stack);

                if (left == NULL) {
                    printf("Error: left token not found\n");
                    exit(14);
                }

                if (right == NULL) {
                    printf("Error: right token not found\n");
                    exit(14);
                }

                root = new_parse_tree((void*)operator, left, right);
                stack_push (children_stack, (void*) root);
            }


    root = (parse_tree_t*) stack_pop (children_stack);

    if (root == NULL) {
        printf("Error: parse tree root vanished :-??\n");
        exit(14);
    }

    return root;
}

