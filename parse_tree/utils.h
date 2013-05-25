#ifndef PT_UTILS_H
#define PT_UTILS_H

#include "../stack/stack.h"

char *string_copy(char *, int, int, char);
char *surround_with_parantheses(char *, int);
char *sanitize_phrase(char *, stack_t *);

#endif

