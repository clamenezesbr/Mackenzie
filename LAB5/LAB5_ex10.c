#include <stdio.h>
#include <stdbool.h>

bool existeNoVetor(int vetor[], int tamanho, int elemento) {
  for (int i = 0; i < tamanho; i++) {
    if (vetor[i] == elemento) return true;
  }
  return false;
}

void imprimirUniao(int A[], int tamanhoA, int B[], int tamanhoB) {
  for (int i = 0; i < tamanhoA; i++) {
    printf("%d ", A[i]);
  }

  for (int j = 0; j < tamanhoB; j++) {
    if (!existeNoVetor(A, tamanhoA, B[j])) {
      printf("%d ", B[j]);
    }
  }
  printf("\n");
}

int main() {
  int A1[] = {7, 2, 5, 8, 4};
  int B1[] = {4, 2, 9, 5};
  
  printf("A B = ");
  imprimirUniao(A1, sizeof(A1)/sizeof(A1[0]), B1,sizeof(B1)/sizeof(B1[0]));
  
  int A2[] = {3, 9 ,11};
  int B2[] = {2 ,6 ,1};
  printf("A B = ");
  imprimirUniao(A2,sizeof(A2)/sizeof(A2[0]),B2,sizeof(B2)/sizeof(B2[0]));
  
return 0;
}