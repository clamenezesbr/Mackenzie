#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N           512   /* dimensao das matrizes quadradas */
#define BLOCK_SIZE   32   /* tamanho do bloco */
#define NUM_THREADS   4   /* numero de threads — altere para seus testes */

int **a, **b, **c;

typedef struct {
    int linha_inicio;  /* primeira linha de blocos que esta thread processa */
    int linha_fim;     /* ultima linha (exclusive) */
} args_t;

/*
 * Cada thread faz a multiplicacao com blocagem para seu intervalo de linhas.
 * Combinacao: paralelismo de threads + reuso de cache por blocagem.
 */
void *multiplica_block(void *arg) {
    args_t *args = (args_t *)arg;
    int ii, jj, kk, i, j, k;

    for (ii = args->linha_inicio; ii < args->linha_fim; ii += BLOCK_SIZE)
        for (jj = 0; jj < N; jj += BLOCK_SIZE)
            for (kk = 0; kk < N; kk += BLOCK_SIZE)
                for (i = ii; i < ii + BLOCK_SIZE && i < args->linha_fim; i++)
                    for (k = kk; k < kk + BLOCK_SIZE && k < N; k++)
                        for (j = jj; j < jj + BLOCK_SIZE && j < N; j++)
                            c[i][j] += a[i][k] * b[k][j];

    return NULL;
}

int main() {
    int i, j;
    struct timeval tv;
    double start_t, end_t, tempo_gasto;

    pthread_t threads[NUM_THREADS];
    args_t    args[NUM_THREADS];

    /* Aloca matrizes */
    a = calloc(N, sizeof(int *));
    b = calloc(N, sizeof(int *));
    c = calloc(N, sizeof(int *));
    for (i = 0; i < N; i++) {
        a[i] = calloc(N, sizeof(int));
        b[i] = calloc(N, sizeof(int));
        c[i] = calloc(N, sizeof(int));
    }

    /* Inicializa A e B */
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            a[i][j] = i + j;
            b[i][j] = i * j;
        }

    printf("=========== Pthreads + Blocagem (N=%d, BLOCK=%d, threads=%d) ============\n",
           N, BLOCK_SIZE, NUM_THREADS);

    gettimeofday(&tv, NULL);
    start_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

    /*
     * Divide as linhas de A em fatias de tamanho multiplo de BLOCK_SIZE
     * para que cada thread trabalhe em blocos completos sempre que possivel.
     */
    int linhas_por_thread = (N / NUM_THREADS / BLOCK_SIZE) * BLOCK_SIZE;
    if (linhas_por_thread == 0) linhas_por_thread = BLOCK_SIZE;

    for (i = 0; i < NUM_THREADS; i++) {
        args[i].linha_inicio = i * linhas_por_thread;
        args[i].linha_fim    = (i == NUM_THREADS - 1) ? N : (i + 1) * linhas_por_thread;
        pthread_create(&threads[i], NULL, multiplica_block, &args[i]);
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&tv, NULL);
    end_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Pthreads + block: tempo para multiplicar: %.6f secs\n", tempo_gasto);
    printf("C[0][0] = %d\n", c[0][0]);

    /* Libera memoria */
    for (i = 0; i < N; i++) {
        free(a[i]); free(b[i]); free(c[i]);
    }
    free(a); free(b); free(c);

    return 0;
}
