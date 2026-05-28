#ifndef MEMORIA_H
#define MEMORIA_H

typedef struct {
    int presente;
    int frame;
    int referencia;
} EntradaPagina;

typedef struct {
    int pid;
    int tamanho_memoria;
    int num_paginas;
    EntradaPagina *paginas;
} Processo;

typedef struct {
    int livre;
    int pid;
    int pagina;
    int referencia;
    int ordem_entrada;
} Frame;

typedef struct {
    Processo *processos;
    Frame *frames;

    int num_frames;
    int tamanho_pagina;
    int num_processos;

    int total_acessos;
    int total_page_faults;

    int contador_fifo;
    int ponteiro_clock;
} Simulador;

void inicializar_simulador(Simulador *sim);

void carregar_configuracao(
    Simulador *sim,
    const char *arquivo_config
);

void executar_acessos(
    Simulador *sim,
    const char *arquivo_acessos,
    const char *algoritmo
);

void liberar_simulador(Simulador *sim);

Processo *buscar_processo(
    Simulador *sim,
    int pid
);

int buscar_frame_livre(
    Simulador *sim
);

void carregar_pagina(
    Simulador *sim,
    Processo *proc,
    int pagina,
    int frame
);

void desalocar_pagina(
    Simulador *sim,
    int frame
);

#endif