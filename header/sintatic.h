#ifndef SINTATIC_H
#define SINTATIC_H

#include "../header/lexer.h"
#include <stdarg.h>

typedef enum {
    NEW = 0,
    CONST,
    VAR,
    PROCEDURE,
    CALL,
    BEGIN,
    ODD,
    IF_T,
    WHILE_T,
    END
} scope_values;

typedef struct State state;

typedef struct Transition {
    unsigned symbols_size;
    char **symbols;
    state *next_state;
} transition;

typedef struct State {
    char *error_message;
    unsigned transition_count;
    transition **transitions;
} state;

typedef struct Graph {
    int scope, end_count;
    state *start_state, *current_state, **end_states;
    char *end_lexic;
    struct Graph *under;
} graph;

typedef struct Stack {
    graph *top;
    int size;
} graph_stack;

state *create_state(const char *error_message);
transition *create_transition(state *to, unsigned symbols_size, ...);
state *get_next_state(state *current_state, char *lexic_input);
void add_transitions_from(state *from, unsigned transition_count, ...);
graph *create_graph(scope_values scope, state *start_state, const char *end_lexic, unsigned end_count, ...);
graph *get_CONST_graph();
graph *get_VAR_graph();
int get_scope(char *lexic_input);
bool is_correct_token(state *current_state, char *lexic_input);
void sintatic(FILE *lexic_file, FILE *output_file);

#endif
