# Arquitetura - Diagrama de Implantação

**Equipe:**
- Gabriel Rezende Menezes (RA: 10419003)
- Heitor Maciel de Vasconcellos Leite (RA: 10402559)
- Luiz Filipe de Almeida Tannus (RA: 10418230)

Neste documento construímos um diagrama de implantação utilizando o PlantUML, mapeando a distribuição física dos nós e artefatos de software.

## Diagrama

```plantuml
@startuml
title Diagrama de Implantação - Topologia Física

left to right direction

node "Navegador do Cliente" <<Dispositivo>> as Cliente {
    artifact "SPA (Single Page Application)" as SPA
}

node "Servidor BFF" <<Servidor - Datacenter>> as ServidorBFF {
    artifact "Serviço BFF" as BFF
}

node "Servidor de Aplicação" <<Servidor - Datacenter>> as ServidorApp {
    artifact "Controladores" as Controllers
    artifact "Componentes de Domínio" as Domain
    artifact "Componentes de Dados" as Data
    
    Controllers --> Domain : acesso local
    Domain --> Data : acesso local
}

node "Servidor de Banco de Dados" <<Servidor - Datacenter>> as ServidorDB {
    database "MariaDB" as DB
}

' Conexões de Rede
Cliente -- ServidorBFF : <<protocolo>>\nHTTPS
ServidorBFF -- ServidorApp : <<protocolo>>\nHTTP
ServidorApp -- ServidorDB : <<protocolo>>\nTCP/IP
@enduml