CC=gcc
OBJDIR=./build

buildData: clean
	(echo "insert 58 user58 person58@example.com";\
    echo "insert 56 user56 person56@example.com" ;\
    echo "insert 8 user8 person8@example.com" ;\
    echo "insert 54 user54 person54@example.com" ;\
    echo "insert 77 user77 person77@example.com" ;\
    echo "insert 7 user7 person7@example.com" ;\
    echo "insert 25 user25 person25@example.com" ;\
    echo "insert 71 user71 person71@example.com" ;\
    echo "insert 13 user13 person13@example.com" ;\
    echo "insert 22 user22 person22@example.com" ;\
    echo "insert 53 user53 person53@example.com" ;\
    echo "insert 51 user51 person51@example.com" ;\
    echo "insert 59 user59 person59@example.com" ;\
    echo "insert 32 user32 person32@example.com" ;\
    echo "insert 36 user36 person36@example.com" ;\
    echo "insert 79 user79 person79@example.com" ;\
    echo "insert 10 user10 person10@example.com" ;\
    echo "insert 33 user33 person33@example.com" ;\
    echo "insert 20 user20 person20@example.com" ;\
    echo "insert 4 user4 person4@example.com" ;\
    echo "insert 35 user35 person35@example.com" ;\
    echo "insert 76 user76 person76@example.com" ;\
    echo "insert 49 user49 person49@example.com" ;\
    echo "insert 24 user24 person24@example.com" ;\
    echo "insert 70 user70 person70@example.com" ;\
    echo "insert 48 user48 person48@example.com" ;\
    echo "insert 39 user39 person39@example.com" ;\
    echo "insert 15 user15 person15@example.com" ;\
    echo "insert 47 user47 person47@example.com" ;\
    echo "insert 30 user30 person30@example.com" ;\
    echo "insert 86 user86 person86@example.com" ;\
    echo "insert 31 user31 person31@example.com" ;\
    echo "insert 68 user68 person68@example.com" ;\
    echo "insert 37 user37 person37@example.com" ;\
    echo "insert 66 user66 person66@example.com" ;\
    echo "insert 63 user63 person63@example.com" ;\
    echo "insert 40 user40 person40@example.com" ;\
    echo "insert 78 user78 person78@example.com" ;\
    echo "insert 19 user19 person19@example.com" ;\
    echo "insert 46 user46 person46@example.com" ;\
    echo "insert 14 user14 person14@example.com" ;\
    echo "insert 81 user81 person81@example.com" ;\
    echo "insert 72 user72 person72@example.com" ;\
    echo "insert 6 user6 person6@example.com" ;\
    echo "insert 50 user50 person50@example.com" ;\
    echo "insert 85 user85 person85@example.com" ;\
    echo "insert 67 user67 person67@example.com" ;\
    echo "insert 2 user2 person2@example.com" ;\
    echo "insert 55 user55 person55@example.com" ;\
    echo "insert 69 user69 person69@example.com" ;\
    echo "insert 5 user5 person5@example.com" ;\
    echo "insert 65 user65 person65@example.com" ;\
    echo "insert 52 user52 person52@example.com" ;\
    echo "insert 1 user1 person1@example.com" ;\
    echo "insert 29 user29 person29@example.com" ;\
    echo "insert 9 user9 person9@example.com" ;\
    echo "insert 43 user43 person43@example.com" ;\
    echo "insert 75 user75 person75@example.com" ;\
    echo "insert 21 user21 person21@example.com" ;\
    echo "insert 82 user82 person82@example.com" ;\
    echo "insert 12 user12 person12@example.com" ;\
    echo "insert 18 user18 person18@example.com" ;\
    echo "insert 60 user60 person60@example.com" ;\
    echo "insert 44 user44 person44@example.com" ;\
	echo ".btree" ; echo ".exit") | ./build/simpleDB myDB.db

clean: simpleDB
	rm -rf ./*.o ./*.db

simpleDB: main.o InputBuffer.o PrepareResult.o MetaCommandResult.o Statement.o ExecuteResult.o Row.o Table.o Pager.o Cursor.o Node.o
	$(CC) -o ${OBJDIR}/simpleDB main.o InputBuffer.o PrepareResult.o MetaCommandResult.o Statement.o ExecuteResult.o Row.o Table.o Pager.o Cursor.o Node.o

main.o: src/main.c MetaCommandResult.o Statement.o PrepareResult.o Table.o ExecuteResult.o
	$(CC) -c src/main.c

InputBuffer.o: src/InputBuffer/InputBuffer.c
	$(CC) -c src/InputBuffer/InputBuffer.c

PrepareResult.o: src/Result/PrepareResult.c InputBuffer.o Statement.o
	$(CC) -c src/Result/PrepareResult.c

MetaCommandResult.o: src/Result/MetaCommandResult.c InputBuffer.o Table.o Row.o Node.o Pager.o
	$(CC) -c src/Result/MetaCommandResult.c

Statement.o: src/Result/Statement.c ExecuteResult.o Table.o Row.o Cursor.o Node.o
	$(CC) -c src/Result/Statement.c

ExecuteResult.o: src/Result/ExecuteResult.c 
	$(CC) -c src/Result/ExecuteResult.c

Row.o: src/Result/Row.c
	$(CC) -c src/Result/Row.c

Pager.o: src/Result/Pager.c
	$(CC) -c src/Result/Pager.c

Table.o: src/Result/Table.c Row.o Pager.o Node.o
	$(CC) -c src/Result/Table.c

Cursor.o: src/Result/Cursor.c Row.o Table.o Node.o
	$(CC) -c src/Result/Cursor.c

Node.o: src/Result/Tree/Node.c Row.o Pager.o Cursor.o
	$(CC) -c src/Result/Tree/Node.c

