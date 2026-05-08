# Lab 04 - Threads e Sincronização

Implementação de programas concorrentes com pthreads, cobrindo criação de threads, race conditions e mecanismos de sincronização.

---

## Ex 1 - Soma paralela de vetor (`ex1_soma_paralela.c`)

**O que o professor pediu:** dividir um vetor de 1.000.000 inteiros entre 4 threads. Cada thread soma sua fatia e retorna o subtotal via heap. A main coleta os resultados com `pthread_join` e soma o total, verificando contra uma soma sequencial.

**Como foi resolvido:** cada thread recebe uma `struct` com os limites da sua fatia (`inicio`, `fim`). O resultado é alocado com `malloc` (retornar endereço de variável local seria inválido). A main libera cada ponteiro após coletar o valor.

---

## Ex 2 - Contagem sem proteção (`ex2_conta_palavras.c`)

**O que o professor pediu:** 8 threads contam ocorrências do caractere `'a'` em um texto de 500.000 caracteres, todas incrementando o mesmo contador global **sem mutex**. O objetivo é observar o resultado errado causado pela race condition.

**Como foi resolvido:** o incremento `(*contador)++` é feito sem proteção propositalmente. A contagem sequencial ao final comprova a divergência.

---

## Ex 3 - Contagem com mutex (`ex3_conta_protegida.c`)

**O que o professor pediu:** corrigir o ex2 adicionando um `pthread_mutex_t` para proteger o incremento do contador. O diff entre ex2 e ex3 deve ser mínimo.

**Como foi resolvido:** adicionado um `pthread_mutex_t *mutex` na struct, com `lock`/`unlock` ao redor do `(*contador)++`. Resultado passa a bater com a contagem sequencial (diferença = 0).

---

## Ex 4 - Produtor-Consumidor (`ex4_fila_pedidos.c`)

**O que o professor pediu:** 3 produtores inserem pedidos num buffer circular de 5 posições; 2 consumidores retiram e processam. Produtores esperam quando o buffer está cheio e consumidores esperam quando está vazio — sem busy-wait, usando condition variables.

**Como foi resolvido:** uso de `pthread_mutex_t` + duas `pthread_cond_t` (`nao_cheio`, `nao_vazio`). O padrão `while (condicao) pthread_cond_wait(...)` protege contra spurious wakeups. Ao fim da produção, um `pthread_cond_broadcast` acorda consumidores para que percebam o encerramento.

---

## Ex 5 - Leitores-Escritores (`ex5_cache_rw.c`) *(desafio)*

**O que o professor pediu:** múltiplos leitores acessam um cache simultaneamente; escritores precisam de acesso exclusivo. Implementar o padrão first-reader/last-reader com `pthread_mutex_t` e `sem_t`.

**Como foi resolvido:** o mutex protege o contador de leitores ativos. O primeiro leitor faz `sem_wait` (bloqueia escritores); o último faz `sem_post` (libera). Escritores usam diretamente `sem_wait`/`sem_post` para acesso exclusivo.

---

## Compilação

```bash
make        # compila todos
make ex1    # compila só o ex1
make clean  # remove binários
```

Flags usadas: `gcc -Wall -Wextra -std=c11 -pthread`
