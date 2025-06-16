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

    while ( currentChar != EOF && !strchr(str, currentChar) ) {
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

bool is_any_alpha(char *str) {
    return strstr("abcdefghijklmnopqrstuvwxyz", str);
}

// Função de retorno do tipo de string.
int is_keyword_or_error(char* str) {
    for ( int i = 0; i < keywordsCount; i++ ) {
        if (!strcmp(str, keywords[i]))
            return TOKEN_KEYWORD;
    }

    if( !is_all_alnum(str) ) 
        return TOKEN_ERROR_INVALID_STRING;

    return TOKEN_IDENTIFIER;
}

// Função para selecionar a resposta no arquivo final.
token get_next_token(bool is_comment, unsigned *line) {
    token token = {.lexeme[0] = currentChar, .type = TOKEN_ERROR_INVALID_CHAR, .line = *line};

    while ( isspace(currentChar) && !strchr("\r\n", currentChar) ) 
        read_char();
        
    if( strchr("\r\t\n", currentChar) ) {
        token.type = TOKEN_NULL;
        (*line)++;

        while( currentChar != EOF && strchr("\r\t\n", currentChar) ) {
            read_char();
        }
        
        return token;
    }

    if( currentChar == ',' ) {
        token.lexeme[0] = '!';
        token.lexeme[1] = '\0';
        token.type = TOKEN_SYMBOL;
        
        read_char();

        return token;
    }

    if( is_comment ) {
        read_token(&token, "}");
        token.type = TOKEN_NULL;

        return token;
    }

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

            if( !strchr("<>+-*/=;:,(){} \n\r", currentChar) ) {
                while ( !strchr("<>+-*/=;:,(){} \n\r", currentChar) ) {
                    token.lexeme[index++] = currentChar;
                    read_char();
                }

                token.lexeme[index] = '\0';
                token.type = (strchr(token.lexeme, '.')) ? TOKEN_ERROR_FLOATING : TOKEN_ERROR_LEFT_ZERO;

                return token;
            } else {
                token.lexeme[index] = '\0';

                token.type = TOKEN_NUMBER;
                return token;
            }
        }

        while ( currentChar != EOF && !strchr("<>+-*/=;:,(){} \r\n", currentChar) ) {
            token.lexeme[index++] = currentChar;
            read_char();
        }

        if ( currentChar == EOF && token.lexeme[index - 1] == '.') {
            index--;
            currentChar = '.';
        }

        token.lexeme[index] = '\0';

        if( is_all_number(token.lexeme) ) {
            token.type = TOKEN_NUMBER;
        } else if ( is_any_alpha(token.lexeme) || !strchr(token.lexeme, '.') ){
            token.type = TOKEN_ERROR_INVALID_STRING;
        } else {
            token.type = TOKEN_ERROR_FLOATING;
        }

        return token;
    }

    // Identificação de símbolos compostos.
    if ( strchr(":<>", currentChar) ) {
        token.lexeme[0] = currentChar;
        
        read_char();

        if ( !strchr("=>", currentChar) ) {
            token.lexeme[1] = '\0';

            token.type = TOKEN_OPERATOR;
            return token;
        }

        token.lexeme[1] = currentChar;
        token.lexeme[2] = '\0';

        read_char();

        token.type = TOKEN_OPERATOR;
        return token;
    }

    // Identificação de operadores e símbolos.
    if ( strchr("+-*/=;.()", currentChar) ) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0'; 
        token.type = strchr("=-*/=+", currentChar)
                     ? TOKEN_OPERATOR
                     : TOKEN_SYMBOL;

        read_char();

        return token;
    }

    if( strchr("{}", currentChar) ) {
        token.lexeme[0] = currentChar;
        token.lexeme[1] = '\0'; 

        token.type = TOKEN_NULL;

        read_char();

        return token;
    }

    token.lexeme[0] = currentChar;
    token.lexeme[1] = '\0';
    token.type = TOKEN_ERROR_INVALID_CHAR;

    read_char();
    return token;
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
            else if (token.lexeme[0] == '!')
                return "simbolo_virgula";
            else if (token.lexeme[0] == '.')
                return "simbolo_ponto";
            else if (token.lexeme[0] == '(')
                return "simbolo_parenteses_abre";
            else if (token.lexeme[0] == ')')
                return "simbolo_parenteses_fecha";
            else
                return "<ERRO_LEXICO>";
        };
        case TOKEN_IDENTIFIER: return "ident";
        case TOKEN_NUMBER: return "numero";
        case TOKEN_KEYWORD: return strdup(token.lexeme);
        case TOKEN_COMMENT: return "conteudo_comentario";
        case TOKEN_ERROR_INVALID_CHAR: return "<ERRO_LEXICO_CHAR>";
        case TOKEN_ERROR_INVALID_STRING: return "<ERRO_LEXICO_STRING>";
        case TOKEN_ERROR_INVALID_NUMBER: return "<ERRO_LEXICO_NUMERO>";
        case TOKEN_ERROR_FLOATING: return "<ERRO_LEXICO_FLOAT>";
        case TOKEN_ERROR_LEFT_ZERO: return "<ERRO_LEXICO_LEFT_ZERO>";
        case TOKEN_EOF: return "EOF";
        default: return "<ERRO_LEXICO>";
    }
}