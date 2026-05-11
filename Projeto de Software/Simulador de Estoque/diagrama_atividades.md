```plantuml
@startuml

title Diagrama de Atividades - Rede Social para Profissionalização de Estudantes

' ==== Raias (Swimlanes) ====
|Usuário|
|Sistema|
|Administrador|

' ==== Fluxo Principal ====
start

|Usuário|
:Usuário acessa plataforma;
if (Novo usuário?) then (Sim)
  :Cadastrar perfil;
  :Preencher dados pessoais;
  :Preencher dados acadêmicos;
  :Preencher interesses profissionais;
else (Não)
  :Fazer login;
endif

|Sistema|
fork
  |Usuário|
  :Publicar conteúdo;
  fork
    :Criar postagem;
    :Adicionar mídia;
  fork again
    :Vincular a curso/evento;
  end fork
  
  |Sistema|
  :Moderação automática;
  
fork again
  |Usuário|
  :Buscar eventos;
  :Filtrar por área de interesse;
  :Visualizar detalhes;
  if (Participar?) then (Sim)
    :Confirmar inscrição;
  else (Não)
    :Voltar para lista;
  endif
  
fork again
  |Usuário|
  :Participar de grupos;
  :Explorar grupos sugeridos;
  :Ler regras do grupo;
  :Entrar no grupo;
  
fork again
  |Usuário|
  :Avaliar recomendações;
  :Visualizar sugestões;
  if (Relevante?) then (Sim)
    :Marcar como útil;
  else (Não)
    :Marcar como irrelevante;
  endif
end fork

' ==== Fluxos Paralelos ====
fork
  |Sistema|
  :Gerar recomendações;
  :Analisar perfil;
  :Cruzamento com banco de dados;
  :Priorizar por relevância;
  
fork again
  |Administrador|
  :Moderar conteúdo;
  :Revisar denúncias;
  if (Conteúdo inadequado?) then (Sim)
    :Remover publicação;
    :Notificar usuário;
  else (Não)
    :Manter publicação;
  endif
end fork

|Sistema|
:Atualizar feed principal;
stop

@enduml