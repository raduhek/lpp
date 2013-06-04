#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "sanitizer.h"

int check_parentheses_closing(const char *str) {
    int i = 0;
    int open_count = 0;
    char unalter_char;
    while (str[i] != '\0') {
        if (str[i] == '\'' || str[i] == '"') {
            unalter_char = str[i];
            i++;
            while (str[i] != '\0') {
                if (str[i] == unalter_char && str[i-1] != '\\') {
                    break;
                }
                i++;
            }
        }
        if (str[i] == '(') {
            open_count++;
        } else if (str[i] == ')') {
            open_count--;
            if (open_count < 0) {
                return 0;
            }
        }
        i++;
    }
    if (open_count != 0) {
        return 0;
    } else {
        return 1;
    }
}

char *remove_whitespaces(const char *sentence, int skip_escapes) {
    char *new_sentence;
    int i = 0;
    int j = 0;
    int length = strlen(sentence);
    char unalter_char; // This is used to skip ad-literam strings (' or ")

    new_sentence = (char*)malloc((length + 1) * sizeof(char));

    while (sentence[i] != '\0') {
        /*
            If we begin a string with ' or ",
            copy it unaltered
        */
        if (sentence[i] == '\'' || sentence[i] == '"') {
            unalter_char = sentence[i];
            i++;
            if (skip_escapes == 0) {
                new_sentence[j] = unalter_char;
                j++;
            }
            while (sentence[i] != '\0') {
                if (skip_escapes == 0) {
                    if (sentence[i] == ')') {
                        new_sentence[j++] = ']';
                        i++;
                        continue;
                    }
                    if (sentence[i] == '(') {
                        new_sentence[j++] = '[';
                        i++;
                        continue;
                    }
                }
                if (sentence[i] == unalter_char && sentence[i-1] != '\\') {
                    if (skip_escapes) {
                        i++;
                    }
                    break;
                }
                if (sentence[i] == '\\') {
                    if (skip_escapes == 0) {
                        new_sentence[j] = '\\';
                        j++;
                    }
                    i++;
                    continue;
                }
                new_sentence[j] = sentence[i];
                i++;
                j++;
            }
        }

        if (sentence[i] != ' ') {
            new_sentence[j] = sentence[i];
            j++;
        }

        i++;
    }
    new_sentence[j] = '\0';
    return new_sentence;
}

/*
 *  Returns a string that does not contain empty pairs of parentheses
 */
void remove_empty_parentheses(char *str) {
    int i = 0, j = 0;
    char unalter_char;
    
    while (str[i] != '\0') {
        if (str[i] == '\'' || str[i] == '"') {
            unalter_char = str[i];
            i++;
            while (str[i] != '\0') {
                if (str[i] == unalter_char && str[i-1] != '\\') {
                    break;
                }
                i++;
            }
        }
        if (str[i] == ')') {
            j = i - 1;
            while (j >= 0 && str[j] == ' ') {
                --j;
            }
            if (str[j] == '(') {
                str[i] = ' ';
                str[j] = ' ';
            }
        }
        i++;
    }
}

/*
 * Just surround the entire phrase with '(' and ')'
 */
void surround_with_parentheses(char *str) {
    int i = 0;
    char *temp_str;
    while (str[i] != '\0') {
        ++i;
    }
    temp_str = (char*)realloc(str, (i + 3) * sizeof(char));
    if (!temp_str) {
        exit(13);
    }
    str = temp_str;
    str[i+1] = ')';
    str[i+2] = '\0';
    i--;
    while (0 <= i) {
        str[i + 1] = str[i];
        --i;
    }
    str[0] = '(';
}

