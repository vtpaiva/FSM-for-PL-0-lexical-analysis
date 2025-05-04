#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    TOKEN_INITIAL,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_COMMENT,
    TOKEN_NULL,
    TOKEN_ERROR,
    TOKEN_EOF
} token_type;

typedef struct {
    char lexeme[128];
    token_type type;
} token;

char *token_type_to_string(token token);
void init_lexer(const char* filename);
token get_next_token(bool is_comment);
FILE *open_file(const char* filename, const char* mode);
void close_lexer();

#endif
