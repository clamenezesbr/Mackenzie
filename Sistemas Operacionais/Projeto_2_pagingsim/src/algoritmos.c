#include <stddef.h>
#include "algoritmos.h"

/* FIFO - First In, First Out*/
int escolher_vitima_fifo(Simulador *sim) {
    int vitima       = 0;
    int mais_antigo  = sim->frames[0].ordem_entrada;
 
    for (int i = 1; i < sim->num_frames; i++) {
        if (sim->frames[i].ordem_entrada < mais_antigo) {
            mais_antigo = sim->frames[i].ordem_entrada;
            vitima      = i;
        }
    }
    return vitima;
}
/*
 * Clock (segunda chance)
 * os frames são tratados como um buffer circular apontado
 * por `ponteiro_clock`. Para cada candidato:
 *   - referencia == 1  →  "segunda chance": zera o bit e avança o
 *                         ponteiro (a página fica mais um ciclo).
 *   - referencia == 0  →  página vítima encontrada; o ponteiro avança
 *                         para o frame seguinte antes de retornar, de
 *                         modo que o próximo page fault comece a busca
 *                         a partir daí.
 *
 * O loop garante que, no pior caso (todos os bits == 1 no início),
 * dê uma volta completa zerando tudo e escolha o primeiro frame na
 * segunda passagem.
 */
int escolher_vitima_clock(Simulador *sim) {
    while (1) {
        int i = sim->ponteiro_clock;
        if (sim->frames[i].referencia == 0) {
            /* Vítima encontrada; avança o ponteiro para a próxima posição */
            sim->ponteiro_clock = (i + 1) % sim->num_frames;
            return i;
        }
        /* Segunda chance: zera o R-bit e avança */
        sim->frames[i].referencia = 0;
        /* Sincroniza o R-bit na tabela de páginas do processo dono */
        Processo *proc = buscar_processo(sim, sim->frames[i].pid);
        if (proc != NULL) {
            proc->paginas[sim->frames[i].pagina].referencia = 0;
        }
        sim->ponteiro_clock = (i + 1) % sim->num_frames;
    }
}
 
