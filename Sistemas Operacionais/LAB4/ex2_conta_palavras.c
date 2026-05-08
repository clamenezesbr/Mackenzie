#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * Ex 2 - Contagem de caracteres SEM protecao
 *
 * N threads contam ocorrencias de um caractere em fatias de texto.
 * Cada thread incrementa um contador global sem mutex.
 * O resultado diverge da contagem sequencial - race condition.
 *
 * Objetivo: observar que o acesso concorrente a uma variavel
 * compartilhada sem protecao produz resultados incorretos.
 *
 * Atencao: este exercicio tem um bug PROPOSITAL.
 * Nao tente corrigir - o ex3 fara isso.
 */

#define TAM_TEXTO   500000
#define NUM_THREADS 8
#define CHAR_ALVO   'a'

typedef struct {
    int id;
    const char *texto;
    int inicio;
    int fim;
    long *contador;
} tarefa_t;

void *worker(void *arg) {
    tarefa_t *t = (tarefa_t *)arg;

    // TODO 1: Para cada posicao de texto[inicio..fim), se o caractere
    //         for igual a CHAR_ALVO, incremente *contador (sem protecao).
    //         Isso e proposital - queremos ver o bug.
    for (int i = t->inicio; i < t->fim; i++)
        if (t->texto[i] == CHAR_ALVO)
            (*t->contador)++;

    printf("[thread %d] contou em [%d..%d]\n", t->id, t->inicio, t->fim - 1);
    return NULL;
}

int main(void) {
    /* gera texto com distribuicao conhecida: 1 em cada 10 caracteres e 'a' */
    char *texto = malloc(TAM_TEXTO + 1);
    if (!texto) { perror("malloc"); return 1; }

    for (int i = 0; i < TAM_TEXTO; i++)
        texto[i] = (i % 10 == 0) ? CHAR_ALVO : 'x';
    texto[TAM_TEXTO] = '\0';

    printf("[main] contando '%c' em texto de %d caracteres, %d threads\n",
           CHAR_ALVO, TAM_TEXTO, NUM_THREADS);

    long contador = 0;
    pthread_t threads[NUM_THREADS];
    tarefa_t *tarefas[NUM_THREADS];
    int fatia = TAM_TEXTO / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        // TODO 2: Aloque tarefa_t com malloc(). Preencha os campos:
        //         id, texto, inicio, fim, contador (= &contador).
        tarefas[i] = malloc(sizeof(tarefa_t));
        tarefas[i]->id = i;
        tarefas[i]->texto = texto;
        tarefas[i]->inicio = i * fatia;
        tarefas[i]->fim = (i == NUM_THREADS - 1) ? TAM_TEXTO : (i + 1) * fatia;
        tarefas[i]->contador = &contador;

        // TODO 3: Crie a thread com pthread_create().
        //         Em caso de erro, imprima com perror() e retorne 1.
        if (pthread_create(&threads[i], NULL, worker, tarefas[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // TODO 4: Faca join em todas as threads e libere as structs.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        free(tarefas[i]);
    }

    // TODO 5: Compute a contagem sequencial correta para comparacao.
    //         Percorra o texto inteiro e conte as ocorrencias de CHAR_ALVO.
    long correto = 0;
    for (int i = 0; i < TAM_TEXTO; i++)
        if (texto[i] == CHAR_ALVO)
            correto++;

    printf("[main] contador global (sem mutex): %ld\n", contador);
    printf("[main] contagem sequencial correta: %ld\n", correto);
    printf("[main] diferenca: %ld%s\n",
           correto - contador,
           (correto != contador) ? " (race condition!)" : " -- OK");

    // TODO 6: Libere o texto alocado.
    free(texto);

    return 0;
}
