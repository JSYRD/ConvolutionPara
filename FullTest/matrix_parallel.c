#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include "matrix.h"
#include "matrix_static.h"

int matrix[N][N];
int core[M][M] = {
    {1,
     0,
     -1},
    {1,
     0,
     -1},
    {1, 0, -1}};

void printMatrix(int n, int **m)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}
int conv(int x, int y)
{
    int sum = 0;
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            sum += matrix[y + i][x + j] * core[j][i];
        }
    }
    return sum;
}
int main(int argc, char **argv)
{
    int thread_count = strtol(argv[1], NULL, 10);
    int dim = N - M + 1;
    // int x;
    int **outputMatrix = (int **)malloc(sizeof(int *) * dim);
    for (int i = 0; i < dim; ++i)
    {
        outputMatrix[i] = (int *)malloc(sizeof(int) * dim);
        memset(outputMatrix[i], 0, sizeof(int) * dim);
    } // init 
    time_t t;
    srand((unsigned)time(&t));
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix[i][j] = rand() % 255;
        }
    }
    // printMatrix(N, matrix);
    // convolution
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int batch_size = 0; batch_size < BATCH_SIZE; ++batch_size)
    {
        for (int matrix_num = 0; matrix_num < MATRIX_NUM; ++matrix_num)
        {
#pragma omp parallel for num_threads(thread_count)
            for (int core_y = 0; core_y <= N - M; core_y++)
            {
                for (int core_x = 0; core_x <= N - M; ++core_x)
                {
                    for (int core_num = 0; core_num < CORE_NUM; ++core_num)
                    {
                        outputMatrix[core_y][core_x] = conv(core_x, core_y);
                        
                    }
                    
                }
            }
        }
    }
    gettimeofday(&end, NULL);
    printf("%lf\n", (end.tv_sec*1000000 + end.tv_usec)/1000000.0 - (start.tv_sec*1000000 + start.tv_usec)/1000000.0);
    // printMatrix(dim, outputMatrix);
    return 0;
}