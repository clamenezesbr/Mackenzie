#include <stdio.h>
#include <stdbool.h>

bool primo(int num) {
  if (num <= 1) return false; // números menores ou iguais a 1 não são primos
  if (num == 2) return true; // 2 é primo
  if (num % 2 == 0) return false; // números pares não são primos
  
for (int i = 3; i * i <= num; i += 2) {
  if (num % i == 0) return false; // se num é divisível por algum número até sua raiz quadrada, então não é primo
  }
  return true; // se passou por todas as verificações, então é primo
}

int main() {
  int N1, N2;
// Solicita ao usuário que insira dois números inteiros
  printf("Digite dois números inteiros:\n");
  scanf("%d%d", &N1, &N2);
// Imprime todos os números primos entre N1 e N2
  printf("Os números primos entre %d e %d são:\n", N1, N2);
  for (int i = N1; i <= N2; i++) {
    if (primo(i)) {
    printf("%d ", i);
    }
  }
  return 0;
}