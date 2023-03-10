/**********************************
* Programmer: Dimitrios Gkoulis   *
* Email: gkould@gmail.com         *
* Harokopio University of Athens  *
* Parallel Computing & Algorithms *
* Project - Winter Semester       *
* Period: 2015 - 2016             *
* IDE: Dec-C++ (4.9.0.2)          *
***********************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string>
#include <iostream>
#include "defs.h"
using namespace std;

//#define I_SIZE 4  // Input Size y (length) = x (length)
#define H_SIZE 3     // Mask  Size y (length) = x (length)
#define SAVETOFILE 1 // Save to file flag


#define OMP_NUM_THREADS 16


/* F U N C T I O N S    D E C L A R A T I O N S */
int convolution(int I[I_SIZE][I_SIZE], int h[H_SIZE][H_SIZE], int y, int x);    // CALCULATE CONVOLUTION (FUNCTION)
void save_to_file(int A[I_SIZE][I_SIZE]);                                       // SAVE TO FILE          (FUNCTION)   

int I[I_SIZE][I_SIZE];
void readfile(){
	FILE* file;
	file = fopen("matrix.txt", "r");
	if (file == NULL){
		cout << "文件打开失败" << endl;
	}
	else{
		for (int i = 0; i < I_SIZE; ++i){
			for (int j = 0; j < I_SIZE; ++j){
				fscanf(file, "%d", &I[i][j]);

			}
		}
		// cout << "ok" << endl;
	}
	fclose(file);

}
/* M A I N */
int main(int argc, char *argv[])
{
	// Input
	readfile();
	double start, end;

	// Mask/Kernel
	int h[H_SIZE][H_SIZE] = {
		{ 0, 1, 0 },
		{ 1, -4, 1 },
		{ 0, 1, 0 }
	};

	// Convolution in each Input point
	int A[I_SIZE][I_SIZE];

	int y, x; // Loop Counters


	int rank, size;           // MPI Declarations
	const int tag = 1;        // MPI Declarations
	MPI_Status status;        // MPI Declarations
	int proc_start, proc_end; // MPI Declarations (start point and end point for each proc)


	// MPI Initialazion
	MPI_Init(&argc, &argv);
	
	// Get current Process ID
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	// Get number of Processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//calculate time
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	start = MPI_Wtime();
	// Divide Loop (each process has start & end point)
	proc_start = (I_SIZE / size) * rank;
	if (I_SIZE % size > rank) {
		proc_start += rank;
		proc_end = proc_start + (I_SIZE / size) + 1;
	}
	else {
		proc_start += I_SIZE % size;
		proc_end = proc_start + (I_SIZE / size);
	}


	// I am a process. This is my job!
	// printf("CPU: %d of %d | Start Point: %d ~ End Point %d \n\n", rank, size, proc_start, proc_end);


	// All nodes except the first one should block on receive.
	if (rank > 0)
		MPI_Recv(&A[0][0], I_SIZE * I_SIZE, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &status);


	// Sequential Way
	// for(y=0; y<I_SIZE; y++) {
	// MPI: - Parallel Way... My way!
	for (int batch = 0; batch < BATCHSIZE; batch++){//here do the same thing for batchsize times
		for (y = proc_start; y<proc_end; y++) {
			for (x = 0; x<I_SIZE; x++) {
				A[y][x] = convolution(I, h, y, x);               // Get convolution value in point y, x
				//printf("A[%d][%d]: %d \n\n", y, x, A[y][x] ); // Print convolution value in each point (y,x)
			}
		}
	}
	


	// The last rank shouldn't send
	if (rank < size - 1)//将整个A全部发送是没有必要的，这里可以进行优化，这就是第一个优化的版本
		MPI_Send(&A[0][0], I_SIZE * I_SIZE, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);//这里将整个矩阵全部发送过去


	
	// The last rank should call 'save_to_file' function

	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();
	

	if (rank == size - 1) { /* use time on master node */
		if (SAVETOFILE) {
			save_to_file(A);
		}
		printf("%f\n", end - start);
		
		
	}

	// MPI Finalize
	MPI_Finalize();


	// system("PAUSE");	
	return 0;

} // End main


/* C O N V O L U T I O N */
int convolution(int I[I_SIZE][I_SIZE], int h[H_SIZE][H_SIZE], int y, int x)//含有padding,输入矩阵和输出矩阵大小相等
{
	int k, j;           // Loop Counters
	int i1, j1, i2, j2; // Helpers (point)
	int res1, res2;     // Helpers
	int result = 0;     // Convolution value in point y, x

	for (k = -1; k<H_SIZE-1; k++) {//更加普适

		for (j = -1; j<H_SIZE -1; j++) {

			i1 = j + 1;
			j1 = k + 1;
			i2 = y - j;
			j2 = x - k;

			if (((i1 >= 0) && (i1 < H_SIZE)) && ((j1 >= 0) && (j1 < H_SIZE))) {
				res1 = h[i1][j1];
			}
			else {
				res1 = 0;
			}

			if (((i2 >= 0) && (i2 < I_SIZE)) && ((j2 >= 0) && (j2 < I_SIZE))) {
				res2 = I[i2][j2];
			}
			else {
				res2 = 0;
			}

			// A += h[j+1][k+1]*I[y-j][x-k];
			result += res1*res2;

		}

	}
	
	return result;

	
}


/* S A V E    L I S T    T O    F I L E */
void save_to_file(int A[I_SIZE][I_SIZE])
{
	FILE *file_conv_res;
	int y, x;

	file_conv_res = fopen("convolution_z2.txt", "w");
	if (file_conv_res == NULL){
		printf("Error: ['Save to file' Failed!]");
		exit(1);
	}

	for (y = 0; y<I_SIZE; y++) {
		for (x = 0; x<I_SIZE; x++) {
			fprintf(file_conv_res, "A[%d][%d] = %d \n", y, x, A[y][x]);
		}
	}
	fclose(file_conv_res);
	return;

} //End of saving to file.