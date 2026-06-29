#include <stdio.h>
#include <string.h>
#include "Token.h"

void append_token(Token* tokens, int* count, TokenType type, int value) {
    if(*count >= 100) {
        printf("Error tokens array overflow");
        return;
    }
    tokens[*count].type = type;
    tokens[*count].value = value;

    (*count)++;
}

void lexer(char* string, Token* tokens, int* token_count) {
    int n = strlen(string);

    for(int i = 0; i < n; i++) {
        if(string[i] == ' '|| string[i] == '\t') {
            continue;
        }

       if(string[i] >= '0' && string[i] <= '9') {
        int current_number = 0;
        while(string[i] >= '0' && string[i] <= '9') {
            current_number = (current_number * 10) + (string[i] - '0');
            i++;
        } 
        append_token(tokens, token_count, TOKEN_NUMBER, current_number);
        printf("Number found: %d\n", current_number);
        fflush(stdout);
        
        
       } else if(string[i] == '+') {
        append_token(tokens, token_count, TOKEN_PLUS, 0);
        printf("found an operation: +\n");
        fflush(stdout);
       } else if(string[i] == '-') {
        append_token(tokens, token_count, TOKEN_MINUS, 0);
        printf("found an operation: -\n");
        fflush(stdout);
       } else if(string[i] == '*') {
        append_token(tokens, token_count, TOKEN_STAR, 0);
        printf("found an operation: *\n");
        fflush(stdout);
       } else if(string[i] == '/') {
        append_token(tokens, token_count, TOKEN_SLASH, 0);
        printf("found an operation: /\n");
        fflush(stdout);
       } else {
        printf("Unknown symbol");
        fflush(stdout);
       }
       
    }
    printf("\n--- Resulting Tokens Array ---\n");
    for(int t = 0; t < *token_count; t++) {
        printf("Token [%d]: ", t);

        switch (tokens[t].type) {
            case TOKEN_NUMBER:
                printf("Type: NUMBER, Value: %d\n", tokens[t].value);
                break;
            case TOKEN_PLUS:
                printf("Type: OPERATION, Value: +\n");
                break;
            case TOKEN_MINUS:
                printf("Type: OPERATION, Value: -\n");
                break;
            case TOKEN_STAR:
                printf("Type: OPERATION, Value: *\n");
                break;
            case TOKEN_SLASH:
                printf("Type: OPERATION, Value: /\n");
                break;
            default:
                printf("Type: Unknown\n");
        }
    }
    printf("---------------------\n");
    fflush(stdout);
    

}