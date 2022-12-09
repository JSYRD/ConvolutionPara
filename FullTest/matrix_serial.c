#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

void
printMatrix(size_t n, int **m)
{
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

int
conv(size_t x, size_t y)
{
    int sum = 0;
    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            sum += matrix[y + i][x + j] * core[j][i];
        }
    }
    return sum;
}

int
main(int argc, char** argv)
{
    int dim = N - M + 1;
    int **outputMatrix = (int **)malloc(sizeof(int *) * dim);
    for (size_t i = 0; i < dim; ++i)
    {
        outputMatrix[i] = (int *)malloc(sizeof(int) * dim);
        memset(outputMatrix[i], 0, sizeof(int) * dim);
    } // init outputMatrix
    time_t t;
    srand((unsigned)time(&t));
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            matrix[i][j] = rand() % 255;
        }
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (size_t batch_size = 0; batch_size < BATCH_SIZE; ++batch_size)
    {
        for (int matrix_num = 0; matrix_num < MATRIX_NUM; ++matrix_num)
        {
            for (size_t core_y = 0; core_y <= N - M; ++core_y)
            {
                for (size_t core_x = 0; core_x <= N - M; ++core_x)
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
    return 0;
}
