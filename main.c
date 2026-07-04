#include <stdio.h>
#include "Token.h"
#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    printf("=== Мой интерпретатор v2.0 ===\n");
    printf("Введите выражение или же 'exit' для выхода: \n");

    char big_buffer[2048];
    int inside_if = 0;

    while(1) {
        char input_string[256];
        int token_count = 0;

        

        if(inside_if) printf("... ");
        else printf(">>> ");
        fflush(stdout);

        if(fgets(input_string, sizeof(input_string), stdin) == NULL) break;

        if(strncmp(input_string, "exit", 4) == 0) break;

        if(strncmp(input_string, "if", 2) == 0 && (input_string[2] == ' ' || input_string[2] == '\n')) {
            inside_if = 1;
        }

        strcat(big_buffer, input_string);

        if(inside_if && strstr(input_string, "endif") != NULL) {
            inside_if = 0;
        }
        if(!inside_if) {
            Token tokens[500];
            lexer(big_buffer, tokens, &token_count);
            if(tokens != NULL && token_count > 0) {
                int res = parser(tokens, token_count);

                if(res != -1 && strncmp(big_buffer, "let", 3) != 0 && strncmp(big_buffer, "if", 2) != 0) {
                    printf("Result: %d\n", res);
                }
            }
            big_buffer[0] = '\0';
        } 
    }

 
    return 0;
}

