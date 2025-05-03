// lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct {
    char lexeme[100];
    TokenType type;
} Token;

void initLexer(const char* filename);
Token getNextToken();
void closeLexer();

#endif
