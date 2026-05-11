#include <stdio.h>

int soma(int n) {
    int total = 0;
    for (int k = 1; k <= n; k++) {
        total += 2 * k * k + k - 1;
    }
    return total;
}

int main() {
    int n;
    printf("Digite um número inteiro: ");
    scanf("%d", &n);
    printf("O valor da soma é: %d\n", soma(n));
    return 0;
}
