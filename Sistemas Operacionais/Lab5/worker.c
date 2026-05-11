#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include "hash_utils.h"

/**
 * PROCESSO TRABALHADOR - Projeto 1: Quebra de Senhas Paralelo
 * 
 * Este programa verifica um subconjunto do espaço de senhas, usando a biblioteca
 * MD5 FORNECIDA para calcular hashes e comparar com o hash alvo.
 * 
 * Uso: ./worker <hash_alvo> <senha_inicial> <senha_final> <charset> <tamanho> <worker_id>
 * 
 * EXECUTADO AUTOMATICAMENTE pelo coordinator através de fork() + execl()
 * SEU TRABALHO: Implementar os TODOs de busca e comunicação
 */

#define RESULT_FILE "password_found.txt"
#define PROGRESS_INTERVAL 100000  // Reportar progresso a cada N senhas

/**
 * Flag global de controle do loop principal.
 * Definida como volatile sig_atomic_t para ser segura de modificar em handlers de sinal.
 * O coordinator envia SIGTERM quando outro worker já encontrou a senha.
 */
volatile sig_atomic_t keep_running = 1;

/**
 * Handler para SIGTERM - chamado pelo SO quando o coordinator executa kill(pid, SIGTERM).
 * Deve apenas sinalizar ao loop principal para encerrar.
 *
 * REGRA: handlers de sinal devem ser simples - sem printf, sem malloc, sem funções não-reentrantes.
 */
void handle_sigterm(int sig) {
    // TODO: sete keep_running = 0 para que o loop principal encerre na próxima iteração
    (void)sig;  // suprime warning de parâmetro não utilizado
    keep_running = 0;
}

/**
 * Incrementa uma senha para a próxima na ordem lexicográfica (aaa -> aab -> aac...)
 * 
 * @param password Senha atual (será modificada)
 * @param charset Conjunto de caracteres permitidos
 * @param charset_len Tamanho do conjunto
 * @param password_len Comprimento da senha
 * @return 1 se incrementou com sucesso, 0 se chegou ao limite (overflow)
 */
int increment_password(char *password, const char *charset, int charset_len, int password_len) {
    
    // TODO 1: Incrementar a senha para a próxima ordem lexicográfica (aaa → aab → aba...).
    // Trate como um contador de base variável: o último caractere incrementa primeiro.
    // Retorne 1 se incrementou com sucesso, 0 se esgotou o espaço de busca.
    for (int i = password_len - 1; i >= 0; i--) {
        const char *pos = strchr(charset, password[i]);
        int idx = pos - charset;
        if (idx < charset_len - 1) {
            password[i] = charset[idx + 1];
            return 1;
        }
        password[i] = charset[0];
    }
    return 0;  // overflow: esgotou o espaço de busca
}

/**
 * Compara duas senhas lexicograficamente
 * 
 * @return -1 se a < b, 0 se a == b, 1 se a > b
 */
int password_compare(const char *a, const char *b) {
    return strcmp(a, b);
}

/**
 * Verifica se o arquivo de resultado já existe
 * Usado para parada antecipada se outro worker já encontrou a senha
 */
int check_result_exists() {
    return access(RESULT_FILE, F_OK) == 0;
}

/**
 * Salva a senha encontrada no arquivo de resultado
 * Usa O_CREAT | O_EXCL para garantir escrita atômica (apenas um worker escreve)
 */
void save_result(int worker_id, const char *password) {
    // TODO 2: Salvar a senha encontrada em RESULT_FILE de forma atômica.
    // Use O_CREAT | O_EXCL para garantir que apenas um worker escreva.
    // Formato: "worker_id:password\n"
    int fd = open(RESULT_FILE, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        return;  // outro worker já escreveu primeiro
    }
    char buf[128];
    int len = snprintf(buf, sizeof(buf), "%d:%s\n", worker_id, password);
    write(fd, buf, len);
    close(fd);
}

/**
 * Função principal do worker
 */
int main(int argc, char *argv[]) {
    // Validar argumentos
    if (argc != 7) {
        fprintf(stderr, "Uso interno: %s <hash> <start> <end> <charset> <len> <id>\n", argv[0]);
        return 1;
    }
    
    // Parse dos argumentos
    const char *target_hash = argv[1];
    char *start_password = argv[2];
    const char *end_password = argv[3];
    const char *charset = argv[4];
    int password_len = atoi(argv[5]);
    int worker_id = atoi(argv[6]);
    int charset_len = strlen(charset);
    
    printf("[Worker %d] Iniciado: %s até %s\n", worker_id, start_password, end_password);

    // Registrar handler para SIGTERM (enviado pelo coordinator ao encerrar workers remanescentes)
    signal(SIGTERM, handle_sigterm);

    // Buffer para a senha atual
    char current_password[11];
    strcpy(current_password, start_password);
    
    // Buffer para o hash calculado
    char computed_hash[33];
    
    // Contadores para estatísticas
    long long passwords_checked = 0;
    time_t start_time = time(NULL);
    time_t last_progress_time = start_time;
    
    // Loop principal de verificação
    // keep_running é zerado pelo handler de SIGTERM (coordinator encerrando este worker)
    while (keep_running) {
        // TODO 3: Para cada senha no intervalo [start_password, end_password]:
        //   a) Calcule o hash MD5 com md5_string() e compare com target_hash
        //   b) Se encontrou: chame save_result() e saia do loop
        //   c) A cada PROGRESS_INTERVAL tentativas, verifique check_result_exists()
        //   d) Incremente com increment_password(); saia se retornar 0
        //      ou se a senha atual ultrapassar end_password

        // a) calcular hash e comparar
        md5_string(current_password, computed_hash);
        if (strcmp(computed_hash, target_hash) == 0) {
            save_result(worker_id, current_password);
            break;
        }

        passwords_checked++;

        // c) verificar periodicamente se outro worker já encontrou
        if (passwords_checked % PROGRESS_INTERVAL == 0) {
            if (check_result_exists()) {
                break;
            }
            time_t now = time(NULL);
            printf("[Worker %d] %lld senhas verificadas (%.0f/s)\n",
                   worker_id, passwords_checked,
                   difftime(now, last_progress_time) > 0
                       ? PROGRESS_INTERVAL / difftime(now, last_progress_time)
                       : 0.0);
            last_progress_time = now;
        }

        // d) incrementar; sair se esgotou ou ultrapassou end_password
        if (!increment_password(current_password, charset, charset_len, password_len)) {
            break;
        }
        if (password_compare(current_password, end_password) > 0) {
            break;
        }
    }
    
    // Estatísticas finais
    time_t end_time = time(NULL);
    double total_time = difftime(end_time, start_time);
    
    printf("[Worker %d] Finalizado. Total: %lld senhas em %.2f segundos", 
           worker_id, passwords_checked, total_time);
    if (total_time > 0) {
        printf(" (%.0f senhas/s)", passwords_checked / total_time);
    }
    printf("\n");
    
    return 0;
}
