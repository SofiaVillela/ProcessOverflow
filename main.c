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
            printf("Erro: numero incorreto de argumentos.\n");
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
                printf("linha vazia\n");
                continue;
            }
            
            for(int i = 1; (token_cadastro[i] = strtok(NULL, " ")) != NULL; i++){}

            processar_comando(token_cadastro, &lista_task, &lista_job, &id_prox_job, &should_run);
            }
                
    
        else { 
            FILE *workfile = fopen(argv[1], "r");
            if(workfile == NULL){
                printf("Erro: nao foi possivel abrir o arquivo. \n");
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

                char *token_cadastro[20];
                token_cadastro[0] = strtok(task, " ");
                if(token_cadastro[0] == NULL){
                    continue;
                }

                for(int i = 1; (token_cadastro[i] = strtok(NULL, " ")) != NULL; i++){}

                processar_comando(token_cadastro, &lista_task, &lista_job, &id_prox_job, &should_run);
                if(should_run == 0) break;
            
            } 
            fclose(workfile);
        }
    }

}