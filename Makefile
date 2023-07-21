CC=gcc
OBJDIR=./build

clean: simpleDB
	rm -rf ./*.o

simpleDB: main.o InputBuffer.o PrepareResult.o MetaCommandResult.o Statement.o ExecuteResult.o Row.o Table.o Pager.o Cursor.o
	$(CC) -o ${OBJDIR}/simpleDB main.o InputBuffer.o PrepareResult.o MetaCommandResult.o Statement.o ExecuteResult.o Row.o Table.o Pager.o Cursor.o

main.o: src/main.c MetaCommandResult.o Statement.o PrepareResult.o Table.o ExecuteResult.o
	$(CC) -c src/main.c

InputBuffer.o: src/InputBuffer/InputBuffer.c
	$(CC) -c src/InputBuffer/InputBuffer.c

PrepareResult.o: src/Result/PrepareResult.c InputBuffer.o Statement.o
	$(CC) -c src/Result/PrepareResult.c

MetaCommandResult.o: src/Result/MetaCommandResult.c InputBuffer.o Table.o
	$(CC) -c src/Result/MetaCommandResult.c

Statement.o: src/Result/Statement.c ExecuteResult.o Table.o Row.o Cursor.o
	$(CC) -c src/Result/Statement.c

ExecuteResult.o: src/Result/ExecuteResult.c
	$(CC) -c src/Result/ExecuteResult.c

Row.o: src/Result/Row.c
	$(CC) -c src/Result/Row.c

Pager.o: src/Result/Pager.c
	$(CC) -c src/Result/Pager.c

Table.o: src/Result/Table.c Row.o
	$(CC) -c src/Result/Table.c

Cursor.o: src/Result/Cursor.c Row.o Table.o
	$(CC) -c src/Result/Cursor.c