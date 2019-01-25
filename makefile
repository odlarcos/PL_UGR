
INCLUDE = .
COMPILE = gcc
.SUFFIXES:

all: clean lex.yy.c y.tab.c y.tab.o main.o analysis

analysis: main.o y.tab.o semantic.o
	$(COMPILE) -o analysis main.o y.tab.o semantic.o

main.o: main.c
	$(COMPILE) -c -o main.o main.c -I$(INCLUDE)

y.tab.o: y.tab.c
	$(COMPILE) -c -o y.tab.o y.tab.c -I$(INCLUDE)

y.tab.c: sintactico.y lex.yy.c
	bison -v -d -o y.tab.c sintactico.y

lex.yy.c: lex.c
	flex -o lex.yy.c lex.c

semantic.o: semantic.c
	$(COMPILE) -c -o semantic.o semantic.c -I$(INCLUDE)

clean:
	-rm -f generated.c
	-rm -f dec_fun.c
	-rm -f *.o
	-rm -f y.tab.c
	-rm -f y.output
	-rm -f y.tab.h
	-rm -f lex.yy.c
	-rm -f aux_file_*
	-rm -f analysis
