#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int numero_sorteado, palpite;

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Sorteia um número entre 1 e 100
    numero_sorteado = rand() % 100 + 1;

    printf("Tente adivinhar o número sorteado (1 a 100): ");

    do {
        scanf("%d", &palpite);

        // Verifica se o palpite é válido
        if (palpite < 1 || palpite > 100) {
            printf("Valor inválido. Por favor, digite um número entre 1 e 100.\n");
            continue;
        }

        // Verifica se o palpite está correto
        if (palpite == numero_sorteado) {
            printf("Parabéns!!! Você acertou!\n");
            break;
        } else if (palpite < numero_sorteado) {
            printf("Você chutou muito baixo! O valor correto é %d.\n", numero_sorteado);
        } else {
            printf("Você chutou muito alto! O valor correto é %d.\n", numero_sorteado);
        }
    } while (palpite != numero_sorteado);

    return 0;
}
