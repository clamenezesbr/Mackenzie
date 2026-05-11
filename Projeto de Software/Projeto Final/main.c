#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PALAVRAS 1000
#define MAX_TAMANHO_PALAVRA 100

typedef struct {
    char palavra[MAX_TAMANHO_PALAVRA];
    int contagem;
} EntradaDicionario;

// Função de busca binária recursiva
int binary_search(EntradaDicionario dict[], int low, int high, char *word) {
    if (high < low)
        return low;
    int mid = (low + high) / 2;
    int cmp = strcmp(dict[mid].palavra, word);
    if (cmp == 0)
        return mid;
    else if (cmp > 0)
        return binary_search(dict, low, mid - 1, word);
    else
        return binary_search(dict, mid + 1, high, word);
}

// Função para inserir uma palavra no dicionário
void insert_word(EntradaDicionario dict[], int *size, char *word) {
    int index = binary_search(dict, 0, *size - 1, word);
    if (index < *size && strcmp(dict[index].palavra, word) == 0) {
        dict[index].contagem++;
    } else {
        for (int i = *size; i > index; i--) {
            dict[i] = dict[i - 1];
        }
        strcpy(dict[index].palavra, word);
        dict[index].contagem = 1;
        (*size)++;
    }
}

// Converte a string para minúsculas
void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Puxa as palavras da string
void extract_words(char *line, EntradaDicionario dict[], int *size) {
    char *token = strtok(line, " ");
    while (token != NULL) {

        // Tira elementos q n sejam alfabéticos no início/fim da palavra
        int start = 0;
        int end = strlen(token) - 1;
        while (start <= end && !isalpha(token[start])) start++;
        while (end >= start && !isalpha(token[end])) end--;
        if (start > end) {
            token = strtok(NULL, " ");
            continue;
        }
        token[end + 1] = '\0';
        to_lower_case(token + start);
        insert_word(dict, size, token + start);
        token = strtok(NULL, " ");
    }
}

// Processar o texto
void processar_texto(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Não foi possível abrir o arquivo");
        printf("Caminho do arquivo: %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    EntradaDicionario dicionario[MAX_PALAVRAS];
    int tamanho = 0;
    char line[1024];
    while (fgets(line, sizeof(line), arquivo)) {
        extract_words(line, dicionario, &tamanho);
    }

    fclose(arquivo); // fechar o arquivo

    // Imprime as palavras do dicionário
    for (int i = 0; i < tamanho; i++) {
        printf("%s, %d\n", dicionario[i].palavra, dicionario[i].contagem);
    }
    printf("total de palavras diferentes no dicionario=%d\n", tamanho);
}

// Abre o arquivo e chama a função de impressao do texto
int main() {
    const char *nome_arquivo = "texto.txt";
    printf("Tentando abrir o arquivo: %s\n", nome_arquivo);
    processar_texto(nome_arquivo);
    return 0;
}
