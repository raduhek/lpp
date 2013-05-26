#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "sanitizer.h"

char *remove_whitespaces(const char *sentence) {
    char *new_sentence;
    int i = 0;
    int j = 0;
    int length = strlen(sentence);
    char unalter_char; // This is used to skip ad-literam strings (' or ")

    new_sentence = (char*)malloc((length + 1) * sizeof(char));

    while (sentence[i] != '\0') {
        /*
            If we begin a string (with ' or "),
            copy it unaltered
        */
        if (sentence[i] == '\'' || sentence[i] == '"') {
            unalter_char = sentence[i];
            i++;
            while (sentence[i] != '\0') {
                if (sentence[i] == unalter_char) {
                    if (sentence[i-1] != '\\') {
                        i++;
                        break;
                    }
                    i++;
                    continue;
                }
                if (sentence[i] == '\\') {
                    i++;
                    continue;
                }
                /*if (sentence[i] == unalter_char && sentence[i-1] != '\\') {
                    break;
                }*/
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

