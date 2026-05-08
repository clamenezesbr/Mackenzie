#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * Ex 3 - Contagem de caracteres COM mutex
 *
 * Ponto de partida: copie sua solucao do ex2 para este arquivo.
 * Em seguida, adicione mutex para proteger o acesso ao contador.
 *
 * O diff final entre ex2 e ex3 deve ser pequeno:
 *   + campo mutex na struct
 *   + pthread_mutex_init antes das threads
 *   + pthread_mutex_lock / unlock ao redor do incremento
 *   + pthread_mutex_destroy ao final
 *
 * A saida deve ser identica a do ex2, exceto que agora
 * o contador bate com a contagem sequencial.
 */

#define TAM_TEXTO   500000
#define NUM_THREADS 8
#define CHAR_ALVO   'a'

// TODO 1: Copie a struct tarefa_t do ex2 e adicione um campo:
//         pthread_mutex_t *mutex
typedef struct {
    int id;
    const char *texto;
    int inicio;
    int fim;
    long *contador;
    pthread_mutex_t *mutex;
} tarefa_t;

void *worker(void *arg) {
    // TODO 2: Copie a implementacao do worker do ex2.
    //         Adicione lock/unlock ao redor do incremento do contador.
    tarefa_t *t = (tarefa_t *)arg;
    for (int i = t->inicio; i < t->fim; i++) {
        if (t->texto[i] == CHAR_ALVO) {
            pthread_mutex_lock(t->mutex);
            (*t->contador)++;
            pthread_mutex_unlock(t->mutex);
        }
    }
    printf("[thread %d] contou em [%d..%d]\n", t->id, t->inicio, t->fim - 1);
    (void)arg;
    return NULL;
}

int main(void) {
    char *texto = malloc(TAM_TEXTO + 1);
    if (!texto) { perror("malloc"); return 1; }

    for (int i = 0; i < TAM_TEXTO; i++)
        texto[i] = (i % 10 == 0) ? CHAR_ALVO : 'x';
    texto[TAM_TEXTO] = '\0';

    printf("[main] contando '%c' em texto de %d caracteres, %d threads (COM mutex)\n",
           CHAR_ALVO, TAM_TEXTO, NUM_THREADS);

    long contador = 0;

    // TODO 3: Declare e inicialize um pthread_mutex_t.
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[NUM_THREADS];
    tarefa_t *tarefas[NUM_THREADS];
    int fatia = TAM_TEXTO / NUM_THREADS;

    // TODO 4: Copie o loop de criacao de threads do ex2.
    //         Acrescente o campo mutex na struct de cada thread.
    for (int i = 0; i < NUM_THREADS; i++) {
        tarefas[i] = malloc(sizeof(tarefa_t));
        tarefas[i]->id = i;
        tarefas[i]->texto = texto;
        tarefas[i]->inicio = i * fatia;
        tarefas[i]->fim = (i == NUM_THREADS - 1) ? TAM_TEXTO : (i + 1) * fatia;
        tarefas[i]->contador = &contador;
        tarefas[i]->mutex = &mutex;
        if (pthread_create(&threads[i], NULL, worker, tarefas[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // TODO 5: Copie o loop de join do ex2.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        free(tarefas[i]);
    }

    long correto = 0;
    for (int i = 0; i < TAM_TEXTO; i++)
        if (texto[i] == CHAR_ALVO)
            correto++;

    printf("[main] contador global (com mutex): %ld\n", contador);
    printf("[main] contagem sequencial correta: %ld\n", correto);
    printf("[main] diferenca: %ld -- %s\n",
           correto - contador,
           (correto == contador) ? "OK" : "ERRO");

    // TODO 6: Destrua o mutex com pthread_mutex_destroy().
    pthread_mutex_destroy(&mutex);

    free(texto);
    return 0;
}
