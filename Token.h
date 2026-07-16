#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,//uhh, looks like garbage. 25.06.26
    TOKEN_LET,
    TOKEN_IDENTIFER,
    TOKEN_ASSIGN,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_EQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_WRITE,
    TOKEN_STRING,
    TOKEN_WHILE,
    TOKEN_RBRACE,
    TOKEN_LBRACE,
    TOKEN_FN,
    TOKEN_RETURN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char name[26];
    char string_value[256];
} Token;


#endif