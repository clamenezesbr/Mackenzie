# Projeto de Garantia de Qualidade (QA)

## Documento de Definição de Escopo e Problema

### 1. Definição do Problema

Uma empresa de desenvolvimento de jogos digitais trabalha simultaneamente em diversos projetos para múltiplas plataformas (**PC, consoles e dispositivos móveis**). Atualmente, cada equipe organiza seu fluxo de trabalho de maneira autônoma, resultando em:

* **Falta de Padronização:** Ferramentas e métodos heterogêneos para registro de tarefas e testes.
* **Fragmentação da Informação:** Dados sobre bugs, tempo de correção e progresso são registrados de forma incompleta ou inconsistente.
* **Dificuldade Gerencial:** Impossibilidade de comparar o desempenho entre projetos ou identificar atrasos críticos com antecedência.
* **Perda de Conhecimento:** Dificuldade em aproveitar aprendizados de projetos passados para otimizar processos futuros.

### 2. Causas Raízes

As falhas identificadas originam-se principalmente de dois fatores:

1. **Processos Descentralizados:** A ausência de um framework padrão para planejamento, desenvolvimento e teste gera disparidade nos registros.
2. **Ecossistema de Ferramentas Isoladas:** O uso de múltiplos softwares não integrados para controle de versão, bugs e tarefas espalha os dados em silos de informação.

### 3. Stakeholders (Partes Interessadas)

| Grupo | Impacto da Situação Atual |
| --- | --- |
| **Equipe de QA** | Dificuldade em priorizar correções e monitorar a qualidade global. |
| **Devs e Designers** | Falhas de comunicação entre programação, arte e design por falta de processos. |
| **Gestão de Projetos** | Visibilidade limitada sobre o cumprimento de prazos e progresso real. |
| **Marketing e Pub.** | Dependência de dados imprecisos para planejar datas de lançamento. |

### 4. Fronteira Sistêmica (A Solução Proposta)

Propõe-se o desenvolvimento de um **sistema interno de monitoramento centralizado**.

* **Escopo:** Registrar etapas concluídas, métricas de bugs, progresso de atividades e prazos.
* **Objetivo:** Apoiar o controle de qualidade e a gestão estratégica através de relatórios e comparação de performance.
* **Interoperabilidade:** O sistema **não substitui** as ferramentas atuais das equipes; ele atua como uma camada complementar de inteligência e centralização de dados.

### 5. Restrições e Limitações

#### Técnicas

* **Infraestrutura:** Deve ser compatível com os servidores e redes existentes na empresa.
* **Integração:** Obrigatoriedade de conexão via API ou plugins com sistemas de controle de versão e gerenciamento de tarefas já em uso.

#### Legais e Segurança

* **LGPD / Proteção de Dados:** Conformidade com normas legais de privacidade.
* **Propriedade Intelectual:** Controle rigoroso de acesso (RBAC) para evitar vazamentos de projetos confidenciais (jogos não anunciados).