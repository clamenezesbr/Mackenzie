void multiplica_matrizes(int n, int m, int p, int A[n][m], int B[m][p], int C[n][p]) {
  for(int i=0; i<n; i++) {
    for(int j=0; j<p; j++) {C[i][j] = 0;
      for(int k=0; k<m; k++) {C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

int main() {
  int n = 2, m = 2, p = 2;
  int A[2][2] = {{1, 2}, {3, 4}};
  int B[2][2] = {{5, 6}, {7, 8}};
  int C[2][2];

multiplica_matrizes(n, m, p, A, B, C);

printf("A multiplicação das matrizes A e B é:\n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<p; j++) {
      printf("%d ", C[i][j]);
    }
    printf("\n");
  }
return 0;
}