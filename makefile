# Makefile para compilar o jucompiler - Meta 2

# Variáveis
CC = gcc
FLEX = flex
YACC = yacc
CFLAGS = -Wall -g

# Ficheiros
TARGET = jucompiler
# LEX_FILE = jucompiler.l
LEX_FILE = jucompiler.l
YACC_FILE = jucompiler.y


# Regra padrão
$(TARGET): y.tab.c lex.yy.c
	$(CC) $(CFLAGS) y.tab.c lex.yy.c -o $(TARGET)
	
y.tab.c: $(YACC_FILE)
	$(YACC) -dv $(YACC_FILE)

lex.yy.c: $(LEX_FILE) y.tab.h
	$(FLEX) $(LEX_FILE)


clean:
	rm -f $(TARGET) lex.yy.c y.tab.c y.tab.h y.output
