#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack/stack.h"
#include "parse_tree/utils.h"
#include "parse_tree/parse_tree.h"

#define INPUT_FILE "phrases"

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

void traverse_parse_tree(parse_tree_t *r, int level) {
    int i = 0;
    if (r == NULL) {
        return;
    }

    traverse_parse_tree(r->children[0], level + 1);

    for(i = 0; i < level; ++i) {
        printf("   ");
    }
    printf("%s\n", (char*)r->val);

    traverse_parse_tree(r->children[1], level + 1);
}

char *remove_whitespaces(char *str) {
    int i=0;
    int start_index = -1;
    int end_index = -1;
    int copy_index = 0;
    int spaces_count = 0, right_count = 0;
    char *temp = NULL;
    char *sanitized_string = NULL;

    if (str[i] == '\0') {
        return NULL;
    }

    /*
        Get index of the first non-whitespace character and length of input
    */
    while (str[i] != '\0') {
        if ((str[i] != '\t' && str[i] != ' ') && (start_index == -1)) {
            start_index = i;
        }
        i++;
    }

    i--;

    /*
        Get the index of last non-whitespace character
    */
    while (str[i] == '\t' || str[i] == ' ') {
        i--;
    }
    end_index = i;

    /*
        Allocate memory and end string in C style
    */
    temp = (char*) malloc( (end_index - start_index + 2) * sizeof(char));
    i = end_index - start_index + 1;
    temp[i] = '\0';

    /*
        Remove whitespaces from between the first set of left parantheses
        up until the first character not a left parantheses
    */
    for (i = start_index; i <= end_index; ++i) {
        if (str[i] == '(') {
            temp[copy_index++] = str[i];
        } else if (str[i] == '\t' || str[i] == ' ') {
            continue;
        } else {
            break;
        }
    }

    /*
        Copy the rest of the input string
    */
    for (; i <= end_index; ++i) {
        temp[copy_index++] = str[i];
    }

    /*
        Count the number of whitespace characters and right parantheses
        from the end of the input down until the first valid character

        A valid character is a non-whitespace character not a right paranthesis
    */
    for (i = copy_index - 1; i >= 0; --i) {
        if (temp[i] == ')') {
            right_count ++;
        } else if (temp[i] == '\t' || temp[i] == ' ') {
            spaces_count ++;
        } else {
            break;
        }
    }

    /*
        Put back parantheses after the last character that is not whitespace
    */
    for (i = copy_index -  (right_count + spaces_count); i < copy_index; ++i) {
        if (right_count) {
            temp[i] = ')';
            right_count--;
        } else {
            break;
        }
    }
    temp[i] = '\0';

    if (i != 1) {
        sanitized_string = (char *) malloc(i * sizeof(char));
        sanitized_string[i--] = '\0';

        while (i >= 0) {
            sanitized_string[i] = temp[i];
            i--;
        }
    }

    free(temp);

    return sanitized_string;
}


int main(int argc, char **argv) {
    char line[512];
    char *phrase;
    FILE *input;
    parse_tree_t *root;
    stack_t *phrase_parantheses_stack = new_stack();
    if (argc != 2) {
        input = freopen (INPUT_FILE, "r", stdin);
    } else {
        input = freopen (argv[1], "r", stdin);
    }

    if (input == NULL) {
        perror("Could not open input file");
    }
    
    while (fgets(line, sizeof(line), input)) {
        //phrase = surround_with_parantheses(line);
        sscanf(line, "%[^\r\n]", line);
        /*kewl = check_parantheses(line, phrase_parantheses_stack);
        if (!kewl) {
            printf("not kool\n");
        }*/
        phrase = remove_whitespaces(line);
        if (phrase) {
            phrase = sanitize_phrase(phrase, phrase_parantheses_stack);
            empty_stack(phrase_parantheses_stack);
            phrase = sanitize_phrase(phrase, phrase_parantheses_stack);
            root = construct_parse_tree(phrase, phrase_parantheses_stack);
            traverse_parse_tree(root, 0);
            printf("\n\n");
        }
    }

    return 0;
}

