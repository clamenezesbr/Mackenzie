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

// Estrutura para passar argumentos para cada thread
typedef struct {
    char *data_start;       // Ponteiro para o início do bloco desta thread em memória
    long size;              // Tamanho em bytes do bloco a ser processado
    char *full_buffer_start; // Ponteiro para o início absoluto do arquivo
} ThreadData;

// Variáveis Globais Compartilhadas
SensorStats sensores[MAX_SENSORS];
double consumo_total = 0;
int total_alertas = 0;
pthread_mutex_t lock; // Mutex para proteger a seção crítica

// Função executada por cada thread
void* process_log_chunk(void* arg) {
    ThreadData* td = (ThreadData*)arg;
    char *ptr = td->data_start;
    char *end = td->data_start + td->size;
    char linha[256];

    // Ajusta e alinha o ponteiro ao início da próxima linha válida
    if (ptr != td->full_buffer_start) {
        while (ptr < (td->full_buffer_start + td->size * 100) && *(ptr - 1) != '\n') { 
            ptr++;
        }
    }

    while (ptr < end) {
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
                    
                    
                    // Seção Crítica. Travamos o mutex global a cada linha para evitar condições de corrida
                    pthread_mutex_lock(&lock);
                    
                    sensores[idx].ativo = 1;
                    if (sensores[idx].id[0] == '\0') strcpy(sensores[idx].id, s_id);
                    
                    if (strcmp(tipo, "temperatura") == 0) {
                        sensores[idx].soma_temp += valor;
                        sensores[idx].soma_quadrados_temp += (valor * valor);
                        sensores[idx].qtd_temp++;
                    } else if (strcmp(tipo, "energia") == 0) {
                        consumo_total += valor;
                    }
                    if (strcmp(status, "ALERTA") == 0 || strcmp(status, "CRITICO") == 0) {
                        total_alertas++;
                    }
                    
                    pthread_mutex_unlock(&lock); // Libera o acesso para outras threads
                }
            }
        }
        ptr = eol + 1;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <num_threads> <arquivo_de_log>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    char *filename = argv[2];

    // Mapeamento do arquivo para memória que é mais rápido que ler linha por linha do disco
    struct stat st;
    stat(filename, &st);
    long file_size = st.st_size;

    FILE *file = fopen(filename, "r");
    char *all_data = malloc(file_size + 1);
    // Esta forma elimina o warning de 'unused-result' do fread
    size_t result = fread(all_data, 1, file_size, file);
    (void)result;
    all_data[file_size] = '\0';
    fclose(file);

    pthread_t threads[num_threads];
    ThreadData t_args[num_threads];
    pthread_mutex_init(&lock, NULL); // Inicializa o mutex
    memset(sensores, 0, sizeof(sensores));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Divide o tamanho total do arquivo pelo número de threads
    long chunk_size = file_size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        t_args[i].data_start = all_data + (i * chunk_size);
        t_args[i].size = (i == num_threads - 1) ? (file_size - (i * chunk_size)) : chunk_size;
        t_args[i].full_buffer_start = all_data;
        pthread_create(&threads[i], NULL, process_log_chunk, &t_args[i]);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);

    // Cálculos finais que são processados sequencialmente após a união das threads
    double maior_desvio = -1.0;
    int idx_instavel = -1;
    for (int i = 0; i < MAX_SENSORS; i++) {
        if (sensores[i].ativo && sensores[i].qtd_temp > 0) {
            sensores[i].media = sensores[i].soma_temp / sensores[i].qtd_temp;
            double var = (sensores[i].soma_quadrados_temp / sensores[i].qtd_temp) - (sensores[i].media * sensores[i].media);
            sensores[i].desvio_padrao = (var > 0) ? sqrt(var) : 0;
            if (sensores[i].desvio_padrao > maior_desvio) {
                maior_desvio = sensores[i].desvio_padrao;
                idx_instavel = i;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double tempo_par = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\n RESULTADO PARALELO (%d Threads) \n", num_threads);
    printf("Média dos 10 Primeiros Sensores:\n");
    int exibidos = 0;
    for (int i = 0; i < MAX_SENSORS && exibidos < 10; i++) {
        if (sensores[i].ativo && sensores[i].qtd_temp > 0) {
            printf("Sensor %s: Média %.2f°C\n", sensores[i].id, sensores[i].media);
            exibidos++;
        }
    }

    printf("\n ESTATÍSTICAS GERAIS \n");
    if (idx_instavel != -1) printf("Sensor mais instável: %s (Desvio Padrão: %.2f)\n", sensores[idx_instavel].id, maior_desvio);
    printf("Total de alertas: %d\n", total_alertas);
    printf("Consumo total de energia: %.2f Wh\n", consumo_total);
    printf("Tempo de execução paralelo: %.4f segundos\n", tempo_par);

    pthread_mutex_destroy(&lock);
    free(all_data);
    return 0;
}