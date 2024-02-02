LANG=C

default: main.c
	gcc -o jokari main.c -lraylib -std=c99
	./jokari
