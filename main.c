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

    int brace_level = 0;
    big_buffer[0] = '\0';

    while(1) {
        char input_string[256];
        int token_count = 0;

        if (brace_level > 0) printf("... ");
        else printf(">>> ");
        fflush(stdout);

        if (fgets(input_string, sizeof(input_string), stdin) == NULL) break;
        if (strncmp(input_string, "exit", 4) == 0) break;

        strcat(big_buffer, input_string);

        for (int i = 0; input_string[i] != '\0'; i++) {
            if (input_string[i] == '{') brace_level++;
            if (input_string[i] == '}') brace_level--;
        }

        if (brace_level <= 0) {
            brace_level = 0;
        
            Token tokens[500];
            lexer(big_buffer, tokens, &token_count);
        
            if (tokens != NULL && token_count > 0) {
                int res = parser(tokens, token_count);
                printf("Result: %d\n", res);
            }
        
        big_buffer[0] = '\0';
    }
}

 
    return 0;
}

