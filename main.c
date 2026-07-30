#include <stdio.h>
#include "Token.h"
#include "lexer.h"
#include "parser.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    srand(time(NULL));
    printf("=== My interpreter v1.0 ===\n");
    printf("Enter the expression or 'exit' for close: \n");

    char big_buffer[4996];

    int brace_level = 0;
    big_buffer[0] = '\0';

    while(1) {
        char input_string[4096];
        int token_count = 0;

        if (brace_level > 0) printf("... ");
        else printf(">>> ");
        fflush(stdout);

        if (fgets(input_string, sizeof(input_string), stdin) == NULL) break;
        if (strncmp(input_string, "exit", 4) == 0) break;

        size_t current_len = strlen(big_buffer);
        size_t remaining = sizeof(big_buffer) - current_len - 1;
        strncat(big_buffer, input_string, remaining);

        for (int i = 0; input_string[i] != '\0'; i++) {
            if (input_string[i] == '{') brace_level++;
            if (input_string[i] == '}') brace_level--;
        }

        if (brace_level <= 0) {
            brace_level = 0;
        
            Token tokens[500];
            lexer(big_buffer, tokens, &token_count);
        
            if (token_count > 0) {
                int res = parser(tokens);
                printf("Result: %d\n", res);
            }

            big_buffer[0] = '\0';
    }
}

 
    return 0;
}

