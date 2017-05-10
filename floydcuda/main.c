#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>


#define DIMENSION 10;
__global__ void gpu_Floyd(int *result, int N)
{
    int j,k;
    int Row = blockIdx.y * blockDim.y + threadIdx.y;

    for(k = 0; k < N; k++)
    {
        for(j = 0; j < N; j++)
        {
            int i0 = Row * N + j;
            int i1 = Row * N + k;
            int i2 = k * N + j;
            if(result[i0] != -1 && result[i2] != -1)
                    result[i0] = (result[i0] != -1 && result[i0] < result[i1] + result[i2]) ?
                      result[i0] : (result[i1] + result[i2]);
            __syncthreads();
        }
    }
}

   void GenMatrix(int *mat, const size_t N)
{
    for(int i = 0; i < N*N; i ++)
        mat[i] = rand()%32 - 1;

}

bool CmpArray(const int *l, const int *r, const size_t eleNum)
{
    for(int i = 0; i < eleNum; i ++)
        if(l[i] != r[i])
        {
            printf("ERROR: l[%d] = %d, r[%d] = %d\n", i, l[i], i, r[i]);
            return false;
        }
    return true;
}

int main(int argc, char **argv)
{

// generate a random matrix.
size_t N = 10;
int *mat = (int*)malloc(sizeof(int)*N*N);
GenMatrix(mat, N);

// compute the reference result.
int *ref = (int*)malloc(sizeof(int)*N*N);
memcpy(ref, mat, sizeof(int)*N*N);
Floyd_sequential(ref, N);


//CUDA Portion
int Grid_Dim_x = 1, Grid_Dim_y = 1;
int noThreads_x, noThreads_y;
int *result = (int*)malloc(sizeof(int)*N*N);
memcpy(result, mat, sizeof(int)*N*N);
int *d_result;

// compute your results

cudaMalloc((void **)&d_result, N*N);

cudaMemcpy(result, N * N, cudaMemcpyHostToDevice);
gpu_Floyd<<<1024, 256>>>(d_result, N);
cudaMemcpy(result, d_result, cudaMemcpyDeviceToHost);

// compare your result with reference result
if(CmpArray(result, ref, N*N))
    printf("The matrix matches.\n");
else
    printf("The matrix do not match.\n");

free(ref);
free(result);
cudaFree(d_result);
}
