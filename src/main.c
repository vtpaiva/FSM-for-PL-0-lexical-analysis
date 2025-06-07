#include "../header/lexer.h"
#include "../header/main.h"

int main(int argc, char **argv) {
    char *input_filename = ( argc < 2 ) ? "data/entrada.txt" : argv[1];

    FILE *f = call_lexic(input_filename);

    rewind(f);

    unsigned line;
    char token[256], definition[256];

    while ( fscanf(f, "%[^,], %[^,], %u\n", token, definition, &line ) == 3 ) {
        if( !strcmp(definition, error_def) )
            printf("Erro léxico na linha %u:\n", line);
    }
        
    return EXIT_SUCCESS;
}

FILE *call_lexic(char *input_filename) {
    init_lexer(input_filename);
    FILE *mid_file = open_file(mid_filename, "w+");

    unsigned line = 1;
    token token = {.lexeme = "\0", .type = TOKEN_INITIAL};

    // Itera sobre o arquivo de entrada, lendo cada chr.
    do {
        token = get_next_token( token.lexeme[0] == '{', &line );

        // Escreve no arquivo caso seja esperado.
        if ( token.type != TOKEN_NULL && token.type != TOKEN_EOF )
            fprintf(mid_file, "%s, %s, %u\n", token.lexeme, token_type_to_string(token), line);

    } while ( token.type != TOKEN_EOF );

    close_lexer();
    return mid_file;
}