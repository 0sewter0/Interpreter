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
    printf("Identifer does NOT exist: '%s'\n", name);
    fflush(stdout);
    return -1;
}

int current_token_index = 0;
Token* global_tokens;
int global_token_count;
int parse_expression();
int parse_term();
int parse_statement();
int parse_comparison();

int parse_factor() {
    Token current = global_tokens[current_token_index];

    if(current_token_index >= global_token_count) return -1;

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
    } else if(current.type == TOKEN_EOF) {
        return 0;
    }
    printf("Syntax error: Expected number or '('\n");
    fflush(stdout);
    return -1;
}

int parse_term() {
    int result = parse_factor();
    if(result == -1) return -1;

    while(current_token_index < global_token_count && (global_tokens[current_token_index].type == TOKEN_STAR || global_tokens[current_token_index].type == TOKEN_SLASH) && global_tokens[current_token_index].type != TOKEN_EOF) {
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
    while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && (global_tokens[current_token_index].type == TOKEN_PLUS || global_tokens[current_token_index].type == TOKEN_MINUS)) {
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

    if(current.type == TOKEN_IF) {
        current_token_index++;
        int condition = parse_comparison();

        printf("[Parser Debug] Condition evaluated to: %d\n", condition);
        if(global_tokens[current_token_index].type == TOKEN_LBRACE) {
            if(condition == 1) {
            int last_result;
            printf("Result: condition is TRUE\n");
            while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE && global_tokens[current_token_index].type != TOKEN_ELSE) {
                printf("Debug inside if, current token index: %d, Type: %d\n", current_token_index, global_tokens[current_token_index].type);
                last_result = parse_statement();
            }
            if(global_tokens[current_token_index].type == TOKEN_ELSE) {
                current_token_index++;
                while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE) {
                    current_token_index++;
                }
            }
            if(global_tokens[current_token_index].type == TOKEN_RBRACE) {
                current_token_index++;
            }
            return last_result;
        }
    else {
        printf("Result: condition is FALSE\n");
        while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE && global_tokens[current_token_index].type != TOKEN_ELSE) {
            current_token_index++;
        }
        if(global_tokens[current_token_index].type == TOKEN_ELSE) {
            current_token_index++;

            while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE) {
                parse_statement();
            }
        }
        if(global_tokens[current_token_index].type == TOKEN_RBRACE) {
            current_token_index++;
        }
        return 0;
    }
        }

    }

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

         int result = parse_comparison();
         if(result != -1) {
             set_variable(var_name, result);
             printf("Saved: %s = %d\n", var_name, result);
         }
         return result;
    } if(current.type == TOKEN_WRITE) {
        current_token_index++;

        if(global_tokens[current_token_index].type != TOKEN_LPAREN) {
            printf("Syntax error: Expected '(' after 'write'\n");
            return -1;
        }
        current_token_index++;

        if(global_tokens[current_token_index].type == TOKEN_STRING) {
            printf("%s\n", global_tokens[current_token_index].string_value);
            current_token_index++;
        } else {
            int value_to_print = parse_expression();
            printf("%d\n", value_to_print);
        }
       
        if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
            printf("Syntax error: Expected closing parenthesis after opening parenthesis\n");
            return -1;
        }
        current_token_index++;
        return 0;
    } if (current.type == TOKEN_WHILE) {
        current_token_index++;
        if (global_tokens[current_token_index].type == TOKEN_LPAREN) {
            current_token_index++;
        } else {
            printf("Syntax error: Expected '(' after 'while'\n");
            return -1;
          }

        int condition_start_index = current_token_index;

   
        while (1) {
        
            current_token_index = condition_start_index;
            int condition_res = parse_comparison();
            if (global_tokens[current_token_index].type == TOKEN_RPAREN) {
                current_token_index++;
            } else {
                printf("Syntax error: Expected ')'\n");
                return -1;
            }

       
            if (global_tokens[current_token_index].type == TOKEN_LBRACE) {
            current_token_index++;
            }

        
            if (condition_res == 1) {
                while (global_tokens[current_token_index].type != TOKEN_RBRACE) {
                    parse_statement();
                }
            
            } 
            else {
                int brace_count = 1;
                while (brace_count > 0) {
                    if (global_tokens[current_token_index].type == TOKEN_LBRACE) brace_count++;
                    if (global_tokens[current_token_index].type == TOKEN_RBRACE) brace_count--;
                    current_token_index++;
                }
                break; 
            }
        }
    return 0; 
}
    else {
        if(current.type == TOKEN_IDENTIFER && global_tokens[current_token_index+1].type == TOKEN_ASSIGN) {
            char var_name[26];
            strcpy(var_name, current.name);
            current_token_index += 2;
            
            int result = parse_comparison();
            
            if(result != -1) {
                set_variable(var_name, result);
                printf("Updated: %s = %d\n", var_name, result);
            }
            return result;
        } 
        
        return parse_comparison();
    }
}

int parse_comparison() {
    int result = parse_expression();

    while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF && (global_tokens[current_token_index].type == TOKEN_EQ || global_tokens[current_token_index].type == TOKEN_GT || global_tokens[current_token_index].type == TOKEN_LT)) {
        Token op = global_tokens[current_token_index];
        current_token_index++;
        int right = parse_expression();
        if(result == -1 || right == -1) return -1;

        if(op.type == TOKEN_EQ) {
            result = (result == right);
        } else if(op.type == TOKEN_LT) {
            result = (result < right);
        } else if(op.type == TOKEN_GT) {
            result = (result > right);
        }
    }
    return result;
}

int parser(Token* tokens, int token_count) {
    global_tokens = tokens;
    global_token_count = token_count;
    current_token_index = 0;
    int result = 0;
    
    while(current_token_index < global_token_count && global_tokens[current_token_index].type != TOKEN_EOF) {
        result = parse_statement();
        

        if(result == -1) {
            return -1;
            printf("\n[PARSER FATAL]");
            break;
        }
    }

    return result;
}
