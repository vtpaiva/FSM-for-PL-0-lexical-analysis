#include "../header/sintatic.h"

state *create_state(const char *error_message, unsigned symbols_size, ...) {
    state *s = malloc(sizeof(state));
    if (!s) {
        fprintf(stderr, "Erro ao alocar state\n");
        exit(1);
    }

    s->symbols_size = symbols_size;
    s->symbols = malloc(symbols_size * sizeof(char *));
    if (!s->symbols) {
        fprintf(stderr, "Erro ao alocar vetor de símbolos\n");
        exit(1);
    }

    va_list args;
    va_start(args, symbols_size);

    for (unsigned i = 0; i < symbols_size; i++) {
        const char *sym = va_arg(args, const char *);
        s->symbols[i] = strdup(sym);
        if (!s->symbols[i]) {
            fprintf(stderr, "Erro ao duplicar símbolo\n");
            exit(1);
        }
    }

    va_end(args);

    s -> error_message = malloc(strlen(error_message) * sizeof(char) + 1);
    strcpy(s -> error_message, error_message);

    return s;
}

graph *create_graph(scope_values scope, state *start_state, const char *end_lexic, unsigned end_count, ...) {
    graph *g = malloc(sizeof(graph));
    if (!g) {
        fprintf(stderr, "Erro ao alocar grafo\n");
        exit(1);
    }

    g->scope = scope;
    g->start_state = start_state;
    g->end_lexic = strdup(end_lexic);
    if (!g->end_lexic) {
        fprintf(stderr, "Erro ao alocar end_lexic\n");
        exit(1);
    }

    g->end_states = malloc(end_count * sizeof(state *));
    if (!g->end_states) {
        fprintf(stderr, "Erro ao alocar non_start_states\n");
        exit(1);
    }

    va_list args;
    va_start(args, end_count);
    for (unsigned i = 0; i < end_count; i++) {
        g->end_states[i] = va_arg(args, state *);
    }
    va_end(args);

    g -> end_count = end_count;

    return g;
}

graph *get_CONST_graph() {
    state *state1 = create_state(strdup("esperado token do tipo 'ident'"), 1, "ident");
    state *state2 = create_state(strdup("esperado token do tipo 'igual'"), 1, "simbolo_igual");
    state *state3 = create_state(strdup("esperado token do tipo 'numero'"), 1, "numero");
    state *state4 = create_state(strdup("esperado token do tipo 'virgula'"), 1, "simbolo_virgula");

    state1->next_state = state2;
    state2->next_state = state3;
    state3->next_state = state4;
    state4->next_state = state1;

    return create_graph(CONST, state1, "simbolo_ponto_virgula", 1, state4);
}

graph *get_VAR_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'", 1, "ident");
    state *state2 = create_state("esperado token do tipo 'virgula'", 1, "virgula");

    state1->next_state = state2;
    state2->next_state = state1;

    return create_graph(CONST, state1, "simbolo_ponto_virgula", 1, state2);
}

int get_scope(char *lexic_input) {
    if( !strcmp(lexic_input, "CONST") )
        return CONST;
    if( !strcmp(lexic_input, "VAR") )
        return VAR;
    if( !strcmp(lexic_input, "simbolo_ponto") )
        return END;

    return NEW;
}

bool is_end_state(graph *graph, state *current_state) {
    for( int i = 0; i < graph -> end_count; i++ ) {
        if( graph -> end_states[i] == current_state )
            return true;
    }

    return false;
}

bool is_correct_token(state *current_state, char *lexic_input) {
    for( int index = 0; index < current_state -> symbols_size; index++ ) {
        if( !strcmp(current_state -> symbols[index], lexic_input) )
            return true;
    }

    return false;
}

void print_graph_errors(graph *g) {
    printf("Mensagens de erro do grafo %s:\n", g->scope == CONST ? "CONST" : "VAR");

    state *s = g->start_state;
    state *inicio = s;

    int count = 0;
    do {
        printf("Estado %d: %s\n", count++, s->error_message);
        s = s->next_state;
    } while (s && s != inicio);
}

void sintatic(FILE *lexic_file, FILE *output_file) {
    graph *GRAPHS[] = {NULL, get_CONST_graph(), get_VAR_graph()};

    unsigned line, current_scope = NEW;
    char token[256], definition[256];
    char previous_definition[256];
    struct Graph *current_graph = NULL;
    state *current_state = NULL;

    bool panic_mode = false;

    while ( fscanf(lexic_file, "%[^,], %[^,], %u\n", token, definition, &line) == 3 ) {

        if( !strcmp(definition, "<ERRO_LEXICO_CHAR>") )
            fprintf(output_file, "Erro léxico na linha %u: caracter inválido (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_STRING>") )
            fprintf(output_file, "Erro léxico na linha %u: palavra inválida (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_NUMERO>") )
            fprintf(output_file, "Erro léxico na linha %u: número inválido (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_FLOAT>") )
            fprintf(output_file, "Erro léxico na linha %u: zero à esquerda (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_LEFT_ZERO>") )
            fprintf(output_file, "Erro léxico na linha %u: número flutuante (%s)\n", line, token);
        
        if( panic_mode ) {
            if( !strcmp(current_graph -> end_lexic, definition) ) {
                panic_mode = false;
                current_scope = NEW;
            } else if(get_scope(definition) != NEW) {
                panic_mode = false;
                current_scope = get_scope(definition);

                if( current_scope != END ) 
                    fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", line, current_graph -> end_lexic);
            }

            strcpy(previous_definition, definition);

            continue;
        }

        if( current_scope == NEW ) {
            current_scope = get_scope(definition);

            if( current_scope != END ) {
                current_graph = GRAPHS[current_scope];
                current_state = current_graph -> start_state;

                strcpy(previous_definition, definition);
            }   

            continue;
        }

        
        if( !strcmp(current_graph -> end_lexic, definition) ) {
            if( !is_end_state(current_graph, current_state) ) {
                printf("%s\n", current_state -> error_message);
            }

            current_scope = NEW;
        strcpy(previous_definition, definition);

            continue;
        }

        if( get_scope(definition) != NEW ) {
            if( !is_end_state(current_graph, current_state) ) {
                printf("%s\n", current_state -> error_message);
            }

            current_scope = get_scope(definition);

            if( current_scope != END ) {
                fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", line, current_graph -> end_lexic);

                current_graph = GRAPHS[current_scope];
                current_state = current_graph -> start_state;

                strcpy(previous_definition, definition);
            }

            continue;
        }

        if( !is_correct_token(current_state, definition) ) {
            fprintf(output_file, "Erro sintático na linha %u: %s\n", line, current_state -> error_message);

            panic_mode = true;

            current_state = current_graph -> end_states[0];

            continue;
        }

        current_state = current_state -> next_state;
        strcpy(previous_definition, definition);
    }

    if( !is_end_state(current_graph, current_state) )
        fprintf(output_file, "Erro sintático na linha %u: %s\n", line, current_state -> error_message);

    if( strcmp(current_graph -> end_lexic, previous_definition) )
        fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", line, current_graph -> end_lexic);

    if( strcmp("simbolo_ponto", definition) ) 
        fprintf(output_file, "Erro sintático na linha %u: símbolo '.' faltando\n", line);
}