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
  int num;
  printf("Digite um número: ");
  scanf("%d", &num);

  if (primo(num)) {
    printf("O número %d é primo.\n", num);
  } else {
    printf("O número %d não é primo.\n", num);
  }

  return 0;
}
