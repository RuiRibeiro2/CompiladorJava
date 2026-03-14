# Makefile para compilar o jucompiler - Meta 1

# Variáveis
CC = gcc
FLEX = flex
CFLAGS = -Wall -g

# Ficheiros
TARGET = jucompiler
LEX_FILE = jucompiler.l

# Regra padrão
$(TARGET): lex.yy.c
	$(CC) $(CFLAGS) lex.yy.c -o $(TARGET)

lex.yy.c: $(LEX_FILE)
	$(FLEX) $(LEX_FILE)

clean:
	rm -f $(TARGET) lex.yy.c
