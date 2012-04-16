teste : mmach
	./mmach < teste.ent

mmach : main.o symrec.o bison.o flex.o
	gcc -o mmach main.o symrec.o bison.o flex.o -lfl 

main.o : main.c bison.h
	gcc -c main.c

symrec.o : symrec.c
	gcc -c symrec.c

bison.o : bison.c
	gcc -c bison.c

flex.o  : flex.c
	gcc -c flex.c

bison.h : bison.c

bison.c : mmach.y
	bison -obison.c -d mmach.y

flex.c : mmach.l bison.h
	flex -oflex.c mmach.l

clean : 
	rm *.o bison.c bison.h *~