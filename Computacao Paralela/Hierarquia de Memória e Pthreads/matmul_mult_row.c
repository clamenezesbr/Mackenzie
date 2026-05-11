#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 512   /* dimensao das matrizes quadradas */

int main() {
    int i, j, k;
    struct timeval tv;
    double start_t, end_t, tempo_gasto;

    /* Aloca matrizes A, B e C */
    int **a = calloc(N, sizeof(int *));
    int **b = calloc(N, sizeof(int *));
    int **c = calloc(N, sizeof(int *));
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

    printf("=========== Multiplicacao em ordem de linha (N=%d) ============\n", N);

    gettimeofday(&tv, NULL);
    start_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

    /*
     * Multiplicacao A * B = C em ordem de linha.
     * Loop: i -> j -> k  (acesso a b[k][j] nao e cache-friendly,
     * mas e o algoritmo classico direto)
     */
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                c[i][j] += a[i][k] * b[k][j];

    gettimeofday(&tv, NULL);
    end_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Row order: tempo para multiplicar: %.6f secs\n", tempo_gasto);

    /* Verificacao simples: imprime c[0][0] */
    printf("C[0][0] = %d\n", c[0][0]);

    /* Libera memoria */
    for (i = 0; i < N; i++) {
        free(a[i]); free(b[i]); free(c[i]);
    }
    free(a); free(b); free(c);

    return 0;
}
