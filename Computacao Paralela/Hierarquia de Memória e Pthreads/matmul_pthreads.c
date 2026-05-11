#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define N           512   /* dimensao das matrizes quadradas */
#define NUM_THREADS   4   /* numero de threads — altere para seus testes */

/* Matrizes globais: cada thread le A e B e escreve em regioes distintas de C */
int **a, **b, **c;

/* Estrutura de argumentos passada para cada thread */
typedef struct {
    int linha_inicio;  /* primeira linha de C que esta thread calcula */
    int linha_fim;     /* ultima linha (exclusive) */
} args_t;

/* Funcao executada por cada thread */
void *multiplica(void *arg) {
    args_t *args = (args_t *)arg;
    int i, j, k;

    /*
     * Cada thread calcula apenas as linhas [linha_inicio, linha_fim) de C.
     * Como as regioes sao disjuntas, nao ha condicao de corrida e
     * nenhum mutex e necessario neste laco.
     */
    for (i = args->linha_inicio; i < args->linha_fim; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
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

    printf("=========== Pthreads (N=%d, threads=%d) ============\n",
           N, NUM_THREADS);

    gettimeofday(&tv, NULL);
    start_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

    /* Divide as linhas de A igualmente entre as threads */
    int linhas_por_thread = N / NUM_THREADS;
    for (i = 0; i < NUM_THREADS; i++) {
        args[i].linha_inicio = i * linhas_por_thread;
        /* ultima thread pega o restante caso N nao seja divisivel */
        args[i].linha_fim = (i == NUM_THREADS - 1) ? N : (i + 1) * linhas_por_thread;
        pthread_create(&threads[i], NULL, multiplica, &args[i]);
    }

    /* Aguarda todas as threads terminarem */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    gettimeofday(&tv, NULL);
    end_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Pthreads row: tempo para multiplicar: %.6f secs\n", tempo_gasto);
    printf("C[0][0] = %d\n", c[0][0]);

    /* Libera memoria */
    for (i = 0; i < N; i++) {
        free(a[i]); free(b[i]); free(c[i]);
    }
    free(a); free(b); free(c);

    return 0;
}
