# Arquitetura - Diagrama de Pacotes

**Equipe:**
- Gabriel Rezende Menezes (RA: 10419003)
- Heitor Maciel de Vasconcellos Leite (RA: 10402559)
- Luiz Filipe de Almeida Tannus (RA: 10418230)

Neste documento construímos um diagrama de pacotes utilizando o PlantUML descrevendo o agrupamento de componentes da arquitetura da nossa solução.

## Diagrama

```plantuml
@startuml
title Diagrama de Pacotes - Arquitetura de Software em Camadas

skinparam packageStyle rectangle

package "Camada de Apresentação" {
    [Interface de Usuário (SPA)]
}

package "Camada de API Gateway / Integração" {
    [Back For Front (BFF)]
}

package "Camada de Aplicação" {
    [Controladores de Casos de Uso]
}

package "Camada de Domínio e Negócios" {
    [Serviços e Entidades de Domínio]
}

package "Camada de Infraestrutura / Dados" {
    [Componentes de Acesso a Dados]
}

database "Armazenamento" {
    [Banco de Dados Relacional]
}

[Interface de Usuário (SPA)] ..> [Back For Front (BFF)] : consome
[Back For Front (BFF)] ..> [Controladores de Casos de Uso] : roteia requisições
[Controladores de Casos de Uso] ..> [Serviços e Entidades de Domínio] : orquestra
[Serviços e Entidades de Domínio] ..> [Componentes de Acesso a Dados] : utiliza
[Componentes de Acesso a Dados] ..> [Banco de Dados Relacional] : persiste/consulta

@enduml