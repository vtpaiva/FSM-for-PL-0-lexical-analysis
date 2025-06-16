#include "../header/sintatic.h"

state *create_state(const char *error_message) {
    state *s = malloc(sizeof(state));
    if (!s) {
        fprintf(stderr, "Erro ao alocar state\n");
        exit(1);
    }

    s -> error_message = malloc(strlen(error_message) * sizeof(char) + 1);
    strcpy(s -> error_message, error_message);

    s -> transition_count = 0;

    return s;
}

transition *create_transition(state *to, unsigned symbols_size, ...) {
    transition *t = malloc(sizeof(transition));
    if (!t) {
        fprintf(stderr, "Erro ao alocar transição\n");
        exit(1);
    }

    t->symbols_size = symbols_size;
    t->symbols = malloc(symbols_size * sizeof(char *));
    if (!t->symbols) {
        fprintf(stderr, "Erro ao alocar símbolos da transição\n");
        exit(1);
    }

    va_list args;
    va_start(args, symbols_size);
    for (unsigned i = 0; i < symbols_size; i++) {
        const char *sym = va_arg(args, const char *);
        t->symbols[i] = strdup(sym);
        if (!t->symbols[i]) {
            fprintf(stderr, "Erro ao duplicar símbolo\n");
            exit(1);
        }
    }
    va_end(args);

    t->next_state = to;
    return t;
}

void add_transitions_from(state *from, unsigned transition_count, ...) {
    va_list args;
    va_start(args, transition_count);
    
    from->transitions = malloc(transition_count * sizeof(transition *));
    if (!from->transitions) {
        fprintf(stderr, "Erro ao alocar transições\n");
        exit(1);
    }

    for (unsigned i = 0; i < transition_count; i++) {
        transition *t = va_arg(args, transition *);
        from->transitions[i] = t;
    }

    from->transition_count = transition_count;

    va_end(args);
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

graph *get_NEW_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'");
    state *state2 = create_state("esperado token do tipo 'simbolo_atribuicao'");
    state *state3 = create_state("esperado token dos tipos 'ident' ou 'numero'");
    state *state4 = create_state("esperado token do tipo 'operador_unario'");
    state *state5 = create_state("esperado token dos tipos 'ident' ou 'numero'");

    transition *transition1 = create_transition(state2, 1, "ident");
    transition *transition2 = create_transition(state3, 1, "simbolo_atribuicao");
    transition *transition3 = create_transition(state4, 2, "ident", "numero");
    transition *transition4 = create_transition(state5, 4, "simbolo_mais", "simbolo_menos", "simbolo_vezes", "simbolo_sobre");
    transition *transition5 = create_transition(state4, 2, "ident", "numero");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 1, transition2);
    add_transitions_from(state3, 1, transition3);
    add_transitions_from(state4, 1, transition4);
    add_transitions_from(state5, 1, transition5);

    return create_graph(VAR, state1, "simbolo_ponto_virgula", 1, state4);
}

graph *get_CONST_graph() {
    state *state1 = create_state(strdup("esperado token do tipo 'ident'"));
    state *state2 = create_state(strdup("esperado token do tipo 'igual'"));
    state *state3 = create_state(strdup("esperado token do tipo 'numero'"));
    state *state4 = create_state(strdup("esperado token do tipo 'virgula'"));

    transition *transition1 = create_transition(state2, 1, "ident");
    transition *transition2 = create_transition(state3, 1, "simbolo_igual");
    transition *transition3 = create_transition(state4, 1, "numero");
    transition *transition4 = create_transition(state1, 1, "simbolo_virgula");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 1, transition2);
    add_transitions_from(state3, 1, transition3);
    add_transitions_from(state4, 1, transition4);

    return create_graph(CONST, state1, "simbolo_ponto_virgula", 1, state4);
}

graph *get_VAR_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'");
    state *state2 = create_state("esperado token do tipo 'virgula'");

    transition *transition1 = create_transition(state2, 1, "ident");
    transition *transition2 = create_transition(state1, 1, "simbolo_virgula");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 1, transition2);

    return create_graph(VAR, state1, "simbolo_ponto_virgula", 1, state2);
}

graph *get_ODD_graph() {
    state *state1 = create_state("esperado token dos tipos 'ident' ou 'numero'");
    state *state2 = create_state("esperado token do tipo 'operador_unario' ou 'relacional'");
    state *state3 = create_state("esperado token dos tipos 'ident' ou 'numero'");

    transition *transition1 = create_transition(state2, 2, "ident", "numero");
    transition *transition2 = create_transition(state1, 4, "simbolo_mais", "simbolo_menos", "simbolo_vezes", "simbolo_sobre");
    transition *transition3 = create_transition(state3, 6, "simbolo_igual", "simbolo_maior_igual", "simbolo_menor_igual", "simbolo_diferente", "simbolo_maior", "simbolo_menor");
    transition *transition4 = create_transition(state2, 2, "ident", "numero");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 2, transition2, transition3);
    add_transitions_from(state3, 1, transition4);

    return create_graph(VAR, state1, "@", 1, state2);
}

graph *get_CALL_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'");
    state *state2 = create_state("END");

    transition *transition1 = create_transition(state2, 1, "ident");

    add_transitions_from(state1, 1, transition1);

    return create_graph(CALL, state1, "@", 1, state2);
}

graph *get_IF_graph() {
    state *state1 = create_state("esperado 'ident' ou 'numero' para iniciar a condição");
    state *state2 = create_state("esperado um operador relacional");
    state *state3 = create_state("esperado 'ident' ou 'numero' para finalizar a condição");
    state *state4 = create_state("esperado a palavra-chave 'THEN'");
    state *state5 = create_state("@");

    transition *t1 = create_transition(state2, 2, "ident", "numero");
    transition *t2 = create_transition(state3, 6, "simbolo_igual", "simbolo_diferente", "simbolo_menor", "simbolo_menor_igual", "simbolo_maior", "simbolo_maior_igual");
    transition *t3 = create_transition(state4, 2, "ident", "numero");
    transition *t4 = create_transition(state5, 1, "THEN");

    add_transitions_from(state1, 1, t1);
    add_transitions_from(state2, 1, t2);
    add_transitions_from(state3, 1, t3);
    add_transitions_from(state4, 1, t4);

    return create_graph(IF_T, state1, "@", 1, state5);
}

graph *get_WHILE_graph() {
    state *state1 = create_state("esperado 'ident' ou 'numero' para iniciar a condição");
    state *state2 = create_state("esperado um operador relacional");
    state *state3 = create_state("esperado 'ident' ou 'numero' para finalizar a condição");
    state *state4 = create_state("esperado a palavra-chave 'DO'");
    state *state5 = create_state("@");

    transition *t1 = create_transition(state2, 2, "ident", "numero");
    transition *t2 = create_transition(state3, 6, "simbolo_igual", "simbolo_diferente", "simbolo_menor", "simbolo_menor_igual", "simbolo_maior", "simbolo_maior_igual");
    transition *t3 = create_transition(state4, 2, "ident", "numero");
    transition *t4 = create_transition(state5, 1, "DO");

    add_transitions_from(state1, 1, t1);
    add_transitions_from(state2, 1, t2);
    add_transitions_from(state3, 1, t3);
    add_transitions_from(state4, 1, t4);

    return create_graph(WHILE_T, state1, "@", 1, state5);
}

graph *get_BEGIN_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'");
    state *state2 = create_state("esperado token do tipo 'simbolo_atribuicao'");
    state *state3 = create_state("esperado token dos tipos 'ident' ou 'numero'");
    state *state4 = create_state("esperado token dos tipos 'operador_unario' ou 'simbolo_ponto_virgula'");

    transition *transition1 = create_transition(state2, 1, "ident");
    transition *transition2 = create_transition(state3, 1, "simbolo_atribuicao");
    transition *transition3 = create_transition(state1, 1, "simbolo_ponto_virgula");
    transition *transition4 = create_transition(state3, 4, "simbolo_mais", "simbolo_menos", "simbolo_vezes", "simbolo_sobre");
    transition *transition5 = create_transition(state4, 2, "ident", "numero");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 1, transition2);
    add_transitions_from(state3, 1, transition5);
    add_transitions_from(state4, 2, transition3, transition4);

    return create_graph(BEGIN, state1, "@", 2, state1, state4);
}

graph *get_PROCEDURE_graph() {
    state *state1 = create_state("esperado token do tipo 'ident'");
    state *state2 = create_state("esperado token do tipo 'simbolo_ponto_virgula'");
    state *state3 = create_state("@");

    transition *transition1 = create_transition(state2, 1, "ident");
    transition *transition2 = create_transition(state3, 1, "simbolo_ponto_virgula");

    add_transitions_from(state1, 1, transition1);
    add_transitions_from(state2, 1, transition2);

    return create_graph(PROCEDURE, state1, ";", 1, state3);
}

int get_scope(char *lexic_input) {
    if( !strcmp(lexic_input, "CONST") ) return CONST;
    if( !strcmp(lexic_input, "VAR") ) return VAR;
    if( !strcmp(lexic_input, "PROCEDURE") ) return PROCEDURE;
    if( !strcmp(lexic_input, "CALL") ) return CALL;
    if( !strcmp(lexic_input, "BEGIN") ) return BEGIN;
    if( !strcmp(lexic_input, "ODD") ) return ODD;
    if( !strcmp(lexic_input, "IF") ) return IF_T;
    if( !strcmp(lexic_input, "WHILE") ) return WHILE_T;
    if( !strcmp(lexic_input, "simbolo_ponto") ) return END;

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
    for( int i = 0; i < current_state -> transition_count; i++) {
        for( int j = 0; j < current_state -> transitions[i] ->symbols_size; j++ ) {
            if( !strcmp(current_state -> transitions[i] -> symbols[j], lexic_input) )
                return true;
        }
    }

    return false;
}

state *get_next_state(state *current_state, char *lexic_input) {
    for( int i = 0; i < current_state -> transition_count; i++) {
        for( int j = 0; j < current_state -> transitions[i] ->symbols_size; j++ ) {
            if( !strcmp(current_state -> transitions[i] -> symbols[j], lexic_input) )
                return current_state -> transitions[i] -> next_state;
        }
    }

    return NULL;
}

state* copy_state(state* original_state) {
    if (original_state == NULL) {
        return NULL;
    }
    state* new_state = (state*)malloc(sizeof(state));
    if (new_state == NULL) {
        perror("Erro ao alocar memória para new_state");
        exit(EXIT_FAILURE);
    }
    memcpy(new_state, original_state, sizeof(state)); // Copia os membros de state
    return new_state;
}

graph_stack *init_stack() {
    graph_stack *stack = malloc(sizeof(graph_stack));
    stack -> top = NULL;
    stack -> size = 0;

    return stack;
}

void push_stack(graph_stack *stack, graph *new) {
    new -> under = stack -> top;
    stack -> top = new;

    stack -> size++;
}

void pop_stack(graph_stack *stack) {
    graph *temp = stack -> top;
    stack -> top = temp -> under;

    free(temp);

    stack -> size--;
}

void print_stack(graph_stack *stack) {
    for( int i = 1; stack -> top; i++ ) {
        printf("%d - %d\n", i, stack -> top -> scope);

        stack -> top = stack -> top -> under;
    }
}

void sintatic(FILE *lexic_file, FILE *output_file) {
    graph *GRAPHS[] = {get_NEW_graph(), get_CONST_graph(), get_VAR_graph(), 
                       get_PROCEDURE_graph(), get_CALL_graph(), get_BEGIN_graph(), 
                       get_ODD_graph(), get_IF_graph(), get_WHILE_graph()};


    unsigned line, previous_line = 0, current_scope = NEW;
    char token[256], definition[256];
    char previous_definition[256];
    struct Graph *current_graph = NULL;
    state *current_state = NULL;

    bool panic_mode = false;

    while ( fscanf(lexic_file, "%[^,], %[^,], %u\n", token, definition, &line) == 3 && current_scope != END) {
        if( !strcmp(definition, "simbolo_virgula") )
            strcpy(token, ",");

        if( !strcmp(definition, "<ERRO_LEXICO_CHAR>") )
            fprintf(output_file, "Erro léxico na linha %u: caracter inválido (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_STRING>") )
            fprintf(output_file, "Erro léxico na linha %u: palavra inválida (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_NUMERO>") )
            fprintf(output_file, "Erro léxico na linha %u: número inválido (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_LEFT_ZERO>") )
            fprintf(output_file, "Erro léxico na linha %u: zero à esquerda (%s)\n", line, token);
        else if( !strcmp(definition, "<ERRO_LEXICO_FLOAT>") )
            fprintf(output_file, "Erro léxico na linha %u: número flutuante (%s)\n", line, token);
        
        if( panic_mode ) {
            if( !strcmp(current_graph -> end_lexic, definition) ) {
                panic_mode = false;
                current_scope = NEW;
            } else if( get_scope(definition) != NEW ) {
                panic_mode = false;
                current_scope = get_scope(definition);

                if( current_scope != END ) {
                    current_graph = GRAPHS[current_scope];
                    current_state = current_graph -> start_state;

                    if( strcmp(current_graph -> end_lexic, "@") ) {
                        fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", previous_line, current_graph -> end_lexic);
                    }
                }
            }

            previous_line = line;
            strcpy(previous_definition, definition);

            continue;
        }

        if( current_scope == NEW || current_state -> transition_count == 0 ) {
            current_scope = get_scope(definition);

            if( current_scope != END ) {

                current_graph = GRAPHS[current_scope];
                current_state = current_graph -> start_state;

                previous_line = line;
                strcpy(previous_definition, definition);
            }   

            continue;
        }
        
        if( !strcmp(current_graph -> end_lexic, definition) ) {
            if( !is_end_state(current_graph, current_state) ) {
                fprintf(output_file, "Erro sintático na linha %u: %s, recebido '%s' (%s)\n", previous_line, current_state -> error_message, token, definition);
            }

            current_scope = NEW;
            previous_line = line;
            strcpy(previous_definition, definition);

            continue;
        }

        if( get_scope(definition) != NEW ) {
            current_scope = get_scope(definition);


            if( current_scope != END ) {
                if( !is_end_state(current_graph, current_state) ) {
                    fprintf(output_file, "Erro sintático na linha %u: %s, recebido '%s' (%s)\n", previous_line, current_state -> error_message, token, definition);
                }

                if( strcmp(current_graph -> end_lexic, "@") && strcmp(current_graph -> end_lexic, "END") )
                    fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", previous_line, current_graph -> end_lexic);

                
                current_graph = GRAPHS[current_scope];
                current_state = current_graph -> start_state;

                previous_line = line;
                strcpy(previous_definition, definition);
            }

            continue;
        }

        if( !is_correct_token(current_state, definition) ) {
            if( get_scope(definition) == NEW )
                fprintf(output_file, "Erro sintático na linha %u: %s, recebido '%s' (%s)\n", previous_line, current_state -> error_message, token, definition);

            panic_mode = true;

            current_state = current_graph -> end_states[0];

            continue;
        }

        if( current_state )
            current_state = get_next_state(current_state, definition);

        previous_line = line;
        strcpy(previous_definition, definition);
    }

    if( strcmp(definition, "simbolo_ponto") && current_scope == END )
        fprintf(output_file, "Erro sintático na linha %u: há código após o término do bloco principal\n", line);

    if( !is_end_state(current_graph, current_state) && strcmp(current_graph -> end_lexic, previous_definition) && strcmp("END", previous_definition) )
        fprintf(output_file, "Erro sintático na linha %u: %s, recebido '%s' (%s)\n", previous_line, current_state -> error_message, token, definition);

    if( strcmp(current_graph -> end_lexic, previous_definition) && strcmp(current_graph -> end_lexic, "@") & strcmp("END", previous_definition) && current_state -> transition_count > 0)
        fprintf(output_file, "Erro sintático na linha %u: símbolo '%s' faltando\n", previous_line, current_graph -> end_lexic);

    if( strcmp("simbolo_ponto", definition) && current_scope != END ) 
        fprintf(output_file, "Erro sintático na linha %u: símbolo '.' faltando\n", line);

    if( ftell(output_file) == 0 )
        fprintf(output_file, "Sucesso! A compilação não apresentou erros.");
}