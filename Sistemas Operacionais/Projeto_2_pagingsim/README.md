# Simulador de Gerenciamento de Memória Virtual
```
Projeto 2
```

Simulador de paginação em C que processa sequências de acessos à memória de múltiplos processos e aplica algoritmos de substituição de páginas (FIFO e Clock) quando ocorre page fault com memória física cheia.

**Disciplina:** Sistemas Operacionais  
**Professor:** Lucas Figueiredo  
**Integrantes:** 
Gabriel Resende Menezes (10419003)
Arthur Danta Gonzales Felix (10419721)Isabele Dal Pogetto Costa (10721328)

---

## 1 — Estrutura do Código (`src/`)

**O que o professor pediu:** implementar o simulador em C com autonomia sobre a organização dos arquivos.

**Como foi resolvido:** o código foi dividido em três módulos:

```
src/
├── main.c         # lê argumentos, valida algoritmo e coordena a execução
├── memoria.c/.h   # inicialização, configuração, execução dos acessos e tratamento de page fault
└── algoritmos.c/.h # escolher_vitima_fifo() e escolher_vitima_clock()
```

`Simulador` é a estrutura central: guarda um array fixo de frames físicos, um array de processos (cada um com sua própria tabela de páginas) e o ponteiro circular usado pelo Clock.

---

## 2 — Algoritmo FIFO (`escolher_vitima_fifo`)

**O que o professor pediu:** substituir a página que está na memória há mais tempo.

**Como foi resolvido:** cada frame possui um campo `ordem_entrada` que recebe um valor crescente toda vez que uma página é carregada. Para escolher a vítima, o simulador percorre todos os frames ocupados e seleciona o de menor `ordem_entrada`. Não foi necessária uma fila encadeada — o array de frames já é suficiente para essa busca linear.

---

## 3 — Algoritmo Clock (`escolher_vitima_clock`)

**O que o professor pediu:** implementar o algoritmo de segunda chance com ponteiro circular e R-bit.

**Como foi resolvido:** um campo `ponteiro_clock` na estrutura do simulador avança circularmente pelos frames usando `% num_frames`. Cada frame e cada entrada da tabela de páginas possuem um campo `referencia` (R-bit). A lógica de substituição:

- Se `referencia == 0` → frame é escolhido como vítima.
- Se `referencia == 1` → bit é zerado, página recebe segunda chance, ponteiro avança.

Se todos os frames tiverem R=1, o algoritmo faz uma volta completa zerando todos os bits e encontra a vítima na passagem seguinte. O R-bit é atualizado para 1 em todo acesso à memória, incluindo HITs.

---

## 4 — Tratamento de Page Fault (`tratar_page_fault`)

**O que o professor pediu:** carregar a página ausente na memória, escolhendo uma vítima quando necessário.

**Como foi resolvido:** ao detectar que `presente == 0` na tabela de páginas, o simulador tenta primeiro encontrar um frame livre (campo `ocupado == 0`). Se não houver frame livre, chama o algoritmo selecionado (`fifo` ou `clock`) para escolher a vítima: a entrada da tabela de páginas da vítima é desmarcada (`presente = 0`), o frame é reatribuído ao processo atual e a nova página é registrada.

---

## Compilação e Execução

```bash
cd src/
gcc *.c -o simulador
```

**FIFO:**
```bash
./simulador fifo ../tests/config_1.txt ../tests/acessos_1.txt
```

**Clock:**
```bash
./simulador clock ../tests/config_1.txt ../tests/acessos_1.txt
```
