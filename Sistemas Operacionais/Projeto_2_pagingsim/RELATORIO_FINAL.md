# Relatório do Projeto 2: Simulador de Memória Virtual

**Disciplina:** Sistemas Operacionais
**Professor:** Lucas Figueiredo
**Data:**

## Integrantes do Grupo

- Arthur Danta Gonzales Felix - 10419721
- Isabele Dal Pogetto Costa - 10721328
- Henrique Ramos Marinho - 10428408
- Gabriel Resende Menezes - 10419003

---

## 1. Instruções de Compilação e Execução

### 1.1 Compilação

Para compilar o projeto e necessario entrar dentro da pasta src, onde estão localizados os arquivos fonte em C.

**Comandos:**
Aqui a lista de todos os comandos necessarios para executar o projeto:

```bash
cd src/
```

```bash
gcc *.c -o simulador
```

O comando gcc *.c -o simulador compila todos os arquivos .c dentro da pasta e ira gerar um arquivo chamado simulador.

### 1.2 Execução

Após a compilação do projeto, o simulador pode ser executado informando:
1. o algoritmo de substituição de páginas (fifo ou clock);
2. o arquivo de configuração;
3. o arquivo contendo os acessos à memória.

**Exemplo com FIFO:**
```bash
./simulador fifo ../tests/config_1.txt ../tests/acessos_1.txt
```

**Exemplo com Clock:**
```bash
./simulador clock ../tests/config_1.txt ../tests/acessos_1.txt
```

---

## 2. Decisões de Design

### 2.1 Estruturas de Dados


**Tabela de Páginas:**

Para representar a tabela de páginas, foi utilizado um array de páginas dentro da estrutura de cada processo. Cada processo possui sua propria tabela, permitindo simular corretamente a memória virtual de múltiplos processos

Cada entrada da tabela de páginas armazena informações como:

- presente 
- frame 
- referencia

O campo presente indica se a página está carregada na memória física. O campo frame indica em qual frame físico a página está armazenada. Já o campo referencia é utilizado pelo algoritmo Clock para controlar o bit de referência da página.

**Frames Físicos:**

Os frames da memória física foram representados por um array de frames dentro da estrutura principal do simulador.

Cada frame armazena informações como:

- ocupado
- pid
- pagina
- ordem_entrada
- referencia

O campo ocupado indica se o frame está livre ou em uso. O campo pid identifica qual processo é dono da página carregada naquele frame. O campo pagina armazena o número da página virtual carregada. O campo ordem_entrada é usado no FIFO para saber qual página entrou primeiro. Já o campo referencia é usado no Clock como bit de referência.

Essa abordagem foi escolhida porque a memória física possui uma quantidade fixa de frames. Portanto, um array facilita a busca por frames livres e também a varredura dos frames durante a substituição de páginas.

**Estrutura para FIFO:**

No algoritmo FIFO, a ordem de chegada das páginas foi mantida utilizando o campo:

- ordem_entrada

Sempre que uma nova página é carregada em um frame da memória física, esse campo recebe um valor crescente, representando a ordem em que a página entrou na memória.

Para identificar a página mais antiga, o simulador percorre todos os frames ocupados e seleciona aquele que possui o menor valor de ordem_entrada. Esse frame é então escolhido como vítima para substituição.

Essa abordagem foi escolhida por ser simples de implementar e eficiente para o contexto do simulador. Como os frames já são armazenados em um array, não foi necessário utilizar estruturas adicionais como filas encadeadas. Além disso, o controle por ordem numérica facilita a visualização e o entendimento do funcionamento do algoritmo FIFO.

**Estrutura para Clock:**

No algoritmo Clock, foi utilizado um ponteiro circular chamado `ponteiro_clock`. Esse ponteiro indica qual frame será analisado durante a busca por uma página vítima.

Os frames são tratados como uma estrutura circular. Quando o ponteiro chega ao último frame, ele retorna para o primeiro usando o operador módulo:

```
ponteiro_clock = (ponteiro_clock + 1) % num_frames;
```

Os R-bits foram representados pelo campo referencia. Esse campo existe nos frames físicos e também na tabela de páginas. Quando uma página é acessada, seu bit de referência é atualizado para 1, indicando que ela foi usada recentemente.

Durante a substituição, o algoritmo verifica o frame apontado por ponteiro_clock. Se o R-bit for 1, ele é zerado e a página recebe uma segunda chance. O ponteiro então avança para o próximo frame. Se o R-bit for 0, o frame é escolhido como vítima para substituição.

Essa abordagem foi escolhida porque simula diretamente o funcionamento clássico do algoritmo Clock, que funciona como uma fila circular com segunda chance. Além disso, o uso de um ponteiro e de bits de referência torna a implementação simples, eficiente e adequada para percorrer os frames físicos sem precisar reorganizar a memória.

### 2.2 Organização do Código

Descreva como organizou seu código:

O código foi organizado em módulos separados, dividindo as responsabilidades do simulador. Ao todo, foram utilizados arquivos para o programa principal, gerenciamento da memória e implementação dos algoritmos de substituição.

**Estrutura:**
```
src/
├── main.c
│   └── main() - lê os argumentos da linha de comando, valida o algoritmo escolhido e coordena a execução geral do simulador.
│
├── memoria.c
│   ├── inicializar_simulador() - inicializa os campos principais da estrutura do simulador.
│   ├── carregar_configuracao() - lê o arquivo de configuração e monta os processos, páginas e frames.
│   ├── executar_acessos() - processa o arquivo de acessos e executa a simulação.
│   ├── buscar_processo() - localiza um processo pelo seu PID.
│   ├── tratar_page_fault() - trata a ausência de uma página na memória física.
│   └── liberar_simulador() - libera a memória alocada dinamicamente.
│
├── algoritmos.c
│   ├── escolher_vitima_fifo() - seleciona o frame vítima usando o algoritmo FIFO.
│   └── escolher_vitima_clock() - seleciona o frame vítima usando o algoritmo Clock.
│
├── memoria.h
│   └── declara as estruturas e funções relacionadas ao simulador e à memória.
│
└── algoritmos.h
    └── declara as funções dos algoritmos de substituição.
```
Essa separação foi escolhida para deixar o código mais organizado.

### 2.3 Algoritmo FIFO

A lógica do algoritmo FIFO foi implementada mantendo um controle da ordem em que cada página entrou na memória física. Sempre que uma nova página é carregada em um frame, o simulador registra um valor de ordem de entrada associado àquela página. Esse valor é incrementado a cada novo carregamento, permitindo identificar qual página chegou primeiro na memória.

Quando ocorre um page fault e não existem frames livres disponíveis, o simulador escolhe uma página vítima para remoção. Para isso, ele percorre todos os frames ocupados e compara os valores de ordem de entrada. O frame que possui o menor valor é considerado o mais antigo e, portanto, é selecionado para substituição.

### 2.4 Algoritmo Clock

A implementação do algoritmo Clock foi baseada na ideia de uma fila circular de frames físicos. Para isso, foi utilizado um ponteiro circular responsável por indicar qual frame será analisado durante a busca por uma página vítima.

O ponteiro avança sequencialmente pelos frames da memória física. Quando chega ao último frame, ele retorna para o primeiro, simulando o comportamento circular do algoritmo. Dessa forma, o simulador percorre continuamente os frames até encontrar uma página que possa ser substituída.

A lógica da “segunda chance” foi implementada utilizando o bit de referência (R-bit). Sempre que uma página é acessada, seu R-bit é marcado como 1, indicando que ela foi utilizada recentemente. Durante a substituição, o algoritmo verifica o frame apontado pelo ponteiro:

- Se o R-bit for 0, a página é escolhida como vítima.
- Se o R-bit for 1, a página recebe uma segunda chance. Nesse caso, o bit é zerado e o ponteiro avança para o próximo frame.

No caso em que todas as páginas possuem R=1, o algoritmo realiza uma volta completa pela memória zerando todos os bits de referência. Após essa primeira passagem, os frames passam a ter R=0, permitindo que uma vítima seja encontrada na próxima análise. Isso garante que o algoritmo sempre consiga selecionar uma página para substituição.

O R-bit é atualizado em todos os acessos à memória

### 2.5 Tratamento de Page Fault

O tratamento de page fault foi implementado verificando inicialmente se a página acessada já está carregada na memória física. Caso a página não esteja presente, ocorre um page fault e o simulador precisa carregar a página na memória.

## Cenário 1: Frame livre disponível

Para identificar se existe um frame livre, o simulador percorre os frames da memória física verificando o campo que indica se o frame está ocupado ou não.

## Cenário 2: Memória cheia (substituição)

A memória é considerada cheia quando o simulador percorre todos os frames e não encontra nenhum frame livre.

Quando isso acontece, o simulador precisa selecionar uma página vítima para substituição. O algoritmo utilizado é definido pelo parâmetro informado na execução do programa (fifo ou clock).

- Se o algoritmo escolhido for FIFO, o simulador chama a função responsável por selecionar a página mais antiga na memória.
- Se o algoritmo escolhido for Clock, o simulador utiliza o ponteiro circular e os bits de referência para encontrar a vítima.

---

## 3. Análise Comparativa FIFO vs Clock

### 3.1 Resultados dos Testes

Preencha a tabela abaixo com os resultados de pelo menos 3 testes diferentes:

| Descrição do Teste | Total de Acessos | Page Faults FIFO | Page Faults Clock | Diferença |
|-------------------|------------------|------------------|-------------------|-----------|
| Teste 1 - Básico  |         6         |          4      |           4        |      0     |
| Teste 2 - Memória Pequena |       8   |           8       |           8        |      0     |
| Teste 3 - Simples |              4    |           3       |          3         |     0      |
| Teste Próprio 1   |            12      |            12      |        12           |       0    |

### 3.2 Análise

1. **Qual algoritmo teve melhor desempenho (menos page faults)?**

Nos testes realizados, os algoritmos FIFO e Clock apresentaram o mesmo número de page faults em todos os cenários analisados.

2. **Por que você acha que isso aconteceu?** Considere:

Isso ocorreu principalmente por causa do padrão de acesso utilizado nos testes. Em muitos casos, os acessos realizavam poucas reutilizações de páginas ainda presentes na memória física. Dessa forma, tanto o FIFO quanto o Clock acabavam escolhendo vítimas semelhantes.

O algoritmo FIFO sempre remove a página mais antiga carregada na memória, sem considerar se ela foi utilizada recentemente. Já o Clock utiliza o R-bit para dar uma segunda chance a páginas acessadas recentemente.

 Entretanto, nos testes realizados, várias páginas diferentes eram acessadas continuamente, especialmente nos cenários de memória pequena. Assim, mesmo o Clock não conseguia aproveitar muito a vantagem do R-bit, pois as páginas acabavam sendo substituídas antes de serem reutilizadas.

3. **Em que situações Clock é melhor que FIFO?**

O algoritmo Clock tende a apresentar melhor desempenho em situações onde existem acessos repetidos às mesmas páginas.

Por exemplo, em aplicações onde algumas páginas são utilizadas constantemente, o Clock consegue identificar essas páginas através do R-bit e evita removê-las imediatamente da memória. Isso reduz a quantidade de page faults.

Exemplo: 

```
0, 1, 2, 0, 1, 3, 0, 1
```

4. **Houve casos onde FIFO e Clock tiveram o mesmo resultado?**
   Sim. Em todos os testes realizados acima, FIFO e Clock apresentaram o mesmo número de page faults.

Isso aconteceu porque os padrões de acesso utilizados não favoreceram suficientemente o mecanismo de segunda chance do Clock. Em vários cenários, praticamente todos os acessos geravam page faults devido à baixa quantidade de frames disponíveis e à grande quantidade de páginas diferentes acessadas.

5. **Qual algoritmo você escolheria para um sistema real e por quê?**

Para um sistema real, o algoritmo Clock seria a melhor escolha.
Clock possui uma estratégia mais inteligente de substituição, pois considera o uso recente das páginas através do R-bit.
---

## 4. Desafios e Aprendizados

### 4.1 Maior Desafio Técnico

O maior desafio técnico durante a implementação foi garantir o funcionamento correto do algoritmo Clock, principalmente no controle do ponteiro circular e dos bits de referência (R-bits).

Inicialmente, o simulador apresentava comportamentos incorretos durante a substituição de páginas, como selecionar páginas erradas para remoção ou entrar em loops durante a busca por uma vítima. O problema foi identificado através da comparação das saídas geradas pelo programa com os arquivos de saída esperados fornecidos nos testes.

Após analisar a lógica do algoritmo, percebemos que o principal problema estava no gerenciamento do ponteiro circular e na atualização correta dos bits de referência. Em alguns casos, o ponteiro não avançava corretamente após encontrar uma vítima, e em outros os R-bits não estavam sendo sincronizados corretamente entre os frames físicos e a tabela de páginas.

A solução foi reorganizar a lógica do algoritmo Clock para garantir que:

- o ponteiro sempre avançasse de forma circular;
- páginas com R=1 recebessem uma segunda chance;
- os R-bits fossem zerados corretamente durante a varredura;
- os bits fossem atualizados em todos os acessos à memória.

### 4.2 Principal Aprendizado

O principal aprendizado obtido com este projeto foi compreender de forma prática como funciona o gerenciamento de memória virtual e a substituição de páginas em sistemas operacionais.

---

## 5. Vídeo de Demonstração

**Link do vídeo:** [Insira aqui o link para YouTube, Google Drive, etc.]

### Conteúdo do vídeo:

Confirme que o vídeo contém:

- [ ] Demonstração da compilação do projeto
- [ ] Execução do simulador com algoritmo FIFO
- [ ] Execução do simulador com algoritmo Clock
- [ ] Explicação da saída produzida
- [ ] Comparação dos resultados FIFO vs Clock
- [ ] Breve explicação de uma decisão de design importante

---

## Checklist de Entrega

Antes de submeter, verifique:

- [X] Código compila sem erros conforme instruções da seção 1.1
- [X] Simulador funciona corretamente com FIFO
- [X] Simulador funciona corretamente com Clock
- [X] Formato de saída segue EXATAMENTE a especificação do ENUNCIADO.md
- [X] Testamos com os casos fornecidos em tests/
- [X] Todas as seções deste relatório foram preenchidas
- [X] Análise comparativa foi realizada com dados reais
- [ ] Vídeo de demonstração foi gravado e link está funcionando
- [X] Todos os integrantes participaram e concordam com a submissão

---
## Referências

https://en.cppreference.com/c

https://cplusplus.com/reference/cstring/strcmp/

---
