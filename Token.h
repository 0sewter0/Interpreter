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
    TOKEN_READ,
    TOKEN_STRING,
    TOKEN_WHILE,
    TOKEN_RBRACE,
    TOKEN_LBRACE,
    TOKEN_FN,
    TOKEN_RETURN,
    TOKEN_COMMA,
    TOKEN_RANDOM,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char name[26];
    char string_value[256];
} Token;

typedef struct {
    Token *data;
    int count;
    int capasity;
} TokenVector;

void init_tokens(TokenVector *vec, int initial_capacity);
void push_token(TokenVector *vec ,Token t);
void free_tokens(TokenVector *vec);

#endif