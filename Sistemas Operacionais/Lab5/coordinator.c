#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "hash_utils.h"

/**
 * PROCESSO COORDENADOR - Projeto 1: Quebra de Senhas Paralelo
 * 
 * Este programa coordena múltiplos workers para quebrar senhas MD5 em paralelo.
 * O MD5 JÁ ESTÁ IMPLEMENTADO - você deve focar na paralelização (fork/exec/wait).
 * 
 * Uso: ./coordinator <hash_md5> <tamanho> <charset> <num_workers>
 * 
 * Exemplo: ./coordinator "900150983cd24fb0d6963f7d28e17f72" 3 "abc" 4
 * 
 * SEU TRABALHO: Implementar os TODOs marcados abaixo
 */

#define MAX_WORKERS 16
#define RESULT_FILE "password_found.txt"

/**
 * Calcula o tamanho total do espaço de busca
 * 
 * @param charset_len Tamanho do conjunto de caracteres
 * @param password_len Comprimento da senha
 * @return Número total de combinações possíveis
 */
long long calculate_search_space(int charset_len, int password_len) {
    long long total = 1;
    for (int i = 0; i < password_len; i++) {
        total *= charset_len;
    }
    return total;
}

/**
 * Converte um índice numérico para uma senha
 * Usado para definir os limites de cada worker
 * 
 * @param index Índice numérico da senha
 * @param charset Conjunto de caracteres
 * @param charset_len Tamanho do conjunto
 * @param password_len Comprimento da senha
 * @param output Buffer para armazenar a senha gerada
 */
void index_to_password(long long index, const char *charset, int charset_len, 
                       int password_len, char *output) {
    for (int i = password_len - 1; i >= 0; i--) {
        output[i] = charset[index % charset_len];
        index /= charset_len;
    }
    output[password_len] = '\0';
}

/**
 * Função principal do coordenador
 */
int main(int argc, char *argv[]) {
    // TODO A: Validar argumentos (argc == 5) e parâmetros
    // (password_len: 1–10, num_workers: 1–MAX_WORKERS, charset não vazio).
    // Imprimir mensagem de uso e sair com código 1 em caso de erro.

    if (argc != 5) {
        fprintf(stderr, "Uso: %s <hash_md5> <tamanho> <charset> <num_workers>\n", argv[0]);
        return 1;
    }

    const char *target_hash = argv[1];
    int password_len = atoi(argv[2]);
    const char *charset = argv[3];
    int num_workers = atoi(argv[4]);

    // Validacao do Tamanho da Senha
    // Restricao: Entre 1 e 10
    if (password_len < 1 || password_len > 10){ 
        fprintf(stderr, "Erro: o tamanho da senha deve estar entre 1 e 10.\n");
        fprintf(stderr, "Uso: %s <hash_md5> <tamanho> <charset> <num_workers>\n", argv[0]);
        return 1;
    }

    // Validacao do Numero de Workers
    // Limitado por MAX_WORKERS (Definido como 16)
    if (num_workers < 1 || num_workers > MAX_WORKERS) { 
        fprintf(stderr, "Erro: o numero de workers deve estar entre 1 e %d.\n", MAX_WORKERS);
        fprintf(stderr, "Uso: %s <hash_md5> <tamanho> <charset> <num_workers>\n", argv[0]);
        return 1;
    }

    // Validacao de Charset
    // Nao pode ser vazio, pois define o espaco de busca
    if (charset == NULL || strlen(charset) == 0) { 
        fprintf(stderr, "Erro: o charset nao pode ser vazio.\n");
        fprintf(stderr, "Uso: %s <hash_md5> <tamanho> <charset> <num_workers>\n", argv[0]);
        return 1; 
    }

    int charset_len = strlen(charset);
    
    printf("=== Projeto 1: Quebra de Senhas Paralelo ===\n");
    printf("Hash MD5 alvo: %s\n", target_hash);
    printf("Tamanho da senha: %d\n", password_len);
    printf("Charset: %s (tamanho: %d)\n", charset, charset_len);
    printf("Número de workers: %d\n", num_workers);
    
    // Calcular espaço de busca total
    long long total_space = calculate_search_space(charset_len, password_len);
    printf("Espaço de busca total: %lld combinações\n\n", total_space);
    
    // Remover arquivo de resultado anterior se existir
    unlink(RESULT_FILE);
    
    // Registrar tempo de início
    time_t start_time = time(NULL);
    
    // TODO B: Dividir o espaço de busca e criar workers com fork()/execl()
    // Para cada worker i (0 a num_workers-1): calcule seu intervalo de índices,
    // converta para senhas com index_to_password(), faça fork() e, no filho,
    // execute execl("./worker", ...) com os 6 argumentos. No pai, armazene o PID.
    pid_t workers[MAX_WORKERS];
    printf("Iniciando workers...\n");

    long long chunk_size = total_space / num_workers;

    for (int i = 0; i < num_workers; i++) {
        // Definir o Intervalo de Workers
        long long start_index = i * chunk_size;
        long long end_index;

        // Ultimo worker pega o resto
        if (i == num_workers - 1) { 
            end_index = total_space - 1;
        } else { 
            end_index = (i + 1) * chunk_size - 1;
        }

        // CONVERTER INDEX PARA SENHA
        char start_pass[password_len + 1];
        char end_pass[password_len + 1];

        index_to_password(start_index, charset, charset_len, password_len, start_pass);
        index_to_password(end_index, charset, charset_len, password_len, end_pass);

        // DEBUG 
         printf("Worker %d: [%lld - %lld] → [%s - %s]\n",
               i, start_index, end_index, start_pass, end_pass);

        // CRIAR PROCESSO 
        pid_t pid = fork();

        if (pid < 0) { 
            perror("Erro no fork");
            exit(1);
        }

        if (pid == 0) { 
            // Processo filho
            // Converter valores para string
            char len_str[16];
            char id_str[16];

            snprintf(len_str, sizeof(len_str), "%d", password_len);
            snprintf(id_str, sizeof(id_str), "%d", i);

            // Executa o worker
            execl("./worker",
                  "./worker",
                  target_hash,
                  len_str,
                  charset,
                  start_pass,
                  end_pass,
                  id_str,
                  NULL);

            // Se o Excel falhar
            perror("Erro no execl");
            exit(1);
        } else { 
            // Processo Pai
            workers[i] = pid; // Guarda PID para controle futuro
        }
    }
    
    printf("\nTodos os workers foram iniciados. Aguardando conclusão...\n");
    
    // TODO C: Aguardar workers e encerrar antecipadamente quando a senha for encontrada
    // - Use um loop com wait() chamado num_workers vezes para evitar zumbis
    // - Após cada wait() retornar, verifique se RESULT_FILE existe (use access())
    // - Se existir, envie SIGTERM com kill(workers[i], SIGTERM) aos workers ainda em execução
    // - Continue o loop até coletar todos os filhos
    
    for (int i = 0; i < num_workers; i++) {
        // Aguardar um worker terminar
        wait(NULL);
        
        // Verificar se o arquivo de resultado foi criado
        if (access(RESULT_FILE, F_OK) == 0) {
            // Arquivo existe - senha foi encontrada
            // Enviar SIGTERM aos workers ainda em execução
            printf("Senha encontrada! Encerrando workers remanescentes...\n");
            for (int j = 0; j < num_workers; j++) {
                if (workers[j] > 0) {
                    kill(workers[j], SIGTERM);
                }
            }
            // Continuar coletando os filhos para evitar zumbis
        }
    }
    
    // Registrar tempo de fim
    time_t end_time = time(NULL);
    double elapsed_time = difftime(end_time, start_time);
    
    printf("\n=== Resultado ===\n");
    
    // TODO D: Verificar se a senha foi encontrada
    // Abra RESULT_FILE (O_RDONLY). Se existir, leia o conteúdo,
    // faça parse do formato "worker_id:password" e exiba a senha encontrada.
    // Se o arquivo não existir, imprima uma mensagem informando que a senha não foi encontrada.
    
    int fd = open(RESULT_FILE, O_RDONLY);
    
    if (fd < 0) {
        // Arquivo não existe - senha não foi encontrada
        printf("Senha não encontrada.\n");
    } else {
        // Arquivo existe - ler o conteúdo
        char buffer[256];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            
            // Parse do formato "worker_id:password"
            char *colon = strchr(buffer, ':');
            if (colon != NULL) {
                // Extrair a senha (após o ':')
                char *password = colon + 1;
                // Remover newline se existir
                char *newline = strchr(password, '\n');
                if (newline != NULL) {
                    *newline = '\0';
                }
                printf("Senha encontrada: %s\n", password);
            }
        }
        close(fd);
    }
    
    printf("Tempo de execução: %.2f segundos\n", elapsed_time);
    
    return 0;
}