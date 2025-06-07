#ifndef MAIN_H
#define MAIN_H

const unsigned CHAR_SIZE = 256;
const char* OUTPUT_FILE = "saida.txt";
const char* MID_FILE = "mid.txt";
const char* error_def_char = "<ERRO_LEXICO_CHAR>";
const char* error_def_string = "<ERRO_LEXICO_STRING>";
const char* error_def_number = "<ERRO_LEXICO_NUMERO>";
const char* error_def_float = "<ERRO_LEXICO_FLOAT>";
const char* error_def_lzero = "<ERRO_LEXICO_LEFT_ZERO>";

int main(int argc, char **argv);
FILE *call_lexic(char *input_filename);

#endif