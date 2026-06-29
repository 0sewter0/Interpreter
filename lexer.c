#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
        i--;
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
       } else if(string[i] == '(') {
        append_token(tokens, token_count, TOKEN_LPAREN, 0);
        printf("found opening parenthesis: (\n");
        fflush(stdout);
       } else if(string[i] == ')') {
        append_token(tokens, token_count, TOKEN_RPAREN, 0);
        printf("found closing parenthesis: )\n");
        fflush(stdout);
       } else if(string[i] == '=') {
        append_token(tokens, token_count, TOKEN_ASSIGN, 0);
        printf("found assignment symbol: =\n");
        fflush(stdout);
       } else if(isalpha(string[i])) {
        char buffer[32] = {0};
        int buf_idx = 0;

        while(i < n && (isalpha(string[i]) || isdigit(string[i]))) {
            if(buf_idx < 31) {
                buffer[buf_idx++] = string[i];
            }
            i++;
        }
        buffer[buf_idx] = '\0';
        i--;
        if(strcmp(buffer, "let") == 0) {
            append_token(tokens, token_count, TOKEN_LET, 0);
            printf("found keyword: let\n");
        } else {
            Token t;
            t.type = TOKEN_IDENTIFER;
            t.value = 0;
            strcpy(t.name, buffer);

            tokens[*token_count] = t;
            (*token_count)++;
            printf("found identifer: %s\n", buffer);
        }
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
            case TOKEN_ASSIGN:
                printf("Type: ASSIGN, Value: =\n");
                break;
            case TOKEN_IDENTIFER:
                printf("Type: IDENTIFER\n");
                break;
            case TOKEN_LPAREN:
                printf("Type: Lparenthesis'('\n");
                break;
            case TOKEN_RPAREN:
                printf("Type: Rparenthesis ')'\n");
            default:
                printf("Type: Unknown\n");
        }
    }
    printf("---------------------\n");
    fflush(stdout);
    

}