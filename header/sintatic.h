#ifndef SINTATIC_H
#define SINTATIC_H

#include "../header/lexer.h"
#include <stdarg.h>

typedef enum {
    NEW = 0,
    CONST,
    VAR,
    END
} scope_values;

typedef struct State {
    unsigned symbols_size;
    char **symbols;
    char *error_message;
    struct State *next_state;
} state;

typedef struct Graph {
    int scope, end_count;
    state *start_state, **end_states;
    char *end_lexic;
} graph;

state *create_state(const char *error_message, unsigned symbols_size, ...);
graph *create_graph(scope_values scope, state *start_state, const char *end_lexic, unsigned end_count, ...);
graph *get_CONST_graph();
graph *get_VAR_graph();
int get_scope(char *lexic_input);
bool is_correct_token(state *current_state, char *lexic_input);
void sintatic(FILE *lexic_file, FILE *output_file);

#endif