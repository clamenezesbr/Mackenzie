#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/*
 * Ex 5 - Leitores-Escritores: cache compartilhado (desafio)
 *
 * Multiplos leitores acessam o cache simultaneamente.
 * Escritores precisam de acesso exclusivo.
 *
 * Padrao first-reader/last-reader:
 *   - O primeiro leitor trava o recurso (bloqueia escritores).
 *   - Os demais leitores entram livremente.
 *   - O ultimo leitor libera o recurso (permite escritores).
 *   - Escritores sempre travam o recurso sozinhos.
 *
 * Primitivas: pthread_mutex_t (protege o contador de leitores)
 *             sem_t           (controla acesso exclusivo ao recurso)
 */

#define TAM_CACHE       10
#define NUM_LEITORES    5
#define NUM_ESCRITORES  2
#define OPS_POR_THREAD  3

typedef struct {
    int dados[TAM_CACHE];
    int versao;

    pthread_mutex_t mutex_leitores;
    sem_t sem_recurso;
    int leitores_ativos;
} cache_t;

typedef struct {
    int id;
    cache_t *cache;
} thread_arg_t;

void iniciar_leitura(cache_t *c) {
    // TODO 2: Trave mutex_leitores, incremente leitores_ativos.
    pthread_mutex_lock(&c->mutex_leitores);
    c->leitores_ativos++;

    // TODO 3: Se for o primeiro leitor (leitores_ativos == 1),
    //         faca sem_wait(&c->sem_recurso) para bloquear escritores.
    //         Depois destrave mutex_leitores.
    if (c->leitores_ativos == 1)
        sem_wait(&c->sem_recurso);
    pthread_mutex_unlock(&c->mutex_leitores);
}

void terminar_leitura(cache_t *c) {
    // TODO 4: Trave mutex_leitores, decremente leitores_ativos.
    pthread_mutex_lock(&c->mutex_leitores);
    c->leitores_ativos--;

    // TODO 5: Se for o ultimo leitor (leitores_ativos == 0),
    //         faca sem_post(&c->sem_recurso) para liberar escritores.
    //         Depois destrave mutex_leitores.
    if (c->leitores_ativos == 0)
        sem_post(&c->sem_recurso);
    pthread_mutex_unlock(&c->mutex_leitores);
}

void iniciar_escrita(cache_t *c) {
    // TODO 6: Faca sem_wait(&c->sem_recurso) para acesso exclusivo.
    sem_wait(&c->sem_recurso);
    (void)c;
}

void terminar_escrita(cache_t *c) {
    // TODO 7: Faca sem_post(&c->sem_recurso) para liberar o recurso.
    sem_post(&c->sem_recurso);
    (void)c;
}

void *leitor(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    cache_t *c = a->cache;

    for (int op = 0; op < OPS_POR_THREAD; op++) {
        iniciar_leitura(c);

        /* le o cache */
        long soma = 0;
        for (int i = 0; i < TAM_CACHE; i++)
            soma += c->dados[i];

        printf("[leitor %d] versao %d, soma=%ld (leitores ativos: %d)\n",
               a->id, c->versao, soma, c->leitores_ativos);

        terminar_leitura(c);

        usleep(rand() % 50000);
    }

    return NULL;
}

void *escritor(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    cache_t *c = a->cache;

    for (int op = 0; op < OPS_POR_THREAD; op++) {
        iniciar_escrita(c);

        /* atualiza o cache */
        c->versao++;
        for (int i = 0; i < TAM_CACHE; i++)
            c->dados[i] = c->versao * 10 + i;

        printf("[escritor %d] atualizou cache para versao %d\n",
               a->id, c->versao);

        terminar_escrita(c);

        usleep(rand() % 80000);
    }

    return NULL;
}

int main(void) {
    srand(42);

    cache_t cache = { .versao = 0, .leitores_ativos = 0 };
    for (int i = 0; i < TAM_CACHE; i++)
        cache.dados[i] = i;

    // TODO 1: Inicialize mutex_leitores e sem_recurso.
    //         pthread_mutex_init(&cache.mutex_leitores, NULL);
    //         sem_init(&cache.sem_recurso, 0, 1);
    pthread_mutex_init(&cache.mutex_leitores, NULL);
    sem_init(&cache.sem_recurso, 0, 1);

    printf("=== Cache Compartilhado (Leitores-Escritores) ===\n");
    printf("Leitores: %d | Escritores: %d | Operacoes: %d cada\n\n",
           NUM_LEITORES, NUM_ESCRITORES, OPS_POR_THREAD);

    pthread_t leitores[NUM_LEITORES], escritores[NUM_ESCRITORES];
    thread_arg_t args_l[NUM_LEITORES], args_e[NUM_ESCRITORES];

    for (int i = 0; i < NUM_LEITORES; i++) {
        args_l[i] = (thread_arg_t){ .id = i, .cache = &cache };
        pthread_create(&leitores[i], NULL, leitor, &args_l[i]);
    }
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        args_e[i] = (thread_arg_t){ .id = i, .cache = &cache };
        pthread_create(&escritores[i], NULL, escritor, &args_e[i]);
    }

    for (int i = 0; i < NUM_LEITORES; i++)
        pthread_join(leitores[i], NULL);
    for (int i = 0; i < NUM_ESCRITORES; i++)
        pthread_join(escritores[i], NULL);

    printf("\n=== Resultado ===\n");
    printf("Leituras: %d | Escritas: %d\n",
           NUM_LEITORES * OPS_POR_THREAD, NUM_ESCRITORES * OPS_POR_THREAD);
    printf("Versao final: %d\n", cache.versao);

    // TODO 8: Destrua mutex e semaforo.
    //         pthread_mutex_destroy, sem_destroy.
    pthread_mutex_destroy(&cache.mutex_leitores);
    sem_destroy(&cache.sem_recurso);

    return 0;
}
