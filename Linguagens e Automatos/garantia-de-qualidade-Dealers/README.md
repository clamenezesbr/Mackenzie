# Garantia de Qualidade — Squad Dealers

Projeto acadêmico desenvolvido para a disciplina de **Linguagens e Autômatos / Engenharia de Software** na Universidade Presbiteriana Mackenzie.

---

## Contexto

Uma empresa de desenvolvimento de jogos digitais opera simultaneamente em múltiplos projetos para **PC, consoles e dispositivos móveis**. Cada equipe atualmente organiza seu fluxo de trabalho de forma autônoma, resultando em falta de padronização, fragmentação de dados e dificuldade gerencial para comparar projetos.

## Solução Proposta

Um **sistema interno de monitoramento centralizado** que atua como camada complementar de inteligência sobre as ferramentas já utilizadas pelas equipes — sem substituí-las. O sistema permite:

- Registrar e acompanhar bugs com prioridade, responsável e tempo de correção
- Monitorar o progresso de atividades (planejado x executado)
- Gerar métricas e relatórios gerenciais comparativos entre projetos
- Integrar com sistemas externos via API

---

## Arquitetura

O sistema segue uma arquitetura em camadas:

```
Cliente (Navegador)
    └── SPA (Single Page Application)
            │ HTTPS
    BFF - Back For Front
            │ HTTP
    Servidor de Aplicação
        ├── Controladores (Atividades, Métricas, Previsão)
        ├── Serviços de Domínio
        └── Repositório de Dados
            │ TCP/IP (JDBC)
    MariaDB
```

Os diagramas completos (pacotes, componentes e implantação) foram modelados com **PlantUML** e estão disponíveis em [`Design/`](./Design/).

---

## Tecnologias

| Camada | Tecnologia |
|---|---|
| Frontend | SPA (Single Page Application) |
| Gateway | BFF (Back For Front) |
| Backend | REST API — Controladores + Serviços de Domínio |
| Banco de Dados | MariaDB |
| Modelagem | PlantUML |
| Comunicação | HTTPS, HTTP, TCP/IP (JDBC) |

---

## Estrutura do Repositório

```
├── Concepção/          # Definição do problema, casos de uso e entidades de domínio
├── Design/             # Diagramas de arquitetura (pacotes e componentes)
├── Implantação/        # Diagrama de implantação (topologia física)
├── Exercícios/         # Exercícios da disciplina
├── GroupInformation.md # Informações do grupo
└── Squad-dealers.md    # Apresentação da squad e competências
```

---

## Processos Principais

**Registro e Tratamento de Bugs** — identificação, classificação por prioridade, atribuição de responsável, ciclo de correção e validação.

**Monitoramento de Progresso** — coleta de dados de atividades, bugs e testes; geração de indicadores e relatórios gerenciais.

Os fluxos completos estão em [`Concepção/processos.md`](./Concepção/processos.md).

---

## Equipe — Squad Dealers

| Nome | RA | Papel |
|---|---|---|
| Luiz Filipe de Almeida Tannus | 10418230 | Representante da Squad |
| Hugo Rafael Weng | 10417866 | Comitê de Projeto |
| Heitor Maciel de Vasconcellos Leite | 10402559 | Comitê de Projeto |
| Gabriel Rezende Menezes | 10419003 | Membro |
