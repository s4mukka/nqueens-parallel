CC=gcc
FLAGS=
N?=32

cseq: n_queens.c lib/board.o implementations/seq.o
	$(CC) n_queens.c lib/board.o implementations/seq.o $(FLAGS) -o n_queens

comp: n_queens.c lib/board.o implementations/omp.o
	$(CC) n_queens.c lib/board.o implementations/omp.o $(FLAGS) -fopenmp -o n_queens

ccuda:
	nvcc -o n_queens n_queens.cu $(FLAGS)

boardc: lib/board.c lib/board.h
	nvcc lib/board.cu -c -o lib/board.o

lib/board.o: lib/board.c lib/board.h
	$(CC) lib/board.c -c -o lib/board.o

implementations/seq.o: implementations/seq.c implementations/implementation.h
	$(CC) implementations/seq.c -c -o implementations/seq.o

implementations/omp.o: implementations/omp.c implementations/implementation.h
	$(CC) implementations/omp.c -c -fopenmp -o implementations/omp.o

implementations/cuda.o: implementations/cuda.cu implementations/implementation.h
	nvcc implementations/cuda.cu -c -o implementations/cuda.o

dseq: cseq
	valgrind --tool=memcheck ./n_queens

seq: execseq
	perf report > ./report/seq.txt

omp: execomp
	perf report > ./report/omp.txt

cuda: execcuda
	perf report > ./report/cuda.txt

execseq: cseq
	perf record ./n_queens $(N)

execomp: comp
	perf record ./n_queens $(N)

execcuda: ccuda
	perf record ./n_queens $(N)

clean:
	rm -rf lib/board.o\
		implementations/omp.o implementations/seq.o\
		n_queens\
		perf.data perf.data.old\
		./scorep*/
