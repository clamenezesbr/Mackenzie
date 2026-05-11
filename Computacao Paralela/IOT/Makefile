CC = gcc
CFLAGS = -Wall -O3
LIBS = -lm -lpthread

TARGETS = sensor_analyzer_seq sensor_analyzer_par sensor_analyzer_optimized

all: $(TARGETS)

sensor_analyzer_seq: sensor_analyzer_seq.c
	$(CC) $(CFLAGS) sensor_analyzer_seq.c -o sensor_analyzer_seq $(LIBS)

sensor_analyzer_par: sensor_analyzer_par.c
	$(CC) $(CFLAGS) sensor_analyzer_par.c -o sensor_analyzer_par $(LIBS)

sensor_analyzer_optimized: sensor_analyzer_optimized.c
	$(CC) $(CFLAGS) sensor_analyzer_optimized.c -o sensor_analyzer_optimized $(LIBS)

clean:
	rm -f $(TARGETS)

# Variáveis de compilação

# Nomes dos executáveis que o projeto pede


# O comando 'all' compila todos os alvos listados em TARGETS

# Regra para a Versão Sequencial


# Regra para a Versão Paralela 


# Regra para a Versão Otimizada 


# Comando para limpar os arquivos gerados e deixar a pasta organizada

