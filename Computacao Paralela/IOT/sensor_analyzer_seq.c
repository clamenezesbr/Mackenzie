#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_SENSORS 1001

// Estrutura para armazenar as métricas de cada sensor individualmente
typedef struct {
    char id[20];
    double soma_temp;
    double soma_quadrados_temp; // Usado para o cálculo do desvio padrão (variância)
    int qtd_temp;
    double media;
    double desvio_padrao;
    int ativo; // Flag para identificar se o sensor apareceu no log
} SensorStats;

int main(int argc, char *argv[]) {
    // Verificação mínima de argumentos via linha de comando
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_log>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) { perror("Erro ao abrir arquivo"); return 1; }

    // Inicialização da estrutura de dados e acumuladores globais
    SensorStats sensores[MAX_SENSORS];
    memset(sensores, 0, sizeof(sensores));
    double consumo_total = 0;
    int total_alertas = 0;
    char linha[256];

    // Início da medição de tempo de CPU
    clock_t start_time = clock();

    // Primeira Passada. Processamento linha a linha do arquivo
    // Esta etapa é puramente sequencial e depende da velocidade de I/O do disco
    while (fgets(linha, sizeof(linha), file)) {
        char s_id[20], tipo[20], status[20];
        float valor;
        // Parsing da linha. Extrai ID, tipo de métrica, valor e status operacional
        if (sscanf(linha, "%s %*s %*s %s %f %*s %s", s_id, tipo, &valor, status) == 4) {
            int idx = atoi(&s_id[7]); // Converte "sensor_XXX" para índice inteiro
            if (idx >= 0 && idx < MAX_SENSORS) {
                sensores[idx].ativo = 1;
                if (sensores[idx].id[0] == '\0') strcpy(sensores[idx].id, s_id);
                
                // Acumulação de dados baseada no tipo de sensor
                if (strcmp(tipo, "temperatura") == 0) {
                    sensores[idx].soma_temp += valor;
                    sensores[idx].soma_quadrados_temp += (valor * valor);
                    sensores[idx].qtd_temp++;
                } else if (strcmp(tipo, "energia") == 0) {
                    consumo_total += valor;
                }
                
                // Contagem global de eventos de alerta ou críticos
                if (strcmp(status, "ALERTA") == 0 || strcmp(status, "CRITICO") == 0) {
                    total_alertas++;
                }
            }
        }
    }

    // Estatísticas Finais
    double maior_desvio = -1.0;
    int idx_instavel = -1;
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (sensores[i].ativo && sensores[i].qtd_temp > 0) {
            // Média aritmética simples
            sensores[i].media = sensores[i].soma_temp / sensores[i].qtd_temp;
            // Cálculo da variância e desvio padrão para identificar instabilidade
            double var = (sensores[i].soma_quadrados_temp / sensores[i].qtd_temp) - (sensores[i].media * sensores[i].media);
            sensores[i].desvio_padrao = (var > 0) ? sqrt(var) : 0;
            
            if (sensores[i].desvio_padrao > maior_desvio) {
                maior_desvio = sensores[i].desvio_padrao;
                idx_instavel = i;
            }
        }
    }

    printf("\n RELATÓRIO DE LOGS IoT \n");
    printf("Média dos 10 Primeiros Sensores:\n");
    int exibidos = 0;
    for (int i = 0; i < MAX_SENSORS && exibidos < 10; i++) {
        if (sensores[i].ativo && sensores[i].qtd_temp > 0) {
            printf("Sensor %s: Média %.2f°C\n", sensores[i].id, sensores[i].media);
            exibidos++;
        }
    }

    // Finalização da medição de tempo
    clock_t end_time = clock();
    double total_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\n ESTATÍSTICAS GERAIS \n");
    if (idx_instavel != -1) printf("Sensor mais instável: %s (Desvio Padrão: %.2f)\n", sensores[idx_instavel].id, maior_desvio);
    printf("Total de alertas: %d\n", total_alertas);
    printf("Consumo total de energia: %.2f Wh\n", consumo_total);
    printf("Tempo de execução: %.4f segundos\n", total_time);

    fclose(file);
    return 0;
}