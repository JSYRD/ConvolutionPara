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
#include <sys/time.h>
#include <iostream>
#include <time.h>
#include "defs.h"

using namespace std;

//#define I_SIZE 4  // Input Size y (length) = x (length)
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
	// Input 将其改成read file
	readfile();

	// Mask/Kernel
	int h[H_SIZE][H_SIZE] = {
		{ 0, 1, 0 },
		{ 1, -4, 1 },
		{ 0, 1, 0 }
	};
	struct timeval tv_begin, tv_end;//calculate the time 

	//clock_t start = clock();    // the code need to measure time    
	gettimeofday(&tv_begin, NULL);

	// Convolution in each Input point
	int A[I_SIZE][I_SIZE];

	int y, x; // Loop Counters
	for (int batch = 0; batch < BATCHSIZE; batch++){
		for (y = 0; y<I_SIZE; y++) {
			for (x = 0; x<I_SIZE; x++) {
				A[y][x] = convolution(I, h, y, x);               // Get convolution value in point y, x
				// printf("A[%d][%d]: %d \n\n", y, x, A[y][x] ); // Print convolution value in each point (y,x)
			}
		}
	}
	

	gettimeofday(&tv_end, NULL);
	double time_use = (tv_end.tv_sec - tv_begin.tv_sec) * 1000000 + (tv_end.tv_usec - tv_begin.tv_usec);

	//clock_t end = clock();
	//double timeSpend = (double)(end - start); // / CLOCKS_PER_SEC; //得到的结果是秒，(end-start)得到的结果是毫秒
	printf("%f\n", time_use/1000000);
	if (SAVETOFILE)
		save_to_file(A);

	return 0;

} 
/* C O N V O L U T I O N */
int convolution(int I[I_SIZE][I_SIZE], int h[H_SIZE][H_SIZE], int y, int x)
{
	int k, j;           // Loop Counters
	int i1, j1, i2, j2; // Helpers (point)
	int res1, res2;     // Helpers
	int result = 0;     // Convolution value in point y, x

	for (k = -1; k<H_SIZE - 1; k++) {

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

	file_conv_res = fopen("convolution200.txt", "w");
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