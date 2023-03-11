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

#define H_SIZE 3     // Mask  Size y (length) = x (length)
#define SAVETOFILE 1 // Save to file flag



/* F U N C T I O N S    D E C L A R A T I O N S */
int convolution(int I[I_SIZE][I_SIZE], int h[H_SIZE][H_SIZE], int y, int x);    // CALCULATE CONVOLUTION (FUNCTION)
void save_to_file(int A[I_SIZE][I_SIZE]);                                       // SAVE TO FILE          (FUNCTION)   
int I[I_SIZE][I_SIZE];
void readfile(){
	FILE* file;
	file = fopen("matrix200.txt", "r");
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
	readfile();//这里还可以做一下优化，每个进程都读取了一下matrix.txt

	double start, end;

	// Mask/Kernel
	int h[H_SIZE][H_SIZE] = {
		{ 0, 1, 0 },
		{ 1, -4, 1 },
		{ 0, 1, 0 }
	};

	// Convolution in each Input point
	int A[I_SIZE][I_SIZE];
	int temp[I_SIZE][I_SIZE];

	int y, x; // Loop Counters


	int rank, size;           // MPI Declarations
	const int tag = 1;        // MPI Declarations
	MPI_Status status;        // MPI Declarations
	//int proc_start, proc_end; // MPI Declarations (start point and end point for each proc)


	// MPI Initialazion
	MPI_Init(&argc, &argv);

	// Get current Process ID
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	// Get number of Processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Divide Loop (each process has start & end point)
	//calculate time
	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	start = MPI_Wtime();

	
	int segment = I_SIZE / size;//每个线程分配到的行数
	int last_proc = I_SIZE - segment*(size-1);//最后一个线程需要处理的行数



	// I am a process. This is my job!
//	printf("CPU: %d of %d | Start Point: %d ~ End Point %d \n\n", rank, size, proc_start, proc_end);


	// All nodes except the first one should block on receive.
	
	//MPI_Scatter(&A[0][0], segment*I_SIZE, MPI_INT, &temp[0][0], segment * I_SIZE, MPI_INT,0, MPI_COMM_WORLD);

	// Sequential Way
	
	for (int batch = 0; batch < BATCHSIZE; batch++){		
		for (y = 0 + rank*segment; y<segment + rank*segment; y++) {
			for (x = 0; x<I_SIZE; x++) {
				temp[y - rank*segment][x] = convolution(I, h, y, x);               // Get convolution value in point y, x
				//printf("A[%d][%d]: %d \n\n", y, x, A[y][x] ); // Print convolution value in each point (y,x)
			}
		}		
	}
	
	//由于使用了gather，所以只能测试那些线程数能够将I_SIZE整除的
	MPI_Gather(&temp[0][0], segment*I_SIZE, MPI_INT, &A[0][0], segment*I_SIZE, MPI_INT, 0, MPI_COMM_WORLD);


	MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
	end = MPI_Wtime();

	// The last rank should call 'save_to_file' function
	if (rank == 0) { /* use time on master node */
		printf("%f\n", end - start);
		if (SAVETOFILE) {
			save_to_file(A);
		}

	}

	// MPI Finalize
	MPI_Finalize();


	return 0;

} // End main


/* C O N V O L U T I O N */
int convolution(int I[I_SIZE][I_SIZE], int h[H_SIZE][H_SIZE], int y, int x)//含有padding,输入矩阵和输出矩阵大小相等
{
	int k, j;           // Loop Counters
	int i1, j1, i2, j2; // Helpers (point)
	int res1, res2;     // Helpers
	int result = 0;     // Convolution value in point y, x

	for (k = -1; k<H_SIZE - 1; k++) {//更加普适

		for (j = -1; j<H_SIZE - 1; j++) {

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

	file_conv_res = fopen("convolution_z2_opt1200.txt", "w");
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