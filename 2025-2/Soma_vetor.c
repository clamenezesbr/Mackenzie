#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long calcular_somatorio(int *vetor, int tamanho) {
    long long soma = 0;
    for (int i = 0; i < tamanho; i++) {
        soma += vetor[i];
    }
    return soma;
}

int main() {
    int N = 100000000;
    int *vetor = (int *)malloc(N * sizeof(int));

    // Inicialização
    for (int i = 0; i < N; i++) {
        vetor[i] = i;
    }

    // Medição de tempo
    clock_t inicio = clock();
    long long soma = calcular_somatorio(vetor, N);
    clock_t fim = clock();

    double tempo_total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Somatório de 1 a %d: %lld\n", N, soma);
    printf("Tempo de execucao: %f segundos\n", tempo_total);

    free(vetor);
    return 0;
}