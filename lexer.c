#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Token.h"

void append_token(Token* tokens, int* count, TokenType type, int value) {
    if(*count >= 150) {
        printf("Error tokens array overflow");
        return;
    }
    tokens[*count].type = type;
    tokens[*count].value = value;

    (*count)++;
}
void append_token_string(Token* tokens, int* count, TokenType type, char* string) {
    
    if(*count >= 100) {
        printf("Error: tokens array overflow");
        return;
    }
    tokens[*count].type = type;
    

    strcpy(tokens[*count].string_value, string);
    

    (*count)++;
    
}

void lexer(char* string, Token* tokens, int* token_count) {
    int n = strlen(string);

    for(int i = 0; i < n; i++) {
        if(string[i] == ' '|| string[i] == '\r' || string[i] == '\t' || string[i] == '\n') {
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
        if(string[i+1] == '=') {
            append_token(tokens, token_count, TOKEN_EQ, 0);
            printf("found '=='\n");
            i++;
        } else {
            append_token(tokens, token_count, TOKEN_ASSIGN, 0);
            printf("found assign symbol\n");
            fflush(stdout);
        }
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
        } else if(strcmp(buffer, "if") == 0) {
            append_token(tokens, token_count, TOKEN_IF, 0);
            printf("Found keyword: if\n");
        } else if(strcmp(buffer, "else") == 0) {
            append_token(tokens, token_count, TOKEN_ELSE, 0);
            printf("found keyword: else\n");
        } 
          else if(strcmp(buffer, "write") == 0) {
            append_token(tokens, token_count, TOKEN_WRITE, 0);
            printf("found keyword: write\n");
        } else if(strcmp(buffer, "while") == 0) {
            append_token(tokens, token_count, TOKEN_WHILE, 0);
            printf("found keyword: while\n");
        } else if(strcmp(buffer, "fn") == 0) {
            append_token(tokens, token_count, TOKEN_FN, 0);
            printf("found keyword: fn\n");
        } else if(strcmp(buffer, "return") == 0) {
            append_token(tokens, token_count, TOKEN_RETURN, 0);
            printf("found keyword: return\n"); 
        } else if(strcmp(buffer, "read") == 0) {
            append_token(tokens, token_count, TOKEN_READ, 0);
            printf("found keyword: read\n");
        } else if(strcmp(buffer, "random") == 0) {
            append_token(tokens, token_count, TOKEN_RANDOM, 0);
            printf("found keyword: random\n");
        }
        else {
            Token t;
            t.type = TOKEN_IDENTIFER;
            t.value = 0;
            strcpy(t.name, buffer);

            tokens[*token_count] = t;
            (*token_count)++;
            printf("found identifer: %s\n", buffer);
        }
        fflush(stdout);
       } else if(string[i] == '>' && string[i+1] != '>' && string[i+1] != '<') {
            append_token(tokens, token_count, TOKEN_GT, 0);
            printf("found GT symbol\n");
       
       } else if(string[i] == '<') {
            append_token(tokens, token_count, TOKEN_LT, 0);
            printf("found LT symbol\n");
       } else if(string[i] == '"') {
        printf("found symbol: quote\n");
        i++;
        char buffer_t[256];
        int buf_idx = 0;

        while(string[i] != '"' && string[i] != '\0') {
            buffer_t[buf_idx++] = string[i];
            i++;
        }
        buffer_t[buf_idx] = '\0';
        if(string[i] == '"') {
            printf("found symbol: quote\n");
            i++;
        } else {
            printf("Lexical error: Missing closing quote\n");
            fflush(stdout);
            return;
        }
        append_token_string(tokens, token_count, TOKEN_STRING, buffer_t);
        i--;
        continue;
       } else if(string[i] == '{') {
        append_token(tokens, token_count, TOKEN_LBRACE, 0);
        printf("found specific symbol: '{'\n");
       } else if(string[i] == '}') {
        append_token(tokens, token_count, TOKEN_RBRACE, 0);
        printf("found specific symbol: '}'\n");
       } else if(string[i] == ',') {
        append_token(tokens, token_count, TOKEN_COMMA, 0);
        printf("found specific symbol: ','\n");
       } else if(string[i] == '[') {
        append_token(tokens, token_count, TOKEN_LBRACKET, 0);
        printf("found specific symbol: '['\n");
       } else if(string[i] == ']') {
        append_token(tokens, token_count, TOKEN_RBRACKET, 0);
        printf("found specific symbol: ']'\n");
       }
       else {
         printf("Token found: Unknown\n");
         fflush(stdout);
         return;
       }
       
    }
    append_token(tokens, token_count, TOKEN_EOF, 0);

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
                break;
            case TOKEN_LET:
                printf("Type: Keyword 'let'\n");
                break;
            case TOKEN_EOF:
                printf("Type: End of file\n");
                break;
            case TOKEN_EQ:
                printf("Type: symbol '=='\n");
                break;
            case TOKEN_IF:
                printf("Type: keyword 'if'\n");
                break;
            case TOKEN_ELSE:
                printf("Type: keyword 'else'\n");
                break;
            case TOKEN_GT:
                printf("Type: symbol '>'\n");
                break;
            case TOKEN_LT:
                printf("Type: symbol '<'\n");
                break;
            case TOKEN_LBRACE:
                printf("Type: specific symbol '{'\n");
                break;
            case TOKEN_WRITE:
                printf("Type: keyword 'write'\n");
                break;
            case TOKEN_STRING:
                printf("Type: string\n");
                break;
            case TOKEN_WHILE:
                printf("Type: keyword 'while'\n");
                break;
            case TOKEN_RBRACE:
                printf("Type: specific symbol '}'\n");
                break;
            case TOKEN_RETURN:
                printf("Type: keyword 'return'\n");
                break;
            case TOKEN_FN:
                printf("Type: keyword 'fn'\n");
                break;
            case TOKEN_READ:
                printf("Type: keyword 'read'\n");
                break;
            case TOKEN_COMMA:
                printf("Type: specific symbol: ','\n");
                break;
            case TOKEN_RANDOM:
                printf("Type: keyword 'random'\n");
                break;
            case TOKEN_LBRACKET:
                printf("Type: specific symbol: '['\n");
                break;
            case TOKEN_RBRACKET:
                printf("Type: specific symbol: ']'\n");
                break;
            default:
                printf("Type: Unknown\n");
        }
    }
    printf("---------------------\n");
    fflush(stdout);
}
