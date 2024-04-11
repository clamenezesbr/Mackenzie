//recurção

#include <stdio.h>

int torre(int base){
  int total = 0;
  for(int i = base; i>0; i--){
    total += i;
  }
  return total;
}

void saudar(int contador) {
  printf("%d\n" , contador);  
  if (contador < 1000) {
  contador++;
  saudar(contador);
  }
}

int main(void) {
  int base;
  printf("Copos na base: ");
  scanf("%d" , &base);
  int copos = torre(base);
  printf("Total de copos: %d\n" , copos);
  return 0;
}