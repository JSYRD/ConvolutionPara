all:
	gcc -Wall -o matrix_parallel matrix_parallel.c -fopenmp
	gcc -Wall -g matrix_serial.c -o matrix_serial
clean:
	rm ./matrix_parallel
	rm ./matrix_serial