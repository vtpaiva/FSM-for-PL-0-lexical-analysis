#include "../header/lexer.h"

const char* output_filename = "saida.txt";

int main(int argc, char **argv) {
    char *input_filename = ( argc < 2 ) ? "data/entrada.txt" : argv[1];

    init_lexer(input_filename);
    FILE *output_file = open_file(output_filename, "w");

    token token = {.lexeme = "\0", .type = TOKEN_INITIAL};

    // Itera sobre o arquivo de entrada, lendo cada chr.
    do {
        token = get_next_token( token.lexeme[0] == '{' );

        // Escreve no arquivo caso seja esperado.
        if ( token.type != TOKEN_NULL && token.type != TOKEN_EOF )
            fprintf(output_file, "%s, %s\n", token.lexeme, token_type_to_string(token));

    } while ( token.type != TOKEN_EOF );

    close_lexer();
    fclose(output_file);

    return EXIT_SUCCESS;
}
