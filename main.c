#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARG 80

typedef struct Cadastro{
    char *nome;
    char *programa;
    char *argumento;
    struct Cadastro *next;
}Cadastro;

int main(int argc, char **argv){
    char task[MAX_ARG]; 
    int should_run = 1;
    while(should_run){

        if(argc > 2){
            printf("Erro: numero incorreto de argumentos.");
            exit(1);
        }
    
        if(argv[1] == NULL){ 
            printf("processflow> ");
            fflush(stdout);
            if(fgets(task, MAX_ARG, stdin) == NULL) break; 
            task[strlen(task) - 1] = '\0';
            if(strcmp(task, "exit") == 0) should_run = 0;
                
        }
    
        else { 
            FILE *workfile = fopen(argv[1], "r");
            if(workfile == NULL){
                printf("Erro: nao foi possivel abrir o arquivo. ");
                exit(1);
            }
            
            while(1){
                if(fgets(task, MAX_ARG, workfile) == NULL) {
                    should_run = 0;
                    break; 

                }
                task[strlen(task) - 1] = '\0';
                printf("%s\n", task);
                if(strcmp(task, "exit") == 0 ){
                    should_run = 0;
                    break;
                
                } 
            }
        }
        printf("teste de rodagem, should run deve estar zerado: %d ", should_run);
    }

}