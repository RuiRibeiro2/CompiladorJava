# CompiladorJava

Este diretório contém o analisador léxico `jucompiler` e os casos de teste usados nas metas da cadeira de Compiladores (Juc — subconjunto de Java).

Conteúdo relevante
- `jucompiler.l` — ficheiro Flex do lexer (implementação principal).
- `jucompiler2.0.l`, `jucompiler_old.l` — versões históricas.
- `makefile` — alvo(s) de compilação para gerar o binário `jucompiler`.
- `meta1/` — casos de teste para a Meta 1 (análise lexical).
- `test2.sh`, `test2_meta1.sh` — scripts de validação/execução dos testes.
- `enunciado/` — enunciado oficial para referência.

Requisitos
- `flex` (ou `lex` compatível)
- `gcc` (ou `clang`) e `libfl` (`-lfl`)
- `bash` e `make` para os scripts de teste

Compilar manualmente
```bash
cd projeto/github/CompiladorJava
flex jucompiler.l
gcc -o jucompiler lex.yy.c -lfl
```

Executar exemplos
```bash
# modo tokens
./jucompiler -l < meta1/Factorial.java

# modo erros (default ou -e1)
./jucompiler < meta1/errors_e1.java
```

Testes automatizados
- Para validar a Meta 1 localmente use `test2_meta1.sh` (recomendado):
```bash
cd projeto/github/CompiladorJava
bash test2_meta1.sh -b ./jucompiler
```

