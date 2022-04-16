CC=gcc
FLAGS=-O3 -Wall
N?=32

cseq: n_queens.c lib/board.o implementations/seq.o
	$(CC) n_queens.c lib/board.o implementations/seq.o $(FLAGS) -o n_queens

comp: n_queens.c lib/board.o implementations/omp.o
	$(CC) n_queens.c lib/board.o implementations/omp.o $(FLAGS) -fopenmp -o n_queens

lib/board.o: lib/board.c lib/board.h
	$(CC) lib/board.c -c -o lib/board.o

implementations/seq.o: implementations/seq.c implementations/implementation.h
	$(CC) implementations/seq.c -c -o implementations/seq.o

implementations/omp.o: implementations/omp.c implementations/implementation.h
	$(CC) implementations/omp.c -c -fopenmp -o implementations/omp.o

dseq: cseq
	valgrind --tool=memcheck ./n_queens

seq: execseq
	perf report > ./report/seq.txt

omp: execomp
	perf report > ./report/omp.txt

execseq: cseq
	perf record ./n_queens $(N)

execomp: comp
	perf record ./n_queens $(N)

clean:
	rm -rf\
		lib/board.o\
		implementations/omp.o implementations/seq.o\
		n_queens\
		perf.data perf.data.old\
		./scorep*/
