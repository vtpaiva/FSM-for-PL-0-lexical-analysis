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
    TOKEN_ERROR_INVALID_CHAR,
    TOKEN_ERROR_INVALID_STRING,
    TOKEN_ERROR_INVALID_NUMBER,
    TOKEN_ERROR_LEFT_ZERO,
    TOKEN_ERROR_FLOATING,
    TOKEN_EOF
} token_type;

typedef struct {
    unsigned line;
    char lexeme[128];
    token_type type;
} token;

char *token_type_to_string(token token);
void init_lexer(const char* filename);
token get_next_token(bool is_comment, unsigned *line);
FILE *open_file(const char* filename, const char* mode);
void close_lexer();

#endif
