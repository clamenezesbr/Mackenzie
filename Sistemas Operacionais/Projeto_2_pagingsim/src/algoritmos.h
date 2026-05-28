#ifndef ALGORITMOS_H
#define ALGORITMOS_H
 
#include "memoria.h"
 
/*escolher_vitima_fifo, percorre todos os frames e retorna o índice do frame cuja página foi carregada há mais tempo (menor ordem_entrada)
 Retorna: índice do frame vítima.
 */
int escolher_vitima_fifo(Simulador *sim);
 
/*
 * escolher_vitima_clock
 *
 * Percorre os frames em ordem circular a partir de ponteiro_clock.
 * - Se o frame tem referencia == 1: zera o bit e avança o ponteiro (segunda chance).
 * - Se o frame tem referencia == 0: este é a vítima.
 *
 * Atualiza sim->ponteiro_clock para o frame seguinte à vítima.
 *
 * Retorna: índice do frame vítima.
 */
int escolher_vitima_clock(Simulador *sim);
 
#endif
 
