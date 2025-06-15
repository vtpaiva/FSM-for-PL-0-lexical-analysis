#ifndef MAIN_H
#define MAIN_H

const unsigned CHAR_SIZE = 256;
const char* OUTPUT_FILE = "saida.txt";
const char* MID_FILE = "mid.txt";

int main(int argc, char **argv);
FILE *call_lexic(char *input_filename);
FILE *call_sintatic(char * input_filename);

#endif