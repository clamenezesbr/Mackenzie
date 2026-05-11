```plantuml
@startuml UC001_Cadastrar_Perfil
actor Usuario
boundary "App" as app
control "ControladorCadastro" as cadastro
entity "Perfil" as perfil

Usuario -> app : acessarTelaCadastro()
app -> cadastro : iniciarCadastro()
cadastro -> Usuario : solicitarDados()
Usuario -> cadastro : enviarDados()
cadastro -> perfil : criarPerfil()
perfil --> cadastro : confirmarCriacao()
cadastro -> app : redirecionarParaTelaInicial()
@enduml


@startuml UC002_Publicar_Conteudo
actor Usuario
boundary "App" as app
control "ControladorPublicacao" as ctrlPub
entity "Publicacao" as pub

Usuario -> app : acessarCriarPublicacao()
app -> ctrlPub : iniciarPublicacao()
ctrlPub -> Usuario : solicitarDadosPublicacao()
Usuario -> ctrlPub : enviarTextoEMidia()
ctrlPub -> pub : salvarPublicacao()
pub --> ctrlPub : confirmacao
ctrlPub -> app : atualizarFeed()
@enduml


@startuml UC003_Buscar_Eventos
actor Usuario
boundary "App" as app
control "ControladorEvento" as ctrlEvt
entity "Evento" as evento

Usuario -> app : acessarEventos()
app -> ctrlEvt : solicitarEventos()
ctrlEvt -> evento : buscarEventosPorInteresse()
evento --> ctrlEvt : listaEventos
ctrlEvt -> app : exibirEventos()

Usuario -> app : selecionarEvento()
app -> ctrlEvt : detalharEvento()
ctrlEvt -> evento : obterDetalhes()
evento --> ctrlEvt : dadosEvento
ctrlEvt -> app : exibirDetalhesEvento()

Usuario -> app : inscreverNoEvento()
app -> ctrlEvt : registrarInscricao()
ctrlEvt -> evento : salvarInscricao()
evento --> ctrlEvt : confirmacao
ctrlEvt -> app : mostrarConfirmacao()
@enduml


@startuml UC004_Participar_Grupo
actor Usuario
boundary "App" as app
control "ControladorGrupo" as ctrlGrupo
entity "Grupo" as grupo
entity "Participacao" as participacao

Usuario -> app : acessarGrupos()
app -> ctrlGrupo : solicitarGrupos()
ctrlGrupo -> grupo : listarPorInteresse()
grupo --> ctrlGrupo : listaGrupos
ctrlGrupo -> app : mostrarGrupos()

Usuario -> app : entrarNoGrupo()
app -> ctrlGrupo : registrarParticipacao()
ctrlGrupo -> participacao : salvarParticipacao()
participacao --> ctrlGrupo : confirmacao
ctrlGrupo -> app : mostrarTelaDoGrupo()
@enduml


@startuml UC005_Avaliar_Recomendacao
actor Usuario
boundary "App" as app
control "ControladorRecomendacao" as ctrlRec
entity "Recomendacao" as rec
entity "Avaliacao" as aval

Usuario -> app : verRecomendacao()
app -> ctrlRec : obterRecomendacao()
ctrlRec -> rec : buscarPorPerfil()
rec --> ctrlRec : listaRecomendacoes
ctrlRec -> app : mostrarRecomendacoes()

Usuario -> app : avaliarRecomendacao()
app -> ctrlRec : registrarAvaliacao()
ctrlRec -> aval : salvarAvaliacao()
aval --> ctrlRec : sucesso
ctrlRec -> app : confirmarFeedback()
@enduml


@startuml UC006_Moderar_Conteudo
actor Administrador
boundary "PainelAdmin" as painel
control "ControladorModeracao" as ctrlMod
entity "Publicacao" as pub
entity "Moderacao" as mod

Administrador -> painel : acessarModeracao()
painel -> ctrlMod : listarPublicacoesPendentes()
ctrlMod -> pub : buscarPendentes()
pub --> ctrlMod : listaPublicacoes
ctrlMod -> painel : exibirLista()

Administrador -> painel : revisarPublicacao()
Administrador -> painel : aprovarOuRemover()
painel -> ctrlMod : aplicarModeracao()
ctrlMod -> mod : registrarAcao()
mod --> ctrlMod : confirmacao
ctrlMod -> painel : atualizarStatus()
@enduml


@startuml UC007_Gerar_Recomendacoes
actor "Sistema de Recomendação" as sistema
control "MotorDeRecomendacao" as motor
entity "Perfil" as perfil
entity "Recomendacao" as recomendacao

sistema -> motor : iniciarRecomendacao()
motor -> perfil : obterDadosUsuario()
perfil --> motor : dadosPerfil
motor -> recomendacao : gerarCursosEventosGrupos()
recomendacao --> motor : listaGerada
motor -> sistema : enviarRecomendacoes()
@enduml
