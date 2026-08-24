# ProcessFlow
ProcessFlow é um orquestrador de processos. Recebe tarefas que representam programas.

## Arquivos utilizados

- `main.c` — [tem o loop principal e o parsing, chama a funcao principal]
- `funcoes.c` — [separei as funcoes aqui para o main ficar mais limpo, tem o cadastro, procura e execucao de task e a funcao de processar comando que faz tudo funcionar]
- `funcoes.h` — [tem as struct, a declaracao das funcoes que estao em funcoes.c para o main ter acesso]
- `Makefile` — regras de compilação e limpeza


## Como executar
gcc -o processflow main.c funcoes.c

Modo interativo: ./processflow
Modo workflow: ./processflow arquivo.pf


## Como compilar

make


## Como limpar

make clean


##  Exemplos de como testar

task listar /bin/ls
run listar


task ordenar /usr/bin/sort
run sequential listar ordenar
run parallel listar ordenar

task contar /usr/bin/wc -l
run pipe listar ordenar contar


task ordenar /usr/bin/sort
input ordenar nomes.txt
run ordenar


exit

## SO
WSL (Windows Subsystem for Linux) - Ubuntu 26.04
