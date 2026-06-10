# Mackenzie

![Mackenzie Preview Image](https://i.ibb.co/0jx4B3qx/Gemini-Generated-Image-pn6qg8pn6qg8pn6q.png)

Repositório com todos os projetos, laboratórios e trabalhos desenvolvidos ao longo do curso de **Ciência da Computação na Universidade Presbiteriana Mackenzie**. Todo o código foi escrito por mim.

---

## Disciplinas

### Sistemas Operacionais

Projetos focados em programação de baixo nível no Linux: gerenciamento de processos, threads POSIX, sincronização e memória virtual.

| Projeto | Descrição |
|---------|-----------|
| **Lab 04 — Threads e Sincronização** | 5 exercícios com `pthreads`: soma paralela de vetores, demonstração de race condition, proteção com mutex, produtor-consumidor com condition variables e leitores-escritores com semáforos |
| **Lab 05 — Quebra-Senhas Paralelo** | Ataque de força bruta MD5 paralelizado via `fork()` + `execl()`. Um coordinator divide o espaço de busca entre workers; o primeiro que encontra a senha grava o resultado com escrita atômica (`O_CREAT | O_EXCL`) e o coordinator encerra os demais com `SIGTERM` |
| **Projeto 2 — Simulador de Paginação** | Simulador de gerenciamento de memória virtual em C que processa sequências de acessos de múltiplos processos. Implementa dois algoritmos de substituição de páginas: **FIFO** (campo `ordem_entrada` por busca linear) e **Clock** (ponteiro circular com R-bit e segunda chance). Conta page faults e suporta configuração externa de frames e processos via arquivos |

**Tecnologias:** C · pthreads · fork/exec/wait · sinais UNIX · Makefile

---

### Computação Paralela

Exploração de otimizações de performance em C: hierarquia de memória, cache misses e paralelismo com Pthreads. Resultados medidos com `valgrind --tool=cachegrind`.

| Projeto | Descrição |
|---------|-----------|
| **Hierarquia de Memória e Pthreads** | Implementação e análise de multiplicação de matrizes (N=512) em 4 versões: row-order, column-order, tiling/blocagem e Pthreads + blocagem. A combinação de 4 threads com blocagem atingiu **354× de speedup** em relação à versão sequencial sem otimização |
| **IoT — Sensor Analyzer** | Analisador de logs de sensores IoT em 3 versões: sequencial, paralela (pthreads) e otimizada. Gerador de logs em Python para testes de carga |

**Tecnologias:** C · pthreads · Valgrind/Cachegrind · Python · Makefile

---

### Engenharia de Software

Modelagem completa de um sistema de chamados, cobrindo todo o ciclo do processo de software: análise de negócio, requisitos, análise e projeto.

| Artefato | Descrição |
|----------|-----------|
| **Diagrama de Classes de Domínio** | Entidades `Cliente`, `Chamado`, `Atendente`, `Supervisor` e `Categoria` com seus relacionamentos |
| **Diagrama de Atividades** | Fluxo completo do chamado com raias por responsável (Cliente → Atendente → Supervisor) |
| **Ciclo de Vida do Chamado** | Diagrama de estados: Aberto → Em Atendimento → Escalado → Aguardando Cliente → Resolvido → Fechado |
| **Diagrama de Casos de Uso** | 9 casos de uso mapeados para os 3 atores do sistema |

**Tecnologias:** PlantUML · UML 2.x

---

### Projeto de Software

Série de laboratórios de programação em C e Java, do básico ao avançado, com exercícios progressivos sobre estruturas de dados, ponteiros, memória dinâmica, algoritmos e orientação a objetos.

| Labs | Conteúdo |
|------|----------|
| LAB 2 – 6 | Fundamentos de C: ponteiros, arrays, strings, structs e manipulação de memória |
| LAB 8 – 9 | Estruturas de dados: listas, filas e pilhas |
| LAB 10 – 11 | Algoritmos de ordenação e busca |
| **Projeto Final** | Implementação completa de sistema em C integrando os conceitos dos labs anteriores |
| **Simulador de Estoque** | Sistema de gerenciamento de estoque em Java 17 com POO. Funcionalidades: cadastro, atualização, consulta e remoção de produtos com armazenamento em memória. Modelagem completa com diagramas de classes, atividades, estados e sequência em PlantUML |

**Tecnologias:** C · GCC · Makefile · Java 17 · PlantUML

---

### Linguagens e Autômatos

Modelagem e análise de sistemas com foco em processos, arquitetura de software e garantia de qualidade.

| Projeto | Descrição |
|---------|-----------|
| **Garantia de Qualidade — Squad Dealers** | Sistema centralizado de monitoramento para uma empresa de jogos digitais que opera em múltiplos projetos simultâneos (PC, consoles, mobile). Permite registrar e acompanhar bugs com prioridade e responsável, monitorar progresso de atividades (planejado × executado) e gerar métricas gerenciais comparativas entre projetos. Arquitetura em camadas: SPA → BFF → REST API → MariaDB |

**Tecnologias:** PlantUML · UML 2.x · REST API · MariaDB · SPA · BFF

---

## Estrutura do Repositório

```
Mackenzie/
├── Sistemas Operacionais/
│   ├── LAB4/                  # Threads e Sincronização (pthreads)
│   ├── Lab5/                  # Quebra-Senhas Paralelo (fork/exec/signals)
│   └── Projeto_2_pagingsim/   # Simulador de Paginação FIFO + Clock
├── Computacao Paralela/
│   ├── Hierarquia de Memória e Pthreads/   # Matmul + cachegrind
│   └── IOT/                   # Sensor analyzer seq/par/otimizado
├── EngenhariaDeSoftware/
│   └── Sistema de Chamados/   # Modelagem UML completa
├── Projeto de Software/
│   ├── LAB2 … LAB11/          # Exercícios progressivos em C
│   ├── Projeto Final/         # Sistema completo em C
│   └── Simulador de Estoque/  # Gerenciamento de estoque em Java 17
└── Linguagens e Automatos/
    └── garantia-de-qualidade-Dealers/  # Sistema de QA para jogos digitais
```

---

## Stack Principal

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Java](https://img.shields.io/badge/Java-ED8B00?style=flat&logo=openjdk&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![GCC](https://img.shields.io/badge/GCC-A42E2B?style=flat&logo=gnu&logoColor=white)
