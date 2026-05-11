#include <stdio.h>

int inverteNumero(int numero, int invertido) {
    if (numero == 0) {
        return invertido;
    }
    return inverteNumero(numero / 10, invertido * 10 + numero % 10);
}

int inverte(int numero) {
    return inverteNumero(numero, 0);
}

int main() {
    int numero;
    printf("Digite um número inteiro: ");
    scanf("%d", &numero);
    int numeroInvertido = inverte(numero);
    printf("Número invertido: %d\n", numeroInvertido);
    return 0;
}
