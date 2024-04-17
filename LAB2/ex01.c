#include <stdio.h>

int main() {
    int num1, num2;

    // Solicita os números ao usuário
    printf("Digite o primeiro número inteiro: ");
    scanf("%d", &num1);

    printf("Digite o segundo número inteiro: ");
    scanf("%d", &num2);

    // Realiza as operações e imprime os resultados
    printf("Soma: %d\n", num1 + num2);
    printf("Subtração: %d\n", num1 - num2);
    printf("Multiplicação: %d\n", num1 * num2);

    // Verifica se o segundo número é zero antes de realizar a divisão
    if (num2 != 0) {
        printf("Divisão inteira: %d\n", num1 / num2);
    } else {
        printf("Divisão por zero não é permitida.\n");
    }

    return 0;
}
