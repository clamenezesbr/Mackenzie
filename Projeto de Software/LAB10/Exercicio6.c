#include <stdio.h>

float calculosomatorio(int n) {
    float resultado = 0.0f;
    int sinal = 1;
    
    for (int i = 1; i <= n; i++) {
        resultado += sinal * ((float)i / (i * i));
        sinal *= -1;
    }
    
    return resultado;
}

int main() {
    int n;

    printf("Digite um valor numérico: ");
    scanf("%d", &n);

    float resultado = calculosomatorio(n);
    printf("O resultado do somatório é: %.14f\n", resultado);

    return 0;
}
