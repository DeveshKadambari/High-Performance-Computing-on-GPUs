/*I have taken the help of lzhengchun/matrix-cuda github repository to get this code working.*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 16

__global__ void g_mat_mul(int *a,int *b, int *c, int m, int n, int k)
{ 
    int r = blockIdx.y * blockDim.y + threadIdx.y; 
    int cl = blockIdx.x * blockDim.x + threadIdx.x;
    int temp = 0;
    if( cl < k && r < m) 
    {
        for(int i = 0; i < n; i++) 
        {
            temp += a[r * n + i] * b[i * k + cl];
        }
        c[r * k + cl] = temp;
    }
} 

int main(int argc, char const *argv[])
{
    int m_rows, n_col_row, k_col;
   
    printf("please type in A=mxn B=nxk \n");
    scanf("%d %d %d", &m_rows, &n_col_row, &k_col);
    int *h_a_cpu, *h_b_cpu, *h_c_cpu, *h_cc_cpu;
    cudaMallocHost((void **) &h_a_cpu, sizeof(int)*m_rows*n_col_row);
    cudaMallocHost((void **) &h_b_cpu, sizeof(int)*n_col_row*k_col);
    cudaMallocHost((void **) &h_c_cpu, sizeof(int)*m_rows*k_col);
    cudaMallocHost((void **) &h_cc_cpu, sizeof(int)*m_rows*k_col);

 for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < n_col_row; ++j) {
            h_a_cpu[i * n_col_row + j] = rand() % 1024;
        }
}

for (int i = 0; i < n_col_row; ++i) {
        for (int j = 0; j < k_col; ++j) {
            h_b_cpu[i * k_col + j] = rand() % 1024;
        }
}

float gpu_elapsed_time_ms, cpu_elapsed_time_ms;

cudaEvent_t start, stop;

cudaEventCreate(&start);
cudaEventCreate(&stop);

cudaEventRecord(start, 0);

 int *d_a_gpu, *d_b_gpu, *d_c_gpu;
    cudaMalloc((void **) &d_a_gpu, sizeof(int)*m_rows*n_col_row);
    cudaMalloc((void **) &d_b_gpu, sizeof(int)*n_col_row*k_col);
    cudaMalloc((void **) &d_c_gpu, sizeof(int)*m_rows*k_col);
    cudaMemcpy(d_a_gpu, h_a_cpu, sizeof(int)*m_rows*n_col_row, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b_gpu, h_b_cpu, sizeof(int)*n_col_row*k_col, cudaMemcpyHostToDevice);

    unsigned int grid_rows = (m_rows + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned int grid_cols = (k_col + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(grid_cols, grid_rows);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);


 g_mat_mul<<<dimGrid, dimBlock>>>(d_a_gpu, d_b_gpu, d_c_gpu, m_rows, n_col_row, k_col);

cudaMemcpy(h_a_cpu, d_c_gpu, sizeof(int)*m_rows*k_col, cudaMemcpyDeviceToHost);

cudaThreadSynchronize();

cudaEventRecord(stop, 0);
cudaEventSynchronize(stop);

cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);
printf("Time elapsed on matrix multiplication of %dx%d . %dx%d on GPU: %f ms.\n\n", m_rows, n_col_row, n_col_row, k_col, gpu_elapsed_time_ms);

cudaFree(d_a_gpu);
    cudaFree(d_b_gpu);
    cudaFree(d_c_gpu);
    cudaFreeHost(h_a_cpu);
    cudaFreeHost(h_b_cpu);
    cudaFreeHost(h_c_cpu);
    cudaFreeHost(h_cc_cpu);
    return 0;
}
