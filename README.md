# Relatório — Projeto `jucompiler`

Este relatório documenta, de forma concisa, as opções técnicas adotadas no desenvolvimento do compilador da linguagem Juc (subconjunto de Java SE 9), implementado com `flex`, `bison/yacc` e `gcc`.

**Autores**
- Leonardo Duarte — 2023213089
- Rui Ribeiro — 202118947

**Contexto**
- Linguagem alvo: Juc (subconjunto de Java SE 9)
- Ferramentas: `flex` + `bison/yacc` + `gcc`
- Ficheiros principais:
  - `jucompiler.l` (lexer)
  - `jucompiler.y` (parser + AST)
  - `semantic.h` / `semantic.c` (análise semântica)
  - `codegen.h`/ `codegen.h` (geração de código)

## (i) Gramática re-escrita

A gramática do enunciado foi fornecida em EBNF e teve de ser convertida para uma forma adequada à análise sintática bottom-up com Yacc/Bison. A estratégia principal foi substituir opcionais e repetições por não-terminais auxiliares, mantendo o comportamento da linguagem e simplificando a construção da AST.

Exemplos de não-terminais auxiliares usados:
- `ProgramMembers` (lista de membros da classe)
- `FieldDeclTail` e `VarDeclTail` (declarações com múltiplos identificadores)
- `FormalParamsTail` (lista de parâmetros)
- `ExprListOpt` e `ExprList` (argumentos de chamada)
- `StatementList` (sequência de statements)

Um ponto central foi tratar corretamente situações com `if` dentro de `if`, quando só existe um `else`.  
Sem uma regra explícita, o parser pode ficar com duas interpretações possíveis.

Exemplo:
```java
if (a)
    if (b)
        x = 1;
    else
        x = 2;
```

A interpretação correta (e compatível com Java) é:
- o `else` pertence ao `if (b)`, não ao `if (a)`.

Isto equivale a:
```java
if (a) {
    if (b)
        x = 1;
    else
        x = 2;
}
```

Para garantir este comportamento de forma determinística, dividimos os statements em `MatchedStatement` e `UnmatchedStatement`.  
De forma simples:
- `MatchedStatement`: statements em que todos os `if` já têm `else` resolvido;
- `UnmatchedStatement`: statements onde ainda existe um `if` sem `else`.

Com esta separação, a gramática força naturalmente o `else` a ligar ao `if` mais interno disponível, removendo a ambiguidade.

Também definimos precedência e associatividade dos operadores com `%left`, `%right` e símbolos de precedência para unários (`UMINUS`, `UPLUS`). Isso permite analisar corretamente expressões como:
- `a + b * c`
- `!x && y`
- `a << b + 1`

Além disso, ativámos `%locations` para associar linha e coluna a tokens e produções.  
Na prática, cada símbolo reconhecido pelo parser passa a ter:
- `first_line`, `first_column`
- `last_line`, `last_column`

No lexer, o `YY_USER_ACTION` atualiza estes campos automaticamente a cada token.  
Depois, nas ações da gramática, usamos esses valores para guardar posição no nó AST.

Exemplo:
- ao criar `Identifier(n)`, guardamos a posição de `n` com `@token.first_line` e `@token.first_column`;
- ao criar `Add`, guardamos tipicamente a posição do operador `+`.

Com isto, os erros semânticos apontam para o local certo.  
Exemplo:
```java
if (1) return 0;
```
pode gerar:
`Line X, col Y: Incompatible type int in if statement`

Por fim, foram adicionadas produções com `error` em pontos críticos da gramática para permitir recuperação local e continuação da análise, de forma a reportar múltiplos erros no mesmo programa.

---

## (ii) Algoritmos e estruturas de dados da AST e da tabela de símbolos

### AST

A AST foi implementada com uma estrutura simples baseada em ponteiros `child`/`sibling`.

Cada `Node` contém:
- `type`: categoria do nó (ex.: `If`, `Add`, `MethodDecl`)
- `value`: valor lexical quando aplicável (ex.: `Identifier(n)`)
- `sem_type`: tipo semântico inferido (ex.: `Identifier(x) - boolean`)
- `line`, `col`: posição para mensagens de erro
- `child`, `sibling`: ligações da árvore

Esta representação facilita listas de tamanho variável (parâmetros, statements, membros da classe) sem precisar de vetores dinâmicos por nó.

Funções principais:
- criação de nós (`new_node`, helpers para folhas/unários/binários)
- ligação de nós (`add_child`, `append_sibling`)
- libertação de memória (`free_ast`)
- impressão:
  - Meta 2: AST estrutural (`-t`)
  - Meta 3: AST anotada com tipos em expressões (`-s`)

### Tabela de símbolos

A análise semântica usa duas estruturas:
- `ClassEntry`: símbolo da classe (campos + métodos)
- `MethodEntry`: símbolo de método (retorno, assinatura, parâmetros e locais)

A assinatura de método é composta por:
- nome
- tipos dos parâmetros por ordem
- tipo de retorno

Exemplo conceptual:
- `factorial(int) -> int`
- `main(String[]) -> void`

Cada entrada de método também mantém a sua própria tabela local (`Symbol`), onde são guardados:
- parâmetro formal (`param`)
- variável local
- símbolo especial `return` (tipo de retorno esperado)

Isto simplifica duas tarefas importantes:
1. procurar tipos de identificadores dentro do método;
2. validar `return` comparando o tipo da expressão retornada com o tipo esperado.

### Organização por campos e prioridade de pesquisa

Na linguagem Juc usada no projeto, trabalhamos com:
- campo global da classe (campos + métodos),
- campo de cada método (parâmetros + locais).

Quando aparece um identificador numa expressão, a pesquisa segue esta ordem:
1. tabela do método atual;
2. tabela global da classe.

### Estrutura das assinaturas e chamadas

A assinatura de método é formada por:
- nome do método;
- sequência ordenada de tipos dos argumentos.

Assim, `g(int,double)` e `g(double,int)` são métodos diferentes.  
Esta representação permite suportar sobrecarga e fazer resolução de chamadas de forma previsível.

Na resolução de `Call`, a tabela de métodos é usada em duas passagens:
1. procurar assinatura exata;
2. se falhar, procurar assinaturas compatíveis com promoção `int -> double`.

Resultados possíveis:
- 1 candidato: chamada resolvida;
- >1 candidatos: ambiguidade;
- 0 candidatos: símbolo não encontrado.

### Verificações de consistência da tabela

Durante a construção e uso da tabela, validamos:
- duplicação de nomes no mesmo escopo (`already defined`);
- identificador reservado `_`;
- referências a identificadores não declarados;
- compatibilidade de tipos em atribuições, operadores e returns.

Esta etapa transforma a AST sintática numa AST semanticamente anotada, preparada para geração de código.

### Algoritmo de construção das tabelas

A construção percorre a AST em ordem de declaração:
1. inserir `FieldDecl` na tabela global
2. inserir `MethodDecl` na tabela de métodos
3. para cada método, inserir:
   - símbolo especial `return`
   - parâmetros (`ParamDecl`)
   - variáveis locais (`VarDecl`)

Durante inserção, são verificadas regras de erro:
- `Symbol <token> already defined`
- `Symbol _ is reserved`

---

## (iii) Geração de código

A geração de código intermédio (LLVM IR) foi integrada no fluxo sem argumentos do compilador: só é emitido código quando não existem erros lexicais, sintáticos ou semânticos.

Módulos principais:
- `jucompiler.l` / `jucompiler.y`: tokenização e controlo das opções
- `semantic.c` / `semantic.h`: validação semântica e anotações de tipo
- `codegen.c` / `codegen.h`: emissão de LLVM IR

### Estratégia geral

O gerador trabalha sobre a AST já validada semanticamente e segue este algoritmo:
1. emitir declarações externas (`printf`, `atoi`) e constantes de formato
2. emitir campos globais da classe
3. recolher assinaturas dos métodos
4. gerar código método a método

Para cada método:
1. iniciar contexto local (temporários `%tN`, labels `Lx`)
2. alocar memória para parâmetros e variáveis locais (`alloca`)
3. copiar argumentos para variáveis locais (`store`)
4. percorrer statements e emitir instruções LLVM

### Mapeamento de tipos

- `boolean` -> `i1`
- `int` -> `i32`
- `double` -> `double`
- `String[]` -> `i8**`

Quando necessário, é aplicada conversão `int -> double` com `sitofp`.

### Expressões e statements

A geração de expressões é recursiva:
- literais: imediatos LLVM
- identificadores: `load`
- atribuições: `store`
- operadores: instruções aritméticas/lógicas/comparação
- chamadas de método: `call`

Controlo de fluxo:
- `if/else`: branches condicionais para labels de ramo e junção
- `while`: label de condição, label de corpo e label de saída
- `return`: termina o bloco atual

Foi adicionado controlo de blocos terminados para evitar emissão inválida após `ret`.

### Funcionalidades específicas de Juc

- `Integer.parseInt(args[i])` -> chamada a `atoi`
- `args.length` -> derivado de `argc`
- `System.out.print(...)`:
  - `int` com `%d`
  - `double` com `%.16e`
  - `boolean` com seleção entre strings `true`/`false`
  - `STRLIT` com constantes globais e `%s`

### Resultado

A arquitetura final ficou modular:
- lexer (`jucompiler.l`)
- parser + construção de AST (`jucompiler.y`)
- semântica + tabelas + anotações (`semantic.c/.h`)
- geração de LLVM IR (`codegen.c/.h`)

Esta separação trouxe vantagens concretas:
1. **manutenção mais simples**: cada meta evoluiu sem reescrever completamente as anteriores;
2. **depuração mais rápida**: quando um teste falha, é mais fácil isolar se o problema é lexical, sintático, semântico ou de codegen;
3. **reutilização de informação**: a AST com `line/col` e `sem_type` serve tanto para mensagens de erro como para orientar a geração de código;
4. **compatibilidade com o enunciado**: todas as opções pedidas nas metas anteriores (`-l`, `-e1`, `-t`, `-e2`, `-s`, `-e3`) foram preservadas.

Em termos práticos, o projeto já cobre a estrutura essencial de um compilador real:
- gramática determinística,
- representação intermédia em AST,
- validação por tabela de símbolos,
- verificação de tipos,
- emissão de código intermédio.

