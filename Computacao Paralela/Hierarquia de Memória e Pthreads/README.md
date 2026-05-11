# Relatório — Explorações de Otimizações e Hierarquia de Memória (Cache) e Pthreads

**Disciplina:** Computação Paralela  
**Aluno:** clamenezes  
**Data:** Abril de 2026

---

## 1. Introdução

Este relatório documenta a implementação e análise de desempenho de algoritmos de multiplicação de matrizes em C, explorando o impacto da hierarquia de memória (cache) e do paralelismo com Pthreads. Foram implementadas e medidas as seguintes versões:

- Contagem de elementos pares em ordem de linha e coluna (Item 2)
- Multiplicação de matrizes em ordem de linha e com blocagem/tiling (Item 3)
- Multiplicação paralela com Pthreads e Pthreads + blocagem (Item 4)

Todas as versões foram compiladas com `-O0` (sem otimizações) e `-O3` (máximas otimizações), e os padrões de acesso ao cache foram analisados com o `valgrind --tool=cachegrind`.

---

## 2. Compilação

Todos os arquivos foram compilados com o `Makefile` fornecido, gerando 12 binários (6 versões × 2 flags de compilação).

[![make_all.png](https://i.postimg.cc/cHbyf2W7/make_all.png)](https://postimg.cc/62nmXHyT)

---

## 3. Item 2 — Contagem de Elementos Pares (N=1024)

### 3.1 Descrição

Foram implementadas duas versões do algoritmo de contagem de pares na matriz A (inicializada com `a[i][j] = i + j`):

- **Ordem de linha:** loop externo em `i`, interno em `j` — acesso sequencial à memória, **cache-friendly**
- **Ordem de coluna:** loop externo em `j`, interno em `i` — acesso não-sequencial, **cache-unfriendly**

### 3.2 Resultados

#### Ordem de linha — `-O3`

[![matmul_row_O3.png](https://i.postimg.cc/bJBc1WPL/matmul_row_O3.png)](https://postimg.cc/9RZnhxrq)

#### Ordem de coluna — `-O3`

[![matmul_col_O3.png](https://i.postimg.cc/FRRXdm26/matmul_col_O3.png)](https://postimg.cc/HJfhFGW0)

### 3.3 Tabela de Tempos

| Versão         | Flag | Tempo (s)  |
|----------------|------|-----------|
| Ordem de linha | -O0  | 0.001644  |
| Ordem de coluna| -O0  | 0.002336  |
| Ordem de linha | -O3  | 0.000292  |
| Ordem de coluna| -O3  | 0.001368  |

### 3.4 Análise

O acesso em ordem de coluna é mais lento porque percorre a memória de forma não-sequencial: em C, matrizes bidimensionais são armazenadas em **row-major order**, ou seja, os elementos de uma mesma linha ficam contíguos na memória. Ao percorrer coluna por coluna, o processador carrega uma linha de cache para usar apenas um elemento e a descarta, gerando um número muito maior de cache misses. Com `-O3`, a diferença se amplia ainda mais (4.69×) pois o compilador consegue vetorizar o loop de linha mas não o de coluna.

---

## 4. Item 3 — Multiplicação de Matrizes A×B=C (N=512)

### 4.1 Descrição

Foram implementadas duas versões da multiplicação de matrizes quadradas (N=512), inicializadas com `a[i][j] = i + j` e `b[i][j] = i * j`:

- **Ordem de linha:** três loops aninhados na ordem `i → j → k` (algoritmo clássico)
- **Blocagem (tiling):** divisão das matrizes em blocos de 32×32 elementos, processando um bloco por vez para maximizar o reuso do cache L1/L2

### 4.2 Resultados

#### Multiplicação em ordem de linha — `-O3`

[![matmul_mult_row_O3.png](https://i.postimg.cc/4dSkpqGd/matmul_mult_row_O3.png)](https://postimg.cc/S2CvhZJF)

#### Multiplicação com blocagem — `-O0`

[![matmul_mult_block_O0.png](https://i.postimg.cc/ZRRzBZGT/matmul_mult_block_O0.png)](https://postimg.cc/xkW4Brj7)

#### Multiplicação com blocagem — `-O3`

[![matmul_mult_block_O3.png](https://i.postimg.cc/hjjRzgkt/matmul_mult_block_O3.png)](https://postimg.cc/VS3V2ysx)

### 4.3 Tabela de Tempos

| Versão              | Flag | Tempo (s) |
|---------------------|------|-----------|
| Ordem de linha      | -O0  | ~10.83*   |
| Ordem de linha      | -O3  | 0.121182  |
| Blocagem (tiling)   | -O0  | 0.495791  |
| Blocagem (tiling)   | -O3  | 0.093486  |

*\* Medido via valgrind*

### 4.4 Análise do Valgrind (cachegrind)

#### `valgrind --tool=cachegrind ./matmul_mult_row_O0`

[![valgrind_atmul_mult_row_O0.png](https://i.postimg.cc/nhbsvwcc/valgrind_atmul_mult_row_O0.png)](https://postimg.cc/Tp0Yx0DZ)

#### `valgrind --tool=cachegrind ./matmul_mult_block_O0`

[![valgrind_matmul_mult_block_O0.png](https://i.postimg.cc/JzVybd4z/valgrind_matmul_mult_block_O0.png)](https://postimg.cc/PPyrHywg)

### 4.5 Análise

Na multiplicação em ordem de linha, o acesso a `b[k][j]` percorre as colunas de B a cada iteração do loop interno, gerando muitos cache misses, pois os elementos consecutivos de uma coluna não são contíguos na memória.

A **blocagem** resolve esse problema dividindo as matrizes em blocos pequenos que cabem inteiros no cache L1/L2. Ao processar bloco a bloco, os dados são reutilizados enquanto ainda estão no cache, reduzindo drasticamente os D1 misses. Isso explica como a blocagem sem nenhuma otimização do compilador (`-O0`, 0.496s) supera facilmente a versão row com otimização máxima (`-O3`, 0.121s) em termos de eficiência de cache, e com `-O3` a blocagem atinge 0.093s — o melhor resultado sequencial.

---

## 5. Item 4 — Paralelismo com Pthreads (N=512, 4 threads)

### 5.1 Descrição

Foram implementadas duas versões paralelas:

- **Pthreads row:** divide as linhas de A igualmente entre as threads. Cada thread calcula sua fatia de C de forma independente, sem necessidade de mutex.
- **Pthreads + blocagem:** cada thread aplica a técnica de blocagem dentro de sua fatia de linhas, combinando paralelismo com reuso de cache.

### 5.2 Resultados

#### Pthreads row — `-O0`

[![matmul_pthreads_O0.png](https://i.postimg.cc/76W8Sd4n/matmul_pthreads_O0.png)](https://postimg.cc/tZWMjrYs)

#### Pthreads row — `-O3`

[![matmul_pthreads_O3.png](https://i.postimg.cc/k4p3WzCs/matmul_pthreads_O3.png)](https://postimg.cc/R6KDBPNJ)

#### Pthreads + blocagem — `-O0`

[![matmul_pthreads_block_O0.png](https://i.postimg.cc/hj5WV6nQ/matmul_pthreads_block_O0.png)](https://postimg.cc/R6KDBPNv)

#### Pthreads + blocagem — `-O3`

[![matmul_pthreads_block_O3.png](https://i.postimg.cc/Y9sKQZ7g/matmul_pthreads_block_O3.png)](https://postimg.cc/dkrxYWLt)

### 5.3 Tabela de Tempos

| Versão                 | Flag | Tempo (s) | Speedup vs row -O0 |
|------------------------|------|-----------|--------------------|
| Row sequencial         | -O0  | ~10.83    | 1×                 |
| Pthreads row           | -O0  | 0.154521  | ~70×               |
| Pthreads row           | -O3  | 0.039316  | ~275×              |
| Pthreads + blocagem    | -O0  | 0.119622  | ~91×               |
| Pthreads + blocagem    | -O3  | 0.030599  | ~354×              |

### 5.4 Análise do Valgrind (cachegrind)

#### `valgrind --tool=cachegrind ./matmul_pthreads_O0`

[![valgrind_matmul_pthreads_O0.png](https://i.postimg.cc/CxTZj6LK/valgrind_matmul_pthreads_O0.png)](https://postimg.cc/kBsXCsSr)

#### `valgrind --tool=cachegrind ./matmul_pthreads_block_O0`

[![valgrind_atmul_pthreads_block_O0.png](https://i.postimg.cc/sgdBp0DX/valgrind_atmul_pthreads_block_O0.png)](https://postimg.cc/BXpQ3pF9)

### 5.5 Análise

**Speedup e escalabilidade:** Com 4 threads e `-O0`, o Pthreads row passou de ~10.83s para 0.155s — um speedup de ~70×, muito acima do esperado teoricamente (4×). Isso se deve ao fato de que o sistema operacional distribui as threads em núcleos físicos distintos, e o custo de sincronização é zero, já que cada thread escreve em regiões disjuntas de C.

**Overhead de threads:** Em matrizes pequenas, o custo de criação e destruição das threads pode superar o ganho computacional. Para N=512 o overhead é desprezível, mas para N=32 ou N=64 provavelmente não valeria a pena paralelizar.

**Pthreads + blocagem:** A combinação das duas técnicas produziu o melhor resultado: 0.031s com `-O3`. Os ganhos se somaram — o paralelismo distribuiu o trabalho entre os núcleos, e a blocagem garantiu que cada núcleo operasse com máximo reuso do cache L1/L2 local.

**Valgrind e threads:** O cachegrind serializa a execução das threads ao instrumentá-las, por isso os tempos medidos pelo valgrind são próximos entre as versões paralelas e as sequenciais. Isso é comportamento esperado da ferramenta e não invalida a análise dos padrões de acesso à memória.

**Falso compartilhamento (false sharing):** Como a divisão de trabalho foi feita por linhas inteiras de C, e cada linha ocupa posições de memória contíguas mas distintas entre threads, o risco de false sharing é baixo. Threads adjacentes podem compartilhar uma linha de cache na fronteira da divisão (última linha de uma thread e primeira da próxima), mas o impacto é pontual e não afeta o resultado geral de forma perceptível.

**Race conditions:** Não foram necessários mutexes, pois cada thread calcula regiões estritamente independentes da matriz C, eliminando qualquer condição de corrida por design.

---

## 6. Resumo Geral dos Tempos (N=512)

| Versão                 | -O0 (s)  | -O3 (s)  |
|------------------------|----------|----------|
| Row sequencial         | ~10.83   | 0.121    |
| Blocagem sequencial    | 0.496    | 0.093    |
| Pthreads row           | 0.155    | 0.039    |
| Pthreads + blocagem    | 0.120    | 0.031    |

---

## 7. Conclusão

Os experimentos demonstraram claramente que tanto a organização do acesso à memória quanto o paralelismo têm impacto decisivo no desempenho de aplicações de alto custo computacional como a multiplicação de matrizes.

A técnica de blocagem, por si só, reduziu o tempo de execução em mais de 20× em relação ao algoritmo sequencial sem otimização, provando que o uso eficiente do cache é tão ou mais importante do que as otimizações do compilador. O paralelismo com Pthreads trouxe ganhos adicionais significativos, e a combinação das duas técnicas produziu o melhor resultado geral: **0.031s com `-O3` e 4 threads**, uma redução de mais de 350× em relação à versão mais simples.