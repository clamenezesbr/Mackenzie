#include <stdio.h>

void intersecao(int A[], int B[], int tamanhoA, int tamanhoB) {
  printf("{ ");
    for(int i = 0; i < tamanhoA; i++) {
      for(int j = 0; j < tamanhoB; j++) {
        if(A[i] == B[j]) {
          printf("%d ", A[i]);
          break;
      }
    }
  }
  printf("}\n");
}

int main() {
  int A[] = {7, 2, 5, 8, 4};
  int B[] = {4, 2, 9, 5};
  int tamanhoA = sizeof(A) / sizeof(A[0]);
  int tamanhoB = sizeof(B) / sizeof(B[0]);
  
  printf("A B = ");
  intersecao(A, B, tamanhoA, tamanhoB);
  
return 0;
}