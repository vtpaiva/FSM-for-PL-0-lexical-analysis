// lexer.c
#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

FILE* source;
int currentChar;

const char* keywords[] = {
    "CONST", "VAR", "PROCEDURE", "CALL",
    "IF", "THEN", "ELSE", "WHILE", "DO",
    "BEGIN", "END", "ODD"
};
const int keywordsCount = 12;

void initLexer(const char* filename) {
    source = fopen(filename, "r");
    if (!source) {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }
    currentChar = fgetc(source);
}

void closeLexer() {
    if (source) fclose(source);
}

void readChar() {
    currentChar = fgetc(source);
}

int isKeyword(const char* str) {
    for (int i = 0; i < keywordsCount; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

Token getNextToken() {
    Token token;
    token.lexeme[0] = '\0';
    token.type = TOKEN_ERROR;

    while (isspace(currentChar)) readChar();

    if (currentChar == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identificador ou Palavra-chave
    if (isalpha(currentChar)) {
        int idx = 0;
        while (isalnum(currentChar)) {
            token.lexeme[idx++] = currentChar;
            readChar();
        }
        token.lexeme[idx] = '\0';

        if (isKeyword(token.lexeme))
            token.type = TOKEN_KEYWORD;
        else
            token.type = TOKEN_IDENTIFIER;

        return token;
    }

    // Número
    if (isdigit(currentChar)) {
        int idx = 0;
        while (isdigit(currentChar)) {
            token.lexeme[idx++] = currentChar;
            readChar();
        }
        token.lexeme[idx] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Operadores e Símbolos Especiais
    if (currentChar == ':' || currentChar == '<' || currentChar == '>') {
        char firstChar = currentChar;
        readChar();
        if (currentChar == '=') {
            token.lexeme[0] = firstChar;
            token.lexeme[1] = currentChar;
            token.lexeme[2] = '\0';
            readChar();
        } else {
            token.lexeme[0] = firstChar;
            token.lexeme[1] = '\0';
        }
        token.type = TOKEN_OPERATOR;
        return token;
    }

    if (strchr("+-*/=;.,()#", currentChar)) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0';
        token.type = (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '=' || currentChar == '#')
                     ? TOKEN_OPERATOR
                     : TOKEN_SYMBOL;
        readChar();
        return token;
    }

    // Caractere desconhecido
    token.lexeme[0] = currentChar;
    token.lexeme[1] = '\0';
    token.type = TOKEN_ERROR;
    readChar();
    return token;
}
