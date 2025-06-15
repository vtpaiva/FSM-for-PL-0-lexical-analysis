#include "../header/sintatic.h"
#include "../header/main.h"

int main(int argc, char **argv) {
    char *input_filename = ( argc < 2 ) ? "data/entrada.txt" : argv[1];

    FILE *output_f = call_sintatic(input_filename);

    fclose(output_f);
        
    return EXIT_SUCCESS;
}

FILE *call_sintatic(char * input_filename) {
    FILE *lexic_file = call_lexic(input_filename), *output_file = fopen(OUTPUT_FILE, "w");
    // remove(MID_FILE);

    sintatic(lexic_file, output_file);

    fclose(lexic_file);

    return output_file;
}

FILE *call_lexic(char *input_filename) {
    init_lexer(input_filename);
    FILE *mid_file = open_file(MID_FILE, "w+");

    unsigned line = 1;
    token token = {.lexeme = "\0", .type = TOKEN_INITIAL};

    // Itera sobre o arquivo de entrada, lendo cada chr.
    do {
        token = get_next_token( token.lexeme[0] == '{', &line );

        // Escreve no arquivo caso seja esperado.
        if ( token.type != TOKEN_NULL && token.type != TOKEN_EOF ) {
            if( token.lexeme[0] == ',' ) {
                token.lexeme[0] = '!';
                token.type = TOKEN_SYMBOL;
            }

            fprintf(mid_file, "%s, %s, %u\n", token.lexeme, token_type_to_string(token), line);
        }

    } while ( token.type != TOKEN_EOF );

    close_lexer();
    rewind(mid_file);

    return mid_file;
}