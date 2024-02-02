LANG=C

default: main.c
	gcc -o jokari main.c -lraylib -lm -std=c11 -D_POSIX_C_SOURCE=199309L
	./jokari
