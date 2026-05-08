#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
 * Ex 1 - Soma paralela de vetor
 *
 * Divide um vetor grande entre N threads. Cada thread soma
 * sua fatia e retorna o subtotal. A main coleta os subtotais
 * com pthread_join e calcula o total.
 *
 * Fluxo:
 *   main cria vetor -> divide em fatias -> cria threads
 *   thread[i] soma vetor[inicio..fim) -> retorna long* via heap
 *   main faz join -> coleta retorno -> soma total
 *
 * Dica: cada thread recebe uma struct com os limites da sua fatia.
 * O retorno é um long* alocado com malloc (não pode retornar endereço local).
 */

#define TAM_VETOR   1000000
#define NUM_THREADS 4

typedef struct {
    int id;
    int inicio;
    int fim;
    int *vetor;
} tarefa_t;

void *worker(void *arg) {
    // TODO 1: Converta o argumento void* para tarefa_t*.
    tarefa_t *t = (tarefa_t *)arg;

    // TODO 2: Aloque um long* no heap com malloc().
    //         Calcule a soma dos elementos vetor[inicio..fim).
    //         Armazene o resultado no ponteiro alocado e retorne-o.
    long *soma = malloc(sizeof(long));
    *soma = 0;
    for (int i = t->inicio; i < t->fim; i++)
        *soma += t->vetor[i];

    printf("[thread %d] soma parcial [%d..%d]: %ld\n",
           t->id, t->inicio, t->fim - 1, *soma);

    return soma;
}

int main(void) {
    int vetor[TAM_VETOR];
    for (int i = 0; i < TAM_VETOR; i++)
        vetor[i] = i;

    printf("[main] vetor de %d elementos, %d threads\n", TAM_VETOR, NUM_THREADS);

    pthread_t threads[NUM_THREADS];
    tarefa_t *tarefas[NUM_THREADS];
    int fatia = TAM_VETOR / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        // TODO 3: Aloque uma tarefa_t com malloc().
        //         Preencha os campos: id, inicio, fim, vetor.
        //         A última thread deve ir até TAM_VETOR (para cobrir o resto).
        tarefas[i] = malloc(sizeof(tarefa_t));
        tarefas[i]->id = i;
        tarefas[i]->inicio = i * fatia;
        tarefas[i]->fim = (i == NUM_THREADS - 1) ? TAM_VETOR : (i + 1) * fatia;
        tarefas[i]->vetor = vetor;

        // TODO 4: Crie a thread com pthread_create(), passando a struct como argumento.
        //         Em caso de erro, imprima com perror() e retorne 1.
        if (pthread_create(&threads[i], NULL, worker, tarefas[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    long total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        // TODO 5: Use pthread_join() para esperar a thread terminar.
        //         O segundo argumento recebe o void* retornado pela thread.
        //         Converta para long*, acumule na soma total, e libere a memória.
        void *retorno;
        pthread_join(threads[i], &retorno);
        long *parcial = (long *)retorno;
        total += *parcial;
        free(parcial);

        // TODO 6: Libere a struct tarefa_t desta thread.
        free(tarefas[i]);
    }

    /* verificacao sequencial */
    long esperado = 0;
    for (int i = 0; i < TAM_VETOR; i++)
        esperado += i;

    printf("[main] soma total: %ld\n", total);
    printf("[main] verificacao (sequencial): %ld -- %s\n",
           esperado, total == esperado ? "OK" : "ERRO");

    return 0;
}
