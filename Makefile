CC = gcc
all: simpleDB
simpleDB: src/main.c src/InputBuffer/InputBuffer.c
	$(CC) -o build/simpleDB src/main.c src/InputBuffer/InputBuffer.c
