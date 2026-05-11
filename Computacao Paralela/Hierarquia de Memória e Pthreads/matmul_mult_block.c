#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N          512   /* dimensao das matrizes quadradas */
#define BLOCK_SIZE  32   /* tamanho do bloco — ajuste conforme o cache L1 */

int main() {
    int i, j, k, ii, jj, kk;
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

    printf("=========== Multiplicacao com blocagem/tiling (N=%d, BLOCK=%d) ============\n",
           N, BLOCK_SIZE);

    gettimeofday(&tv, NULL);
    start_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

    /*
     * Multiplicacao com BLOCAGEM (tiling).
     *
     * A ideia e dividir as matrizes em blocos de tamanho BLOCK_SIZE x BLOCK_SIZE.
     * Cada bloco cabe inteiro no cache L1/L2, reduzindo drasticamente os
     * cache misses em comparacao com o algoritmo de ordem de linha simples.
     *
     * Os loops externos (ii, jj, kk) percorrem os blocos.
     * Os loops internos (i, j, k) percorrem os elementos dentro de cada bloco.
     */
    for (ii = 0; ii < N; ii += BLOCK_SIZE)
        for (jj = 0; jj < N; jj += BLOCK_SIZE)
            for (kk = 0; kk < N; kk += BLOCK_SIZE)
                /* bloco atual: linhas [ii..ii+BS), colunas [jj..jj+BS) */
                for (i = ii; i < ii + BLOCK_SIZE && i < N; i++)
                    for (k = kk; k < kk + BLOCK_SIZE && k < N; k++)
                        for (j = jj; j < jj + BLOCK_SIZE && j < N; j++)
                            c[i][j] += a[i][k] * b[k][j];

    gettimeofday(&tv, NULL);
    end_t = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Block order: tempo para multiplicar: %.6f secs\n", tempo_gasto);

    /* Verificacao simples: imprime c[0][0] — deve ser igual ao da versao row */
    printf("C[0][0] = %d\n", c[0][0]);

    /* Libera memoria */
    for (i = 0; i < N; i++) {
        free(a[i]); free(b[i]); free(c[i]);
    }
    free(a); free(b); free(c);

    return 0;
}
