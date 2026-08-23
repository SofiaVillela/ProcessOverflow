#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "funcoes.h"


int main(int argc, char **argv){
    char task[TAM_TASK]; 
    int should_run = 1;
    Cadastro *lista_task = NULL;
    Job *lista_job = NULL;
    int id_prox_job = 1;

    while(should_run){

        if(argc > 2){
            printf("Erro: numero incorreto de argumentos.");
            exit(1);
        }
    
        if(argv[1] == NULL){ 
            printf("processflow> ");
            fflush(stdout);

            if(fgets(task, TAM_TASK, stdin) == NULL) break; 
            task[strlen(task) - 1] = '\0';
        
            char *token_cadastro[20];
            token_cadastro[0] = strtok(task, " ");
            if(token_cadastro[0] == NULL){
                printf("linha vazia");
                continue;
            }
            
            for(int i = 1; (token_cadastro[i] = strtok(NULL, " ")) != NULL; i++){}

            if(strcmp(token_cadastro[0], "task") == 0){
                cadastra_task(token_cadastro, &lista_task);
            }
            


            else if(strcmp(token_cadastro[0], "run") == 0){

                if(strcmp(token_cadastro[1], "sequential") == 0){
                    for(int i = 2; token_cadastro[i]!= NULL; i++){

                        Cadastro *task_encontrada = procurar_task(token_cadastro[i], lista_task);
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
                        Cadastro *task_encontrada = procurar_task(token_cadastro[i], lista_task);
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
                        if(pipe(pipes[i]) == -1 ){
                            printf("erro: nao foi possivel criar pipe\n");
                            exit(1);
                        }
                    }
                    for(int i = 0; i < total_tasks; i++){
                        Cadastro *task_encontrada = procurar_task(token_cadastro[i + 2], lista_task);
                        if(task_encontrada == NULL){
                            printf("erro: task nao encontrada");
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
                            for(int j =0; j < total_tasks - 1; j++){
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
                    }
                    for(int j = 0; j < total_tasks - 1; j++){
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }

                    for(int j = 0; j < total_tasks; j++){
                        wait(NULL);
                    }
                }

                else{
                    Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
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
                should_run = 0;
            }


            else if(strcmp(token_cadastro[0], "workdir") == 0){
                if(chdir(token_cadastro[1]) == -1){
                    printf("erro: diretorio nao encontrado\n");
                }   
            }



            else if(strcmp(token_cadastro[0], "input") == 0){
                Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
                if(task_encontrada == NULL){
                    printf("erro: tarefa nao encontrada.\n");
                }
                else{
                    task_encontrada->file_input = malloc(strlen(token_cadastro[2]) + 1);
                    strcpy(task_encontrada->file_input, token_cadastro[2]);
                } 
            }

            else if(strcmp(token_cadastro[0], "output") == 0){
                Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
                if(task_encontrada == NULL){
                    printf("Erro: tarefa nao encontrada");
                }
                else{
                    task_encontrada->file_output = malloc(strlen(token_cadastro[2]) + 1);
                    strcpy(task_encontrada->file_output, token_cadastro[2]);
                }
            }

            else if(strcmp(token_cadastro[0], "append") == 0){
                Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
                if(task_encontrada == NULL){
                    printf("Erro: tarefa nao encontrada");
                }
                else{
                    task_encontrada->file_output = malloc(strlen(token_cadastro[2]) + 1);
                    strcpy(task_encontrada->file_output, token_cadastro[2]);
                    task_encontrada->append = 1;
                }
            }


            
            else if(strcmp(token_cadastro[0], "start") == 0){
                Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
                if(task_encontrada == NULL){
                    printf("Erro: tarefa nao encontrada");
                }
                else{
                    pid_t novo_pid = executar_task(task_encontrada);
                    Job *novo_job = malloc(sizeof(Job));
                    if(novo_job == NULL){
                        printf("Erro: falha ao alocar job\n");
                        exit(1);
                    }
                        novo_job->job_id = id_prox_job;
                        novo_job->pid = novo_pid;
                        novo_job->nome_task = malloc(strlen(token_cadastro[1]) + 1);
                        strcpy(novo_job->nome_task, token_cadastro[1]);
                        novo_job->next = NULL;

                        if(lista_job == NULL){
                            lista_job = novo_job;
                        }
                        else if(lista_job != NULL){
                            Job *atual = lista_job;
                            while(atual->next != NULL){
                                atual = atual->next;
                            }
                            atual->next = novo_job;
                        }

                        id_prox_job++;
                        printf("[%d] %d\n", novo_job->job_id, novo_job->pid);
                }
            }
            else if(strcmp(token_cadastro[0], "jobs") == 0){
                Job *atual = lista_job;
                while(atual != NULL){
                    printf("[%d] %d\n", atual->job_id, atual->pid);
                    atual = atual->next;
                }
            }
            else if(strcmp(token_cadastro[0], "wait") == 0){
                int id_procurado = atoi(token_cadastro[1]);
    
                Job *atual = lista_job;
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
    
        else { 
            FILE *workfile = fopen(argv[1], "r");
            if(workfile == NULL){
                printf("Erro: nao foi possivel abrir o arquivo. ");
                exit(1);
            }
            
            while(1){
                if(fgets(task, TAM_TASK, workfile) == NULL) {
                    should_run = 0;
                    break; 


                }
                int tam_palavra = strlen(task);
                if(task[tam_palavra - 1] == '\n'){
                    task[strlen(task) - 1] = '\0';
                }
                printf("%s\n", task);
                if(strcmp(task, "exit") == 0 ){
                    should_run = 0;
                    break;
                
                } 
            }
            fclose(workfile);
        } 
    }

}