# Exercício 001

## Introdução

Neste exercício você deverá construir um diagrama de classes considerando o contexto discutido em aula que contempla a existência de:

- Curso
- Projeto pedagógico de curso, 
- diretriz curricular nacional, 
- MEC, 
- Instituição de Ensino Superior, 
- Perfil de Ingressante, 
- Perfil de Egresso, 
- componentes curriculares (disciplinas), 
- Ementa, 
- Competências,
- Professor
- Aluno
- Unidade Acadêmcia (Faculdade)
- Avaliação
- Turma (Oferecimento)
- Aula
- Calendário


## Instruções

No ramo (branch) de sua equipe, na pasta Exercícios, crie um arquivo com o nome Ex001-\<Numero do seu RA>.md. 
No inicio de seu arquivo, copie e cole o seguinte conteúdo:




```
# Exercício 001 - Construção de diagrama de classes

Aluno: Seu nome
RA: Seu RA


Neste exercício construiremos um diagrama de classes utilizando o PlantUML.


## Diagrama

Desenhe aqui o seu diagrama.

```

Em seguida, clique em Commit changes. Desta forma você irá salvar e realizar o commit do arquivo no git.

Estude o exemplo dado para construir o diagrama de classes solicitado neste exercício. Para visualizar "o código fonte" do diagrama, clique no botão "Display source" (</>) que fica próximo ao botão "blame".

Ao editar o seu arquivo, você sempre poderá clicar em Preview para que o gitlab renderize seu diagrama.

Com o seu diagrama finalizado, coloque o diagrama em um arquivo pdf informando seu nome, RA, seu grupo e poste na tarefa indicada no Moodle.

ATENÇÃO: Selecione apenas 5 classes para a construção de seu diagrama, classes que possuam associações entre elas. Identifique as associações e estabeleça as multiplicidades necessárias.


## Diagrama de classes exemplo

```plantuml
@startuml 

hide circle
left to right direction

'skinparam classAttributeIconSize 0
'skinparam classFontStyle bold
'skinparam classFontSize 14
'skinparam classBackgroundColor LightBlue
'skinparam classStereotypeFontSize 12

class Pessoa {
  +id: String
  +nome: String
  +email: String
  +telefone: String
  +dataCadastro: Date
}
class Usuario {
  +login: String
  +senha: String
}
class Civil {
  +cpf: String
}
class PrestadorServico {
  +cnpj: String
  +areaAtuacao: String
  +status: StatusPrestador
}
class Ocorrencia {
  +id: String
  +dataCriacao: Date
  +descricao: String
  +status: StatusOcorrencia
  +prioridade: NivelPrioridade
}
class OrdemServico {
  +id: String
  +dataCriacao: Date
  +dataAtribuicao: Date
  +dataFinalizacao: Date
  +status: StatusOrdemServico
}
class Anexo {
  +id: String
  +tipo: TipoAnexo
  +nomeArquivo: String
  +dataUpload: Date
}
class Localizacao {
  +latitude: Double
  +longitude: Double
  +endereco: String
  +bairro: String
  +cidade: String
  +cep: String
}
class Notificacao {
  +id: String
  +titulo: String
  +mensagem: String
  +dataEnvio: Date
  +lida: Boolean
}
class Avaliacao {
  +id: String
  +nota: Integer
  +comentario: String
  +dataCriacao: Date
}
class Monitor {
  +regiaoResponsavel: String
}
class Supervisor {
  +regiaoSupervisao: String
}

enum StatusOcorrencia {
  REGISTRO
  PENDENTE_DE_ANALISE
  DADOS_VALIDADOS
  PRONTA_PARA_ATRIBUICAO
  ATRIBUIDA
  RECUSADA
  EM_ANDAMENTO
  CONCLUIDA
  AVALIADA
  CANCELADA
}

enum StatusOrdemServico {
  ABERTA
  ATRIBUIDA
  ACEITA
  EM_ANDAMENTO
  FINALIZADA
  RECUSADA
  CANCELADA
}

enum StatusPrestador {
  ATIVO
  INATIVO
  SUSPENSO
  EM_SERVICO
}

enum NivelPrioridade {
  BAIXA
  MEDIA
  ALTA
  URGENTE
}

enum TipoAnexo {
  FOTO
  DOCUMENTO
  VIDEO
  AUDIO
}

Pessoa "1" --> "1" Usuario : possui
Pessoa <|-- Civil
Pessoa <|-- PrestadorServico
Pessoa <|-- Monitor
Pessoa <|-- Supervisor

Civil "1" --> "0..*" Ocorrencia : cria
Civil "1" --> "0..*" Avaliacao : realiza

Ocorrencia "1" --> "1" Localizacao : localizada em
Ocorrencia "1" --> "0..*" Anexo : contém
Ocorrencia "1" --> "0..1" OrdemServico : gera

OrdemServico "0..*" --> "1" PrestadorServico : atribuída a
OrdemServico "1" --> "0..1" Avaliacao : avaliada por

Pessoa "1" --> "0..*" Notificacao : recebe
PrestadorServico "1" --> "0..*" Notificacao : envia

' Relacionamentos de Monitor e Supervisor
Monitor "1" --> "0..*" Ocorrencia : monitora
Supervisor "1" --> "0..*" OrdemServico : supervisiona

Ocorrencia .. StatusOcorrencia
OrdemServico .. StatusOrdemServico
PrestadorServico .. StatusPrestador
Ocorrencia .. NivelPrioridade
Anexo .. TipoAnexo

@enduml
```
