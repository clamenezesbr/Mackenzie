# Processos - Projeto Garantia de Qualidade (QA)

## Processo 1: Registro e Tratamento de Bugs

```plantuml
@startuml
start

:Usuário identifica bug;
:Registrar bug no sistema;

if (Bug válido?) then (Sim)
  :Classificar prioridade;
  :Atribuir responsável;
  :Atualizar status para "Aberto";

  while (Bug corrigido?) is (Não)
    :Desenvolvedor analisa problema;
    :Implementa correção;
    :Executar testes de validação;
  endwhile (Sim)

  :Atualizar status para "Resolvido";
  :Registrar tempo de correção;

else (Não)
  :Descartar registro;
endif

stop
@enduml
```

---

## Processo 2: Monitoramento de Progresso e Geração de Relatórios

```plantuml
@startuml
start

:Coletar dados de atividades;
:Coletar dados de bugs;
:Coletar dados de testes;
:Atualizar métricas;

if (Dados completos?) then (Sim)
  :Calcular indicadores de progresso;
  :Comparar planejado vs executado;
  :Gerar relatório gerencial;
  :Disponibilizar dashboard;

else (Não)
  :Solicitar atualização de dados;
endif

stop
@enduml
```

---

## Descrição dos Processos

### Processo 1 - Registro e Tratamento de Bugs
Este processo descreve o fluxo de identificação, registro, classificação, correção e validação de bugs encontrados durante o desenvolvimento dos projetos.

### Processo 2 - Monitoramento de Progresso e Geração de Relatórios
Este processo representa a consolidação de informações dos projetos para geração de métricas, acompanhamento de desempenho e apoio à tomada de decisão gerencial.