#include<stdio.h>

void soma_matrizes(int n, int m, int A[n][m], int B[n][m], int C[n][m]) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++) {C[i][j] = A[i][j] + B[i][j];
    }
  }
}

int main() {int n = 2, m = 2;
  int A[2][2] = {{1, 2}, {3, 4}};
  int B[2][2] = {{5, 6}, {7, 8}};
  int C[2][2];
            
soma_matrizes(n, m, A, B, C);
  printf("A soma das matrizes A e B é:\n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<m; j++) {
      printf("%d ", C[i][j]);
    }
    printf("\n");
  }
  return 0;
}