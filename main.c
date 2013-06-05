#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack/stack.h"
#include "pair/pair.h"
#include "parse_tree/utils.h"
#include "parse_tree/parse_tree.h"
#include "sanitizer/sanitizer.h"

#define INPUT_FILE "phrases"


void traverse_parse_tree(parse_tree_t *r, int level) {
    int i = 0;
    if (r == NULL) {
        return;
    }

    traverse_parse_tree(r->left, level + 1);

    for(i = 0; i < level; ++i) {
        printf("   ");
    }
    if (r->is_operator) {
        printf("%c\n", (char)r->val);
    } else {
        printf("%s\n", (char*)r->val);
    }

    traverse_parse_tree(r->right, level + 1);
}

int main(int argc, char **argv) {
    char line[512];
    char *phrase;
    FILE *input;
    int start, end;
    int ep_ok, bo_ok;
    parse_tree_t *root;
    pair_t *pair;
    pair_t *last;
    stack_t *phrase_parentheses_stack = new_stack();
    if (argc != 2) {
        input = freopen (INPUT_FILE, "r", stdin);
    } else {
        input = freopen (argv[1], "r", stdin);
    }

    if (input == NULL) {
        perror("Could not open input file");
    }
    
    while (fgets(line, sizeof(line), input)) {
        sscanf(line, "%[^\r\n]", line);
        if (0 == check_parentheses_closing(line)) {
            printf("unkewl\n");
            continue;
        }
        phrase = remove_whitespaces(line, 0);
        remove_empty_parentheses(phrase);
        phrase = remove_whitespaces(phrase, 0);
        surround_with_parentheses(phrase); 
        ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
        if (0  == ep_ok) {
            printf("\nOopsie\n");
            continue;
        }
        remove_duplicate_parentheses(phrase, phrase_parentheses_stack);
        phrase = remove_whitespaces(phrase, 1);

        bo_ok = check_boolean_operators(phrase);
        if (0 == bo_ok) {
            printf("\nOops\n");
            continue;
        }

        ep_ok = extract_parentheses(phrase, phrase_parentheses_stack);
        
        if (0  == ep_ok) {
            printf("\nOopsie\n");
            continue;
        }
        printf("%s\n", phrase);
        root = construct_tree(phrase, phrase_parentheses_stack);
        if (root == NULL) {
            printf("FUUUUUUUCK\n");
        }
        traverse_parse_tree(root, 0);
        printf("\nDONE\n");
    }

    return 0;
}

