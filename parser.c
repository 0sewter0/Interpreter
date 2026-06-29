#include <stdio.h>
#include "Token.h"

int current_token_index = 0;
Token* global_tokens;
int global_token_count;
int parse_expression();
int parse_term();

int parse_factor() {
    if(current_token_index >= global_token_count) return -1;

    Token current = global_tokens[current_token_index];

    if(current.type == TOKEN_LPAREN) {
        current_token_index++;
        int result = parse_expression();
        if(result == -1) return -1;

        if(current_token_index < global_token_count && global_tokens[current_token_index].type == TOKEN_RPAREN) {
            current_token_index++;
            return result;
        } else {
            printf("Syntax error: Expected closing parenthesis ')'\n");
            fflush(stdout);
            return -1;
        }

    } else if(current.type == TOKEN_NUMBER) {
        current_token_index++;
        return current.value;

    }
    printf("Syntax error: Expected number or '('");
    fflush(stdout);
    return -1;
}

int parse_term() {
    int result = parse_factor();
    if(result == -1) return -1;

    while(current_token_index < global_token_count && (global_tokens[current_token_index].type == TOKEN_STAR || global_tokens[current_token_index].type == TOKEN_SLASH)) {
        TokenType op = global_tokens[current_token_index].type;
        current_token_index++;

        int right = parse_factor();
        if(right == -1) return -1;

        if(op == TOKEN_STAR) {
            result *= right;
        } else if(op == TOKEN_SLASH) {
            if(right == 0) {
                printf("Runtime Error: Division by zero\n");
                fflush(stdout);
                return -1;
            }
            result /= right;
        }
    } 
    return result;
}

int parse_expression() {
    int result = parse_term();
    if(result == -1) return -1;

    while(current_token_index < global_token_count && (global_tokens[current_token_index].type == TOKEN_PLUS || global_tokens[current_token_index].type == TOKEN_MINUS)) {
        TokenType op = global_tokens[current_token_index].type;
        current_token_index++;

        int right = parse_term();
        if(right == -1) return -1;

        if(op == TOKEN_PLUS) {
            result += right;
        } else if(op == TOKEN_MINUS) {
            result -= right;
        }
    }
    return result;
}

int parser(Token* tokens, int token_count) {
    global_tokens = tokens;
    global_token_count = token_count;
    current_token_index = 0;
    int result = parse_expression();
    if(current_token_index < global_token_count) {
        printf("Syntax Error: Unexpected token '%d' at index %d\n", global_tokens[current_token_index].value, current_token_index);
        fflush(stdout);
        return -1;

    }

    return result;
}