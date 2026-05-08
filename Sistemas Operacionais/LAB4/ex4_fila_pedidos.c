#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Ex 4 - Produtor-Consumidor: fila de pedidos
 *
 * Produtores criam pedidos e inserem num buffer circular.
 * Consumidores retiram e processam. Buffer tem capacidade limitada:
 * produtores esperam quando cheio, consumidores esperam quando vazio.
 *
 * Mecanismo: mutex + condition variables (nao busy-wait).
 *
 * Atencao ao padrao:
 *   while (condicao_desfavoravel)
 *       pthread_cond_wait(&cond, &mutex);
 *
 * Nunca use 'if' no lugar de 'while' - spurious wakeups existem.
 */

#define TAM_BUFFER      5
#define NUM_PRODUTORES  3
#define NUM_CONSUMIDORES 2
#define PEDIDOS_POR_PRODUTOR 4

typedef struct {
    int id;
    int valor;
} pedido_t;

typedef struct {
    pedido_t dados[TAM_BUFFER];
    int inicio;
    int fim;
    int quantidade;

    pthread_mutex_t mutex;
    pthread_cond_t nao_cheio;
    pthread_cond_t nao_vazio;

    int total_produzidos;
    int total_consumidos;
    int producao_encerrada;
} fila_t;

typedef struct {
    int id;
    fila_t *fila;
} thread_arg_t;

static int proximo_id = 1;

void *produtor(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    fila_t *f = a->fila;

    for (int i = 0; i < PEDIDOS_POR_PRODUTOR; i++) {
        pedido_t p;
        p.valor = rand() % 1000;

        // TODO 2: Trave o mutex antes de acessar o buffer.
        pthread_mutex_lock(&f->mutex);

        // TODO 3: Enquanto o buffer estiver cheio, espere com
        //         pthread_cond_wait(&f->nao_cheio, &f->mutex).
        //         Use while, nao if!
        while (f->quantidade == TAM_BUFFER)
            pthread_cond_wait(&f->nao_cheio, &f->mutex);

        p.id = proximo_id++;
        f->dados[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_BUFFER;
        f->quantidade++;
        f->total_produzidos++;

        printf("[produtor %d] pedido #%d (R$%d) inserido [buffer: %d/%d]\n",
               a->id, p.id, p.valor, f->quantidade, TAM_BUFFER);

        // TODO 4: Sinalize que o buffer nao esta mais vazio
        //         (pthread_cond_signal) e destrave o mutex.
        pthread_cond_signal(&f->nao_vazio);
        pthread_mutex_unlock(&f->mutex);

        usleep(rand() % 50000);
    }

    return NULL;
}

void *consumidor(void *arg) {
    thread_arg_t *a = (thread_arg_t *)arg;
    fila_t *f = a->fila;

    while (1) {
        // TODO 5: Trave o mutex antes de acessar o buffer.
        pthread_mutex_lock(&f->mutex);

        // TODO 6: Enquanto o buffer estiver vazio E a producao nao
        //         tiver encerrado, espere com
        //         pthread_cond_wait(&f->nao_vazio, &f->mutex).
        //         Use while, nao if!
        while (f->quantidade == 0 && !f->producao_encerrada)
            pthread_cond_wait(&f->nao_vazio, &f->mutex);

        /* condicao de saida: buffer vazio e producao encerrada */
        if (f->quantidade == 0 && f->producao_encerrada) {
            pthread_mutex_unlock(&f->mutex);
            break;
        }

        pedido_t p = f->dados[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_BUFFER;
        f->quantidade--;
        f->total_consumidos++;

        printf("[consumidor %d] pedido #%d (R$%d) processado [buffer: %d/%d]\n",
               a->id, p.id, p.valor, f->quantidade, TAM_BUFFER);

        // TODO 7: Sinalize que o buffer nao esta mais cheio
        //         (pthread_cond_signal) e destrave o mutex.
        pthread_cond_signal(&f->nao_cheio);
        pthread_mutex_unlock(&f->mutex);

        usleep(rand() % 80000);
    }

    return NULL;
}

int main(void) {
    srand(42);

    fila_t fila = {
        .inicio = 0, .fim = 0, .quantidade = 0,
        .total_produzidos = 0, .total_consumidos = 0,
        .producao_encerrada = 0
    };

    // TODO 1: Inicialize o mutex e as duas variaveis de condicao.
    //         pthread_mutex_init(&fila.mutex, NULL);
    //         pthread_cond_init(&fila.nao_cheio, NULL);
    //         pthread_cond_init(&fila.nao_vazio, NULL);
    pthread_mutex_init(&fila.mutex, NULL);
    pthread_cond_init(&fila.nao_cheio, NULL);
    pthread_cond_init(&fila.nao_vazio, NULL);

    int total_esperado = NUM_PRODUTORES * PEDIDOS_POR_PRODUTOR;
    printf("=== Sistema de Pedidos ===\n");
    printf("Buffer: %d posicoes | Produtores: %d | Consumidores: %d\n",
           TAM_BUFFER, NUM_PRODUTORES, NUM_CONSUMIDORES);
    printf("Pedidos por produtor: %d | Total esperado: %d\n\n",
           PEDIDOS_POR_PRODUTOR, total_esperado);

    pthread_t prod[NUM_PRODUTORES], cons[NUM_CONSUMIDORES];
    thread_arg_t args_prod[NUM_PRODUTORES], args_cons[NUM_CONSUMIDORES];

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        args_prod[i] = (thread_arg_t){ .id = i, .fila = &fila };
        pthread_create(&prod[i], NULL, produtor, &args_prod[i]);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        args_cons[i] = (thread_arg_t){ .id = i, .fila = &fila };
        pthread_create(&cons[i], NULL, consumidor, &args_cons[i]);
    }

    /* espera produtores terminarem */
    for (int i = 0; i < NUM_PRODUTORES; i++)
        pthread_join(prod[i], NULL);

    /* sinaliza fim de producao e acorda consumidores que estejam dormindo */
    pthread_mutex_lock(&fila.mutex);
    fila.producao_encerrada = 1;
    pthread_cond_broadcast(&fila.nao_vazio);
    pthread_mutex_unlock(&fila.mutex);

    for (int i = 0; i < NUM_CONSUMIDORES; i++)
        pthread_join(cons[i], NULL);

    printf("\n=== Resultado ===\n");
    printf("Produzidos: %d | Consumidos: %d | Perdidos: %d\n",
           fila.total_produzidos, fila.total_consumidos,
           fila.total_produzidos - fila.total_consumidos);

    // TODO 8: Destrua o mutex e as variaveis de condicao.
    //         pthread_mutex_destroy, pthread_cond_destroy.
    pthread_mutex_destroy(&fila.mutex);
    pthread_cond_destroy(&fila.nao_cheio);
    pthread_cond_destroy(&fila.nao_vazio);

    return 0;
}
