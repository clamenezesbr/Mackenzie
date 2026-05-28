#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memoria.h"
#include "algoritmos.h"

// Inicializa o simulador com valores padrão antes da leitura dos arquivos
void inicializar_simulador(Simulador *sim) {
    sim->processos = NULL;
    sim->frames = NULL;

    sim->num_frames = 0; 
    sim->tamanho_pagina = 0; 
    sim->num_processos = 0;

    sim->total_acessos = 0; 
    sim->total_page_faults = 0;

    sim->contador_fifo = 0;
    sim->ponteiro_clock = 0; 
}

// Procura um processo pelo PID informado no arquivo de acessos
Processo *buscar_processo(Simulador *sim, int pid) { 
    for (int i = 0; i < sim->num_processos; i++) { 
        if (sim->processos[i].pid == pid) { 
            return &sim->processos[i];
        }
    }

    return NULL; 
}

// Percorre os frames físicos procurando um frame livre
int buscar_frame_livre(Simulador *sim) { 
    for (int i = 0; i < sim->num_frames; i++) { 
        if (sim->frames[i].livre) { 
            return i; 
        }
    }
    
    return -1; 
}

// Lê o arquivo de configuração e cria os processos, frames e tabelas de páginas
void carregar_configuracao(Simulador *sim, const char *arquivo_config) {
    FILE *arquivo = fopen(arquivo_config, "r");

    if (arquivo == NULL) { 
        printf("Erro ao abrir arquivo de configuracao\n");
        exit(1);
    }

    fscanf(arquivo, "%d", &sim->num_frames);
    fscanf(arquivo, "%d", &sim->tamanho_pagina);
    fscanf(arquivo, "%d", &sim->num_processos);

    sim->processos =
        malloc(sim->num_processos * sizeof(Processo));

    sim->frames =
        malloc(sim->num_frames * sizeof(Frame));

    for (int i = 0; i < sim->num_frames; i++) {

        sim->frames[i].livre = 1;
        sim->frames[i].pid = -1;
        sim->frames[i].pagina = -1;
        sim->frames[i].referencia = 0;
        sim->frames[i].ordem_entrada = -1;
    }

    for (int i = 0; i < sim->num_processos; i++) {

        int pid;
        int tamanho_memoria;

        fscanf(
            arquivo,
            "%d %d",
            &pid,
            &tamanho_memoria
        );

        sim->processos[i].pid = pid;

        sim->processos[i].tamanho_memoria =
            tamanho_memoria;

        sim->processos[i].num_paginas =
            (tamanho_memoria + sim->tamanho_pagina - 1)
            / sim->tamanho_pagina;

        sim->processos[i].paginas =
            malloc(
                sim->processos[i].num_paginas
                * sizeof(EntradaPagina)
            );

        for (int j = 0; j < sim->processos[i].num_paginas; j++) {

            sim->processos[i].paginas[j].presente = 0;

            sim->processos[i].paginas[j].frame = -1;

            sim->processos[i].paginas[j].referencia = 0;
        }
    }

    fclose(arquivo);
}

// Carrega uma página em um frame físico e atualiza a tabela de páginas
void carregar_pagina( Simulador *sim, Processo *proc, int pagina, int frame) { 
    sim->frames[frame].livre = 0; 
    sim->frames[frame].pid = proc->pid;
    sim->frames[frame].pagina = pagina; 
    sim->frames[frame].referencia = 1; 
    sim->frames[frame].ordem_entrada = sim->contador_fifo++;

    proc->paginas[pagina].presente = 1;
    proc->paginas[pagina].frame = frame; 
    proc->paginas[pagina].referencia = 1; 
}

// Remove uma página da memória física e atualiza sua entrada na tabela de páginas
void desalocar_pagina(Simulador *sim, int frame) { 
    Processo *proc = buscar_processo(sim, sim->frames[frame].pid);

    int pagina = sim->frames[frame].pagina;

    proc->paginas[pagina].presente = 0; 
    proc->paginas[pagina].frame = -1;
    proc->paginas[pagina].referencia = 0;
}

// Função principal da simulação: lê os acessos, calcula página/deslocamento,
// identifica HIT ou PAGE FAULT e aplica FIFO ou Clock quando necessário
void executar_acessos(Simulador *sim, const char *arquivo_acessos, const char *algoritmo) {
    FILE *arquivo = fopen(arquivo_acessos, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo de acessos\n");
        exit(1);
    }

    int pid;
    int endereco;

    while (fscanf(arquivo, "%d %d", &pid, &endereco) == 2) {

        sim->total_acessos++;

        Processo *proc = buscar_processo(sim, pid);

        if (proc == NULL) {
            printf("PID %d não encontrado\n", pid);
            continue;
        }

        // Calcula a página virtual e o deslocamento dentro da página
        int pagina = endereco / sim->tamanho_pagina;
        int deslocamento = endereco % sim->tamanho_pagina;

        if (pagina < 0 || pagina >= proc->num_paginas) {
            printf("Endereço inválido para o processo %d\n", pid);
            continue;
        }

        if (proc->paginas[pagina].presente) {

            int frame = proc->paginas[pagina].frame;

            proc->paginas[pagina].referencia = 1;
            sim->frames[frame].referencia = 1;

            printf(
                "Acesso: PID %d, Endereço %d (Página %d, Deslocamento %d) -> HIT: Página %d (PID %d) já está no Frame %d\n",
                pid,
                endereco,
                pagina,
                deslocamento,
                pagina,
                pid,
                frame
            );
        }
        else {

            sim->total_page_faults++;

            int frame_livre = buscar_frame_livre(sim);

            if (frame_livre != -1) {

                carregar_pagina(sim, proc, pagina, frame_livre);

                printf(
                    "Acesso: PID %d, Endereço %d (Página %d, Deslocamento %d) -> PAGE FAULT -> Página %d (PID %d) alocada no Frame livre %d\n",
                    pid,
                    endereco,
                    pagina,
                    deslocamento,
                    pagina,
                    pid,
                    frame_livre
                );
            }
            else {

                int vitima;

                if (strcmp(algoritmo, "fifo") == 0) {
                    vitima = escolher_vitima_fifo(sim);
                }
                else {
                    vitima = escolher_vitima_clock(sim);
                }

                int pid_antigo = sim->frames[vitima].pid;
                int pagina_antiga = sim->frames[vitima].pagina;

                desalocar_pagina(sim, vitima);

                carregar_pagina(sim, proc, pagina, vitima);

                printf(
                    "Acesso: PID %d, Endereço %d (Página %d, Deslocamento %d) -> PAGE FAULT -> Memória cheia. Página %d (PID %d) (Frame %d) será desalocada. -> Página %d (PID %d) alocada no Frame %d\n",
                    pid,
                    endereco,
                    pagina,
                    deslocamento,
                    pagina_antiga,
                    pid_antigo,
                    vitima,
                    pagina,
                    pid,
                    vitima
                );
            }
        }
    }

    fclose(arquivo);

    printf(
        "--- Simulação Finalizada (Algoritmo: %s)\n",
        algoritmo
    );

    printf(
        "Total de Acessos: %d\n",
        sim->total_acessos
    );

    printf(
        "Total de Page Faults: %d\n",
        sim->total_page_faults
    );
}

// Libera toda a memória alocada dinamicamente ao final da execução
void liberar_simulador(Simulador *sim) {

    for (int i = 0; i < sim->num_processos; i++) {

        free(sim->processos[i].paginas);
    }

    free(sim->processos);

    free(sim->frames);
}
