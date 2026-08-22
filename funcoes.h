#ifndef FUNCOES_H
#define FUNCOES_H
#define TAM_TASK 100
#define MAX_ARGS 10

typedef struct Cadastro{
    char *nome;
    char *programa;
    char *argumentos[MAX_ARGS];
    struct Cadastro *next;
} Cadastro;

void cadastra_task(char **token_cadastro, Cadastro **lista_task);
Cadastro *procurar_task(char **token_cadastro, Cadastro *lista_task);

#endif