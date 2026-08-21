#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAM_TASK 100
#define MAX_ARGS 10

typedef struct Cadastro{
    char *nome;
    char *programa;
    char *argumentos[MAX_ARGS];
    struct Cadastro *next;
}Cadastro;


void cadastra_task(char **token_cadastro, Cadastro **lista_task){

    Cadastro *nova_task = malloc(sizeof(Cadastro));
    if(nova_task == NULL){
        printf("Erro: Falha ao alocar dinamicamente cadastro\n");
        exit(1);
    }

    nova_task->nome = token_cadastro[1];
    nova_task->programa = token_cadastro[2];

    for(int i = 3, j = 0; token_cadastro[i] != NULL; i++, j++){
        nova_task->argumentos[j] = token_cadastro[i];
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


int main(int argc, char **argv){
    char task[TAM_TASK]; 
    int should_run = 1;
    Cadastro *lista_task = NULL;

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

            }

            else if(strcmp(token_cadastro[0], "exit") == 0){
                should_run = 0;
            }

            else if(strcmp(token_cadastro[0], "input") == 0){

            }

            else if(strcmp(token_cadastro[0], "output") == 0){

            }

            else if(strcmp(token_cadastro[0], "append") == 0){
                
            }
            
            else if(strcmp(token_cadastro[0], "start") == 0){

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