#ifndef MAIN_H
#define MAIN_H

const char* output_filename = "saida.txt";
const char* mid_filename = "mid.txt";
const char* error_def = "<ERRO_LEXICO>";

int main(int argc, char **argv);
FILE *call_lexic(char *input_filename);

#endif