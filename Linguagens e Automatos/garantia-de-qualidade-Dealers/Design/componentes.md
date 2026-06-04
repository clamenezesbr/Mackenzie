# Arquitetura - Diagrama de Componentes

**Equipe:**
- Gabriel Rezende Menezes (RA: 10419003)
- Heitor Maciel de Vasconcellos Leite (RA: 10402559)
- Luiz Filipe de Almeida Tannus (RA: 10418230)

Neste documento construímos um diagrama de componentes utilizando o PlantUML, seguindo a arquitetura proposta e as diretrizes de desenvolvimento (SPA, BFF, Controladores e Banco MariaDB).

## Diagrama

```plantuml
@startuml
title Diagrama de Componentes - Sistema de Monitoramento QA

skinparam componentStyle uml2
left to right direction

interface "HTTPS" as API_Externa
interface "HTTP" as API_Interna
interface "TCP/IP (JDBC)" as DB_Conn

component "Fronteira (Cliente)" {
    [SPA Monitoramento QA]
}

component "Gateway (Datacenter)" {
    [BFF (Back For Front)]
}

component "Aplicação e Domínio (Datacenter)" {
    
    package "Controladores" {
        [Controlador de Atividades]
        [Controlador de Métricas]
        [Controlador de Previsão]
    }
    
    package "Domínio / Negócios" {
        [Serviço de Atividades]
        [Serviço de Métricas]
        [Serviço de Progresso]
    }
    
    package "Acesso a Dados" {
        [Repositório de Projetos e QA]
    }
}

database "MariaDB" as Banco

' Relações Externas
[SPA Monitoramento QA] -( API_Externa
API_Externa - [BFF (Back For Front)]

' Relações Internas (Rede)
[BFF (Back For Front)] -( API_Interna
API_Interna -- [Controlador de Atividades]
API_Interna -- [Controlador de Métricas]
API_Interna -- [Controlador de Previsão]

' Relações Internas (Memória/Local)
[Controlador de Atividades] --> [Serviço de Atividades]
[Controlador de Métricas] --> [Serviço de Métricas]
[Controlador de Previsão] --> [Serviço de Progresso]
[Controlador de Previsão] --> [Serviço de Métricas]

[Serviço de Atividades] --> [Repositório de Projetos e QA]
[Serviço de Métricas] --> [Repositório de Projetos e QA]
[Serviço de Progresso] --> [Repositório de Projetos e QA]

' Relações de Banco de Dados
[Repositório de Projetos e QA] -( DB_Conn
DB_Conn - Banco

@enduml