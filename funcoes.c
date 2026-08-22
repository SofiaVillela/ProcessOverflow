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
        execvp(path_exec[0], path_exec);
        printf("erro: execvp falhou, programa: %s \n", path_exec[0]);
        exit(1);
    }
    return pid;

}



