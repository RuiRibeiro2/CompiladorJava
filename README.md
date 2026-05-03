# CompiladorJava

Relatório até à Meta 3(última meta desenvolvida) — Compilador Juc (`jucompiler`)

Autores:
- Leonardo Duarte — 2023213089
- Rui Ribeiro — 202118947

## 1. Objetivo e enquadramento
Até à Meta 3, desenvolvemos um compilador para a linguagem Juc (subconjunto de Java SE 9), com três fases funcionais:
- análise lexical (Meta 1),
- análise sintática com construção de AST (Meta 2),
- análise semântica com tabelas de símbolos e anotação de tipos (Meta 3).

O compilador lê da entrada padrão (`stdin`) e suporta as opções pedidas no enunciado: `-l`, `-e1`, `-t`, `-e2`, `-s`, `-e3`, além do modo por omissão.

## 2. Meta 1 — Análise lexical
O analisador lexical foi implementado em `lex` (`jucompiler.l`) e reconhece os tokens definidos no enunciado:
- identificadores, naturais, decimais, string literals, bool literals,
- operadores, delimitadores e palavras‑chave de Juc,
- `RESERVED` para palavras reservadas fora do subconjunto.

Foi implementada contagem de linha/coluna (início em 1) e tratamento de comentários `//` e `/* ... */`.  
Erros lexicais produzidos no formato oficial:
- `illegal character`,
- `invalid escape sequence`,
- `unterminated comment`,
- `unterminated string literal`.

Com `-l`, imprime tokens + erros; com `-e1` (ou sem opção lexical), imprime apenas erros.

## 3. Meta 2 — Análise sintática e AST
A gramática EBNF do enunciado foi convertida para Yacc/Bison (`jucompiler.y`), com:
- regras de precedência/associatividade para expressões,
- resolução do “dangling else” por separação de statements matched/unmatched,
- recuperação básica de erros sintáticos com `error`.

A AST contém apenas os nós relevantes (sem nós supérfluos), incluindo:
`Program`, `MethodDecl`, `FieldDecl`, `VarDecl`, `If`, `While`, `Return`, `Print`, `Assign`, `Call`, `ParseArgs`, operadores e literais.

Com `-t`, imprime a AST no formato exigido; com `-e2`, reporta só erros léxicos/sintáticos.

## 4. Meta 3 — Análise semântica
A semântica foi implementada em C (`semantic.h`/`semantic.c`) com:

### 4.1 Tabelas de símbolos
- tabela global da classe: campos e métodos (com assinatura),
- tabela por método: `return`, parâmetros e variáveis locais.

Foram validadas:
- redefinições (`Symbol <token> already defined`),
- nome reservado `_` (`Symbol _ is reserved`).

Com `-s`, são impressas as tabelas e a AST anotada.

### 4.2 Anotação de tipos
Cada nó de expressão recebe `sem_type`:
- terminais: `int`, `double`, `boolean`,
- identificadores por lookup em escopo de método e depois classe,
- operadores aritméticos, lógicos, comparação, shifts, unários,
- `Assign`, `Call`, `ParseArgs`, `Length`.

A resolução de métodos considera:
- assinatura exata,
- compatibilidade com promoção `int -> double`,
- erro de ambiguidade ou símbolo inexistente.

### 4.3 Erros semânticos
Foram integrados os erros semânticos pedidos no enunciado (incluindo incompatibilidades em `if`, `while`, `return`, operadores, bounds de naturais e chamadas ambíguas).  
A análise continua após erro, usando `undef` para manter propagação sem abortar compilação.

## 5. Estruturas de dados e decisões técnicas
- AST com nós ligados por `child/sibling`, incluindo `line/col` e `sem_type`.
- Tabelas de símbolos por listas ligadas simples, suficientes para o escopo da linguagem (uma classe estática, sem blocos com novo escopo lexical independente).
- Separação clara entre parsing e semântica permitiu evoluir de Meta 2 para Meta 3 sem refatoração estrutural pesada.

## 6. Estado final até Meta 3
O `jucompiler` cumpre os requisitos funcionais das Metas 1, 2 e 3 do enunciado:
- opções e formatos de output alinhados,
- AST correta,
- tabelas de símbolos e tipagem semântica com deteção de erros.

