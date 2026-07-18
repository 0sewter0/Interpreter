#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"

#define MAX_VARIABLES 100
#define MAX_CALL_STACK 20

typedef struct {
    char name[26];
    int value;
    char string_value[256];
} Variable;

Variable call_stack[MAX_CALL_STACK][MAX_VARIABLES];
int stack_var_count[MAX_CALL_STACK] = {0};
int current_frame = 0;

#define MAX_FUNCTIONS 50

typedef struct {
    char name_fn[32];
    int token_index;
    char args[10][32];
    int arg_count;
    Token body_tokens[100];
    int body_token_count;
} Function;

Function function_table[MAX_FUNCTIONS];
int function_count = 0;

void set_variable(char* name, int value) {
    for(int i = 0; i < stack_var_count[current_frame]; i++) {
        if(strcmp(call_stack[current_frame][i].name, name) == 0) {
            call_stack[current_frame][i].value = value;
            return;
        }
    }
    if(current_frame > 0) {
        for(int i = 0; i < stack_var_count[0]; i++) {
            if(strcmp(call_stack[0][i].name, name) == 0) {
                call_stack[0][i].value = value;
                return;
            }
        }
    }
    int idx = stack_var_count[current_frame];
    if(idx >= MAX_VARIABLES) {
        printf("Runtime error: too many variables in current frame\n");
        return;
    }
    strcpy(call_stack[current_frame][idx].name, name);
    call_stack[current_frame][idx].value = value;//tututu srz
    stack_var_count[current_frame]++;
}

int get_variable(char* name) {
    for(int i = 0; i < stack_var_count[current_frame]; i++) {
        if(strcmp(call_stack[current_frame][i].name, name) == 0) {
            return call_stack[current_frame][i].value;
        }
    }
    if(current_frame > 0) {
        for(int i = 0; i < stack_var_count[0]; i++) {
            if(strcmp(call_stack[0][i].name, name) == 0) {
                return call_stack[0][i].value;
            }
        }
    }
    printf("Runtime Error: Variable %s does not found\n", name);
    return -1;
}

int current_token_index = 0;
Token* global_tokens;
//int global_token_count;
int parse_expression();
int parse_term();
int parse_statement();
int parse_comparison();
int execute_function_call(char *func_name);

int parse_factor() {
    Token current = global_tokens[current_token_index];

    if(current.type == TOKEN_LPAREN) {
        current_token_index++;
        int result = parse_expression();
        if(result == -1) return -1;

        if(global_tokens[current_token_index].type == TOKEN_RPAREN) {
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
        if(global_tokens[current_token_index+1].type == TOKEN_LPAREN) {
            char func_name[32];
            strcpy(func_name, current.name);

            current_token_index++;

            int result = execute_function_call(func_name);
            return result;
        } else {
            current_token_index++;
            int value = get_variable(current.name);
            return value;
        }
    } else if(current.type == TOKEN_EOF) {
        return 0;
    } else {
        printf("Syntax error: Unexpected object\n");
        fflush(stdout);
        exit(1);
        return -1;
    }
}

int parse_term() {
    int result = parse_factor();
    if(result == -1) return -1;

    while((global_tokens[current_token_index].type == TOKEN_STAR || global_tokens[current_token_index].type == TOKEN_SLASH) && global_tokens[current_token_index].type != TOKEN_EOF) {
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
    while(global_tokens[current_token_index].type != TOKEN_EOF && (global_tokens[current_token_index].type == TOKEN_PLUS || global_tokens[current_token_index].type == TOKEN_MINUS)) {
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
            while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE && global_tokens[current_token_index].type != TOKEN_ELSE) {
                printf("Debug inside if, current token index: %d, Type: %d\n", current_token_index, global_tokens[current_token_index].type);
                last_result = parse_statement();
            }
            if(global_tokens[current_token_index].type == TOKEN_ELSE) {
                current_token_index++;
                while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE) {
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
        while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE && global_tokens[current_token_index].type != TOKEN_ELSE) {
            current_token_index++;
        }
        if(global_tokens[current_token_index].type == TOKEN_ELSE) {
            current_token_index++;

            while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RBRACE) {
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

        while(global_tokens[current_token_index].type != TOKEN_RPAREN && global_tokens[current_token_index].type != TOKEN_EOF) {
            if(global_tokens[current_token_index].type == TOKEN_STRING) {
                printf("%s ", global_tokens[current_token_index].string_value);
                current_token_index++;
            } else {
                int value_to_print = parse_expression();
                printf("%d ", value_to_print);
            }
            if(global_tokens[current_token_index].type == TOKEN_COMMA) {
                current_token_index++;
                if(global_tokens[current_token_index].type == TOKEN_EOF || global_tokens[current_token_index].type == TOKEN_RPAREN) {
                    printf("Syntax error: Trailing comma in write() at index %d\n", current_token_index-1);
                    return -1;
                }
            } else if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
                printf("Syntax error: Expected ')' or ',' in write() at index %d\n", current_token_index);
                return -1;
            } 
        }
       
        if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
            printf("Syntax error: Expected closing parenthesis after opening parenthesis\n");
            return -1;
        }
        current_token_index++;

        printf("\n");
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
}   if(current.type == TOKEN_FN) {
        current_token_index++;
        if(global_tokens[current_token_index].type != TOKEN_IDENTIFER) {
            printf("Bro, I was waiting for function name, isn't that obvious?\n");
            return -1;
        }
        Function* f = &function_table[function_count];
        strcpy(f->name_fn, global_tokens[current_token_index].name);
        f->arg_count = 0;
        f->body_token_count = 0;
        current_token_index++;

        if(global_tokens[current_token_index].type == TOKEN_LPAREN) {
            current_token_index++;
            while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RPAREN) {
                if(global_tokens[current_token_index].type == TOKEN_IDENTIFER) {
                    strcpy(f->args[f->arg_count], global_tokens[current_token_index].name);
                    f->arg_count++;
                    current_token_index++;
                }

                if(global_tokens[current_token_index].type == TOKEN_COMMA) {
                    current_token_index++;

                    if(global_tokens[current_token_index].type == TOKEN_RPAREN) {
                        printf("Syntax error: Trailing comma in function declaration at index %d\n", current_token_index-1);
                        return -1;
                    }
                } else if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
                    printf("Syntax error: Expected ')' or ',' in  function declaration at index %d\n", current_token_index);
                    return -1;
                }
            }
        current_token_index++;
        if(global_tokens[current_token_index].type != TOKEN_LBRACE) {
            printf("Syntax error: Expected '{'\n");
            return -1;
        }
            current_token_index++;
        }    

        int brace_stack = 1;
        do {
            if(global_tokens[current_token_index].type == TOKEN_LBRACE) brace_stack++;
            if(global_tokens[current_token_index].type == TOKEN_RBRACE) brace_stack--;

            if(brace_stack > 0) {
                f->body_tokens[f->body_token_count] = global_tokens[current_token_index];
                f->body_token_count++;
            }
            current_token_index++;
        } while(brace_stack > 0 && global_tokens[current_token_index].type != TOKEN_EOF);

        f->body_tokens[f->body_token_count].type = TOKEN_EOF;
        f->body_token_count++;

        function_count++;
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

    while(global_tokens[current_token_index].type != TOKEN_EOF && (global_tokens[current_token_index].type == TOKEN_EQ || global_tokens[current_token_index].type == TOKEN_GT || global_tokens[current_token_index].type == TOKEN_LT)) {
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

int execute_function_call(char* func_name) {
    Function *f = NULL;
    for(int i = 0; i < function_count; i++) {
        if(strcmp(function_table[i].name_fn, func_name) == 0) {
            f = &function_table[i];
            break;
        }
    }
    if(f == NULL) {
        printf("Runtime error: Function %s not found\n", func_name);
        return -1;
    }
    if(global_tokens[current_token_index].type != TOKEN_LPAREN) {
        printf("Syntax error: Expected '(' in function call\n");
        return -1;
    }
    current_token_index++;

    int temp_args[10] = {0};
    int counted_args = 0;

    while(global_tokens[current_token_index].type != TOKEN_EOF && global_tokens[current_token_index].type != TOKEN_RPAREN) {
        temp_args[counted_args++] = parse_expression();

        if(global_tokens[current_token_index].type == TOKEN_COMMA) {
            current_token_index++;
            if(global_tokens[current_token_index].type == TOKEN_RPAREN) {
                printf("Syntax error: Trailing comma in function call at index %d\n", current_token_index-1);
                return -1;
            }
        } else if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
            printf("Syntax error: Expected ')' or ',' in function call at index %d\n", current_token_index);
            return -1;
        }
    }
    if(global_tokens[current_token_index].type != TOKEN_RPAREN) {
        printf("Syntax error: Expected ')' after arguments at index %d\n", current_token_index);
        return -1;
    }
    current_token_index++;
    

    int saved_return_index = current_token_index;
    Token *saved_global_tokens = global_tokens;

    current_frame++;
    stack_var_count[current_frame] = 0;

    int args_to_copy = (counted_args < f->arg_count) ? counted_args : f->arg_count;

    for(int i = 0; i < args_to_copy; i++) {
        strcpy(call_stack[current_frame][i].name, f->args[i]);
        call_stack[current_frame][i].value = temp_args[i];
        stack_var_count[current_frame]++;
    }
    global_tokens = f->body_tokens;
    current_token_index = 0;
    int return_value = 0;

    while(global_tokens[current_token_index].type != TOKEN_RBRACE && global_tokens[current_token_index].type != TOKEN_EOF) {
        if(global_tokens[current_token_index].type == TOKEN_RETURN) {
            current_token_index++;
            return_value = parse_expression();
            break;
        }
        parse_statement();
    }
    stack_var_count[current_frame] = 0;
    current_frame--;
    
    global_tokens = saved_global_tokens;
    current_token_index = saved_return_index;

    return return_value;
}

int parser(Token* tokens) {
    global_tokens = tokens;
    current_token_index = 0;
    int result = 0;
    
    while(global_tokens[current_token_index].type != TOKEN_EOF) {
        result = parse_statement();
        
        if(result == -1) {
            return -1;
            printf("\n[PARSER FATAL]");
            break;
        }
    }

    return result;
}
