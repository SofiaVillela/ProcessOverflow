#ifndef FUNCOES_H
#define FUNCOES_H
#define TAM_TASK 100
#define MAX_ARGS 10

typedef struct Cadastro{
    char *nome;
    char *programa;
    char *argumentos[MAX_ARGS];
    char *file_input;
    char *file_output;
    int append;
    struct Cadastro *next;
} Cadastro;

typedef struct Job{
    int job_id;
    pid_t pid;
    char *nome_task;
    struct Job *next;
} Job;

void cadastra_task(char **token_cadastro, Cadastro **lista_task);
Cadastro *procurar_task(char *nome, Cadastro *lista_task);
pid_t executar_task(Cadastro *task);
void processar_comando(char **token_cadastro, Cadastro **lista_task, Job **lista_job, int *id_prox_job, int *should_run);

#endif