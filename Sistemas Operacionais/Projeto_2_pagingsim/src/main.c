#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memoria.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: ./simulador <algoritmo> <arquivo_config> <arquivo_acessos>\n");
        return 1;
    }

    char *algoritmo = argv[1];
    char *arquivo_config = argv[2];
    char *arquivo_acessos = argv[3];

    if (strcmp(algoritmo, "fifo") != 0 && strcmp(algoritmo, "clock") != 0) {
        printf("Algoritmo inválido. Use fifo ou clock.\n");
        return 1;
    }

    Simulador sim;

    inicializar_simulador(&sim);

    carregar_configuracao(&sim, arquivo_config);

    executar_acessos(&sim, arquivo_acessos, algoritmo);

    liberar_simulador(&sim);

    return 0;
}