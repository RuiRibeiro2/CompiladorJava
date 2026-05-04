# Relatório — Projeto `jucompiler`

Este relatório resume o desenvolvimento do compilador Juc até à Meta 3.

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

A gramática do enunciado (em EBNF) foi reescrita para uma forma compatível com análise bottom-up em Yacc/Bison.

**Principais decisões**
- Conversão de opcionais e repetições para não-terminais auxiliares:
  - `ProgramMembers`, `FieldDeclTail`, `VarDeclTail`, `FormalParamsTail`, `StatementList`, `ExprListOpt`.
- Resolução do `dangling else` com separação:
  - `MatchedStatement` e `UnmatchedStatement`.
- Definição explícita de precedência/associatividade, recorrendo a operadores `%left/right`:
  - atribuição, lógicos, igualdade, relacionais, shifts, aritméticos, unários.
- Criação de `%locations` para rastrear linha/coluna de tokens e nós AST, ao longo do ficheiro ".y".

**Resultado**
- Parser estável para os construtores da linguagem Juc.
- AST produzida sem nós supérfluos relevantes. ??? Falar neste tipo de nós
- Recuperação básica de erro com produções `error` para continuar análise e reportar múltiplos erros.

#####################################

## (ii) AST e tabela de símbolos

A AST foi implementada com modelo `child/sibling`, adequado a árvores com número variável de filhos.

**Estrutura do Node**
- `type`: categoria do nó
- `value`: valor lexical (quando aplicável)(ex:Identifier(n))
- `sem_type`: tipo semântico anotado (ex:Identifier(x) - boolean)
- `line`, `col`: localização para mensagens de erro
- `child`, `sibling`: ligação da árvore

**Operações principais na AST**
- criação de nós (`new_node`, nós unários/binários/folha)
- anexação (`add_child`, `append_sibling`)
- libertação de memória (`free_ast`)
- impressão da AST:
  - anotação simples da Meta 2 (`-t`)
  - com anotação de tipos em expressões na Meta 3 (`-s`)

**Tabela de símbolos**
- Classe (`ClassEntry`):
  - campos globais
  - métodos com assinaturas ??? explicar melhor assinaturas
- Método (`MethodEntry`):
  - símbolo especial `return`
  - parâmetros
  - variáveis locais

**Regras verificadas na construção**
- redefinições: `Symbol <token> already defined`
- reservado `_`: `Symbol _ is reserved`

## (iii) Geração semântica (Meta 3)

A Meta 3 foi implementada em `semantic.c`/`semantic.h`, com duas fases: construção de tabelas e anotação/verificação de tipos.

**Anotação de tipos por expressão**
??? explicar melhor
- Literais:
  - `Natural -> int` (com validação de bounds)
  - `Decimal -> double`
  - `BoolLit -> boolean`
- Identificadores:
  - lookup em escopo de método, depois classe
- Operadores:
  - aritméticos (`+ - * / %`)
  - lógicos (`&& || ^`)
  - relacionais/equivalência (`< <= > >= == !=`)
  - shifts (`<< >>`)
  - unários (`+ - !`)
- Nós especiais:
  - `Assign`, `Call`, `ParseArgs`, `Length`

**Resolução de métodos (`Call`)**
- tenta assinatura exata
- se não existir, tenta assinatura compatível (`int -> double`)
- se >1 compatível: ambiguidade
- se 0 compatíveis: símbolo não encontrado

**Erros semânticos tratados**
- `Cannot find symbol <token>`
- `Reference to method <token> is ambiguous`
- `Operator <op> cannot be applied to type <t>`
- `Operator <op> cannot be applied to types <t1>, <t2>`
- `Incompatible type <t> in if/while/return/... statement`
- `Number <token> out of bounds`
- erros de duplicação/reservado nas tabelas

**Estratégia de robustez**
- em erro, propaga `undef` no nó e continua análise
- permite acumular erros no mesmo input em vez de abortar no primeiro

## Estado final (até Meta 3)

**Opções funcionais**
- `-l`: tokens + erros lexicais
- `-e1`: apenas erros lexicais
- `-t`: erros léxicos/sintáticos + AST (sem erros sintáticos)
- `-e2`: apenas erros léxicos/sintáticos
- `-s`: semântica + tabelas + AST anotada (sem erros sintáticos)
- `-e3`: apenas erros semânticos (após sintaxe válida)

**Conclusão**
- As metas 1, 2 e 3 ficaram integradas no mesmo executável `jucompiler`.
- A arquitetura ficou modular (lexer / parser+AST / semântica), facilitando manutenção e evolução.