# Entidades de Domínio - Projeto Garantia de Qualidade (QA)

## Diagrama de Classes Conceitual

```plantuml
@startuml

class Projeto {
  id
  nome
  descricao
  status
  dataInicio
  dataFimPrevista
  dataFimReal
}

class Plataforma {
  id
  nome
  tipo
}

class Equipe {
  id
  nome
  tipo
}

class Usuario {
  id
  nome
  email
  cargo
}

class Atividade {
  id
  titulo
  descricao
  status
  percentualConclusao
}

class Bug {
  id
  titulo
  descricao
  prioridade
  status
  tempoCorrecao
}

class Teste {
  id
  tipo
  status
  dataExecucao
}

class Metrica {
  id
  tipo
  valor
  dataColeta
}

class Relatorio {
  id
  titulo
  tipo
  dataGeracao
}

class Prazo {
  id
  dataLimite
  status
}

class IntegracaoAPI {
  id
  nomeSistema
  tipoIntegracao
  status
}

Projeto "1" -- "*" Plataforma : suporta
Projeto "1" -- "*" Equipe : possui
Projeto "1" -- "*" Atividade : contem
Projeto "1" -- "*" Bug : registra
Projeto "1" -- "*" Teste : executa
Projeto "1" -- "*" Metrica : gera
Projeto "1" -- "*" Relatorio : produz
Projeto "1" -- "1" Prazo : possui
Projeto "1" -- "*" IntegracaoAPI : integra

Equipe "1" -- "*" Usuario : composta por
Usuario "*" -- "*" Atividade : executa
Bug "*" -- "1" Usuario : atribuido a
Teste "*" -- "*" Bug : detecta
Relatorio "*" -- "*" Metrica : consolida

@enduml
```

## Descrição das Entidades

- **Projeto**: representa cada projeto de desenvolvimento de jogos monitorado pelo sistema.
- **Plataforma**: identifica as plataformas suportadas (PC, console, mobile).
- **Equipe**: grupos responsáveis por desenvolvimento, design ou QA.
- **Usuario**: membros das equipes que executam atividades e tratam bugs.
- **Atividade**: tarefas relacionadas ao progresso do projeto.
- **Bug**: falhas registradas durante desenvolvimento e testes.
- **Teste**: validações executadas para garantir qualidade.
- **Metrica**: indicadores coletados, como quantidade de bugs e progresso.
- **Relatorio**: consolidação gerencial das métricas e indicadores.
- **Prazo**: controle de datas e acompanhamento de deadlines.
- **IntegracaoAPI**: integração com ferramentas externas via API.
