CC=gcc
CFLAGS=-Wall -g
LEX=flex
YACC=bison -d -y
EXECUTABLE=jucompiler

all: $(EXECUTABLE)

$(EXECUTABLE): lex.yy.c y.tab.c semantic.c codegen.c
	$(CC) $(CFLAGS) lex.yy.c y.tab.c semantic.c codegen.c -o $(EXECUTABLE)

lex.yy.c: jucompiler.l
	$(LEX) jucompiler.l

y.tab.c y.tab.h: jucompiler.y
	$(YACC) jucompiler.y

clean:
	rm -f jucompiler lex.yy.c y.tab.c y.tab.h *.o jucompiler.zip

.PHONY: all clean

zip:
	zip jucompiler.zip jucompiler.l jucompiler.y semantic.c semantic.h