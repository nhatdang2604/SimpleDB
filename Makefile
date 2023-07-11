CC=gcc
OBJDIR=./build

clean: simpleDB
	rm -rf ./*.o

simpleDB: main.o PrepareResult.o MetaCommandResult.o Statement.o InputBuffer.o
	$(CC) -o ${OBJDIR}/simpleDB main.o PrepareResult.o MetaCommandResult.o Statement.o InputBuffer.o

main.o: src/main.c src/InputBuffer/InputBuffer.h src/Result/Statement.h src/Result/MetaCommandResult.h src/Result/PrepareResult.h
	$(CC) -c src/main.c

PrepareResult.o: src/Result/PrepareResult.c src/InputBuffer/InputBuffer.h src/Result/Statement.h
	$(CC) -c src/Result/PrepareResult.c

MetaCommandResult.o: src/Result/MetaCommandResult.c src/InputBuffer/InputBuffer.h
	$(CC) -c src/Result/MetaCommandResult.c

Statement.o: src/Result/Statement.c 
	$(CC) -c src/Result/Statement.c

InputBuffer.o: src/InputBuffer/InputBuffer.c
	$(CC) -c src/InputBuffer/InputBuffer.c
