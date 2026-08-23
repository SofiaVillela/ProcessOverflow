#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funcoes.h"


void cadastra_task(char **token_cadastro, Cadastro **lista_task){

    Cadastro *nova_task = malloc(sizeof(Cadastro));
    if(nova_task == NULL){
        printf("Erro: Falha ao alocar dinamicamente cadastro\n");
        exit(1);
    }

    nova_task->nome = malloc(strlen(token_cadastro[1]) + 1);
    strcpy(nova_task->nome, token_cadastro[1]);
    
    nova_task->programa = malloc(strlen(token_cadastro[2]) + 1);
    strcpy(nova_task->programa, token_cadastro[2]);

    for(int i = 3, j = 0; token_cadastro[i] != NULL; i++, j++){
        nova_task->argumentos[j] = malloc(strlen(token_cadastro[i]) + 1);
        strcpy(nova_task->argumentos[j], token_cadastro[i]);
    }

    nova_task->next = NULL;
    nova_task->file_input = NULL;
    nova_task->file_output = NULL;
    nova_task->append = 0;


    if(*lista_task == NULL){
        *lista_task = nova_task;
    }

    else if(*lista_task !=NULL){
        Cadastro *atual = *lista_task;
        while(atual->next !=NULL){
            atual = atual->next;
        }
        atual->next = nova_task;
    }
}


Cadastro *procurar_task(char *nome, Cadastro *lista_task){
    if(lista_task == NULL){
        printf("Erro: nenhuma task foi cadastrada");
        return NULL;
    }

    Cadastro *procurar = lista_task;

    while(procurar != NULL){
        if(strcmp(nome,procurar->nome) == 0){
            return procurar;
        }
        procurar = procurar->next;
    }
    return NULL;
}

pid_t executar_task(Cadastro *task){
    char *path_exec[MAX_ARGS + 2];
    path_exec[0] = task->programa;
    int i;
    for(i = 1; task->argumentos[i - 1] != NULL; i++){
        path_exec[i] = task->argumentos[i - 1];
    }
    path_exec[i] = NULL;

    pid_t pid = fork();

    
    if(pid < 0){
        printf("Erro: fork falhou.");
        return -1;
    }
    
    if(pid == 0){

        if(task->file_input != NULL){
            FILE *arquivo = fopen(task->file_input, "r");
    
            if(arquivo == NULL){
                printf("Erro: nao foi possivel abrir arquivo de entrada\n");
                exit(1);
            }
            int direcionador = fileno(arquivo);
            dup2(direcionador, 0);
        }

        if(task->file_output != NULL){
            FILE *arquivo;
            if(task->append == 1){
                arquivo = fopen(task->file_output, "a");
            }
            else{
                arquivo = fopen(task->file_output, "w");
            }

            if(arquivo == NULL){
                printf("erro: nao foi possivel abrir o arquivo de saida\n");
                exit(1);
            }
            int direcionador = fileno(arquivo);
            dup2(direcionador, 1);
        }

        execvp(path_exec[0], path_exec);
        printf("erro: execvp falhou, programa: %s \n", path_exec[0]);
        exit(1);
    }
    return pid;

}

void processar_comando(char **token_cadastro, Cadastro **lista_task, Job **lista_job, int *id_prox_job, int *should_run){

    if(token_cadastro[0] == NULL){
        return;
    }

    if(strcmp(token_cadastro[0], "task") == 0){
        cadastra_task(token_cadastro, lista_task);
    }

    else if(strcmp(token_cadastro[0], "run") == 0){

        if(strcmp(token_cadastro[1], "sequential") == 0){
            for(int i = 2; token_cadastro[i] != NULL; i++){
                Cadastro *task_encontrada = procurar_task(token_cadastro[i], *lista_task);
                if(task_encontrada == NULL){
                    printf("task nao encontrada\n");
                }
                else {
                    executar_task(task_encontrada);
                    wait(NULL);
                }
            }
        }



        else if(strcmp(token_cadastro[1], "parallel") == 0){
            int quant_tasks = 0;
            for(int i = 2; token_cadastro[i] != NULL; i++){
                Cadastro *task_encontrada = procurar_task(token_cadastro[i], *lista_task);
                if(task_encontrada == NULL){
                    printf("task nao encontrada\n");
                }
                else {
                    executar_task(task_encontrada);
                    quant_tasks++;
                }
            }
            for(int i = 0; i < quant_tasks; i++){
                wait(NULL);
            }
        }






        else if(strcmp(token_cadastro[1], "pipe") == 0){
            int total_tasks = 0;
            for(int i = 2; token_cadastro[i] != NULL; i++){
                total_tasks++;
            }
            int pipes[total_tasks - 1][2];
            for(int i = 0; i < total_tasks - 1; i++){
                if(pipe(pipes[i]) == -1){
                    printf("erro: nao foi possivel criar pipe\n");
                    exit(1);
                }
            }
            int tasks_iniciadas = 0;
            for(int i = 0; i < total_tasks; i++){
                Cadastro *task_encontrada = procurar_task(token_cadastro[i + 2], *lista_task);
                if(task_encontrada == NULL){
                    printf("erro: task nao encontrada\n");
                    continue;
                }

                pid_t pid_pipe = fork();
                if(pid_pipe == 0){
                    if(i > 0){
                        dup2(pipes[i-1][0], 0);
                    }
                    if(i < total_tasks - 1){
                        dup2(pipes[i][1], 1);
                    }
                    for(int j = 0; j < total_tasks - 1; j++){
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }
                    char *path_exec[MAX_ARGS + 2];
                    path_exec[0] = task_encontrada->programa;
                    int k;
                    for(k = 1; task_encontrada->argumentos[k - 1] != NULL; k++){
                        path_exec[k] = task_encontrada->argumentos[k - 1];
                    }
                    path_exec[k] = NULL;

                    execvp(path_exec[0], path_exec);
                    printf("erro: execvp falhou no pipe\n");
                    exit(1);
                }
                tasks_iniciadas++;
            }
            for(int j = 0; j < total_tasks - 1; j++){
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            for(int j = 0; j < tasks_iniciadas; j++){
                wait(NULL);
            }
        }

        else{
            Cadastro *task_encontrada = procurar_task(token_cadastro[1], *lista_task);
            if(task_encontrada == NULL){
                printf("task nao encontrada\n");
            }
            else{
                executar_task(task_encontrada);
                wait(NULL);
            }
        }
    }

    else if(strcmp(token_cadastro[0], "exit") == 0){
        *should_run = 0;
    }


    else if(strcmp(token_cadastro[0], "workdir") == 0){
        if(chdir(token_cadastro[1]) == -1){
            printf("erro: diretorio nao encontrado\n");
        }
    }





    else if(strcmp(token_cadastro[0], "input") == 0){
        Cadastro *task_encontrada = procurar_task(token_cadastro[1], *lista_task);
        if(task_encontrada == NULL){
            printf("erro: tarefa nao encontrada.\n");
        }
        else{
            task_encontrada->file_input = malloc(strlen(token_cadastro[2]) + 1);
            strcpy(task_encontrada->file_input, token_cadastro[2]);
        }
    }


    else if(strcmp(token_cadastro[0], "output") == 0){
        Cadastro *task_encontrada = procurar_task(token_cadastro[1], *lista_task);
        if(task_encontrada == NULL){
            printf("Erro: tarefa nao encontrada\n");
        }
        else{
            task_encontrada->file_output = malloc(strlen(token_cadastro[2]) + 1);
            strcpy(task_encontrada->file_output, token_cadastro[2]);
        }
    }


    else if(strcmp(token_cadastro[0], "append") == 0){
        Cadastro *task_encontrada = procurar_task(token_cadastro[1], *lista_task);
        if(task_encontrada == NULL){
            printf("Erro: tarefa nao encontrada\n");
        }
        else{
            task_encontrada->file_output = malloc(strlen(token_cadastro[2]) + 1);
            strcpy(task_encontrada->file_output, token_cadastro[2]);
            task_encontrada->append = 1;
        }
    }





    else if(strcmp(token_cadastro[0], "start") == 0){
        Cadastro *task_encontrada = procurar_task(token_cadastro[1], *lista_task);
        if(task_encontrada == NULL){
            printf("Erro: tarefa nao encontrada\n");
        }
        else{
            pid_t novo_pid = executar_task(task_encontrada);
            Job *novo_job = malloc(sizeof(Job));
            if(novo_job == NULL){
                printf("Erro: falha ao alocar job\n");
                exit(1);
            }
            novo_job->job_id = *id_prox_job;
            novo_job->pid = novo_pid;
            novo_job->nome_task = malloc(strlen(token_cadastro[1]) + 1);
            strcpy(novo_job->nome_task, token_cadastro[1]);
            novo_job->next = NULL;

            if(*lista_job == NULL){
                *lista_job = novo_job;
            }
            else{
                Job *atual = *lista_job;
                while(atual->next != NULL){
                    atual = atual->next;
                }
                atual->next = novo_job;
            }

            (*id_prox_job)++;
            printf("[%d] %d\n", novo_job->job_id, novo_job->pid);
        }
    }


    else if(strcmp(token_cadastro[0], "jobs") == 0){
        Job *atual = *lista_job;
        while(atual != NULL){
            printf("[%d] %d\n", atual->job_id, atual->pid);
            atual = atual->next;
        }
    }



    else if(strcmp(token_cadastro[0], "wait") == 0){
        int id_procurado = atoi(token_cadastro[1]);

        Job *atual = *lista_job;
        while(atual != NULL){
            if(atual->job_id == id_procurado){
                waitpid(atual->pid, NULL, 0);
                break;
            }
            atual = atual->next;
        }
        

        if(atual == NULL){
            printf("Erro: job nao encontrado\n");
        }
    }
}



