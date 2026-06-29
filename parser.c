#include <stdio.h>
#include <string.h>
#include "Token.h"

typedef struct {
    char name[26];
    int value;
} Variable;

Variable memory[100];
int variable_count = 0;

void set_variable(char* name, int value) {

    for(int i = 0; i < variable_count; i++) {
        if(strcmp(memory[i].name, name) == 0) {
            memory[i].value = value;
            return;
        }
    }
    strcpy(memory[variable_count].name, name);
    memory[variable_count].value = value;
    variable_count++;
}

int get_variable(char* name) {

    for(int i = 0; i < variable_count; i++) {
        if(strcmp(memory[i].name, name) == 0) {
            return memory[i].value;
        }
    }
    printf("Identifer does NOT exist");
    fflush(stdout);
    return -1;
}

int current_token_index = 0;
Token* global_tokens;
int global_token_count;
int parse_expression();
int parse_term();
int parse_statement();

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

    } else if(current.type == TOKEN_IDENTIFER) {
        current_token_index++;
        int value = get_variable(current.name);
        return value;
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

int parse_statement() {
    Token current = global_tokens[current_token_index];
    if(current.type == TOKEN_LET) {
        current_token_index++;
    
         if(global_tokens[current_token_index].type != TOKEN_IDENTIFER) {
             printf("Syntax error: Expected variable name after 'let'\n");
             return -1;
         }

         char var_name[26];
         strcpy(var_name, global_tokens[current_token_index].name);
         current_token_index++;

         if(global_tokens[current_token_index].type != TOKEN_ASSIGN) {
             printf("Syntax error: Expected '=' after variable name\n");
             return -1;
         }
         current_token_index++;

         int result = parse_expression();
         if(result != -1) {
             set_variable(var_name, result);
             printf("Saved: %s = %d\n", var_name, result);
         }
         return result;
    } else {
        return parse_expression();
    }
    

}

int parser(Token* tokens, int token_count) {
    global_tokens = tokens;
    global_token_count = token_count;
    current_token_index = 0;
    int result = parse_statement();
    if(current_token_index < global_token_count) {
        printf("Syntax Error: Unexpected token '%d' at index %d\n", global_tokens[current_token_index].value, current_token_index);
        fflush(stdout);
        return -1;

    }

    return result;
}
