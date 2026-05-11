#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_SENSORS 1001

typedef struct {
    char id[20];
    double soma_temp;
    double soma_quadrados_temp;
    int qtd_temp;
    double media;
    double desvio_padrao;
    int ativo;
} SensorStats;

// Inclui limites de segurança para evitar perda de dados
typedef struct {
    char *data_start;
    long size;
    char *full_buffer_start;
    char *full_buffer_end; // Garante que a thread não tente ler além do fim do arquivo em RAM
} ThreadData;

// Variáveis Globais de Consolidação
SensorStats sensores_globais[MAX_SENSORS];
double consumo_total_global = 0;
int total_alertas_global = 0;
pthread_mutex_t lock_global;

void* process_log_optimized(void* arg) {
    ThreadData* td = (ThreadData*)arg;
    char *ptr = td->data_start;
    char *block_end = td->data_start + td->size;
    char linha[256];

    // Redução Local. Cada thread cria suas próprias variáveis privadas. 
    // Isso elimina a necessidade de travar o mutex dentro do loop de milhões de linhas.
    SensorStats local_sensores[MAX_SENSORS];
    memset(local_sensores, 0, sizeof(local_sensores));
    double local_energia = 0;
    int local_alertas = 0;

    // Ajusta e alinha o ponteiro ao início da próxima linha válida
    if (ptr != td->full_buffer_start) {
        while (ptr < td->full_buffer_end && *(ptr - 1) != '\n') ptr++;
    }

    // Processamento em velocidade máxima sem travas ou mutexes
    while (ptr < block_end && ptr < td->full_buffer_end) {
        char *eol = strchr(ptr, '\n');
        if (!eol) break;

        size_t len = eol - ptr;
        if (len > 0 && len < sizeof(linha)) {
            strncpy(linha, ptr, len);
            linha[len] = '\0';

            char s_id[20], tipo[20], status[20];
            float valor;

            if (sscanf(linha, "%s %*s %*s %s %f %*s %s", s_id, tipo, &valor, status) == 4) {
                int idx = atoi(&s_id[7]);
                if (idx >= 0 && idx < MAX_SENSORS) {
                    local_sensores[idx].ativo = 1;
                    if (local_sensores[idx].id[0] == '\0') strcpy(local_sensores[idx].id, s_id);
                    
                    if (strcmp(tipo, "temperatura") == 0) {
                        local_sensores[idx].soma_temp += valor;
                        local_sensores[idx].soma_quadrados_temp += (valor * valor);
                        local_sensores[idx].qtd_temp++;
                    } else if (strcmp(tipo, "energia") == 0) {
                        local_energia += valor;
                    }
                    if (strcmp(status, "ALERTA") == 0 || strcmp(status, "CRITICO") == 0) {
                        local_alertas++;
                    }
                }
            }
        }
        ptr = eol + 1;
    }

    // Fusõa dos Resultados. O mutex é travado apenas uma vez ao final da thread.
    // Isso reduz o número de travas de milhões para apenas 'N_threads'.
    pthread_mutex_lock(&lock_global);
    consumo_total_global += local_energia;
    total_alertas_global += local_alertas;
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (local_sensores[i].ativo) {
            sensores_globais[i].ativo = 1;
            if (sensores_globais[i].id[0] == '\0') strcpy(sensores_globais[i].id, local_sensores[i].id);
            sensores_globais[i].soma_temp += local_sensores[i].soma_temp;
            sensores_globais[i].soma_quadrados_temp += local_sensores[i].soma_quadrados_temp;
            sensores_globais[i].qtd_temp += local_sensores[i].qtd_temp;
        }
    }
    pthread_mutex_unlock(&lock_global);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    int num_threads = atoi(argv[1]);
    char *filename = argv[2];

    struct stat st;
    stat(filename, &st);
    long file_size = st.st_size;

    // Carregamento completo do arquivo em RAM para acesso aleatório rápido
    FILE *file = fopen(filename, "r");
    char *all_data = malloc(file_size + 1);
    // Esta forma elimina o warning de 'unused-result' do fread
    size_t result = fread(all_data, 1, file_size, file);
    (void)result;
    all_data[file_size] = '\0';
    fclose(file);

    pthread_t threads[num_threads];
    ThreadData t_args[num_threads];
    pthread_mutex_init(&lock_global, NULL);
    memset(sensores_globais, 0, sizeof(sensores_globais));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Lógica de particionamento contíguo
    long chunk_size = file_size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        t_args[i].data_start = all_data + (i * chunk_size);
        t_args[i].size = (i == num_threads - 1) ? (file_size - (i * chunk_size)) : chunk_size;
        t_args[i].full_buffer_start = all_data;
        t_args[i].full_buffer_end = all_data + file_size;
        pthread_create(&threads[i], NULL, process_log_optimized, &t_args[i]);
    }

    for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);

    // Finalização das estatísticas 
    double maior_desvio = -1.0;
    int idx_instavel = -1;
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (sensores_globais[i].ativo && sensores_globais[i].qtd_temp > 0) {
            sensores_globais[i].media = sensores_globais[i].soma_temp / sensores_globais[i].qtd_temp;
            double var = (sensores_globais[i].soma_quadrados_temp / sensores_globais[i].qtd_temp) - (sensores_globais[i].media * sensores_globais[i].media);
            sensores_globais[i].desvio_padrao = (var > 0) ? sqrt(var) : 0;
            if (sensores_globais[i].desvio_padrao > maior_desvio) {
                maior_desvio = sensores_globais[i].desvio_padrao;
                idx_instavel = i;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double tempo_opt = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Saída de dados
    printf("\n RESULTADO OTIMIZADO (%d Threads) \n", num_threads);
    printf("Médias dos 10 Primeiros Sensores:\n");
    int exibidos = 0;
    for (int i = 0; i < MAX_SENSORS && exibidos < 10; i++) {
        if (sensores_globais[i].ativo && sensores_globais[i].qtd_temp > 0) {
            printf("Sensor %s: Média %.2f°C\n", sensores_globais[i].id, sensores_globais[i].media);
            exibidos++;
        }
    }
    printf("\n ESTATÍSTICAS GERAIS \n");
    if (idx_instavel != -1) printf("Sensor mais instável: %s (Desvio Padrão: %.2f)\n", sensores_globais[idx_instavel].id, maior_desvio);
    printf("Total de alertas: %d\n", total_alertas_global);
    printf("Consumo total de energia: %.2f Wh\n", consumo_total_global);
    printf("Tempo de execução otimizada: %.4f segundos\n", tempo_opt);

    pthread_mutex_destroy(&lock_global);
    free(all_data);
    return 0;
}