#include <stdio.h>

int buscaBinariaRec(int vetor[], int inicio, int fim, int chave) {
    if (fim >= inicio) {
        int meio = inicio + (fim - inicio) / 2;

        if (vetor[meio] == chave) {
            return meio;
        }

        if (vetor[meio] > chave) {
            return buscaBinariaRec(vetor, inicio, meio - 1, chave);
        }

        return buscaBinariaRec(vetor, meio + 1, fim, chave);
    }

    return -1;
}

int main() {
    int vetor[] = {2, 3, 4, 10, 40};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);
    int chave = 10;
    int resultado = buscaBinariaRec(vetor, 0, tamanho - 1, chave);
    
    if (resultado != -1) {
        printf("Elemento encontrado no índice %d\n", resultado);
    } else {
        printf("Elemento não encontrado no array\n");
    }
    
    return 0;
}

