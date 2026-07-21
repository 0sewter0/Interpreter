#include "Token.h"
#include <stdio.h>
#include <stdlib.h>

void init_tokens(TokenVector *vec, int initial_capasity) {
    vec->count = 0;
    vec->capasity = initial_capasity;

    vec->data = (Token *)malloc(sizeof(Token) * vec->capasity);

    if(vec->data == NULL) {
        printf("Fatal error: Memory allocation failed!\n");
        exit(1);
    }
}
void free_tokens(TokenVector *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->count = 0;
    vec->capasity = 0;
}

void push_token(TokenVector *vec, Token t) {
    if(vec->count >= vec->capasity) {
        int new_capasity = vec->capasity * 2;

        Token *new_data = (Token *)realloc(vec->data, sizeof(Token) * new_capasity);

        if(new_data == NULL) {
            printf("Fatal error: Memory reallocation failed!\n");
            exit(1);
        }
        vec->data = new_data;
        vec->capasity = new_capasity;
        printf("[DEBUG] Memory expanded to %d tokens\n", new_capasity);
    }
    vec->data[vec->count] = t;
    vec->count++;
}