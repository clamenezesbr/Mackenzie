#include <stdio.h>

float calcular_soma(float vetor[], int tamanho) {
    float soma = 0;
    for(int i = 0; i < tamanho; i++) {
        soma += vetor[i];
    }
    return soma;
}

void exibir_vetor(float vetor[], int tamanho) {
    printf("[ ");
    for (int i = 0; i < tamanho; i++) {
        printf("%.1f ", vetor[i]);
    }
    printf("]\n");
}

int main() {
    float vetor[] = {1.1, 2.2, 3.3, 3.3, 1.1};
    int tamanho = sizeof(vetor)/sizeof(vetor[0]);
    exibir_vetor(vetor, tamanho);
    printf("soma = %.1f\n", calcular_soma(vetor, tamanho));
    return 0;
}
