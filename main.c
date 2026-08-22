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
                        else executar_task(task_encontrada);
                        
                    }
                }

                else if(strcmp(token_cadastro[1], "parallel") == 0){

                }
                else{
                    Cadastro *task_encontrada = procurar_task(token_cadastro[1], lista_task);
                    if(task_encontrada == NULL){ 
                        printf("task nao encontrada\n"); 
                    }
                    else executar_task(task_encontrada); 

                }
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