```plantuml
@startuml

' ==== Classes ====
class Usuario {
  +id: UUID
  +nome: String
  +email: String
  +senha: String
}

class Perfil {
  +id: UUID
  +dadosAcademicos: String
  +interesses: String
  +dadosProfissionais: String
}

class Publicacao {
  +id: UUID
  +texto: String
  +dataCriacao: Date
  +midia: String
}

class Curso {
  +id: UUID
  +nome: String
  +descricao: String
}

class Evento {
  +id: UUID
  +nome: String
  +descricao: String
  +data: Date
}

class InscricaoEvento {
  +id: UUID
  +dataInscricao: Date
}

class Grupo {
  +id: UUID
  +nome: String
  +descricao: String
}

class ParticipacaoGrupo {
  +id: UUID
  +dataEntrada: Date
}

class Recomendacao {
  +id: UUID
  +tipo: String
  +descricao: String
}

class AvaliacaoRecomendacao {
  +id: UUID
  +avaliacao: String
}

class Administrador {
  +id: UUID
  +nome: String
  +email: String
}

class Moderacao {
  +id: UUID
  +status: String
  +dataModeracao: Date
}

' ==== Relacionamentos ====
Usuario "1" -- "1" Perfil : possui
Usuario "1" -- "0..*" Publicacao : cria
Usuario "1" -- "0..*" InscricaoEvento : se inscreve
Usuario "1" -- "0..*" ParticipacaoGrupo : participa
Usuario "1" -- "0..*" AvaliacaoRecomendacao : avalia

Publicacao "0..*" --> "0..1" Curso : referencia
InscricaoEvento "0..*" --> "1" Evento : refere-se a
ParticipacaoGrupo "0..*" --> "1" Grupo : refere-se a
Recomendacao "0..*" --> "1" Perfil : baseada em
AvaliacaoRecomendacao "0..*" --> "1" Recomendacao : refere-se a

Administrador "1" -- "0..*" Moderacao : realiza
Moderacao "0..*" --> "1" Publicacao : modera

@enduml
