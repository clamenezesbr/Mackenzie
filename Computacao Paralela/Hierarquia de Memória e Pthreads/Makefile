CC      = gcc
CFLAGS0 = -O0 -Wall
CFLAGS3 = -O3 -Wall
LIBS    = -lpthread

# Lista de todos os alvos
all: \
	matmul_row_O0   matmul_row_O3   \
	matmul_col_O0   matmul_col_O3   \
	matmul_mult_row_O0  matmul_mult_row_O3  \
	matmul_mult_block_O0 matmul_mult_block_O3 \
	matmul_pthreads_O0  matmul_pthreads_O3  \
	matmul_pthreads_block_O0 matmul_pthreads_block_O3

# ---------- Item 2: contagem de pares ----------
matmul_row_O0: matmul_row.c
	$(CC) $(CFLAGS0) -o $@ $<

matmul_row_O3: matmul_row.c
	$(CC) $(CFLAGS3) -o $@ $<

matmul_col_O0: matmul_col.c
	$(CC) $(CFLAGS0) -o $@ $<

matmul_col_O3: matmul_col.c
	$(CC) $(CFLAGS3) -o $@ $<

# ---------- Item 3: multiplicacao ----------
matmul_mult_row_O0: matmul_mult_row.c
	$(CC) $(CFLAGS0) -o $@ $<

matmul_mult_row_O3: matmul_mult_row.c
	$(CC) $(CFLAGS3) -o $@ $<

matmul_mult_block_O0: matmul_mult_block.c
	$(CC) $(CFLAGS0) -o $@ $<

matmul_mult_block_O3: matmul_mult_block.c
	$(CC) $(CFLAGS3) -o $@ $<

# ---------- Item 4: pthreads ----------
matmul_pthreads_O0: matmul_pthreads.c
	$(CC) $(CFLAGS0) -o $@ $< $(LIBS)

matmul_pthreads_O3: matmul_pthreads.c
	$(CC) $(CFLAGS3) -o $@ $< $(LIBS)

matmul_pthreads_block_O0: matmul_pthreads_block.c
	$(CC) $(CFLAGS0) -o $@ $< $(LIBS)

matmul_pthreads_block_O3: matmul_pthreads_block.c
	$(CC) $(CFLAGS3) -o $@ $< $(LIBS)

# ---------- Valgrind (cachegrind) nos binarios O0 ----------
valgrind:
	valgrind --tool=cachegrind ./matmul_mult_row_O0
	valgrind --tool=cachegrind ./matmul_mult_block_O0
	valgrind --tool=cachegrind ./matmul_pthreads_O0
	valgrind --tool=cachegrind ./matmul_pthreads_block_O0

clean:
	rm -f matmul_row_O0 matmul_row_O3 \
	      matmul_col_O0 matmul_col_O3 \
	      matmul_mult_row_O0 matmul_mult_row_O3 \
	      matmul_mult_block_O0 matmul_mult_block_O3 \
	      matmul_pthreads_O0 matmul_pthreads_O3 \
	      matmul_pthreads_block_O0 matmul_pthreads_block_O3 \
	      cachegrind.out.*
