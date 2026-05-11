#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX 1024

int main() {
    int i, j, par = 0;
    struct timeval tv;
    double start_t, end_t, tempo_gasto;

    /* Aloca a matriz */
    int **a = calloc(MAX, sizeof(int *));
    for (i = 0; i < MAX; i++)
        a[i] = calloc(MAX, sizeof(int));

    /* Inicializa a matriz: a[i][j] = i + j */
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++)
            a[i][j] = i + j;

    printf("=========== Iniciando execucao (ordem de linha) ============\n");

    gettimeofday(&tv, NULL);
    start_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

    /* Percurso em ordem de LINHA: i externo, j interno — cache-friendly */
    for (i = 0; i < MAX; i++)
        for (j = 0; j < MAX; j++)
            if (a[i][j] % 2 == 0)
                par++;

    gettimeofday(&tv, NULL);
    end_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Ordem de linha: %d pares, tempo %.6f secs\n", par, tempo_gasto);

    /* Libera memoria */
    for (i = 0; i < MAX; i++)
        free(a[i]);
    free(a);

    return 0;
}