.PHONY: clean

processflow: main.c funcoes.c
	gcc -o processflow main.c funcoes.c

clean:
	rm -f processflow