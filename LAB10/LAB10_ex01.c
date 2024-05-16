#include <stdio.h>
#include <string.h>

int main() {
    char nome[100];
    int tamanho;

    printf("Digite o seu nome: ");
    fgets(nome, sizeof(nome), stdin);

    tamanho = strlen(nome) - 1;oa

    printf("Nome ao contrário: ");
    for(int i = tamanho - 1; i >= 0; i--) {
        printf("%c", nome[i]);
    }

    printf("\n");
    return 0;
}
