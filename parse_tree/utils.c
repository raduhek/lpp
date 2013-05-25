/*
 *  This file contains utility functions needed to input for parse tree parse
 *
 *  Author ep0
 */

#include <stdlib.h>

#include "utils.h"
#include "parse_tree.h"
#include "../stack/stack.h"

/*
 * Copies a substring of @source with a maximum length of @to - @from
 * and replaces characters in @source with @replacer
 *
 * The returned string does not contain white spaces at beggining and end
 * hence the maximum length
 */
char *string_copy(char *source, int from, int to, char replacer) {
    int start, end, idx;
    char *ret;

    start = from + 1;
    while (source[start] == '\t' || source[start] == ' ') {
        start++;
    }

    end = to - 1;
    while ((source[end] == '\t' || source[end] == ' ') && end >= start) {
        end--;
    }
    
    if (end <= start) {
        source[from] = source[to] = ' ';
        return NULL;
    }

    ret = (char*) malloc((end - start + 2) * sizeof(char));
    ret[end - start + 1] = '\0';
    
    for (idx = end; idx >= start; --idx) {
        ret[idx - start] = source[idx];
        if (replacer) {
            source[idx] = replacer;
        }
    }
    if (replacer) {
        source[from] = source[to] = replacer;
    }

    return ret;
}

/*
 * Returns a string that is surrounded by '(' and ')'
 */
char *surround_with_parantheses(char *str, int len) {
    int i;
    char *ret = (char *) malloc((len + 4) * sizeof(char));
    ret[0] = '(';
    for (i = 0; i <= len; i++) {
        ret[i + 1] = str[i];
    }
    ret[len + 2] = ')';
    ret[len + 3] = '\0';

    return ret;
}

/*
 * Returns a string slightly modified version of the string in the
 * sense that any white space from the beginning (or end) and any 
 * white spaces from between parantheses are removed and
 * only a pair of parantheses is left to surround the phrase
 *
 * e.g. "  (   ( a & ( (b | c) & d)))" -> "(a & ((b | c) & d)"
 *
 * @pairs_stack contains a stack of matched parantheses used 
 * to construct the parsing tree
 */
char *sanitize_phrase(char *str, stack_t *pairs_stack) {
    stack_t *temp_stack = new_stack();
    pair_t *pair;
    pair_t *last_pair;
    int ok = 1;
    int i = 0;
    int start, end;
    int enclose = 1;
    int offset = 0;

    char *return_string;

    while (str[i]) {
        if (str[i] == '(') {
            pair = (pair_t*) malloc(sizeof(pair_t));
            pair->start = i;
            stack_push(temp_stack, (void*)pair);
        }
        if (str[i] == ')') {
            pair = (pair_t*) stack_pop(temp_stack);
            if (pair == NULL) {
                while ((pair = (pair_t*)stack_pop(pairs_stack)) != NULL) {
                    free(pair);
                }
                free(temp_stack);
                return 0;
            }

            pair->end = i;
            stack_push(pairs_stack, (void*)pair);
        }
        i++;
    }

    while ((pair = (pair_t*) stack_pop(temp_stack)) != NULL) {
        free(pair);
        ok = 0;
    }

    if (! ok ) {
        return 0;
    }
    start = 0;
    end = i - 1;

    while ((pair = (pair_t*) stack_peak (pairs_stack)) != NULL) {
        if (pair->start == start && pair->end == end) {
            last_pair = (pair_t*) stack_pop (pairs_stack);
            enclose = 0;
            start ++;
            end --;
        } else {
            break;
        }
    }

    offset = start;

    if (enclose == 0) {
        stack_push (pairs_stack, (void*) last_pair);
        return_string = (char *) malloc((last_pair->end - last_pair->start + 1) * sizeof(char));
        return_string[last_pair->end - last_pair->start] = '\0';
        for (i = last_pair->end; i >= last_pair->start ; --i) {
            return_string[i - last_pair->start ] = str[i];
        }
    } else {
        return_string = surround_with_parantheses(str, end);
        pair = (pair_t*) malloc(sizeof(pair_t));
        pair->start = offset;
        pair->end = end + offset;
        stack_push (pairs_stack, (void*)pair);
    }
    return return_string;
}
