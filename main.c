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
    
    char input_string[256];

    printf("=== Мой интерпретатор v1.0 ===\n");
    printf("Введите выражение или же 'exit' для выхода: \n");

    while(1) {
        printf("> ");
        fflush(stdout);

        if(fgets(input_string, sizeof(input_string), stdin) == NULL) {
            break;
        }

        input_string[strcspn(input_string, "\n")] = '\0';

        if(strcmp(input_string, "exit") == 0) {
            break;
        }
        if(strlen(input_string) == 0) {
            continue;
        }
        Token tokens[100];
        int token_count = 0;

        lexer(input_string, tokens, &token_count);
        int result = parser(tokens, token_count);

        printf("Result: %d\n", result);

    }

 
    return 0;
}

