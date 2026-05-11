# Diagrama de Transição de Estado

```plantuml
@startuml

' ==== Estados Principais ====
[*] --> Inativo : Sistema iniciado
Inativo --> Cadastro : Usuário acessa cadastro
Cadastro --> Ativo : Cadastro concluído com sucesso
Ativo --> Publicando : Usuário cria publicação
Ativo --> BuscandoEventos : Usuário busca eventos
Ativo --> ParticipandoGrupo : Usuário entra em grupo
Ativo --> Avaliando : Usuário avalia recomendação
Ativo --> Moderando : Admin acessa moderação
Ativo --> GerandoRecomendacoes : Sistema gera recomendações

' ==== Sub-estados ====

state Publicando {
  [*] --> Editando
  Editando --> Publicado : Confirma
  Editando --> [*] : Cancela
  Publicado --> Moderacao : Flag de conteúdo
  Moderacao --> Aprovado : Admin aprova
  Moderacao --> Rejeitado : Admin rejeita
}

state BuscandoEventos {
  [*] --> Listando
  Listando --> Visualizando : Seleciona evento
  Visualizando --> Inscrito : Confirma inscrição
  Visualizando --> [*] : Volta
}

state ParticipandoGrupo {
  [*] --> Explorando
  Explorando --> Participando : Entra no grupo
  Participando --> [*] : Sai do grupo
}

state Avaliando {
  [*] --> VisualizandoRec
  VisualizandoRec --> Avaliado : Envia feedback
  Avaliado --> [*] : Fecha
}

state Moderando {
  [*] --> Revisando
  Revisando --> Aprovando : Conteúdo OK
  Revisando --> Removendo : Conteúdo inapropriado
  Aprovando --> [*] : Finaliza
  Removendo --> [*] : Finaliza
}

state GerandoRecomendacoes {
  [*] --> ColetandoDados
  ColetandoDados --> Processando : Dados suficientes
  Processando --> Disponibilizando : Gera recomendações
  Disponibilizando --> [*] : Completo
}

' ==== Transições de Erro ====
Cadastro --> [*] : Falha no cadastro
Publicando --> [*] : Erro ao publicar
BuscandoEventos --> [*] : Sem eventos disponíveis
ParticipandoGrupo --> [*] : Erro ao entrar
Avaliando --> [*] : Erro na avaliação
Moderando --> [*] : Erro na moderação
GerandoRecomendacoes --> [*] : Sem dados suficientes

' ==== Transições de Sistema ====
Ativo --> Inativo : Logout/Inatividade
Ativo --> [*] : Usuário desativa conta

@enduml