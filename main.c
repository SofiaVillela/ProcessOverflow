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
    if(argc > 2){
        printf("Erro: numero incorreto de argumentos.")
        exit(1);
    }

    if(argv[1] == NULL){ 
        printf("processflow> ");
        fflush(stdout);
        fgets(task, MAX_ARG, stdin);
    }

    else { 
        FILE *workfile = fopen(argv[1], "r");
    }

}