#include "../header/lexer.h"
#include <ctype.h>

FILE* source;
int currentChar;

const char* divisor_chars = "<>+-*/=;:.,(){} \n\r";

// Define as palavras reservadas da linguagem;
const char* keywords[] = {
    "CONST", "VAR", "PROCEDURE", "CALL",
    "IF", "THEN", "ELSE", "WHILE", "DO",
    "BEGIN", "END", "ODD"
};
const int keywordsCount = 12;

// Inicializa o arquivo fonte do analisador léxico.
void init_lexer(const char* filename) {
    source = fopen(filename, "r");

    if (!source) {
        perror("Erro ao abrir o arquivo.");
        exit(EXIT_FAILURE);
    }

    currentChar = fgetc(source);
}

// Função para abrir arquivo e abortar em caso de erro.
FILE *open_file(const char* filename, const char* mode) {
    FILE *file = fopen(filename, mode);

    if( !file ) {
        perror("Erro ao abrir o arquivo.");
        exit(EXIT_FAILURE);
    }

    return file;
}

// Função para fechar arquivo de entrada.
void close_lexer() {
    if (source) 
        fclose(source);
}

// Função de leitura de caracter.
void read_char(void) {
    currentChar = fgetc(source);
}

// Função de leitura de uma sequência de caracteres até um delimitador.
void read_token(token *token, const char* str) {
    int index = 0;

    while ( !strchr(str, currentChar) ) {
        token -> lexeme[index++] = currentChar;
        read_char();
    }

    token -> lexeme[index] = '\0';
}

// Função para verificar se todos os elementos de uma string são alfanuméricos.
bool is_all_alnum(char *str) {
    for( char *st = str; *st; st++ ) {
        if( !isalnum(*st) )
            return false;
    }

    return true;
}

// Função para verificar se todos os elementos de uma string são números.
bool is_all_number(char *str) {
    for( char *st = str; *st; st++ ) {
        if( !isdigit(*st) )
            return false;
    }

    return true;
}

// Função de retorno do tipo de string.
int is_keyword_or_error(char* str) {
    for ( int i = 0; i < keywordsCount; i++ ) {
        if (!strcmp(str, keywords[i]))
            return TOKEN_KEYWORD;
    }

    if( !is_all_alnum(str) )
        return TOKEN_ERROR;

    return TOKEN_IDENTIFIER;
}

// Função de conversão de token para saída no arquivo final.
char *token_type_to_string(token token) {
    switch (token.type) {
        case TOKEN_OPERATOR: { 
            if (token.lexeme[0] == '+')
                return "simbolo_mais";
            else if (token.lexeme[0] == '-')
                return "simbolo_menos";
            else if (token.lexeme[0] == '*')
                return "simbolo_vezes";
            else if (token.lexeme[0] == '/')
                return "simbolo_sobre";
            else if (token.lexeme[0] == '=')
                return "simbolo_igual";
            else if (token.lexeme[0] == '>' && token.lexeme[1] == '=')
                return "simbolo_maior_igual";
            else if (token.lexeme[0] == '<' && token.lexeme[1] == '=')
                return "simbolo_menor_igual";
            else if (token.lexeme[0] == '<' && token.lexeme[1] == '>')
                return "simbolo_diferente";
            else if (token.lexeme[0] == ':' && token.lexeme[1] == '=')
                return "simbolo_atribuicao";
            else if (token.lexeme[0] == '>' && token.lexeme[1] == '\0')
                return "simbolo_maior";
            else if (token.lexeme[0] == '<' && token.lexeme[1] == '\0')
                return "simbolo_menor";
            else
                return "<ERRO_LEXICO>";
        };
        case TOKEN_SYMBOL: {
            if (token.lexeme[0] == ';')
                return "simbolo_ponto_virgula";
            else if (token.lexeme[0] == ',')
                return "simbolo_virgula";
            else if (token.lexeme[0] == '.')
                return "simbolo_ponto";
            else if (token.lexeme[0] == '(')
                return "simbolo_parenteses_abre";
            else if (token.lexeme[0] == ')')
                return "simbolo_parenteses_fecha";
            else if (token.lexeme[0] == '{')
                return "simbolo_comentario_abre";
            else if (token.lexeme[0] == '}')
                return "simbolo_comentario_fecha";
            else
                return "<ERRO_LEXICO>";
        };
        case TOKEN_IDENTIFIER: return "ident";
        case TOKEN_NUMBER: return "numero";
        case TOKEN_KEYWORD: return strdup(token.lexeme);
        case TOKEN_COMMENT: return "conteudo_comentario";
        case TOKEN_ERROR: return "<ERRO_LEXICO>";
        case TOKEN_EOF: return "EOF";
        default: return "<ERRO_LEXICO>";
    }
}

// Função para selecionar a resposta no arquivo final.
token get_next_token(bool is_comment) {
    token token = {.lexeme = "\0", .type = TOKEN_ERROR};

    if( is_comment ) {
        read_token(&token, "}");
        token.type = TOKEN_COMMENT;

        return token;
    }

    while ( isspace(currentChar) ) 
        read_char();

    if ( currentChar == EOF ) {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identificação de identificadores.
    if ( isalpha(currentChar) ) {
        read_token(&token, divisor_chars);
        token.type = is_keyword_or_error(token.lexeme);

        return token;
    }

    // Identificação de números. (Não são permitidos números com 0s à esquerda)
    if ( isdigit(currentChar) ) {
        int index = 0;

        if( currentChar == '0' ) {
            index++;
            token.lexeme[0] = currentChar;

            read_char();

            if( !strchr(divisor_chars, currentChar) ) {
                while ( !strchr(divisor_chars, currentChar) ) {
                    token.lexeme[index++] = currentChar;
                    read_char();
                }

                token.lexeme[index] = '\0';
                token.type = TOKEN_ERROR;

                return token;
            } else {
                token.lexeme[index] = '\0';

                token.type = TOKEN_NUMBER;
                return token;
            }
        }

        while ( !strchr(divisor_chars, currentChar) ) {
            token.lexeme[index++] = currentChar;
            read_char();
        }

        token.lexeme[index] = '\0';
        token.type = (is_all_number(token.lexeme)) ? TOKEN_NUMBER : TOKEN_ERROR;

        return token;
    }

    // Identificação de símbolos compostos.
    if ( strchr(":<>=", currentChar) ) {
        token.lexeme[0] = currentChar;
        read_char();

        if (strchr("=>", currentChar)) {
            token.lexeme[1] = currentChar;
            token.lexeme[2] = '\0';
            read_char();
        } 
        
        else
            token.lexeme[1] = '\0';

        token.type = TOKEN_OPERATOR;
        return token;
    }

    // Identificação de operadores e símbolos.
    if ( strchr("+-*/=;.(){}", currentChar) ) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0'; 
        token.type = strchr("=-*/=+", currentChar)
                     ? TOKEN_OPERATOR
                     : TOKEN_SYMBOL;

        read_char();

        return token;
    }

    // Vírgulas não são explícitas na saída.
    if( currentChar == ',' ) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0'; 

        token.type = TOKEN_NULL;

        read_char();

        return token;
    }

    token.lexeme[0] = currentChar;
    token.lexeme[1] = '\0';
    token.type = TOKEN_ERROR;

    read_char();
    return token;
}
