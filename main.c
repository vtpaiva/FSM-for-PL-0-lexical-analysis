// main.c
#include "lexer.h"
#include <stdio.h>
#include <string.h>

const char* tokenTypeToString(Token token) {
    //printf("\n%s\n", token.lexeme);
    switch (token.type) {
        case TOKEN_IDENTIFIER: return "ident";
        case TOKEN_NUMBER: return "numero";
        case TOKEN_KEYWORD: {
            if (strcmp(token.lexeme, "BEGIN") == 0)
                return "BEGIN";
            if (strcmp(token.lexeme, "END") == 0)
                return "END";
            if (strcmp(token.lexeme, "CONST") == 0)
                return "CONST";
            if (strcmp(token.lexeme, "VAR") == 0)
                return "VAR";
            if (strcmp(token.lexeme, "PROCEDURE") == 0)
                return "PROCEDURE";
            if (strcmp(token.lexeme, "CALL") == 0)
                return "CALL";
            if (strcmp(token.lexeme, "IF") == 0)
                return "IF";
            if (strcmp(token.lexeme, "THEN") == 0)
                return "THEN";
            if (strcmp(token.lexeme, "WHILE") == 0)
                return "WHILE";
            if (strcmp(token.lexeme, "DO") == 0)
                return "DO";
            if (strcmp(token.lexeme, "ELSE") == 0)
                return "ELSE";
            if (strcmp(token.lexeme, "ODD") == 0)
                return "ODD";
        };
        case TOKEN_OPERATOR: { 
            if (token.lexeme[0] == '+')
                return "simbolo_mais";
            if (token.lexeme[0] == ':' && token.lexeme[1] == '=')
                return "simbolo_atribuicao";
            if (strcmp(token.lexeme, "VAR") == 0)
                return "VAR";
            else
                return "simbolo_desconhecido";
        };
        case TOKEN_SYMBOL: {
            if (token.lexeme[0] == ';')
                return "simbolo_ponto_virgula";
            if (token.lexeme[0] == ',')
                return "simbolo_virgula";
            if (token.lexeme[0] == '.')
                return "simbolo_ponto";
            else
                return "simbolo";
        };
        case TOKEN_ERROR: return "<ERRO_LEXICO>";
        case TOKEN_EOF: return "EOF";
        default: return "DESCONHECIDO";
    }
}

int main() {
    FILE* output = fopen("saida.txt", "w");
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    initLexer("entrada.txt");

    Token token;
    do {
        token = getNextToken();
        if (token.type != TOKEN_EOF) {
            fprintf(output, "%s, %s\n", token.lexeme, tokenTypeToString(token));
        }
    } while (token.type != TOKEN_EOF);

    closeLexer();
    fclose(output);
    return 0;
}
